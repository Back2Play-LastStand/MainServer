#pragma once

class Player : public enable_shared_from_this<Player>
{
public:
	Player();
	virtual ~Player();

	GameSession* GetSession() const;
	shared_ptr<Room> GetRoom() const;

private:
	GameSession* m_session;
	weak_ptr<Room> m_room;
};

