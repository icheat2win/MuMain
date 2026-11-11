#include "Engine/Object/TeamSkinParts.h"

#include "Core/Platform/WinCompat.h" // the Win32 scalar types _enum.h uses
#include "Core/Globals/_enum.h"

namespace Engine::Object
{
namespace
{
bool InRange(int modelType, int first, int last)
{
    return modelType >= first && modelType <= last;
}
} // namespace

bool TeamSkinHidesPart(int modelType)
{
    return InRange(modelType, MODEL_WING, MODEL_WINGS_OF_DARKNESS)                      // 1st and 2nd wings
           || InRange(modelType, MODEL_WING_OF_STORM, MODEL_WING_OF_DIMENSION)          // 3rd wings
           || InRange(modelType, MODEL_SMALL_CAPE_OF_LORD, MODEL_LITTLE_WARRIORS_CLOAK) // small wings and capes
           || InRange(modelType, MODEL_CAPE_OF_FIGHTER, MODEL_CAPE_OF_OVERRULE);        // capes
}
} // namespace Engine::Object
