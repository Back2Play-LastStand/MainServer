#include "pch.h"
#include "Room.h"

shared_ptr<Room> GRoom = make_shared<Room>();

Room::Room()
{
}

Room::~Room()
{
}

unordered_map<unsigned long long, shared_ptr<Player>> Room::GetPlayers()
{
	return m_players;
}

bool Room::EnterPlayer(shared_ptr<Player> player)
{
	if (m_players.find(player->GetId()) != m_players.end())
		return false;

	m_players.insert({ player->GetId(), player });

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
	for (const auto& [id, player] : m_players)
	{
		if (id == exceptId)
			continue;

		if (auto session = player->GetSession())
			session->SendContext(buffer);
	}
}

void Room::HandleMove(Session* session, Protocol::REQ_MOVE pkt)
{
	if (!session)
		return;

	auto gameSession = static_cast<GameSession*>(session);
	auto player = gameSession->GetPlayer();
	if (!player)
		return;

	player->GetObjectInfo().mutable_posinfo()->set_posx(pkt.info().posx());
	player->GetObjectInfo().mutable_posinfo()->set_posy(pkt.info().posy());

	{
		Protocol::RES_MOVE move;
		auto info = move.mutable_player();
		info->set_objectid(player->GetId());
		info->mutable_posinfo()->set_posx(pkt.info().posx());
		info->mutable_posinfo()->set_posy(pkt.info().posy());

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(move);

		for (auto p : m_players)
		{
			session->SendContext(std::move(*sendBuffer));
		}
	}
}