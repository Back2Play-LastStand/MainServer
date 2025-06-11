#pragma once
#include "Enum.pb.h"

class Room;

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject() = default;
	GameObject(unsigned long long id, Protocol::ObjectType type);
	virtual ~GameObject();

	unsigned long long GetId() const { return m_objectId; }
	Protocol::ObjectType GetType() const { return m_objectType; }
	unsigned int GetHp() const { return m_hp; }
	unsigned int GetPower() const { return m_power; }
	virtual shared_ptr<Room> GetRoom() const;

	void SetId(unsigned long long id) { m_objectId = id; }
	void SetType(Protocol::ObjectType type) { m_objectType = type; }
	void SetHp(unsigned int hp) { m_hp = hp; }
	void SetPower(unsigned int power) { m_power = power; }
	void SetRoom(shared_ptr<Room> room) { m_room = room; }

	virtual void BeginPlay() {};
	virtual void Tick();

	virtual void TakeDamage(shared_ptr<GameObject> attacker, int amount);
	virtual void OnDead(shared_ptr<GameObject> attacker);

	virtual void EnterRoom(shared_ptr<Room> gameRoom) {};
	virtual void LeaveRoom() {};

protected:
	weak_ptr<Room> m_room;
	float m_lastHitTime = 0;
	unsigned int m_maxHp = 100;

private:
	unsigned long long m_objectId;
	Protocol::ObjectType m_objectType;
	unsigned int m_hp = 0;
	unsigned int m_power = 0;
	unsigned long long m_delta = 0;
	unsigned long long m_lastTick;
};

