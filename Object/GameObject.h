#pragma once

class GameObject
{
public:
	GameObject() = default;
	virtual ~GameObject();

	unsigned long long GetId() const { return m_objectId; }
	unsigned int GetHp() const { return m_hp; }

	void SetId(unsigned long long id) { m_objectId = id; }
	void SetHp(unsigned int hp) { m_hp = hp; }

	virtual void BeginPlay() {};
	virtual void Tick() {};

private:
	unsigned long long m_objectId;
	unsigned int m_hp = 0;
};

