#include <iostream>

#include <boost/asio/basic_socket_acceptor.hpp>
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

// Function to pass to NetworkStream::receiveMessage() (see below in main:68) to instanciate the right type of INetworkMessage based on the receivedId
transporter::network::messages::NetworkMessagePtr networkMessageSelector(transporter::network::messages::NetworkMessageId receivedId)
{
	switch (receivedId)
	{
	case TestMessage::MESSAGE_ID:
		return transporter::network::messages::NetworkMessagePtr{ new TestMessage{} };
	default:
		return nullptr;
	}
}

int main()
{
	boost::asio::io_context context{}; // Instanciate an I/O Context needed by Boost sockets
	boost::asio::ip::tcp::endpoint bindEndpoint{ boost::asio::ip::address_v4::from_string("127.0.0.1"), 23205 }; // Instanciate a class representing the endpoint we'll bind to
	boost::asio::ip::tcp::acceptor tcpServer{ context, bindEndpoint }; // Instanciate a Boost TCP "acceptor" that will accept incoming connections
	boost::asio::ip::tcp::socket clientSocket{ context }; // Instanciate a Boost TCP socket that will be used for our client
	transporter::adapters::boost::asio::TransporterAdapterBoostAsioTcpSocket boostAdapter{ clientSocket }; // Instanciate an adapter for our Boost TCP socket
	transporter::network::io::NetworkStream networkStream{ boostAdapter }; // Instanciate a network stream that will use the adapter we just instanciated
	transporter::network::messages::NetworkMessagePtr recvMessage{}; // Instanciate a pointer to an INetworkMessage to store the message we'll receive later

	std::cout << "Listening on 127.0.0.1:23205..." << std::endl;

	try
	{
		tcpServer.accept(clientSocket); // Accept an incoming connection
		std::cout << "Successfully accepted client " << clientSocket.remote_endpoint().address().to_string() << ":" << clientSocket.remote_endpoint().port() << std::endl;
	}

	catch (const boost::system::system_error &e)
	{
		std::cerr << "Error accepting client: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}


	std::cout << "Waiting to receive test message..." << std::endl;

	// Loop while we haven't received a network message (this ugly loop isn't recommended, it is for simplicity and clarity)
	while (!recvMessage)
	{
		recvMessage = networkStream.receiveMessage(&networkMessageSelector);
	}


	if (recvMessage->getMessageId() == TestMessage::MESSAGE_ID) // Make sure we received the correct type of message (TestMessage)
	{
		TestMessage &testMessage = *dynamic_cast<TestMessage*>(recvMessage.get()); // Cast the base network message type into an actual TestMessage -- /!\ THIS CLASS MUST BE IDENTICAL IN BOTH CLIENT AND SERVER PROJECTS. /!\ It is best to put all network messages in a SHARED LIBRARY (it is not the case in this example for the sake of simplicity and clarity).

		std::cout << "Received TestMessage!" << std::endl;
		std::cout << "m_str1 = " << testMessage.getStr1() << ", m_i1 = " << testMessage.getI1() << std::endl; // Print received attributes values
	}

	else
	{
		std::cerr << "Something went wrong, received a network message with ID #" << recvMessage->getMessageId() << " instead of #" << TestMessage::MESSAGE_ID << std::endl;
	}


	std::cout << "Press RETURN to exit...";
	std::cin.get();

	return EXIT_SUCCESS;
}
