#ifndef BOOLS_H
#define BOOLS_H

// Bit flags for each zone.
enum ZoneFlag
{
    ZONE_WATER = 0x01,  // 0001 (decimal 1)
    ZONE_LAVA = 0x02,   // 0010 (decimal 2)
    ZONE_FOREST = 0x04, // 0100 (decimal 4)
    ZONE_CITY = 0x08    // 1000 (decimal 8)
};

// Check if a given zone is already set in boolVal
inline bool hasZone(int bitmask, ZoneFlag zone)
{
    return (bitmask & zone) != 0;
}

// Add a zone bit to boolVal
inline int addZone(int bitmask, ZoneFlag zone)
{
    return bitmask | zone;
}

#endif // BOOLS_H