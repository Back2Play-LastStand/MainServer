#pragma once
#include "Enum.pb.h"

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

	void SetId(unsigned long long id) { m_objectId = id; }
	void SetType(Protocol::ObjectType type) { m_objectType = type; }
	void SetHp(unsigned int hp) { m_hp = hp; }
	void SetPower(unsigned int power) { m_power = power; }

	virtual void BeginPlay() {};
	virtual void Tick();

	virtual void TakeDamage(int amount) {};

private:
	unsigned long long m_objectId;
	Protocol::ObjectType m_objectType;
	unsigned int m_hp = 0;
	unsigned int m_power = 0;
	unsigned long long m_delta = 0;
	unsigned long long m_lastTick;
};

