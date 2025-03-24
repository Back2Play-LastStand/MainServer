#include "pch.h"
#include "Player.h"

Player::Player(unsigned long long id)
{
	SetId(id);
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

void Player::SetPosition(Protocol::PositionInfo posInfo)
{
	m_posInfo = posInfo;
}

Protocol::PositionInfo Player::GetPosition()
{
	return m_posInfo;
}

void Player::BeginPlay()
{
	Super::BeginPlay();
}

void Player::Tick()
{
	Super::Tick();
}

void Player::EnterRoom(shared_ptr<Room> gameRoom)
{
	if (auto room = m_room.lock())
		LeaveRoom();

	m_room = gameRoom;
	if (auto room = m_room.lock())
		room->EnterPlayer(static_pointer_cast<Player>(shared_from_this()));
}

void Player::LeaveRoom()
{
	if (auto room = m_room.lock())
	{
		room->PushJob(&Room::LeavePlayer, GetId());
		m_room.reset();
	}
}
