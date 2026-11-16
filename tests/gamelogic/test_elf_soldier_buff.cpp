// Tests of the Elf Soldier's buff as the character window adds it to the shown damage and defense.

#include "doctest.h"

#include "GameLogic/Buffs/ElfSoldierBuff.h"

using GameLogic::Buffs::ElfSoldierDamageBonus;
using GameLogic::Buffs::ElfSoldierDefenseBonus;

TEST_CASE("the Elf Soldier's bonus grows with the level up to 180 [gamelogic][buffs]")
{
    CHECK(ElfSoldierDamageBonus(1) == 45);
    CHECK(ElfSoldierDefenseBonus(1) == 50);
    CHECK(ElfSoldierDamageBonus(180) == 105);
    CHECK(ElfSoldierDefenseBonus(180) == 86);
}

TEST_CASE("the Elf Soldier's bonus keeps growing above level 180, as the server's does [gamelogic][buffs]")
{
    CHECK(ElfSoldierDamageBonus(200) == 111);
    CHECK(ElfSoldierDefenseBonus(200) == 90);
    CHECK(ElfSoldierDamageBonus(220) == 118);
    CHECK(ElfSoldierDefenseBonus(220) == 94);
}
