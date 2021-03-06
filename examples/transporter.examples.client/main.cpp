#include <iostream>

#include <boost/asio/buffer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/system_error.hpp>
#include <boost/version.hpp>

#include "transporter/NetworkStream.h"
#include "transporter/TransporterAdapterBoostAsioTcpSocket.h"

#include "TestMessage.h"

// Fix for Boost < 1.66.0 (boost::asio::io_service was renamed to boost::asio::io_context)
#if BOOST_VERSION < 106600
namespace boost
{
	namespace asio
	{
		typedef boost::asio::io_service io_context;
	}
}
#endif

int main()
{
	boost::asio::io_context context{}; // Instanciate an I/O Context needed by Boost sockets
	boost::asio::ip::tcp::socket tcpSocket{ context }; // Instanciate a Boost TCP socket
	boost::asio::ip::tcp::endpoint connectEndpoint{ boost::asio::ip::address_v4::from_string("127.0.0.1"), 23205 }; // Instanciate a class representing the endpoint we'll connect to
	transporter::adapters::boost::asio::TransporterAdapterBoostAsioTcpSocket boostAdapter{ tcpSocket }; // Instanciate an adapter for our Boost TCP socket
	transporter::network::io::NetworkStream networkStream{ boostAdapter }; // Instanciate a network stream that will use the adapter we just instanciated
	TestMessage testMessage{}; // Instanciate a test message // /!\ THIS CLASS MUST BE IDENTICAL IN BOTH CLIENT AND SERVER PROJECTS. /!\ It is best to put all network messages in a SHARED LIBRARY (it is not the case in this example for the sake of simplicity and clarity).
	
	std::cout << "Connecting to 127.0.0.1:23205..." << std::endl;

	try
	{
		tcpSocket.connect(connectEndpoint); // Connect to the server
		std::cout << "Successfully connected to server" << std::endl;
	}

	catch (const boost::system::system_error &e)
	{
		std::cerr << "Error connecting to server: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	// Set our test message's attributes values (this does NOT send anything, just setting values)
	testMessage.setStr1("Hello, World!");
	testMessage.setI1(2302);

	networkStream.sendMessage(testMessage); // Actually sends the message

	std::cout << "TestMessage sent, have a look at the server window!" << std::endl;


	std::cout << "Press RETURN to exit...";
	std::cin.get();

	return EXIT_SUCCESS;
}
