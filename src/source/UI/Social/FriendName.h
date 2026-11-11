#pragma once

#include <string_view>

namespace UI::Social
{
// Whether a friend request names the hero's own character, which the client refuses before it asks the server.
[[nodiscard]] bool NamesOwnCharacter(std::wstring_view friendName, std::wstring_view heroName);
} // namespace UI::Social
