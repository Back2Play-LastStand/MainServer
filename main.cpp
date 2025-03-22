#include "pch.h"
#include "Network/Server.h"
#include "Manager/Manager.h"
#include "Thread/ThreadManager.h"
#include "Network/Client.h"

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
	auto server = Server::Make<GameSession>();
	server->Run(endpoint);

	GEngine->RunThread(4);

	while (true)
	{
		this_thread::sleep_for(0.1s);
	}

	GEngine->GetThreadManager()->Join();
}