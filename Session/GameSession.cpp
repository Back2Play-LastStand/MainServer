#include "pch.h"
#include "GameSession.h"

void GameSession::OnConnected(endpoint ep)
{
	cout << "connected" << endl;
}

void GameSession::OnRecv(BYTE* buffer, int len)
{
	if (len < 3)
		return;

	int processLen = 0;
	PacketHeader header{};
	auto session = GetSession();

	while (true)
	{
		int dataSize = len - processLen;
		// 최소한 헤더는 파싱할 수 있어야 한다
		if (dataSize < sizeof(PacketHeader))
			break;

		header = *(reinterpret_cast<PacketHeader*>(&buffer[processLen]));
		// 헤더에 기록된 패킷 크기를 파싱할 수 있어야 한다
		if (dataSize < header.size)
			break;

		ServerPacketHandler::HandlePacket(session, &buffer[processLen], header.size);
		
		processLen += header.size;
	}

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

void GameSession::SetPlayer(shared_ptr<Player> player)
{
	m_player = player;
}
