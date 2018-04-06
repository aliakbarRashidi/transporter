#include <catch/catch.hpp>

#include <memory>

#include "transporter/Buffer.h"
#include "transporter/MemoryStream.h"

#include "Buffer_tests.h"

TEST_CASE("Constructor")
{
	transporter::data::io::MemoryStream ms{};

	REQUIRE(ms.getLength() == 0);
	REQUIRE(ms.getPosition() == 0);
}

TEST_CASE("Writing")
{
	transporter::data::io::MemoryStream ms{};
	transporter::data::Buffer buffer{ 100 };

	ms.writeBytes(buffer);
	REQUIRE(ms.getLength() == 100);
	REQUIRE(ms.getPosition() == 100);
}

TEST_CASE("Setting stream position")
{
	transporter::data::io::MemoryStream ms{};
	transporter::data::Buffer buffer{ 100 };

	ms.writeBytes(buffer);
	REQUIRE(ms.getPosition() == 100);

	ms.setPosition(50);
	REQUIRE(ms.getPosition() == 50);

	REQUIRE_THROWS_AS(ms.setPosition(300), std::invalid_argument);
}

TEST_CASE("Reading")
{
	transporter::data::io::MemoryStream ms{};
	transporter::data::Buffer buffer{ 100 };
	std::unique_ptr<transporter::data::Buffer> readBuffer1{};
	std::unique_ptr<transporter::data::Buffer> readBuffer2{};

	fillBuffer(buffer);

	ms.writeBytes(buffer);
	ms.setPosition(0);
	readBuffer1 = ms.readBytes(50);
	readBuffer2 = ms.readBytes(50);
	REQUIRE((*readBuffer1 + *readBuffer2 == buffer) == true);

	REQUIRE(ms.readBytes(1) == nullptr);
}
