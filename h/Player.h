#pragma once

class Player : public enable_shared_from_this<Player>
{
public:
	Player();
	virtual ~Player();

	unsigned long long GetId() const;
	GameSession* GetSession() const;
	shared_ptr<Room> GetRoom() const;

private:
	unsigned long long m_id;
	GameSession* m_session;
	weak_ptr<Room> m_room;
};

