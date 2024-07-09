/**
 * Collections of Boost.Asio TCP errors
 *
 * SPDX-License-Identifier: MIT
 **/
#include "errors.hpp"

const std::set<boost::system::error_code> droid0::tcp::graceful_read_errors{
    boost::asio::error::eof,
};

const std::set<boost::system::error_code> droid0::tcp::graceful_write_errors{
    boost::asio::error::operation_aborted,
    boost::asio::error::bad_descriptor,
};
