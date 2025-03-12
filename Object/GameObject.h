#pragma once

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject() = default;
	GameObject(unsigned long long id, Protocol::ObjectType type);
	virtual ~GameObject();

	unsigned long long GetId() const { return m_objectId; }
	unsigned int GetHp() const { return m_hp; }

	void SetId(unsigned long long id) { m_objectId = id; }
	void SetHp(unsigned int hp) { m_hp = hp; }

	virtual void BeginPlay() {};
	virtual void Tick() {};

private:
	unsigned long long m_objectId;
	Protocol::ObjectType m_objectType;
	unsigned int m_hp = 0;
};

