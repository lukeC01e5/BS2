//// filepath: c:\Users\OEM\Documents\GitHub\BS2\data\animals.h
#ifndef ANIMALS_H
#define ANIMALS_H

// #define ZONE 'water'

struct AnimalInfo
{
    const char *name;
    int id;
    const char *environment;
    const char *code; // Unique two-letter code
};

static const AnimalInfo ANIMALS[] = {
    {"FLAMINGO", 3, "lava", "FI"},
    {"FLAME_KINGO", 6, "lava", "FK"},
    {"KITTEN", 2, "city", "KT"},
    {"FLAME_ON", 7, "lava", "FO"},
    {"PUP", 2, "city", "PP"},
    {"DOG", 6, "forest", "DG"},
    {"WOLF", 9, "forest", "WF"},
    {"BIRDY", 3, "forest", "BY"},
    {"HAAST_EAGLE", 10, "forest", "HE"},
    {"SQUIDY", 2, "water", "SQ"},
    {"GIANT_SQUID", 8, "water", "GQ"},
    {"KRAKEN", 12, "water", "KR"},
    {"BABYSHARK", 2, "water", "BS"},
    {"SHARK", 6, "water", "SK"},
    {"MEGALODON", 12, "water", "MG"},
    {"TADPOLE", 2, "forest", "TP"},
    {"POISON_DART_FROG", 7, "forest", "PF"},
    {"UNICORN", 3, "forest", "UC"},
    {"MASTER_UNICORN", 5, "forest", "MU"},
    {"SPROUTY", 3, "forest", "SR"},
    {"TREE_FOLK", 5, "forest", "TF"},
    {"BUSH_MONSTER", 11, "forest", "BM"},
    {"BABY_DRAGON", 3, "lava", "BD"},
    {"DRAGON", 13, "lava", "DR"},
    {"DINO_EGG", 3, "city", "DE"},
    {"T_REX", 10, "city", "TR"},
    {"BABY_RAY", 2, "water", "BR"},
    {"MEGA_MANTA", 9, "water", "MM"},
    {"ORCA", 3, "water", "OC"},
    {"BIG_BITEY", 11, "water", "BB"},
    {"FLAME_LILY", 3, "lava", "FL"},
    {"MONSTER_LILY", 10, "lava", "ML"},
    {"BEAR_CUB", 3, "forest", "BC"},
    {"MOSS_BEAR", 13, "forest", "MB"}};

#endif // ANIMALS_H