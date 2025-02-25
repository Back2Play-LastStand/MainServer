#pragma once

class Room
{
public:
	Room();
	virtual ~Room();

	void BroadCast(vector<char> buffer, unsigned long long exceptId = 0);

private:
	unordered_map<unsigned long long, shared_ptr<Player>> m_players;
};

