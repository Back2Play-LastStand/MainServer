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

void Monster::BeginPlay()
{
	Super::BeginPlay();
}

void Monster::Tick()
{
	Super::Tick();

	// Update AI State
	switch (m_state)
	{
	case MonsterState::Idle:
		UpdateIdle();
		break;
	case MonsterState::Chase:
		UpdateChase();
		break;
	case MonsterState::Attack:
		UpdateAttack();
		break;
	case MonsterState::Dead:
		break;
	}
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

// Monster AI State
void Monster::SetState(MonsterState newState)
{
	if (m_state == newState)
		return;

	m_state = newState;

	// Enter New State
	switch (m_state)
	{
	case MonsterState::Idle:
		OnEnterIdle();
		break;
	case MonsterState::Chase:
		OnEnterChase();
		break;
	case MonsterState::Attack:
		OnEnterAttack();
		break;
	case MonsterState::Dead:
		OnEnterDead();
		break;
	}
}

void Monster::UpdateIdle()
{
	auto target = FindNearestPlayer();

	if (target)
	{
		m_target = target;
		SetState(MonsterState::Chase);
	}
}

void Monster::UpdateChase()
{
	auto& target = m_target;
	if (!target)
	{
		SetState(MonsterState::Idle);
		return;
	}

	if (IsInAttackRange(target))
	{
		SetState(MonsterState::Attack);
		return;
	}

	if (NeedRepath())
	{
		auto room = m_room.lock();
		if (room)
		{
			auto path = room->FindPath(GetX(), GetY(), target->GetX(), target->GetY());
			m_path.clear();
			for (auto& pos : path)
				m_path.push_back(pos);
			m_lastPathTime = GetTickCount64();
		}
	}

	MoveAlongPath();
}

void Monster::UpdateAttack()
{
	auto& target = m_target;
	if (!target)
	{
		SetState(MonsterState::Idle);
		return;
	}

	if (!IsInAttackRange(target))
	{
		SetState(MonsterState::Chase);
		return;
	}

	uint64_t now = GetTickCount64();
	if (now - m_lastAttackTime > 2000) // 2s
	{
		Attack(target);
		m_lastAttackTime = now;
	}
}

void Monster::OnEnterIdle()
{
	m_target = nullptr;
	m_path.clear();
}

void Monster::OnEnterChase()
{
	m_lastPathTime = 0;
}

void Monster::OnEnterAttack()
{
	m_lastAttackTime = 0;
}

void Monster::OnEnterDead()
{
	LeaveRoom();
}

shared_ptr<Player> Monster::FindNearestPlayer()
{
	auto room = m_room.lock();
	if (!room) return nullptr;

	return room->FindClosestPlayer(GetX(), GetY());
}

bool Monster::IsInAttackRange(shared_ptr<Player> target)
{
	return Distance(GetX(), target->GetX(), GetY(), target->GetY()) < 2.0f;
}

void Monster::Attack(shared_ptr<Player> target)
{
	if (!target) return;

	target->TakeDamage(shared_from_this(), 10);
}

bool Monster::NeedRepath()
{
	uint64_t now = GetTickCount64();
	return (now - m_lastPathTime) > 200; // 200ms
}

void Monster::MoveAlongPath()
{
	if (m_path.empty())
		return;

	uint64_t now = GetTickCount64();
	if (now - m_lastMoveTime < 300)
		return;

	m_lastMoveTime = now;

	auto [x, y] = m_path.front();
	cout << "move to: " << x << "," << y << endl;
	SetPos(x, y);
	Protocol::RES_MOVE_MONSTER move;
	auto info = move.mutable_monster();
	info->set_objectid(GetId());
	auto posInfo = info->mutable_posinfo();
	posInfo->set_posx(GetX());
	posInfo->set_posy(GetY());

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(move);
	if (auto room = GetRoom())
	{
		for (auto& [id, p] : room->GetPlayers())
		{
			if (auto s = p->GetSession())
				s->SendContext(*sendBuffer);
		}
	}

	m_path.pop_front();
}

int Monster::Distance(int x1, int x2, int y1, int y2)
{
	return abs(x1 - x2) + abs(y1 - y2); // Manhattan distance
}
