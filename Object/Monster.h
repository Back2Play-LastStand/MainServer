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

	void SetObjectInfo(Protocol::ObjectInfo info);
	Protocol::ObjectInfo& GetObjectInfo();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void TakeDamage(shared_ptr<GameObject> attacker, int amount) override;

	virtual void EnterRoom(shared_ptr<Room> gameRoom) override;
	virtual void LeaveRoom() override;

private:
	Protocol::ObjectInfo m_info;
	shared_ptr<Player> m_target = nullptr;
	MonsterState m_state = MonsterState::Idle;
	uint64_t m_lastAttackTime = 0;
};

