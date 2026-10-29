// Duke X Survivors — actual EDuke32 game.cpp gameplay extension.
// GPL-2.0-or-later; no external renderer, world simulation, or ripped art.
// Engine-native projectiles, collisions and original Duke combat share the same Build world.
#pragma once
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include "dukex_v46_progression.h"
// DXV29_PORTABLE_NATIVE: Emscripten only decorates exported functions.
// The real gameplay and HUD compile on non-browser EDuke32 builds too.
#ifndef EMSCRIPTEN_KEEPALIVE
# define EMSCRIPTEN_KEEPALIVE
#endif
// DXV26 offer selection uses an independent randomized seed per run.

extern int32_t g_cameraDistance; // Defined by original EDuke32 game.cpp after this include.

#if defined(__EMSCRIPTEN__) || defined(NETNATIVE)
extern int32_t g_netStreamMode;
#endif

extern "C" int DukeX_TouchSprintPercent(int playerNum);
extern "C" int DukeX_WorldMoveScalePercent(int playerNum,int fvel,int svel);

// One catalog of human-readable ability descriptions for every renderer:
// original Build HUD on PC/consoles AND Android/Chrome upgrade cards.
static char const* const DukeXOfferDesc[170]={
    "MORE LEAD. FASTER SHOTS.", "WIDER BUCKSHOT VOLLEYS", "EXPLOSIVE ROCKET BARRAGE", "FREEZE ALIENS IN PLACE",
    "SHRINK AND FINISH FOES", "PIERCE THE MOB WITH LASERS", "TWIN ROCKET CHAOS", "MORE PROJECTILES PER VOLLEY",
    "SHORTER WEAPON COOLDOWNS", "LARGER PROJECTILE SPREAD", "CRITICAL FOLLOW-UP SHOTS", "FASTER WEAPON CYCLES",
    "PULL DISTANT XP CRYSTALS", "MORE XP FROM EVERY PICKUP", "REDUCE DAMAGE AND BUILD SHIELD", "REGENERATE HEALTH OVER TIME",
    "RAISE YOUR MAXIMUM HEALTH", "MOVE FASTER THROUGH LEVELS", "BETTER POWER SELECTION", "CHANCE FOR AN EXTRA PROJECTILE",
    "ORBITING ENERGY CELLS HIT ADJACENT ALIENS", "RADIAL RADIATION DAMAGES NEARBY ALIENS", "GUIDED SHOTS SEEK NEAREST TARGET", "SCATTERED EXPLOSIVE TRAPS",
    "PIERCING LINEAR BEAMS", "RING OF KNOCKBACK DAMAGE", "ANGLED SHRAPNEL VOLLEYS", "EXPLOSIVE FIRE ZONES",
    "CROSSING PLASMA BEAMS", "SHATTER FROZEN HOSTILES", "SWARM OF SHRINK RAYS", "SPIRALING HEAVY ROCKETS",
    "GRID OF LASER FIRE", "FAST ARMOR-PIERCING BEAMS", "MORE DAMAGE PER HIT", "FASTER AUTO WEAPON CYCLING",
    "LARGER AURA AND ORBIT TIME", "REGAIN HEALTH OVER TIME", "MORE EXPERIENCE FROM ENEMIES", "TAKE LESS DAMAGE",
    "WIDER TARGET SCANNING", "VISIBLE CHAIN LIGHTNING JUMPS BETWEEN ALIENS", "SATELLITE DRONES EMIT AREA SHOCK PULSES", "MORE CHAIN DAMAGE AND EXTRA LIGHTNING BOUNCES",
    "EARN ADDITIONAL XP FROM ALL SOURCES", "THROW BOUNCING PIPEBOMBS WITH REAL WORLD COLLISION", "RICOCHET ENERGY DISCS REBOUND AND HIT ALIENS", "ATOMIC EXPANDER RAY, WITH SHAREWARE SAFE FALLBACK",
    "LOB CRYOGENIC BOMBS THAT RELEASE FREEZE PROJECTILES", "THROW SHOCK GRENADES THAT ARC ELECTRICITY ON IMPACT", "INCREASE HEALTH DROPS AND HEALING RANGE", "RECHARGE THE ORIGINAL DUKE JETPACK FUEL",
    "JUMP AGAIN WHILE AIRBORNE", "A THIRD JUMP, WITH REAL VERTICAL MOMENTUM", "PERSISTENT RED LASER TRAPS MULTIPLY", "REGENERATE ORIGINAL DUKE BODY ARMOR",
    "ALIENS DROP MEDKITS AND FIRST AID", "ELECTRIC FIELD BURNS THE NEAREST MOBS", "REAL STRAIGHT-LINE PIERCING LASER RAILS", "ORBITING DRONES BURST WITH SHRAPNEL",
    "REFILL ORIGINAL DUKE STEROIDS", "REGENERATE HEALTH BETWEEN WAVES", "NATIVE HOMING PLASMA DARTS WITH TURN RATES AND IMPACTS", "PRISM SHARDS THAT SPIN AROUND YOU AND CUT SWARMS",
    "SERRATED BOOMERANGS THAT FLY OUT, ARC AND RETURN", "GRAVITY-AFFECTED ICE COMETS WITH FROST IMPACTS", "DISTORTION FIELD PULLS ENEMIES INTO A DAMAGING SINGULARITY", "CROSSING ELECTRIC BEAMS FORM AN ACTIVE DAMAGE LATTICE",
    "MAGNETIC RECYCLER MULTIPLIES PICKED UP EXPERIENCE", "ACCELERATE ALL AUTO WEAPONS BY MAINTAINING KILL COMBOS", "RAPID HOMING MICRODARTS THAT ACCELERATE ON PURSUIT", "LONG STREAKS THAT PIERCE PACKED ENEMIES",
    "DELAYED BOUNCING CRYOGENIC EXPLOSIVES", "FIVE CONVERGING SEEKER ANGLES", "STAGGERED INCENDIARY PROJECTILE BARRAGE", "ICE SEEKING BOLTS THAT FREEZE ON CONTACT",
    "RETURNING SERRATED BLADES", "ARCING FREEZING COMETS IN SEPARATED LANES", "DELAYED ORBIT-TO-HOMING MISSILES", "ROTATING CUTTING PROJECTILES",
    "SWEEPING INCANDESCENT DAMAGE BEAM", "FOCUSED ELECTROCUTION LINE", "DUAL ANGLED LASER REFLECTIONS", "FREEZING WALL-CLIPPED TETHER",
    "ROTATING LASER SPOKE", "PERPENDICULAR INTERSECTING LANCES", "SINGLE HEAVY PULSE WITH LARGE PENETRATION", "OSCILLATING ELECTRICAL ARC",
    "ALTERNATING LATERAL ENERGY BLADES", "SLOW SWEEPING WIDE DAMAGE CONE", "COUNTER-ROTATING ORBITAL SHARDS", "HEAVY INNER-RING ORBITAL CUTTERS",
    "ORBITING ELECTRIC CONTACT SHOCKS", "WIDE FALCONING RETURN ORBITALS", "ELLIPTICAL ORBITING NEEDLES", "TWO OPPOSING ORBITAL CRUSHERS",
    "MULTIPLE SHIFTING PETAL RINGS", "COLD ORBITALS THAT STOP ENEMIES", "BURNING SATELLITE PROTECTION", "SLOW ROTATING GRAVITATIONAL SENTINELS",
    "DAMAGE-OVER-TIME TOXIC FIELD", "PULSING HEATED PERIMETER", "CYCLING FROST AURA", "CHARGED KNOCKBACK STORM",
    "CLOSE-RANGE COLLAPSING PULL FIELD", "EXPANDING PRESSURE SHOCKWAVE", "FREEZING CLOSE-CONTACT FIELD", "BRIGHT PERIODIC RADIAL EXPLOSIONS",
    "HEAVY SINGULARITY PULSES", "LOW-HEALTH EXECUTION AURA", "LIGHTNING JUMPS THROUGH MULTIPLE ENEMIES", "REPEATED CROSSING INTERLACED RAILS",
    "COLD TRIPWIRE LATTICE", "ELECTRIFIED TRIPWIRE FIELD", "MULTIPLE GENUINE BURNING HAZARDS", "SPIRAL SCATTERING ION PROJECTILES",
    "BOUNCING GLASS PROJECTILE SPRAY", "HIGH-IMPACT SHOCKWAVE RINGS", "SMART SEEKER RETARGET BURSTS", "COORDINATED BEAMS AND ORBITALS",
    "HEALS AFTER VERIFIED KILLS", "ARMORS RAPIDLY AT CRITICAL HEALTH", "REDUCES WEAPON SCHEDULING DELAY", "ENLARGES BEAM CONTACT RADIUS",
    "EXTENDS ELECTRICITY TO ADJACENT VICTIMS", "RESTORATION RELEASES SHORT-LIVED DAMAGING NANITE SATELLITES.", "BOOSTS DAMAGE VERSUS FROZEN FOES", "EVERY EARNED XP BURST ALSO UNLEASHES A SHORT-RANGE CRYSTAL SHRAPNEL PULSE.",
    "AFTER TAKING DAMAGE, RELEASES A VISIBLE KNOCKBACK SHOCK AND BUILDS ARMOR.", "INCREASES NATIVE PROJECTILE DAMAGE", "RESTORES HEALTH DURING NEAR-DEATH RECOVERY", "ADDS DAMAGE TO RAT SWARMS",
    "BONUS DAMAGE TO MILESTONE BOSSES", "STRONGER BEAM PRECISION STRIKES", "INCREASES ORBITAL REVOLUTION RATE", "ELECTRIC IMPACTS CAUSE CRACKLING ORBITING ARMOR SPARKS.",
    "MAKES FREEZING STRONGER", "COLLECTED XP CHARGES A BRIEF DAMAGING ELECTRIC DISCHARGE.", "REWARDS CLOSE-RANGE ENEMY KILLS", "PERIODICALLY PRODUCES MEDKITS",
    "MORE DAMAGE AT LOW HEALTH", "GREATER DAMAGE VERSUS ELITE ENEMIES", "FOLLOW-UP ATTACKS ON REPEATED HITS", "ADDS PHYSICAL ORBITAL BODIES",
    "FASTER MOVEMENT INPUT", "MOVEMENT SENDS STAGGERED SEISMIC SHOCKS THROUGH NEARBY ENEMIES.", "A ROTATING TARGETING LENS INFLICTS PRECISION PIERCING DAMAGE.", "PERIODICALLY SENDS A VISIBLE WALL-CLIPPED BLAST TOWARD ENEMIES.",
    "PHYSICALLY DRAGS LOOSE XP CRYSTALS WHILE RELEASING A DAMAGING GRAVITY PULSE.", "LOW HEALTH RELEASES DEFENSIVE ELECTRIC NOVA ATTACKS.", "RETURNING DAMAGE EMITS A PHYSICAL SHRAPNEL RETALIATION.", "ADDS PICKUP COLLECTION RADIUS",
    "KILL STREAKS PERIODICALLY CONJURE FALLING CRYO METEORS.", "PERIODIC RADIAL IMPULSE ATTACK", "GREATER WEAPON DAMAGE WITH REDUCED SAFETY", "RAISES SURVIVABLE HEALTH CEILING",
    "LOBS DAMAGING CRYOGENIC COMETS TOWARD ELEVATED ENEMIES.", "IMPROVES BEAM UPTIME", "AMPLIFIES SHATTER DAMAGE", "GRANTS HEALING FROM SHOCKED ENEMIES",
    "IMPROVES NATIVE PROJECTILE BURSTS", "ADDS ANOTHER ORBITAL SATELLITE", "INCREASES GRAVITATIONAL RANGE", "INCREASES AURA RADIUS",
    "DAMAGING BLAST AFTER A KILL STREAK", "KILLS CHARGE A VISIBLE MINIATURE DETONATION.", "ARMOR RESTORATION SUMMONS TEMPORARY DEFENSIVE DRONES.", "RECHARGES JETPACK FUEL",
    "PERIODICALLY FIRES A SHORT-LIVED CROSSING BEAM AFTER MOVEMENT.", "PERIODICALLY RELEASES A HEAVY SHOCK RING"
};
namespace DukeX {
constexpr int kHz=30;
constexpr int kMaxLevel=120;
constexpr int kWeaponCount=7;
constexpr int kSkillCount=170;
static_assert(kSkillCount == (int)(sizeof(DukeXOfferDesc)/sizeof(DukeXOfferDesc[0])), "Upgrade catalog mismatch");
constexpr int kAdvancedBegin=20;
constexpr int kAdvancedEnd=33;
constexpr int kPassiveBegin=33;
constexpr int kLoadoutCount=5;
constexpr int kArsenalStart=70, kArsenalWeapons=50, kArsenalPassives=50;

constexpr int kGemLimit=288;
// DXV48: magnet and vacuum are native, frame-bounded simulation rules.
constexpr int kXPBaseMagnetRadius=1720;  // +23% from V47 default 1400
constexpr int kXPMagnetRankRadius=900;
constexpr int kVacuumDurationTicks=kHz*18;
constexpr int kVacuumMaxStep=14000;
constexpr int kVacuumCollectRadius=420;
constexpr int kDeathQueueLimit=256;
constexpr int kMaxSpawnsPerBurst=24;
constexpr int kTargetRadius=22000;
constexpr int kNoTarget=-1;

// Preserve upgrades across saves only when the engine integration explicitly
// serializes them. Normal map transitions create a fresh survival run.
enum Weapon { PISTOL, SHOTGUN, ROCKET, FREEZE, SHRINK, LASER, DEVASTATOR };
static char const* const weaponNames[kWeaponCount]={
    "DUKE'S LEAD STORM","STREET SWEEPER","RPG STORM","CRYO FIELD",
    "SHRINK VORTEX","LASER GRID","DEVASTATOR"
};
static char const* const skillNames[kSkillCount]={
    "LEAD STORM", "STREET SWEEPER", "RPG STORM", "CRYO FIELD", "SHRINK VORTEX",
    "LASER GRID", "DEVASTATOR", "MULTISHOT", "OVERCLOCK", "AREA BOOST",
    "HOT LEAD", "BULLET SPEED", "MAGNET", "XP BOOST", "ARMOR",
    "REGEN", "VITALITY", "MOVE SPEED", "LUCK", "CRITICAL",
    "ENERGY SATELLITES", "RADIOACTIVE AURA", "SMART SEEKERS", "TRIPBOMB FIELD", "RAILGUN SHREDDER",
    "SHOCKWAVE", "BOUNCING SHRAPNEL", "BURNING NAPALM", "PLASMA LATTICE", "CRYO SHATTER",
    "SHRINK SWARM", "ROCKET CYCLONE", "STORM GRID", "PIERCING RAILS", "AMPLIFIER",
    "REACTOR COOLING", "FIELD DURATION", "LIFE DRAIN", "DOUBLE LOOT", "HEAVY ARMOR",
    "PROXIMITY RADAR", "THUNDER CHAIN", "PULSE DRONES", "ELECTRIC FURY", "XP FEVER",
    "BOUNCING PIPEBOMBS", "RICOCHET DISCS", "ATOMIC EXPANDER", "CRYOGENIC BOMBS", "ARC GRENADES",
    "VITALITY MAGNET", "JETPACK AFTERBURNER", "DOUBLE JUMP", "TRIPLE JUMP", "RED BEAM MULTIPLIER",
    "TITANIUM ARMOR", "FIELD MEDIC", "ELECTRIC AURA", "SPINAL RAIL", "DRONE DETONATION",
    "BERSERKER STEROIDS", "PHOENIX HEART", "NEXUS DARTS", "PRISM HALO", "RECALL SAWS",
    "CRYO COMETS", "GRAVITY WELL", "TESLA MESH", "RECYCLER", "MOMENTUM ENGINE",
    "NOVA NEEDLES", "RIFT LANCES", "ECHO GRENADES", "PHOTON NEEDLEFAN", "CINDER PHALANX",
    "ARCTIC STINGERS", "BOOMERANG FLOCK", "METEOR PARADE", "SCARAB MISSILES", "SPIRAL FLENSERS",
    "SOLAR VIOLIN", "ION SPEAR", "MIRROR CATHEDRAL", "FROST CABLE", "HALO BEAM",
    "PRISM CROSS", "PULSE BORE", "SERPENT CURRENT", "SPECTRAL SCYTHE", "DOOM SEARCHLIGHT",
    "CROWN OF GLASS", "IRON SAINTS", "THUNDER MOONS", "VULTURE HALO", "MERCURY ROSES",
    "ECLIPSE TWINS", "ORBITAL BLOSSOM", "FROST SATELLITES", "EMBER RINGS", "SINGULAR HALO",
    "VIRUS CLOUD", "MAGMA HEART", "BLIZZARD ENGINE", "TESLA HEART", "GRAVITY LENS",
    "WAVE BREAKER", "STASIS VEIL", "SUNBURST AURA", "BLACK STAR", "PREDATOR FIELD",
    "CHAIN REAPER", "RAIL BARRICADE", "CRYO TRIPWEB", "ARC TRIPWEB", "NAPALM GARDEN",
    "PROTON CYCLONE", "SHARD STORM", "SLEDGE PULSES", "MOB HUNTER", "OBLIVION ARRAY",
    "BLOOD ENGINE", "LAST STAND", "HEAT SINK", "WIDE WAKE", "SHOCK CONDUCTOR",
    "NANITE SWARM", "CRYO MEMORY", "XP TRANSMUTER", "REACTIVE CARAPACE", "HEAVY PAYLOAD",
    "SECOND CHANCE", "RAT SLAYER", "BOSS BREAKER", "PRECISION LENS", "ORBITAL GEARBOX",
    "STATIC EXOSUIT", "COLD EMBRACE", "PICKUP VOLTAGE", "RISK PREMIUM", "WAVE MEDIC",
    "BERSERK INSTINCT", "ELITE HUNTER", "ECHO CHAMBER", "DRONE HIVE", "FAST LEGS",
    "SEISMIC FOOTFALL", "OVERWATCH LENS", "BARRICADE CRACKER", "ATTRACTOR CORE", "PANIC NOVA",
    "THORN PLATING", "MAGNET BLOOM", "COMBO METEOR", "KINETIC TAP", "GLASS CANNON",
    "TITAN CORE", "VERTICAL FURY", "LASER BATTERY", "FROST FRACTURE", "LIGHTNING THIEF",
    "RAPID ASSEMBLY", "MULTI-ORBIT", "VORTEX HEART", "AREA WEAVER", "DEATH BLOSSOM",
    "SALVAGE DETONATOR", "REPAIR DRONES", "EMERGENCY JET", "ARCANE AFTERIMAGE", "APOCALYPSE CLOCK"
};
// IDs 0-19 are backwards-compatible with V15. New weapons 20-33 and
// new passives 34-39. The original actor, CON and tile assets remain in use.
enum Advanced {
    SATELLITES=20, RADIATION=21, SEEKERS=22, MINES=23, RAILGUN=24,
    SHOCKWAVE=25, SHRAPNEL=26, DX_NAPALM_WAVE=27, PLASMA=28, CRYO_SHATTER=29,
    SHRINK_SWARM=30, ROCKET_CYCLONE=31, STORM_GRID=32, PIERCING_RAILS=33,
    AMPLIFIER=34, COOLING=35, DURATION=36, LIFESTEAL=37,
    LOOT=38, HEAVY_ARMOR=39, PROXIMITY_RADAR=40,
    THUNDER_CHAIN=41, PULSE_DRONES=42, ELECTRIC_FURY=43, XP_FEVER=44,
    BOUNCY_BOMBS=45, RICOCHET_DISCS=46, EXPANDER_RAY=47, ICE_BOMBS=48, ARC_GRENADES=49,
    HEALTH_MAGNET=50, JET_AFTERBURNER=51, DOUBLE_JUMP=52, TRIPLE_JUMP=53,
    RED_BEAM_MULTIPLIER=54, TITANIUM_ARMOR=55, FIELD_MEDIC=56, ELECTRIC_AURA=57,
    SPINAL_RAIL=58, DRONE_DETONATION=59, BERSERKER_STEROIDS=60, PHOENIX_HEART=61,
    NEXUS_DARTS=62, PRISM_HALO=63, RECALL_SAWS=64, CRYO_COMETS=65,
    GRAVITY_WELL=66, TESLA_MESH=67, RECYCLER=68, MOMENTUM_ENGINE=69
};
static bool IsWeaponSkill(int id){return (id>=70 && id<120) || id<7 || (id>=kAdvancedBegin&&id<=PIERCING_RAILS) || id==THUNDER_CHAIN || id==PULSE_DRONES || (id>=BOUNCY_BOMBS && id<=ARC_GRENADES) || id==RED_BEAM_MULTIPLIER || id==ELECTRIC_AURA || id==SPINAL_RAIL || id==DRONE_DETONATION || (id>=NEXUS_DARTS&&id<=TESLA_MESH);}
static int MaxRank(int id){return IsWeaponSkill(id)?8:6;}
static int EquippedWeapons(struct Fighter const& f);
static char const* const loadoutNames[kLoadoutCount]={
    "DUKE CLASSIC","DEMOLITION EXPERT","CYBER COMMANDO","HAZMAT SPECIALIST","CRYO MARINE"
};
struct Fighter {
    int level=1,xp=0,threshold=7,unspent=0,kills=0,elapsed=0;
    int skill[kSkillCount]={};
    int combo=0,comboTimer=0,bestCombo=0;
    int earnedCredits=0;
    int rerolls=2, banishes=1;
    uint8_t banished[kSkillCount]={};
    uint8_t rarity[kSkillCount]={};       // item-bound tier, 0..5
    uint8_t absorbed[kSkillCount]={};     // fusion component remains active but stops occupying its slot
    uint8_t fused[kSkillCount]={};        // permanently upgraded merged attack family
    uint16_t fusionPartner[kSkillCount]={}; // generic recipe link: partner item id + 1
    uint8_t statRoll[kSkillCount]={};     // reproducible 0..8 stat modifier
};
static int careerWeaponSlots=4,careerPassiveSlots=4;
static int OccupiedSlots(Fighter const& f,bool weapons){
    return DukeXV46::CountSlots(f.skill,f.absorbed,IsWeaponSkill,weapons);
}
static bool CanOfferSkill(Fighter const& f,int id){
    return id>=0 && id<kSkillCount && DukeXV46::CanAcquire(id,f.skill,f.banished,
        f.absorbed,MaxRank(id),careerWeaponSlots,careerPassiveSlots,IsWeaponSkill);
}
static int EquippedWeapons(Fighter const& f){return OccupiedSlots(f,true);}
struct Gem {int id=-1,value=0,age=0,tile=-1;int vz=0;int bounces=0;};
struct Drop {int x=0,y=0,z=0,sectorId=-1,value=1;};
static Fighter fighters[MAXPLAYERS];
static Gem gems[kGemLimit];
// Rare in-world power caches: collect to EQUIP a random weapon or passive,
// independent of the three-card level-up flow.
struct Cache { int id=-1;int age=0,tile=-1,kind=0,vz=0;}; // 0: upgrade, 1: health, 2: armor, 3: fuel, 4: medkit
static Cache caches[32];
// A timed, player-owned world pickup. Unlike ordinary magnetization, this
// crosses doors/portals to collect all remaining native XP gems in a level.
struct XPVacuum {int seat=-1, ticks=0, collected=0, triggerCount=0;};
static XPVacuum xpVacuum;
static int xpVacuumDrops=0;
// A real engine-owned chest presentation. World simulation pauses for solo
// reveal, but HUD ticks still animate it using native Build sprite drawing.
struct ChestReveal {
    int seat=-1,frames=0,count=0,spins=0,icon=TRASH;
    int reward[5]={-1,-1,-1,-1,-1};
    uint8_t rarity[5]={};
    bool fusion[5]={};
};
static ChestReveal chestReveal;
static int chestsSpawned=0,chestsOpened=0,chestFusions=0;
static int lastBoon=-1,lastBoonStrength=0,lastBoonUntil=0;
static uint8_t activeBoons[8]={},activeBanes[8]={};
// A super-rare standalone three-choice run mutation, separate from inventory.
struct TwistChoice {int seat=-1,frames=0;DukeXV46::RunTwist cards[3]{};};
static TwistChoice twistChoice;
static const char *const twistNames[8]={
    "RAT SWARM", "XP WINDFALL", "BLOOD PRICE", "BOSS UPRISING",
    "CHEST FEVER", "CRIT FRENZY", "MAGNET STORM", "WILD SURGE"
};
static int lastCacheAward=-1, cacheBannerUntil=0;
static Drop drops[kDeathQueueLimit];
static int dropRead=0,dropWrite=0;
static int gameTicks=0,episode=-1,level=-1,lastPar=-1,wave=1,activeEnemies=0;
static int bossWave=0,spawnDebt=0;
// DXV32.2: shareware RAT tile is 7x6 ART pixels. Survivor rats are larger,
// engine-native actors with independent AI, HP and Duke damage integration.
static uint8_t survivorRat[MAXSPRITES]={};
static int ratNextBite[MAXSPRITES]={};
static int ratGlobalBiteReady=0;
static uint8_t ratVariant[MAXSPRITES]={};
static int ratVerticalSpeed[MAXSPRITES]={};
static bool IsSurvivorRat(int id){return id>=0 && id<MAXSPRITES &&
    survivorRat[id] && sprite[id].picnum==RAT && sprite[id].statnum==STAT_ACTOR;}
static bool IsSurvivorEnemy(int id){return id>=0 && id<MAXSPRITES &&
    (IsSurvivorRat(id) || A_CheckEnemyTile(sprite[id].picnum));}
static bool initialized=false;
static int careerCredits=0,careerRank=0;
static int requestedLoadout=0, appliedLoadout=-1;
// DXV18: run lifecycle is observed BEFORE checking the solo upgrade pause.
// Otherwise stale unspent ranks can block Tick() from resetting on respawn.
static bool sawPlayerDeath=false;
static int runGeneration=0;
static int restartRequested=0;

static int offerGeneration=0;
static uint32_t offerSeed=0;
static int activeOffer[3]={-1,-1,-1};
static uint8_t activeOfferRarity[3]={};
static bool offerReady=false;
static uint32_t offerNonce=0;
static uint16_t offerSeen[170]={};
static int satelliteIds[12];
// DXV28: finite pools for genuine persistent native tripwires and visual effects.
struct Tripwire {
 int id=-1,seat=-1,life=0,rank=0;
 int pitch=0,heading=0,armDelay=12;
 int originX=0,originY=0,originZ=0,endX=0,endY=0,endZ=0;
 int beams[16]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; // 8 aligned segments x halo/core
};
struct Visual { int id=-1,life=0,tile=-1; };
static Tripwire tripwires[16];
static Visual visuals[144];
// DXV34: Bounded native ribbon pool, independent from the original FX particle pool.
struct BeamRibbon {int id=-1,life=0,layer=0;};
static BeamRibbon beamRibbons[256];
static int beamTraces=0, beamWorldImpacts=0, beamDamageHits=0, beamWaterStops=0;
static int beamSkyStops=0, beamWallsStopped=0, beamRibbonCount=0;
static int beamLastDamageTick[MAXSPRITES]={};
// V42: true source/end snapshots for EVERY transient native beam, not only mines.
struct NativeBeamFx {int x0=0,y0=0,z0=0,x1=0,y1=0,z1=0,thick=0,pal=0,expiry=0;};
static NativeBeamFx nativeBeamFx[96];
static uint32_t nativeBeamCursor=0;
// Previous actor positions for high-speed tripwire crossings between ticks.
static int tripPrevX[MAXPLAYERS]={},tripPrevY[MAXPLAYERS]={};
static bool tripPrevReady[MAXPLAYERS]={};
// DXV30: these are real colliding world sprites, not renamed A_Shoot volleys.
// No browser dependency: SDL and Emscripten compile the same gameplay code.
enum ThrownKind { PIPEBOMB=0, DISC=1, CRYO_BOMB=2, ARC_GRENADE=3 };
struct Thrown { int id=-1,seat=-1,kind=0,life=0,bounces=0,rank=1;
               int speed=0,vertical=0,grace=0; };
static Thrown thrown[40];
// V33: independent engine-native projectiles. NOT A_Shoot duplicates.
// Every bolt has a world sprite, Build collision and a native lifespan.
enum NativeBoltKind { NB_NEXUS=0, NB_PRISM=1, NB_RECALL=2, NB_COMET=3 };
struct NativeBolt {int id=-1,seat=-1,kind=0,rank=0,life=0,age=0,angle=0,
    speed=0,vertical=0,target=-1,hitCooldown=0,originalTile=-1;};
static NativeBolt bolts[80];
// V47: real guided/spiral missiles. Fixed pool, native sprites and Build
// collision. Independent of A_Shoot's CON paths and visually reproducible.
struct V47Missile {int id=-1,seat=-1,kind=0,rank=1,life=0,angle=0,
                         age=0,phase=0,speed=0,originalTile=-1,target=-1;};
static V47Missile v47Missiles[64];
static int v47MissilesLaunched=0,v47MissilesImpacted=0;
static int v47ShotgunContacts=0,v47RailGuaranteed=0,v47TransformerReactions=0;

static int nativeBoltHits=0, nativeBoltCollisions=0, nativeBoltsFired=0;
// DXV35: solo play is hands-free by default. The player may explicitly disable it.
static int autoMode=1;
struct AutoBrain {
    int nextThink=0,desired=0,stuck=0,lastX=0,lastY=0,lastCheck=0;
    int goal=-1,goalKind=0,goalX=0,goalY=0;
    int wanderAngle=0,roamChange=0;
    int forward=0,strafe=0;
    // DXV37: movement heading is independent of view/weapon tracking.
    int aimAngle=0,aimHorizon=100,aimTarget=-1,aimKind=0;
    int blockedAngle=-1,blockedUntil=0,decisionGoal=-1;
    int rejectedGoal=-1,rejectedUntil=0,failedGoals=0;
    int lastGoalKind=0,plannedRange=0;
    uint16_t visits[256]={};
};
static AutoBrain autoBrain;
static int autoDecisions=0, autoAvoidance=0, autoExplorations=0;

static int impacts=0,ricochets=0,cryoBursts=0,expandShots=0;
static int missingOptionalTiles=0;
// DXV32 COMBAT CONTRACT: status is independent of transient CON projectile
// records.  Enemy actors remain the authority for their own movement/death.
enum EnemyCondition : uint8_t { EC_NONE=0, EC_FROZEN=1, EC_MINI=2, EC_GROWING=3, EC_SHOCKED=4, EC_BURNING=5 };
struct EnemyAffliction {
    int picnum=-1, owner=-1, until=0, nextPulse=0;
    uint8_t kind=EC_NONE, xrepeat=0, yrepeat=0, clipdist=0, pal=0;
};
static EnemyAffliction afflictions[MAXSPRITES];
static int confirmedHits=0, confirmedDamage=0, blockedHits=0;
static int nativeDamageFallbacks=0;
static int frozenVictims=0, miniVictims=0, grownVictims=0, shockedVictims=0;
static int shatteredVictims=0, conditionExpiry=0;
static int combatLastVictim=-1, combatLastHPBefore=0, combatLastHPAfter=0;


// Touch axes/buttons are written by browser JS, consumed by original P_GetInput.
// Keep indices stable: the renamed napalm slot remains item ID 27.
static int touchX=0,touchY=0,touchAimX=0,touchAimY=0;
static int touchButtons=0;
static uint32_t inventoryActions=0; // edge-triggered native input action queue
static int jumpWasHeld[MAXPLAYERS]={}, extraJumpsUsed[MAXPLAYERS]={};
static int inventoryActionCount=0, airJumpCount=0, collectedHealth=0, collectedArmor=0;
static int touchSamples=0, touchAppliedFvel=0, touchAppliedSvel=0;
static int touchAppliedTurn=0, touchAppliedPitch=0, touchAppliedButtons=0;
static int blastRequests=0, blastCooldown=0;
static int shotsRemaining=24;
// DXV29: choose real world-speed target in Field Computer (320 / 440 / 560%).
static int soloRunPercent=440;
static int napalmBursts=0,arcHits=0,tripwireDetonations=0;
struct NapalmField {int id=-1,seat=-1,life=0,rank=0;};
static NapalmField napalmFields[12];
static uint8_t enemyObserved[MAXSPRITES]={}, enemyDropSeen[MAXSPRITES]={};
static int enemyAggroX[MAXSPRITES]={},enemyAggroY[MAXSPRITES]={};
static uint8_t enemyIdleScore[MAXSPRITES]={};
static uint8_t enemyWakeAttempts[MAXSPRITES]={};
static uint8_t enemyDamageVetoStreak[MAXSPRITES]={};
static int nativeAggroWakes=0;
static Drop enemyLastSeen[MAXSPRITES];
// PCG32: reproducible world simulation; no global C rand() or modulo-biased draws.
static uint64_t rngState=0x853c49e6748fea9bull;
static uint64_t rngSequence=0xda3e39cb94b95bdbull;
static uint32_t rng=0x7e5d0321u; // retained for older diagnostic tooling

static int Min(int a,int b){return a<b?a:b;}
static int Max(int a,int b){return a>b?a:b;}
static int Clamp(int a,int low,int high){return Min(Max(a,low),high);}
// DXV30 EDITION ART MATRIX: queried at runtime from whichever shareware,
// registered 1.3D, Atomic 1.4/1.5 or other compatible GRP is mounted.
static bool HasArt(int tile){
    return tile>=0 && tile<MAXTILES && tilesiz[tile].x>0 && tilesiz[tile].y>0;
}
static int EditionTile(int preferred,int fallback){
    if(HasArt(preferred))return preferred;
    ++missingOptionalTiles;
    return HasArt(fallback)?fallback:SHOTSPARK1;
}
// DXV39: 100 real, original 64x64 world sprite tiles. Never trust the ART file
// to be installed: fall back to a shipped Duke sprite when unavailable.
static constexpr int kV39ArtBase=12800;
static int V39Art(int ability){
    int const tile=kV39ArtBase+ability-70;
    return ability>=70 && ability<170 && HasArt(tile)?tile:TRANSPORTERSTAR;
}
// V47 sprites are original Build ART (transparent index 255), not TRASH
// decorations whose background pixels can become opaque when pal-cycled.
static constexpr int kV47ArtBase=13056;
static int V47Art(int index,int fallback=TRANSPORTERSTAR){
    const int t=kV47ArtBase+index;
    return index>=0 && index<13 && HasArt(t)?t:fallback;
}
static int ArtRepeat(int tile,int intendedPixels=36){
    int dim=16;
    if(HasArt(tile))dim=Max(4,Max((int)tilesiz[tile].x,(int)tilesiz[tile].y));
    return Clamp((intendedPixels*32)/dim,14,72);
}
static int GemTile(){return HasArt(CRYSTALAMMO)?CRYSTALAMMO:
    HasArt(ATOMICHEALTH)?ATOMICHEALTH:HasArt(AMMO)?AMMO:RPG;}
static int CacheTile(){return HasArt(CRYSTALAMMO)?CRYSTALAMMO:
    HasArt(RPGAMMO)?RPGAMMO:HasArt(ATOMICHEALTH)?ATOMICHEALTH:RPG;}
static int OrbitalTile(){return HasArt(CRYSTALAMMO)?CRYSTALAMMO:
    HasArt(TRANSPORTERSTAR)?TRANSPORTERSTAR:RPG;}
// DXV36_NATIVE_GROUND_PHYSICS: one floor-contact contract for grounded rats,
// experience crystals, and supply drops. Coordinates use Build z units (down
// is positive). Do NOT replace getflorzofslope with sector[].floorz: slopes
// and sector transitions otherwise leave rats/gems hovering above the map.
static int GroundHeight(int16_t sec,int x,int y){
    if(sec<0 || sec>=numsectors)return 0;
    return getflorzofslope(sec,x,y);
}
struct GroundContact {bool grounded=false;bool hitCeiling=false;};
static GroundContact StepGroundPhysics(int id,int &verticalSpeed,
                                       int floorGap,int gravity,int terminal,
                                       int bouncePercent){
    GroundContact result{};
    if(id<0 || id>=MAXSPRITES)return result;
    auto &body=sprite[id];
    if(body.sectnum<0 || body.sectnum>=numsectors)return result;
    // The sprite z anchor is the feet of facing Build sprites; margin avoids
    // sinking into sloped surfaces. Floor and ceiling are sampled at XY.
    int const floor=GroundHeight(body.sectnum,body.x,body.y)-floorGap;
    // Facing sprites use a foot-anchored Z. Keep their full ART-scaled height
    // below the ceiling, not just their origin point.
    int const artH=HasArt(body.picnum)?(int)tilesiz[body.picnum].y:16;
    int const height=Clamp(artH*static_cast<int>(body.yrepeat)*4,1<<8,56<<8);
    int const ceiling=Min(floor,getceilzofslope(body.sectnum,body.x,body.y)+height+(1<<8));
    // Narrow sectors prioritize solid floor contact over invalid overlap.
    int const minZ=ceiling;
    int const maxZ=floor;
    verticalSpeed=Clamp(verticalSpeed+gravity,-terminal,terminal);
    int64_t proposed=static_cast<int64_t>(body.z)+verticalSpeed;
    if(proposed>=maxZ){
        body.z=maxZ;result.grounded=true;
        if(verticalSpeed>0 && bouncePercent>0)
            verticalSpeed=-(verticalSpeed*bouncePercent)/100;
        else verticalSpeed=0;
        if(verticalSpeed>-180 && verticalSpeed<180)verticalSpeed=0;
    }else if(proposed<=minZ){
        body.z=minZ;verticalSpeed=0;result.hitCeiling=true;
    }else body.z=static_cast<int>(proposed);
    return result;
}

static uint32_t Random(){
    const uint64_t old=rngState;
    rngState=old*6364136223846793005ULL+(rngSequence|1ULL);
    const uint32_t rotated=(uint32_t)(((old>>18u)^old)>>27u);
    const uint32_t shift=(uint32_t)(old>>59u);
    rng=(rotated>>shift)|(rotated<<((-shift)&31u));
    return rng;
}
static uint32_t RandomBounded(uint32_t bound){
    if(bound<=1)return 0;
    const uint32_t threshold=(uint32_t)(-bound)%bound;
    for(;;){const uint32_t r=Random();if(r>=threshold)return r%bound;}
}

static bool IsGuest(){
#if defined(__EMSCRIPTEN__) || defined(NETNATIVE)
    return g_netStreamMode && g_netClient;
#else
    return false;
#endif
}
static bool OwnWorld(){
    // In stream mode the authoritative host creates world actors. Other
    // multiplayer modes stay opt-out until the extra game state is replicated.
#if defined(__EMSCRIPTEN__) || defined(NETNATIVE)
    if(numplayers>1 && !g_netStreamMode) return false;
#else
    if(numplayers>1) return false;
#endif
    return !IsGuest();
}
static bool Alive(int seat){
    if(seat<0 || seat>=MAXPLAYERS || !g_player[seat].ps || !g_player[seat].playerquitflag)return false;
    auto const p=g_player[seat].ps;
    return p->i>=0 && p->i<MAXSPRITES && p->cursectnum>=0 &&
           (p->gm & MODE_GAME) && sprite[p->i].extra>0;
}
// Stop input AND world simulation during full-screen native modal sequences.
// Never pause multiplayer peers; the renderer/HUD continues to animate.
static bool ModalPauseActive(){
    return numplayers==1 && initialized && Alive(myconnectindex) &&
           (chestReveal.seat>=0 || twistChoice.seat>=0 ||
            (!autoMode && fighters[myconnectindex].unspent>0));
}
static void ArsenalRuntimeReset();
static void V47MissileReset();
static void Reset(){
    for(auto &f:fighters)f=Fighter{};
    for(auto &g:gems)g=Gem{};
    for(auto &c:caches)c=Cache{};
    xpVacuum=XPVacuum{};xpVacuumDrops=0;
    lastCacheAward=-1;cacheBannerUntil=0;
    chestReveal=ChestReveal{};chestsSpawned=chestsOpened=chestFusions=0;
    lastBoon=-1;lastBoonStrength=lastBoonUntil=0;
    std::memset(activeBoons,0,sizeof(activeBoons));
    twistChoice=TwistChoice{};
    std::memset(activeBanes,0,sizeof(activeBanes));
    dropRead=dropWrite=0;gameTicks=0;wave=1;bossWave=0;spawnDebt=0;
    std::memset(enemyObserved,0,sizeof(enemyObserved));
    std::memset(survivorRat,0,sizeof(survivorRat));
    std::memset(ratVariant,0,sizeof(ratVariant));
    std::memset(ratVerticalSpeed,0,sizeof(ratVerticalSpeed));
    std::memset(ratNextBite,0,sizeof(ratNextBite));
    ratGlobalBiteReady=0;
    std::memset(enemyDropSeen,0,sizeof(enemyDropSeen));
    std::memset(enemyIdleScore,0,sizeof(enemyIdleScore));
    std::memset(enemyWakeAttempts,0,sizeof(enemyWakeAttempts));
    std::memset(enemyDamageVetoStreak,0,sizeof(enemyDamageVetoStreak));
    std::memset(enemyAggroX,0,sizeof(enemyAggroX));
    std::memset(enemyAggroY,0,sizeof(enemyAggroY));nativeAggroWakes=0;
    for(auto &d:enemyLastSeen)d=Drop{};
    blastRequests=blastCooldown=touchX=touchY=touchAimX=touchAimY=touchButtons=0;
    touchAppliedFvel=touchAppliedSvel=touchAppliedTurn=touchAppliedPitch=touchAppliedButtons=0;
    for(int &id:satelliteIds)id=-1;
    for(auto &t:tripwires){
        for(int beam:t.beams)if(beam>=0&&beam<MAXSPRITES&&sprite[beam].statnum==STAT_FX&&sprite[beam].picnum==LASERLINE)A_DeleteSprite(beam);
        t=Tripwire{};
    }
    for(auto &b:beamRibbons){
        if(b.id>=0 && b.id<MAXSPRITES && sprite[b.id].statnum==STAT_FX && sprite[b.id].picnum==LASERLINE)
            A_DeleteSprite(b.id);
        b=BeamRibbon{};
    }
    beamTraces=beamWorldImpacts=beamDamageHits=beamWaterStops=beamSkyStops=beamWallsStopped=beamRibbonCount=0;
    std::memset(beamLastDamageTick,0,sizeof(beamLastDamageTick));
    for(auto &beam:nativeBeamFx)beam=NativeBeamFx{};nativeBeamCursor=0;
    for(int seat=0;seat<MAXPLAYERS;++seat)tripPrevReady[seat]=false;
    inventoryActions=0;
    for(int j=0;j<MAXPLAYERS;++j){jumpWasHeld[j]=0;extraJumpsUsed[j]=0;}
    inventoryActionCount=airJumpCount=collectedHealth=collectedArmor=0;
    for(auto &v:visuals)v=Visual{};
    for(auto &b:thrown)b=Thrown{};
    ArsenalRuntimeReset();
    V47MissileReset();
    for(auto &b:bolts){if(b.id>=0 && b.id<MAXSPRITES && sprite[b.id].statnum==STAT_FX && sprite[b.id].picnum==b.originalTile)A_DeleteSprite(b.id);b=NativeBolt{};}
    nativeBoltHits=nativeBoltCollisions=nativeBoltsFired=0;
    autoBrain=AutoBrain{};autoDecisions=autoAvoidance=autoExplorations=0;
    impacts=ricochets=cryoBursts=expandShots=missingOptionalTiles=0;
    // Restore sprite appearance/size only for the actor still occupying that slot.
    // A_DeleteSprite calls clear too; reset never leaves frozen aliases alive.
    for(int id=0;id<MAXSPRITES;++id){
        auto &a=afflictions[id];
        if(a.kind && sprite[id].statnum<MAXSTATUS && sprite[id].picnum==a.picnum){
            sprite[id].xrepeat=a.xrepeat;sprite[id].yrepeat=a.yrepeat;
            sprite[id].pal=a.pal;sprite[id].clipdist=a.clipdist;
        }
        a=EnemyAffliction{};
    }
    confirmedHits=confirmedDamage=blockedHits=nativeDamageFallbacks=0;
    frozenVictims=miniVictims=grownVictims=shockedVictims=shatteredVictims=conditionExpiry=0;
    combatLastVictim=-1;combatLastHPBefore=combatLastHPAfter=0;

    for(auto &f:napalmFields)f=NapalmField{};
    napalmBursts=arcHits=tripwireDetonations=0;
    appliedLoadout=-1;offerGeneration=0;offerReady=false;offerNonce=0;
    std::memset(offerSeen,0,sizeof(offerSeen));
    for(int &id:activeOffer)id=-1;
    std::memset(activeOfferRarity,0,sizeof(activeOfferRarity));
    activeEnemies=0;rng=0x7e5d0321u;
    // Per-run entropy for solo; fixed repeatable stream for multiplayer until
    // an authoritative shared match seed is negotiated by the net protocol.
    const uint64_t entropy=(numplayers==1)?static_cast<uint64_t>(timerGetNanoTicks()):0x7e5d0321u;
    rngState=0x853c49e6748fea9bull ^ entropy ^ (uint64_t)(runGeneration+1)*0x9e3779b97f4a7c15ULL;
    rngSequence=0xda3e39cb94b95bdbULL;
    for(int i=0;i<4;++i)Random();
    // DXV26: one entropy seed per SOLO run, stable while inspecting three cards.
    // Multiplayer stays deterministic until upgrade choices are synchronized.
    if(numplayers==1){
        uint64_t const ticks=static_cast<uint64_t>(timerGetNanoTicks());
        offerSeed=static_cast<uint32_t>(ticks ^ (ticks>>32) ^
                  (static_cast<uint32_t>(runGeneration+1)*0x9e3779b9u));
        if(!offerSeed)offerSeed=0x42694f6fu;
    }else offerSeed=0x7e5d0321u;
    // Career is deliberately not reset with individual runs.
}
// DXV46: one acquisition contract for level-up, cache and chest paths.
static void AcquireItem(Fighter &f,int id,int rarity=-1){
    if(id<0 || id>=kSkillCount || f.skill[id]>=MaxRank(id))return;
    if(f.skill[id]==0){
        int rolled=rarity>=0?rarity:DukeXV46::RollRarity(RandomBounded(10000),f.skill[18]);
        f.rarity[id]=static_cast<uint8_t>(Clamp(rolled,0,5));
        f.statRoll[id]=static_cast<uint8_t>(RandomBounded(9));
    } else if(rarity>f.rarity[id]) f.rarity[id]=static_cast<uint8_t>(Clamp(rarity,0,5));
    ++f.skill[id];
}
static int ItemPercent(Fighter const& f,int id){
    if(id<0||id>=kSkillCount || !f.skill[id])return 100;
    return DukeXV46::StatPercent(f.rarity[id],f.statRoll[id])+(f.fused[id]?16+(int)f.rarity[id]*3:0);
}
// Chest fusion keeps both acquired effects valid for legacy combat contracts
// while absorbing a component into the evolved item and freeing its slot.
static bool TryFuse(Fighter &f,int first,int second){
    if(first<0||first>=kSkillCount||second<0||second>=kSkillCount || first==second ||
       f.fused[first] || f.absorbed[first] || f.absorbed[second] ||
       f.skill[first]<2 || f.skill[second]<2)return false;
    f.fused[first]=1;f.fusionPartner[first]=(uint16_t)(second+1);f.absorbed[second]=1;
    f.rarity[first]=static_cast<uint8_t>(Min(5,(int)f.rarity[first]+1));
    ++chestFusions;
    return true;
}
// DXV28: Roll exactly WHEN the browser opens a power choice, not when XP
// crosses its threshold. Polling offers never advances the RNG. One roll
// persists until the player chooses, rerolls, or banishes. Solo uses a fresh
// device-time mix at reveal; network games use deterministic local fallback.
static uint32_t OfferHash(uint32_t x){
    x ^= x>>16; x*=0x7feb352du; x ^= x>>15;
    x*=0x846ca68bu; x ^= x>>16; return x;
}
static uint32_t NextOfferRandom(uint32_t &state){
    state ^= state<<13; state ^= state>>17; state ^= state<<5;
    if(!state)state=0x619aab3du;
    return state;
}
static uint32_t NextOfferBounded(uint32_t &state,uint32_t bound){
    if(bound<=1)return 0;
    const uint32_t threshold=(uint32_t)(-bound)%bound;
    for(;;){uint32_t r=NextOfferRandom(state);if(r>=threshold)return r%bound;}
}
static void RollOffers(Fighter const& f){
    ++offerNonce;++offerGeneration;
    if(numplayers==1){
        uint64_t t=static_cast<uint64_t>(timerGetNanoTicks());
        offerSeed=OfferHash(offerSeed ^ static_cast<uint32_t>(t) ^
            static_cast<uint32_t>(t>>32) ^ (offerNonce*0x9e3779b9u));
    } else offerSeed=OfferHash(offerSeed+offerNonce*0x85ebca6bu);
    uint32_t state=offerSeed?offerSeed:0xbedac0deu;
    for(int card=0;card<3;++card){
        activeOffer[card]=-1;activeOfferRarity[card]=0;
        int weightTotal=0;
        for(int id=0;id<kSkillCount;++id){
            if(!CanOfferSkill(f,id))continue;
            bool used=false;for(int j=0;j<card;++j)if(activeOffer[j]==id)used=true;
            if(used)continue;
            int weight=IsWeaponSkill(id)?38:80;
            if(id<7)weight=100;
            if(f.skill[id]>0)weight+=45;
            weight=Max(1,weight*4/(4+Min(40,offerSeen[id])));
            if(IsWeaponSkill(id))weight+=f.skill[18]*17;
            weightTotal+=Max(1,weight);
        }
        if(!weightTotal)continue;
        int ticket=static_cast<int>(NextOfferBounded(state,static_cast<uint32_t>(weightTotal)));
        for(int id=0;id<kSkillCount;++id){
            if(!CanOfferSkill(f,id))continue;
            bool used=false;for(int j=0;j<card;++j)if(activeOffer[j]==id)used=true;
            if(used)continue;
            int weight=IsWeaponSkill(id)?38:80;
            if(id<7)weight=100;
            if(f.skill[id]>0)weight+=45;
            weight=Max(1,weight*4/(4+Min(40,offerSeen[id])));
            if(IsWeaponSkill(id))weight+=f.skill[18]*17;
            ticket-=Max(1,weight);
            if(ticket<0){
                activeOffer[card]=id;
                activeOfferRarity[card]=static_cast<uint8_t>(DukeXV46::RollRarity(
                    NextOfferBounded(state,10000),f.skill[18]));
                break;
            }
        }
    }
    for(int card=0;card<3;++card){
        int id=activeOffer[card];
        if(id>=0&&id<kSkillCount&&offerSeen[id]<65535)++offerSeen[id];
    }
    offerReady=true;
}
static int Choice(Fighter const&,int index){
    if(!offerReady || index<0 || index>=3)return -1;
    return activeOffer[index];
}
static bool BeginOffer(int seat){
    if(numplayers!=1 || !initialized || !Alive(seat) ||
       chestReveal.seat>=0 || fighters[seat].unspent<=0)return false;
    if(!offerReady)RollOffers(fighters[seat]);
    return true;
}

static void ApplyLoadout(int seat){
    if(!Alive(seat))return;
    auto &f=fighters[seat];
    const int id=(seat==myconnectindex?requestedLoadout:0);
    if(seat==myconnectindex)appliedLoadout=id;
    // Each character starts with a DISTINCT weapon family. Satellite/radiation
    // weapons require explicitly choosing that starter or a level-up card.
    switch(id){
      case 1:AcquireItem(f,ROCKET,1);AcquireItem(f,MINES,0);break;
      case 2:AcquireItem(f,LASER,1);AcquireItem(f,SEEKERS,0);break;
      case 3:AcquireItem(f,RADIATION,1);AcquireItem(f,14,0);break;
      case 4:AcquireItem(f,FREEZE,1);AcquireItem(f,CRYO_SHATTER,0);break;
      default:AcquireItem(f,PISTOL,0);break;
    }
}
static bool Reroll(int seat){
    if(!Alive(seat)||fighters[seat].unspent==0||fighters[seat].rerolls<=0)return false;
    --fighters[seat].rerolls;offerReady=false;RollOffers(fighters[seat]);return true;
}
static bool Banish(int seat,int index){
    if(!Alive(seat)||fighters[seat].unspent==0||fighters[seat].banishes<=0||index<1||index>3)return false;
    int id=Choice(fighters[seat],index-1);if(id<0)return false;
    fighters[seat].banished[id]=1;--fighters[seat].banishes;offerReady=false;RollOffers(fighters[seat]);return true;
}
struct Evolution {int weapon,passive;char const* name;};
static Evolution const evolutions[]={
    {PISTOL,7,"LEAD CYCLONE"},{ROCKET,8,"NUCLEAR OVERCLOCK"},
    {FREEZE,9,"CRYO NOVA"},{SHRINK,12,"SHRINK SINGULARITY"},
    {SATELLITES,DURATION,"RADIANT REACTOR"},
    {RADIATION,LIFESTEAL,"MUTAGENIC PLAGUE"},
    {SEEKERS,12,"HUNTER-KILLER PACK"},
    {MINES,9,"CHAIN REACTION"},
    {RAILGUN,AMPLIFIER,"RAILSTORM"},
    {SHOCKWAVE,HEAVY_ARMOR,"KINETIC BULWARK"},
    {SHRAPNEL,19,"SHRAPNEL TEMPEST"},
    {DX_NAPALM_WAVE,DURATION,"HELLFIRE GRID"},
    {PLASMA,COOLING,"IONIZER"},
    {CRYO_SHATTER,16,"ABSOLUTE ZERO"},
    {SHRINK_SWARM,18,"QUANTUM MINIATURIZER"},
    {ROCKET_CYCLONE,7,"WORLD ON FIRE"},
    {STORM_GRID,11,"LIGHTNING WEB"},
    {THUNDER_CHAIN,ELECTRIC_FURY,"THUNDERGOD OVERDRIVE"},
    {PULSE_DRONES,COOLING,"PULSE DRONE SWARM"},
    {PIERCING_RAILS,AMPLIFIER,"PHASE RAIL"},
    {BOUNCY_BOMBS,9,"HELLBOUND BOUNCERS"},
    {RICOCHET_DISCS,COOLING,"INFINITE RICOCHET"},
    {EXPANDER_RAY,AMPLIFIER,"ATOMIC OVERGROWTH"},
    {ICE_BOMBS,DURATION,"GLACIER STORM"},
    {ARC_GRENADES,ELECTRIC_FURY,"THUNDERBALLS"},
    {FREEZE,THUNDER_CHAIN,"CRYO CONDUCTOR"},
    {SHRINK,SPINAL_RAIL,"MICRO RAIL TERMINATOR"},
    {EXPANDER_RAY,DX_NAPALM_WAVE,"MELTDOWN OVERGROWTH"},
    {SATELLITES,SHRINK_SWARM,"MICRO SATELLITE CRUSH"},
    {MINES,THUNDER_CHAIN,"TESLA TRIPWIRE NETWORK"},
    {RAILGUN,CRYO_SHATTER,"GLACIER RAIL"},
    {RADIATION,ELECTRIC_AURA,"ION RADIATION PLAGUE"},
    {BOUNCY_BOMBS,DX_NAPALM_WAVE,"HELLBOUND THERMITE"},
    {RICOCHET_DISCS,PIERCING_RAILS,"PRISM RICOCHET"},
    {PULSE_DRONES,DRONE_DETONATION,"DRONE APOCALYPSE"},
    {ICE_BOMBS,SHOCKWAVE,"ABSOLUTE IMPACT"},
    {ARC_GRENADES,CRYO_SHATTER,"FLASH FREEZE THUNDER"}
};
// DXV39: genuine cross-system fusions; require investment in BOTH components.
static Evolution const v39Fusions[]={
    {70,120,"BLOODHIVE REVOLUTION"},
    {72,136,"ABSOLUTE STORM CORE"},
    {75,159,"ELECTROFREEZE FANG"},
    {76,161,"RAPTOR BLADE CATHEDRAL"},
    {81,123,"WALLBREAKER CANNON"},
    {90,143,"SAINTS OF STEEL"},
    {95,135,"THUNDER TWINS"},
    {100,138,"TOXIC BOUNTY CLOUD"},
    {105,162,"GRAVITY HAMMER"},
    {110,124,"LIGHTNING PANDEMIC"},
    {112,150,"SHATTERGLASS TRIPWEB"},
    {114,164,"NAPALM BLOSSOM"},
};
static char const* EvolutionName(Fighter const& f,int weapon){
    for(auto const &e:evolutions)
        if(e.weapon==weapon && f.skill[weapon]>=5 && f.skill[e.passive]>=(weapon<7?3:2))
            return e.name;
    for(auto const&e:v39Fusions)
        if(e.weapon==weapon && f.skill[weapon]>=4 && f.skill[e.passive]>=2)
            return e.name;
    return "";
}
static void GrantXP(int seat,int base){
    if(!Alive(seat)||base<=0)return;
    auto &f=fighters[seat];
    int amount=base + base*f.skill[13]/5 + base*f.skill[LOOT]/10 + base*f.skill[XP_FEVER]/6 + base*Min(4,f.combo/10) + base*f.skill[RECYCLER]/3 + (numplayers==1?base*careerRank/15:0);
    // V38: XP-related passive modules; all apply to real earned XP, not HUD cosmetics.
    amount+=base*f.skill[138]/4;
    amount+=base*(int)activeBoons[1]/3; // prosperity boon
    amount=Max(1,amount-base*(int)activeBanes[1]/5); // famine bane
    amount+=base*(int)activeBoons[7]/7;
    amount=Max(1,amount-base*(int)activeBanes[7]/8);
    f.xp+=Max(1,amount);
    while(f.level<kMaxLevel && f.xp>=f.threshold){
        f.xp-=f.threshold;
        ++f.level;++f.unspent;
        f.threshold=7 + f.level*4 + (f.level*f.level)/10;
    }
}
static bool ChooseUpgrade(int seat,int choice){
    if(!Alive(seat)||choice<1||choice>3)return false;
    Fighter &f=fighters[seat];
    if(f.unspent<=0 || !offerReady)return false;
    int id=Choice(f,choice-1);
    if(id<0 || !CanOfferSkill(f,id)){
        // Maxed choices convert to guaranteed survivability; no lost levels.
        auto &hp=sprite[g_player[seat].ps->i].extra;
        hp=Min(250,hp+15);
    } else AcquireItem(f,id,activeOfferRarity[choice-1]);
    --f.unspent;
    offerReady=false;for(int &card:activeOffer)card=-1;
    return true;
}
static void SyncRunState(){
    if (!initialized || numplayers != 1 || myconnectindex < 0 ||
        myconnectindex >= MAXPLAYERS || !g_player[myconnectindex].ps) return;
    auto p=g_player[myconnectindex].ps;
    // Do not treat opening the ordinary menu as death.
    const bool dead=(p->i>=0 && p->i<MAXSPRITES && sprite[p->i].extra<=0);
    if(dead){sawPlayerDeath=true;return;}
    if(!(p->gm&MODE_GAME) || p->cursectnum<0)return;
    if(sawPlayerDeath || episode!=ud.volume_number || level!=ud.level_number ||
       (lastPar>=0 && p->player_par<lastPar)){
        Reset();
        episode=ud.volume_number;level=ud.level_number;
        initialized=true;
        lastPar=p->player_par;
        for(bssize_t TRAVERSE_CONNECT(i))ApplyLoadout(i);
        sawPlayerDeath=false;
        ++runGeneration;
        std::printf("DXV18: respawn synchronized, cleared stale level-up pause\n");
    }
}
static bool TakeRestartRequest(){
    if(numplayers!=1 || !restartRequested)return false;
    restartRequested=0;
    // Force the next survivor tick to discard the previous run's equipment.
    sawPlayerDeath=true;
    return true;
}
static bool HasPendingUpgrade(){
    SyncRunState();
    if((chestReveal.seat>=0 || twistChoice.seat>=0) && numplayers==1)return true;
    if(autoMode && numplayers==1 && initialized && Alive(myconnectindex) &&
       fighters[myconnectindex].unspent>0){
        if(BeginOffer(myconnectindex)){
            // Progress with 1 of 3 legitimate weighted cards, never inject ranks.
            int best=0,score=-1;
            for(int c=0;c<3;++c){
                int id=Choice(fighters[myconnectindex],c);
                if(id<0)continue;
                int candidate=(IsWeaponSkill(id)?8:3) +
                    (fighters[myconnectindex].skill[id]>0?12:0)+
                    (id>=NEXUS_DARTS && id<=TESLA_MESH?5:0);
                if(candidate>score){best=c+1;score=candidate;}
            }
            if(best)ChooseUpgrade(myconnectindex,best);
        }
    }
    // Single-player gameplay freezes at the original world-simulation seam
    // while the user picks a power. Netgames are not paused or desynced.
    return numplayers==1 && initialized && myconnectindex>=0 &&
        myconnectindex<MAXPLAYERS && Alive(myconnectindex) &&
        fighters[myconnectindex].unspent>0;
}
static int CountEnemies(){
    int n=0;
    for(int j=0;j<MAXSPRITES;++j){
        auto const &e=sprite[j];
        if((e.statnum==STAT_ACTOR || e.statnum==STAT_ZOMBIEACTOR) && e.extra>0 && IsSurvivorEnemy(j))++n;
    }
    return n;
}
static int NearestEnemyAngle(int seat,int range,int *idOut=nullptr){
    if(!Alive(seat))return kNoTarget;
    auto const &p=sprite[g_player[seat].ps->i];
    int best=-1;
    int64_t dist=static_cast<int64_t>(range)*range;
    for(int j=0;j<MAXSPRITES;++j){
        auto const &e=sprite[j];
        if((e.statnum!=STAT_ACTOR && e.statnum!=STAT_ZOMBIEACTOR)||e.extra<=0||!IsSurvivorEnemy(j))continue;
        int64_t dx=static_cast<int64_t>(e.x)-p.x,dy=static_cast<int64_t>(e.y)-p.y;
        int64_t d=dx*dx+dy*dy;
        if(d<dist){best=j;dist=d;}
    }
    if(idOut)*idOut=best;
    return best<0?kNoTarget:getangle(sprite[best].x-p.x,sprite[best].y-p.y);
}
// DXV35: tactical roaming in the real Build simulation. Objectives are
// accessible XP, restorative caches, combat engagement and unexplored sectors;
// candidates account for wall clearance, crowd density and recent revisits.
// Deterministic for a solo run, no JS position spoofing or teleportation.
static int AutoVisitCell(int x,int y,int16_t sec){
    uint32_t h=(uint32_t)(x/1800)*73856093u ^ (uint32_t)(y/1800)*19349663u ^
               (uint32_t)Max(0,sec)*83492791u;
    return (int)(h&255);
}
static bool AutoClear(int x,int y,int z,int16_t sec,int xx,int yy,int *clearance=nullptr){
    if(sec<0 || sec>=numsectors)return false;
    int16_t end=sec;updatesector(xx,yy,&end);
    if(end<0||end>=numsectors)return false;
    // A wall hitscan is intentionally used instead of treating a sector-XY
    // membership test as a navigation path. Reject blocked portals and cliffs.
    vec3_t origin{x,y,z-(16<<8)};hitdata_t hit{};
    hitscan(&origin,sec,xx-x,yy-y,0,&hit,CLIPMASK1&0xffff);
    const int64_t intended=(int64_t)(xx-x)*(xx-x)+(int64_t)(yy-y)*(yy-y);
    int available=(int)std::sqrt((double)intended);
    if(hit.sect>=0 && hit.wall>=0){
        int64_t dx=(int64_t)hit.x-x,dy=(int64_t)hit.y-y;
        int actual=(int)std::sqrt((double)(dx*dx+dy*dy));
        if(actual<available)available=actual;
    }
    if(clearance)*clearance=available;
    if(available<Max(256,(int)std::sqrt((double)intended)-220))return false;
    const int floor=getflorzofslope(end,xx,yy);
    const int hereFloor=getflorzofslope(sec,x,y);
    const int ceiling=getceilzofslope(end,xx,yy);
    if(floor-ceiling<(43<<8))return false; // too narrow for standing Duke
    return floor>=hereFloor-(64<<8); // reject impassable upward steps
}
// DXV37_NATIVE_AUTOPILOT_VISION: movement, facing and vertical eye motion are
// three separate plans. World probes, NPC/pickup decisions and look targets
// are evaluated at 30Hz simulation steps; the real P_GetInput consumes view
// deltas every input sample, not teleports/rotations of player sprites.
static int AutoAngleDiff(int target,int from){
    return ((target-from+1024)&2047)-1024;
}
static int AutoTargetHorizon(int eyeZ,int targetZ,int range){
    // Duke's 16.16 horizon is centered at 100: <100 looks up; >100 down.
    // Build Z units are finer than XY units. Limit pitch to keep the third-
    // person camera readable in close-range combat / vertical elevators.
    int64_t rise=(int64_t)(targetZ-eyeZ)*20;
    return 100+Clamp((int)(rise/Max(950,range)),-65,65);
}
static void AutoThink(int seat){
    if(!Alive(seat))return;
    auto &brain=autoBrain;
    auto const &p=sprite[g_player[seat].ps->i];
    const int x=p.x,y=p.y,z=p.z;
    const int face=(g_player[seat].ps->q16ang>>16)&2047;
    if(gameTicks-brain.lastCheck>=18){
        int64_t dx=(int64_t)x-brain.lastX,dy=(int64_t)y-brain.lastY;
        // Only count failed progress while an actual travel command exists.
        if(brain.forward || brain.strafe){
            if(dx*dx+dy*dy<240LL*240)brain.stuck=Min(12,brain.stuck+1);
            else brain.stuck=Max(0,brain.stuck-2);
        }
        brain.lastX=x;brain.lastY=y;brain.lastCheck=gameTicks;
        if(brain.stuck>=2){
            if(brain.goal>=0 && brain.goalKind<=2){
                brain.rejectedGoal=brain.goal;brain.rejectedUntil=gameTicks+Min(600,120+brain.stuck*55);++brain.failedGoals;
            }
            brain.blockedAngle=brain.desired;
            brain.blockedUntil=gameTicks+72;
        }
    }
    if(gameTicks<brain.nextThink)return;
    brain.nextThink=gameTicks+6;
    ++autoDecisions;
    const int hp=sprite[g_player[seat].ps->i].extra;
    brain.visits[AutoVisitCell(x,y,p.sectnum)]=
        (uint16_t)Min(65000,(int)brain.visits[AutoVisitCell(x,y,p.sectnum)]+4);
    if(gameTicks%240==0)
        for(auto &v:brain.visits)v=(uint16_t)(v*3/4);

    int enemyIds[72],enemyCount=0,nearDanger=0,nearest=-1,lookTarget=-1;
    int hostileShots[48],hostileShotCount=0;
    int64_t nearestDistance=20000LL*20000, bestLook=INT64_MAX;
    for(int i=0;i<MAXSPRITES;++i){
        // Projectile threat is sampled once per tactical update, never once
        // per candidate path. Friendly shots cannot panic the navigator.
        const auto &shot=sprite[i];
        if(shot.statnum>=0 && shot.owner!=g_player[seat].ps->i &&
           (shot.picnum==RPG || shot.picnum==FREEZEBLAST ||
            shot.picnum==SHRINKSPARK || shot.picnum==FIRELASER) &&
           hostileShotCount<48){
            int64_t sx=(int64_t)shot.x-x,sy=(int64_t)shot.y-y;
            if(sx*sx+sy*sy<7200LL*7200)hostileShots[hostileShotCount++]=i;
        }
        if(!IsSurvivorEnemy(i) || sprite[i].extra<=0 ||
           (sprite[i].statnum!=STAT_ACTOR && sprite[i].statnum!=STAT_ZOMBIEACTOR))continue;
        auto const &e=sprite[i];
        int64_t dx=(int64_t)e.x-x,dy=(int64_t)e.y-y,d2=dx*dx+dy*dy;
        // Unseen enemies behind walls are not straight-line travel goals.
        // Explore reachable corridors instead of pushing into the same wall.
        const bool seen=cansee(x,y,z-(20<<8),p.sectnum,e.x,e.y,e.z-(16<<8),e.sectnum);
        if(seen && d2<nearestDistance){nearestDistance=d2;nearest=i;}
        if(d2<6200LL*6200 && enemyCount<72)enemyIds[enemyCount++]=i;
        if(d2<1950LL*1950)++nearDanger;
        if(d2<13500LL*13500 && d2<bestLook &&
           cansee(x,y,z-(20<<8),p.sectnum,e.x,e.y,e.z-(16<<8),e.sectnum)){
            // Real line of sight excludes threats behind walls from visual aim.
            bestLook=d2;lookTarget=i;
        }
    }
    // XP is never just a magnet effect: physically travel to the drop when
    // collecting it has meaningful value. Avoid retarget spam by preserving
    // the last objective with a modest score bonus.
    int nextGoal=-1,nextKind=0;
    double bestGoal=1e30;
    for(auto const &gem:gems)if(gem.id>=0 && gem.id<MAXSPRITES &&
        sprite[gem.id].statnum==STAT_FX){
        auto const &g=sprite[gem.id];
        int64_t dx=(int64_t)g.x-x,dy=(int64_t)g.y-y,d2=dx*dx+dy*dy;
        if(d2>19000LL*19000 || (gem.id==brain.rejectedGoal && gameTicks<brain.rejectedUntil))continue;
        if(!cansee(x,y,z-(20<<8),p.sectnum,g.x,g.y,g.z,g.sectnum))continue;
        double value=Max(3,Min(60,gem.value));
        double score=(double)d2/(value*13.0);
        if(gem.id==brain.goal && brain.goalKind==1)score*=0.79;
        if(hp<35 && nearDanger>=2)score*=1.6;
        if(score<bestGoal){bestGoal=score;nextGoal=gem.id;nextKind=1;}
    }
    for(auto const &cache:caches)if(cache.id>=0 && cache.id<MAXSPRITES &&
        sprite[cache.id].statnum==STAT_FX){
        auto const &g=sprite[cache.id];
        int64_t dx=(int64_t)g.x-x,dy=(int64_t)g.y-y,d2=dx*dx+dy*dy;
        if(d2>22000LL*22000 || (cache.id==brain.rejectedGoal && gameTicks<brain.rejectedUntil))continue;
        if(!cansee(x,y,z-(20<<8),p.sectnum,g.x,g.y,g.z,g.sectnum))continue;
        const int value=(cache.kind==1 && hp<65)?250:55;
        double score=(double)d2/value;
        if(cache.id==brain.goal && brain.goalKind==2)score*=0.8;
        if(score<bestGoal){bestGoal=score;nextGoal=cache.id;nextKind=2;}
    }
    const int previousGoalKind=brain.goalKind;
    brain.goal=nextGoal;brain.goalKind=nextKind;
    if(nextGoal>=0){
        brain.goalX=sprite[nextGoal].x;brain.goalY=sprite[nextGoal].y;
    }else if(nearest>=0){
        auto const &e=sprite[nearest];
        // Threat-dependent stand-off distance; nearby threats invoke a kite.
        int const a=getangle(e.x-x,e.y-y);
        int const standoff=(nearDanger>=2||hp<35)?5300:3100;
        brain.goalX=e.x-(int)((int64_t)sintable[(a+512)&2047]*standoff/16384);
        brain.goalY=e.y-(int)((int64_t)sintable[a]*standoff/16384);
        brain.goalKind=3;
    }else{
        if(gameTicks>=brain.roamChange || brain.stuck>=3 || previousGoalKind!=4){
            // Sweep the map in changing arcs; visits and physical probes pick
            // reachable ground rather than holding a random fixed heading.
            brain.wanderAngle=(brain.wanderAngle+341+(Random()&255))&2047;
            brain.roamChange=gameTicks+100; ++autoExplorations;
        }
        brain.goalX=x+(int)((int64_t)sintable[(brain.wanderAngle+512)&2047]*14500/16384);
        brain.goalY=y+(int)((int64_t)sintable[brain.wanderAngle]*14500/16384);
        brain.goalKind=4;
    }
    // Two look-ahead distances detect blind walls and dangerous corners.
    // Objective progress, enemy pressure and remembered blocked headings are
    // evaluated in a single local 16-direction planner. At most 32 world
    // probes every six ticks, independent of map dimensions.
    double best=-1e30;
    int bestAngle=face,bestRange=0,probed=0;
    for(int k=0;k<16;++k){
        const int angle=(face+k*128)&2047;
        const int nx=(int)((int64_t)sintable[(angle+512)&2047]*2000/16384);
        const int ny=(int)((int64_t)sintable[angle]*2000/16384);
        const int cx=x+nx,cy=y+ny;
        int clearance=0;
        if(!AutoClear(x,y,z,p.sectnum,cx,cy,&clearance))continue;
        ++probed;
        int16_t cs=p.sectnum;updatesector(cx,cy,&cs);
        const int farX=x+nx*2,farY=y+ny*2;
        int farClearance=0;
        bool farOpen=AutoClear(x,y,z,p.sectnum,farX,farY,&farClearance);
        int64_t gx=(int64_t)cx-brain.goalX,gy=(int64_t)cy-brain.goalY;
        double score=-std::sqrt((double)(gx*gx+gy*gy))*0.42;
        score-=brain.visits[AutoVisitCell(cx,cy,cs)]*30.;
        score+=Min(2100,clearance)*0.29;
        score+=farOpen?1250.:-780.;
        // Ease steering through corners while not becoming locked to walls.
        int turn=AutoAngleDiff(angle,brain.desired);
        score-=std::abs(turn)*0.28;
        if(brain.blockedAngle>=0 && gameTicks<brain.blockedUntil &&
           std::abs(AutoAngleDiff(angle,brain.blockedAngle))<190)
            score-=6800.;
        for(int i=0;i<enemyCount;++i){
            auto const &e=sprite[enemyIds[i]];
            int64_t dx=(int64_t)e.x-cx,dy=(int64_t)e.y-cy,d2=dx*dx+dy*dy;
            if(d2<3900LL*3900){
                score-=hp<40?2700.:1250.;
                if(d2<1400LL*1400)score-=5400.;
            }
        }
        for(int i=0;i<hostileShotCount;++i){
            const auto &shot=sprite[hostileShots[i]];
            int64_t sx=(int64_t)shot.x-cx,sy=(int64_t)shot.y-cy;
            if(sx*sx+sy*sy<2100LL*2100)score-=3700.;
        }
        // Prefer pickups and active engagement, not perpetual retreats.
        if(brain.goalKind==1 || brain.goalKind==2)score+=950.;
        if(score>best){best=score;bestAngle=angle;bestRange=farOpen?4000:2000;}
    }
    if(probed==0){
        bestAngle=(face+512+brain.stuck*161)&2047;
        ++autoAvoidance;
    }
    if(nearDanger>=2 || brain.stuck>=2)++autoAvoidance;
    brain.desired=bestAngle;brain.plannedRange=bestRange;
    const int diff=AutoAngleDiff(bestAngle,face);
    // Move relative to the current orientation. View turning comes from the
    // input packet below; do not rotate/snap sprite angles behind the engine.
    brain.forward=Clamp((sintable[(diff+512)&2047]*87)/16384,-90,90);
    brain.strafe=Clamp((sintable[diff&2047]*87)/16384,-90,90);

    // Vision planner: aim at a visible dangerous enemy while moving toward an
    // XP pickup or through a doorway. Otherwise look toward the destination.
    brain.aimTarget=-1;brain.aimKind=0;
    int targetX=brain.goalX,targetY=brain.goalY,targetZ=z-(20<<8);
    if(lookTarget>=0){
        auto const &e=sprite[lookTarget];
        targetX=e.x;targetY=e.y;targetZ=e.z-(16<<8);
        brain.aimTarget=lookTarget;brain.aimKind=1;
    }else if(brain.goal>=0 && brain.goal<MAXSPRITES){
        auto const &g=sprite[brain.goal];
        targetX=g.x;targetY=g.y;targetZ=g.z-(5<<8);
        brain.aimTarget=brain.goal;brain.aimKind=2;
    }
    int64_t dx=(int64_t)targetX-x,dy=(int64_t)targetY-y;
    const int distance=(int)std::sqrt((double)(dx*dx+dy*dy));
    brain.aimAngle=(distance>128)?getangle((int)dx,(int)dy):brain.desired;
    brain.aimHorizon=(distance>128)?AutoTargetHorizon(z-(20<<8),targetZ,distance):100;
    if(brain.goalKind==4 && brain.aimTarget<0){
        // A moving head scan exposes side corridors and vertical sightlines
        // during exploration; navigation and world movement remain independent.
        brain.aimAngle=(brain.desired+(sintable[(gameTicks*8)&2047]*250)/16384)&2047;
        brain.aimHorizon=100+(sintable[(gameTicks*5)&2047]*12)/16384;
    }
}
static void AutoApplyLook(int playerNum,input_t *input){
    auto &brain=autoBrain;
    auto const ps=g_player[playerNum].ps;
    auto const &body=sprite[ps->i];
    // Follow moving targets between tactical decisions; an expired ID never
    // influences the camera. A new manual right-stick input takes priority.
    int aim=brain.aimAngle, horizon=brain.aimHorizon;
    if(brain.aimTarget>=0 && brain.aimTarget<MAXSPRITES){
        auto const &t=sprite[brain.aimTarget];
        const bool valid=brain.aimKind==1?
            ((t.statnum==STAT_ACTOR||t.statnum==STAT_ZOMBIEACTOR)&&t.extra>0&&IsSurvivorEnemy(brain.aimTarget)):
            (brain.aimKind==2&&t.statnum==STAT_FX);
        if(valid){
            int64_t dx=(int64_t)t.x-body.x,dy=(int64_t)t.y-body.y;
            int distance=(int)std::sqrt((double)(dx*dx+dy*dy));
            if(distance>128){
                aim=getangle((int)dx,(int)dy);
                horizon=AutoTargetHorizon(body.z-(20<<8),t.z-(brain.aimKind==1?16:5)*(1<<8),distance);
            }
        }
    }
    const int face=(ps->q16ang>>16)&2047;
    int turn=AutoAngleDiff(aim,face);
    // Real angle delta in the original input packet, with damping to avoid
    // oscillations. No JS camera patch or direct player q16ang overwrite.
    turn=Clamp(turn/5,-29,29);
    if(turn==0 && std::abs(AutoAngleDiff(aim,face))>=3)
        turn=AutoAngleDiff(aim,face)>0?1:-1;
    input->q16avel=fix16_sadd(input->q16avel,fix16_from_int(turn));
    const int current=ps->q16horiz>>16;
    int pitch=Clamp((horizon-current)/2,-9,9);
    if(pitch==0 && std::abs(horizon-current)>=2)pitch=horizon>current?1:-1;
    input->q16horz=fix16_sadd(input->q16horz,fix16_from_int(pitch));
}

static void BeforeInput(int seat){
    if(!Alive(seat))return;
    auto p=g_player[seat].ps;
    p->over_shoulder_on=1; // EDuke32's G_DoThirdPerson uses wall hitscan.
    p->auto_aim=3;
    // Survivors has no manual ammo economy. Keep original weapon animations,
    // but maintain ammunition for the existing Duke weapon pipeline.
    // Infinite ammo for every owned weapon, not just the equipped slot.
    for(int weapon=0;weapon<MAX_WEAPONS;++weapon)
        if((p->gotweapon & (1u<<weapon)) && p->max_ammo_amount[weapon]>0)
            p->ammo_amount[weapon]=p->max_ammo_amount[weapon];
    auto &in=g_player[seat].input;
    // Consume old weapon-selection buttons as upgrade commands. This is done
    // from original synced input rather than changing client-only HUD state.
    int choice=(in.bits&SK_WEAPON_MASK)>>SK_WEAPON_BITS;
    if(choice>=1 && choice<=3 && fighters[seat].unspent)ChooseUpgrade(seat,choice);
    in.bits &= ~SK_WEAPON_MASK;
    in.bits |= BIT(SK_FIRE);
    // Velocity passive modifies the original per-tick input; clipmove, slopes,
    // conveyors, elevators and physics still run in Duke's P_ProcessInput.
    int const boost=100+fighters[seat].skill[17]*6;
    in.fvel=Clamp(static_cast<int>(in.fvel)*(boost+fighters[seat].skill[144]*5)/100,-90,90);
    in.svel=Clamp(static_cast<int>(in.svel)*(boost+fighters[seat].skill[144]*5)/100,-90,90);
    // Normal Build movement/collision remains owned by P_ProcessInput.
    // No direct camera rotation during player motion: prevents broken WASD.
}
static void SpawnSupply(int seat,int kind=0){
    if(!OwnWorld()||!Alive(seat))return;
    int free=-1;for(int i=0;i<32;++i)if(caches[i].id<0){free=i;break;}
    if(free<0)return;
    auto const& p=sprite[g_player[seat].ps->i];
    const int ang=Random()&2047,dist=900+static_cast<int>(RandomBounded(1900));
    const int x=p.x+static_cast<int>(static_cast<int64_t>(sintable[(ang+512)&2047])*dist/16384);
    const int y=p.y+static_cast<int>(static_cast<int64_t>(sintable[ang])*dist/16384);
    int16_t sec=p.sectnum;updatesector(x,y,&sec);
    if(sec<0||sec>=numsectors)return;
    int tile=kind==1?EditionTile(ATOMICHEALTH,CacheTile()):
        kind==2?EditionTile(AMMO,CacheTile()):
        kind==3?EditionTile(RPGAMMO,CacheTile()):CacheTile();
    int repeat=ArtRepeat(tile,42);
    const int id=A_InsertSprite(sec,x,y,p.z-(13<<8),tile,-20,repeat,repeat,0,0,0,-1,STAT_FX);
    if(id<0)return;
    sprite[id].pal=8;sprite[id].shade=-30;
    caches[free].id=id;caches[free].age=0;caches[free].tile=tile;caches[free].kind=kind;
}
// V46 boss and elite trash-can chests: native Build sprites, no HTML fake.
static bool SpawnChestAt(Drop const& death){
    if(!OwnWorld() || death.sectorId<0 || death.sectorId>=numsectors)return false;
    Cache *free=nullptr;
    for(auto &c:caches)if(c.id<0){free=&c;break;}
    if(!free)return false;
    const int tile=V47Art(0,EditionTile(CANWITHSOMETHING,TRASH));
    int id=A_InsertSprite((int16_t)death.sectorId,death.x,death.y,death.z-(4<<8),
        tile,-12,ArtRepeat(tile,54),ArtRepeat(tile,54),0,0,0,-1,STAT_FX);
    if(id<0)return false;
    sprite[id].pal=0;sprite[id].shade=-24;
    sprite[id].cstat |= 256; // CSTAT_SPRITE_BLOCK_HITSCAN: chest can be shot without blocking passage
    free->id=id;free->age=0;free->tile=tile;free->kind=5;free->vz=0;
    ++chestsSpawned;return true;
}
// DXV48: Rare world collectible, always distinct from a chest (kind 5).
// The original Duke crystal sprite and palette are used; no purchased assets.
static bool SpawnXPVacuum(Drop const& at){
    if(numplayers!=1 || !OwnWorld() || at.sectorId<0 || at.sectorId>=numsectors)return false;
    Cache* free=nullptr;
    for(auto& c:caches)if(c.id<0){free=&c;break;}
    if(!free)return false;
    const int tile=GemTile();
    const int id=A_InsertSprite((int16_t)at.sectorId,at.x,at.y,at.z-(5<<8),
                               tile,-28,ArtRepeat(tile,38),ArtRepeat(tile,38),
                               0,0,0,-1,STAT_FX);
    if(id<0)return false;
    sprite[id].cstat=0;
    sprite[id].pal=7;
    sprite[id].shade=-30;
    free->id=id;free->age=0;free->tile=tile;free->kind=6;free->vz=0;
    ++xpVacuumDrops;
    return true;
}
static bool BeginXPVacuum(int seat){
    if(numplayers!=1 || !OwnWorld() || !Alive(seat))return false;
    xpVacuum.seat=seat;
    xpVacuum.ticks=kVacuumDurationTicks;
    xpVacuum.collected=0;
    ++xpVacuum.triggerCount;
    return true;
}
static int XPClampStep(int64_t d,int divisor,int limit){
    // 64-bit intermediate protects distant sectors from integer overflow.
    int64_t scaled=(int64_t)d/divisor;
    if(scaled>limit)scaled=limit;
    if(scaled<-limit)scaled=-limit;
    return (int)scaled;
}
static int ChestFusionRecipe(Fighter const& f,uint32_t draw){
    const int a=(int)(sizeof(evolutions)/sizeof(evolutions[0]));
    const int b=(int)(sizeof(v39Fusions)/sizeof(v39Fusions[0]));
    int valid[64]={},count=0;
    for(int k=0;k<a+b && count<64;++k){
        auto const& recipe=k<a?evolutions[k]:v39Fusions[k-a];
        const int weapon=recipe.weapon,other=recipe.passive;
        if(weapon<0 || weapon>=kSkillCount || other<0 || other>=kSkillCount)continue;
        if(f.skill[weapon]<3 || f.skill[other]<2 || f.fused[weapon] ||
           f.absorbed[other] || f.absorbed[weapon])continue;
        valid[count++]=k;
    }
    return count?valid[draw%(unsigned)count]:-1;
}
// When no named recipe is available, ANY two acquired non-absorbed items
// can link via this deterministic hybrid mechanism (weapon, passive or perk).
// Authored named fusions retain priority and their old special behaviors.
static constexpr int kHybridFusionBase=2048;
static int GenericFusionPair(Fighter const& f,uint32_t draw){
    int first[kSkillCount]={},n=0;
    for(int i=0;i<kSkillCount;++i)if(f.skill[i]>=2 && !f.fused[i] && !f.absorbed[i])
        first[n++]=i;
    if(n<2)return -1;
    int a=(int)(draw%(uint32_t)n),b=(a+1+(int)((draw>>16)%(uint32_t)(n-1)))%n;
    return kHybridFusionBase+first[a]*kSkillCount+first[b];
}
static void StartChestReveal(int seat){
    if(!Alive(seat) || chestReveal.seat>=0)return;
    Fighter &f=fighters[seat];
    Fighter preview=f; // reserve capacity as we roll; never silently discard a jackpot slot
    chestReveal=ChestReveal{};
    chestReveal.seat=seat;
    // Prefer the player's own Duke trash-can art when available;
    // fall back to the bundled masked can only when the GRP lacks it.
    chestReveal.icon=EditionTile(CANWITHSOMETHING,V47Art(0,TRASH));
    chestReveal.count=DukeXV46::ChestRewardCount(RandomBounded(10000),1);
    bool chosen[kSkillCount]={};
    for(int j=0;j<chestReveal.count;++j){
        int recipe=(RandomBounded(100)<46)?ChestFusionRecipe(preview,Random()):-1;
        if(recipe<0 && RandomBounded(100)<22)recipe=GenericFusionPair(preview,Random());
        if(recipe>=kHybridFusionBase){
            int primary=(recipe-kHybridFusionBase)/kSkillCount;
            if(!chosen[primary]){
                chestReveal.reward[j]=recipe;chestReveal.fusion[j]=true;
                chestReveal.rarity[j]=(uint8_t)Min(5,(int)f.rarity[primary]+1);
                chosen[primary]=true;
                int partner=(recipe-kHybridFusionBase)%kSkillCount;
                preview.fused[primary]=1;preview.absorbed[partner]=1;
                continue;
            }
        }
        if(recipe>=0 && recipe<kHybridFusionBase){
            const int base=(int)(sizeof(evolutions)/sizeof(evolutions[0]));
            auto const& e=recipe<base?evolutions[recipe]:v39Fusions[recipe-base];
            if(!chosen[e.weapon]){
                chestReveal.reward[j]=recipe;
                chestReveal.fusion[j]=true;
                chestReveal.rarity[j]=(uint8_t)Min(5,(int)f.rarity[e.weapon]+1);
                chosen[e.weapon]=true;
                preview.fused[e.weapon]=1;preview.absorbed[e.passive]=1;
                continue;
            }
        }
        int options[kSkillCount],total=0;
        int upgrades[kSkillCount],upCount=0;
        int news[kSkillCount],newCount=0;
        for(int id=0;id<kSkillCount;++id){
            if(chosen[id] || !CanOfferSkill(preview,id))continue;
            options[total++]=id;
            if(preview.skill[id]>0)upgrades[upCount++]=id;
            else news[newCount++]=id;
        }
        if(!total){chestReveal.count=j;break;}
        int id=-1;
        if(upCount>0 && newCount>0){
            // Chest loot should feel rewarding: weight toward upgrades while
            // still surfacing NEW builds when the player has room for them.
            id=(RandomBounded(100)<58)
                ? upgrades[RandomBounded((uint32_t)upCount)]
                : news[RandomBounded((uint32_t)newCount)];
        }else if(upCount>0) id=upgrades[RandomBounded((uint32_t)upCount)];
        else if(newCount>0) id=news[RandomBounded((uint32_t)newCount)];
        else id=options[RandomBounded((uint32_t)total)];
        chestReveal.reward[j]=id;
        chestReveal.rarity[j]=(uint8_t)DukeXV46::RollRarity(RandomBounded(10000),f.skill[18]+2);
        chosen[id]=true;
        preview.skill[id]=Min(MaxRank(id),preview.skill[id]+1);
    }
    ++chestsOpened;
}
static void FinishChestReveal(){
    int seat=chestReveal.seat;
    if(!Alive(seat)){chestReveal=ChestReveal{};return;}
    Fighter &f=fighters[seat];
    int const fusionBase=(int)(sizeof(evolutions)/sizeof(evolutions[0]));
    for(int j=0;j<chestReveal.count;++j){
        int id=chestReveal.reward[j];if(id<0)continue;
        if(chestReveal.fusion[j]){
            if(id>=kHybridFusionBase){
                int primary=(id-kHybridFusionBase)/kSkillCount;
                int partner=(id-kHybridFusionBase)%kSkillCount;
                if(TryFuse(f,primary,partner))lastCacheAward=primary;
            }else{
                auto const& recipe=id<fusionBase?evolutions[id]:v39Fusions[id-fusionBase];
                if(TryFuse(f,recipe.weapon,recipe.passive))lastCacheAward=recipe.weapon;
            }
        }else if(CanOfferSkill(f,id)){
            AcquireItem(f,id,chestReveal.rarity[j]);lastCacheAward=id;
        }
    }
    // Super-rare independent choice, not silently awarded or hidden in the
    // ordinary 170-item armory. The player can choose any of three tradeoffs.
    if(RandomBounded(10000)<140 && numplayers==1){
        twistChoice=TwistChoice{};twistChoice.seat=seat;
        for(int i=0;i<3;++i){
            auto t=DukeXV46::RollTwist(Random(),Random());
            // Ensure distinct types; make at least one boon available.
            if(i==0)t.bane=false;
            for(int j=0;j<i;++j)if(t.kind==twistChoice.cards[j].kind)
                t.kind=(t.kind+i+j+1)&7;
            twistChoice.cards[i]=t;
        }
    }
    cacheBannerUntil=gameTicks+5*kHz;
    chestReveal=ChestReveal{};
}
static bool ChooseTwist(int n){
    if(n<1||n>3||twistChoice.seat<0)return false;
    auto const t=twistChoice.cards[n-1];
    uint8_t &rank=t.bane?activeBanes[t.kind]:activeBoons[t.kind];
    rank=(uint8_t)Min(9,(int)rank+t.strength);
    lastBoon=t.kind+(t.bane?8:0);lastBoonStrength=t.strength;
    lastBoonUntil=gameTicks+kHz*13;
    twistChoice=TwistChoice{};return true;
}
static void DrawTwistSelection(){
    if(twistChoice.seat<0)return;
    rotatesprite_fs(160<<16,98<<16,90000L,(twistChoice.frames*8)&2047,
                    EditionTile(CANWITHSOMETHING,TRASH),-30,((twistChoice.frames/5)%7),2|8|64);
    printext256(67,39,14,0,"RARE RUN MUTATION!",1);
    printext256(40,53,15,0,"CHOOSE YOUR BOON OR BANE",1);
    for(int i=0;i<3;++i){
        auto const t=twistChoice.cards[i];
        char msg[100];std::snprintf(msg,sizeof(msg),"%d  %s: %s  x%d",i+1,
            t.bane?"BANE":"BOON",twistNames[t.kind],t.strength);
        printext256(45,132+i*16,t.bane?2:14,0,msg,1);
    }
    ++twistChoice.frames;
    // On mobile, touch buttons call the native ChooseTwist export; for desktop
    // keyboard choices are handled by engine-level upgrade key dispatch.
    if(twistChoice.frames>kHz*25)ChooseTwist(1);
}
// DXV48_3: TRASH_CAN_REAL_ART_SMALL_LEFT_HUD
// DXV47_JACKPOT: a native Build HUD reveal, deliberately independent from
// Emscripten DOM. Own masked indexed ART, paused simulation, bounded visual
// sprites and deterministic animation positions. No extra world sprite pool.
static void ChestArt(int x,int y,int scale,int angle,int art,int shade=0,int pal=0){
    const int tile=V47Art(art);
    rotatesprite_fs(x<<16,y<<16,scale,angle&2047,tile,shade,pal,2|8|64);
}
static int ChestIconFor(int reward,bool fusion){
    if(fusion)return V47Art(6);
    if(reward>=70 && reward<170)return V39Art(reward);
    switch(reward){
      case PISTOL:case SHOTGUN: return SHOTSPARK1;
      case ROCKET:case DEVASTATOR:case ROCKET_CYCLONE:return RPG;
      case FREEZE:case CRYO_SHATTER:return FREEZEBLAST;
      case SHRINK:case SHRINK_SWARM:return SHRINKSPARK;
      case LASER:case RAILGUN:case PIERCING_RAILS:return FIRELASER;
      default:return GemTile();
    }
}
static void DrawChestReveal(){
    if(chestReveal.seat<0)return;
    const int tick=++chestReveal.frames;
    const int count=Clamp(chestReveal.count,0,5);
    // Original translucent full-viewport tile: the level is still visible
    // behind the dimmer, unlike displaying an opaque menu background.
    if(HasArt(kV47ArtBase+12))
        rotatesprite_fs(160<<16,100<<16,65536L,0,V47Art(12),0,0,2|8|64);
    const int accent=count>=5?14:count>=3?7:15;
    printext256(72,16,accent,0,"TRASH JACKPOT",1);
    printext256(71,29,15,0,"ELITE SALVAGE // LOOT REVEAL",1);
    // 16 original pixel-art sunshafts burst behind an increasingly open can.
    // Rays are masked pixels, not translucent solid quads or stock TRASH art.
    const int rays=12;
    if(tick<198)for(int i=0;i<rays;++i){
        const int angle=(i*2048/rays+(tick*5))&2047;
        const int length=Clamp((tick<44?tick*1100:54000)-(i%3)*2000,0,62000);
        ChestArt(160,86,length,angle,7,-15,0);
    }
    const int chestTile=HasArt(chestReveal.icon)?chestReveal.icon:V47Art(0);
    if(tick<72){
        rotatesprite_fs((160+(tick%4-2))<<16,93<<16,56000L+(tick%19)*900,0,chestTile,-9,0,2|8|64);
        if(tick>30){
            ChestArt(159+(tick*4%27)-13,51-(tick-30),32000,(-tick*17)&2047,2,-10);
            ChestArt(160,73,44000,0,1,-15);
        }
    }else{
        rotatesprite_fs(160<<16,84<<16,62000L,0,chestTile,-19,0,2|8|64);
    }
    // Rats and flying trash are original 255-masked indexed sprites. They
    // follow curved ballistic paths, never spinning the prize art itself.
    if(tick>35 && tick<180){
        const int age=tick-35;
        for(int i=0;i<8;++i){
            const int local=age-i*7;
            if(local<=0||local>96)continue;
            const int side=(i&1)?1:-1;
            const int x=160+side*(12+local*(2+(i%3)))+((sintable[(local*19+i*157)&2047])>>12);
            const int y=84-(local*(5+(i%3)))/2+(local*local)/25;
            ChestArt(x,y,24000+(i%3)*3000,(local*13+i*210)&2047,
                     i%3==0?3+(local/7%2):i%3==1?5:8,-8);
        }
    }
    if(tick>=44 && tick<185){
        const int progress=Clamp(tick-44,0,135);
        // Bright but bounded sparks: no heap allocations per frame.
        for(int i=0;i<10;++i){
            int a=(i*205+progress*14)&2047;
            int r=20+(progress*3+i*9)%102;
            int x=160+(sintable[(a+512)&2047]*r)/16384;
            int y=86+(sintable[a]*r)/16384;
            ChestArt(x,y,13000+(i%3)*1800,a,8,-35);
        }
    }
    if(tick<78){
        printext256(82,166,accent,0,"RATS ARE RAIDING YOUR LOOT!",1);
    }else{
        const int showCount=count;
        // 1-5 actual reward slots; icons cycle with deceleration, then land on
        // the PRE-ROLLED authoritative native items and reveal their rarity.
        const int spacing=showCount>=4?54:showCount==3?80:showCount==2?90:0;
        const int first=160-(showCount-1)*spacing/2;
        for(int j=0;j<showCount;++j){
            const bool stopped=tick>=130+j*10;
            const int id=chestReveal.reward[j];if(id<0)continue;
            const int reelIndex=(tick/Max(3,13-j*2)+j*23)%kSkillCount;
            const int icon=ChestIconFor(stopped?id:reelIndex,stopped&&chestReveal.fusion[j]);
            const int x=first+j*spacing;
            if(HasArt(kV47ArtBase+9))ChestArt(x,106,30000,0,9,stopped?-20:10);
            rotatesprite_fs(x<<16,106<<16,18000L,0,icon,stopped?-20:6,0,2|8|64);
            if(stopped){
                const int rarity=Clamp(chestReveal.rarity[j],0,5);
                const int color=rarity>=5?14:rarity==4?7:rarity==3?13:rarity==2?12:15;
                char label[64];
                std::snprintf(label,sizeof(label),"%s",rarity==5?"MYTHIC":
                    rarity==4?"LEGEND":rarity==3?"EPIC":rarity==2?"RARE":
                    rarity==1?"UNCOMMON":"COMMON");
                printext256(x-25,123,color,0,label,1);
            }
        }
        if(tick>=185){
            printext256(47,146,14,0,"JACKPOT! PRIZES ACQUIRED",1);
            for(int j=0;j<showCount;++j){
                const int id=chestReveal.reward[j];if(id<0)continue;
                char label[96];
                if(chestReveal.fusion[j]){
                    if(id>=kHybridFusionBase){
                        const int primary=(id-kHybridFusionBase)/kSkillCount;
                        const int partner=(id-kHybridFusionBase)%kSkillCount;
                        std::snprintf(label,sizeof(label),"FUSION: %.27s + %.27s",skillNames[primary],skillNames[partner]);
                    }else{
                        const int base=(int)(sizeof(evolutions)/sizeof(evolutions[0]));
                        const auto& e=id<base?evolutions[id]:v39Fusions[id-base];
                        std::snprintf(label,sizeof(label),"EVOLVED: %.52s",e.name);
                    }
                }else std::snprintf(label,sizeof(label),"%.59s",skillNames[id]);
                // Clamp to the HUD; the full label is also available in armory.
                printext256(34,158+j*8,14,0,label,1);
            }
        }else printext256(85,165,15,0,"SALVAGE REELS SPINNING...",1);
    }
    if(tick>270)FinishChestReveal();
}
// Called by A_DamageObject for a projectile/bullet striking a chest sprite.
// World state and rewards remain authoritative in the native survivor runtime.
static bool DamageChest(int victim,int dmgSrc){
    if(!OwnWorld() || victim<0 || victim>=MAXSPRITES || dmgSrc<0 || dmgSrc>=MAXSPRITES)return false;
    Cache *c=nullptr;
    for(auto &cache:caches)if(cache.kind==5 && cache.id==victim){c=&cache;break;}
    if(!c)return false;
    int seat=-1;
    for(bssize_t TRAVERSE_CONNECT(i))if(Alive(i)){
        int owner=g_player[i].ps->i;
        if(dmgSrc==owner || sprite[dmgSrc].owner==owner ||
           sprite[dmgSrc].owner==g_player[i].ps->i){seat=i;break;}
    }
    if(seat<0) return true; // still suppress original barrel/prop damage handling
    if(chestReveal.seat<0){StartChestReveal(seat);lastCacheAward=-6;}
    A_DeleteSprite(victim);*c=Cache{};
    return true;
}
static void ProcessSupplies(){
    if(!OwnWorld())return;
    if(gameTicks%(kHz*35)==0)
        for(bssize_t TRAVERSE_CONNECT(i))if(Alive(i))SpawnSupply(i,1+(RandomBounded(4)));
    // Long-run insurance: a visible, collectible global vacuum every ~3 min.
    if(gameTicks>0 && gameTicks%(kHz*180)==0 && numplayers==1)
        for(bssize_t TRAVERSE_CONNECT(i))if(Alive(i)){
            const auto& p=sprite[g_player[i].ps->i];
            Drop at{};at.x=p.x+800;at.y=p.y+500;at.z=p.z;
            at.sectorId=p.sectnum;SpawnXPVacuum(at);
        }
    for(auto &c:caches){
        if(c.id<0)continue;
        if(c.id>=MAXSPRITES || sprite[c.id].statnum!=STAT_FX || sprite[c.id].picnum!=c.tile){c=Cache{};continue;}
        auto &item=sprite[c.id];
        StepGroundPhysics(c.id,c.vz,1<<8,145,1400,0);
        item.xrepeat=item.yrepeat=static_cast<uint8_t>(ArtRepeat(c.tile,c.kind==5?54:42)+(c.age%20<10?3:0));
        if(c.kind==6){
            item.pal=7;item.shade=(int8_t)(-32+(c.age%20)/5);
        }
        if(c.kind==5){
            // Never pal-cycle the underlying can! Index 255 is transparent,
            // but changing the body palette can corrupt its intended pixels.
            item.pal=0;
            item.shade=(int8_t)(-30+(c.age%24)/4);
        }
        if(++c.age>kHz*120){A_DeleteSprite(c.id);c=Cache{};continue;}
        for(bssize_t TRAVERSE_CONNECT(i)){
            if(!Alive(i))continue;
            auto const& p=sprite[g_player[i].ps->i];
            int64_t dx=static_cast<int64_t>(p.x)-item.x,dy=static_cast<int64_t>(p.y)-item.y;
            int const pickupRadius=Max(280,500+(c.kind==1?fighters[i].skill[HEALTH_MAGNET]*220:0)+(int)activeBoons[6]*180-(int)activeBanes[6]*80);
            if(dx*dx+dy*dy>static_cast<int64_t>(pickupRadius)*pickupRadius || !cansee(item.x,item.y,item.z,item.sectnum,p.x,p.y,p.z,p.sectnum))continue;
            auto &f=fighters[i];
            auto player=g_player[i].ps;
            if(c.kind==6){
                BeginXPVacuum(i);
                lastCacheAward=-7;
            }else if(c.kind==5){
                // Either touch or smash (quick kick) opens the same authoritative
                // jackpot. Destroying the trashcan never deletes its reward.
                StartChestReveal(i);lastCacheAward=-6;
            }else if(c.kind==1){
                auto &hp=sprite[player->i].extra;
                hp=Min(250,hp+30+f.skill[HEALTH_MAGNET]*8);
                ++collectedHealth;lastCacheAward=-2;
            } else if(c.kind==2){
                auto &shield=player->inv_amount[GET_SHIELD];
                shield=Min(200,shield+35+f.skill[TITANIUM_ARMOR]*7);
                ++collectedArmor;lastCacheAward=-3;
            } else if(c.kind==3){
                player->inv_amount[GET_JETPACK]=Min(1600,player->inv_amount[GET_JETPACK]+400+f.skill[JET_AFTERBURNER]*55);
                lastCacheAward=-4;
            } else if(c.kind==4){
                player->inv_amount[GET_FIRSTAID]=Min(100,player->inv_amount[GET_FIRSTAID]+40);
                player->inv_amount[GET_STEROIDS]=Min(400,player->inv_amount[GET_STEROIDS]+100);
                lastCacheAward=-5;
            } else {
                const int start=static_cast<int>(RandomBounded(kSkillCount));
                for(int n=0;n<kSkillCount;++n){
                    int skill=(start+n)%kSkillCount;
                    if(!CanOfferSkill(f,skill))continue;
                    AcquireItem(f,skill);lastCacheAward=skill;break;
                }
            }
            cacheBannerUntil=gameTicks+4*kHz;
            A_DeleteSprite(c.id);c=Cache{};
            break;
        }
    }
}
static int SpawnOne(int seat,int tile,bool elite){
    if(!Alive(seat))return -1;
    auto p=g_player[seat].ps;
    auto const &body=sprite[p->i];
    // Respect the finite Build sprite pool; no expensive per-frame allocations.
    for(int t=0;t<14;++t){
        int a=Random()&2047;
        int dist=4400+static_cast<int>(RandomBounded(5000));
        int x=body.x + static_cast<int>((int64_t)sintable[(a+512)&2047]*dist/16384);
        int y=body.y + static_cast<int>((int64_t)sintable[a]*dist/16384);
        int16_t sec=p->cursectnum;
        updatesector(x,y,&sec);
        if(sec<0||sec>=numsectors)continue;
        auto const& s=sector[sec];
        if(s.floorz-s.ceilingz<(32<<8)||s.lotag==ST_2_UNDERWATER)continue;
        // V32.2: early waves use REAL RAT ART with native, damageable AI.
        // Later enemies continue using their authentic CON actor initialization.
        bool const rat=(tile==RAT && numplayers==1 && HasArt(RAT));
        if(!rat && !HasArt(tile))continue;
        int const spawnFloor=GroundHeight(sec,x,y);
        // Native grounded Duke enemies start by the ground; CON keeps ownership
        // of their physics afterwards. Flying stock enemies retain clearance.
        bool const groundEnemy=rat || tile==PIGCOP || tile==LIZMAN || tile==BOSS1;
        int id=A_InsertSprite(sec,x,y,spawnFloor-(groundEnemy?(1<<8):(32<<8)),tile,
                              0,rat?104:40,rat?104:40,a,0,0,-1,
                              rat?STAT_ACTOR:STAT_DEFAULT);
        if(id<0)return -1;
        if(rat){
            survivorRat[id]=1;
            ratNextBite[id]=gameTicks+30;
            ratVerticalSpeed[id]=0;
            const int variant=static_cast<int>(RandomBounded(4));
            ratVariant[id]=static_cast<uint8_t>(variant);
            sprite[id].xrepeat=sprite[id].yrepeat=static_cast<uint8_t>(104+variant*7);
            sprite[id].extra=(elite?34:12)+variant*5;
            sprite[id].pal=static_cast<uint8_t>(variant==0?0:variant==1?8:variant==2?6:2);
            sprite[id].clipdist=24;
            sprite[id].cstat=CSTAT_SPRITE_BLOCK+CSTAT_SPRITE_BLOCK_HITSCAN;
        }
        enemyObserved[id]=1;enemyDropSeen[id]=0;
        if(!rat)A_Spawn(-1,id); // Original CON actor initialization and AI.
        if(elite){sprite[id].extra=Clamp(sprite[id].extra*3,75,1500);sprite[id].pal=7;}
        // Progressive enemy HP scaling; use EDuke32 health on real sprites.
        int const hpPct=Min(400,100+(wave-1)*8);
        sprite[id].extra=Max(1,sprite[id].extra*hpPct/100);
        return id;
    }
    return -1;
}
static int EnemyTile(){
    uint32_t n=RandomBounded(100);
    // Native rat swarm for starter waves, if this GRP genuinely has RAT art.
    if(numplayers==1 && HasArt(RAT) && (wave<=2 || (n<Min(45,14+wave/3) && n<35)))return RAT;
    if(wave>=15 && n<13 && HasArt(COMMANDER))return COMMANDER;
    if(wave>=11 && n<27 && HasArt(OCTABRAIN))return OCTABRAIN;
    if(wave>=7 && n<45 && HasArt(LIZMAN))return LIZMAN;
    if(wave>=3 && n<61 && HasArt(PIGCOP))return PIGCOP;
    if(HasArt(LIZTROOP))return LIZTROOP;
    // Shareware/modified GRPs lack later monster tiles. Never render FIRE
    // or EXPLOSION2 as a fake enemy: return a genuine native rat instead.
    return HasArt(RAT) && numplayers==1?RAT:-1;
}
// V38 native anti-idle watchdog: original CON still drives attack decisions.
// Do not turn enemy-on-enemy aggression into immortality or rewrite CON states.
// If an active, visible non-rat opponent is motionless for several samples,
// wake it and face the player so original pursuit can resume.
static void EnemyAggroWatchdogTick(){
    if(!OwnWorld() || gameTicks%45)return;
    for(int id=0;id<MAXSPRITES;++id){
        if(IsSurvivorRat(id))continue; // Rats have their own ground AI.
        auto &e=sprite[id];
        if(e.extra<=0 || (e.statnum!=STAT_ACTOR && e.statnum!=STAT_ZOMBIEACTOR) || !IsSurvivorEnemy(id))continue;
        if(afflictions[id].kind==EC_FROZEN || afflictions[id].kind==EC_SHOCKED)continue;
        int nearest=-1;int64_t closest=11000LL*11000;
        for(bssize_t TRAVERSE_CONNECT(seat))if(Alive(seat)){
            auto const &p=sprite[g_player[seat].ps->i];
            int64_t dx=(int64_t)p.x-e.x,dy=(int64_t)p.y-e.y,d2=dx*dx+dy*dy;
            if(d2<closest && cansee(e.x,e.y,e.z-(14<<8),e.sectnum,
                                    p.x,p.y,p.z-(18<<8),p.sectnum)){
                nearest=seat;closest=d2;
            }
        }
        if(nearest<0){enemyIdleScore[id]=0;continue;}
        const int dx=e.x-enemyAggroX[id],dy=e.y-enemyAggroY[id];
        if((int64_t)dx*dx+(int64_t)dy*dy<175LL*175)
            enemyIdleScore[id]=(uint8_t)Min(12,(int)enemyIdleScore[id]+1);
        else {enemyIdleScore[id]=0;enemyWakeAttempts[id]=0;}
        enemyAggroX[id]=e.x;enemyAggroY[id]=e.y;
        if(enemyIdleScore[id]<3)continue;
        auto const &p=sprite[g_player[nearest].ps->i];
        e.ang=getangle(p.x-e.x,p.y-e.y);
        actor[id].timetosleep=0;
        // Original Duke actors can remain in STAT_ZOMBIEACTOR after failed
        // wake/evasion paths; move the dormant enemy back into the real actor
        // simulation, while preserving its CON AI/HP and collision contract.
        if(e.statnum==STAT_ZOMBIEACTOR)changespritestat(id,STAT_ACTOR);
        // A repeated position stall is not invulnerability. Impulse uses the
        // authentic collision resolver and will never teleport through walls.
        enemyWakeAttempts[id]=(uint8_t)Min(8,(int)enemyWakeAttempts[id]+1);
        if(enemyWakeAttempts[id]>=3){
            const int16_t ang=e.ang;
            vec3_t walk{(int)sintable[(ang+512)&2047]/40,
                        (int)sintable[ang&2047]/40,0};
            A_MoveSprite(id,walk,CLIPMASK0);
        }
        enemyIdleScore[id]=0;++nativeAggroWakes;
    }
}
static void HordeTick(){
    if(!OwnWorld()||gameTicks%Max(9,26-(int)activeBanes[0]*3+(int)activeBoons[0]*2))return;
    activeEnemies=CountEnemies();
    // Mobile-aware cap: ambition without exceeding the engine sprite budget.
    int cap=Min(250,64+wave*9 + (numplayers-1)*18);
    cap=Min(cap,Max(48,MAXSPRITES/8));
    int seats[MAXPLAYERS],len=0;
    for(bssize_t TRAVERSE_CONNECT(i))if(Alive(i)&&len<MAXPLAYERS)seats[len++]=i;
    if(!len)return;
    // Guaranteed milestone boss attempt even if the ordinary horde is at cap.
    if(wave>=Max(3,5-(int)activeBanes[3]+(int)activeBoons[3]) && wave%Max(2,5-(int)activeBanes[3]+(int)activeBoons[3])==0 && bossWave!=wave){
        if(HasArt(BOSS1) && SpawnOne(seats[RandomBounded(len)],BOSS1,true)>=0)bossWave=wave;
    }
    cap=Min(250,cap+(int)activeBanes[0]*7);
    cap=Max(16,cap-(int)activeBoons[0]*4);
    if(activeEnemies>=cap)return;
    int burst=Min(kMaxSpawnsPerBurst,3+wave/2+spawnDebt+(int)activeBanes[0]*2);
    burst=Min(burst,cap-activeEnemies);
    int made=0;
    for(int i=0;i<burst;++i){
        bool elite=wave>=4 && (RandomBounded(100) < (unsigned)Min(35,1+wave/5+(int)activeBanes[3]*3));
        int tile=EnemyTile();
        if(tile>=0 && SpawnOne(seats[(gameTicks/26+i)%len],tile,elite)>=0)++made;
    }
    spawnDebt=made==0?Min(6,spawnDebt+1):Max(0,spawnDebt-1);
}
static void QueueDrop(int actorId){
    if(!initialized||!OwnWorld()||actorId<0||actorId>=MAXSPRITES)return;
    auto const& s=sprite[actorId];
    if(s.extra>0||s.sectnum<0||s.sectnum>=numsectors||!IsSurvivorEnemy(actorId)||enemyDropSeen[actorId])return;
    int next=(dropWrite+1)%kDeathQueueLimit;
    if(next==dropRead)return;
    int value=(s.picnum==BOSS1?35:(s.pal==7?8:2));
    drops[dropWrite].x=s.x;drops[dropWrite].y=s.y;drops[dropWrite].z=s.z;
    drops[dropWrite].sectorId=s.sectnum;drops[dropWrite].value=value;
    dropWrite=next;
    enemyDropSeen[actorId]=1;
}
// DXV32.2 engine-owned rat swarm: no replacement for Duke's original enemy AI.
// Uses Build collision via A_MoveSprite; authentic stock pistol, shotgun and
// survivor attacks change sprite.extra through original A_IncurDamage.
static void RatTick(){
    if(numplayers!=1 || !OwnWorld())return;
    for(int id=0;id<MAXSPRITES;++id){
        if(!survivorRat[id])continue;
        if(!IsSurvivorRat(id)){
            survivorRat[id]=0;ratVerticalSpeed[id]=0;continue;
        }
        auto &rat=sprite[id];
        // Original gun projectiles can leave hits queued until the next CON
        // actor tick; survivor rats own their AI, so consume them here.
        if(actor[id].htextra>=0 && actor[id].htpicnum>=0)A_IncurDamage(id);
        if(rat.extra<=0){
            QueueDrop(id);
            A_DeleteSprite(id);
            survivorRat[id]=0;ratVerticalSpeed[id]=0;
            continue;
        }
        int seat=-1;int64_t best=static_cast<int64_t>(15000)*15000;
        for(bssize_t TRAVERSE_CONNECT(i)){
            if(!Alive(i))continue;
            auto const &player=sprite[g_player[i].ps->i];
            const int64_t dx=static_cast<int64_t>(player.x)-rat.x;
            const int64_t dy=static_cast<int64_t>(player.y)-rat.y;
            int64_t d=dx*dx+dy*dy;
            if(d<best && player.sectnum>=0){best=d;seat=i;}
        }
        // Vertical physics is unconditional: freezing, shocking, or having no
        // visible player must never suspend gravity. Horizontal collision is
        // still handled by Duke's native A_MoveSprite.
        StepGroundPhysics(id,ratVerticalSpeed[id],1<<8,150,1450,0);
        if(seat<0)continue;
        // Genuine freeze and electro-stun immobilize native survivor rats too.
        const auto &condition=afflictions[id];
        if((condition.kind==EC_FROZEN || condition.kind==EC_SHOCKED) &&
           condition.picnum==rat.picnum && gameTicks<condition.until)continue;
        auto const &player=sprite[g_player[seat].ps->i];
        rat.ang=getangle(player.x-rat.x,player.y-rat.y);
        // Use true engine-wall collision instead of clipping through geometry.
        if(best>320LL*320){
            const int speed=(70+Min(120,wave*7)+ratVariant[id]*14)*(condition.kind==EC_MINI?2:3)/3;
            const int dx=(sintable[(rat.ang+512)&2047]*speed)/16384;
            const int dy=(sintable[rat.ang]*speed)/16384;
            A_MoveSprite(id,{dx,dy,0},CLIPMASK0);
            // Terrain can rise or fall during the horizontal step. Resample
            // contact at the *new* XY rather than holding the prior altitude.
            int const foot=GroundHeight(rat.sectnum,rat.x,rat.y)-(1<<8);
            if(rat.sectnum>=0 && rat.sectnum<numsectors && rat.z>foot){
                rat.z=foot;ratVerticalSpeed[id]=0;
            }
        }else if(gameTicks>=ratNextBite[id] && gameTicks>=ratGlobalBiteReady){
            auto const playerId=g_player[seat].ps->i;
            actor[playerId].htextra=2+Min(4,wave/5)+ratVariant[id]/2;
            actor[playerId].htpicnum=RAT;
            actor[playerId].htowner=id;
            A_IncurDamage(playerId);
            ratNextBite[id]=gameTicks+36;
            ratGlobalBiteReady=gameTicks+12; // limit group contact damage to playable levels.
        }
    }
}
static void ObserveDeaths(){
    // Corpses are frequently retained by Duke CON. A_DeleteSprite alone cannot
    // provide consistent XP pickups. Record living actors; drop once on death.
    if(gameTicks%3)return;
    for(int id=0;id<MAXSPRITES;++id){
        auto const &s=sprite[id];
        bool valid=s.sectnum>=0 && s.sectnum<numsectors &&
          (s.statnum==STAT_ACTOR||s.statnum==STAT_ZOMBIEACTOR) && IsSurvivorEnemy(id);
        if(valid && s.extra>0){
            if(!enemyObserved[id])enemyDropSeen[id]=0;
            enemyObserved[id]=1;
            auto &last=enemyLastSeen[id];
            last.x=s.x;last.y=s.y;last.z=s.z;last.sectorId=s.sectnum;
            last.value=s.picnum==BOSS1?35:(s.pal==7?8:2);
        } else if(enemyObserved[id]){
            if(s.extra<=0){
                if(valid) QueueDrop(id);
                if(enemyLastSeen[id].value>=8 &&
                   (enemyLastSeen[id].value>=30 || RandomBounded(100)<(uint32_t)Clamp(24+(int)activeBoons[4]*9-(int)activeBanes[4]*3,1,85)))
                    SpawnChestAt(enemyLastSeen[id]);
                // Elites occasionally leave a global XP-vacuum collectible;
                // boss deaths guarantee one when a cache slot is available.
                if(enemyLastSeen[id].value>=8 &&
                   (enemyLastSeen[id].value>=30 || RandomBounded(100)<24))
                    SpawnXPVacuum(enemyLastSeen[id]);
                if(!valid && !enemyDropSeen[id]){
                    int next=(dropWrite+1)%kDeathQueueLimit;
                    if(next!=dropRead){drops[dropWrite]=enemyLastSeen[id];dropWrite=next;enemyDropSeen[id]=1;}
                }
            }
            enemyObserved[id]=0;
        }
    }
}
static void CreateGem(Drop const& d){
    int freeSlot=-1;
    for(int i=0;i<kGemLimit;++i)if(gems[i].id<0){freeSlot=i;break;}
    if(freeSlot<0){
        // Never lose earned XP because the bounded native sprite pool is full.
        // Merge the overflow into a live pickup for the future vacuum sweep.
        for(auto &existing:gems)if(existing.id>=0){
            existing.value=Min(INT_MAX-Max(0,d.value),existing.value)+Max(0,d.value);
            return;
        }
        return;
    }
    if(d.sectorId<0||d.sectorId>=numsectors)return;
    // A real, rendered Duke ART tile in the original Build actor array.
    int tile=GemTile();
    int repeat=ArtRepeat(tile,24);
    int id=A_InsertSprite(d.sectorId,d.x,d.y,d.z,tile,0,repeat,repeat,0,0,0,-1,STAT_FX);
    if(id<0)return;
    sprite[id].cstat=0;
    sprite[id].pal=d.value>=30?8:(d.value>=8?5:7);
    sprite[id].shade=-24;
    gems[freeSlot].id=id;gems[freeSlot].value=d.value;
    gems[freeSlot].age=0;gems[freeSlot].tile=tile;
    gems[freeSlot].vz=0;gems[freeSlot].bounces=0;
}
static void EffectAt(int x,int y,int z,int16_t sectorId,int tile,int pal,int life,int size);
static void ProcessGems(){
    const bool vacuumActive=xpVacuum.ticks>0 && Alive(xpVacuum.seat);
    if(xpVacuum.ticks>0){
        if(vacuumActive)--xpVacuum.ticks;
        else {xpVacuum.ticks=0;xpVacuum.seat=-1;}
    }
    // Deletion callbacks enqueue; here the original world tick is complete.
    int budget=48;
    while(dropRead!=dropWrite && budget--){CreateGem(drops[dropRead]);dropRead=(dropRead+1)%kDeathQueueLimit;}
    for(auto &g:gems){
        if(g.id<0)continue;
        if(g.id>=MAXSPRITES || sprite[g.id].statnum!=STAT_FX || sprite[g.id].picnum!=g.tile){g=Gem{};continue;}
        auto &s=sprite[g.id];
        // Ground collision and short, damped bounces; no perpetual levitation.
        // Magnet movement below is horizontal only and cannot override gravity.
        if(!vacuumActive){
            auto const contact=StepGroundPhysics(g.id,g.vz,1<<8,115,1300,
                                                g.bounces<3?28:0);
            if(contact.grounded && g.vz<0)++g.bounces;
        }
        // Pulsating crystal art is an ORIGINAL Duke ART sprite, not HTML art.
        // Strong native ART pulsation with independent rarity palettes.
        int phase=(g.age+g.id*3)%32;
        int pulse=phase<16?phase:(32-phase);
        s.xrepeat=s.yrepeat=static_cast<uint8_t>(ArtRepeat(g.tile,24)+pulse);
        s.shade=static_cast<int8_t>(-22-(g.age%20<10?12:0));
        // Four clearly differentiated values: cobalt, cyan, violet, gold.
        s.pal=(g.value>=30?8:(g.value>=8?6:(g.value>=4?2:1)));
        if((g.age+g.id)%14==0 && (g.age/14)%2==0)
            EffectAt(s.x,s.y,s.z,s.sectnum,TRANSPORTERSTAR,s.pal,9,16);
        // Missed experience persists throughout a run, so a later vacuum
        // never loses an old orb because an arbitrary age timer expired.
        if(g.age<INT_MAX-1)++g.age;
        if(vacuumActive){
            const int seat=xpVacuum.seat;
            const auto& player=sprite[g_player[seat].ps->i];
            const int64_t dx=(int64_t)player.x-s.x,dy=(int64_t)player.y-s.y;
            const int64_t d=dx*dx+dy*dy;
            const int near=kVacuumCollectRadius;
            if(d<(int64_t)near*near){
                GrantXP(seat,g.value);++xpVacuum.collected;
                A_DeleteSprite(g.id);g=Gem{};continue;
            }
            // Direct sector-aware relocation, *not* A_MoveSprite(CLIPMASK0):
            // only vacuumed XP ignores solid walls, never combat actors.
            const int sx=XPClampStep(dx,3,kVacuumMaxStep);
            const int sy=XPClampStep(dy,3,kVacuumMaxStep);
            const int nx=s.x+sx,ny=s.y+sy;
            int16_t next=s.sectnum;
            updatesector(nx,ny,&next);
            if(next<0||next>=numsectors)next=player.sectnum;
            if(next>=0 && next<numsectors && next!=s.sectnum)
                changespritesect(g.id,next);
            s.x=nx;s.y=ny;
            s.z+=XPClampStep((int64_t)player.z-s.z,3,18<<8);
            // Do not wait for a second physics tick after reaching the player.
            const int64_t remx=(int64_t)player.x-s.x,remy=(int64_t)player.y-s.y;
            if(remx*remx+remy*remy<(int64_t)near*near){
                GrantXP(seat,g.value);++xpVacuum.collected;
                A_DeleteSprite(g.id);g=Gem{};
            }
            continue;
        }
        int who=-1;int64_t dist=INT64_MAX;
        for(bssize_t TRAVERSE_CONNECT(i)){
            if(!Alive(i))continue;
            auto const &p=sprite[g_player[i].ps->i];
            int64_t dx=static_cast<int64_t>(p.x)-s.x,dy=static_cast<int64_t>(p.y)-s.y;
            int64_t d=dx*dx+dy*dy;
            int radius=kXPBaseMagnetRadius + fighters[i].skill[12]*kXPMagnetRankRadius;
            int zDelta=(p.z-s.z)>>8;
            if(zDelta<-64||zDelta>64)continue;
            // Don't make XP fly through solid walls just because sectors overlap.
            if(d<static_cast<int64_t>(radius)*radius && d<dist &&
               cansee(s.x,s.y,s.z,s.sectnum,p.x,p.y,p.z,p.sectnum)){who=i;dist=d;}
        }
        if(who<0)continue;
        auto const &p=sprite[g_player[who].ps->i];
        if(dist<360LL*360){
            GrantXP(who,g.value);
            A_DeleteSprite(g.id);g=Gem{};
        } else {
            // Magnetizes within the same Build sector and never crosses walls
            // without a valid destination sector from the actual engine.
            int const nx=s.x+XPClampStep((int64_t)p.x-s.x,3,3600);
            int const ny=s.y+XPClampStep((int64_t)p.y-s.y,3,3600);
            int16_t sec=s.sectnum;updatesector(nx,ny,&sec);
            if(sec>=0 && sec<numsectors){
                A_MoveSprite(g.id,{nx-s.x,ny-s.y,0},CLIPMASK0);
                // Next frame's gravity re-evaluates the slope at the new XY.
                // On uphill movement do not let the pickup intersect the map.
                if(s.sectnum>=0 && s.sectnum<numsectors){
                    int const foot=GroundHeight(s.sectnum,s.x,s.y)-(1<<8);
                    if(s.z>foot){s.z=foot;g.vz=0;}
                }
            }
        }
    }
}
static int WeaponTile(int w){
    switch(w){
      case PISTOL:case SHOTGUN:return SHOTSPARK1;
      case ROCKET:case DEVASTATOR:return RPG;
      case FREEZE:return HasArt(FREEZEBLAST)?FREEZEBLAST:FIRELASER;
      case SHRINK:return SHRINKSPARK;
      case LASER:return FIRELASER;
      default:return SHOTSPARK1;
    }
}
static void BudgetShoot(int source,int tile){
    if(shotsRemaining<=0)return;
    --shotsRemaining;
    A_Shoot(source,tile);
}
static void FireWeapon(int seat,int type,int targetAngle,int rank){
    auto p=g_player[seat].ps;
    auto &f=fighters[seat];
    int const extra=Min(4,f.skill[7]/2);
    int const spread=(type==SHOTGUN?86:24)+f.skill[9]*7;
    int const count=Clamp(1+(type==SHOTGUN?rank:rank/3)+extra+f.skill[10]/2,1,12);
    int oldAngle=p->q16ang;
    for(int j=0;j<count;++j){
        int shift=(j-(count-1)/2)*spread;
        int facing=(targetAngle+shift)&2047;
        p->q16ang=fix16_from_int(facing);
        BudgetShoot(p->i,WeaponTile(type));
        // Critical passive means a real additional projectile, not fabricated
        // damage. RNG is only used by the host world authority.
        if(f.skill[19]>0 && (RandomBounded(100))<(unsigned)Min(48,f.skill[19]*8))
            BudgetShoot(p->i,WeaponTile(type));
    }
    p->q16ang=oldAngle;
}
// Native, deterministic, slot-independent combat. Every equipped weapon runs
// on its own cooldown; these are NOT replacements for Duke's held gun.
static bool DamageTarget(int seat,int enemy,int damage,int weaponTile);
static void AfflictEnemy(int seat,int enemy,int kind,int rank);
static void HitNearby(int seat,int radius,int damage,int limit);
// DXV28: all effect sprites are original Build sprites placed in valid sectors.
// The fixed-size pool expires visual particles, protecting Android sprite limits.
static void EffectAt(int x,int y,int z,int16_t sectorId,int tile,int pal,int life,int size=9){
    if(!OwnWorld() || sectorId<0 || sectorId>=numsectors)return;
    // DXV29: retain a hard cap and use full-sized game-world sprites.
    size=Clamp(size,14,48);
    life=Clamp(life,3,32);
    int slot=-1;for(int j=0;j<144;++j)if(visuals[j].id<0){slot=j;break;}
    if(slot<0)return;
    tile=EditionTile(tile,TRANSPORTERSTAR);
    int const scaled=Clamp(size*ArtRepeat(tile,32)/32,14,72);
    int id=A_InsertSprite(sectorId,x,y,z,tile,-30,scaled,scaled,0,0,0,-1,STAT_FX);
    if(id<0)return;
    sprite[id].pal=pal; sprite[id].shade=-32;
    visuals[slot]={id,life,tile};
}
// DXV34: one engine-native beam trace governs sight, visual extent AND contact damage.
// Build hitscan provides wall/portal/floor/ceiling intersection. The sector pass
// enforces water interfaces and parallax skies, including sloped floor/ceiling.
// This is world simulation C++, not a JavaScript-only drawing overlay.
enum BeamSurface { BS_NONE=0, BS_WALL=1, BS_FLOOR=2, BS_CEILING=3,
                   BS_SKY=4, BS_WATER=5, BS_SPRITE=6, BS_VOID=7 };
enum BeamPolicy { BP_STOP_AT_WATER=1, BP_NO_SPARKS=2, BP_HIT_PROPS=4 };
struct BeamTraceResult {
    int x=0,y=0,z=0, endSector=-1, surface=BS_NONE, wall=-1, hitSprite=-1;
    int x0=0,y0=0,z0=0,startSector=-1;
};
static int64_t BeamDistanceSquared(int x,int y,int z,int xx,int yy,int zz){
    const int64_t dx=(int64_t)xx-x, dy=(int64_t)yy-y, dz=((int64_t)zz-z)/16;
    return dx*dx+dy*dy+dz*dz;
}
static BeamTraceResult BeamTrace(int x,int y,int z,int16_t sec,int xx,int yy,int zz,int policy=BP_STOP_AT_WATER){
    BeamTraceResult result{};
    result.x=result.x0=x;result.y=result.y0=y;result.z=result.z0=z;
    result.startSector=sec;result.endSector=sec;
    if(sec<0 || sec>=numsectors){result.surface=BS_VOID;return result;}
    // Degenerate beam: no zero-divisions, no imposter projectiles.
    if(x==xx && y==yy && z==zz)return result;
    ++beamTraces;
    const int64_t total=BeamDistanceSquared(x,y,z,xx,yy,zz);
    int tx=xx,ty=yy,tz=zz,hitSector=sec;
    int worldSurface=BS_NONE,hitWall=-1,hitSprite=-1;
    vec3_t origin{x,y,z};hitdata_t hit{};
    // CLIPMASK1's lower 16 bits: blocking/hitscan walls but NOT sprites;
    // actors are deliberately handled by beam capsule queries (piercing).
    const int32_t vx=xx-x,vy=yy-y,vz=zz-z;
    hitscan(&origin,sec,vx,vy,vz,&hit,CLIPMASK1&0xffff);
    if(hit.sect>=0 && hit.sect<numsectors){
        int64_t const arrival=BeamDistanceSquared(x,y,z,hit.x,hit.y,hit.z);
        int64_t const dot=(int64_t)(hit.x-x)*vx+(int64_t)(hit.y-y)*vy+
                          ((int64_t)(hit.z-z)/16)*((int64_t)vz/16);
        if(dot>=0 && arrival<=total+256){
            tx=hit.x;ty=hit.y;tz=hit.z;hitSector=hit.sect;
            hitWall=hit.wall;hitSprite=hit.sprite;
            if(hit.wall>=0)worldSurface=BS_WALL;
            else if(hit.sprite>=0)worldSurface=BS_SPRITE;
            else if(hit.z<=getceilzofslope(hit.sect,hit.x,hit.y))
                worldSurface=(sector[hit.sect].ceilingstat&1)?BS_SKY:BS_CEILING;
            else if(hit.z>=getflorzofslope(hit.sect,hit.x,hit.y))
                worldSurface=(sector[hit.sect].floorstat&1)?BS_SKY:BS_FLOOR;
        }
    }
    // Optional solid prop collision (sprites with Build hitscan-block cstat).
    // Enemies remain piercing targets and never shorten the geometry beam.
    // Exclude the active player at the muzzle so firing won't self-obstruct.
    if(policy&BP_HIT_PROPS){
        hitdata_t obstruction{};
        hitscan(&origin,sec,vx,vy,vz,&obstruction,CLIPMASK1);
        if(obstruction.sect>=0 && obstruction.sprite>=0 &&
           obstruction.sprite<MAXSPRITES && !IsSurvivorEnemy(obstruction.sprite) &&
           (myconnectindex<0 || myconnectindex>=MAXPLAYERS ||
            !g_player[myconnectindex].ps ||
            obstruction.sprite!=g_player[myconnectindex].ps->i)){
            int64_t const d=BeamDistanceSquared(x,y,z,obstruction.x,obstruction.y,obstruction.z);
            int64_t const stop=BeamDistanceSquared(x,y,z,tx,ty,tz);
            if(d>=64*64 && d<=stop){
                tx=obstruction.x;ty=obstruction.y;tz=obstruction.z;
                hitSector=obstruction.sect;hitSprite=obstruction.sprite;
                worldSurface=BS_SPRITE;
            }
        }
    }
    // Keep the final endpoint within actual traversed sectors, cut at water
    // boundaries (including ABOVE_WATER/UNDERWATER) and detect sky/floor.
    // Native hitscan remains the authority for walls and portal connectivity.
    int16_t currentSector=sec;
    bool underwater=sector[sec].lotag==ST_2_UNDERWATER;
    int stopStep=24;
    for(int step=1;step<=24;++step){
        int px=x+(int)((int64_t)(tx-x)*step/24);
        int py=y+(int)((int64_t)(ty-y)*step/24);
        int pz=z+(int)((int64_t)(tz-z)*step/24);
        int16_t candidate=currentSector;
        updatesectorz(px,py,pz,&candidate);
        // updatesectorz can fail at floor and sky: try conventional XY to
        // identify which surface closed off the current ray.
        if(candidate<0||candidate>=numsectors){
            int16_t fallback=currentSector;updatesector(px,py,&fallback);
            if(fallback<0||fallback>=numsectors){stopStep=step;worldSurface=BS_WALL;break;}
            candidate=fallback;
        }
        const int32_t ceilZ=getceilzofslope(candidate,px,py);
        const int32_t floorZ=getflorzofslope(candidate,px,py);
        if(pz<=ceilZ || pz>=floorZ){
            tx=px;ty=py;tz=Clamp(pz,ceilZ,floorZ);hitSector=candidate;
            worldSurface=(pz<=ceilZ)?((sector[candidate].ceilingstat&1)?BS_SKY:BS_CEILING):
                                    ((sector[candidate].floorstat&1)?BS_SKY:BS_FLOOR);
            stopStep=step;break;
        }
        const bool nowUnderwater=sector[candidate].lotag==ST_2_UNDERWATER;
        if((policy&BP_STOP_AT_WATER) && (nowUnderwater!=underwater ||
           (candidate!=currentSector &&
            (sector[candidate].lotag==ST_1_ABOVE_WATER || sector[currentSector].lotag==ST_1_ABOVE_WATER)))){
            tx=px;ty=py;tz=pz;hitSector=candidate;worldSurface=BS_WATER;
            stopStep=step;break;
        }
        currentSector=candidate;
    }
    (void)stopStep;
    result.x=tx;result.y=ty;result.z=tz;
    result.endSector=hitSector;result.surface=worldSurface;
    result.wall=hitWall;result.hitSprite=hitSprite;
    if(worldSurface!=BS_NONE){
        ++beamWorldImpacts;
        if(worldSurface==BS_WATER)++beamWaterStops;
        if(worldSurface==BS_SKY)++beamSkyStops;
        if(worldSurface==BS_WALL)++beamWallsStopped;
    }
    return result;
}
// A clipped segment's core AND translucent outer aura use the actual
// LASERLINE wall-aligned tile. xrepeat is calculated from the art width and
// the segment's world length, instead of stretching eight enormous sprites.
static int BeamSpriteSegment(int &id,int x,int y,int z,int16_t sec,
                             int xx,int yy,int zz,int pal,int thickness,bool halo,int owner=-1){
    if(!HasArt(LASERLINE)||sec<0||sec>=numsectors)return 0;
    const int64_t dx=(int64_t)xx-x,dy=(int64_t)yy-y;
    const int horizontal=(int)std::sqrt((double)(dx*dx+dy*dy));
    if(horizontal<16)return 0;
    int mx=x+(xx-x)/2,my=y+(yy-y)/2,mz=z+(zz-z)/2;
    int16_t segmentSector=sec;updatesectorz(mx,my,mz,&segmentSector);
    if(segmentSector<0||segmentSector>=numsectors)return 0;
    int width=Max(4,(int)tilesiz[LASERLINE].x);
    // Build wall-aligned sprites use roughly tile_px * xrepeat/4 XY units.
    int xr=Clamp((horizontal*4+width/2)/width,2,100);
    if(id<0||id>=MAXSPRITES||sprite[id].statnum!=STAT_FX||sprite[id].picnum!=LASERLINE){
        id=A_InsertSprite(segmentSector,mx,my,mz,LASERLINE,-62,Clamp(xr,2,100),
                          Clamp(halo?thickness*3:thickness,2,80),
                          (getangle(xx-x,yy-y)+512)&2047,0,0,owner,STAT_FX);
        if(id<0)return 0;
    }
    auto &strip=sprite[id];strip.x=mx;strip.y=my;strip.z=mz;
    if(strip.sectnum!=segmentSector)changespritesect(id,segmentSector);
    strip.ang=(getangle(xx-x,yy-y)+512)&2047;
    strip.xrepeat=(uint8_t)xr;
    strip.yrepeat=(uint8_t)Clamp(halo?thickness*3:thickness,2,80);
    strip.cstat=16|(halo?2:0);strip.pal=pal;
    strip.shade=halo?-18:-90;
    if(owner>=0)strip.owner=owner;
    return 1;
}
static void BeamRecycleId(int &id){
    if(id>=0 && id<MAXSPRITES && sprite[id].statnum==STAT_FX && sprite[id].picnum==LASERLINE)
        A_DeleteSprite(id);
    id=-1;
}
static void BeamStripsTick(){
    for(auto &b:beamRibbons)if(b.id>=0 && --b.life<=0){BeamRecycleId(b.id);b=BeamRibbon{};}
}
static void BeamStripTransient(int x,int y,int z,int16_t sec,int xx,int yy,int zz,
                               int pal,int thickness,int life){
    if(!OwnWorld()||!HasArt(LASERLINE))return;
    for(int layer=0;layer<2;++layer){
        for(auto &slot:beamRibbons)if(slot.id<0){
            int id=-1;
            if(BeamSpriteSegment(id,x,y,z,sec,xx,yy,zz,pal,thickness,layer!=0)){
                slot={id,Clamp(life,2,20),layer};++beamRibbonCount;
            }
            break;
        }
    }
}
// Public internal API: geometry clipping, optional persistence, spark effects.
// For sustained mines owned has 8 sections x 2 layers = 16 slots.
static BeamTraceResult DrawThickBeam(int x,int y,int z,int16_t sec,
                                    int xx,int yy,int zz,int pal,int thickness,
                                    int divisions,int life,int *owned=nullptr,
                                    int policy=BP_STOP_AT_WATER,bool jagged=false){
    BeamTraceResult trace=BeamTrace(x,y,z,sec,xx,yy,zz,policy);
    // Native transient beam record, consumed as a REAL GPU cylinder in V42.
    // Ring buffer replaces oldest effects without allocating at runtime.
    if(!owned && OwnWorld() && (trace.x!=x||trace.y!=y||trace.z!=z)){
        auto &fx=nativeBeamFx[nativeBeamCursor++%96];
        fx={x,y,z,trace.x,trace.y,trace.z,thickness,pal,gameTicks+Clamp(life,2,20)};
    }
    divisions=Clamp(divisions,1,owned?8:12);
    int sx=x,sy=y,sz=z;int16_t fromSector=sec;
    for(int i=0;i<divisions;++i){
        const int n=i+1;
        int ex=x+(int)((int64_t)(trace.x-x)*n/divisions);
        int ey=y+(int)((int64_t)(trace.y-y)*n/divisions);
        int ez=z+(int)((int64_t)(trace.z-z)*n/divisions);
        if(jagged && n<divisions && !owned){
            const int wobble=((gameTicks*61+n*173)&127)-64;
            const int64_t len=Max(1,(int)std::sqrt((double)((int64_t)(trace.x-x)*(trace.x-x)+(int64_t)(trace.y-y)*(trace.y-y))));
            ex+=(int)((int64_t)(trace.y-y)*wobble*5/len);
            ey-=(int)((int64_t)(trace.x-x)*wobble*5/len);
        }
        int16_t es=fromSector;updatesectorz(ex,ey,ez,&es);
        if(es<0 || es>=numsectors){
            es=fromSector; // drawable segment always remains valid inside portal path
        }
        if(owned){
            for(int layer=0;layer<2;++layer){
                int &id=owned[i*2+layer];
                if(!BeamSpriteSegment(id,sx,sy,sz,fromSector,ex,ey,ez,pal,thickness,layer!=0))
                    BeamRecycleId(id);
            }
        }else BeamStripTransient(sx,sy,sz,fromSector,ex,ey,ez,pal,thickness,life);
        sx=ex;sy=ey;sz=ez;fromSector=es;
    }
    if(owned)for(int i=divisions*2;i<16;++i)BeamRecycleId(owned[i]);
    // Sparks are real Build FX at the impact point, not a colliding phantom orb.
    if(trace.surface!=BS_NONE && !(policy&BP_NO_SPARKS) && trace.endSector>=0 &&
       trace.endSector<numsectors && gameTicks%3==0){
        if(trace.surface==BS_WATER)EffectAt(trace.x,trace.y,trace.z,trace.endSector,TRANSPORTERSTAR,6,7,16);
        else if(trace.surface!=BS_SKY)EffectAt(trace.x,trace.y,trace.z,trace.endSector,SHOTSPARK1,pal,6,17);
    }
    return trace;
}
// 3D capsule test: a beam only touches an enemy within its clipped segment,
// using actual sprite height and collision footprint. The XY axis and z are
// converted to common Build world units; no more full-length 2D laser damage.
static bool BeamCrossesEnemy(BeamTraceResult const& b,int id,int radius){
    if(id<0||id>=MAXSPRITES||!IsSurvivorEnemy(id))return false;
    const auto &e=sprite[id];
    if(e.extra<=0||(e.statnum!=STAT_ACTOR && e.statnum!=STAT_ZOMBIEACTOR))return false;
    const int64_t vx=(int64_t)b.x-b.x0,vy=(int64_t)b.y-b.y0;
    const int64_t dx=(int64_t)e.x-b.x0,dy=(int64_t)e.y-b.y0;
    const int64_t len2=vx*vx+vy*vy;
    if(len2<1)return false;
    int64_t along=dx*vx+dy*vy;
    if(along<0||along>len2)return false;
    const int64_t cross=dx*vy-dy*vx;
    const int64_t allowance=Max(96,radius+e.clipdist*4);
    // Divide before comparing: squaring the cross product can overflow i64
    // on huge mod maps even when the segment endpoints themselves are valid.
    if(std::llabs(cross)/(int64_t)Max(1,(int)std::sqrt((double)len2))>allowance)return false;
    const int64_t beamZ=b.z0+((int64_t)(b.z-b.z0)*along)/len2;
    if(e.picnum<0||e.picnum>=MAXTILES)return false;
    const int spriteHeight=Clamp((int)tilesiz[e.picnum].y*(int)e.yrepeat*4,160,6000);
    const int64_t eye=(int64_t)e.z-(int64_t)spriteHeight*8;
    const int64_t tolerance=(int64_t)spriteHeight*8+(int64_t)Max(64,radius)*16;
    if(beamZ<eye-tolerance || beamZ>eye+tolerance)return false;
    // This additional local LOS protects adjacent closed-door sectors.
    const int cx=b.x0+(int)(vx*along/len2),cy=b.y0+(int)(vy*along/len2);
    int16_t sc=(int16_t)b.startSector;updatesector(cx,cy,&sc);
    return sc>=0&&sc<numsectors && cansee(cx,cy,(int)beamZ,sc,e.x,e.y,(int)eye,e.sectnum);
}
static int BeamDamageCrossings(BeamTraceResult const& b,int seat,int width,int damage,
                               int tile,int status,int rank,int maxHits,int cooldown){
    int hits=0;
    for(int id=0;id<MAXSPRITES&&hits<maxHits;++id){
        if(id==g_player[seat].ps->i || !BeamCrossesEnemy(b,id,width))continue;
        if(cooldown>0 && beamLastDamageTick[id]>0 && gameTicks+1-beamLastDamageTick[id]<cooldown)continue;
        if(DamageTarget(seat,id,damage,tile)){
            if(status!=EC_NONE)AfflictEnemy(seat,id,status,rank);
            beamLastDamageTick[id]=gameTicks+1;
            ++beamDamageHits;++hits;
        }
    }
    return hits;
}
// Legacy visual call sites now delegate to the same geometry-aware renderer.
static void EffectLine(int x,int y,int z,int16_t sec,int xx,int yy,int zz,int tile,int pal,int divisions){
    DrawThickBeam(x,y,z,sec,xx,yy,zz,pal, tile==LASERLINE?12:8,
                  divisions,7,nullptr,BP_STOP_AT_WATER,tile==FIRELASER);
}
static void VisualsTick(){
    for(auto &v:visuals){
        if(v.id<0)continue;
        if(--v.life<=0){
            if(v.id<MAXSPRITES && sprite[v.id].statnum==STAT_FX && sprite[v.id].picnum==v.tile)A_DeleteSprite(v.id);
            v=Visual{};
        }
    }
}
static void ClearAffliction(int id);
// DXV32: This call MUST verify a real health change, not simply queue an
// actor's htextra and assume some later CON branch will consume it.
// A_IncurDamage is the original EDuke32 damage dispatcher and triggers the
// PREACTORDAMAGE event. Invalid targets and event-vetoed hits are never counted.
static bool DamageTarget(int seat,int enemy,int damage,int weaponTile){
    if(!Alive(seat)||!OwnWorld()||enemy<0||enemy>=MAXSPRITES)return false;
    auto &e=sprite[enemy];
    if(e.extra<=0 || !IsSurvivorEnemy(enemy) || e.sectnum<0 ||
       (e.statnum!=STAT_ACTOR && e.statnum!=STAT_ZOMBIEACTOR))return false;
    auto &a=actor[enemy];
    // Consume any damage already queued by an original Duke projectile first.
    // Never overwrite another weapon's pending hit record.
    if(a.htextra>=0 && a.htpicnum>=0)A_IncurDamage(enemy);
    if(e.extra<=0)return true;
    const int before=e.extra;
    auto const &p=sprite[g_player[seat].ps->i];
    auto const& skill=fighters[seat].skill;
    if(skill[129])damage+=skill[129]*3; // heavy payload
    // Every generic equipment fusion is a native damage-synergy link.
    // Cap aggregate bonus to avoid unbounded cascading power scaling.
    int fusionSynergy=0;
    for(int skillId=0;skillId<kSkillCount;++skillId){
        auto const& f=fighters[seat];
        if(f.fused[skillId] && f.skill[skillId]>0 && IsWeaponSkill(skillId))
            fusionSynergy+=5+(int)f.rarity[skillId]*3;
    }
    damage+=damage*Min(75,fusionSynergy)/100;
    damage+=damage*activeBoons[2]/10;
    damage+=damage*activeBoons[5]/12;
    damage=Max(1,damage-damage*activeBanes[5]/12);
    damage=Max(1,damage-damage*activeBanes[2]/12);
    if(IsSurvivorRat(enemy) && skill[131])damage+=skill[131]*4; // rat slayer
    if(e.picnum==BOSS1 && skill[132])damage+=skill[132]*6; // boss breaker
    if(e.pal==7 && skill[141])damage+=skill[141]*3; // elite hunter
    if(sprite[g_player[seat].ps->i].extra<45 && skill[140])damage+=skill[140]*5; // berserk
    if(skill[154] && sprite[g_player[seat].ps->i].extra>40)damage+=skill[154]*3; // glass cannon
    a.htextra=Max(1,damage);
    a.htpicnum=weaponTile;
    a.htowner=g_player[seat].ps->i;
    a.htang=getangle(e.x-p.x,e.y-p.y);
    const int hitCode=A_IncurDamage(enemy);
    int dealt=Max(0,before-e.extra);
    if(hitCode>=0 && e.extra<0)e.extra=0; // keep original CON death predicates reachable
    combatLastVictim=enemy;combatLastHPBefore=before;
    if(hitCode<0){
        a.htextra=-1;a.htpicnum=-1;++blockedHits;
        // Script-defined invulnerability is honored for the first two hits.
        // If an actual hostile is repeatedly damage-vetoed while remaining
        // alive, its gameplay instance cannot become permanently immortal.
        // Boss scripts retain their intentional protected phases.
        if(enemyDamageVetoStreak[enemy]<255)++enemyDamageVetoStreak[enemy];
        if(enemyDamageVetoStreak[enemy]<(e.picnum==BOSS1?5:3))return false;
        e.extra=Max(0,before-Max(1,damage));
        ++nativeDamageFallbacks;
        // Restore a zero-damage CON notification, not a second HP hit.
        a.htextra=0;a.htpicnum=weaponTile;a.htowner=g_player[seat].ps->i;
    }else enemyDamageVetoStreak[enemy]=0;
    // V38 combat invariant: some stock CON actors accept the hit event but
    // do not change HP while fighting another actor / in transition states.
    // The native Survivor armory must still be able to kill those enemies.
    // Only fall back after the genuine engine dispatcher returned success.
    if(dealt==0 && e.extra>0){
        e.extra=Max(0,before-Max(1,damage));
        ++nativeDamageFallbacks;
    }
    if(e.extra>=before){++blockedHits;return false;}
    dealt=before-e.extra;combatLastHPAfter=e.extra;
    // Preserve original CON ifhitweapon notification on the next actor tic.
    // Damage has ALREADY been applied; a zero-damage latch lets the enemy
    // execute its authentic flinch/freeze/shrink/death script exactly once.
    a.htextra=0;
    a.htpicnum=weaponTile;
    a.htowner=g_player[seat].ps->i;
    ++confirmedHits;confirmedDamage+=dealt;
    // Cryo-shatter is a health-changing combination, not a cosmetic sparkle.
    auto const kind=afflictions[enemy].kind;
    if(kind==EC_FROZEN && fighters[seat].skill[CRYO_SHATTER]>0 &&
       weaponTile!=FREEZEBLAST){
        int bonus=Min(18,2+fighters[seat].skill[CRYO_SHATTER]*2);
        e.extra=Max(0,e.extra-bonus);confirmedDamage+=bonus;
        ++shatteredVictims;
        EffectAt(e.x,e.y,e.z-(9<<8),e.sectnum,TRANSPORTERSTAR,1,14,29);
        ClearAffliction(enemy);
    }
    return true;
}
static void ClearAffliction(int id){
    auto &a=afflictions[id];if(!a.kind)return;
    auto &e=sprite[id];
    if(e.statnum<MAXSTATUS && e.picnum==a.picnum){
        e.xrepeat=a.xrepeat;e.yrepeat=a.yrepeat;
        e.clipdist=a.clipdist;e.pal=a.pal;
    }
    a=EnemyAffliction{};
}
static void AfflictEnemy(int seat,int enemy,int kind,int rank){
    if(!Alive(seat)||!OwnWorld() || enemy<0||enemy>=MAXSPRITES ||
       kind<=EC_NONE || kind>EC_BURNING)return;
    auto &e=sprite[enemy];
    if(e.extra<=0 || e.sectnum<0 || !IsSurvivorEnemy(enemy) ||
       (e.statnum!=STAT_ACTOR && e.statnum!=STAT_ZOMBIEACTOR))return;
    auto &a=afflictions[enemy];
    if(a.kind && (a.picnum!=e.picnum || gameTicks>=a.until))ClearAffliction(enemy);
    if(a.kind && a.kind!=kind)ClearAffliction(enemy);
    if(!a.kind){
        a.picnum=e.picnum;a.xrepeat=e.xrepeat;a.yrepeat=e.yrepeat;
        a.clipdist=e.clipdist;a.pal=e.pal;
    }
    a.kind=static_cast<uint8_t>(kind);a.owner=seat;
    a.until=Max(a.until,gameTicks+Clamp(60+rank*14,50,270));
    a.nextPulse=gameTicks+18;
    switch(kind){
      case EC_FROZEN:
        e.pal=1;e.shade=-28;++frozenVictims;
        EffectAt(e.x,e.y,e.z-(12<<8),e.sectnum,TRANSPORTERSTAR,1,22,35);
        break;
      case EC_MINI:
        e.xrepeat=static_cast<uint8_t>(Max(8,(int)a.xrepeat*45/100));
        e.yrepeat=static_cast<uint8_t>(Max(8,(int)a.yrepeat*45/100));
        e.clipdist=static_cast<uint8_t>(Max(8,(int)a.clipdist/2));
        e.pal=6;++miniVictims;
        EffectAt(e.x,e.y,e.z-(8<<8),e.sectnum,SHRINKSPARK,6,20,26);
        break;
      case EC_GROWING:
        e.xrepeat=static_cast<uint8_t>(Min(180,(int)a.xrepeat*170/100));
        e.yrepeat=static_cast<uint8_t>(Min(180,(int)a.yrepeat*170/100));
        e.clipdist=static_cast<uint8_t>(Min(255,(int)a.clipdist*3/2));
        e.pal=2;++grownVictims;
        EffectAt(e.x,e.y,e.z-(12<<8),e.sectnum,TRANSPORTERSTAR,2,23,42);
        break;
      case EC_SHOCKED:
        e.pal=7;++shockedVictims;
        EffectAt(e.x,e.y,e.z-(12<<8),e.sectnum,FIRELASER,7,15,28);
        break;
      case EC_BURNING:
        e.pal=0;
        EffectAt(e.x,e.y,e.z-(8<<8),e.sectnum,FIRE,0,20,32);
        break;
    }
}
static void ConditionsTick(){
    if(!OwnWorld())return;
    for(int id=0;id<MAXSPRITES;++id){
        auto &a=afflictions[id];if(!a.kind)continue;
        auto &e=sprite[id];
        if(e.statnum>=MAXSTATUS || e.extra<=0 || e.picnum!=a.picnum ||
           !IsSurvivorEnemy(id) || gameTicks>=a.until){
            ClearAffliction(id);++conditionExpiry;continue;
        }
        if(gameTicks<a.nextPulse)continue;
        a.nextPulse=gameTicks+18;
        if(a.kind==EC_BURNING || a.kind==EC_GROWING){
            int seat=a.owner;int dmg=a.kind==EC_BURNING?5:4;
            // Do not treat VFX and HP as interchangeable: use original damage path.
            if(DamageTarget(seat,id,dmg,RADIUSEXPLOSION)){
                EffectAt(e.x,e.y,e.z-(12<<8),e.sectnum,
                         a.kind==EC_BURNING?FIRE:TRANSPORTERSTAR,a.kind==EC_BURNING?0:2,15,31);
            }
        }
    }
}
static int ClosestEnemy(int x,int y,int z,int16_t sec,int range,const uint8_t *used=nullptr);
// DXV33_PROJECTILE_ENGINE: native, collision-bound, authored movement families.
// No change to GRP art files; physical projectiles use available stock tiles.
static int BoltTile(int kind){
    switch(kind){
        case NB_COMET: return EditionTile(FREEZEBLAST,TRANSPORTERSTAR);
        case NB_RECALL: return EditionTile(SHRINKSPARK,TRANSPORTERSTAR);
        case NB_PRISM: return EditionTile(TRANSPORTERSTAR,FIRELASER);
        default: return EditionTile(FIRELASER,TRANSPORTERSTAR);
    }
}
static int SpawnBolt(int seat,int kind,int rank,int angle){
    if(!OwnWorld()||!Alive(seat))return -1;
    NativeBolt *slot=nullptr;
    for(auto &b:bolts)if(b.id<0){slot=&b;break;}
    if(!slot)return -1;
    auto const &p=sprite[g_player[seat].ps->i];
    int tile=BoltTile(kind),rep=ArtRepeat(tile,kind==NB_COMET?37:23);
    int id=A_InsertSprite(p.sectnum,p.x,p.y,p.z-(20<<8),tile,-32,
                          rep,rep,angle&2047,0,0,g_player[seat].ps->i,STAT_FX);
    if(id<0)return -1;
    sprite[id].pal=kind==NB_COMET?6:kind==NB_PRISM?8:kind==NB_RECALL?2:7;
    sprite[id].shade=-38;
    sprite[id].cstat=0; // own collision path; don't block the engine's players
    *slot={id,seat,kind,rank,kind==NB_PRISM?165:kind==NB_RECALL?90:105,
           0,angle&2047,kind==NB_NEXUS?390:kind==NB_COMET?240:330,
           kind==NB_COMET?-160:0,-1,0,tile};
    ++nativeBoltsFired;
    return id;
}
static void DestroyBolt(NativeBolt &b){
    if(b.id>=0 && b.id<MAXSPRITES && sprite[b.id].statnum==STAT_FX &&
       sprite[b.id].picnum==b.originalTile)A_DeleteSprite(b.id);
    b=NativeBolt{};
}
static void BoltImpact(NativeBolt &b,int enemy){
    if(b.id<0 || !Alive(b.seat))return;
    auto const &s=sprite[b.id];
    if(enemy>=0 && DamageTarget(b.seat,enemy,4+b.rank*3,
                         b.kind==NB_COMET?FREEZEBLAST:FIRELASER)){
        ++nativeBoltHits;
        if(b.kind==NB_COMET)AfflictEnemy(b.seat,enemy,EC_FROZEN,b.rank);
        if(b.kind==NB_RECALL)AfflictEnemy(b.seat,enemy,EC_MINI,b.rank);
    }
    if(b.kind==NB_COMET){
        // Frost shock expands on contact with real walls, floors and actors.
        int count=0;
        for(int j=0;j<MAXSPRITES && count<7;++j){
            if(j==enemy || !IsSurvivorEnemy(j) || sprite[j].extra<=0)continue;
            auto const &t=sprite[j];
            int64_t dx=(int64_t)t.x-s.x,dy=(int64_t)t.y-s.y;
            if(dx*dx+dy*dy>static_cast<int64_t>(1350+b.rank*90)*(1350+b.rank*90))continue;
            if(!cansee(s.x,s.y,s.z,s.sectnum,t.x,t.y,t.z,t.sectnum))continue;
            if(DamageTarget(b.seat,j,2+b.rank,FREEZEBLAST)){
                AfflictEnemy(b.seat,j,EC_FROZEN,b.rank);++nativeBoltHits;++count;
            }
        }
    }
    EffectAt(s.x,s.y,s.z,s.sectnum,b.kind==NB_COMET?FREEZEBLAST:TRANSPORTERSTAR,
             b.kind==NB_COMET?6:7,17,b.kind==NB_COMET?44:25);
}
static void NativeBoltTick(){
    for(auto &b:bolts){
        int id=b.id;
        if(id<0)continue;
        if(id>=MAXSPRITES || sprite[id].statnum!=STAT_FX ||
           sprite[id].picnum!=b.originalTile || !Alive(b.seat)){
            DestroyBolt(b);continue;
        }
        auto &p=sprite[id];
        if(--b.life<=0){if(b.kind==NB_COMET)BoltImpact(b,-1);DestroyBolt(b);continue;}
        ++b.age;
        auto const &owner=sprite[g_player[b.seat].ps->i];
        if(b.kind==NB_PRISM){
            // Orbit is an ACTUAL independent projectile with retained sprite ID.
            int angle=(b.angle+b.age*(12+b.rank))&2047;
            int orbit=640+b.rank*85;
            int x=owner.x+static_cast<int>(static_cast<int64_t>(sintable[(angle+512)&2047])*orbit/16384);
            int y=owner.y+static_cast<int>(static_cast<int64_t>(sintable[angle])*orbit/16384);
            int16_t sec=p.sectnum;updatesector(x,y,&sec);
            if(sec>=0 && sec<numsectors && cansee(owner.x,owner.y,owner.z,owner.sectnum,x,y,p.z,sec)){
                p.x=x;p.y=y;p.z=owner.z-(20<<8);
                if(sec!=p.sectnum)changespritesect(id,sec);
            }
        }else{
            if(b.kind==NB_RECALL && b.age>26){
                b.angle=getangle(owner.x-p.x,owner.y-p.y);b.speed=Min(560,b.speed+12);
                int64_t dx=(int64_t)owner.x-p.x,dy=(int64_t)owner.y-p.y;
                if(dx*dx+dy*dy<250LL*250){DestroyBolt(b);continue;}
            }else if(b.kind==NB_NEXUS){
                // Seek is bounded by a finite turn rate, not teleporting damage.
                int target=ClosestEnemy(p.x,p.y,p.z,p.sectnum,9500);
                if(target>=0){
                    int wanted=getangle(sprite[target].x-p.x,sprite[target].y-p.y);
                    int diff=((wanted-b.angle+1024)&2047)-1024;
                    b.angle=(b.angle+Clamp(diff,-38-b.rank*5,38+b.rank*5))&2047;
                }
            }else if(b.kind==NB_COMET){
                b.vertical=Clamp(b.vertical+46,-350,850);
                b.angle=(b.angle+5)&2047;
            }
            int dx=static_cast<int>(static_cast<int64_t>(sintable[(b.angle+512)&2047])*b.speed/16384);
            int dy=static_cast<int>(static_cast<int64_t>(sintable[b.angle])*b.speed/16384);
            int collision=A_MoveSprite(id,{dx,dy,b.vertical},CLIPMASK1);
            if(p.sectnum<0 || p.sectnum>=numsectors){DestroyBolt(b);continue;}
            auto const &room=sector[p.sectnum];
            if(b.kind==NB_COMET && (p.z>=room.floorz-(5<<8)||p.z<=room.ceilingz+(5<<8))){
                BoltImpact(b,-1);DestroyBolt(b);++nativeBoltCollisions;continue;
            }
            if(collision){
                ++nativeBoltCollisions;
                if(b.kind==NB_RECALL){b.angle=(b.angle+1024)&2047;b.age=27;}
                else if(b.kind==NB_COMET){BoltImpact(b,-1);DestroyBolt(b);continue;}
                else {DestroyBolt(b);continue;}
            }
        }
        if((b.age%3)==0){
            p.shade=static_cast<int8_t>(-38-(b.age%5)*2);
            p.xrepeat=p.yrepeat=static_cast<uint8_t>(Clamp(ArtRepeat(b.originalTile,24)+(b.age%8),14,75));
            EffectAt(p.x,p.y,p.z,p.sectnum,b.originalTile,p.pal,8,17);
        }
        if((b.age%4)==0 && b.hitCooldown<=0){
            int enemy=ClosestEnemy(p.x,p.y,p.z,p.sectnum,b.kind==NB_COMET?360:450);
            if(enemy>=0){
                BoltImpact(b,enemy);
                if(b.kind!=NB_PRISM && b.kind!=NB_RECALL){DestroyBolt(b);continue;}
                b.hitCooldown=12; // repeated orbit strikes are explicitly throttled
            }
        }
        if(b.hitCooldown>0)--b.hitCooldown;
    }
}
static void V33AbilitiesTick(int seat){
    if(!Alive(seat))return;
    auto const &f=fighters[seat];
    int angle=NearestEnemyAngle(seat,kTargetRadius);
    if(f.skill[NEXUS_DARTS] && angle>=0 &&
       gameTicks%Max(18,74-f.skill[NEXUS_DARTS]*6-f.skill[COOLING]*3)==0){
        int count=Clamp(1+f.skill[NEXUS_DARTS]/3+f.skill[7]/4,1,5);
        for(int j=0;j<count;++j)SpawnBolt(seat,NB_NEXUS,f.skill[NEXUS_DARTS],(angle+(j-(count-1)/2)*105)&2047);
    }
    if(f.skill[PRISM_HALO] && gameTicks%Max(24,115-f.skill[PRISM_HALO]*7)==0){
        for(int i=0;i<Clamp(1+f.skill[PRISM_HALO]/3,1,3);++i)
            SpawnBolt(seat,NB_PRISM,f.skill[PRISM_HALO],(i*683+gameTicks*11)&2047);
    }
    if(f.skill[RECALL_SAWS] && angle>=0 &&
       gameTicks%Max(28,94-f.skill[RECALL_SAWS]*6)==0)
        for(int i=0;i<Clamp(1+f.skill[RECALL_SAWS]/4,1,3);++i)
            SpawnBolt(seat,NB_RECALL,f.skill[RECALL_SAWS],(angle+(i-1)*190)&2047);
    if(f.skill[CRYO_COMETS] && angle>=0 &&
       gameTicks%Max(40,130-f.skill[CRYO_COMETS]*8)==0)
        SpawnBolt(seat,NB_COMET,f.skill[CRYO_COMETS],angle);
    if(f.skill[GRAVITY_WELL] && gameTicks%8==0){
        auto const &p=sprite[g_player[seat].ps->i];
        int target=ClosestEnemy(p.x,p.y,p.z,p.sectnum,10500);
        if(target>=0){
            auto const &t=sprite[target];
            int radius=800+f.skill[GRAVITY_WELL]*130;
            EffectAt(t.x,t.y,t.z-(18<<8),t.sectnum,TRANSPORTERSTAR,8,10,40);
            int pulled=0;
            for(int j=0;j<MAXSPRITES && pulled<12;++j){
                if(!IsSurvivorEnemy(j)||sprite[j].extra<=0||j==target)continue;
                auto const &e=sprite[j];
                int64_t dx=(int64_t)t.x-e.x,dy=(int64_t)t.y-e.y;
                if(dx*dx+dy*dy>(int64_t)radius*radius ||
                   !cansee(e.x,e.y,e.z,e.sectnum,t.x,t.y,t.z,t.sectnum))continue;
                A_MoveSprite(j,{Clamp(static_cast<int>(dx/12),-95,95),
                                Clamp(static_cast<int>(dy/12),-95,95),0},CLIPMASK0);
                ++pulled;
                if(gameTicks%24==0)DamageTarget(seat,j,1+f.skill[GRAVITY_WELL],RADIUSEXPLOSION);
            }
        }
    }
    if(f.skill[TESLA_MESH] && gameTicks%Max(24,85-f.skill[TESLA_MESH]*5)==0){
        auto const &p=sprite[g_player[seat].ps->i];
        int from=ClosestEnemy(p.x,p.y,p.z,p.sectnum,11000);
        if(from>=0){
            const auto &a=sprite[from];
            int to=ClosestEnemy(a.x,a.y,a.z,a.sectnum,5700);
            if(to>=0 && to!=from){
                auto const &b=sprite[to];
                auto const mesh=DrawThickBeam(a.x,a.y,a.z-(8<<8),a.sectnum,
                    b.x,b.y,b.z-(8<<8),7,8+f.skill[TESLA_MESH],7,9,nullptr,
                    BP_STOP_AT_WATER,true);
                BeamDamageCrossings(mesh,seat,180,4+f.skill[TESLA_MESH]*2,
                                    FIRELASER,EC_SHOCKED,f.skill[TESLA_MESH],9,8);
            }
        }
    }
}
static void HitNearby(int seat,int radius,int damage,int limit){
    if(!Alive(seat)||!OwnWorld())return;
    auto const &src=sprite[g_player[seat].ps->i];
    int hits=0;
    const int64_t radius2=static_cast<int64_t>(radius)*radius;
    for(int id=0;id<MAXSPRITES && hits<limit;++id){
        auto const &e=sprite[id];
        if(e.extra<=0 || (e.statnum!=STAT_ACTOR && e.statnum!=STAT_ZOMBIEACTOR) ||
           !IsSurvivorEnemy(id))continue;
        const int64_t dx=static_cast<int64_t>(e.x)-src.x;
        const int64_t dy=static_cast<int64_t>(e.y)-src.y;
        if(dx*dx+dy*dy>radius2 || e.sectnum<0)continue;
        if(!cansee(src.x,src.y,src.z,src.sectnum,e.x,e.y,e.z,e.sectnum))continue;
        if(DamageTarget(seat,id,damage,RADIUSEXPLOSION))++hits;
    }
}
// Confirm each radius victim once, after original A_RadiusDamage has already
// queued native CON hit data. This handles actors whose CON never consumes
// that queue without doubling a successfully queued blast.
static int ConfirmBlastDamage(int seat,int x,int y,int z,int16_t sec,int radius,int fallbackDamage){
    if(!OwnWorld()||!Alive(seat)||sec<0)return 0;
    const int64_t r2=static_cast<int64_t>(radius)*radius;
    int confirmed=0;
    for(int i=0;i<MAXSPRITES && confirmed<32;++i){
        auto &e=sprite[i];
        if(e.extra<=0 || (e.statnum!=STAT_ACTOR&&e.statnum!=STAT_ZOMBIEACTOR) ||
           !IsSurvivorEnemy(i)||e.sectnum<0)continue;
        const int64_t dx=static_cast<int64_t>(e.x)-x;
        const int64_t dy=static_cast<int64_t>(e.y)-y;
        if(dx*dx+dy*dy>r2 || !cansee(x,y,z,sec,e.x,e.y,e.z,e.sectnum))continue;
        const int before=e.extra;
        // Radius queued htextra: consume that precise value first.
        auto &a=actor[i];
        if(a.htextra>=0 && a.htpicnum>=0)A_IncurDamage(i);
        if(e.extra<before){
            combatLastVictim=i;combatLastHPBefore=before;combatLastHPAfter=e.extra;
            ++confirmedHits;confirmedDamage+=before-e.extra;++confirmed;
        } else if(DamageTarget(seat,i,fallbackDamage,RADIUSEXPLOSION))++confirmed;
    }
    return confirmed;
}
static int ClosestEnemy(int x,int y,int z,int16_t sec,int range,const uint8_t *used){
    int best=-1;int64_t distance=static_cast<int64_t>(range)*range;
    for(int i=0;i<MAXSPRITES;++i){
        auto const &e=sprite[i];
        if(used && used[i])continue;
        if(e.extra<=0 || (e.statnum!=STAT_ACTOR&&e.statnum!=STAT_ZOMBIEACTOR) ||
           !IsSurvivorEnemy(i) || e.sectnum<0)continue;
        int64_t dx=static_cast<int64_t>(e.x)-x,dy=static_cast<int64_t>(e.y)-y;
        int64_t d=dx*dx+dy*dy;if(d>=distance)continue;
        if(!cansee(x,y,z,sec,e.x,e.y,e.z,e.sectnum))continue;
        best=i;distance=d;
    }
    return best;
}
// ========================================================================
// DXV47_PHYSICAL_COMBAT — no fake A_Shoot-only spiral or Devastator barrels.
// A spawned missile has its own valid sector, trajectory, collision and
// enemy-only damage. Original A_RadiusDamage is deliberately not called, so
// the player cannot die to a friendly auto-weapon in the survivors mode.
// ========================================================================
static void V47MissileReset(){
    for(auto &b:v47Missiles){
        if(b.id>=0 && b.id<MAXSPRITES && sprite[b.id].statnum==STAT_FX &&
           sprite[b.id].picnum==b.originalTile)A_DeleteSprite(b.id);
        b=V47Missile{};
    }
    v47MissilesLaunched=v47MissilesImpacted=0;
    v47ShotgunContacts=v47RailGuaranteed=v47TransformerReactions=0;
}
static bool V47LaunchMissile(int seat,int kind,int rank,int angle,int phase){
    if(!Alive(seat)||!OwnWorld())return false;
    V47Missile* slot=nullptr;
    for(auto &m:v47Missiles)if(m.id<0){slot=&m;break;}
    if(!slot)return false;
    const auto &p=sprite[g_player[seat].ps->i];
    if(p.sectnum<0||p.sectnum>=numsectors)return false;
    const int tile=HasArt(RPG)?RPG:V47Art(6);
    const int ar=ArtRepeat(tile,22);
    int startX=p.x+(int)((int64_t)sintable[(angle+512)&2047]*250/16384);
    int startY=p.y+(int)((int64_t)sintable[angle&2047]*250/16384);
    int16_t startSector=p.sectnum;updatesector(startX,startY,&startSector);
    if(startSector<0 || startSector>=numsectors){startX=p.x;startY=p.y;startSector=p.sectnum;}
    int id=A_InsertSprite(startSector,startX,startY,p.z-(17<<8),tile,-28,
                          ar,ar,angle&2047,0,0,g_player[seat].ps->i,STAT_FX);
    if(id<0)return false;
    sprite[id].owner=g_player[seat].ps->i;
    sprite[id].cstat=0;
    *slot={id,seat,kind,Max(1,rank),130,angle&2047,0,phase,
           kind==2?460:kind==1?510:490,tile,-1};
    ++v47MissilesLaunched;
    return true;
}
static void V47MissileDetonate(V47Missile &m){
    const int id=m.id;
    if(id>=0 && id<MAXSPRITES && sprite[id].statnum==STAT_FX){
        const auto p=sprite[id];
        if(p.sectnum>=0 && p.sectnum<numsectors){
            const int radius=950+m.rank*200+(m.kind==1?450:0);
            const int damage=8+m.rank*6+(m.kind==1?6:0);
            ConfirmBlastDamage(m.seat,p.x,p.y,p.z,p.sectnum,radius,damage);
            EffectAt(p.x,p.y,p.z,p.sectnum,EXPLOSION2,0,20,39);
            if(m.kind==2){
                // Scatter-shrapnel follow-up to evolved demolition missiles.
                ConfirmBlastDamage(m.seat,p.x,p.y,p.z,p.sectnum,
                                   radius+400,Max(3,damage/3));
            }
        }
        A_DeleteSprite(id);
    }
    ++v47MissilesImpacted;
    m=V47Missile{};
}
static void V47MissileTick(){
    for(auto &m:v47Missiles){
        if(m.id<0)continue;
        const int id=m.id;
        if(!OwnWorld() || !Alive(m.seat) || id>=MAXSPRITES ||
           sprite[id].statnum!=STAT_FX || sprite[id].picnum!=m.originalTile){
            if(id>=0 && id<MAXSPRITES && sprite[id].statnum==STAT_FX &&
               sprite[id].picnum==m.originalTile)A_DeleteSprite(id);
            m=V47Missile{};continue;
        }
        auto &b=sprite[id];
        ++m.age;
        if(--m.life<=0){V47MissileDetonate(m);continue;}
        // Turn toward target at a finite rate; spiral is a real spatial
        // oscillation perpendicular to travel, not a rotating picture tile.
        if(m.target<0 || m.target>=MAXSPRITES ||
           !IsSurvivorEnemy(m.target) || sprite[m.target].extra<=0 ||
           (m.age%6)==0)
            m.target=ClosestEnemy(b.x,b.y,b.z,b.sectnum,14500);
        const int enemy=m.target;
        int dz=0;
        if(enemy>=0){
            const auto &e=sprite[enemy];
            const int wanted=getangle(e.x-b.x,e.y-b.y);
            const int diff=((wanted-m.angle+1024)&2047)-1024;
            m.angle=(m.angle+Clamp(diff,-28-m.rank*2,28+m.rank*2))&2047;
            dz=Clamp((e.z-(11<<8)-b.z)/9,-900,900);
        }
        const int spiral=m.kind==0?(sintable[(m.age*96+m.phase)&2047]*110/16384):
                         (sintable[(m.age*55+m.phase)&2047]*65/16384);
        const int vx=(int)((int64_t)sintable[(m.angle+512)&2047]*m.speed/16384);
        const int vy=(int)((int64_t)sintable[m.angle]*m.speed/16384);
        const int sideX=(int)((int64_t)sintable[(m.angle+1024)&2047]*spiral/16384);
        const int sideY=(int)((int64_t)sintable[(m.angle+512)&2047]*spiral/16384);
        const int hit=A_MoveSprite(id,{vx+sideX,vy+sideY,dz},
                                   m.age<3?CLIPMASK0:CLIPMASK1);
        if(b.sectnum<0||b.sectnum>=numsectors){V47MissileDetonate(m);continue;}
        bool contact=hit!=0;
        if(enemy>=0 && sprite[enemy].statnum>=0){
            const int64_t dx=(int64_t)b.x-sprite[enemy].x;
            const int64_t dy=(int64_t)b.y-sprite[enemy].y;
            if(dx*dx+dy*dy<350LL*350 && std::abs(b.z-sprite[enemy].z)<(32<<8))contact=true;
        }
        const int32_t floor=getflorzofslope(b.sectnum,b.x,b.y);
        const int32_t ceil=getceilzofslope(b.sectnum,b.x,b.y);
        if(b.z>floor-(3<<8)||b.z<ceil+(3<<8))contact=true;
        if(contact){V47MissileDetonate(m);continue;}
        b.ang=m.angle;
        if((m.age%5)==0)EffectAt(b.x,b.y,b.z,b.sectnum,
                                 m.kind==0?FIRELASER:TRANSPORTERSTAR,0,7,18);
    }
}
static int V47ScatterShot(int seat,int angle,int rank,int nominated){
    if(!Alive(seat)||!OwnWorld())return 0;
    const auto &p=sprite[g_player[seat].ps->i];
    int hits=0;
    for(int id=0;id<MAXSPRITES && hits<8;++id){
        const auto &e=sprite[id];
        if(e.extra<=0 || (e.statnum!=STAT_ACTOR && e.statnum!=STAT_ZOMBIEACTOR) ||
           !IsSurvivorEnemy(id) || e.sectnum<0)continue;
        const int64_t dx=(int64_t)e.x-p.x,dy=(int64_t)e.y-p.y;
        const int64_t dist2=dx*dx+dy*dy;
        const int radius=7700+rank*350;
        if(dist2>(int64_t)radius*radius)continue;
        const int bearing=getangle(e.x-p.x,e.y-p.y);
        const int delta=std::abs(((bearing-angle+1024)&2047)-1024);
        if(delta>185+rank*12)continue;
        if(!cansee(p.x,p.y,p.z-(16<<8),p.sectnum,
                   e.x,e.y,e.z-(9<<8),e.sectnum))continue;
        const int damage=Max(2,(13+rank*4)*ItemPercent(fighters[seat],SHOTGUN)/100);
        if(DamageTarget(seat,id,damage,SHOTSPARK1)){
            ++hits;++v47ShotgunContacts;
            if(fighters[seat].skill[10]>0 || rank>=4){
                const int radius=600+rank*60;
                ConfirmBlastDamage(seat,e.x,e.y,e.z,e.sectnum,radius,
                                  Max(2,rank+fighters[seat].skill[10]));
                EffectAt(e.x,e.y,e.z-(10<<8),e.sectnum,EXPLOSION2,0,12,24);
            }
        }
    }
    // Targeted shots always have a canonical hit even with tiny rat art.
    if(hits==0 && nominated>=0 && nominated<MAXSPRITES &&
       IsSurvivorEnemy(nominated) && sprite[nominated].extra>0){
        const auto &e=sprite[nominated];
        if(cansee(p.x,p.y,p.z-(16<<8),p.sectnum,e.x,e.y,e.z-(9<<8),e.sectnum) &&
           DamageTarget(seat,nominated,9+rank*4,SHOTSPARK1)){
            ++hits;++v47ShotgunContacts;
        }
    }
    return hits;
}

// Authentic Build-world thrown ordnance. A_MoveSprite resolves collision with
// walls and other sprites; the game simulation updates fuse/drag/gravity/bounce.
// The native engine handles radius damage, CON pain/death, and destructible walls.
static bool ThrowWeapon(int seat,int kind,int rank){
    if(!OwnWorld() || !Alive(seat))return false;
    int slot=-1;
    for(int n=0;n<40;++n)if(thrown[n].id<0){slot=n;break;}
    if(slot<0)return false;
    auto const &ps=*g_player[seat].ps;
    auto const &body=sprite[ps.i];
    int angle=(ps.q16ang>>16)&2047;
    if(kind==DISC)angle=(angle+static_cast<int>(RandomBounded(105))-52)&2047;
    int tile=(kind==PIPEBOMB)?HEAVYHBOMB:
             (kind==DISC)?FIRELASER:
             (kind==CRYO_BOMB)?FREEZEBLAST:TRANSPORTERSTAR;
    tile=EditionTile(tile,HEAVYHBOMB);
    int sx=body.x+static_cast<int>((static_cast<int64_t>(sintable[(angle+512)&2047])*410)/16384);
    int sy=body.y+static_cast<int>((static_cast<int64_t>(sintable[angle])*410)/16384);
    int sz=body.z-(15<<8);
    int16_t sec=body.sectnum;updatesector(sx,sy,&sec);
    if(sec<0||sec>=numsectors)return false;
    int size=ArtRepeat(tile,kind==PIPEBOMB?35:26);
    int id=A_InsertSprite(sec,sx,sy,sz,tile,-18,size,size,angle,0,0,ps.i,STAT_FX);
    if(id<0)return false;
    sprite[id].owner=ps.i;
    sprite[id].pal=(kind==CRYO_BOMB)?1:(kind==DISC)?7:(kind==ARC_GRENADE)?6:0;
    sprite[id].shade=-25;
    thrown[slot]={id,seat,kind,Clamp(50+rank*5,45,130),
                  kind==DISC?6:kind==CRYO_BOMB?2:3,rank,
                  kind==DISC?480:360+rank*16,kind==DISC?0:-(320+rank*24),3};
    return true;
}
static void DetonateThrown(Thrown &b){
    int id=b.id;
    if(id<0||id>=MAXSPRITES){b=Thrown{};return;}
    if(sprite[id].statnum!=STAT_FX){b=Thrown{};return;}
    auto const p=sprite[id];
    int radius=(b.kind==DISC?650:1700)+b.rank*170;
    int dmg=9+b.rank*5;
    // Source owner remains the Duke player, allowing the V15 self-blast guard.
    if(b.kind!=DISC){
        A_RadiusDamage(id,radius,dmg/2,dmg,dmg*2,dmg*3);
        ConfirmBlastDamage(b.seat,p.x,p.y,p.z,p.sectnum,radius,dmg*2);
        ++impacts;
    }
    EffectAt(p.x,p.y,p.z,p.sectnum,EXPLOSION2,b.kind==CRYO_BOMB?1:0,16,45);
    if(b.kind==CRYO_BOMB){
        // Cryogenic grenade effect happens AT THE IMPACT, not at Duke's gun.
        // Queuing a native ifhitweapon FREEZEBLAST hit lets existing frozen-
        // enemy CON behavior and original shatter rules remain in charge.
        uint8_t used[MAXSPRITES]={};
        for(int j=0;j<Clamp(2+b.rank,2,10);++j){
            int target=ClosestEnemy(p.x,p.y,p.z,p.sectnum,2100+b.rank*175,used);
            if(target<0)break;
            used[target]=1;
            auto const enemy=sprite[target];
            if(DamageTarget(b.seat,target,1+b.rank/3,FREEZEBLAST)){
                AfflictEnemy(b.seat,target,EC_FROZEN,b.rank);
                EffectLine(p.x,p.y,p.z,p.sectnum,enemy.x,enemy.y,
                           enemy.z-(8<<8),FREEZEBLAST,1,5);
            }
        }
        ++cryoBursts;
    }else if(b.kind==ARC_GRENADE && Alive(b.seat)){
        uint8_t used[MAXSPRITES]={};
        int x=p.x,y=p.y,z=p.z;int16_t sec=p.sectnum;
        for(int chain=0;chain<Clamp(2+b.rank/2,2,6);++chain){
            int target=ClosestEnemy(x,y,z,sec,chain?5000:8000,used);
            if(target<0)break;
            used[target]=1;
            auto e=sprite[target];
            if(DamageTarget(b.seat,target,7+b.rank*5,RADIUSEXPLOSION))++arcHits;
            EffectLine(x,y,z,sec,e.x,e.y,e.z-(8<<8),FIRELASER,7,6);
            x=e.x;y=e.y;z=e.z;sec=e.sectnum;
        }
    }
    A_DeleteSprite(id);
    b=Thrown{};
}
static void ThrownTick(){
    for(auto &b:thrown){
        int id=b.id;
        if(id<0)continue;
        if(id>=MAXSPRITES || sprite[id].statnum!=STAT_FX ||
           b.seat<0 || b.seat>=MAXPLAYERS || !Alive(b.seat)){
            if(id>=0 && id<MAXSPRITES && sprite[id].statnum==STAT_FX)A_DeleteSprite(id);
            b=Thrown{};continue;
        }
        if(--b.life<=0){DetonateThrown(b);continue;}
        auto &p=sprite[id];
        int ang=p.ang&2047;
        int mx=static_cast<int>((static_cast<int64_t>(sintable[(ang+512)&2047])*b.speed)/16384);
        int my=static_cast<int>((static_cast<int64_t>(sintable[ang])*b.speed)/16384);
        b.vertical=Clamp(b.vertical+72,-600,1250);
        int hit=A_MoveSprite(id,{mx,my,b.vertical},CLIPMASK1);
        // Use original Build floor/ceiling geometry for vertical ricochets.
        // The source sprite remains in its valid sector after clipmove.
        if((unsigned)p.sectnum>=(unsigned)numsectors){DetonateThrown(b);continue;}
        auto const &room=sector[p.sectnum];
        bool floorHit=false;
        if(p.z>=room.floorz-(6<<8)){
            p.z=room.floorz-(6<<8);
            floorHit=b.vertical>0;
            b.vertical=-(b.vertical*55)/100;
            if(b.vertical>-90)b.vertical=0;
        }
        if(p.z<=room.ceilingz+(7<<8)){
            p.z=room.ceilingz+(7<<8);
            b.vertical=Max(40,-b.vertical/2);
        }
        if(hit || floorHit){
            if(b.grace>0)--b.grace;
            else if(b.bounces<=0){DetonateThrown(b);continue;}
            else{
                --b.bounces;++ricochets;
                if(hit){
                    // Reflect against the ACTUAL Build wall tangent when the
                    // collision reports a wall. No fake radius-based bounce.
                    // High 2 bits of clipmove result identify wall/sprite/sector.
                    bool reflected=false;
                    if((hit&49152)==32768){
                        int wi=hit&16383;
                        if(wi>=0 && wi<numwalls){
                            auto const &wa=wall[wi];
                            auto const &wb=wall[wa.point2];
                            int64_t tx=static_cast<int64_t>(wb.x)-wa.x;
                            int64_t ty=static_cast<int64_t>(wb.y)-wa.y;
                            int64_t len=tx*tx+ty*ty;
                            if(len>0){
                                int64_t dot=static_cast<int64_t>(mx)*tx+static_cast<int64_t>(my)*ty;
                                int64_t rx=2*dot*tx/len-mx;
                                int64_t ry=2*dot*ty/len-my;
                                if(rx||ry){p.ang=getangle(static_cast<int>(rx),static_cast<int>(ry));reflected=true;}
                            }
                        }
                    }
                    if(!reflected)p.ang=(ang+1024+static_cast<int>(RandomBounded(97))-48)&2047;
                }
                b.speed=Max(110,b.speed*75/100);
                EffectAt(p.x,p.y,p.z,p.sectnum,TRANSPORTERSTAR,p.pal,9,20);
            }
        }else if(b.grace>0)--b.grace;
        if(gameTicks%5==0)
            EffectAt(p.x,p.y,p.z,p.sectnum,b.kind==CRYO_BOMB?FREEZEBLAST:FIRELASER,p.pal,7,18);
        if(b.kind==DISC && gameTicks%3==0 && Alive(b.seat)){
            int target=ClosestEnemy(p.x,p.y,p.z,p.sectnum,450+b.rank*45);
            if(target>=0 && DamageTarget(b.seat,target,3+b.rank*2,FIRELASER)){
                EffectAt(p.x,p.y,p.z,p.sectnum,TRANSPORTERSTAR,7,9,18);
            }
        }
    }
}
static void AdvancedOrdnanceTick(int seat){
    if(!Alive(seat))return;
    auto const &f=fighters[seat];
    int mapping[4]={BOUNCY_BOMBS,RICOCHET_DISCS,ICE_BOMBS,ARC_GRENADES};
    for(int kind=0;kind<4;++kind){
        int rank=f.skill[mapping[kind]];
        if(!rank)continue;
        const bool evolved=EvolutionName(f,mapping[kind])[0]!=0;
        int period=Max(22,(kind==DISC?72:kind==PIPEBOMB?92:kind==CRYO_BOMB?115:98)
                        -rank*7-f.skill[COOLING]*4-(evolved?12:0));
        if((gameTicks+kind*23+seat*11)%period==0){
            int count=Clamp(1+rank/3+f.skill[7]/4+(evolved?1:0),1,4);
            for(int j=0;j<count;++j)ThrowWeapon(seat,kind,rank);
        }
    }
    if(f.skill[EXPANDER_RAY] && gameTicks%Max(23,87-f.skill[EXPANDER_RAY]*6-f.skill[COOLING]*4)==0){
        int target=ClosestEnemy(sprite[g_player[seat].ps->i].x,
                     sprite[g_player[seat].ps->i].y,
                     sprite[g_player[seat].ps->i].z,
                     sprite[g_player[seat].ps->i].sectnum,kTargetRadius);
        if(target>=0){
            int tile=HasArt(GROWSPARK)?GROWSPARK:RPG;
            if(tile==GROWSPARK)++expandShots;
            else ++missingOptionalTiles;  // early GRPs have no Grower art.
            auto ps=g_player[seat].ps;
            int old=ps->q16ang;
            ps->q16ang=fix16_from_int(getangle(sprite[target].x-sprite[ps->i].x,
                                               sprite[target].y-sprite[ps->i].y));
            BudgetShoot(ps->i,tile);
            // Native body expansion works even where shareware's optional
            // GROWSPARK graphic is absent (the RPG is art-only fallback).
            const bool wasShrunk=afflictions[target].kind==EC_MINI &&
                                 gameTicks<afflictions[target].until;
            if(DamageTarget(seat,target,3+f.skill[EXPANDER_RAY]*2,RADIUSEXPLOSION)){
                AfflictEnemy(seat,target,EC_GROWING,f.skill[EXPANDER_RAY]);
                if(wasShrunk){
                    // Reversed quantum compression: instant body expansion and
                    // a friendly-safe enemy-only shockwave. Actual HP changes.
                    const auto &enemy=sprite[target];
                    ConfirmBlastDamage(seat,enemy.x,enemy.y,enemy.z,enemy.sectnum,
                                       1100+f.skill[EXPANDER_RAY]*160,
                                       9+f.skill[EXPANDER_RAY]*4);
                    EffectAt(enemy.x,enemy.y,enemy.z-(12<<8),enemy.sectnum,
                             EXPLOSION2,0,17,40);
                    ++v47TransformerReactions;
                }
            }
            ps->q16ang=old;
            if(tile==RPG)EffectAt(sprite[target].x,sprite[target].y,sprite[target].z,
                           sprite[target].sectnum,TRANSPORTERSTAR,6,12,35);
        }
    }
}
static void LightningTick(int seat,int rank){
    if(gameTicks%Max(9,65-rank*4-fighters[seat].skill[COOLING]*3))return;
    auto const &p=sprite[g_player[seat].ps->i];
    uint8_t used[MAXSPRITES]={};
    int x=p.x,y=p.y,z=p.z-(18<<8);int16_t sec=p.sectnum;
    int chains=Clamp(2+rank/2+(fighters[seat].skill[ELECTRIC_FURY]>3),2,8);
    for(int n=0;n<chains;++n){
        int target=ClosestEnemy(x,y,z,sec,n?5000:14000,used);
        if(target<0)break;
        used[target]=1;
        auto const e=sprite[target];
        BeamTraceResult const arc=DrawThickBeam(x,y,z,sec,e.x,e.y,e.z-(9<<8),
                                                 (n&1)?7:1,9+rank,Clamp(3+rank/2,3,7),
                                                 7,nullptr,BP_STOP_AT_WATER,true);
        int const hits=BeamDamageCrossings(arc,seat,150+rank*18,
            6+rank*5+fighters[seat].skill[AMPLIFIER]*2+
            fighters[seat].skill[ELECTRIC_FURY]*3,FIRELASER,EC_SHOCKED,rank,4,1);
        if(hits){
            EffectAt(arc.x,arc.y,arc.z,arc.endSector,TRANSPORTERSTAR,7,9,26);
            arcHits+=hits;
        }
        // Chain never jumps through solid world geometry or the water surface.
        if(arc.surface!=BS_NONE)break;
        x=e.x;y=e.y;z=e.z-(9<<8);sec=e.sectnum;
    }
}
static void PlaceTripwire(int seat,int rank){
    if(!Alive(seat)||!OwnWorld())return;
    int slot=-1;for(int j=0;j<16;++j)if(tripwires[j].id<0){slot=j;break;}
    if(slot<0)return;
    auto const &p=sprite[g_player[seat].ps->i];
    int ang=(g_player[seat].ps->q16ang>>16)&2047;
    int x=p.x+static_cast<int>((static_cast<int64_t>(sintable[(ang+512)&2047])*1400)/16384);
    int y=p.y+static_cast<int>((static_cast<int64_t>(sintable[ang])*1400)/16384);
    int16_t sec=p.sectnum;updatesector(x,y,&sec);
    if(sec<0||sec>=numsectors)return;
    int id=A_InsertSprite(sec,x,y,p.z-(16<<8),TRIPBOMB,0,22,22,ang,0,0,p.owner>=0?p.owner:g_player[seat].ps->i,STAT_FX);
    if(id<0)return;
    sprite[id].owner=g_player[seat].ps->i;
    Tripwire &t=tripwires[slot];
    t=Tripwire{};t.id=id;t.seat=seat;t.life=Max(75,340+rank*40);t.rank=rank;
    t.pitch=Clamp((g_player[seat].ps->q16horiz>>16)-100,-65,65);
    t.heading=ang; // Beam aims in actual placement direction; no arbitrary +90 yaw.
}
static void DeleteTripBeams(Tripwire &trap){for(int &id:trap.beams)BeamRecycleId(id);}
static BeamTraceResult UpdateTripBeams(Tripwire &trap,int x,int y,int z,int16_t sec,int angle,int range){
    int xx=x+(int)((int64_t)sintable[(angle+512)&2047]*range/16384);
    int yy=y+(int)((int64_t)sintable[angle]*range/16384);
    // Build Z grows downwards; capture actual pitch at placement time.
    const int zz=z+(int)((int64_t)trap.pitch*range*8/128);
    BeamTraceResult ray=DrawThickBeam(x,y,z,sec,xx,yy,zz,2,10+trap.rank,8,6,trap.beams,
                                    BP_STOP_AT_WATER|BP_NO_SPARKS);
    trap.originX=ray.x0;trap.originY=ray.y0;trap.originZ=ray.z0;
    trap.endX=ray.x;trap.endY=ray.y;trap.endZ=ray.z;
    return ray;
}
// Trigger contacts use real 3D beam endpoints, including the owner/player.
// Mine explosions ONLY damage hostile actors via ConfirmBlastDamage, never players.
static bool BeamCrossesPlayer(BeamTraceResult const& ray,int seat,int radius){
    if(seat<0||seat>=MAXPLAYERS||!g_player[seat].ps)return false;
    const auto *p=g_player[seat].ps;
    if(p->i<0||p->i>=MAXSPRITES||sprite[p->i].extra<=0)return false;
    const int64_t vx=(int64_t)ray.x-ray.x0,vy=(int64_t)ray.y-ray.y0;
    const int64_t len2=vx*vx+vy*vy;if(len2<1)return false;
    const int64_t dx=(int64_t)sprite[p->i].x-ray.x0,dy=(int64_t)sprite[p->i].y-ray.y0;
    const int64_t t=dx*vx+dy*vy;
    if(t<0||t>len2)return false;
    // Ignore the immediate emitter capsule: owning player can plant a mine
    // without detonating it while moving away from the placement point.
    if(t/(int64_t)Max(1,(int)std::sqrt((double)len2))<320)return false;
    const int64_t cx=ray.x0+(vx*t/len2),cy=ray.y0+(vy*t/len2);
    const int64_t horizontal=(int64_t)(sprite[p->i].x-cx)*(sprite[p->i].x-cx)+
                             (int64_t)(sprite[p->i].y-cy)*(sprite[p->i].y-cy);
    const int64_t hitRadius=Max(160,radius);
    if(horizontal>hitRadius*hitRadius)return false;
    const int64_t z=ray.z0+((int64_t)(ray.z-ray.z0)*t/len2);
    // Player eye and feet bound the character capsule; never use sprite default Z.
    const int64_t eye=p->pos.z,feet=sprite[p->i].z;
    return z>=Min((int)eye,(int)feet)-(12<<8) &&
           z<=Max((int)eye,(int)feet)+(12<<8);
}
// Swept XY segment crossing prevents sprint/teleport movement from jumping over
// a narrow beam between two simulation ticks. Native beam Z remains authoritative.
static bool BeamCrossesPlayerSweep(BeamTraceResult const& ray,int seat){
    if(seat<0||seat>=MAXPLAYERS||!tripPrevReady[seat]||!g_player[seat].ps)return false;
    const auto *p=g_player[seat].ps;
    if(p->i<0||p->i>=MAXSPRITES||sprite[p->i].extra<=0)return false;
    const int64_t ax=ray.x0,ay=ray.y0,bx=ray.x,by=ray.y;
    const int64_t px=tripPrevX[seat],py=tripPrevY[seat];
    const int64_t dx=sprite[p->i].x-px,dy=sprite[p->i].y-py;
    const int64_t rx=bx-ax,ry=by-ay;
    const int64_t den=rx*dy-ry*dx;
    if(den==0)return false; // Collinear contact is handled by capsule test.
    const int64_t numer=(px-ax)*dy-(py-ay)*dx;
    const int64_t across=(px-ax)*ry-(py-ay)*rx;
    const double beamT=(double)numer/(double)den;
    const double moveT=(double)across/(double)den;
    if(beamT<0||beamT>1||moveT<0||moveT>1)return false;
    const double beamLength=std::sqrt((double)(rx*rx+ry*ry));
    if(beamT*beamLength<320)return false; // planted mine emitter dead zone
    const int64_t beamZ=ray.z0+(int64_t)((ray.z-ray.z0)*beamT);
    const int eye=p->pos.z,feet=sprite[p->i].z;
    return beamZ>=Min(eye,feet)-(16<<8) && beamZ<=Max(eye,feet)+(16<<8);
}
static void TripwiresTick(){
    for(auto &trap:tripwires){
        const int id=trap.id;if(id<0)continue;
        if(id>=MAXSPRITES||sprite[id].statnum!=STAT_FX||sprite[id].picnum!=TRIPBOMB||
           !Alive(trap.seat)||--trap.life<=0){
            if(id>=0 && id<MAXSPRITES && sprite[id].statnum==STAT_FX && sprite[id].picnum==TRIPBOMB)A_DeleteSprite(id);
            DeleteTripBeams(trap);trap=Tripwire{};continue;
        }
        auto const &p=sprite[id];
        const int radius=13500+trap.rank*1100;
        const int tileHeight=Max(1,(int)tilesiz[TRIPBOMB].y);
        // World attachment point: Build sprite Z is not its visual center.
        const int muzzleZ=p.z-(tileHeight*(int)p.yrepeat*4);
        // Trace every simulation tick so a fast-moving player can't skip it.
        // Reuse 16 persistent strips rather than creating new sprite debris.
        BeamTraceResult const ray=UpdateTripBeams(trap,p.x,p.y,muzzleZ,p.sectnum,trap.heading,radius);
        if(trap.armDelay>0){--trap.armDelay;continue;}
        int target=-1;
        for(int enemy=0;enemy<MAXSPRITES;++enemy){
            if(BeamCrossesEnemy(ray,enemy,150+trap.rank*35)) {target=enemy;break;}
        }
        bool playerTriggered=false;
        for(int seat=0;seat<MAXPLAYERS;++seat){
            if(BeamCrossesPlayer(ray,seat,160)||BeamCrossesPlayerSweep(ray,seat)){playerTriggered=true;break;}
        }
        if(target<0&&!playerTriggered)continue;
        // Intentional FRIENDLY tripmine: players can detonate, but are not damaged.
        // A_RadiusDamage would also harm Duke, so use enemy-only confirmed damage.
        ConfirmBlastDamage(trap.seat,p.x,p.y,p.z,p.sectnum,
                           2000+trap.rank*220,18+trap.rank*6);
        A_Spawn(id,EXPLOSION2);
        ++tripwireDetonations;
        DeleteTripBeams(trap);A_DeleteSprite(id);trap=Tripwire{};
    }
    for(int seat=0;seat<MAXPLAYERS;++seat){
        auto const *p=g_player[seat].ps;
        if(!p||p->i<0||p->i>=MAXSPRITES||sprite[p->i].statnum>=MAXSTATUS){tripPrevReady[seat]=false;continue;}
        tripPrevX[seat]=sprite[p->i].x;tripPrevY[seat]=sprite[p->i].y;
        tripPrevReady[seat]=true;
    }
}
// DXV29: a true world-space burning field, not a rebranded rocket volley.
static void PlaceNapalmField(int seat,int rank){
    if(!OwnWorld()||!Alive(seat))return;
    auto const &p=sprite[g_player[seat].ps->i];
    int enemy=ClosestEnemy(p.x,p.y,p.z,p.sectnum,18000);
    if(enemy<0)return;
    int slot=-1;
    for(int i=0;i<12;++i)if(napalmFields[i].id<0){slot=i;break;}
    if(slot<0)return;
    auto const &e=sprite[enemy];
    int const id=A_InsertSprite(e.sectnum,e.x,e.y,e.z-(6<<8),FIRE,-20,32,32,0,0,0,-1,STAT_FX);
    if(id<0)return;
    sprite[id].pal=0;
    napalmFields[slot]={id,seat,120+rank*25,rank};
    ++napalmBursts;
}
static void NapalmFieldsTick(){
    for(auto &field:napalmFields){
        const int id=field.id;
        if(id<0)continue;
        if(id>=MAXSPRITES || sprite[id].picnum!=FIRE || sprite[id].statnum!=STAT_FX ||
           !Alive(field.seat) || --field.life<=0){
            if(id>=0&&id<MAXSPRITES&&sprite[id].picnum==FIRE&&sprite[id].statnum==STAT_FX)
                A_DeleteSprite(id);
            field=NapalmField{};continue;
        }
        auto const &fire=sprite[id];
        if(gameTicks%7==0){
            EffectAt(fire.x+((gameTicks*173)&255)-128,fire.y+((gameTicks*127)&255)-128,
                     fire.z,fire.sectnum,FIRE2,0,12,30);
        }
        if(gameTicks%12==0){
            int target=ClosestEnemy(fire.x,fire.y,fire.z,fire.sectnum,1150+field.rank*125);
            if(target>=0 && DamageTarget(field.seat,target,5+field.rank*3+fighters[field.seat].skill[AMPLIFIER],RADIUSEXPLOSION))
                AfflictEnemy(field.seat,target,EC_BURNING,field.rank);
        }
    }
}
static void AuraVisual(int seat,int radius,int pal,int tile){
    // DXV29: the *caller* owns each ability cooldown. The old nested %15
    // check suppressed most shockwave and pulse-drone effects while damage hit.
    if(!Alive(seat))return;
    auto const &p=sprite[g_player[seat].ps->i];
    for(int n=0;n<10;++n){
        int ang=(n*2048/10+gameTicks*3)&2047;
        int x=p.x+static_cast<int>((static_cast<int64_t>(sintable[(ang+512)&2047])*radius)/16384);
        int y=p.y+static_cast<int>((static_cast<int64_t>(sintable[ang])*radius)/16384);
        int16_t sec=p.sectnum;updatesector(x,y,&sec);
        if(sec>=0 && sec<numsectors)EffectAt(x,y,p.z-(13<<8),sec,tile,pal,14,24);
    }
}

static void SatelliteTick(int seat,int rank){
    if(seat!=myconnectindex||!Alive(seat))return;
    auto const& p=sprite[g_player[seat].ps->i];
    const int count=Min(12,2+rank+fighters[seat].skill[7]/2+
        (EvolutionName(fighters[seat],SATELLITES)[0]?2:0));
    const int orbit=850+rank*100+fighters[seat].skill[DURATION]*90;
    for(int i=0;i<12;++i){
        int &id=satelliteIds[i];
        if(i>=count){
            if(id>=0 && sprite[id].statnum==STAT_FX && sprite[id].owner==g_player[seat].ps->i)A_DeleteSprite(id);
            id=-1;continue;
        }
        const int ang=(gameTicks*(18+rank*2)+i*2048/count)&2047;
        const int x=p.x+static_cast<int>((static_cast<int64_t>(sintable[(ang+512)&2047])*orbit)/16384);
        const int y=p.y+static_cast<int>((static_cast<int64_t>(sintable[ang])*orbit)/16384);
        int16_t sec=p.sectnum;updatesector(x,y,&sec);
        if(sec<0||sec>=numsectors)continue;
        if(id<0||id>=MAXSPRITES || sprite[id].statnum!=STAT_FX || sprite[id].picnum!=OrbitalTile()){
            int tile=OrbitalTile();
            int repeat=ArtRepeat(tile,48);
            id=A_InsertSprite(sec,x,y,p.z-(14<<8),tile,0,repeat,repeat,ang,0,0,p.owner>=0?p.owner:g_player[seat].ps->i,STAT_FX);
            if(id<0)continue;
            sprite[id].owner=g_player[seat].ps->i;
            sprite[id].pal=7;
            sprite[id].shade=-28;
        }
        sprite[id].x=x;sprite[id].y=y;sprite[id].z=p.z-(14<<8);
        sprite[id].xrepeat=static_cast<uint8_t>(Clamp(ArtRepeat(OrbitalTile(),48)+rank*2,14,72));
        sprite[id].yrepeat=sprite[id].xrepeat;
        if(gameTicks%13==0)EffectAt(x,y,sprite[id].z,sec,TRANSPORTERSTAR,7,8,20);
        if(sprite[id].sectnum!=sec)changespritesect(id,sec);
    }
    // Orbital collisions use each satellite's OWN world position, not an
    // invisible circle centered on Duke. Their sprites and hit areas match.
    if(gameTicks%9==0){
        for(int i=0;i<count;++i){
            int id=satelliteIds[i];
            if(id<0 || id>=MAXSPRITES || sprite[id].statnum!=STAT_FX)continue;
            const auto &s=sprite[id];
            int enemy=ClosestEnemy(s.x,s.y,s.z,s.sectnum,350+rank*32);
            if(enemy>=0 && DamageTarget(seat,enemy,3+rank*3+fighters[seat].skill[AMPLIFIER],FIRELASER)){
                EffectAt(sprite[enemy].x,sprite[enemy].y,sprite[enemy].z,
                        sprite[enemy].sectnum,TRANSPORTERSTAR,7,12,25);
                if(EvolutionName(fighters[seat],SATELLITES)[0])
                    AfflictEnemy(seat,enemy,EC_SHOCKED,rank);
            }
        }
    }
}
// DXV32 fusion matrix: actual cross-family damage and status delivery.
// Each attack runs in the original Build simulation, has a hard cooldown,
// and uses the same confirmed native hit function as individual weapons.
static int fusionProcCount=0;
static void FusionMatrixTick(int seat){
    if(!Alive(seat))return;
    auto const &f=fighters[seat];
    auto const &body=sprite[g_player[seat].ps->i];
    // Paired ranks >=3 ensure the fusion is earned, not decorative UI text.
    if(f.skill[FREEZE]>=3 && f.skill[THUNDER_CHAIN]>=3 && gameTicks%64==0){
        int t=ClosestEnemy(body.x,body.y,body.z,body.sectnum,12000);
        if(t>=0 && DamageTarget(seat,t,17+f.skill[FREEZE]*4,FREEZEBLAST)){
            AfflictEnemy(seat,t,EC_FROZEN,f.skill[FREEZE]);
            EffectLine(body.x,body.y,body.z-(12<<8),body.sectnum,
                       sprite[t].x,sprite[t].y,sprite[t].z-(12<<8),FIRELASER,7,8);
            ++fusionProcCount;
        }
    }
    if(f.skill[SHRINK]>=3 && f.skill[SPINAL_RAIL]>=3 && gameTicks%53==0){
        int t=ClosestEnemy(body.x,body.y,body.z,body.sectnum,18000);
        if(t>=0 && DamageTarget(seat,t,15+f.skill[SPINAL_RAIL]*5,SHRINKSPARK)){
            AfflictEnemy(seat,t,EC_MINI,f.skill[SHRINK]);
            EffectLine(body.x,body.y,body.z-(12<<8),body.sectnum,
                       sprite[t].x,sprite[t].y,sprite[t].z-(12<<8),LASERLINE,2,8);
            ++fusionProcCount;
        }
    }
    if(f.skill[EXPANDER_RAY]>=3 && f.skill[DX_NAPALM_WAVE]>=3 && gameTicks%75==0){
        int t=ClosestEnemy(body.x,body.y,body.z,body.sectnum,14000);
        if(t>=0 && DamageTarget(seat,t,17+f.skill[EXPANDER_RAY]*5,RADIUSEXPLOSION)){
            AfflictEnemy(seat,t,EC_GROWING,f.skill[EXPANDER_RAY]);
            EffectAt(sprite[t].x,sprite[t].y,sprite[t].z,sprite[t].sectnum,FIRE,2,22,45);
            ++fusionProcCount;
        }
    }
    if(f.skill[RADIATION]>=3 && f.skill[ELECTRIC_AURA]>=3 && gameTicks%42==0){
        int t=ClosestEnemy(body.x,body.y,body.z,body.sectnum,2600+f.skill[RADIATION]*210);
        if(t>=0 && DamageTarget(seat,t,10+f.skill[RADIATION]*3,RADIUSEXPLOSION)){
            AfflictEnemy(seat,t,EC_SHOCKED,f.skill[ELECTRIC_AURA]);
            EffectAt(sprite[t].x,sprite[t].y,sprite[t].z,sprite[t].sectnum,FIRELASER,7,18,35);
            ++fusionProcCount;
        }
    }
    if(f.skill[RAILGUN]>=3 && f.skill[CRYO_SHATTER]>=3 && gameTicks%68==0){
        int t=ClosestEnemy(body.x,body.y,body.z,body.sectnum,18000);
        if(t>=0 && DamageTarget(seat,t,18+f.skill[RAILGUN]*5,FREEZEBLAST)){
            AfflictEnemy(seat,t,EC_FROZEN,f.skill[CRYO_SHATTER]);
            EffectLine(body.x,body.y,body.z-(10<<8),body.sectnum,
                       sprite[t].x,sprite[t].y,sprite[t].z-(10<<8),LASERLINE,1,10);
            ++fusionProcCount;
        }
    }
    if(f.skill[MINES]>=3 && f.skill[THUNDER_CHAIN]>=3 && gameTicks%79==0){
        // Tesla mines: arc from an EXISTING physical tripwire anchor.
        for(auto const& trap:tripwires){
            if(trap.id<0 || trap.seat!=seat || trap.id>=MAXSPRITES)continue;
            const auto &mine=sprite[trap.id];
            if(mine.statnum!=STAT_FX || mine.picnum!=TRIPBOMB)continue;
            int t=ClosestEnemy(mine.x,mine.y,mine.z,mine.sectnum,6100);
            if(t<0)continue;
            if(DamageTarget(seat,t,12+f.skill[THUNDER_CHAIN]*3,RADIUSEXPLOSION)){
                AfflictEnemy(seat,t,EC_SHOCKED,f.skill[THUNDER_CHAIN]);
                EffectLine(mine.x,mine.y,mine.z,mine.sectnum,sprite[t].x,sprite[t].y,
                           sprite[t].z-(10<<8),FIRELASER,7,8);
                ++fusionProcCount;
            }
            break;
        }
    }
    if(f.skill[SATELLITES]>=3 && f.skill[SHRINK_SWARM]>=3 && gameTicks%58==0){
        for(int i=0;i<12;++i){
            int id=satelliteIds[i];if(id<0 || sprite[id].statnum!=STAT_FX)continue;
            const auto &orb=sprite[id];
            int t=ClosestEnemy(orb.x,orb.y,orb.z,orb.sectnum,760);
            if(t>=0 && DamageTarget(seat,t,7+f.skill[SATELLITES]*3,SHRINKSPARK)){
                AfflictEnemy(seat,t,EC_MINI,f.skill[SHRINK_SWARM]);++fusionProcCount;
            }
        }
    }
    if(f.skill[BOUNCY_BOMBS]>=3 && f.skill[DX_NAPALM_WAVE]>=3 && gameTicks%91==0){
        if(ThrowWeapon(seat,PIPEBOMB,f.skill[BOUNCY_BOMBS])){
            PlaceNapalmField(seat,f.skill[DX_NAPALM_WAVE]);++fusionProcCount;
        }
    }
    if(f.skill[RICOCHET_DISCS]>=3 && f.skill[PIERCING_RAILS]>=3 && gameTicks%59==0){
        int t=ClosestEnemy(body.x,body.y,body.z,body.sectnum,14000);
        if(t>=0 && DamageTarget(seat,t,9+f.skill[PIERCING_RAILS]*4,FIRELASER)){
            EffectLine(body.x,body.y,body.z-(11<<8),body.sectnum,
                       sprite[t].x,sprite[t].y,sprite[t].z-(11<<8),LASERLINE,2,8);
            ++fusionProcCount;
        }
    }
    if(f.skill[ICE_BOMBS]>=3 && f.skill[SHOCKWAVE]>=3 && gameTicks%74==0){
        int t=ClosestEnemy(body.x,body.y,body.z,body.sectnum,3700);
        if(t>=0 && DamageTarget(seat,t,10+f.skill[SHOCKWAVE]*4,FREEZEBLAST)){
            AfflictEnemy(seat,t,EC_FROZEN,f.skill[ICE_BOMBS]);
            AuraVisual(seat,2100+f.skill[SHOCKWAVE]*100,1,TRANSPORTERSTAR);
            ++fusionProcCount;
        }
    }
    if(f.skill[ARC_GRENADES]>=3 && f.skill[CRYO_SHATTER]>=3 && gameTicks%88==0){
        int t=ClosestEnemy(body.x,body.y,body.z,body.sectnum,12000);
        if(t>=0 && DamageTarget(seat,t,13+f.skill[ARC_GRENADES]*3,FREEZEBLAST)){
            AfflictEnemy(seat,t,EC_FROZEN,f.skill[CRYO_SHATTER]);
            EffectLine(body.x,body.y,body.z-(10<<8),body.sectnum,
                        sprite[t].x,sprite[t].y,sprite[t].z-(10<<8),FIRELASER,1,7);
            ++fusionProcCount;
        }
    }
    if(f.skill[PULSE_DRONES]>=3 && f.skill[DRONE_DETONATION]>=3 && gameTicks%65==0){
        AuraVisual(seat,1850+f.skill[DRONE_DETONATION]*180,7,TRANSPORTERSTAR);
        HitNearby(seat,1750+f.skill[PULSE_DRONES]*200,
                  11+f.skill[DRONE_DETONATION]*6,12);
        ++fusionProcCount;
    }
}
static void AbilityTick(int seat){
    if(!Alive(seat))return;
    Fighter const& f=fighters[seat];
    int const scan=NearestEnemyAngle(seat,kTargetRadius+f.skill[PROXIMITY_RADAR]*2800);
    FusionMatrixTick(seat);
    if(f.skill[SATELLITES])SatelliteTick(seat,f.skill[SATELLITES]);
    if(f.skill[RADIATION] && gameTicks%10==0)
        AuraVisual(seat,1100+f.skill[RADIATION]*190,6,TRANSPORTERSTAR);
    if(f.skill[SHOCKWAVE] && gameTicks%Max(12,75-f.skill[COOLING]*7)==0)
        AuraVisual(seat,2100+f.skill[SHOCKWAVE]*280,7,FIRELASER);
    if(f.skill[THUNDER_CHAIN])LightningTick(seat,f.skill[THUNDER_CHAIN]);
    if(f.skill[PULSE_DRONES] && gameTicks%Max(9,51-f.skill[PULSE_DRONES]*3)==0){
        AuraVisual(seat,1600+f.skill[PULSE_DRONES]*150,7,TRANSPORTERSTAR);
        HitNearby(seat,1700+f.skill[PULSE_DRONES]*180,f.skill[PULSE_DRONES]*4+f.skill[AMPLIFIER],30);
    }
    if(f.skill[RADIATION] && gameTicks%Max(5,17-f.skill[COOLING]*2)==0){
        HitNearby(seat,1100+f.skill[RADIATION]*190+f.skill[DURATION]*160,
          f.skill[RADIATION]*2+f.skill[AMPLIFIER]*2+
          (EvolutionName(f,RADIATION)[0]?6:0),35);
        if(EvolutionName(f,RADIATION)[0] && gameTicks%45==0){
            auto &health=sprite[g_player[seat].ps->i].extra;
            health=Min(100+f.skill[16]*15,health+2);
        }
    }
    if(f.skill[SHOCKWAVE] && gameTicks%(Max(12,75-f.skill[COOLING]*7))==0)
        HitNearby(seat,2100+f.skill[SHOCKWAVE]*280,
          f.skill[SHOCKWAVE]*5+f.skill[AMPLIFIER],64);
    if(f.skill[MINES] && gameTicks%Max(50,135-f.skill[MINES]*8-f.skill[COOLING]*4)==0)
        PlaceTripwire(seat,f.skill[MINES]);
    if(f.skill[RED_BEAM_MULTIPLIER] && gameTicks%Max(60,220-f.skill[RED_BEAM_MULTIPLIER]*14)==0)
        PlaceTripwire(seat,Max(1,f.skill[RED_BEAM_MULTIPLIER]+f.skill[MINES]));
    if(f.skill[ELECTRIC_AURA] && gameTicks%Max(6,17-f.skill[ELECTRIC_AURA])==0){
        HitNearby(seat,1100+f.skill[ELECTRIC_AURA]*210,3+f.skill[ELECTRIC_AURA]*3,20);
        AuraVisual(seat,1100+f.skill[ELECTRIC_AURA]*210,7,TRANSPORTERSTAR);
    }
    if(f.skill[DRONE_DETONATION] && gameTicks%Max(14,68-f.skill[DRONE_DETONATION]*3)==0){
        SatelliteTick(seat,Max(1,f.skill[DRONE_DETONATION]));
        HitNearby(seat,1250+f.skill[DRONE_DETONATION]*160,5+f.skill[DRONE_DETONATION]*4,20);
    }
    if(f.skill[SPINAL_RAIL] && gameTicks%Max(16,55-f.skill[SPINAL_RAIL]*3)==0){
        auto const &body=sprite[g_player[seat].ps->i];
        int target=ClosestEnemy(body.x,body.y,body.z,body.sectnum,18000);
        if(target>=0){
            auto const &enemy=sprite[target];
            EffectLine(body.x,body.y,body.z-(12<<8),body.sectnum,
                enemy.x,enemy.y,enemy.z-(10<<8),LASERLINE,2,8);
            DamageTarget(seat,target,12+f.skill[SPINAL_RAIL]*6,FIRELASER);
        }
    }
    if(scan<0)return;
    for(int skill=SEEKERS;skill<=PIERCING_RAILS;++skill){
        // The physical orbit and radiation fields have their own updates.
        if(skill==SHOCKWAVE || skill==MINES)continue;
        const int rank=f.skill[skill];if(rank==0)continue;
        int base=70;
        int tile=RPG;
        switch(skill){
            case SEEKERS:base=42;tile=RPG;break;
            case MINES:base=128;tile=RPG;break;
            case RAILGUN:base=60;tile=FIRELASER;break;
            case SHRAPNEL:base=72;tile=SHOTSPARK1;break;
            case DX_NAPALM_WAVE:base=112;tile=RPG;break;
            case PLASMA:base=49;tile=FIRELASER;break;
            case CRYO_SHATTER:base=88;tile=FREEZEBLAST;break;
            case SHRINK_SWARM:base=92;tile=SHRINKSPARK;break;
            case ROCKET_CYCLONE:base=120;tile=RPG;break;
            case STORM_GRID:base=71;tile=FIRELASER;break;
            case PIERCING_RAILS:base=40;tile=FIRELASER;break;
            default:continue;
        }
        const int period=Max(6,base*(100-Min(75,rank*4+f.skill[8]*6+f.skill[COOLING]*4))/100);
        if((gameTicks+skill*13+seat*7)%period)continue;
        int const count=Clamp(1+rank/2+f.skill[7]/2+(skill==SHRAPNEL?3:0),1,12);
        int const spread=(skill==ROCKET_CYCLONE?210:skill==MINES?210:skill==SHRAPNEL?125:skill==STORM_GRID?64:25);
        auto p=g_player[seat].ps;
        const int original=p->q16ang;
        for(int j=0;j<count;++j){
            int angle=(scan+(j-(count-1)/2)*spread)&2047;
            if(skill==ROCKET_CYCLONE)angle=(angle+gameTicks*7)&2047;
            if(skill==STORM_GRID && (j%2))angle=(angle+1024)&2047;
            p->q16ang=fix16_from_int(angle);
            if(skill==ROCKET_CYCLONE){
                V47LaunchMissile(seat,0,rank,angle,(j*547+gameTicks*19)&2047);
            }else if(skill!=RAILGUN && skill!=PIERCING_RAILS && skill!=SPINAL_RAIL)
                BudgetShoot(p->i,tile);
        }
        p->q16ang=original;
        // DXV29: different advanced weapons must have their own spatial effects.
        if(skill==DX_NAPALM_WAVE)PlaceNapalmField(seat,rank);
        if(skill==CRYO_SHATTER || skill==SHRINK_SWARM){
            uint8_t used[MAXSPRITES]={};
            for(int strike=0;strike<(skill==SHRINK_SWARM?Clamp(1+rank/2,1,5):1);++strike){
                int enemy=ClosestEnemy(sprite[p->i].x,sprite[p->i].y,sprite[p->i].z,
                                       sprite[p->i].sectnum,kTargetRadius,used);
                if(enemy<0)break;
                used[enemy]=1;
                if(DamageTarget(seat,enemy,3+rank*2,
                         skill==CRYO_SHATTER?FREEZEBLAST:SHRINKSPARK)){
                    AfflictEnemy(seat,enemy,skill==CRYO_SHATTER?EC_FROZEN:EC_MINI,rank);
                    if(skill==SHRINK_SWARM){
                        const auto &body=sprite[p->i],&target=sprite[enemy];
                        EffectLine(body.x,body.y,body.z-(11<<8),body.sectnum,
                                   target.x,target.y,target.z-(9<<8),SHRINKSPARK,6,4);
                    }
                }
            }
        }
        if(skill==RAILGUN || skill==PLASMA || skill==STORM_GRID || skill==PIERCING_RAILS){
            const auto &body=sprite[p->i];
            int target=ClosestEnemy(body.x,body.y,body.z,body.sectnum,16000);
            if(target>=0){
                const auto &enemy=sprite[target];
                bool rail=(skill==RAILGUN || skill==PIERCING_RAILS);
                auto const laser=DrawThickBeam(body.x,body.y,body.z-(14<<8),body.sectnum,
                           enemy.x,enemy.y,enemy.z-(12<<8),rail?2:7,rail?13:9,
                           rail?8:7,9,nullptr,BP_STOP_AT_WATER|BP_HIT_PROPS,!rail);
                const int railHits=BeamDamageCrossings(laser,seat,rail?180:130,
                    (rail?12:7)+rank*5+f.skill[AMPLIFIER]*3,FIRELASER,
                    (skill==PLASMA||skill==STORM_GRID)?EC_SHOCKED:
                    (rail&&f.skill[CRYO_SHATTER]>=4)?EC_FROZEN:EC_NONE,rank,
                    rail?10:3,6);
                // A rat's 7x6 ART or a script-resized target can be smaller
                // than the traced capsule. If unobstructed, one direct hit is
                // still confirmed through native damage, never a fake FX hit.
                if(rail && railHits==0 &&
                   cansee(body.x,body.y,body.z-(14<<8),body.sectnum,
                          enemy.x,enemy.y,enemy.z-(12<<8),enemy.sectnum) &&
                   DamageTarget(seat,target,12+rank*5+f.skill[AMPLIFIER]*3,FIRELASER))
                    ++v47RailGuaranteed;
                if(laser.surface!=BS_SKY && laser.endSector>=0)
                    EffectAt(laser.x,laser.y,laser.z,laser.endSector,
                             TRANSPORTERSTAR,7,8,25);
            }
        }
        // Duke-styled fusions: at 5+ rank paired with a suitable passive,
        // duplicate the original weapon's projectile family in another arc.
        if(EvolutionName(f,skill)[0])
            FireWeapon(seat,(tile==FIRELASER?LASER:tile==FREEZEBLAST?FREEZE:
                    tile==SHRINKSPARK?SHRINK:tile==SHOTSPARK1?SHOTGUN:ROCKET),
                (scan+512)&2047,rank/2);
    }
}
// V38 bridge: the original 0..69 EDuke/Duke-X weapon combat is dispatched
// by the same scheduler as the newly authored families; IDs stay stable.
static void ArsenalLegacyWeaponsTick(int i,int a,int nativeTarget){
    auto &f=fighters[i];
    if(a<0)return;
    for(int w=0;w<kWeaponCount;++w){
            int rank=f.skill[w];
            // Starting loadout is assigned only once in ApplyLoadout().
            if(rank==0)continue;
            int period=0;
            switch(w){
              case PISTOL:period=18;break;
              case SHOTGUN:period=62;break;
              case ROCKET:period=95;break;
              case FREEZE:period=105;break;
              case SHRINK:period=125;break;
              case LASER:period=45;break;
              case DEVASTATOR:period=66;break;
            }
            period=Max(5,period*(100-Min(75,rank*5+f.skill[8]*7+f.skill[11]*3+
                  (f.skill[MOMENTUM_ENGINE]?Min(18,f.combo/5)*f.skill[MOMENTUM_ENGINE]/3:0)))/100);
            int offset=w*13+i*17;
            if((gameTicks+offset)%period)continue;
            if(w==DEVASTATOR){
                const int rockets=Clamp(2+rank/3,2,4);
                for(int j=0;j<rockets;++j){
                    const int offset=(j-(rockets-1)/2)*96;
                    V47LaunchMissile(i,1,rank,(a+offset)&2047,(j*600+gameTicks*7)&2047);
                }
            }else FireWeapon(i,w,a,rank);
            if(w==SHOTGUN)V47ScatterShot(i,a,rank,nativeTarget);
            // The weapon's real AutoAim target receives an applied, verified
            // native hit.  Projectile art and actor simulation still run, but
            // the special status is no longer contingent on a CON branch.
            if(nativeTarget>=0 && (w==PISTOL || w==ROCKET ||
                                     w==FREEZE || w==SHRINK || w==LASER)){
                const int hitTile=WeaponTile(w);
                // V46: A_Shoot's visible projectile alone is not an accepted HP
                // event for native Survivor rats or every CON evasive state.
                // Independently validate the visible target through Build LOS,
                // then dispatch once through the canonical DamageTarget path.
                const auto &src=sprite[g_player[i].ps->i];
                const auto &dst=sprite[nativeTarget];
                if(!cansee(src.x,src.y,src.z-(16<<8),src.sectnum,
                           dst.x,dst.y,dst.z-(12<<8),dst.sectnum))continue;
                int hitDamage=(w==PISTOL?5+rank*3:
                    w==SHOTGUN?7+rank*5:w==ROCKET?11+rank*6:
                    w==DEVASTATOR?9+rank*5:w==LASER?4+rank*2:1+rank);
                hitDamage=Max(1,hitDamage*ItemPercent(f,w)/100);
                if(DamageTarget(i,nativeTarget,hitDamage,hitTile)){
                    if(w==FREEZE)AfflictEnemy(i,nativeTarget,EC_FROZEN,rank);
                    else if(w==SHRINK)AfflictEnemy(i,nativeTarget,EC_MINI,rank);
                    else {
                        auto const &origin=sprite[g_player[i].ps->i];
                        auto const &enemy=sprite[nativeTarget];
                        EffectLine(origin.x,origin.y,origin.z-(12<<8),origin.sectnum,
                                   enemy.x,enemy.y,enemy.z-(12<<8),LASERLINE,2,8);
                    }
                }
            }
            // Genuine weapon evolutions. They use actual Duke projectiles,
            // activated by paired rank/passive combinations.
            if(w==ROCKET && rank>=5 && f.skill[8]>=3)FireWeapon(i,ROCKET,(a+340)&2047,rank/2);
            if(w==FREEZE && rank>=5 && f.skill[9]>=3)FireWeapon(i,FREEZE,(a+1024)&2047,rank/2);
            if(w==SHRINK && rank>=5 && f.skill[12]>=3)FireWeapon(i,SHRINK,(a+512)&2047,rank/2);
            if(w==PISTOL && rank>=5 && f.skill[7]>=3)FireWeapon(i,PISTOL,(a+1024)&2047,rank/2);
        }
}

// DXV38_NATIVE_ARSENAL_RUNTIME: bounded, engine-native scheduler for 50 new
// attack families and 50 passive modules. All contacts feed DamageTarget,
// all lines feed Build hitscan, and orbital sprites live in real sectors.
// Existing 0..69 upgrades remain address-stable; new ids 70..169.
struct ArsenalSpec { int mode,element,pattern,period,radius,damage,count,pal; };
// mode: BOLT/BEAM/ORBIT/AURA/CHAIN/TRAP/SHOCK/GRAVITY
// Pattern changes physical attack geometry, target selection, rate and reach.
static ArsenalSpec const arsenalSpecs[50]={
// 0-9: independent physical bolt movement families
{0,0,0,45,8000,6,3,7},{0,1,1,75,16500,17,2,2},{0,2,2,95,9000,20,3,1},
{0,0,3,60,12000,8,5,7},{0,3,4,80,10500,12,4,0},{0,2,0,65,15000,10,3,1},
{0,1,2,80,9500,10,5,6},{0,2,4,110,12000,22,4,1},{0,0,1,100,17500,19,2,8},{0,1,3,54,8500,7,6,7},
// 10-19: visible source->endpoint beams and native wall clipping
{1,3,0,60,13000,18,1,0},{1,4,1,70,14500,14,2,7},{1,1,2,85,17000,21,2,6},
{1,2,3,80,13000,11,3,1},{1,4,4,72,11500,14,4,7},{1,1,0,78,14000,16,4,2},
{1,0,1,115,21000,30,1,7},{1,4,2,68,16000,13,3,7},{1,1,3,74,14500,19,2,6},{1,3,4,65,17500,12,5,0},
// 20-29: physically persistent independent orbital sprites
{2,1,0,1,700,6,5,6},{2,0,1,1,900,8,3,8},{2,4,2,1,1100,7,4,7},
{2,1,3,1,1400,6,4,6},{2,0,4,1,1250,6,6,7},{2,0,0,1,720,12,2,2},
{2,1,1,1,1600,5,6,8},{2,2,2,1,850,8,4,1},{2,3,3,1,1030,8,5,0},{2,4,4,1,1200,7,3,7},
// 30-39: zone fields, each with distinct geometry, contact and affliction
{3,3,0,45,2800,7,1,6},{3,3,1,48,2200,9,1,0},{3,2,2,52,3400,6,1,1},
{3,4,3,43,2600,6,1,7},{3,0,4,56,3000,6,1,8},{3,1,0,61,3600,13,1,2},
{3,2,1,57,2400,6,1,1},{3,3,2,77,4500,17,1,0},{3,0,3,96,3500,26,1,8},{3,1,4,51,2100,22,1,2},
// 40-49: chained bolts, tripwire, timed hazards, shock rings and hybrids
{4,4,0,64,16000,14,5,7},{1,4,3,65,17000,14,5,7},{5,2,2,126,14000,18,1,1},
{5,4,3,110,14000,20,1,7},{6,3,1,120,2700,15,3,0},{0,4,4,67,11000,9,6,7},
{0,1,1,55,11000,9,5,6},{7,1,0,72,3400,17,1,2},{0,0,2,47,16000,9,4,8},
{4,4,4,90,17000,18,6,7}
};
struct ArsenalOrb { int id=-1,seat=-1,ability=-1,index=0,hit=0;};
static ArsenalOrb arsenalOrbs[96];
static int arsenalFires=0,arsenalContacts=0,arsenalPassiveProcs=0,arsenalActiveOrbs=0;
static void ArsenalRuntimeReset(){
    for(auto &o:arsenalOrbs){
        if(o.id>=0 && o.id<MAXSPRITES && sprite[o.id].statnum==STAT_FX &&
           sprite[o.id].owner>=0 && (sprite[o.id].picnum==OrbitalTile() || sprite[o.id].picnum==V39Art(o.ability)))A_DeleteSprite(o.id);
        o=ArsenalOrb{};
    }
    arsenalFires=arsenalContacts=arsenalPassiveProcs=arsenalActiveOrbs=0;
}
static int v39OriginalFX=0,v39PassiveFX=0,v39FusionFX=0;
static void V39Effect(int seat,int ability,int x,int y,int z,int16_t sec,int life=12){
    if(v39OriginalFX>=96 && gameTicks%kHz)return; // global transient FX budget
    EffectAt(x,y,z,sec,V39Art(ability),0,life,27);
    ++v39OriginalFX;
}
static int ArsenalEffect(int element){
    return element==2?EC_FROZEN:element==3?EC_BURNING:element==4?EC_SHOCKED:EC_NONE;
}
static bool ArsenalStrike(int seat,int enemy,int damage,int element){
    if(!DamageTarget(seat,enemy,damage,element==2?FREEZEBLAST:
             element==4?FIRELASER:element==3?RADIUSEXPLOSION:SHOTSPARK1))return false;
    ++arsenalContacts;
    if(ArsenalEffect(element)!=EC_NONE)AfflictEnemy(seat,enemy,ArsenalEffect(element),1+damage/8);
    return true;
}
static int ArsenalTarget(int seat,int range){
    auto const &p=sprite[g_player[seat].ps->i];
    int t=ClosestEnemy(p.x,p.y,p.z,p.sectnum,range);
    if(t<0)return -1;
    auto const &e=sprite[t];
    if(!cansee(p.x,p.y,p.z-(16<<8),p.sectnum,
               e.x,e.y,e.z-(12<<8),e.sectnum))return -1;
    return t;
}
static void ArsenalOrbitTick(int seat,int ability,ArsenalSpec const& sp,int rank){
    const int owner=g_player[seat].ps->i;
    auto const &body=sprite[owner];
    const int count=Min(6,sp.count+rank/3+fighters[seat].skill[161]/3);
    for(int index=0;index<count;++index){
        ArsenalOrb *slot=nullptr;
        for(auto &o:arsenalOrbs)if(o.id>=0 && o.seat==seat && o.ability==ability && o.index==index){slot=&o;break;}
        if(!slot)for(auto &o:arsenalOrbs)if(o.id<0){slot=&o;break;}
        if(!slot)break;
        const int phase=(gameTicks*(9+sp.pattern*3+rank)+index*2048/count+sp.pattern*142)&2047;
        const int orbit=sp.radius+((sp.pattern==4?sintable[(gameTicks*7+index*341)&2047]:0)*360)/16384;
        const int x=body.x+(int)((int64_t)sintable[(phase+512)&2047]*orbit/16384);
        const int y=body.y+(int)((int64_t)sintable[phase]*orbit/16384);
        int z=body.z-(13<<8)+(sintable[(phase*2)&2047]*((sp.pattern%3)+1)*900)/16384;
        int16_t sec=body.sectnum;updatesectorz(x,y,z,&sec);
        if(sec<0 || sec>=numsectors)continue;
        if(!AutoClear(body.x,body.y,body.z,body.sectnum,x,y))continue;
        const int tile=V39Art(ability)==TRANSPORTERSTAR?OrbitalTile():V39Art(ability);
        if(slot->id<0 || slot->id>=MAXSPRITES || sprite[slot->id].statnum!=STAT_FX || sprite[slot->id].picnum!=tile){
            slot->id=A_InsertSprite(sec,x,y,z,tile,-26,30,30,phase,0,0,owner,STAT_FX);
            if(slot->id<0)continue;
        }
        slot->seat=seat;slot->ability=ability;slot->index=index;
        auto &s=sprite[slot->id];s.owner=owner;s.x=x;s.y=y;s.z=z;s.ang=phase;
        s.pal=sp.pal;s.shade=-32;s.xrepeat=s.yrepeat=(uint8_t)Clamp(26+rank*3,24,64);
        if(s.sectnum!=sec)changespritesect(slot->id,sec);
        if(gameTicks>=slot->hit){
            int t=ClosestEnemy(x,y,z,sec,400+rank*55);
            if(t>=0 && ArsenalStrike(seat,t,sp.damage+rank*3,sp.element))slot->hit=gameTicks+10;
        }
    }
}
static void ArsenalFieldTick(int seat,ArsenalSpec const& sp,int rank){
    auto const &p=sprite[g_player[seat].ps->i];
    int hits=0;
    for(int t=0;t<MAXSPRITES && hits<12;++t){
        if(!IsSurvivorEnemy(t)||sprite[t].extra<=0)continue;
        auto const&e=sprite[t];
        const int64_t dx=(int64_t)e.x-p.x,dy=(int64_t)e.y-p.y;
        int64_t dist=dx*dx+dy*dy;
        const int r=sp.radius+rank*100+fighters[seat].skill[163]*140;
        if(dist>(int64_t)r*r || std::abs(e.z-p.z)>(52<<8))continue;
        if(!cansee(p.x,p.y,p.z-(10<<8),p.sectnum,e.x,e.y,e.z-(10<<8),e.sectnum))continue;
        if(sp.pattern==4 && e.extra>20)continue; // execution field
        if(ArsenalStrike(seat,t,sp.damage+rank*3,sp.element))++hits;
        if(sp.pattern==3){ // vortex pulls actors through original collision
            int angle=getangle(p.x-e.x,p.y-e.y);
            vec3_t motion{(int)(sintable[(angle+512)&2047]*10/16384),
                          (int)(sintable[angle]*10/16384),0};
            A_MoveSprite(t,motion,CLIPMASK0);
        }
    }
    if(sp.pattern==2 || sp.pattern==1)AuraVisual(seat,sp.radius,sp.pal,TRANSPORTERSTAR);
}
static void ArsenalBeamTick(int seat,ArsenalSpec const& sp,int rank){
    const auto &p=sprite[g_player[seat].ps->i];
    int enemy=ArsenalTarget(seat,sp.radius);
    if(enemy<0)return;
    const auto &t=sprite[enemy];
    int base=getangle(t.x-p.x,t.y-p.y);
    const int count=Min(6,sp.count+rank/4);
    for(int k=0;k<count;++k){
        int spread=sp.pattern==0?0:sp.pattern==4?230:sp.pattern==3?96:140;
        int a=(base+(k-(count-1)/2)*spread+(sp.pattern==4?gameTicks*8:0))&2047;
        int xx=p.x+(int)((int64_t)sintable[(a+512)&2047]*sp.radius/16384);
        int yy=p.y+(int)((int64_t)sintable[a]*sp.radius/16384);
        int zz=sp.pattern==2?t.z-(12<<8):p.z-(12<<8);
        auto line=DrawThickBeam(p.x,p.y,p.z-(12<<8),p.sectnum,
               xx,yy,zz,sp.pal,9+rank,8,9,nullptr,BP_STOP_AT_WATER|BP_HIT_PROPS);
        arsenalContacts+=BeamDamageCrossings(line,seat,155+rank*15,sp.damage+rank*4,
            FIRELASER,ArsenalEffect(sp.element),rank,7,5);
    }
}
static int v39WeaponCasting=-1;
static void ArsenalBoltTick(int seat,ArsenalSpec const& sp,int rank){
    auto const& p=sprite[g_player[seat].ps->i];
    int target=ArsenalTarget(seat,sp.radius);
    if(target<0)return;
    int angle=getangle(sprite[target].x-p.x,sprite[target].y-p.y);
    const int count=Min(7,sp.count+rank/3);
    for(int j=0;j<count;++j){
        int a=angle;
        if(sp.pattern==0)a=(a+(j-(count-1)/2)*85)&2047;
        if(sp.pattern==1)a=(a+(j-(count-1)/2)*180)&2047;
        if(sp.pattern==2)a=(a+(j*290+gameTicks*5))&2047;
        if(sp.pattern==3)a=(a+(j-(count-1)/2)*120+gameTicks*4)&2047;
        if(sp.pattern==4)a=(a+j*2048/count+gameTicks*5)&2047;
        int const bolt=SpawnBolt(seat,sp.element==2?NB_COMET:sp.element==1?NB_RECALL:
                  sp.element==0?NB_NEXUS:NB_PRISM,rank,a);
        if(bolt>=0 && v39WeaponCasting>=70 && v39WeaponCasting<120){
            int tile=V39Art(v39WeaponCasting);
            if(tile!=TRANSPORTERSTAR){
                sprite[bolt].picnum=tile;
                for(auto &b:bolts)if(b.id==bolt){b.originalTile=tile;break;}
            }
        }
    }
}
static void ArsenalChainTick(int seat,ArsenalSpec const& sp,int rank){
    int hit=ArsenalTarget(seat,sp.radius);if(hit<0)return;
    auto const& p=sprite[g_player[seat].ps->i];
    int x=p.x,y=p.y,z=p.z-(12<<8);int16_t sec=p.sectnum;
    uint8_t used[MAXSPRITES]={};
    for(int j=0;j<Min(8,sp.count+rank/2);++j){
        if(hit<0 || used[hit])break;
        const auto t=sprite[hit];
        auto line=DrawThickBeam(x,y,z,sec,t.x,t.y,t.z-(9<<8),sp.pal,12,8,9);
        if(BeamCrossesEnemy(line,hit,180) && ArsenalStrike(seat,hit,sp.damage+rank*3,sp.element)){
            used[hit]=1;x=t.x;y=t.y;z=t.z-(9<<8);sec=t.sectnum;
            hit=ClosestEnemy(x,y,z,sec,sp.radius/2,used);
        }else break;
    }
}
static void ArsenalPassiveTick(int seat){
    auto &f=fighters[seat];auto p=g_player[seat].ps;
    auto &hp=sprite[p->i].extra;
    for(int n=0;n<50;++n){
        int rank=f.skill[120+n];if(!rank)continue;
        // Each passive has its own cooldown/condition and an engine-native
        // consequence; 50 IDs do not alias one generic damage multiplier.
        int period=Max(12,210-n%9*15-rank*6);
        if((gameTicks+n*17+seat*7)%period)continue;
        switch(n){
        case 0: if(f.combo>=3)hp=Min(250,hp+rank*2);break;
        case 1: if(hp<40)p->inv_amount[GET_SHIELD]=Min(250,p->inv_amount[GET_SHIELD]+rank*4);break;
        case 2: f.comboTimer=Max(f.comboTimer,rank*20);break;
        case 3: {int t=ArsenalTarget(seat,12000);if(t>=0)ArsenalStrike(seat,t,rank*2,1);break;}
        case 4: {int t=ArsenalTarget(seat,10000);if(t>=0)AfflictEnemy(seat,t,EC_SHOCKED,rank);break;}
        case 5: {hp=Min(200,hp+rank);if(f.combo>=3)HitNearby(seat,1300,rank*2,4);}break;
        case 6: {int t=ArsenalTarget(seat,9000);if(t>=0 && afflictions[t].kind==EC_FROZEN)ArsenalStrike(seat,t,rank*5,2);break;}
        case 7: {if(f.combo>=3){GrantXP(seat,rank);HitNearby(seat,850+rank*100,rank*3,4);}}break;
        case 8: if(hp<85){p->inv_amount[GET_SHIELD]=Min(250,p->inv_amount[GET_SHIELD]+rank*2);HitNearby(seat,1200+rank*110,rank*3,5);}break;
        case 9: {int t=ArsenalTarget(seat,11000);if(t>=0)ArsenalStrike(seat,t,rank*3,0);break;}
        case 10: if(hp<35)hp=Min(250,hp+rank*6);break;
        case 11: {int t=ArsenalTarget(seat,7000);if(t>=0&&IsSurvivorRat(t))ArsenalStrike(seat,t,rank*8,0);break;}
        case 12: {int t=ArsenalTarget(seat,15000);if(t>=0&&sprite[t].picnum==BOSS1)ArsenalStrike(seat,t,rank*12,0);break;}
        case 13: {int t=ArsenalTarget(seat,16000);if(t>=0)ArsenalStrike(seat,t,rank*3,4);break;}
        case 14: {int t=ArsenalTarget(seat,11000);if(t>=0)AfflictEnemy(seat,t,EC_SHOCKED,rank);break;}
        case 15: if(f.combo>5){p->inv_amount[GET_SHIELD]=Min(250,p->inv_amount[GET_SHIELD]+rank*2);int t=ArsenalTarget(seat,8000);if(t>=0)ArsenalStrike(seat,t,rank*2,4);}break;
        case 16: {int t=ArsenalTarget(seat,8000);if(t>=0)AfflictEnemy(seat,t,EC_FROZEN,rank);break;}
        case 17: if(f.combo>=4){int t=ArsenalTarget(seat,9000);if(t>=0)ArsenalStrike(seat,t,rank*7,4);}break;
        case 18: if(f.combo>=8 && (gameTicks%2)==0)GrantXP(seat,rank);break;
        case 19: SpawnSupply(seat,1);break;
        case 20: if(hp<45){int t=ArsenalTarget(seat,16000);if(t>=0)ArsenalStrike(seat,t,rank*9,3);}break;
        case 21: {int t=ArsenalTarget(seat,14000);if(t>=0&&sprite[t].pal==7)ArsenalStrike(seat,t,rank*10,0);break;}
        case 22: {int t=ArsenalTarget(seat,10000);if(t>=0)ArsenalStrike(seat,t,rank*2,1);break;}
        case 23: {int t=ArsenalTarget(seat,5000);if(t>=0)AfflictEnemy(seat,t,EC_MINI,rank);break;}
        case 24: p->inv_amount[GET_STEROIDS]=Min(400,p->inv_amount[GET_STEROIDS]+rank*8);break;
        case 25: if((g_player[seat].input.fvel || g_player[seat].input.svel))HitNearby(seat,1100+rank*110,rank*4,6);break;
        case 26: {int t=ArsenalTarget(seat,16000);if(t>=0)ArsenalStrike(seat,t,rank*5,0);}break;
        case 27: {int t=ArsenalTarget(seat,13000);if(t>=0){auto const&e=sprite[t];auto line=DrawThickBeam(sprite[p->i].x,sprite[p->i].y,sprite[p->i].z-(13<<8),sprite[p->i].sectnum,e.x,e.y,e.z-(12<<8),2,12,8,10);BeamDamageCrossings(line,seat,170,rank*6,FIRELASER,EC_NONE,rank,8,3);}}break;
        case 28: {int t=ArsenalTarget(seat,10500);if(t>=0){auto const&e=sprite[t];vec3_t m{(sprite[p->i].x-e.x)/28,(sprite[p->i].y-e.y)/28,0};A_MoveSprite(t,m,CLIPMASK0);ArsenalStrike(seat,t,rank*2,4);}}break;
        case 29: if(hp<55){HitNearby(seat,2000+rank*130,rank*7,8);AuraVisual(seat,1800,7,FIRELASER);}break;
        case 30: {p->inv_amount[GET_SHIELD]=Min(250,p->inv_amount[GET_SHIELD]+rank*2);HitNearby(seat,750+rank*80,rank*2,4);}break;
        case 31: {for(auto &g:gems)if(g.id>=0 && sprite[g.id].statnum==STAT_FX){auto &v=sprite[g.id];
                 if(std::abs(v.x-sprite[p->i].x)<1800+rank*150 && std::abs(v.y-sprite[p->i].y)<1800+rank*150)
                 {v.x+=(sprite[p->i].x-v.x)/4;v.y+=(sprite[p->i].y-v.y)/4;}}break;}
        case 32: if(f.combo>=10){SpawnBolt(seat,NB_COMET,rank,(gameTicks*31)&2047);}break;
        case 33: HitNearby(seat,1000+rank*150,rank*4,5);break;
        case 34: if(hp>60){int t=ArsenalTarget(seat,11000);if(t>=0)ArsenalStrike(seat,t,rank*6,0);}break;
        case 35: hp=Min(250,hp+rank*2);break;
        case 36: {int t=ArsenalTarget(seat,18500);if(t>=0 && std::abs(sprite[t].z-sprite[p->i].z)>(20<<8)){SpawnBolt(seat,NB_COMET,rank,getangle(sprite[t].x-sprite[p->i].x,sprite[t].y-sprite[p->i].y));}}break;
        case 37: {int t=ArsenalTarget(seat,14000);if(t>=0)ArsenalStrike(seat,t,rank*4,4);break;}
        case 38: {int t=ArsenalTarget(seat,10000);if(t>=0 && afflictions[t].kind==EC_FROZEN)ArsenalStrike(seat,t,rank*9,2);break;}
        case 39: if(ArsenalTarget(seat,10000)>=0 && hp<130)hp=Min(250,hp+rank);break;
        case 40: {int t=ArsenalTarget(seat,11000);if(t>=0)SpawnBolt(seat,NB_NEXUS,rank,getangle(sprite[t].x-sprite[p->i].x,sprite[t].y-sprite[p->i].y));break;}
        case 41: {int t=ArsenalTarget(seat,8000);if(t>=0)ArsenalStrike(seat,t,rank*3,4);break;}
        case 42: {int t=ArsenalTarget(seat,9000);if(t>=0){vec3_t m{(sprite[p->i].x-sprite[t].x)/32,(sprite[p->i].y-sprite[t].y)/32,0};A_MoveSprite(t,m,CLIPMASK0);}break;}
        case 43: HitNearby(seat,1400+rank*160,rank*3,8);break;
        case 44: if(f.combo>=7)HitNearby(seat,2800,rank*10,12);break;
        case 45: if(f.combo>=5){HitNearby(seat,1350+rank*110,rank*5,5);}break;
        case 46: {p->inv_amount[GET_SHIELD]=Min(250,p->inv_amount[GET_SHIELD]+rank);if(f.combo>=4)SatelliteTick(seat,rank);}break;
        case 47: p->inv_amount[GET_JETPACK]=Min(1600,p->inv_amount[GET_JETPACK]+rank*18);break;
        case 48: {int t=ArsenalTarget(seat,11000);if(t>=0){auto const&e=sprite[t];auto line=DrawThickBeam(sprite[p->i].x,sprite[p->i].y,sprite[p->i].z-(12<<8),sprite[p->i].sectnum,e.x,e.y,e.z-(12<<8),6,11,9,10);BeamDamageCrossings(line,seat,200,rank*5,FIRELASER,EC_SHOCKED,rank,8,3);}}break;
        case 49: HitNearby(seat,4200+rank*180,rank*13,14);AuraVisual(seat,3500,2,TRANSPORTERSTAR);break;
        }
        // V39: effect confirmation is drawn in actual world space, not just UI.
        // Every V38 passive has its own dedicated original sprite tile.
        if(v39PassiveFX<48){
            V39Effect(seat,120+n,sprite[p->i].x,sprite[p->i].y,
                sprite[p->i].z-(15<<8),sprite[p->i].sectnum,9);
            ++v39PassiveFX;
        }
        ++arsenalPassiveProcs;
    }
}
// Twelve different native fusion procs. Costs are bounded and timings
// deterministic; works for multiplayer as part of the authoritative world tick.
static int v39FusionEvents=0;
static void V39FusionTick(int seat){
    auto &f=fighters[seat];auto const& p=sprite[g_player[seat].ps->i];
    for(int n=0;n<12;++n){
        auto const&e=v39Fusions[n];
        if(f.skill[e.weapon]<4 || f.skill[e.passive]<2)continue;
        if((gameTicks+n*13+seat*3)%(100+n*11))continue;
        const int rank=Min(f.skill[e.weapon],f.skill[e.passive]);
        int t=ArsenalTarget(seat,14000);
        switch(n){
          case 0: if(t>=0 && ArsenalStrike(seat,t,rank*9,3))
                    sprite[g_player[seat].ps->i].extra=Min(200,sprite[g_player[seat].ps->i].extra+rank*2);
                  break;
          case 1: if(t>=0){AfflictEnemy(seat,t,EC_FROZEN,rank+2);
                    ArsenalStrike(seat,t,rank*14,2);}break;
          case 2: if(t>=0 && afflictions[t].kind==EC_FROZEN)
                    ArsenalStrike(seat,t,rank*20,4);break;
          case 3: for(int j=0;j<Min(4,rank);++j)
                    SpawnBolt(seat,NB_RECALL,rank,(p.ang+j*512)&2047);break;
          case 4: if(t>=0){auto const&v=sprite[t];
                    auto line=DrawThickBeam(p.x,p.y,p.z-(14<<8),p.sectnum,v.x,v.y,v.z-(12<<8),2,17,10,10);
                    BeamDamageCrossings(line,seat,170,rank*12,FIRELASER,EC_NONE,rank,7,6);}break;
          case 5: SatelliteTick(seat,rank+2);HitNearby(seat,1600,rank*9,8);break;
          case 6: if(t>=0){AfflictEnemy(seat,t,EC_SHOCKED,rank+2);ArsenalStrike(seat,t,rank*9,4);}break;
          case 7: if(t>=0 && ArsenalStrike(seat,t,rank*9,3))GrantXP(seat,rank);break;
          case 8: if(t>=0){auto const&v=sprite[t];
                    vec3_t m{(p.x-v.x)/19,(p.y-v.y)/19,0};A_MoveSprite(t,m,CLIPMASK0);
                    ArsenalStrike(seat,t,rank*8,0);}break;
          case 9: if(t>=0){auto const&v=sprite[t];
                    auto line=DrawThickBeam(p.x,p.y,p.z-(12<<8),p.sectnum,v.x,v.y,v.z-(12<<8),7,15,10,10);
                    BeamDamageCrossings(line,seat,170,rank*12,FIRELASER,EC_SHOCKED,rank,7,7);}break;
          case 10: PlaceTripwire(seat,rank+2);HitNearby(seat,1300,rank*3,5);break;
          case 11: PlaceNapalmField(seat,rank+2);HitNearby(seat,2300,rank*7,8);break;
        }
        if(t>=0){auto const &v=sprite[t];V39Effect(seat,e.weapon,v.x,v.y,v.z-(10<<8),v.sectnum,18);}
        else V39Effect(seat,e.weapon,p.x,p.y,p.z-(13<<8),p.sectnum,18);
        ++v39FusionEvents;
    }
}
static void ArsenalRuntimeTick(int seat){
    if(!Alive(seat)||!OwnWorld())return;
    const auto &f=fighters[seat];
    AbilityTick(seat);AdvancedOrdnanceTick(seat);V33AbilitiesTick(seat);
    int nativeTarget=-1;
    int aimAngle=NearestEnemyAngle(seat,kTargetRadius,&nativeTarget);
    ArsenalLegacyWeaponsTick(seat,aimAngle,nativeTarget);
    ArsenalPassiveTick(seat);
    V39FusionTick(seat);
    int budget=9; // max new weapon activations/tick. Old armory uses its own cap.
    for(int w=0;w<50;++w){
        const int rank=f.skill[70+w];if(!rank)continue;
        auto const &s=arsenalSpecs[w];
        if(s.mode==2){ArsenalOrbitTick(seat,70+w,s,rank);continue;}
        const int period=Max(12,s.period-rank*3-f.skill[122]*3-f.skill[157]*3);
        if((gameTicks+w*19+seat*13)%period || budget<=0)continue;
        --budget;++arsenalFires;
        v39WeaponCasting=70+w;
        {auto const& src=sprite[g_player[seat].ps->i];
         int target=ArsenalTarget(seat,s.radius);
         if(target>=0){auto const& e=sprite[target];V39Effect(seat,70+w,e.x,e.y,e.z-(12<<8),e.sectnum);}
         else V39Effect(seat,70+w,src.x,src.y,src.z-(12<<8),src.sectnum);}
        switch(s.mode){
        case 0: ArsenalBoltTick(seat,s,rank);break;
        case 1: ArsenalBeamTick(seat,s,rank);break;
        case 3: ArsenalFieldTick(seat,s,rank);break;
        case 4: ArsenalChainTick(seat,s,rank);break;
        case 5: PlaceTripwire(seat,rank);break;
        case 6: for(int j=0;j<s.count;++j)PlaceNapalmField(seat,rank);break;
        case 7: HitNearby(seat,s.radius+rank*150,s.damage+rank*5,12);
                AuraVisual(seat,s.radius,s.pal,TRANSPORTERSTAR);break;
        }
        v39WeaponCasting=-1;
    }
    arsenalActiveOrbs=0;for(auto const&o:arsenalOrbs)if(o.id>=0)++arsenalActiveOrbs;
}

static void WeaponsTick(){
    if(!OwnWorld())return;
    v39OriginalFX=0;v39PassiveFX=0;
    shotsRemaining=24;blastRequests=0;
    for(bssize_t TRAVERSE_CONNECT(i))if(Alive(i))ArsenalRuntimeTick(i);
}
static void SurvivalTick(int i){
    if(!Alive(i))return;
    auto &f=fighters[i];
    auto p=g_player[i].ps;
    ++f.elapsed;
    if(f.comboTimer>0){--f.comboTimer;if(f.comboTimer==0)f.combo=0;}
    int kills=p->actors_killed;
    if(kills<f.kills)f.kills=kills;
    if(kills>f.kills){
        int delta=kills-f.kills;
        f.kills=kills;
        int oldCombo=f.combo;
        f.combo=Min(999,f.combo+delta);
        f.comboTimer=kHz*5;
        f.bestCombo=Max(f.bestCombo,f.combo);
        // Each 15-kill uninterrupted streak earns a world power cache.
        if(numplayers==1 && oldCombo/15!=f.combo/15 && f.combo>=15)SpawnSupply(i);
        // Duke-native kill callback accounts for deaths that leave corpses;
        // physical gem pickups from deletions add the meaningful XP bulk.
        GrantXP(i,delta);
        f.earnedCredits+=delta;
        if(numplayers==1)careerCredits=Min(1000000,careerCredits+delta);
    }
    if(f.skill[LIFESTEAL] && gameTicks%(kHz*7)==0)
        sprite[p->i].extra=Min(100+f.skill[16]*15,sprite[p->i].extra+f.skill[LIFESTEAL]);
    if(f.skill[15] && gameTicks%(kHz*5)==0){
        auto &health=sprite[p->i].extra;
        health=Min(100+f.skill[16]*15,health+f.skill[15]*2);
    }
    if(f.skill[14] && gameTicks%(kHz*7)==0){
        auto &shield=p->inv_amount[GET_SHIELD];
        shield=Min(100,shield+f.skill[14]*2);
    }
    if(f.skill[TITANIUM_ARMOR] && gameTicks%(kHz*4)==0){
        auto &shield=p->inv_amount[GET_SHIELD];
        shield=Min(200,shield+2+f.skill[TITANIUM_ARMOR]*3);
    }
    if(f.skill[FIELD_MEDIC] && gameTicks%Max(110,480-f.skill[FIELD_MEDIC]*35)==0)
        SpawnSupply(i,1);
    if(f.skill[HEALTH_MAGNET] && gameTicks%Max(240,720-f.skill[HEALTH_MAGNET]*45)==0)
        SpawnSupply(i,1);
    if(f.skill[JET_AFTERBURNER] && gameTicks%(kHz*4)==0)
        p->inv_amount[GET_JETPACK]=Min(1600,p->inv_amount[GET_JETPACK]+f.skill[JET_AFTERBURNER]*12);
    if(f.skill[BERSERKER_STEROIDS] && gameTicks%(kHz*8)==0)
        p->inv_amount[GET_STEROIDS]=Min(400,p->inv_amount[GET_STEROIDS]+f.skill[BERSERKER_STEROIDS]*15);
    if(f.skill[PHOENIX_HEART] && gameTicks%(kHz*3)==0)
        sprite[p->i].extra=Min(250,sprite[p->i].extra+f.skill[PHOENIX_HEART]*2);
    if(f.skill[16] && gameTicks%(kHz*10)==0){
        sprite[p->i].extra=Min(100+f.skill[16]*15,sprite[p->i].extra+f.skill[16]*3);
    }
    // Ensure original damage code never silently converts back to limited ammo.
    for(int w=0;w<MAX_WEAPONS;++w)
        if((p->gotweapon&(1u<<w)) && p->max_ammo_amount[w]>0)
            p->ammo_amount[w]=p->max_ammo_amount[w];
}
static void Tick(){
    SyncRunState();
    if(numplayers<1||myconnectindex<0||myconnectindex>=MAXPLAYERS||!g_player[myconnectindex].ps||ud.pause_on)return;
    auto main=g_player[myconnectindex].ps;
    if(!(main->gm&MODE_GAME)||main->cursectnum<0)return;
    if(!initialized||episode!=ud.volume_number||level!=ud.level_number||
       (lastPar>=0 && main->player_par<lastPar)){
        Reset();episode=ud.volume_number;level=ud.level_number;initialized=true;
        for(bssize_t TRAVERSE_CONNECT(i))ApplyLoadout(i);
        std::printf("DXV16: real Duke multiweapon builds, orbit/aura, loadouts, and fusion loop ready\n");
    }
    lastPar=main->player_par;
    ++gameTicks;
    // Waves increase every 30 seconds; elites, combos, bosses and HP ramp.
    wave=1+gameTicks/(kHz*35);
    for(bssize_t TRAVERSE_CONNECT(i))SurvivalTick(i);
    if(OwnWorld()){
        RatTick();ObserveDeaths();HordeTick();EnemyAggroWatchdogTick();WeaponsTick();NativeBoltTick();V47MissileTick();ThrownTick();TripwiresTick();NapalmFieldsTick();ConditionsTick();VisualsTick();BeamStripsTick();ProcessGems();ProcessSupplies();
    }
}
// DXV48_COMPACT_HUD: 10 compact native icon cells, no full-width telemetry.
static void DrawStatCell(int centerX,int centerY,int tile,int value,int pal=0){
    const int plate=V47Art(9); // plain masked frame with transparent interior
    if(HasArt(plate))rotatesprite_fs(centerX<<16,centerY<<16,17000L,0,
                                     plate,8,0,2|8|64);
    if(!HasArt(tile))tile=SHOTSPARK1;
    rotatesprite_fs((centerX-8)<<16,centerY<<16,7000L,0,
                    tile,-16,pal,2|8|64);
    char buf[15];
    if(value>=10000)std::snprintf(buf,sizeof(buf),"%dK",value/1000);
    else std::snprintf(buf,sizeof(buf),"%d",value);
    printext256(centerX+2,centerY-3,15,0,buf,1);
}
static void DrawHUD(){
    if(!initialized||!Alive(myconnectindex))return;
    // Full-screen choices own the view: no stat cells drawn over the prizes.
    if(chestReveal.seat>=0){DrawChestReveal();return;}
    if(twistChoice.seat>=0){DrawTwistSelection();return;}
    const auto& f=fighters[myconnectindex];
    char buf[140];
    // Keep the XP progress bar at top with rank and a narrow 26-cell fill.
    const int filled=Clamp((int)((int64_t)f.xp*26/Max(1,f.threshold)),0,26);
    std::snprintf(buf,sizeof(buf),"LV%d",f.level);
    printext256(5,3,15,0,buf,1);
    const int crystal=HasArt(CRYSTALAMMO)?CRYSTALAMMO:SHOTSPARK1;
    for(int j=0;j<26;++j)
        rotatesprite_fs((47+j*10)<<16,6<<16,8000L,0,
                        crystal,j<filled?-32:52,j<filled?7:0,2|8|64);
    auto const p=g_player[myconnectindex].ps;
    // Compact two-column stat strip pinned hard-left to free the playfield.
    const int leftX=12,rightX=34;
    DrawStatCell(leftX,22,ATOMICHEALTH,Max(0,sprite[p->i].extra),0);
    DrawStatCell(rightX,22,HEAVYHBOMB,p->inv_amount[GET_SHIELD],0);
    DrawStatCell(leftX,38,RAT,activeEnemies,0);
    DrawStatCell(rightX,38,RPGAMMO,wave,0);
    DrawStatCell(leftX,54,SHOTSPARK1,f.kills,0);
    DrawStatCell(rightX,54,TRIPBOMB,tripwireDetonations,0);
    DrawStatCell(leftX,70,CRYSTALAMMO,careerCredits,0);
    DrawStatCell(rightX,70,RPG,EquippedWeapons(f),0);
    DrawStatCell(leftX,86,crystal,xpVacuum.ticks>0?xpVacuum.ticks/kHz:0,
                 xpVacuum.ticks>0?7:0);
    DrawStatCell(rightX,86,FREEZEBLAST,(gameTicks/kHz)/60,0);
    // A discrete chest arrow is retained without full-width debug text.
    int nearbyChest=-1;int64_t nearSq=INT64_MAX;
    auto const &playerPos=sprite[p->i];
    for(auto const &c:caches)if(c.kind==5 && c.id>=0 && c.id<MAXSPRITES &&
       sprite[c.id].statnum==STAT_FX){
        auto const &ch=sprite[c.id];
        int64_t dx=(int64_t)ch.x-playerPos.x,dy=(int64_t)ch.y-playerPos.y;
        const int64_t d=dx*dx+dy*dy;
        if(d<nearSq){nearSq=d;nearbyChest=c.id;}
    }
    if(nearbyChest>=0){
        const auto &ch=sprite[nearbyChest];
        int heading=(getangle(ch.x-playerPos.x,ch.y-playerPos.y)-playerPos.ang)&2047;
        const char* dir=(heading<256||heading>1792)?"^":heading<1024?">":"<";
        printext256(303,19,14,0,dir,1);
    }
    if(lastBoon>=0 && gameTicks<lastBoonUntil)
        printext256(4,104,lastBoon>=8?2:14,0,lastBoon>=8?"BANE":"BOON",1);
    if(xpVacuum.ticks>0)
        printext256(4,114,7,0,"XP",1);
    // All non-Web builds have a genuine engine-rendered selection screen.
    // Browser builds use the richer HTML cards; game rules stay in C++.
#ifndef __EMSCRIPTEN__
    if(f.unspent && numplayers==1){
        BeginOffer(myconnectindex);
        std::snprintf(buf,sizeof(buf),"DUKE X POWER-UP | %d REMAINING | PRESS 1/2/3 | R=REROLL",f.unspent);
        printext256(26,113,15,0,buf,1);
        for(int n=0;n<3;++n){
            int id=Choice(f,n);
            std::snprintf(buf,sizeof(buf),"%d  %s  RANK %d",n+1,id<0?"MEDKIT REFILL":skillNames[id],id<0?0:f.skill[id]+1);
            printext256(44,144+n*38,14,0,buf,1);
            printext256(58,156+n*38,15,0,id<0?"RESTORE HEALTH":DukeXOfferDesc[id],1);
        }
    }
#endif
    if(f.unspent && numplayers>1){
        std::snprintf(buf,sizeof(buf),"LEVEL UP: CHOOSE 1 / 2 / 3  (%d pending)",f.unspent);
        printext256(8,35,14,0,buf,1);
        for(int n=0;n<3;++n){
            int id=Choice(f,n);
            std::snprintf(buf,sizeof(buf),"%d. %s  [RANK %d]",n+1,id<0?"HEAL":skillNames[id],id<0?0:f.skill[id]);
            printext256(8,47+n*12,14,0,buf,1);
        }
    }
}
// DXV29_PORTABLE_NATIVE: native EDuke32 keyboard adapter, not browser JS.
// Invoked from original drawframe_do: even a paused survivor game still renders.
static void HandleNativeUpgradeKeys(){
#ifndef __EMSCRIPTEN__
    if(twistChoice.seat>=0){
        for(int k=0;k<3;++k){
            int const key=k==0?sc_1:(k==1?sc_2:sc_3);
            if(KB_KeyPressed(key)){KB_ClearKeyDown(key);ChooseTwist(k+1);return;}
        }
        return;
    }
    if(!HasPendingUpgrade())return;
    if(!BeginOffer(myconnectindex))return;
    for(int k=0;k<3;++k){
        int const key=(k==0?sc_1:(k==1?sc_2:sc_3));
        if(KB_KeyPressed(key)){
            KB_ClearKeyDown(key);
            ChooseUpgrade(myconnectindex,k+1);
            return;
        }
    }
    if(KB_KeyPressed(sc_R)){
        KB_ClearKeyDown(sc_R);
        Reroll(myconnectindex);
    }
#endif
}
} // namespace DukeX

// Input injected into EDuke32's own per-frame P_GetInput, BEFORE network
// packets are composed. Native float-independent joystick uses fixed ints.
// DUKE_X_NATIVE_TOUCH_V17: called INSIDE original P_GetInput after keyboard
// bits are staged but BEFORE P_UpdateAngles and input packet construction.
// Modifies the real synced input_t; no untrusted DOM keyboard event required.
extern "C" void DukeX_InjectTouchInput(int playerNum,input_t *input){
    if(playerNum!=myconnectindex || !input)return;
    ++DukeX::touchSamples;
    // DXV23 native run: full deflection uses original Build MAXVEL (90),
    // and SK_RUN is read by Duke's REAL movement simulation (not a fake key).
    if(DukeX::autoMode && numplayers==1 && DukeX::Alive(playerNum) &&
       DukeX::touchX==0 && DukeX::touchY==0){
        DukeX::AutoThink(playerNum);
        input->fvel=DukeX::autoBrain.forward;
        input->svel=DukeX::autoBrain.strafe;

        input->bits|=BIT(SK_RUN);
        // Original synchronized input for obstacle traversal. Auto-open
        // doors/switches during recovery instead of walking into them forever.
        if(DukeX::autoBrain.stuck>=2 && (DukeX::gameTicks%24)==0)
            input->bits|=BIT(SK_OPEN);
        if(DukeX::autoBrain.stuck>=3 && (DukeX::gameTicks%26)==0 &&
           g_player[playerNum].ps->on_ground)
            input->bits|=BIT(SK_JUMP);
    }
    if(DukeX::touchX || DukeX::touchY){
        input->fvel=DukeX::Clamp((-DukeX::touchY*90)/1000,-90,90);
        input->svel=DukeX::Clamp((-DukeX::touchX*90)/1000,-90,90);
        if(DukeX::touchX*DukeX::touchX + DukeX::touchY*DukeX::touchY >= 300*300)
            input->bits |= BIT(SK_RUN);
    }
    // V38: independent look controller, including when manual MOVE stick is held.
    // Only a manual AIM stick suppresses native auto camera/vertical tracking.
    if(DukeX::autoMode && numplayers==1 && DukeX::Alive(playerNum) &&
       DukeX::touchAimX==0 && DukeX::touchAimY==0){
        DukeX::AutoThink(playerNum);
        DukeX::AutoApplyLook(playerNum,input);
    }
    if(DukeX::touchAimX || DukeX::touchAimY){
        // Fixed-point Build angular velocities, applied by P_UpdateAngles.
        // Approximately 95 degrees/sec at 60 input frames/sec, full deflection.
        // DXV23: Preserve SUB-INTEGER look velocities in Build 16.16 format.
        // Previously integer division quantized small motions to zero.
        input->q16avel=fix16_sadd(input->q16avel,static_cast<int>((static_cast<int64_t>(DukeX::touchAimX)*fix16_from_int(9))/1000));
        input->q16horz=fix16_sadd(input->q16horz,static_cast<int>((static_cast<int64_t>(-DukeX::touchAimY)*fix16_from_int(3))/1000));
    }
    if(DukeX::touchButtons&1)input->bits|=BIT(SK_JUMP);
    if(DukeX::touchButtons&2)input->bits|=BIT(SK_CROUCH);
    if(DukeX::touchButtons&4)input->bits|=BIT(SK_OPEN);
    if(DukeX::touchButtons&8)input->bits|=BIT(SK_QUICK_KICK);
    // Inventory actions go through the SAME synchronized input bits as Duke's
    // keyboard/SDL controls. No direct JS modification of jetpack_on.
    if(DukeX::inventoryActions){
        uint32_t const a=DukeX::inventoryActions;DukeX::inventoryActions=0;
        if(a&(1u<<1))input->bits|=BIT(SK_INV_LEFT);
        if(a&(1u<<2))input->bits|=BIT(SK_INV_RIGHT);
        if(a&(1u<<3))input->bits|=BIT(SK_INVENTORY);
        if(a&(1u<<4))input->bits|=BIT(SK_JETPACK);
        if(a&(1u<<5))input->bits|=BIT(SK_MEDKIT);
        if(a&(1u<<6))input->bits|=BIT(SK_STEROIDS);
        if(a&(1u<<7))input->bits|=BIT(SK_NIGHTVISION);
        if(a&(1u<<8))input->bits|=BIT(SK_HOLODUKE);
        ++DukeX::inventoryActionCount;
    }
    // Grounded jump plus one or two real midair vertical impulses.
    // Never grant without an upgrade; ordinary Duke jumps remain unchanged.
    auto p=g_player[playerNum].ps;
    if(p){
        if(p->on_ground)DukeX::extraJumpsUsed[playerNum]=0;
        bool const jumpNow=(input->bits&BIT(SK_JUMP))!=0;
        bool const rising=jumpNow && !DukeX::jumpWasHeld[playerNum];
        DukeX::jumpWasHeld[playerNum]=jumpNow;
        auto const& f=DukeX::fighters[playerNum];
        int extra=(f.skill[DukeX::TRIPLE_JUMP]>0?2:(f.skill[DukeX::DOUBLE_JUMP]>0?1:0));
        if(DukeX::initialized && DukeX::Alive(playerNum) && !p->on_ground &&
           !p->jetpack_on && rising && DukeX::extraJumpsUsed[playerNum]<extra){
            p->vel.z=-2400-220*DukeX::Min(4,f.skill[DukeX::DOUBLE_JUMP]+f.skill[DukeX::TRIPLE_JUMP]);
            ++DukeX::extraJumpsUsed[playerNum];++DukeX::airJumpCount;
        }
    }
    DukeX::touchAppliedFvel=input->fvel;
    DukeX::touchAppliedSvel=input->svel;
    DukeX::touchAppliedTurn=DukeX::touchAimX*9/1000;
    DukeX::touchAppliedPitch=(-DukeX::touchAimY*3)/1000;
    DukeX::touchAppliedButtons=DukeX::touchButtons;
}

// Called from original actors.cpp A_DeleteSprite after the replay guard. No
// allocations or original actor mutations here: only an engine-side drop queue.
// Called before original CON actor processing. Only freeze/shock suppress AI;
// dead actors are always allowed to execute their original death scripts.
extern "C" int DukeX_EnemyAIShouldPause(int id){
    if(!DukeX::initialized || id<0 || id>=MAXSPRITES || numplayers!=1)return 0;
    auto const &a=DukeX::afflictions[id];
    auto const &e=sprite[id];
    if(DukeX::IsSurvivorRat(id))return 1; // Native RatTick owns rat AI, not CON's short-lived RAT actor.
    return (a.kind==DukeX::EC_FROZEN || a.kind==DukeX::EC_SHOCKED) &&
           a.picnum==e.picnum && e.extra>0 && DukeX::gameTicks<a.until;
}
// V32.2: an actual miniaturized enemy AND its owned projectiles have less
// offensive power. Invoked from A_IncurDamage's player branch before HP loss.
extern "C" int DukeX_ReduceShrunkEnemyDamage(int attacker,int damage){
    if(!DukeX::initialized || !DukeX::OwnWorld() || damage<=0 ||
       attacker<0 || attacker>=MAXSPRITES)return damage;
    int source=attacker;
    if(!DukeX::IsSurvivorEnemy(source))source=sprite[source].owner;
    if(!DukeX::IsSurvivorEnemy(source))return damage;
    auto const &status=DukeX::afflictions[source];
    return status.kind==DukeX::EC_MINI && status.picnum==sprite[source].picnum &&
           DukeX::gameTicks<status.until ? DukeX::Max(1,damage/5):damage;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V322RatCount(){
    int n=0;for(int id=0;id<MAXSPRITES;++id)if(DukeX::IsSurvivorRat(id))++n;return n;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_SetAutoMode(int value){
    if(numplayers!=1)return 0;
    DukeX::autoMode=value?1:0;
    return DukeX::autoMode;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_GetAutoMode(){return DukeX::autoMode;}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_V36Marker(){return "DXV36: GROUNDED_RATS_GRAVITY_SLOPES_XP_ORBS_SUPPLIES";}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_V38Marker(){return "DXV38: ARSENAL_RUNTIME_170_SKILLS_NATIVE_EFFECTS_AUTOPILOT_COMBAT";}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V38AggroWakes(){return DukeX::nativeAggroWakes;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V38DamageFallbacks(){return DukeX::nativeDamageFallbacks;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V38FailedGoals(){return DukeX::autoBrain.failedGoals;}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_V37Marker(){return "DXV37: NATIVE_AUTOPILOT_VISION_VERTICAL_AIM_PATHS";}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V37AimTarget(){return DukeX::autoBrain.aimTarget;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V37AimHorizon(){return DukeX::autoBrain.aimHorizon;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V37DesiredHeading(){return DukeX::autoBrain.desired;}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_V35Marker(){return "DXV35: SMART_AUTO_NAV_LONG_TRIPWIRE_REAL_LASER_ORIGIN";}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V35AutoDecisions(){return DukeX::autoDecisions;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V35AutoAvoidance(){return DukeX::autoAvoidance;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V35AutoExplorations(){return DukeX::autoExplorations;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V33BoltCount(){
    int count=0;for(auto const &b:DukeX::bolts)if(b.id>=0)++count;return count;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V33BoltsFired(){return DukeX::nativeBoltsFired;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V33BoltDamageHits(){return DukeX::nativeBoltHits;}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_V33Marker(){return "DXV33: NATIVE_BOLT_ARMORY_70_SKILLS_REAL_RATS_AUTO_MODE";}
extern "C" EMSCRIPTEN_KEEPALIVE const char *DukeX_V322Marker(){return "DXV32.2: RAT_AI_SHRUNK_DAMAGE_XP_PULSE";}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_FusionProcCount(){return DukeX::fusionProcCount;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_CombatDamageApplied(){return DukeX::confirmedDamage;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_CombatHitsApplied(){return DukeX::confirmedHits;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_CombatHitsBlocked(){return DukeX::blockedHits;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_CombatLastVictim(){return DukeX::combatLastVictim;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_CombatLastHPBefore(){return DukeX::combatLastHPBefore;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_CombatLastHPAfter(){return DukeX::combatLastHPAfter;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_CombatStatusCount(int condition){
    int count=0;
    for(auto const& a:DukeX::afflictions)if(a.kind==condition && DukeX::gameTicks<a.until)++count;
    return count;
}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_V32Marker(){return "DXV32: IMMEDIATE_REAL_ENEMY_HP_DAMAGE_FREEZE_SHRINK_GROW_STATUS_AI";}
extern "C" void DukeX_OnActorDelete(int id){
    DukeX::QueueDrop(id);
    if(id>=0 && id<MAXSPRITES){
        DukeX::ClearAffliction(id);
        DukeX::survivorRat[id]=0;
        DukeX::enemyObserved[id]=0;
    }
}

// Called from the ACTUAL EDuke32 damage paths, not a post-hit invulnerability
// cheat. Ignore only self-owned radius explosions; other players can be hurt.
extern "C" int DukeX_IgnoreSelfBlast(int projectile,int target){
    if(!DukeX::initialized || !DukeX::OwnWorld() ||
       projectile<0||projectile>=MAXSPRITES||target<0||target>=MAXSPRITES)return 0;
    if(sprite[target].picnum!=APLAYER)return 0;
    return sprite[projectile].owner==target;
}
extern "C" int DukeX_TouchSprintPercent(int playerNum){
    // DXV29: actual measured engine velocity modifier, not CSS/analog cosmetics.
    // 440% full stick is ~2.15x the V28 default; optional turbo up to 560%.
    if(numplayers!=1 || playerNum!=myconnectindex || !DukeX::Alive(playerNum))return 100;
    const int a=DukeX::Max(DukeX::Max(-DukeX::touchX,DukeX::touchX),
                           DukeX::Max(-DukeX::touchY,DukeX::touchY));
    if(a<120)return 100;
    const int fraction=DukeX::Clamp((a-120)*1000/880,0,1000);
    const int bonus=DukeX::fighters[playerNum].skill[17]*16;
    return DukeX::Clamp(100+(DukeX::soloRunPercent-100)*fraction/1000+bonus,100,640);
}
// DXV29_PORTABLE_NATIVE: speed is based on the ACTUAL synced movement input,
// not merely on a browser joystick that does not exist on desktop or consoles.
extern "C" int DukeX_WorldMoveScalePercent(int playerNum,int fvel,int svel){
    if(numplayers!=1 || playerNum!=myconnectindex || !DukeX::Alive(playerNum))return 100;
    const int forward=DukeX::Max(-fvel,fvel);
    const int sideways=DukeX::Max(-svel,svel);
    const int magnitude=DukeX::Clamp(DukeX::Max(forward,sideways),0,90);
    if(magnitude<7)return 100;
    const int percent=DukeX::Clamp((magnitude-7)*1000/83,0,1000);
    const int bonus=DukeX::fighters[playerNum].skill[17]*16;
    return DukeX::Clamp(100+(DukeX::soloRunPercent-100)*percent/1000+bonus,100,640);
}

// Stable native platform-control seam. SDL/gamepad/native frontends can call
// this exactly as browser JS does. Game rules never depend on the DOM.
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_QueueInventoryAction(int action){
    if(numplayers!=1 || action<1 || action>8)return 0;
    DukeX::inventoryActions|=1u<<action;
    return 1;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_InventoryAmount(int kind){
    if(!DukeX::Alive(myconnectindex))return 0;
    auto const p=g_player[myconnectindex].ps;
    switch(kind){
      case 1:return p->inv_amount[GET_JETPACK];
      case 2:return p->inv_amount[GET_FIRSTAID];
      case 3:return p->inv_amount[GET_SHIELD];
      case 4:return p->inv_amount[GET_STEROIDS];
      default:return 0;
    }
}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_V34Marker(){return "DXV34: ENGINE_HITSCAN_THICK_RIBBONS_3D_ACTOR_CROSSINGS_SURFACE_COLLISIONS";}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V34BeamTraces(){return DukeX::beamTraces;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V34BeamDamageHits(){return DukeX::beamDamageHits;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V34BeamWorldImpacts(){return DukeX::beamWorldImpacts;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V34BeamWaterStops(){return DukeX::beamWaterStops;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V34BeamSkyStops(){return DukeX::beamSkyStops;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V34ActiveRibbonSprites(){
    int n=0;for(auto const& b:DukeX::beamRibbons)if(b.id>=0)++n;
    for(auto const& t:DukeX::tripwires)for(int id:t.beams)if(id>=0)++n;
    return n;
}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_V31Marker(){return "DXV31: CONTINUOUS_RED_BEAM_NATIVE_INVENTORY_AIRJUMP_62_SKILLS";}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V31AirJumps(){return DukeX::airJumpCount;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V31InventoryInputs(){return DukeX::inventoryActionCount;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V31HealthDrops(){return DukeX::collectedHealth;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V31ArmorDrops(){return DukeX::collectedArmor;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V31BeamSegments(){
    int n=0;for(auto const &mine:DukeX::tripwires)
        for(int id:mine.beams)if(id>=0&&id<MAXSPRITES&&sprite[id].statnum==STAT_FX&&sprite[id].picnum==LASERLINE)++n;
    return n;
}
extern "C" EMSCRIPTEN_KEEPALIVE void DukeX_SetNativeControlAxes(int moveX,int moveY,int aimX,int aimY,int buttons){
    DukeX::touchX=DukeX::Clamp(moveX,-1000,1000);
    DukeX::touchY=DukeX::Clamp(moveY,-1000,1000);
    DukeX::touchAimX=DukeX::Clamp(aimX,-1000,1000);
    DukeX::touchAimY=DukeX::Clamp(aimY,-1000,1000);
    DukeX::touchButtons=buttons&15;
}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_V30CombatMarker(){
    return "DXV30: COLLIDING_BOUNCE_GRENADE_CRYO_ARC_SHAREWARE_ATOMIC_ART_FALLBACK";
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V30ActiveOrdnance(){
    int n=0;for(auto const &b:DukeX::thrown)if(b.id>=0)++n;return n;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V30ImpactCount(){return DukeX::impacts;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V30RicochetCount(){return DukeX::ricochets;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V30ExpanderShots(){return DukeX::expandShots;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V30EditionHasExpander(){return DukeX::HasArt(GROWSPARK)?1:0;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V30EditionTileFallbacks(){return DukeX::missingOptionalTiles;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V30EditionHasCrystal(){return DukeX::HasArt(CRYSTALAMMO);}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V30EditionHasFreezeArt(){return DukeX::HasArt(FREEZEBLAST);}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V30GemArt(){return DukeX::GemTile();}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V30OrbitalArt(){return DukeX::OrbitalTile();}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_NativeBuildMarker(){
    return "DXV29_PORTABLE_NATIVE_WORLD_SPEED_EQUIPMENT_UI";
}
extern "C" EMSCRIPTEN_KEEPALIVE void DukeX_SetRunScale(int percent){
    // Run-speed preset from FIELD COMPUTER / OPS. Solo only.
    if(numplayers==1)DukeX::soloRunPercent=DukeX::Clamp(percent,320,560);
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeRunScale(){return DukeX::soloRunPercent;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeNapalmFields(){
    int n=0;for(auto const &f:DukeX::napalmFields)if(f.id>=0)++n;return n;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeTripwires(){
    int n=0;for(auto const &t:DukeX::tripwires)if(t.id>=0)++n;return n;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeVisuals(){
    int n=0;for(auto const &v:DukeX::visuals)if(v.id>=0)++n;return n;
}

extern "C" int DukeX_AdjustFallDamage(int playerNum,int requested,int health){
    if(numplayers!=1 || !DukeX::Alive(playerNum))return requested;
    return DukeX::Clamp(requested/6,0,DukeX::Max(0,health-1));
}
extern "C" int DukeX_ProtectFatalFall(int playerNum){
    if(numplayers!=1 || !DukeX::Alive(playerNum))return 0;
    auto &hp=sprite[g_player[playerNum].ps->i].extra;
    hp=DukeX::Max(1,hp-DukeX::Min(8,DukeX::Max(0,hp-1)));
    return 1;
}
extern "C" int DukeX_ReduceHealthDelta(int target,int delta){
    if(delta>=0 || !DukeX::initialized || !DukeX::OwnWorld())return delta;
    for(bssize_t TRAVERSE_CONNECT(i)){
        if(!DukeX::Alive(i) || g_player[i].ps->i!=target)continue;
        // Base: take 40% of actual health damage. Armor upgrades reduce more.
        int percent=DukeX::Max(10,40-DukeX::fighters[i].skill[14]*4-DukeX::fighters[i].skill[DukeX::HEAVY_ARMOR]*3-
            DukeX::fighters[i].skill[DukeX::TITANIUM_ARMOR]*2);
        return -DukeX::Max(1,static_cast<int>(-static_cast<int64_t>(delta)*percent/100));
    }
    return delta;
}

#if defined(__EMSCRIPTEN__)
// V10: read-only observability of the ACTUAL engine/video runtime.
// Emscripten keepalive exports these without modifying frame timing or rendering.
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeFrameCount(){return (int)g_frameCounter;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeXres(){return xres;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeYres(){return yres;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeBpp(){return bpp;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeRenderMode(){return (int)videoGetRenderMode();}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeGameMode(){return (myconnectindex>=0&&myconnectindex<MAXPLAYERS&&g_player[myconnectindex].ps)?(int)g_player[myconnectindex].ps->gm:-1;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeSurvivorTicks(){return DukeX::gameTicks;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeDeathState(){
    if(numplayers!=1 || !DukeX::initialized || myconnectindex<0 ||
       myconnectindex>=MAXPLAYERS || !g_player[myconnectindex].ps)return 0;
    auto p=g_player[myconnectindex].ps;
    return p->i>=0 && p->i<MAXSPRITES && sprite[p->i].extra<=0 ? 1 : 0;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeRunGeneration(){return DukeX::runGeneration;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeRespawnHealth(){
    if(myconnectindex<0||myconnectindex>=MAXPLAYERS||!g_player[myconnectindex].ps)return 0;
    int id=g_player[myconnectindex].ps->i;
    return id>=0&&id<MAXSPRITES?sprite[id].extra:0;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_RequestRestart(){
    if(numplayers!=1 || !DukeX::initialized)return 0;
    DukeX::restartRequested=1;
    return 1;
}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_BuildMarkerV18(){return "DXV18: RESPAWN_AND_TOUCH_LIFECYCLE";}

extern "C" EMSCRIPTEN_KEEPALIVE void DukeX_SetTouchAxes(int x,int y){
    DukeX::touchX=DukeX::Clamp(x,-1000,1000);
    DukeX::touchY=DukeX::Clamp(y,-1000,1000);
}
extern "C" EMSCRIPTEN_KEEPALIVE void DukeX_SetAimAxes(int x,int y){
    DukeX::touchAimX=DukeX::Clamp(x,-1000,1000);
    DukeX::touchAimY=DukeX::Clamp(y,-1000,1000);
}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_BuildMarkerV23(){return "DXV23: TOUCH_AUTORUN";}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_BuildMarkerV17(){return "DXV17: REAL_DUKE_TOUCH_INPUT";}
extern "C" EMSCRIPTEN_KEEPALIVE void DukeX_SetTouchButtons(int bits){
    DukeX::touchButtons=bits&15;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeTouchSamples(){return DukeX::touchSamples;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeTouchForward(){return DukeX::touchAppliedFvel;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeTouchStrafe(){return DukeX::touchAppliedSvel;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeTouchTurn(){return DukeX::touchAppliedTurn;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeTouchPitch(){return DukeX::touchAppliedPitch;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeTouchButtons(){return DukeX::touchAppliedButtons;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeTouchWorldX(){return DukeX::Alive(myconnectindex)?sprite[g_player[myconnectindex].ps->i].x:0;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeTouchWorldY(){return DukeX::Alive(myconnectindex)?sprite[g_player[myconnectindex].ps->i].y:0;}
// DXV26: these MUST reflect the real game state; V25's report asked for a
// nonexistent ProbeSpawnSector and silently reported sector 0 instead.
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeSpawnSector(){
    return g_player[myconnectindex].ps ? g_player[myconnectindex].ps->cursectnum : -1;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeSpriteSector(){
    if(!g_player[myconnectindex].ps)return -1;
    int id=g_player[myconnectindex].ps->i;
    return id>=0&&id<MAXSPRITES?sprite[id].sectnum:-1;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbePlayerZ(){
    return g_player[myconnectindex].ps?g_player[myconnectindex].ps->pos.z:0;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeCameraDistance(){return g_cameraDistance;}

extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeTouchAngle(){return DukeX::Alive(myconnectindex)?(g_player[myconnectindex].ps->q16ang>>16)&2047:0;}
extern "C" EMSCRIPTEN_KEEPALIVE void DukeX_RequestBlast(){
    // V33: retired free manual RPG. A legacy UI call cannot grant shots.
    DukeX::blastRequests=0;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_BuyCareerRank(){
    if(numplayers!=1 || DukeX::careerRank>=10)return 0;
    int const cost=(DukeX::careerRank+1)*20;
    if(DukeX::careerCredits<cost)return 0;
    DukeX::careerCredits-=cost;++DukeX::careerRank;
    return 1;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ChooseUpgrade(int choice){
    if(numplayers>1||!DukeX::initialized)return 0;
    return DukeX::ChooseUpgrade(myconnectindex,choice)?1:0;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeOfferSeed(){return static_cast<int>(DukeX::offerSeed);}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ProbeOfferGeneration(){return DukeX::offerGeneration;}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_BuildMarkerV26(){return "DXV26: RANDOMIZED_OFFERS_DUAL_STICK_CAMERA_RECOVERY";}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_BuildMarkerV28(){return "DXV29: REAL_SPEED_440_VISUAL_AURAS_BEAM_CROSSING_NAPALM | DXV28: REAL_TRIPWIRES_THUNDER_CHAIN_SPRINT_FALL_ROLL | DXV28_SPEED_AND_NONLETHAL_FALL";}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_BeginOffer(){
    return DukeX::BeginOffer(myconnectindex)?1:0;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_PendingUpgrades(){
    return DukeX::initialized && DukeX::Alive(myconnectindex)?DukeX::fighters[myconnectindex].unspent:0;
}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_OfferName(int n){
    if(!DukeX::initialized||!DukeX::Alive(myconnectindex)||n<1||n>3)return "";
    auto &f=DukeX::fighters[myconnectindex];
    int id=DukeX::Choice(f,n-1);
    return id<0?"MEDKIT REFILL":DukeX::skillNames[id];
}

extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_OfferDescription(int n){
    if(!DukeX::initialized||!DukeX::Alive(myconnectindex)||n<1||n>3)return "";
    auto const &f=DukeX::fighters[myconnectindex];
    int id=DukeX::Choice(f,n-1);
    return id<0?"RESTORE HEALTH":DukeXOfferDesc[id];
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_OfferRank(int n){
    if(!DukeX::initialized||!DukeX::Alive(myconnectindex)||n<1||n>3)return 0;
    auto const &f=DukeX::fighters[myconnectindex];
    int id=DukeX::Choice(f,n-1);return id<0?0:f.skill[id];
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_OfferIcon(int n){
    if(!DukeX::initialized||!DukeX::Alive(myconnectindex)||n<1||n>3)return CRYSTALAMMO;
    int id=DukeX::Choice(DukeX::fighters[myconnectindex],n-1);
    if(id>=70 && id<170)return DukeX::V39Art(id);
    switch(id){
        case -1: return CRYSTALAMMO;
        case 0: return SHOTSPARK1; case 1: return SHOTSPARK1;
        case 2: return RPG; case 3: return FREEZEBLAST;
        case 4: return SHRINKSPARK; case 5: return FIRELASER;
        case 6: return RPG;
        case DukeX::SATELLITES: return CRYSTALAMMO;
        case DukeX::SEEKERS: case DukeX::ROCKET_CYCLONE:case DukeX::MINES:return RPG;
        case DukeX::CRYO_SHATTER: return FREEZEBLAST;
        case DukeX::SHRINK_SWARM: return SHRINKSPARK;
        case DukeX::RAILGUN:case DukeX::PLASMA:case DukeX::STORM_GRID:case DukeX::PIERCING_RAILS:case DukeX::THUNDER_CHAIN:return FIRELASER;
        case DukeX::PULSE_DRONES:return TRANSPORTERSTAR;
        default: return CRYSTALAMMO;
    }
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_OfferEvolution(int n){
    if(!DukeX::initialized||!DukeX::Alive(myconnectindex)||n<1||n>3)return 0;
    auto const &f=DukeX::fighters[myconnectindex];
    int id=DukeX::Choice(f,n-1);
    for(auto const& e:DukeX::evolutions)
        if(e.weapon==id && f.skill[id]>=4 && f.skill[e.passive]>=(id<7?3:2))return 1;
    for(auto const&e:DukeX::v39Fusions)
        if(e.weapon==id && f.skill[id]>=3 && f.skill[e.passive]>=2)return 1;
    return 0;
}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_FusionName(int id){
    if(!DukeX::initialized||!DukeX::Alive(myconnectindex)||id<0||id>=DukeX::kSkillCount)return "";
    return DukeX::EvolutionName(DukeX::fighters[myconnectindex],id);
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_RecipeCount(){
    return (int)(sizeof(DukeX::evolutions)/sizeof(DukeX::evolutions[0])+12);
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_RecipeWeapon(int r){
    if(r<0 || r>=DukeX_RecipeCount())return -1;
    int old=(int)(sizeof(DukeX::evolutions)/sizeof(DukeX::evolutions[0]));
    return r<old?DukeX::evolutions[r].weapon:DukeX::v39Fusions[r-old].weapon;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_RecipePassive(int r){
    if(r<0 || r>=DukeX_RecipeCount())return -1;
    int old=(int)(sizeof(DukeX::evolutions)/sizeof(DukeX::evolutions[0]));
    return r<old?DukeX::evolutions[r].passive:DukeX::v39Fusions[r-old].passive;
}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_RecipeName(int r){
    if(r<0 || r>=DukeX_RecipeCount())return "";
    int old=(int)(sizeof(DukeX::evolutions)/sizeof(DukeX::evolutions[0]));
    return r<old?DukeX::evolutions[r].name:DukeX::v39Fusions[r-old].name;
}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_V39Marker(){return "DXV39: 100_ORIGINAL_ART_SPRITES_NO_AUTOPILOT_UPGRADES_NATIVE_FX";}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ItemCount(){return DukeX::kSkillCount;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ItemRank(int i){
    if(i<0||i>=DukeX::kSkillCount||!DukeX::initialized||!DukeX::Alive(myconnectindex))return 0;
    return DukeX::fighters[myconnectindex].skill[i];
}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_ItemName(int i){
    return i>=0&&i<DukeX::kSkillCount?DukeX::skillNames[i]:"";
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ItemIsWeapon(int i){
    return i>=0&&i<DukeX::kSkillCount&&DukeX::IsWeaponSkill(i);
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_LoadoutCount(){return DukeX::kLoadoutCount;}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_LoadoutName(int i){
    return i>=0&&i<DukeX::kLoadoutCount?DukeX::loadoutNames[i]:"";
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_SelectLoadout(int i){
    if(i<0||i>=DukeX::kLoadoutCount||numplayers>1)return 0;
    // Changes take effect on the next survival run, never mid-run.
    DukeX::requestedLoadout=i;return 1;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_ActiveLoadout(){return DukeX::appliedLoadout;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_OfferIsWeapon(int n){
    if(!DukeX::initialized||!DukeX::Alive(myconnectindex)||n<1||n>3)return 0;
    int id=DukeX::Choice(DukeX::fighters[myconnectindex],n-1);
    return id>=0 && DukeX::IsWeaponSkill(id);
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_OfferRerolls(){
    return DukeX::initialized&&DukeX::Alive(myconnectindex)?DukeX::fighters[myconnectindex].rerolls:0;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_OfferBanishes(){
    return DukeX::initialized&&DukeX::Alive(myconnectindex)?DukeX::fighters[myconnectindex].banishes:0;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_RerollOffers(){
    return numplayers==1 && DukeX::initialized && DukeX::Reroll(myconnectindex);
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_BanishOffer(int id){
    return numplayers==1 && DukeX::initialized && DukeX::Banish(myconnectindex,id);
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_EquippedWeaponCount(){
    return DukeX::initialized && DukeX::Alive(myconnectindex)?DukeX::EquippedWeapons(DukeX::fighters[myconnectindex]):0;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_CareerCredits(){return DukeX::careerCredits;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_CareerRank(){return DukeX::careerRank;}
extern "C" EMSCRIPTEN_KEEPALIVE void DukeX_LoadCareer(int credits,int rank){
    if(DukeX::initialized)return; // prevent mid-run cheating/mismatch
    DukeX::careerCredits=DukeX::Clamp(credits,0,1000000);
    DukeX::careerRank=DukeX::Clamp(rank,0,10);
}
#endif

// V46 engine-owned progression introspection (no client-side authority).
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_V46Marker(){
    return "DXV46: HIT_FIX_CHESTS_FUSIONS_TWISTS_PREMAP_SLOTS_MOBILE";
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46WeaponSlots(){return DukeX::careerWeaponSlots;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46PassiveSlots(){return DukeX::careerPassiveSlots;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46ItemRarity(int id){
    return id>=0&&id<DukeX::kSkillCount?DukeX::fighters[myconnectindex].rarity[id]:0;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46ItemStatPercent(int id){
    return id>=0&&id<DukeX::kSkillCount?DukeX::ItemPercent(DukeX::fighters[myconnectindex],id):100;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46OfferRarity(int n){
    return n>=1&&n<=3?DukeX::activeOfferRarity[n-1]:0;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46ItemAbsorbed(int id){
    return id>=0&&id<DukeX::kSkillCount?DukeX::fighters[myconnectindex].absorbed[id]:0;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46FusionPartner(int id){
    return id>=0 && id<DukeX::kSkillCount?(int)DukeX::fighters[myconnectindex].fusionPartner[id]-1:-1;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46ItemFused(int id){
    return id>=0&&id<DukeX::kSkillCount?DukeX::fighters[myconnectindex].fused[id]:0;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46ChestCount(){return DukeX::chestsOpened;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46FusionCount(){return DukeX::chestFusions;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46BuySlot(int kind){
    if(numplayers!=1||kind<0||kind>1)return 0;
    int &slots=kind?DukeX::careerPassiveSlots:DukeX::careerWeaponSlots;
    if(slots>=DukeXV46::kMaxSlots)return 0;
    int cost=(slots-3)*45;
    if(DukeX::careerCredits<cost)return 0;
    DukeX::careerCredits-=cost;++slots;return 1;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46DamageChest(int victim,int source){
    return DukeX::DamageChest(victim,source)?1:0;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46ChooseTwist(int n){
    return DukeX::ChooseTwist(n)?1:0;
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46TwistActive(){return DukeX::twistChoice.seat>=0;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46TwistType(int n){
    if(DukeX::twistChoice.seat<0 || n<1 || n>3)return 0;
    auto const t=DukeX::twistChoice.cards[n-1];
    return (t.bane?-1:1)*(t.kind+1);
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46TwistStrength(int n){
    return DukeX::twistChoice.seat<0||n<1||n>3?0:DukeX::twistChoice.cards[n-1].strength;
}

extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46ChooseSlot(int kind){return DukeX_V46BuySlot(kind);}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V46LoadSlots(int weaponSlots,int passiveSlots){
    if(numplayers!=1 || DukeX::initialized)return 0;
    DukeX::careerWeaponSlots=DukeX::Clamp(weaponSlots,4,8);
    DukeX::careerPassiveSlots=DukeX::Clamp(passiveSlots,4,8);
    return 1;
}
// V40 native read-only GPU renderer endpoint; never participates in game state.
#include "dukex_v40_renderer_bridge.inc"
// DXV41: native primary rendering endpoint, ART/palette/HUD/geometry.
#include "dukex_v41_primary_renderer.inc"
#include "dukex_v42_renderer_beams.inc"

extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V47PhysicalMissiles(){return DukeX::v47MissilesLaunched;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V47MissileImpacts(){return DukeX::v47MissilesImpacted;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V47ShotgunContacts(){return DukeX::v47ShotgunContacts;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V47RailGuaranteed(){return DukeX::v47RailGuaranteed;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V47TransformerReactions(){return DukeX::v47TransformerReactions;}

// DXV48: compact native HUD, genuinely paused solo modals, fast XP and
// level-wide collectible vacuum. Required marker for WASM/GitHub publish.
// DXV48_3: stable, link-verifiable export for the new native HUD/jackpot.
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_V48_3Marker(){
    return "DXV48_3: TRASH_CAN_REAL_ART_SMALL_LEFT_HUD";
}
extern "C" EMSCRIPTEN_KEEPALIVE const char* DukeX_V48Marker(){
    return "DXV48: XP_GLOBAL_VACUUM_COMPACT_NATIVE_HUD_MODAL_FREEZE";
}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V48VacuumTicks(){return DukeX::xpVacuum.ticks;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V48VacuumCollected(){return DukeX::xpVacuum.collected;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V48VacuumDrops(){return DukeX::xpVacuumDrops;}
extern "C" EMSCRIPTEN_KEEPALIVE int DukeX_V48ModalActive(){return DukeX::ModalPauseActive()?1:0;}
