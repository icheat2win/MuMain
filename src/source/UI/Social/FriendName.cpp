#include "UI/Social/FriendName.h"

namespace UI::Social
{
bool NamesOwnCharacter(std::wstring_view friendName, std::wstring_view heroName)
{
    return !heroName.empty() && friendName == heroName;
}
} // namespace UI::Social
