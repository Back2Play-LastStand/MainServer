#pragma once

class GameOjbect;
class Room;

class Monster : public GameObject
{
	using Super = GameObject;
public:
	Monster();
	virtual ~Monster();

	shared_ptr<Room> GetRoom();

	virtual void BeginPlay() override;
	virtual void Tick() override;

	virtual void Attack() {};

private:
	shared_ptr<Room> m_room;
};

