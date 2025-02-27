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
}

void Player::Tick()
{
}
