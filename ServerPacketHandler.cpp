#include "pch.h"
#include "ServerPacketHandler.h"

#include "Manager.h"
#include "ObjectManager.h"
#include "RoomManager.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(Session* session, BYTE* buffer, int len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	return false;
}

bool Handle_REQ_ENTER(Session* session, Protocol::REQ_ENTER& pkt)
{
	cout << "req enter" << endl;
	GManager->Object()->PushJob(&ObjectManager::HandleEnterGame, session, pkt);
	return false;
}

bool Handle_REQ_ENTER_ROOM(Session* session, Protocol::REQ_ENTER_ROOM& pkt)
{
	cout << "req enter room" << endl;
	GManager->Room()->PushJob(&RoomManager::HandleEnterRoom, session, pkt);
	return false;
}

bool Handle_REQ_LEAVE(Session* session, Protocol::REQ_LEAVE& pkt)
{
	return false;
}

bool Handle_REQ_MOVE(Session* session, Protocol::REQ_MOVE& pkt)
{
	auto gameSession = static_cast<GameSession*>(session);
	if (auto room = gameSession->GetPlayer()->GetRoom())
		room->PushJob(&Room::HandleMove, session, pkt);
	return false;
}
