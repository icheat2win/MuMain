#pragma once

namespace Engine::Object
{
// Whether a Cursed Temple team skin hides a part it would link: the 1st, 2nd and 3rd wings, the small wings and capes,
// and the fighters' capes. Weapons, shields and every other part are still drawn.
[[nodiscard]] bool TeamSkinHidesPart(int modelType);
} // namespace Engine::Object
