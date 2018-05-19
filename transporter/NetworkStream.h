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

#include <functional>
#include <memory>
#include <type_traits>

#include "IByteStream.h"
#include "IDataInput.h"
#include "IDataOutput.h"
#include "INetworkMessage.h"
#include "TransactionException.h"

namespace transporter
{
	namespace network
	{
		namespace io
		{
			typedef std::function<transporter::network::messages::NetworkMessagePtr(transporter::network::messages::NetworkMessageId)> NetworkMessageSelector;

			/**
			* @brief Stream handling network messages (transporter::network::messages::INetworkMessage)
			*
			* @details Despite its name, this class can be used with any stream (transporter::data::io::IByteStream)
			*/
			class TRANSPORTER_DLL NetworkStream : public transporter::data::io::IDataInput, public transporter::data::io::IDataOutput
			{
			public:
				/**
				* @brief Initialize a network stream 
				*
				* @param stream Underlying stream this network stream will read from and write to
				*/
				NetworkStream(transporter::data::io::IByteStream &stream) noexcept;

				~NetworkStream() noexcept = default;


				/**
				* @brief Send \p message to the underlying stream
				*/
				void sendMessage(const transporter::network::messages::INetworkMessage &message);
				
				/**
				* @brief Try to receive a network message
				*
				* @details Use \p selector to instanciate the right type of network message and deserialize it using the underlying stream
				*
				* @param selector Network message selector used to instanciate the right type of network message. Must return \c nullptr if not matching ID was found
				*
				* @return Pointer to the newly-constructed, received network message. Return \c nullptr if no message was available or a transporter::network::messages::UnknownMessage if data was available but \p selector returned \c nullptr
				*/
				transporter::network::messages::NetworkMessagePtr receiveMessage(const transporter::network::io::NetworkMessageSelector &selector);


				/**
				* @brief Start a read transaction
				*
				* @return True if read transaction could be started, false otherwise
				*/
				bool startReadTransaction() noexcept;

				/**
				* @brief Commit a read transaction
				*
				* @return True if read transaction could be committed, false otherwise
				*/
				bool commitReadTransaction() noexcept;

				/**
				* @brief Rollback a read transaction
				*
				* @return True if read transaction could be rollbacked, false otherwise
				*/
				bool rollbackReadTransaction() noexcept;


				/**
				* @brief Start a write transaction
				*
				* @return True if write transaction could be started, false otherwise
				*/
				bool startWriteTransaction() noexcept;

				/**
				* @brief Commit a write transaction
				*
				* @return True if write transaction could be committed, false otherwise
				*/
				bool commitWriteTransaction() noexcept;

				/**
				* @brief Rollback a write transaction
				*
				* @return True if write transaction could be rollbacked, false otherwise
				*/
				bool rollbackWriteTransaction() noexcept;


				virtual transporter::data::BufferPtr readBytes(std::size_t count) noexcept override;
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

				template<typename T>
				T readData() noexcept
				{
					std::size_t dataSize = sizeof(T);
					transporter::data::BufferPtr buffer = this->readBytes(dataSize);
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


				static const std::size_t READ_BLOCK_SIZE = 2048;
				static const int INT_CHUNK_BITS = 7;
				static const int INT_CHUNK_MASK = 0b01111111;
				static const int INT_CHUNK_FLAG_MASK = 0b10000000;


				data::io::IByteStream &m_stream;
				data::Buffer m_readBuffer;
				data::Buffer m_readTransactionBackupBuffer;
				data::Buffer m_writeTransctionBuffer;
				bool m_readTransaction;
				bool m_readTransactionError;
				bool m_writeTransaction;
				bool m_writeTransactionError;
			};
		}
	}
}
