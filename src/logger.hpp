/**
 * Logger class and static instances
 *
 * SPDX-License-Identifier: MIT
 **/
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <iostream>
#include <string>

namespace droid0
{

enum loglevel : int
{
    error,
    warning,
    info,
    debug,
    trace,
};

const char *const LOGLEVEL_STR[] = {
    "ERROR", "WARNING", "INFO", "DEBUG", "TRACE",
};

class logger
{
  private:
    static const loglevel default_level;
    static loglevel m_level;

    static std::ofstream m_file_stream;
    static std::ostream &default_stream;
    static std::ostream *m_stream;

  public:
    static void set_loglevel(loglevel level = default_level);
    static void set_stream(std::ostream &stream = default_stream);
    static void set_stream(const std::string &logfile);

    void error(const std::string &message);
    void warning(const std::string &message);
    void info(const std::string &message);
    void debug(const std::string &message);
    void trace(const std::string &origin, const std::string &message);

  private:
    std::string datetime() const;
    void print(loglevel level, const std::string &message);
};

extern logger logging;

class trace_instance
{
  private:
    std::string m_func;

  public:
    trace_instance(const std::string &func);
    ~trace_instance();
};

}; // namespace droid0

using droid0::logging;

#define SCOPE_TRACE() droid0::trace_instance __trace(__PRETTY_FUNCTION__);

#endif /* LOGGER_HPP */
