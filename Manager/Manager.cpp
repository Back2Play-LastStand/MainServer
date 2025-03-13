#include "pch.h"
#include "Manager.h"
#include "ObjectManager.h"
#include "RoomManager.h"

Manager::Manager()
{
	m_object = MakeShared<ObjectManager>();
	m_room = MakeShared<RoomManager>();
}

Manager::~Manager()
{
}
