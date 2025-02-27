#include "pch.h"
#include "GameSession.h"

void GameSession::OnConnected(endpoint ep)
{
}

int GameSession::OnRecv(byte* buffer, int len)
{
	return 0;
}

void GameSession::OnSend(int len)
{
}

void GameSession::OnDisconnected()
{
}

shared_ptr<Player> GameSession::GetPlayer()
{
	return m_player;
}
