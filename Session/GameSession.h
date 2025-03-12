#pragma once
#include "Network/Session.h"

class GameSession : public Session
{
public:
	GameSession() = default;
	~GameSession() = default;

	virtual void OnConnected(endpoint ep) override;
	virtual void OnRecv(byte* buffer, int len) override;
	virtual void OnSend(int len) override;
	virtual void OnDisconnected() override;

	Session* GetSession();
	shared_ptr<Player> GetPlayer();
	void SetPlayer(shared_ptr<Player> player);

private:
	shared_ptr<Player> m_player;
};

