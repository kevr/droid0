/**
 * Network handler types (std::function)
 *
 * SPDX-License-Identifier: MIT
 **/
#ifndef NET_HANDLERS_HPP
#define NET_HANDLERS_HPP

#include <boost/asio.hpp>
#include <functional>

namespace droid0
{
namespace tcp
{

template <typename T>
using readline_function = std::function<void(
    T, const boost::system::error_code &, const std::string &)>;

template <typename T>
using error_function =
    std::function<void(T, const boost::system::error_code &)>;

template <typename T>
using close_function =
    std::function<void(T, const boost::system::error_code &)>;

}; // namespace tcp
}; // namespace droid0

#endif /* NET_HANDLERS_HPP */
