#include <catch/catch.hpp>

#include <limits>
#include <memory>

#include "transporter/Buffer.h"
#include "transporter/MemoryStream.h"
#include "transporter/NetworkStream.h"

TEST_CASE("READs/WRITEs")
{
	transporter::data::io::MemoryStream ms{};
	transporter::network::io::NetworkStream ns{ ms };
	const char *bufferStr = "Hello, Buffer!";
	std::size_t bufferStrLen = std::strlen(bufferStr);
	transporter::data::Buffer buffer{ bufferStr, bufferStrLen };
	std::unique_ptr<transporter::data::Buffer> readBuffer{};

	REQUIRE(ns.writeBytes(buffer) == buffer.getSize());
	REQUIRE(ms.getPosition() == buffer.getSize());

	ns.writeInt8(0);
	ns.writeInt16(0);
	ns.writeInt32(0);
	ns.writeInt64(0);

	ns.writeInt8(1);
	ns.writeInt16(2);
	ns.writeInt32(3);
	ns.writeInt64(4);

	ns.writeInt8(-1);
	ns.writeInt16(-2);
	ns.writeInt32(-3);
	ns.writeInt64(-4);

	ns.writeInt8(std::numeric_limits<std::int8_t>::max());
	ns.writeInt16(std::numeric_limits<std::int16_t>::max());
	ns.writeInt32(std::numeric_limits<std::int32_t>::max());
	ns.writeInt64(std::numeric_limits<std::int64_t>::max());

	ns.writeInt8(std::numeric_limits<std::int8_t>::min());
	ns.writeInt16(std::numeric_limits<std::int16_t>::min());
	ns.writeInt32(std::numeric_limits<std::int32_t>::min());
	ns.writeInt64(std::numeric_limits<std::int64_t>::min());

	ns.writeUInt8(std::numeric_limits<std::uint8_t>::max());
	ns.writeUInt16(std::numeric_limits<std::uint16_t>::max());
	ns.writeUInt32(std::numeric_limits<std::uint32_t>::max());
	ns.writeUInt64(std::numeric_limits<std::uint64_t>::max());

	ns.writeUInt8(std::numeric_limits<std::uint8_t>::min());
	ns.writeUInt16(std::numeric_limits<std::uint16_t>::min());
	ns.writeUInt32(std::numeric_limits<std::uint32_t>::min());
	ns.writeUInt64(std::numeric_limits<std::uint64_t>::min());

	ns.writeFloat(5.00f);
	ns.writeDouble(6.00);
	ns.writeString("Hello, World!");

	ms.setPosition(0);

	readBuffer = ns.readBytes(buffer.getSize());
	REQUIRE(std::strncmp(readBuffer->getRawBuffer(), buffer.getRawBuffer(), bufferStrLen) == 0);

	REQUIRE(ns.readInt8() == 0);
	REQUIRE(ns.readInt16() == 0);
	REQUIRE(ns.readInt32() == 0);
	REQUIRE(ns.readInt64() == 0);

	REQUIRE(ns.readInt8() == 1);
	REQUIRE(ns.readInt16() == 2);
	REQUIRE(ns.readInt32() == 3);
	REQUIRE(ns.readInt64() == 4);

	REQUIRE(ns.readInt8() == -1);
	REQUIRE(ns.readInt16() == -2);
	REQUIRE(ns.readInt32() == -3);
	REQUIRE(ns.readInt64() == -4);

	REQUIRE(ns.readInt8() == std::numeric_limits<std::int8_t>::max());
	REQUIRE(ns.readInt16() == std::numeric_limits<std::int16_t>::max());
	REQUIRE(ns.readInt32() == std::numeric_limits<std::int32_t>::max());
	REQUIRE(ns.readInt64() == std::numeric_limits<std::int64_t>::max());

	REQUIRE(ns.readInt8() == std::numeric_limits<std::int8_t>::min());
	REQUIRE(ns.readInt16() == std::numeric_limits<std::int16_t>::min());
	REQUIRE(ns.readInt32() == std::numeric_limits<std::int32_t>::min());
	REQUIRE(ns.readInt64() == std::numeric_limits<std::int64_t>::min());

	REQUIRE(ns.readUInt8() == std::numeric_limits<std::uint8_t>::max());
	REQUIRE(ns.readUInt16() == std::numeric_limits<std::uint16_t>::max());
	REQUIRE(ns.readUInt32() == std::numeric_limits<std::uint32_t>::max());
	REQUIRE(ns.readUInt64() == std::numeric_limits<std::uint64_t>::max());

	REQUIRE(ns.readUInt8() == std::numeric_limits<std::uint8_t>::min());
	REQUIRE(ns.readUInt16() == std::numeric_limits<std::uint16_t>::min());
	REQUIRE(ns.readUInt32() == std::numeric_limits<std::uint32_t>::min());
	REQUIRE(ns.readUInt64() == std::numeric_limits<std::uint64_t>::min());
	
	REQUIRE(ns.readFloat() == 5.00f);
	REQUIRE(ns.readDouble() == 6.00);
	REQUIRE(ns.readString() == std::string("Hello, World!"));
}

TEST_CASE("Transactions")
{
	transporter::data::io::MemoryStream ms{};
	transporter::network::io::NetworkStream ns{ ms };
	const char *bufferStr = "Hello, Buffer!";
	std::size_t bufferStrLen = std::strlen(bufferStr);
	transporter::data::Buffer buffer{ bufferStr, bufferStrLen };
	std::unique_ptr<transporter::data::Buffer> readBuffer{};

	SECTION("Successful transaction")
	{
		ns.writeInt32(70);
		ms.setPosition(0);

		REQUIRE(ns.startReadTransaction() == true);
		REQUIRE(ns.readInt32() == 70);
		REQUIRE(ns.commitReadTransaction() == true);
	}

	SECTION("Bad transaction & rollback")
	{
		ns.writeInt32(70);
		ms.setPosition(0);

		REQUIRE(ns.startReadTransaction() == true);
		ns.readInt32();
		ns.readInt32();
		REQUIRE(ns.commitReadTransaction() == false);
		REQUIRE(ns.rollbackReadTransaction() == true);
		REQUIRE(ns.startReadTransaction() == true);
		REQUIRE(ns.readInt32() == 70);
		REQUIRE(ns.commitReadTransaction() == true);
	}

	SECTION("Tricky cases/errors")
	{
		REQUIRE(ns.rollbackReadTransaction() == false);
		REQUIRE(ns.commitReadTransaction() == false);
		REQUIRE(ns.startReadTransaction() == true);
		REQUIRE(ns.startReadTransaction() == false);
	}
}
