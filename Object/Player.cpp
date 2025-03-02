#include "pch.h"
#include "Player.h"

Player::Player(unsigned long long id)
{
	SetId(id);
}

Player::~Player()
{
}

GameSession* Player::GetSession() const
{
	return m_session;
}

shared_ptr<Room> Player::GetRoom() const
{
	return m_room.lock();
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
		return;

	m_room = gameRoom;
	if (auto room = m_room.lock())
		room->EnterPlayer(static_pointer_cast<Player>(shared_from_this()));
}

void Player::LeaveRoom()
{
	if (auto room = m_room.lock())
	{
		room->LeavePlayer(GetId());
		m_room.reset();
	}
}
