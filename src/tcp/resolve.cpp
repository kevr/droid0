/**
 * Boost.Asio TCP address resolution helpers
 *
 * SPDX-License-Identifier: MIT
 **/
#include "resolve.hpp"

namespace droid0
{
namespace tcp
{

boost::asio::ip::tcp::resolver::iterator resolve(boost::asio::io_service &io,
                                                 const std::string &host,
                                                 const std::string &port)
{
    boost::asio::ip::tcp::resolver resolver(io);
    boost::asio::ip::tcp::resolver::query q(host, port);
    return resolver.resolve(q);
}

}; // namespace tcp
}; // namespace droid0
