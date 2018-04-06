#pragma once

#include "Transporter.h"

#include <cstdint>

#include <memory>
#include <stdexcept>

namespace transporter
{
	namespace data
	{
		class TRANSPORTER_DLL Buffer
		{
		public:
			Buffer() noexcept;
			Buffer(std::size_t size) throw(std::bad_alloc);
			Buffer(const transporter::data::Buffer &other) throw(std::bad_alloc, std::overflow_error);
			Buffer(transporter::data::Buffer &&other) noexcept;
			Buffer(const char *buffer, std::size_t bufferSize) throw(std::bad_alloc);
			Buffer(std::unique_ptr<char[]> &&buffer, std::size_t bufferSize) noexcept;
			~Buffer() noexcept = default;

			transporter::data::Buffer &operator=(const transporter::data::Buffer &rhs) throw(std::bad_alloc, std::overflow_error);
			transporter::data::Buffer &operator=(transporter::data::Buffer &&rhs) noexcept;

			transporter::data::Buffer operator+(const transporter::data::Buffer &rhs) throw(std::bad_alloc, std::overflow_error);
			transporter::data::Buffer &operator+=(const transporter::data::Buffer &rhs) throw(std::bad_alloc, std::overflow_error);

			bool operator==(const transporter::data::Buffer &rhs) noexcept;

			char operator[](std::size_t index) const throw(std::out_of_range);
			char &operator[](std::size_t index) throw(std::out_of_range);

			const char *getRawBuffer() const noexcept;
			std::size_t getSize() const noexcept;
			std::unique_ptr<transporter::data::Buffer> getSlice(std::size_t beginning, std::size_t length) const throw(std::bad_alloc, std::invalid_argument);

			char *release() noexcept;
			void extend(std::size_t increase) throw(std::bad_alloc, std::overflow_error);
			void insert(const transporter::data::Buffer &buffer, std::size_t at) throw(std::bad_alloc, std::invalid_argument, std::overflow_error);
			void shrinkEnd(std::size_t decrease) throw(std::bad_alloc, std::length_error);
			void shrinkBeginning(std::size_t decrease) throw(std::bad_alloc, std::length_error);
			void reverse() const noexcept;
			std::unique_ptr<transporter::data::Buffer> slice(std::size_t beginning, std::size_t length) throw(std::bad_alloc, std::invalid_argument);


		private:
			void concatenate(const transporter::data::Buffer &buffer) throw(std::bad_alloc, std::overflow_error);
			void reset() noexcept;

			std::unique_ptr<char[]> m_buffer;
			std::size_t m_bufferSize;
		};
	}
}
