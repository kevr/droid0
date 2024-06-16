/**
 * Logger class and static instances
 *
 * SPDX-License-Identifier: MIT
 **/
#include "logger.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace droid0
{

const loglevel logger::default_level = loglevel::info;
loglevel logger::m_level = logger::default_level;
std::ofstream logger::m_file_stream;
std::ostream &logger::default_stream = std::cout;
std::ostream *logger::m_stream = &logger::default_stream;

void logger::set_loglevel(loglevel level)
{
    m_level = level;
}

void logger::set_stream(std::ostream &stream)
{
    m_file_stream.close();
    m_stream = &stream;
}

void logger::set_stream(const std::string &logfile)
{
    m_file_stream.open(logfile.c_str(), std::ios::app);
    if (!m_file_stream.is_open()) {
        throw std::runtime_error("unable to open " + logfile + " for writing");
    }

    m_stream = &m_file_stream;
}

void logger::error(const std::string &message)
{
    return print(loglevel::error, message);
}

void logger::warning(const std::string &message)
{
    return print(loglevel::warning, message);
}

void logger::info(const std::string &message)
{
    return print(loglevel::info, message);
}

void logger::debug(const std::string &message)
{
    return print(loglevel::debug, message);
}

void logger::trace(const std::string &source, const std::string &message)
{
    std::string trace_message("(" + source + ") " + message);
    return print(loglevel::trace, trace_message);
}

std::string logger::datetime() const
{
    const auto now = std::chrono::system_clock::now();
    const std::time_t in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S %Z");
    return ss.str();
}

void logger::print(loglevel level, const std::string &message)
{
    if (level <= m_level) {
        std::stringstream ss;
        ss << "[" << datetime() << "] [" << LOGLEVEL_STR[level] << "] "
           << message << "\n";
        *m_stream << ss.str();
    }
}

logger logging = logger();

}; // namespace droid0
