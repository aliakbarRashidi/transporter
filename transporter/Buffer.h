/*
* MIT License
*
* Copyright (c) 2018 Guillaume Truchot - guillaume.truchot@outlook.com
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#pragma once

#include "Transporter.h"

#include <cstdint>

#include <memory>
#include <stdexcept>

namespace transporter
{
	namespace data
	{
		class Buffer;
		typedef std::unique_ptr<transporter::data::Buffer> BufferPtr;

		/**
		* @brief Provide high-level operations on an array of bytes
		*/
		class TRANSPORTER_DLL Buffer
		{
		public:
			/**
			* @brief Initialize an empty buffer
			*/
			Buffer() noexcept;

			/**
			* @brief Initialize a buffer of size \p size
			*/
			Buffer(std::size_t size);

			/**
			* @brief Initialize a buffer by copying \p other into this buffer
			*/
			Buffer(const transporter::data::Buffer &other);

			/**
			* @brief Initialize a buffer by moving \p other into this buffer
			*/
			Buffer(transporter::data::Buffer &&other) noexcept;

			/**
			* @brief Initialize a buffer by copying \p bufferSize bytes from \p buffer into this buffer
			*/
			Buffer(const char *buffer, std::size_t bufferSize);

			/**
			* @brief Initialize a buffer by moving \p buffer into its internal buffer and setting \p bufferSize to its internal buffer size
			*/
			Buffer(std::unique_ptr<char[]> &&buffer, std::size_t bufferSize) noexcept;

			~Buffer() noexcept = default;


			/**
			* @brief Set this buffer to a copy of \p rhs
			*
			* @return Reference to this buffer
			*/
			transporter::data::Buffer &operator=(const transporter::data::Buffer &rhs);

			/**
			* @brief Move \p rhs to this buffer
			*
			* @return Reference to this buffer
			*/
			transporter::data::Buffer &operator=(transporter::data::Buffer &&rhs) noexcept;


			/**
			* @brief Create a copy of this buffer, concatenate \p rhs to it and return the copy
			*
			* @return Newly-constructed buffer result of the concatenation of this buffer and \p rhs
			*/
			transporter::data::Buffer operator+(const transporter::data::Buffer &rhs);

			/**
			* @brief Concatenate \p rhs to this buffer
			*
			* @return Reference to this buffer
			*/
			transporter::data::Buffer &operator+=(const transporter::data::Buffer &rhs);


			/**
			* @brief Check if the contents of this buffer are the same as \p rhs
			*
			* @return \c true if the contents are the same, \c false otherwise
			*/
			bool operator==(const transporter::data::Buffer &rhs) noexcept;


			/**
			* @brief Return the byte at index \p index of this buffer (accessor)
			*
			* @return Byte value at index \p index of this buffer
			*/
			char operator[](std::size_t index) const;

			/**
			* @brief Return a reference to the byte at index \p index of this buffer (mutator)
			*
			* @return Reference to the byte at index \p index of this buffer
			*/
			char &operator[](std::size_t index);


			/**
			* @brief Get the raw pointer to the (constant) internal buffer
			*
			* @return Raw pointer to (constant) internal buffer
			*/
			const char *getRawBuffer() const noexcept;

			/**
			* @brief Get the raw pointer to the internal buffer
			*
			* @return Raw pointer to internal buffer
			*/
			char *getRawBuffer() noexcept;

			/**
			* @brief Get the size of this buffer
			*
			* @return Size of this buffer
			*/
			std::size_t getSize() const noexcept;

			/**
			* @brief Get a slice of this buffer of length \p length starting at index \p beginning
			*
			* @return Pointer to newly-constructed transporter::data::Buffer containing the slice
			*/
			transporter::data::BufferPtr getSlice(std::size_t beginning, std::size_t length) const;


			/**
			* @brief Release the internal buffer pointer and return it
			*
			* @return Raw pointer to the released internal buffer
			*/
			char *release() noexcept;

			/**
			* @brief Extend this buffer by \p increase bytes
			*/
			void extend(std::size_t increase);

			/**
			* @brief Insert \p buffer at index \p at in this buffer
			*/
			void insert(const transporter::data::Buffer &buffer, std::size_t at);

			/**
			* @brief Shrink this buffer by the end of \p decrease bytes
			*/
			void shrinkEnd(std::size_t decrease);

			/**
			* @brief Shrink this buffer by the beginning of \p decrease bytes
			*/
			void shrinkBeginning(std::size_t decrease);

			/**
			* @brief Reverse this buffer
			*/
			void reverse() const noexcept;

			/**
			* @brief Slice this buffer by length \p length starting at index \p beginning and return a pointer to the newly-constructed slice
			*
			* @return Pointer to the newly-constructed slice
			*/
			transporter::data::BufferPtr slice(std::size_t beginning, std::size_t length);


		private:
			void concatenate(const transporter::data::Buffer &buffer);
			void reset() noexcept;

			// NOTE: warning C4251 "class X needs to have dll-interface to be used by clients of class..." can be safely disabled as it is private and won't be used by any client
#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4251)
#endif
			std::unique_ptr<char[]> m_buffer;
#if defined(_MSC_VER)
#pragma warning( pop )
#endif

			std::size_t m_bufferSize;
		};
	}
}
