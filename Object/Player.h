#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player(unsigned long long id);
	virtual ~Player();

	GameSession* GetSession() const;
	shared_ptr<Room> GetRoom() const;

private:
	GameSession* m_session;
	weak_ptr<Room> m_room;
};

