# transporter

![Build Status](https://jenkins.guillaume.tech/buildStatus/icon?job=transporter-pipeline)

_transporter_ is a cross-platform, modern C++11 library facilitating the development of network applications by providing an implementation of **network streams** and **network messages**.

* **Network streams** add an **abstraction layer** _on top_ of your ordinary network library that takes care of the processing of the data you send and receive
* **Network messages** act as a **wrapper** around your data so you can manipulate them easily

**Note:** transporter does **NOT** provide an implementation of **sockets**. It is to be used **_jointly_** with such a library. See [below](#interfacing-transporter-with-network-libraries) to use transporter with your current network library.


## Why the need for transporter?

Network libraries provide you with an easy way of **sending** and **receiving** **raw bytes**. However, the **processing** of these bytes is left to you.

There are common problematics that need to be addressed when doing network programming:
* **Identification**: identify the meaning of a sequence of bytes (e.g. is it a command, or is it bytes part of a file transfer?)
* **Sizing**: determine the size of a sequence of bytes (e.g. I know I'm receiving a string of characters, but how long is it?)
* **Buffering**: buffer data until there is enough to make sense of it (e.g. I know I want to read 4 bytes, but there are only 2 available right now)
* **Endianness**: machines/OSes often have different endiannesses (e.g. Linux and Windows often have a different endianness)

These problematics are cumbersome and error-prone. _transporter_ takes care of them all!


## Interfacing _transporter_ with network libraries

To use transporter, you need to use an **adapter** to interface it with your current network library.

Currently, adapters exist for:
* **Boost.Asio**
	* [TransporterAdapterBoostAsioTcpSocket](https://guitek.github.io/transporter/classtransporter_1_1adapters_1_1boost_1_1asio_1_1_transporter_adapter_boost_asio_tcp_socket.html) for `boost::asio::ip::tcp::socket`
	* [TransporterAdapterBoostAsioUdpSocket](https://guitek.github.io/transporter/classtransporter_1_1adapters_1_1boost_1_1asio_1_1_transporter_adapter_boost_asio_udp_socket.html) for `boost::asio::ip::udp::socket`
* **Qt**
	* [TransporterAdapterQtQAbstractSocket](https://guitek.github.io/transporter/classtransporter_1_1adapters_1_1qt_1_1_transporter_adapter_qt_q_abstract_socket.html) for `QTcpSocket`, `QSslSocket`, `QSctpSocket` and `QUdpSocket`
* **SFML**
	* [TransporterAdapterSfmlTcpSocket](https://guitek.github.io/transporter/classtransporter_1_1adapters_1_1sfml_1_1_transporter_adapter_sfml_tcp_socket.html) for `sf::TcpSocket`
	* [TransporterAdapterSfmlUdpSocket](https://guitek.github.io/transporter/classtransporter_1_1adapters_1_1sfml_1_1_transporter_adapter_sfml_udp_socket.html) for `sf::UdpSocket`

If the library you're using is listed above, you can immediately switch to [Quick start: sending and receiving messages](#quick-start-sending-and-receiving-messages).

Otherwise, if your library isn't listed above, you can easily write one in less than 10 lines of code. See [Writing an adapter](#writing-an-adapter) below. Don't forget to make a pull request so we can merge it into the repository and everyone can use it!

**Note:** adapters are header-only and do **NOT** require to recompile transporter.


### Writing an adapter

The core of transporter is the [NetworkStream class](https://guitek.github.io/transporter/classtransporter_1_1network_1_1io_1_1_network_stream.html) which can send and receive [network messages](https://guitek.github.io/transporter/classtransporter_1_1network_1_1messages_1_1_i_network_message.html).

All `NetworkStream` needs is an [IByteStream](https://guitek.github.io/transporter/classtransporter_1_1data_1_1io_1_1_i_byte_stream.html) to be able to write and read the messages. All you have to do is to create a class inheriting from the `IByteStream` interface and implement the methods [IByteStream::readBytes()](https://guitek.github.io/transporter/classtransporter_1_1data_1_1io_1_1_i_byte_input.html#a1eedf2ac2bb60a019fce02438a36c0fc) and [IByteStream::writeBytes()](https://guitek.github.io/transporter/classtransporter_1_1data_1_1io_1_1_i_byte_output.html#a3f2a5a3a2cee96610c1b257d2f71ce14).

You can have a look at the [Boost.Asio TCP adapter implementation](https://github.com/GuiTeK/transporter/blob/master/transporter/TransporterAdapterBoostAsioTcpSocket.h) to see how straightforward it is. You can copy/paste it as a base for your own adapter and modify it as needed.


## Quick start: sending and receiving messages

One complete and fully-commented example is available in the [examples/ folder](https://github.com/GuiTeK/transporter/tree/master/examples) of the repository. It features a client sending a test message to a local server using Boost.Asio TCP sockets.

### Creating network messages

Before you can send or receive any message, you must create it. A network message is a class:
* Inheriting from [INetworkMessage](https://guitek.github.io/transporter/classtransporter_1_1network_1_1messages_1_1_i_network_message.html)
* Having attributes: these are your data (e.g. a `std::string m_nickname`, a `int m_level`) 
* Implementing the [INetworkMessage::serialize()](https://guitek.github.io/transporter/classtransporter_1_1network_1_1messages_1_1_i_network_message.html#af40a45bf71f46d9bc192b7a6f85056d1) and [INetworkMessage::deserialize()](https://guitek.github.io/transporter/classtransporter_1_1network_1_1messages_1_1_i_network_message.html#aed797d78daaafb7310db1435a4a15753) methods correctly so the attributes are correctly sent/received in the right order

You can take a look at the `TestMessage` implemented in the unit tests project of transporter:
* [TestMessage.h](https://github.com/GuiTeK/transporter/blob/master/transporter.tests/TestMessage.h)
* [TestMessage.cpp](https://github.com/GuiTeK/transporter/blob/master/transporter.tests/TestMessage.cpp)

It is a very simple message which has 2 attributes, a string of characters `m_str1` and an integer `m_i1`. You can copy/paste it and use it as a base for your own messages.


### Sending and receiving

Here is a detailed step-by-step guide to send your first message:
1. Create a socket using your network library
2. Instanciate the correct adapter class and pass it the socket
3. Instanciate a network stream and pass it the adapter
4. Instanciate the message you want to send
5. Connect the socket to the remote endpoint
6. Send your message using the network stream

Here is an example with a _QTcpSocket_ (Qt library):
```cpp
QTcpSocket *myTcpSocket = new QTcpSocket();	// Step 1: create a socket
transporter::adapters::qt::TransporterAdapterQtQAbstractSocket qtTcpAdapter(myTcpSocket);	// Step 2: instanciate the correct adapter
transporter::network::io::NetworkStream myNetworkStream(qtTcpAdapter);	// Step 3: instanciate a network stream using the adapter
TestMessage myTestMessage;	// Step 4: instanciate your network message

myTcpSocket->connectToHost("127.0.0.1", 25);	// Step 5: connect the socket

myNetworkStream.send(myTestMessage);	// Step 6: send the message
```


## C++ Network Libraries implementing sockets

As stated above, transporter doesn't implement sockets. To use it, you need a network library. Here are some well known and powerful libraries you can pick:
* [Boost.Asio](https://www.boost.org/doc/libs/1_67_0/doc/html/boost_asio.html)
* [Qt](https://www.qt.io/)'s network module
* [SFML](https://www.sfml-dev.org/)'s network module

**You can use _any_ network library**: you just need to write an adapter for it if it doesn't exist yet.


## Contributing

You can contribute to the project by:
* Writing new [adapters](#interfacing-transporter-with-network-libraries) for transporter
* Creating an [issue](https://github.com/GuiTeK/transporter/issues) to signal a bug or to suggest a feature
* Fixing bugs or implementing new features (see [Creating a pull request - GitHub](https://help.github.com/articles/creating-a-pull-request/))
* Writing documentation

Make sure you read [CONTRIBUTING.md](https://github.com/GuiTeK/transporter/blob/master/CONTRIBUTING.md) before writing code.

Any contribution is very much appreciated!
