#pragma once

class ObjectManager;
class RoomManager;

class Manager
{
public:
	Manager();
	virtual ~Manager();

	void Init();

	shared_ptr<ObjectManager> Object() { return m_object; }
	shared_ptr<RoomManager> Room() { return m_room; }

private:
	shared_ptr<ObjectManager> m_object;
	shared_ptr<RoomManager> m_room;
};

extern Manager* GManager;