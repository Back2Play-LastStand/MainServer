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

void Room::HandleMove(Session* session, Protocol::REQ_MOVE pkt)
{
	if (!session)
		return;

	auto gameSession = static_cast<GameSession*>(session);
	auto player = gameSession->GetPlayer();

	Protocol::ObjectInfo info;
	Protocol::PositionInfo* posInfo = new Protocol::PositionInfo;
	posInfo->set_posx(pkt.info().posx());
	posInfo->set_posy(pkt.info().posy());
	info.set_allocated_posinfo(posInfo);
	player->SetPosition(info);

	{
		Protocol::RES_MOVE move;
		Protocol::ObjectInfo* info = new Protocol::ObjectInfo();
		Protocol::PositionInfo* posInfo = new Protocol::PositionInfo();
		info->set_objectid(player->GetId());
		posInfo->set_posx(pkt.info().posx());
		posInfo->set_posy(pkt.info().posy());
		info->set_allocated_posinfo(posInfo);
		move.set_allocated_player(info);

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(move);
		BroadCast(std::move(*sendBuffer), info->objectid());
	}
}
