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

shared_ptr<Room> Monster::GetRoom()
{
	return m_room;
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

void Monster::TakeDamage(int amount)
{
	Super::TakeDamage(amount);
}