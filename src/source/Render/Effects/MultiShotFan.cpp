#include "Render/Effects/MultiShotFan.h"

namespace Render::Effects::MultiShot
{
namespace
{
constexpr int BaseArrows = 3;
constexpr int FourArrowWeaponArrows = 4;
constexpr int MasteryExtraArrows = 1;
constexpr float WideSpacing = 15.f;
constexpr float NarrowSpacing = 10.f;
} // namespace

int ArrowCount(bool fourArrowWeapon, bool mastery)
{
    const int weaponArrows = fourArrowWeapon ? FourArrowWeaponArrows : BaseArrows;
    return mastery ? weaponArrows + MasteryExtraArrows : weaponArrows;
}

float ArrowSpacing(int arrowCount)
{
    return arrowCount > BaseArrows ? NarrowSpacing : WideSpacing;
}

float ArrowTurn(int index, int arrowCount, float spacing)
{
    const float centre = static_cast<float>(arrowCount - 1) / 2.f;
    return (static_cast<float>(index) - centre) * spacing;
}
} // namespace Render::Effects::MultiShot
