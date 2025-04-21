#pragma once

class GameOjbect;
class Room;

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

	shared_ptr<Room> GetRoom();
	void SetObjectInfo(Protocol::ObjectInfo info);
	Protocol::ObjectInfo& GetObjectInfo();

	float GetDistance(const Protocol::PositionInfo& myPos, const Protocol::PositionInfo& targetPos);
	void UpdateState(float dist);

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void TakeDamage(int amount) override;

	virtual void Attack();

private:
	shared_ptr<Room> m_room;
	Protocol::ObjectInfo m_info;
	shared_ptr<Player> m_target = nullptr;
	MonsterState m_state = MonsterState::Idle;
	uint64_t m_lastAttackTime = 0;
};

