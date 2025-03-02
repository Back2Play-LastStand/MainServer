#include "pch.h"
#include "GameSession.h"

void GameSession::OnConnected(endpoint ep)
{
}

int GameSession::OnRecv(byte* buffer, int len)
{
	auto session = GetSession();
	ServerPacketHandler::HandlePacket(session, buffer, len);
}

void GameSession::OnSend(int len)
{
}

void GameSession::OnDisconnected()
{
	m_player->LeaveRoom();
	// TODO
}

Session* GameSession::GetSession()
{
	return this;
}

shared_ptr<Player> GameSession::GetPlayer()
{
	return m_player;
}
