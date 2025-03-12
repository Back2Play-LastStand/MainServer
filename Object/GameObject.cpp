#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(unsigned long long id, Protocol::ObjectType type) : m_objectId(id), m_objectType(type)
{
}

GameObject::~GameObject()
{
}
