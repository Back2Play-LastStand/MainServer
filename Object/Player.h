#pragma once
#include "GameObject.h"
#include "Struct.pb.h"

class GameSession;

class Player : public GameObject
{
	using Super = GameObject;
public:
	Player(unsigned long long id);
	virtual ~Player();

	void SetSession(GameSession* session);
	GameSession* GetSession() const;
	string GetName() const;
	void SetName(string name);
	void SetObjectInfo(Protocol::ObjectInfo info);
	Protocol::ObjectInfo& GetObjectInfo();

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void TakeDamage(shared_ptr<GameObject> attacker, int amount) override;

	virtual void EnterRoom(shared_ptr<Room> gameRoom) override;
	virtual void LeaveRoom() override;

private:
	GameSession* m_session;
	string m_playerName;
	Protocol::ObjectInfo m_info;

	float m_lastRegenTime = 0;
};

