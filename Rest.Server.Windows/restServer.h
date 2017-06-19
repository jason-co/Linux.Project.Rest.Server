#pragma once

#include "stdafx.h"

using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

class restServer
{
public:
	restServer();
	restServer(utility::string_t url);
	~restServer();

	void on_initialize(const string_t& address);
	void on_shutdown();

	pplx::task<void>open() { return m_listener.open(); }
	pplx::task<void>close() { return m_listener.close(); }

private:
	void handle_get(http_request message);
	void handle_put(http_request message);
	void handle_post(http_request message);
	void handle_delete(http_request message);
	void handle_error(pplx::task<void>& t);
	http_listener m_listener;
};

