/**
 * Boost.Asio TCP address resolution helpers
 *
 * SPDX-License-Identifier: MIT
 **/
#include "resolve.hpp"

using tcp = boost::asio::ip::tcp;

namespace droid0
{
namespace net
{

tcp::resolver::iterator resolve(boost::asio::io_service &io, const char *host,
                                const char *port)
{
    tcp::resolver resolver(io);
    tcp::resolver::query q(host, port);
    return resolver.resolve(q);
}

}; // namespace net
}; // namespace droid0
