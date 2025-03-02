#pragma once
#include "GameObject.h"

class GameSession;
class Room;

class Player : public GameObject
{
	using Super = GameObject;
public:
	Player(unsigned long long id);
	virtual ~Player();

	GameSession* GetSession() const;
	shared_ptr<Room> GetRoom() const;

	virtual void BeginPlay() override;
	virtual void Tick() override;

	void EnterRoom(shared_ptr<Room> gameRoom);
	void LeaveRoom();

private:
	GameSession* m_session;
	weak_ptr<Room> m_room;
};

