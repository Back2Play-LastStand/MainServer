#pragma once
#include "Network/Session.h"

class GameSession : public Session
{
public:
	GameSession() = default;
	~GameSession() = default;

	virtual void OnConnected(endpoint ep) override;
	virtual int OnRecv(byte* buffer, int len) override;
	virtual void OnSend(int len) override;
	virtual void OnDisconnected() override;

