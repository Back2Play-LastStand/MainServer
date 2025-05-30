#include "pch.h"
#include "GameObject.h"
#include "Manager/ObjectManager.h"

GameObject::GameObject(unsigned long long id, Protocol::ObjectType type) : m_objectId(id), m_objectType(type), m_lastTick(GetTickCount64())
{
}

GameObject::~GameObject()
{
}

shared_ptr<Room> GameObject::GetRoom() const
{
	return m_room.lock();
}

void GameObject::Tick()
{
	uint64_t now = GetTickCount64();
	m_delta = static_cast<float>(now - m_lastTick);
	m_lastTick = GetTickCount64();
}

void GameObject::TakeDamage(shared_ptr<GameObject> attacker, int amount)
{
	m_hp -= amount;

	auto change = new Protocol::RES_CHANGE_HP;
	change->set_objectid(m_objectId);
	change->set_hp(m_hp);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(*change);
	m_room.lock()->BroadCast(move(*sendBuffer));

	if (m_hp <= 0)
	{
		m_hp = 0;
		OnDead(attacker);
		return;
	}
}

void GameObject::OnDead(shared_ptr<GameObject> attacker)
{
	auto die = new Protocol::RES_DIE;
	die->set_objectid(m_objectId);
	die->set_attacker(attacker->GetId());
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(*die);
	m_room.lock()->BroadCast(move(*sendBuffer));

	shared_ptr<Room> room = m_room.lock();
	room->LeaveObject(GManager->Object()->FindById(m_objectId));
}