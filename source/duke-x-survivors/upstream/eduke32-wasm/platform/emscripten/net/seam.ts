// ─────────────────────────────────────────────────────────────────────────────
// SEAM — the runtime half of the net_transport.h bridge.
//
// The C engine's transport functions live in net/seam_library.js (an Emscripten
// --js-library linked INTO the wasm). Those tiny shims forward to the methods this
// class exposes on window.DukeNet:
//
//   C net_send(peerToken, channel, reliable, data, len)  -> Seam.send(...)
//   C net_broadcast(channel, reliable, data, len)        -> Seam.broadcast(...)
//   C net_poll()                                         -> Seam.drain()  (JS->C:
//                                                            Net_ReceiveFrame /
//                                                            Net_PeerEvent per item)
//   C net_transport_init()      -> Seam.init()
//   C net_transport_shutdown()  -> Seam.shutdown()
//
// peerToken == the Duke connectindex. The host assigns tokens during the join
// handshake (duke-net.ts); this class holds the token<->deviceId map so outbound
// sends resolve a token to a peer and inbound frames resolve a peer to its token.
//
// Ordering guarantee: peer-up/-down events and frames are delivered to the netcode
// in the exact order they were enqueued (one FIFO), so a NET_PEER_UP always
// precedes that peer's first frame and a NET_PEER_DOWN follows its last.
// ─────────────────────────────────────────────────────────────────────────────

/** net_channel_t (mirrors the frozen header). */
export const NET_CHAN_MOVE = 0;
export const NET_CHAN_REL = 1;
export const NET_CHAN_BULK = 2;

/** net_peerevent_t. */
export const NET_PEER_DOWN = 0;
export const NET_PEER_UP = 1;
// DXV33: transport boundaries; do not confuse packet size with peer capacity.
// Engine MAXPLAYERS remains 16 until the native state model is expanded.
export const NET_V33_MAX_FRAME_BYTES = [16 * 1024, 256 * 1024, 256 * 1024] as const;
export const NET_V33_MAX_QUEUE_ITEMS = 4096;
export const NET_V33_MAX_QUEUED_BYTES = 12 * 1024 * 1024;

/** An item drained to net_poll. kind 0 = frame, kind 1 = peer event. */
export type SeamItem =
  | { kind: 0; peer: number; channel: number; data: Uint8Array }
  | { kind: 1; peer: number; event: number };

/** The actual wire send, injected by duke-net (routes to the right peer + channel).
 *  Returns false if the peer/channel is not deliverable. */
export type SeamSender = (deviceId: string, channel: number, bytes: Uint8Array) => boolean;

export class Seam {
  private inbound: SeamItem[] = [];
  private tokenToDevice = new Map<number, string>();
  private deviceToToken = new Map<string, number>();
  private sender: SeamSender | null = null;
  private _active = false;
  private queuedBytes = 0;
  private droppedFrames = 0;
  private oversizedFrames = 0;
  private rejectedChannels = 0;
  stats(): { queuedItems: number; queuedBytes: number; droppedFrames: number; oversizedFrames: number; rejectedChannels: number; peers: number } {
    return { queuedItems:this.inbound.length,queuedBytes:this.queuedBytes,droppedFrames:this.droppedFrames,
      oversizedFrames:this.oversizedFrames,rejectedChannels:this.rejectedChannels,peers:this.tokenToDevice.size };
  }
  private validFrame(channel:number,bytes:Uint8Array):boolean {
    if (!Number.isInteger(channel) || channel < 0 || channel > 2) { this.rejectedChannels++;return false; }
    if (!(bytes instanceof Uint8Array) || !bytes.byteLength || bytes.byteLength > NET_V33_MAX_FRAME_BYTES[channel]) {
      this.oversizedFrames++;return false;
    }
    return true;
  }

  /** True between net_transport_init() and net_transport_shutdown(). */
  get active(): boolean {
    return this._active;
  }

  setSender(fn: SeamSender | null): void {
    this.sender = fn;
  }

  // ── token <-> device mapping (driven by the join handshake) ────────────────

  registerPeer(deviceId: string, token: number): void {
    // Rejoin must never leave two devices bound to the same native seat.
    if (!Number.isInteger(token) || token < 0 || token >= 16 || !deviceId) return;
    const previous=this.tokenToDevice.get(token);
    if(previous && previous!==deviceId)this.deviceToToken.delete(previous);
    const previousToken=this.deviceToToken.get(deviceId);
    if(previousToken!==undefined && previousToken!==token)this.tokenToDevice.delete(previousToken);
    this.tokenToDevice.set(token, deviceId);
    this.deviceToToken.set(deviceId, token);
  }

  unregisterPeer(deviceId: string): void {
    const t = this.deviceToToken.get(deviceId);
    if (t !== undefined) this.tokenToDevice.delete(t);
    this.deviceToToken.delete(deviceId);
  }

  tokenOf(deviceId: string): number | undefined {
    return this.deviceToToken.get(deviceId);
  }

  deviceOf(token: number): string | undefined {
    return this.tokenToDevice.get(token);
  }

  connectedTokens(): number[] {
    return [...this.tokenToDevice.keys()];
  }

  // ── Inbound (WebRTC -> queue -> net_poll -> C) ─────────────────────────────

  /** A raw netcode frame arrived from a peer (post-attach). Enqueued for net_poll. */
  enqueueFrameByDevice(deviceId: string, channel: number, bytes: Uint8Array): void {
    const peer = this.deviceToToken.get(deviceId);
    if (peer === undefined) return; // not a registered game peer: drop
    if (!this.validFrame(channel,bytes)) return;
    if(this.inbound.length >= NET_V33_MAX_QUEUE_ITEMS ||
       this.queuedBytes+bytes.byteLength > NET_V33_MAX_QUEUED_BYTES){
      this.droppedFrames++;return; // bounded heap; transport caller must recover using existing netcode retransmission
    }
    // Copy: the caller's view may be backed by a buffer that is reused.
    this.inbound.push({ kind: 0, peer, channel, data: bytes.slice() });
    this.queuedBytes += bytes.byteLength;
  }

  /** A peer became a game participant (up) or left (down). Enqueued in order. */
  enqueuePeerEventByDevice(deviceId: string, up: boolean): void {
    const peer = this.deviceToToken.get(deviceId);
    if (peer === undefined) return;
    // Events must never be silently dropped: preserve seat state by replacing
    // queued frames with a peer-state event at capacity.
    if(this.inbound.length>=NET_V33_MAX_QUEUE_ITEMS){
      const at=this.inbound.findIndex(i=>i.kind===0);
      if(at<0){this.droppedFrames++;return;}
      const old=this.inbound.splice(at,1)[0];
      if(old.kind===0)this.queuedBytes-=old.data.byteLength;
      this.droppedFrames++;
    }
    this.inbound.push({ kind: 1, peer, event: up ? NET_PEER_UP : NET_PEER_DOWN });
  }

  /** Drain all queued inbound items (frames + events) in arrival order. Called by
   *  the js-library's net_poll implementation, which then invokes the C entrypoints
   *  Net_ReceiveFrame / Net_PeerEvent for each. */
  drain(): SeamItem[] {
    if (this.inbound.length === 0) return [];
    const out = this.inbound;
    this.inbound = [];
    this.queuedBytes = 0;
    return out;
  }

  // ── Outbound (C -> here -> WebRTC) ─────────────────────────────────────────

  /** net_send: to a single peer by token. Bytes are already copied by the js-library
   *  out of the wasm heap. */
  send(peerToken: number, channel: number, _reliable: number, bytes: Uint8Array): void {
    const dev = this.tokenToDevice.get(peerToken);
    if (dev === undefined || !this.sender || !this.validFrame(channel,bytes)) return;
    this.sender(dev, channel, bytes);
  }

  /** net_broadcast: to every registered peer (never loops back to self — self is
   *  never in the token map). */
  broadcast(channel: number, _reliable: number, bytes: Uint8Array): void {
    if (!this.sender || !this.validFrame(channel,bytes)) return;
    for (const dev of this.tokenToDevice.values()) this.sender(dev, channel, bytes);
  }

  // ── Lifecycle ──────────────────────────────────────────────────────────────

  init(): void {
    this._active = true;
  }

  shutdown(): void {
    this._active = false;
    this.inbound = [];
    this.queuedBytes = 0;
    this.tokenToDevice.clear();
    this.deviceToToken.clear();
  }
}
