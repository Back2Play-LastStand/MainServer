#include "pch.h"
#include "ServerPacketHandler.h"

#include "Manager.h"
#include "ObjectManager.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(Session* session, BYTE* buffer, int len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	return false;
}

bool Handle_REQ_ENTER(Session* session, Protocol::REQ_ENTER& pkt)
{	
	GManager->Object()->PushJob(&ObjectManager::HandleEnterGame, session, pkt);
	return false;
}
