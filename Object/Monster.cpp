#include "pch.h"
#include "Monster.h"

Monster::Monster(unsigned long long id)
{
	SetId(id);
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

float Monster::GetDistance(const Protocol::PositionInfo& myPos, const Protocol::PositionInfo& targetPos)
{
	float x = targetPos.posx() - myPos.posx();
	float y = targetPos.posy() - myPos.posy();
	return x * x + y * y;
}

void Monster::BeginPlay()
{
	Super::BeginPlay();
}

void Monster::Tick()
{
	Super::Tick();
}
