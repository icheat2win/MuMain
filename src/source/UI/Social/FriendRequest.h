#pragma once

#include <string_view>

namespace UI::Social
{
// Asks the server to add `friendName` to the friend list, from the friend window's add field or the command window.
// An empty name sends nothing; the hero's own name shows the notice the server gives for it and sends nothing.
void RequestAddFriend(std::wstring_view friendName);
} // namespace UI::Social
