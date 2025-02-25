#include "pch.h"
#include "Room.h"

Room::Room()
{
}

Room::~Room()
{
}

void Room::BroadCast(vector<char> buffer, unsigned long long exceptId)
{
	for (auto& item : m_players)
	{
		shared_ptr<Player> player = item.second;
		if (player->GetId() == exceptId)
			continue;

		if (auto session = player->GetSession())
			session->SendContext(buffer);
	}
}
