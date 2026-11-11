#include <doctest.h>
#include "Engine/Object/TeamSkinParts.h"

#include "Core/Platform/WinCompat.h"
#include "Core/Globals/_enum.h"

using Engine::Object::TeamSkinHidesPart;

TEST_CASE("a Cursed Temple team skin hides wings and capes")
{
    CHECK(TeamSkinHidesPart(MODEL_WING));
    CHECK(TeamSkinHidesPart(MODEL_WINGS_OF_DARKNESS));
    CHECK(TeamSkinHidesPart(MODEL_WING_OF_STORM));
    CHECK(TeamSkinHidesPart(MODEL_SMALL_CAPE_OF_LORD));
    CHECK(TeamSkinHidesPart(MODEL_SMALL_CAPE_OF_LORD + 1)); // the small wings after the small cape
    CHECK(TeamSkinHidesPart(MODEL_LITTLE_WARRIORS_CLOAK));
    CHECK(TeamSkinHidesPart(MODEL_CAPE_OF_FIGHTER));
    CHECK(TeamSkinHidesPart(MODEL_CAPE_OF_OVERRULE));
}

TEST_CASE("a Cursed Temple team skin still draws weapons and shields")
{
    CHECK_FALSE(TeamSkinHidesPart(MODEL_SWORD));
    CHECK_FALSE(TeamSkinHidesPart(MODEL_SHIELD));
    CHECK_FALSE(TeamSkinHidesPart(MODEL_BOW));
    CHECK_FALSE(TeamSkinHidesPart(MODEL_ITEM));
}
