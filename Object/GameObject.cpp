#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(unsigned long long id, Protocol::ObjectType type) : m_objectId(id), m_objectType(type), m_lastTick(GetTickCount64())
{
}

GameObject::~GameObject()
{
}

void GameObject::Tick()
{
	uint64_t now = GetTickCount64();
	m_delta = static_cast<float>(now - m_lastTick);
	m_lastTick = GetTickCount64();
}

void GameObject::TakeDamage(int amount)
{
	m_hp -= amount;
	if (m_hp <= 0)
	{
		m_hp = 0;
		// Destroy
		return;
	}
}