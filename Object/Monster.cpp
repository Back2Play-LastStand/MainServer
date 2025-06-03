#include "pch.h"
#include "Monster.h"
#include <cmath>

Monster::Monster(unsigned long long id, unsigned int power)
{
	SetId(id);
	SetPower(power);
	SetType(Protocol::MONSTER);
}

Monster::~Monster()
{
}

void Monster::SetObjectInfo(Protocol::ObjectInfo info)
{
	m_info = info;
}

Protocol::ObjectInfo& Monster::GetObjectInfo()
{
	return m_info;
}

void Monster::BeginPlay()
{
	Super::BeginPlay();
}

void Monster::Tick()
{
	Super::Tick();
}

void Monster::TakeDamage(shared_ptr<GameObject> attacker, int amount)
{
	Super::TakeDamage(attacker, amount);
}

void Monster::EnterRoom(shared_ptr<Room> gameRoom)
{
	if (auto room = m_room.lock())
		LeaveRoom();

	m_room = gameRoom;
	if (auto room = m_room.lock())
		gameRoom->EnterObject(static_pointer_cast<Monster>(shared_from_this()));
}

void Monster::LeaveRoom()
{
	if (auto room = m_room.lock())
	{
		room->PushJob(&Room::LeaveObject, shared_from_this());
		m_room.reset();
	}
}
