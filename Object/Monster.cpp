#include "pch.h"
#include "Monster.h"

Monster::Monster()
{
	SetType(Protocol::MONSTER);
}

Monster::~Monster()
{
}

shared_ptr<Room> Monster::GetRoom()
{
	return m_room;
}

void Monster::BeginPlay()
{
	Super::BeginPlay();
}

void Monster::Tick()
{
	Super::Tick();
}
