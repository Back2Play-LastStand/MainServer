#include "pch.h"
#include "Network/Server.h"
#include "Manager/Manager.h"

Manager* GManager;

int main()
{
	ServerPacketHandler::Init();
	cppx::native::init(5);
	GEngine = new Engine();
	GManager = new Manager();

	GEngine->Init();
	GManager->Init();

	auto endpoint = cppx::endpoint(cppx::ip_address::any, 3333);
	auto server = MakeShared<Server>();
	server->Run(endpoint);

	GEngine->RunThread(4, 1);
	while (true)
	{
	}
}