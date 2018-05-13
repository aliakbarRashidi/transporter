#pragma once

#include "Transporter.h"

#include <exception>
#include <string>

namespace transporter
{
	namespace exceptions
	{
		class TRANSPORTER_DLL TransactionException : public std::exception
		{
		public:
			TransactionException(const std::string &msg);

			virtual const char *what() const noexcept override;


		private:
			std::string m_msg;
		};
	}
}
