#pragma once

class Room
{
public:
	Room();
	virtual ~Room();

private:
	unordered_map<unsigned long long, Player> m_players;
};

