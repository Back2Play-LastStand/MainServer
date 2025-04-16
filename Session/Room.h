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
	unordered_map<unsigned long long, shared_ptr<Monster>> GetMonsters();
	string GetName();

	bool EnterObject(shared_ptr<GameObject> object);
	bool LeaveObject(shared_ptr<GameObject> object);
	void BroadCast(vector<char> buffer, unsigned long long exceptId = 0);
 
	void HandleMove(Session* session, Protocol::REQ_MOVE pkt);
	void SpawnMonster();

	void Tick();

private:
	unordered_map<unsigned long long, shared_ptr<Player>> m_players;
	unordered_map<unsigned long long, shared_ptr<Monster>> m_monsters;
	string m_name;
};

extern shared_ptr<Room> GRoom;