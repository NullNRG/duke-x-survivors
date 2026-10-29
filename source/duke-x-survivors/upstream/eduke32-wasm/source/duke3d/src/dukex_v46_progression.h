// Duke X V46 progression rules shared by the native EDuke32 gameplay and tests.
// No browser dependencies and no retail game assets. GPL-2.0-or-later.
#pragma once
#include <cstdint>
namespace DukeXV46 {
constexpr int kItems = 170;
constexpr int kBaseSlots = 4;
constexpr int kMaxSlots = 8;
constexpr int kRarityCount = 6;
static const char *const rarityNames[kRarityCount] = {
    "COMMON", "UNCOMMON", "RARE", "EPIC", "LEGENDARY", "MYTHIC"
};
// Pre-requisite dependent upgrades must not appear without their actual host
// ability. The rule applies in every acquisition path (level up, supply, chest).
inline bool Any(const int *rank, const int *ids, int count) {
    for (int i = 0; i < count; ++i) if (rank[ids[i]] > 0) return true;
    return false;
}
inline bool HasPrerequisite(int id, const int *s) {
    switch (id) {
        case 36: { const int r[]={20,21,25,27,42,57,63,66,67,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109}; return Any(s,r,sizeof(r)/sizeof(*r)); } // field/orbit lifetime
        case 43: { const int r[]={28,41,42,49,57,67,82,83,84,85,86,87,88,89,110,111,112,113,114};return Any(s,r,sizeof(r)/sizeof(*r)); } // stronger electric attack
        case 53: return s[52]>0; // triple jump requires double jump
        case 54: return s[23]>0 || s[112]>0 || s[113]>0; // laser mine multiplier
        case 59: return s[42]>0 || s[20]>0 || s[93]>0 || s[96]>0; // drone detonation
        case 125: {const int r[]={15,39,55,56,61,128,130,135,166};return Any(s,r,sizeof(r)/sizeof(*r)); } // nanite restoration satellite proc
        case 126: {const int r[]={3,29,48,65,76,97,102,136,158};return Any(s,r,sizeof(r)/sizeof(*r)); } // bonus only versus frozen foes
        case 128: return true; // reactive carapace provides its own armor when struck
        case 134: { const int r[]={20,63,90,91,92,93,94,95,96,97,98,99,161};return Any(s,r,sizeof(r)/sizeof(*r)); } // actual orbital gearbox id
        case 138: return true; // standalone close-range kill reward
        case 143: { const int r[]={20,42,63,90,91,92,93,94,95,96,97,98,99,161};return Any(s,r,sizeof(r)/sizeof(*r)); } // physical orbiting bodies
        case 161: { const int r[]={20,42,63,90,91,92,93,94,95,96,97,98,99};return Any(s,r,sizeof(r)/sizeof(*r)); } // extra orbital bodies
        case 162: return s[66]>0 || s[108]>0 || s[151]>0; // gravity range
        case 163: { const int r[]={21,25,27,57,66,100,101,102,103,104,105,106,107,108,109};return Any(s,r,sizeof(r)/sizeof(*r)); } // aura radius
        case 157: { const int r[]={5,24,33,58,81,82,85,86,87,88,89};return Any(s,r,sizeof(r)/sizeof(*r)); } // beam uptime
        case 158: return s[3]>0 || s[29]>0 || s[48]>0 || s[65]>0; // frost shatter
        case 159: { const int r[]={5,41,43,49,57,67,103,110};return Any(s,r,sizeof(r)/sizeof(*r)); } // requires a shock source
        default: return true;
    }
}
inline int CountSlots(const int *rank, const uint8_t *absorbed,
                     bool (*isWeapon)(int), bool wantWeapons) {
    int n=0;
    for (int id=0; id<kItems; ++id)
        if(rank[id]>0 && !absorbed[id] && isWeapon(id)==wantWeapons) ++n;
    return n;
}
inline bool CanAcquire(int id, const int *rank, const uint8_t *banished,
                       const uint8_t *absorbed, int maxRank,
                       int weaponSlots, int passiveSlots, bool (*isWeapon)(int)) {
    if (id<0 || id>=kItems || banished[id] || absorbed[id] || rank[id]>=maxRank)
        return false;
    if (!HasPrerequisite(id,rank)) return false;
    if (rank[id]>0) return true; // Rank upgrades never consume a new slot.
    bool weapon=isWeapon(id);
    return CountSlots(rank,absorbed,isWeapon,weapon) < (weapon?weaponSlots:passiveSlots);
}
// 0 common / 1 uncommon / 2 rare / 3 epic / 4 legendary / 5 mythic.
// Luck reduces the common bracket; bounded input makes replays deterministic.
inline int RollRarity(uint32_t draw, int luckRank) {
    int r=(int)(draw%10000u), luck=luckRank>0?(luckRank<12?luckRank:12):0;
    if (r<2+luck/4) return 5;
    if (r<60+luck*7) return 4;
    if (r<450+luck*35) return 3;
    if (r<1750+luck*75) return 2;
    if (r<4550+luck*110) return 1;
    return 0;
}
inline int StatPercent(int rarity, uint32_t roll) {
    int r=rarity<0?0:(rarity>5?5:rarity);
    return 100+r*7+(int)(roll%9u); // 100–143% for item-specific base stats
}
inline int ChestRewardCount(uint32_t draw, int elite) {
    const uint32_t n=draw%10000u;
    if (n<2 && elite) return 5;
    if (n<30 && elite) return 4;
    if (n<190) return 3;
    if (n<1300) return 2;
    return 1;
}
// Rare modifiers are outside the 170-slot armory. Bit 3 means a bane.
struct RunTwist {
    int kind=-1; int strength=0; bool bane=false;
};
inline RunTwist RollTwist(uint32_t type, uint32_t strength) {
    RunTwist out;
    out.kind=(int)(type%8u);
    out.bane=((type/8u)&1u)!=0;
    out.strength=1+(int)(strength%3u);
    return out;
}
} // namespace DukeXV46
