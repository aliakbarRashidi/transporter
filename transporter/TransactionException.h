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
