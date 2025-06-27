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
		room->Tick();
		m_rooms.insert({ name ,room });
	}
}

void RoomManager::HandleCreateRoom(string roomName)
{
	auto room = MakeShared<Room>(roomName);
	m_rooms.insert({ roomName,room });
}

void RoomManager::HandleJoinGameRoom(Session* session, Protocol::REQ_ENTER_GAMEROOM pkt)
{
	auto gameSession = static_cast<GameSession*>(session);
	auto myPlayer = gameSession->GetPlayer();
	Protocol::RES_ENTER_GAMEROOM enter;

	int count = 1;
	if (pkt.iscreate())
		HandleCreateRoom(pkt.name());

	for (auto& it : m_rooms)
	{
		if (it.first == pkt.name())
		{
			auto& room = it.second;
			myPlayer->EnterRoom(it.second);
			for (auto& i : room->GetPlayers())
			{
				auto info = enter.add_players();
				info->set_name(i.second->GetName());
				count++;
			}
		}
	}

	enter.set_membercount(count);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enter);
	session->SendContext(move(*sendBuffer));
}

void RoomManager::HandleLeaveGameRoom(Session* session, Protocol::REQ_LEAVE_GAMEROOM pkt)
{
	auto gameSession = static_cast<GameSession*>(session);
	auto myPlayer = gameSession->GetPlayer();

	myPlayer->LeaveRoom();
}

void RoomManager::HandleEnterRoom(Session* session, Protocol::REQ_ENTER_ROOM pkt)
{
	if (!session)
		return;

	auto gameSession = static_cast<GameSession*>(session);
	auto& room = m_rooms[pkt.name()];
	room->BeginPlay();
	room->Tick();
	auto myPlayer = gameSession->GetPlayer();

	cout << room.get() << endl;
	cout << myPlayer->GetRoom().get() << endl;

	Protocol::RES_ENTER_ROOM res;
	res.set_success(room != nullptr && myPlayer->GetRoom().get() == room.get());
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(res);
	session->SendContext(move(*sendBuffer));

	if (res.success())
	{
		{
			Protocol::RES_SPAWN spawn;
			Protocol::ObjectInfo* info = new Protocol::ObjectInfo();
			info->set_objectid(myPlayer->GetId());
			info->set_name(myPlayer->GetName());
			info->set_health(myPlayer->GetHp());
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
			info->set_health(myPlayer->GetHp());
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
					info->set_health(player->GetHp());

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
			session->SendContext(move(*sendBuffer));
		}
	}
}
