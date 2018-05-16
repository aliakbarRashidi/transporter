#pragma once

#include "Transporter.h"

#include <exception>
#include <string>

namespace transporter
{
	namespace exceptions
	{
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
			std::string m_msg;
		};
	}
}
