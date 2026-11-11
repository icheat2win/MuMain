#include <doctest.h>
#include "UI/Social/FriendName.h"

using UI::Social::NamesOwnCharacter;

TEST_CASE("a friend request with the hero's own name names the own character")
{
    CHECK(NamesOwnCharacter(L"Hero", L"Hero"));
}

TEST_CASE("a friend request with another name does not name the own character")
{
    CHECK_FALSE(NamesOwnCharacter(L"Friend", L"Hero"));
    CHECK_FALSE(NamesOwnCharacter(L"Her", L"Hero"));
    CHECK_FALSE(NamesOwnCharacter(L"Hero2", L"Hero"));
}

TEST_CASE("without a hero name no request names the own character")
{
    CHECK_FALSE(NamesOwnCharacter(L"", L""));
    CHECK_FALSE(NamesOwnCharacter(L"Hero", L""));
}
