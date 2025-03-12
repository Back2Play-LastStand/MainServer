#pragma once

class ObjectManager;

class Manager
{
public:
	Manager();
	virtual ~Manager();

	shared_ptr<ObjectManager> Object() { return m_object; }

private:
	shared_ptr<ObjectManager> m_object;
};

extern shared_ptr<Manager> GManager;