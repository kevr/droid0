/**
 * Collections of Boost.Asio tcpwork errors
 *
 * SPDX-License-Identifier: MIT
 **/
#ifndef NET_ERRORS_HPP
#define NET_ERRORS_HPP

#include <boost/asio.hpp>
#include <set>

namespace droid0
{
namespace tcp
{

extern const std::set<boost::system::error_code> graceful_read_errors;
extern const std::set<boost::system::error_code> graceful_write_errors;

}; // namespace tcp
}; // namespace droid0

#endif /* NET_ERRORS_HPP */
