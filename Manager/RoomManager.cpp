#include "pch.h"
#include "RoomManager.h"

RoomManager::RoomManager()
{
}

void RoomManager::HandleEnterRoom(Session* session, Protocol::REQ_ENTER_ROOM pkt)
{
	if (!session)
		return;

	auto gameSession = static_cast<GameSession*>(session);
	auto it = m_rooms.insert({ pkt.name(), make_shared<Room>()});
	auto& room = it.first->second;
	auto myPlayer = gameSession->GetPlayer();

	Protocol::RES_ENTER_ROOM res;
	res.set_success(myPlayer->GetRoom() == nullptr);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(res);
	session->SendContext(move(*sendBuffer));

	if (res.success())
	{
		{
			myPlayer->EnterRoom(room);

			Protocol::RES_SPAWN spawn;
			Protocol::ObjectInfo* info = new Protocol::ObjectInfo();
			info->set_objectid(myPlayer->GetId());
			info->set_name(myPlayer->GetName());
			spawn.set_allocated_player(info);

			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(spawn);
			session->SendContext(move(*sendBuffer));
		}

		// send other player
		{
			Protocol::RES_SPAWN_ALL spawn;
			for (auto& it : room->GetPlayers())
			{
				auto& player = it.second;
				if (player->GetId() != myPlayer->GetId())
				{
					Protocol::ObjectInfo* info = spawn.add_players();
					info->set_objectid(player->GetId());
					info->set_name(player->GetName());
				}
			}

			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(spawn);
			session->SendContext(move(*sendBuffer));
		}
	}
}
