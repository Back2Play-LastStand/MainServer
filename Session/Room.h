#pragma once
#include "Network/Session.h"
#include "Protocol.pb.h"
#include "Path/PathFinder.h"

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
	void HandleAttack(Session* session, Protocol::REQ_ATTACK_OBJECT pkt);
	void HandleRespawnPlayer(Session* session, Protocol::REQ_RESPAWN pkt);
	void SpawnMonster();

public:
	shared_ptr<Player> FindClosestPlayer(int x, int y);
	vector<pair<int, int>> FindPath(int sx, int sz, int ex, int ez);

	void BeginPlay();
	void Tick();

private:
	unordered_map<unsigned long long, shared_ptr<Player>> m_players;
	unordered_map<unsigned long long, shared_ptr<Monster>> m_monsters;
	string m_name;
	PathFinder _pathFinder;
};

extern shared_ptr<Room> GRoom;