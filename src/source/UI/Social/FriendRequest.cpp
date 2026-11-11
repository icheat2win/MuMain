#include "stdafx.h"

#include "UI/Social/FriendRequest.h"

#include "UI/Social/FriendName.h"
#include "UI/Social/SocialUpdates.h"
#include "Engine/Object/ZzzCharacter.h"
#include "Network/Server/WSclient.h"
#include "I18N/All.h"

#include <string>

namespace UI::Social
{
void RequestAddFriend(std::wstring_view friendName)
{
    if (friendName.empty())
        return;

    if (NamesOwnCharacter(friendName, Hero->ID))
    {
        ShowNotice(I18N::Game::YouCannotRegisterYourOwnID);
        return;
    }

    const std::wstring name(friendName);
    SocketClient->ToGameServer()->SendFriendAddRequest(MU_C16(name.c_str()));
}
} // namespace UI::Social
