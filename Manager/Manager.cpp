#include "pch.h"
#include "Manager.h"

Manager::Manager()
{
	m_object = MakeShared<ObjectManager>();
}

Manager::~Manager()
{
}
