#pragma once

// What the Elf Soldier's buff (eBuff_HelpNpc) adds to the hero, as the server adds it: 45 + level / 3 damage and
// 50 + level / 5 defense. The bonus grows with the level up to the end of the buff, which she gives up to level 220
// (OpenMU's Elf Soldier buff, see MUnique/OpenMU#1492). The original client capped the level at 180 here, so the
// character window showed less than the hero got above level 180.
namespace GameLogic::Buffs
{
constexpr int ElfSoldierDamageBonus(int level)
{
    return 45 + level / 3;
}

constexpr int ElfSoldierDefenseBonus(int level)
{
    return 50 + level / 5;
}
} // namespace GameLogic::Buffs
