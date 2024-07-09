/**
 * Boost.Asio TCP address resolution helpers
 *
 * SPDX-License-Identifier: MIT
 **/
#ifndef NET_RESOLVE_HPP
#define NET_RESOLVE_HPP

#include <boost/asio.hpp>

namespace droid0
{
namespace tcp
{

/**
 * Resolve a (host, port) pair into an endpoint iterator
 **/
boost::asio::ip::tcp::resolver::iterator
resolve(boost::asio::io_service &io, const char *host, const char *port);

}; // namespace tcp
}; // namespace droid0

#endif /* NET_RESOLVE_HPP */
