#include "TransactionException.h"

transporter::exceptions::TransactionException::TransactionException(const std::string &msg) : std::exception{},
m_msg{ msg }
{

}

const char *transporter::exceptions::TransactionException::what() const noexcept
{
	return m_msg.c_str();
}
