#pragma once

class GameObject
{
public:
	GameObject() = default;
	virtual ~GameObject();

	unsigned long long GetId() const { return m_objectId; }
	void SetId(unsigned long long id) { m_objectId = id; }

private:
	unsigned long long m_objectId;
};

