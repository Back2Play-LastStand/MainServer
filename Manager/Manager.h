#pragma once

class ObjectManager;

class Manager
{
public:
	Manager();
	virtual ~Manager();

private:
	shared_ptr<ObjectManager> m_object;
};

extern shared_ptr<Manager> GManager;