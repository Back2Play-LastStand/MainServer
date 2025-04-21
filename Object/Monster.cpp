#include "pch.h"
#include "Monster.h"
#include <cmath>

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

void Monster::UpdateState(float dist)
{
	if (GetHp() <= 0)
	{
		m_state = MonsterState::Dead;
		return;
	}

	if (dist <= 5.f)
		m_state = MonsterState::Attack;
	else if (dist <= 10.f)
		m_state = MonsterState::Chase;
	else
		m_state = MonsterState::Idle;
}

void Monster::BeginPlay()
{
	Super::BeginPlay();
}

void Monster::Tick()
{
	Super::Tick();

	float best = FLT_MAX;
	for (const auto& [id, player] : m_room->GetPlayers())
	{
		if (player)
		{
			auto& playerPos = player->GetObjectInfo().posinfo();
			auto& monsterPos = m_info.posinfo();
			auto now = GetDistance(monsterPos, playerPos);

			if (best > now)
			{
				best = now;
				m_target = player;
			}
		}
	}

	if (m_target)
	{
		UpdateState(sqrtf(best));

		switch (m_state)
		{
		case MonsterState::Idle:
			break;
		case MonsterState::Chase:
			break;
		case MonsterState::Attack:
			Attack();
			break;
		case MonsterState::Dead:
			break;
		}
	}
}
