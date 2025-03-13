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
	auto myPlayer = gameSession->GetPlayer();
	auto room = myPlayer->GetRoom();

	Protocol::RES_ENTER_ROOM res;
	auto success = myPlayer->GetRoom() == nullptr;
	if (success)
	{
		res.set_success(success);

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
					info->set_objectid(myPlayer->GetId());
					info->set_name(myPlayer->GetName());
				}
			}

			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(spawn);
			session->SendContext(move(*sendBuffer));
		}
	}
}
