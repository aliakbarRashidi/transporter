#pragma once

#include <type_traits>

#include "Transporter.h"

#include "IBytesStream.h"
#include "IDataInput.h"
#include "IDataOutput.h"

namespace transporter
{
	namespace network
	{
		namespace io
		{
			class TRANSPORTER_DLL NetworkStream : public transporter::data::io::IDataInput, public transporter::data::io::IDataOutput
			{
			public:
				NetworkStream(transporter::data::io::IBytesStream &stream) noexcept;
				~NetworkStream() noexcept = default;

				bool startReadTransaction() noexcept;
				bool commitReadTransaction() noexcept;
				bool rollbackReadTransaction() noexcept;

				virtual std::unique_ptr<transporter::data::Buffer> readBytes(std::size_t count) noexcept override;
				virtual ssize_t writeBytes(const transporter::data::Buffer &buffer) noexcept override;

				virtual std::int8_t readInt8() noexcept override;
				virtual std::int16_t readInt16() noexcept override;
				virtual std::int32_t readInt32() noexcept override;
				virtual std::int64_t readInt64() noexcept override;
				virtual std::uint8_t readUInt8() noexcept override;
				virtual std::uint16_t readUInt16() noexcept override;
				virtual std::uint32_t readUInt32() noexcept override;
				virtual std::uint64_t readUInt64() noexcept override;
				virtual float readFloat() noexcept override;
				virtual double readDouble() noexcept override;
				virtual std::string readString() noexcept override;

				virtual void writeInt8(std::int8_t value) noexcept override;
				virtual void writeInt16(std::int16_t value) noexcept override;
				virtual void writeInt32(std::int32_t value) noexcept override;
				virtual void writeInt64(std::int64_t value) noexcept override;
				virtual void writeUInt8(std::uint8_t value) noexcept override;
				virtual void writeUInt16(std::uint16_t value) noexcept override;
				virtual void writeUInt32(std::uint32_t value) noexcept override;
				virtual void writeUInt64(std::uint64_t value) noexcept override;
				virtual void writeFloat(float value) noexcept override;
				virtual void writeDouble(double value) noexcept override;
				virtual void writeString(const std::string &value) noexcept override;

				template<typename T>
				T readData() noexcept
				{
					std::size_t dataSize = sizeof(T);
					std::unique_ptr<data::Buffer> buffer = this->readBytes(dataSize);
					T data{};

					if (buffer && buffer->getSize() == dataSize)
					{
						if (dataSize > 1 && !isBigEndian())
						{
							buffer->reverse();
						}
						
						data = *reinterpret_cast<const T*>(buffer->getRawBuffer());
					}

					return data;
				}

				template<typename T>
				void writeData(const T &value) noexcept
				{
					std::size_t dataSize = sizeof(T);

					try
					{
						data::Buffer buffer{ reinterpret_cast<const char*>(&value), dataSize };

						if (dataSize > 1 && !isBigEndian())
						{
							buffer.reverse();
						}

						this->writeBytes(buffer);
					}

					catch (...)
					{
						// TODO: what to do in case of failure?
					}
				}


			private:
				template<typename T>
				inline void assertIntType()
				{
					static_assert(std::is_same<T, std::int8_t>::value ||
						std::is_same<T, std::uint8_t>::value ||
						std::is_same<T, std::int16_t>::value ||
						std::is_same<T, std::uint16_t>::value ||
						std::is_same<T, std::int32_t>::value ||
						std::is_same<T, std::uint32_t>::value ||
						std::is_same<T, std::int64_t>::value ||
						std::is_same<T, std::uint64_t>::value, "readInt<T>()/writeInt<T>() must be used either with std::int8_t, std::int16_t, std::int32_t, std::int64_t or their unsigned version");
				}

				template<typename T>
				T readInt() noexcept
				{
					T value = 0;
					bool moreData = false;
					std::size_t bitsHandled = 0;

					assertIntType<T>();

					do
					{
						std::uint64_t currentByte = this->readData<std::uint8_t>();

						value = value | ((currentByte & INT_CHUNK_MASK) << bitsHandled);

						moreData = (currentByte >> INT_CHUNK_BITS) == 1;
						bitsHandled += INT_CHUNK_BITS;
					} while (moreData && bitsHandled < sizeof(T) * 8);

					return value;
				}

				template<typename T>
				void writeInt(T value) noexcept
				{
					std::size_t bitsHandled = 0;

					assertIntType<T>();

					do
					{
						std::uint8_t currentByte = value & INT_CHUNK_MASK;

						if (value > INT_CHUNK_MASK || value < 0)
						{
							currentByte |= INT_CHUNK_FLAG_MASK;
						}

						this->writeData(currentByte);

						value >>= INT_CHUNK_BITS;
						bitsHandled += INT_CHUNK_BITS;
					} while (value != 0 && bitsHandled < sizeof(T) * 8);
				}

				static constexpr bool isBigEndian() noexcept;

				static const std::size_t READ_BLOCK_SIZE = 2048;
				static const int INT_CHUNK_BITS = 7;
				static const int INT_CHUNK_MASK = 0b01111111;
				static const int INT_CHUNK_FLAG_MASK = 0b10000000;


				data::io::IBytesStream &m_stream;
				data::Buffer m_readBuffer;
				data::Buffer m_transactionBackupReadBuffer;
				bool m_transaction;
				bool m_transactionReadPastEnd;
			};
		}
	}
}
