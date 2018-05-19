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

#include <exception>
#include <string>

namespace transporter
{
	namespace exceptions
	{
		class TRANSPORTER_DLL std::exception; // NOTE: exporting std::exception because TransactionException inherits from it and otherwise MSVC will produce a C4251 warning

		/**
		* @brief Exception class for transaction-related errors
		*/
		class TRANSPORTER_DLL TransactionException : public std::exception
		{
		public:
			TransactionException(const std::string &msg);

			/**
			* @brief Get the detailed error message of this exception
			*/
			virtual const char *what() const noexcept override;


		private:
			// NOTE: warning C4251 "class X needs to have dll-interface to be used by clients of class..." can be safely disabled as it is private and won't be used by any client
#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4251)
#endif
			std::string m_msg;
#if defined(_MSC_VER)
#pragma warning( pop )
#endif
		};
	}
}
