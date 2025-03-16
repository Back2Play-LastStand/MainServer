#pragma once

class Room;

class RoomManager : public JobSerializer
{
public:
	RoomManager();

	void HandleEnterRoom(Session* session, Protocol::REQ_ENTER_ROOM pkt);

private:
	concurrent_unordered_map<string, shared_ptr<Room>> m_rooms;
};

