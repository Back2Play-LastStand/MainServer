#include "pch.h"
#include "Room.h"

Room::Room()
{
}

Room::~Room()
{
}

bool Room::EnterPlayer(shared_ptr<Player> player)
{
	if (m_players.find(player->GetId()) != m_players.end())
		return false;

	m_players.insert(make_pair(player->GetId(), player));

	return true;
}

bool Room::LeavePlayer(unsigned long long objectId)
{
	if (m_players.find(objectId) == m_players.end())
		return false;

	m_players.erase(objectId);

	return true;
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
