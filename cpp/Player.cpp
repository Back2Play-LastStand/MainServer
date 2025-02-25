#include "pch.h"
#include "Player.h"

Player::Player()
{
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
