#pragma once

namespace Render::Effects::MultiShot
{
// The arrows of one multi-arrow shot (Triple Shot and its strengthened forms): three, or four from the bows and
// crossbows that shoot four, and one more with Triple Shot Mastery.
[[nodiscard]] int ArrowCount(bool fourArrowWeapon, bool mastery);

// The degrees between two neighbouring arrows of a shot of `arrowCount`: three arrows fan out 15 degrees apart, four
// or more 10 degrees apart, as in the original client.
[[nodiscard]] float ArrowSpacing(int arrowCount);

// How far the arrow at `index` (0 to arrowCount - 1) turns from the archer's facing, so that the fan is centred on it.
[[nodiscard]] float ArrowTurn(int index, int arrowCount, float spacing);
} // namespace Render::Effects::MultiShot
