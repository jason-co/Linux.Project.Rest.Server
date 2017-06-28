#include "linuxRestServer.h"


linuxRestServer::linuxRestServer()
{
}

linuxRestServer::linuxRestServer(utility::string_t url) :m_listener(url)
{
	m_listener.support(methods::GET, std::bind(&linuxRestServer::handle_get, this, std::placeholders::_1));
	m_listener.support(methods::PUT, std::bind(&linuxRestServer::handle_put, this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&linuxRestServer::handle_post, this, std::placeholders::_1));
	m_listener.support(methods::DEL, std::bind(&linuxRestServer::handle_delete, this, std::placeholders::_1));
}

linuxRestServer::~linuxRestServer()
{
}

void linuxRestServer::on_initialize(const string_t & address)
{
	uri_builder uri(address);

	auto addr = uri.to_uri().to_string();

	this->open().wait();

	isRunning = true;

	ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;

	return;
}

void linuxRestServer::on_shutdown()
{
	this->close().wait();
	return;
}

void linuxRestServer::handle_error(pplx::task<void>& t)
{
	try
	{
		t.get();
	}
	catch (...)
	{
		// Ignore the error, Log it if a logger is available
	}
}

bool linuxRestServer::getIsRunning()
{
	return isRunning;
}

std::vector<utility::string_t> requestPath(const http_request & message) {
	auto relativePath = uri::decode(message.relative_uri().path());
	return uri::split_path(relativePath);
}

//
// Get Request 
//
void linuxRestServer::handle_get(http_request message)
{
	auto path = requestPath(message);

	if (!path.empty()) {
		if (path[0] == U("service") && path[1] == U("test")) {
			auto response = json::value::object();
			response[U("version")] = json::value::string(U("0.1.1"));
			response[U("status")] = json::value::string(U("ready!"));
			message.reply(status_codes::OK, response);
		}
		else if (path[0] == U("api")) {
			auto response = json::value::object();
			response[U("body")] = json::value::string(message.to_string());

			if (path[1] == U("status")) {
				response[U("path")] = json::value::string(U("Status GET response"));
			}
			else if (path[1] == U("shutdown")) {
				isRunning = false;
				response[U("message")] = json::value::string(U("Shutdown response"));
			}
			ucout << response.serialize() << endl;

			message.reply(status_codes::OK, response);

			return;
		}
	}

	auto response = json::value::object();
	response[U("body")] = json::value::string(message.to_string());

	ucout << response.serialize() << endl;

	message.reply(status_codes::OK, response);

	return;

};

//
// A POST request
//
void linuxRestServer::handle_post(http_request message)
{
	auto path = requestPath(message);

	if (!path.empty()) {
		if (path[0] == U("api")) {

			auto response = json::value::object();
			response[U("body")] = json::value::string(message.to_string());

			if (path[1] == U("status")) {
				response[U("message")] = json::value::string(U("Status POST response"));
			}
		}
	}

	ucout << message.to_string() << endl;

	auto body = message.extract_json();

	message.reply(status_codes::OK, body.get().serialize());
	return;
};

//
// A DELETE request
//
void linuxRestServer::handle_delete(http_request message)
{
	ucout << message.to_string() << endl;

	string rep = "WRITE YOUR OWN DELETE OPERATION";
	message.reply(status_codes::OK, rep);
	return;
};


//
// A PUT request 
//
void linuxRestServer::handle_put(http_request message)
{
	ucout << message.to_string() << endl;
	string rep = "WRITE YOUR OWN PUT OPERATION";
	message.reply(status_codes::OK, rep);
	return;
};
