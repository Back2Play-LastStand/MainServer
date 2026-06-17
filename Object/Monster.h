#pragma once

class GameOjbect;

enum class MonsterState
{
	Idle,
	Chase,
	Attack,
	Dead,
};

class Monster : public GameObject
{
	using Super = GameObject;
public:
	Monster(unsigned long long id, unsigned int power = 10);
	virtual ~Monster();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void TakeDamage(shared_ptr<GameObject> attacker, int amount) override;

	virtual void EnterRoom(shared_ptr<Room> gameRoom) override;
	virtual void LeaveRoom() override;

public:
	// Monster AI State
	void SetState(MonsterState newState);
	void UpdateIdle();
	void UpdateChase();
	void UpdateAttack();

	// Monster Set State
	void OnEnterIdle();
	void OnEnterChase();
	void OnEnterAttack();
	void OnEnterDead();

	shared_ptr<Player> FindNearestPlayer();
	bool IsInAttackRange(shared_ptr<Player> target);
	void Attack(shared_ptr<Player> target);

	bool NeedRepath();
	void MoveAlongPath();
	int Distance(int x1, int x2, int y1, int y2);

private:
	Protocol::ObjectInfo m_info;
	shared_ptr<Player> m_target = nullptr;
	MonsterState m_state = MonsterState::Idle;
	uint64_t m_lastAttackTime = 0;
	uint64_t m_lastPathTime = 0;
	deque<pair<int, int>> m_path;
};

