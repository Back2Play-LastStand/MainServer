#include "pch.h"
#include "Room.h"

shared_ptr<Room> GRoom = make_shared<Room>();

Room::Room(string name) : m_name(name)
{
}

Room::~Room()
{
}

unordered_map<unsigned long long, shared_ptr<Player>> Room::GetPlayers()
{
	return m_players;
}

unordered_map<unsigned long long, shared_ptr<Monster>> Room::GetMonsters()
{
	return m_monsters;
}

string Room::GetName()
{
	return m_name;
}

bool Room::EnterObject(shared_ptr<GameObject> object)
{
	Protocol::ObjectType type = object->GetType();
	if (type == Protocol::PLAYER)
	{
		if (m_players.find(object->GetId()) != m_players.end())
			return false;

		m_players.insert({ object->GetId(), static_pointer_cast<Player>(object) });
	}
	else if (type >= Protocol::MONSTER)
	{
		if (m_monsters.find(object->GetId()) != m_monsters.end())
			return false;

		m_monsters.insert({ object->GetId(), static_pointer_cast<Monster>(object) });
	}

	return true;
}

bool Room::LeaveObject(shared_ptr<GameObject> object)
{
	unsigned long long objectId = object->GetId();
	Protocol::ObjectType type = object->GetType();
	if (type == Protocol::PLAYER)
	{
		if (m_players.find(objectId) == m_players.end())
			return false;

		m_players.erase(objectId);
	}
	else if (type >= Protocol::MONSTER)
	{
		if (m_monsters.find(objectId) == m_monsters.end())
			return false;

		m_monsters.erase(objectId);
	}

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

	player->GetObjectInfo().mutable_posinfo()->CopyFrom(pkt.info());
	{
		Protocol::RES_MOVE move;
		auto info = move.mutable_player();
		info->set_objectid(player->GetId());
		info->mutable_posinfo()->CopyFrom(pkt.info());

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(move);
		for (auto& [id, p] : m_players)
		{
			if (auto s = p->GetSession())
				s->SendContext(*sendBuffer);
		}
	}
}