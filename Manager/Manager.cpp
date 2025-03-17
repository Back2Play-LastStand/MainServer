#include "pch.h"
#include "Manager.h"
#include "ObjectManager.h"
#include "RoomManager.h"

Manager::Manager()
{
}

Manager::~Manager()
{
}

void Manager::Init()
{
	m_object = MakeShared<ObjectManager>();
	m_room = MakeShared<RoomManager>();
}
