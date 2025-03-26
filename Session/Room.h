#pragma once
#include "Network/Session.h"
#include "Protocol.pb.h"

class Room : public JobSerializer
{
public:
	Room() = default;
	Room(string name);
	virtual ~Room();

	unordered_map<unsigned long long, shared_ptr<Player>> GetPlayers();
	string GetName();

	bool EnterPlayer(shared_ptr<Player> player);
	bool LeavePlayer(unsigned long long objectId);
	void BroadCast(vector<char> buffer, unsigned long long exceptId = 0);
 
	void HandleMove(Session* session, Protocol::REQ_MOVE pkt);

private:
	unordered_map<unsigned long long, shared_ptr<Player>> m_players;
	string m_name;
};

extern shared_ptr<Room> GRoom;