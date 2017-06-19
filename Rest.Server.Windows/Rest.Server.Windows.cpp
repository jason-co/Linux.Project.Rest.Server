// Rest.Server.Windows.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "restServer.h"

using namespace std;

int main()
{
	utility::string_t port = U("34568");

	utility::string_t address = U("http://localhost:");
	address.append(port);

	std::unique_ptr<restServer> server;
	server = std::unique_ptr<restServer>(new restServer(address));
	server->on_initialize(address);

	std::cout << "Press ENTER to exit." << std::endl;

	std::string line;
	std::getline(std::cin, line);
	
	server->on_shutdown();

    return 0;
}

