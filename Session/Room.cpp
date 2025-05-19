#include "pch.h"
#include "Room.h"
#include "ObjectManager.h"

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
	Protocol::RES_LEAVE leave;
	Protocol::ObjectInfo* info = new Protocol::ObjectInfo;
	info->set_objectid(objectId);
	leave.set_allocated_object(info);

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
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(leave);
	BroadCast(move(*sendBuffer));

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

void Room::HandleAttack(Session* session, Protocol::REQ_ATTACK_OBJECT pkt)
{
	auto attacker = GManager->Object()->FindById(pkt.attacker());
	auto object = GManager->Object()->FindById(pkt.objectid());
	if (object)
	{
		Protocol::RES_ATTACK_OBJECT attack;
		object->TakeDamage(pkt.damage());
		attack.set_attacker(attacker->GetId());
		attack.set_objectid(object->GetId());    
		attack.set_damage(object->GetPower());
		attack.set_remainhp(object->GetHp());

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(attack);
		BroadCast(move(*sendBuffer));
	}
}

void Room::SpawnMonster()
{
	TimerPushJob(120000, &Room::SpawnMonster); // 2min
	Protocol::RES_SPAWN_MONSTER spawn;
	auto monster = GManager->Object()->CreateObject<Monster>();
	{
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int> dis(-10, 10);

		Protocol::PositionInfo* pos = new Protocol::PositionInfo();
		pos->set_posx(dis(gen));
		pos->set_posy(dis(gen));
		monster->GetObjectInfo().set_allocated_posinfo(pos);

		monster->BeginPlay();
		monster->SetHp(100);
		m_monsters[monster->GetId()] = monster;
		Protocol::ObjectInfo* info = spawn.add_monsters();
		info->set_objectid(monster->GetId());
		info->set_health(monster->GetHp());
		info->set_name("monster");

		Protocol::PositionInfo* posInfo = new Protocol::PositionInfo();
		*posInfo = monster->GetObjectInfo().posinfo();
		info->set_allocated_posinfo(posInfo);

		cout << m_name << endl;
		cout << spawn.DebugString() << endl;
	}
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(spawn);
	BroadCast(move(*sendBuffer));
}

void Room::BeginPlay()
{
	TimerPushJob(1000, &Room::SpawnMonster); // Spawn Monster
}

void Room::Tick()
{
	TimerPushJob(33, &Room::Tick); // 30FPS
	for (auto& [id, player] : m_players)
	{
		player->Tick();
	}
	for (auto& [id, monster] : m_monsters)
	{
		monster->Tick();
	}
}