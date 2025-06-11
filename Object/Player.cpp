#include "pch.h"
#include "Player.h"

Player::Player(unsigned long long id)
{
	SetId(id);
	SetType(Protocol::PLAYER);
}

Player::~Player()
{
}

void Player::SetSession(GameSession* session)
{
	m_session = session;
}

GameSession* Player::GetSession() const
{
	return m_session;
}

string Player::GetName() const
{
	return m_playerName;
}

void Player::SetName(string name)
{
	m_playerName = name;
}

void Player::SetObjectInfo(Protocol::ObjectInfo info)
{
	m_info = info;
}

Protocol::ObjectInfo& Player::GetObjectInfo()
{
	return m_info;
}

void Player::BeginPlay()
{
	Super::BeginPlay();
}

void Player::Tick()
{
	Super::Tick();
	float now = GetTickCount64();

	if (now < m_lastHitTime + 3000) // 3s
		return;

	if (now >= m_lastRegenTime + 5000) // 5s
	{
		m_lastRegenTime = now;
		unsigned int hp = GetHp();
		hp += 10;
		SetHp(min(hp, m_maxHp));

		auto change = new Protocol::RES_CHANGE_HP;
		change->set_objectid(GetId());
		change->set_hp(GetHp());
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(*change);
		m_room.lock()->BroadCast(move(*sendBuffer));
	}
}

void Player::TakeDamage(shared_ptr<GameObject> attacker, int amount)
{
	Super::TakeDamage(attacker, amount);
}

void Player::EnterRoom(shared_ptr<Room> gameRoom)
{
	if (auto room = m_room.lock())
		LeaveRoom();

	m_room = gameRoom;
	if (auto room = m_room.lock())
		gameRoom->EnterObject(static_pointer_cast<Player>(shared_from_this()));
}

void Player::LeaveRoom()
{
	if (auto room = m_room.lock())
	{
		room->PushJob(&Room::LeaveObject, shared_from_this());
		m_room.reset();
	}
}
