#pragma once

class Room;

class RoomManager : public JobSerializer
{
public:
	RoomManager();

	void HandleEnterRoom(Session* session, Protocol::REQ_ENTER_ROOM pkt);
};

