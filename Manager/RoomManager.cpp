#include "pch.h"
#include "RoomManager.h"
#include <random>

RoomManager::RoomManager()
{
	auto names = vector<string>{ "roomname", "room1", "room2" };
	for (auto& name : names)
	{
		auto room = MakeShared<Room>(name);
		room->BeginPlay();
		m_rooms.insert({ name ,room });
	}
}

void RoomManager::HandleEnterRoom(Session* session, Protocol::REQ_ENTER_ROOM pkt)
{
	if (!session)
		return;

	auto gameSession = static_cast<GameSession*>(session);
	auto& room = m_rooms[pkt.name()];
	auto myPlayer = gameSession->GetPlayer();

	Protocol::RES_ENTER_ROOM res;
	res.set_success(room != nullptr && myPlayer->GetRoom() != room);
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
			// Temp
			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<int> dis(-2, 2);
			info->mutable_posinfo()->set_posx(dis(gen));
			info->mutable_posinfo()->set_posy(dis(gen));
			spawn.set_allocated_player(info);
			spawn.set_mine(true);

			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(spawn);
			session->SendContext(move(*sendBuffer));
		}

		{
			Protocol::RES_SPAWN spawn;
			auto info = spawn.mutable_player();
			info->set_objectid(myPlayer->GetId());
			info->set_name(myPlayer->GetName());
			info->mutable_posinfo()->set_posx(myPlayer->GetObjectInfo().posinfo().posx());
			info->mutable_posinfo()->set_posy(myPlayer->GetObjectInfo().posinfo().posy());
			spawn.set_mine(false);

			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(spawn);
			room->BroadCast(move(*sendBuffer), info->objectid());
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

					Protocol::PositionInfo* posInfo = new Protocol::PositionInfo();
					*posInfo = player->GetObjectInfo().posinfo();

					info->set_allocated_posinfo(posInfo);
				}
			}

			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(spawn);
			session->SendContext(move(*sendBuffer));
		}

		// spawn monster
		{
			Protocol::RES_SPAWN_MONSTER spawn;
			for (auto& it : room->GetMonsters())
			{
				auto& monster = it.second;
				Protocol::ObjectInfo* info = spawn.add_monsters();
				info->set_objectid(monster->GetId());
				*info->mutable_posinfo() = monster->GetObjectInfo().posinfo();
				// TODO
			}
			
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(spawn);
			room->BroadCast(move(*sendBuffer));
		}
	}
}
