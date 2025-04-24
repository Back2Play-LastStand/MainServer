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

shared_ptr<Room> Player::GetRoom() const
{
	return m_room.lock();
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
}

void Player::TakeDamage(int amount)
{
	Super::TakeDamage(amount);
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
