#if defined(_MSC_VER)
#pragma warning( disable : 4996 )
#endif

#include "Buffer_tests.h"

#include "catch.hpp"

#include <cstddef>
#include <cstdlib>
#include <cstring>

#include <limits>
#include <stdexcept>

#include "transporter/Buffer.h"

void fillBuffer(transporter::data::Buffer &buffer)
{
	for (std::size_t i = 0; i < buffer.getSize(); ++i)
	{
		*const_cast<char*>(buffer.getRawBuffer() + i) = rand() % 256;
	}
}

TEST_CASE("Constructors")
{
	SECTION("Default-constructed buffer has a size of 0 and a nullptr buffer")
	{
		transporter::data::Buffer buffer{};

		REQUIRE(buffer.getSize() == 0);
		REQUIRE(buffer.getRawBuffer() == nullptr);
	}

	SECTION("Constructor with initial size")
	{
		transporter::data::Buffer buffer{400};

		REQUIRE(buffer.getSize() == 400);
		REQUIRE(buffer.getRawBuffer() != nullptr);
	}

	SECTION("Copy-constructor")
	{
		transporter::data::Buffer buffer{ 400 };
		transporter::data::Buffer bufferCopy{};

		fillBuffer(buffer);
		
		bufferCopy = transporter::data::Buffer{ buffer };

		REQUIRE(bufferCopy.getSize() == 400);
		REQUIRE(bufferCopy.getRawBuffer() != nullptr);
		REQUIRE((buffer == bufferCopy) == true);
	}

	SECTION("Move-constructor")
	{
		transporter::data::Buffer initialBuffer{ 400 };
		const char *initialRawBuffer = initialBuffer.getRawBuffer();
		transporter::data::Buffer buffer{ std::move(initialBuffer) };

		REQUIRE(initialBuffer.getSize() == 0);
		REQUIRE(initialBuffer.getRawBuffer() == nullptr);
		REQUIRE(buffer.getSize() == 400);
		REQUIRE(buffer.getRawBuffer() == initialRawBuffer);
	}

	SECTION("Constructor with const char* and initial size")
	{
		const char *initialBuffer = "Hello World";
		std::size_t initialBufferLen = std::strlen(initialBuffer);
		transporter::data::Buffer buffer{ initialBuffer, initialBufferLen };

		REQUIRE(buffer.getSize() == initialBufferLen);
		REQUIRE(std::strncmp(initialBuffer, buffer.getRawBuffer(), initialBufferLen) == 0);
	}

	SECTION("Constructor with std::unique_ptr<char[]>&& and initial size")
	{
		const char *initialBufferHelper = "Hello World";
		std::size_t initialBufferLen = std::strlen(initialBufferHelper);
		std::unique_ptr<char[]> initialBuffer{ new char[initialBufferLen] };
		transporter::data::Buffer buffer{};

		std::copy(initialBufferHelper, initialBufferHelper + initialBufferLen, initialBuffer.get());
		buffer = transporter::data::Buffer{ std::move(initialBuffer), initialBufferLen };

		REQUIRE(buffer.getSize() == initialBufferLen);
		REQUIRE(std::strncmp(initialBufferHelper, buffer.getRawBuffer(), initialBufferLen) == 0);
	}
}

TEST_CASE("Subscript operators")
{
	transporter::data::Buffer buffer{ 100 };

	fillBuffer(buffer);

	SECTION("Subscript Getter operator")
	{
		REQUIRE(*(buffer.getRawBuffer() + 55) == buffer[55]);
	}

	SECTION("Subscript Setter operator")
	{
		char old = buffer[55];

		buffer[55] += 1;
		REQUIRE(buffer[55] == old + 1);
	}
}

TEST_CASE("Equality operator")
{
	transporter::data::Buffer buffer1{ 100 };
	transporter::data::Buffer buffer2{};

	fillBuffer(buffer1);
	buffer2 = buffer1;

	REQUIRE((buffer1 == buffer2) == true);
}

TEST_CASE("Addition operator")
{
	transporter::data::Buffer buffer1{ 100 };
	transporter::data::Buffer buffer2{ 100 };
	transporter::data::Buffer bufferResult{};

	fillBuffer(buffer1);
	fillBuffer(buffer2);

	bufferResult = buffer1 + buffer2;
	REQUIRE(std::equal(buffer1.getRawBuffer(), buffer1.getRawBuffer() + 100, bufferResult.getRawBuffer()) == true);
	REQUIRE(std::equal(buffer2.getRawBuffer(), buffer2.getRawBuffer() + 100, bufferResult.getRawBuffer() + 100) == true);

	bufferResult = buffer2 + buffer1;
	REQUIRE(std::equal(buffer2.getRawBuffer(), buffer2.getRawBuffer() + 100, bufferResult.getRawBuffer()) == true);
	REQUIRE(std::equal(buffer1.getRawBuffer(), buffer1.getRawBuffer() + 100, bufferResult.getRawBuffer() + 100) == true);
}

TEST_CASE("Assignment operators")
{
	transporter::data::Buffer buffer1{ 100 };
	transporter::data::Buffer buffer2{};

	fillBuffer(buffer1);

	SECTION("Copy assignment")
	{
		buffer2 = buffer1;

		REQUIRE((buffer1 == buffer2) == true);
	}

	SECTION("Addition assignment")
	{
		transporter::data::Buffer buffer3{ 25 };
		transporter::data::Buffer bufferCmp{};

		fillBuffer(buffer3);
		bufferCmp = buffer1 + buffer3;

		buffer2 = buffer1;
		buffer2 += buffer3;

		REQUIRE((buffer2 == bufferCmp) == true);
	}

	SECTION("Move assignment")
	{
		transporter::data::Buffer buffer1Copy{ buffer1 };
		buffer2 = std::move(buffer1);

		REQUIRE((buffer2 == buffer1Copy) == true);
		REQUIRE(buffer1.getSize() == 0);
		REQUIRE(buffer1.getRawBuffer() == nullptr);
	}
}

TEST_CASE("Extending buffer")
{
	transporter::data::Buffer buffer{};

	SECTION("Extending buffer increases its size")
	{
		buffer.extend(100);
		REQUIRE(buffer.getSize() == 100);

		buffer.extend(50);
		REQUIRE(buffer.getSize() == 150);
	}

	SECTION("Extending buffer tricky-cases")
	{
		buffer.extend(0);
		REQUIRE(buffer.getSize() == 0);

		REQUIRE_THROWS_AS(buffer.extend(std::numeric_limits<std::size_t>::max()), std::bad_alloc);
		REQUIRE(buffer.getSize() == 0);

		buffer.extend(1);
		REQUIRE_THROWS_AS(buffer.extend(std::numeric_limits<std::size_t>::max()), std::overflow_error);
		REQUIRE(buffer.getSize() == 1);
	}

	SECTION("Extending buffer keeps original contents")
	{
		transporter::data::Buffer bufferCopy{};

		buffer.extend(100);
		fillBuffer(buffer);

		bufferCopy = buffer;

		buffer.extend(50);
		REQUIRE(std::equal(buffer.getRawBuffer(), buffer.getRawBuffer() + 100, bufferCopy.getRawBuffer()) == true);
	}
}

TEST_CASE("Shrinking buffer")
{
	transporter::data::Buffer buffer{};

	SECTION("Shrinking buffer decreases its size")
	{
		buffer.extend(100);

		buffer.shrinkBeginning(10);
		REQUIRE(buffer.getSize() == 90);

		buffer.shrinkEnd(20);
		REQUIRE(buffer.getSize() == 70);
	}

	SECTION("Shrinking buffer tricky-cases")
	{
		buffer.extend(100);

		buffer.shrinkBeginning(0);
		REQUIRE(buffer.getSize() == 100);

		buffer.shrinkEnd(0);
		REQUIRE(buffer.getSize() == 100);

		REQUIRE_THROWS_AS(buffer.shrinkBeginning(200), std::length_error);
		REQUIRE(buffer.getSize() == 100);

		REQUIRE_THROWS_AS(buffer.shrinkEnd(200), std::length_error);
		REQUIRE(buffer.getSize() == 100);
	}

	SECTION("Shrinking buffer keeps remaining contents")
	{
		transporter::data::Buffer bufferCopy{};

		buffer.extend(100);
		fillBuffer(buffer);

		bufferCopy = buffer;

		buffer.shrinkBeginning(25);
		REQUIRE(std::equal(buffer.getRawBuffer(), buffer.getRawBuffer() + 75, bufferCopy.getRawBuffer() + 25) == true);

		buffer.shrinkEnd(25);
		REQUIRE(std::equal(buffer.getRawBuffer(), buffer.getRawBuffer() + 50, bufferCopy.getRawBuffer() + 25) == true);
	}
}

TEST_CASE("Reverse buffer")
{
	transporter::data::Buffer buffer{ 5 };

	for (int i = 0; i < 5; ++i)
	{
		buffer[i] = i;
	}

	buffer.reverse();

	REQUIRE((buffer[0] == 4 && buffer[1] == 3 && buffer[2] == 2 && buffer[3] == 1 && buffer[4] == 0) == true);
}


TEST_CASE("Release buffer")
{
	transporter::data::Buffer buffer{ 5 };
	const char *rawBuffer = buffer.getRawBuffer();
	char *releasedBuffer = buffer.release();

	REQUIRE(rawBuffer == releasedBuffer);
	REQUIRE(buffer.getSize() == 0);
	REQUIRE(buffer.getRawBuffer() == nullptr);
}

TEST_CASE("Slicing buffer")
{
	transporter::data::Buffer buffer{ 100 };
	transporter::data::Buffer bufferCopy{};
	transporter::data::BufferPtr bufferSlice1{};
	transporter::data::BufferPtr bufferSlice2{};

	fillBuffer(buffer);
	bufferCopy = buffer;

	bufferSlice1 = buffer.slice(0, 10);
	REQUIRE((*bufferSlice1 + buffer == bufferCopy) == true);

	bufferSlice2 = buffer.slice(80, 10);
	REQUIRE((*bufferSlice1 + buffer + *bufferSlice2 == bufferCopy) == true);

	buffer = bufferCopy;
	bufferSlice1 = buffer.slice(10, 10);
	REQUIRE(std::equal(buffer.getRawBuffer(), buffer.getRawBuffer() + 10, bufferCopy.getRawBuffer()) == true);
	REQUIRE(std::equal(bufferSlice1->getRawBuffer(), bufferSlice1->getRawBuffer() + 10, bufferCopy.getRawBuffer() + 10) == true);
	REQUIRE(std::equal(buffer.getRawBuffer() + 10, buffer.getRawBuffer() + 90, bufferCopy.getRawBuffer() + 20) == true);

	// TODO: transporter::data::Buffer::slice() exceptions
	// TODO: transporter::data::Buffer::getSlice()
}

TEST_CASE("Inserting in buffer")
{
	transporter::data::Buffer buffer1{ 100 };
	transporter::data::Buffer buffer1Copy{};
	transporter::data::Buffer buffer2{ 100 };

	fillBuffer(buffer1);
	fillBuffer(buffer2);

	buffer1Copy = buffer1;

	SECTION("Beginning/End Insertion")
	{
		buffer1.insert(buffer2, 0);
		REQUIRE((buffer1 == buffer2 + buffer1Copy) == true);

		buffer1.insert(buffer2, buffer1.getSize());
		REQUIRE((buffer1 == buffer2 + buffer1Copy + buffer2) == true);
	}

	SECTION("Middle Insertion")
	{
		buffer1.insert(buffer2, 40);
		REQUIRE(std::equal(buffer1.getRawBuffer(), buffer1.getRawBuffer() + 40, buffer1Copy.getRawBuffer()) == true);
		REQUIRE(std::equal(buffer1.getRawBuffer() + 40, buffer1.getRawBuffer() + 140, buffer2.getRawBuffer()) == true);
		REQUIRE(std::equal(buffer1.getRawBuffer() + 140, buffer1.getRawBuffer() + 200, buffer1Copy.getRawBuffer() + 40) == true);
	}

	SECTION("Inserting in buffer tricky-cases")
	{
		REQUIRE_THROWS_AS(buffer1.insert(buffer2, 101), std::invalid_argument);
	}
}
