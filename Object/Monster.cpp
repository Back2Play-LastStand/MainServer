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
			Idle();
			break;
		case MonsterState::Chase:
			Chase();
			break;
		case MonsterState::Attack:
			Attack();
			break;
		case MonsterState::Dead:
			break;
		}
	}
}

void Monster::TakeDamage(int amount)
{
	Super::TakeDamage(amount);
}

void Monster::Idle()
{
}

void Monster::Chase()
{
	if (!m_target)
		return;

	auto& monsterPos = m_info.posinfo();
	auto& target = m_target->GetObjectInfo().posinfo();

	float dx = target.posx() - monsterPos.posx();
	float dz = target.posy() - monsterPos.posy();

	float lenSq = dx * dx * dz * dz;
	if (lenSq < 0.0001f)
		return;

	float len = sqrtf(lenSq);
	dx /= len;
	dz /= len;

	Protocol::PositionInfo* pos = m_info.mutable_posinfo();
	pos->set_posx(pos->posx() + dx);
	pos->set_posy(pos->posy() + dz);
	m_info.set_allocated_posinfo(pos);
}

void Monster::Attack()
{
	if (!m_target)
		return;

	uint64_t now = ::GetTickCount64();
	if (now - m_lastAttackTime < 2000) return;
	m_lastAttackTime = now;

	m_target->TakeDamage(GetPower());
}