#include <doctest.h>
#include "Render/Effects/MultiShotFan.h"

namespace MultiShot = Render::Effects::MultiShot;

TEST_CASE("triple shot shoots three arrows, four from a four-arrow bow")
{
    CHECK(MultiShot::ArrowCount(false, false) == 3);
    CHECK(MultiShot::ArrowCount(true, false) == 4);
}

TEST_CASE("triple shot mastery adds one arrow")
{
    CHECK(MultiShot::ArrowCount(false, true) == 4);
    CHECK(MultiShot::ArrowCount(true, true) == 5);
}

TEST_CASE("three arrows fan out 15 degrees apart around the facing")
{
    const float spacing = MultiShot::ArrowSpacing(3);
    CHECK(MultiShot::ArrowTurn(0, 3, spacing) == doctest::Approx(-15.f));
    CHECK(MultiShot::ArrowTurn(1, 3, spacing) == doctest::Approx(0.f));
    CHECK(MultiShot::ArrowTurn(2, 3, spacing) == doctest::Approx(15.f));
}

TEST_CASE("four arrows keep the four-arrow bow's fan at 5 and 15 degrees")
{
    const float spacing = MultiShot::ArrowSpacing(4);
    CHECK(MultiShot::ArrowTurn(0, 4, spacing) == doctest::Approx(-15.f));
    CHECK(MultiShot::ArrowTurn(1, 4, spacing) == doctest::Approx(-5.f));
    CHECK(MultiShot::ArrowTurn(2, 4, spacing) == doctest::Approx(5.f));
    CHECK(MultiShot::ArrowTurn(3, 4, spacing) == doctest::Approx(15.f));
}

TEST_CASE("five arrows fan out 10 degrees apart around the facing")
{
    const float spacing = MultiShot::ArrowSpacing(5);
    CHECK(MultiShot::ArrowTurn(0, 5, spacing) == doctest::Approx(-20.f));
    CHECK(MultiShot::ArrowTurn(2, 5, spacing) == doctest::Approx(0.f));
    CHECK(MultiShot::ArrowTurn(4, 5, spacing) == doctest::Approx(20.f));
}
