#pragma once

class Player : public enable_shared_from_this<Player>
{
public:
	Player();
	virtual ~Player();

private:
	weak_ptr<GameSession> m_session;
	atomic<weak_ptr<Room>> m_room;
};

