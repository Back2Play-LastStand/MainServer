#pragma once
#include <tbb/tbb.h>

class Room;

class RoomManager : public JobSerializer
{
public:
	RoomManager();

	void HandleCreateRoom(string roomName);
	void HandleJoinGameRoom(Session* session, Protocol::REQ_ENTER_GAMEROOM pkt);
	void HandleLeaveGameRoom(Session* session, Protocol::REQ_LEAVE_GAMEROOM pkt);
	void HandleEnterRoom(Session* session, Protocol::REQ_ENTER_ROOM pkt);

private:
	tbb::concurrent_unordered_map<string, shared_ptr<Room>> m_rooms;
};

