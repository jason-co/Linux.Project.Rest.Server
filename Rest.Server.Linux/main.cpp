#include <cstdio>
#include <chrono>
#include <thread>

#include "linuxRestServer.h"

int main()
{
	utility::string_t port = U("34568");

	utility::string_t address = U("http://localhost:");
	address.append(port);

	std::unique_ptr<linuxRestServer> server;
	server = std::unique_ptr<linuxRestServer>(new linuxRestServer(address));
	server->on_initialize(address);

	printf("Running Linux Rest Server\n");

	std::cout << "Send 'api/shutdown' GET request to close" << std::endl;

	while (server->getIsRunning()) {

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	server->on_shutdown();

	return 0;
}