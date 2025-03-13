#pragma once

class Room : public JobSerializer
{
public:
	Room();
	virtual ~Room();

	unordered_map<unsigned long long, shared_ptr<Player>> GetPlayers();

	bool EnterPlayer(shared_ptr<Player> player);
	bool LeavePlayer(unsigned long long objectId);
	void BroadCast(vector<char> buffer, unsigned long long exceptId = 0);

private:
	unordered_map<unsigned long long, shared_ptr<Player>> m_players;
};

extern shared_ptr<Room> GRoom;