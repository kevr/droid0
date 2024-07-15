#ifndef CONF_CONFIG_HPP
#define CONF_CONFIG_HPP

#include <boost/program_options.hpp>

namespace droid0
{
namespace conf
{

class config
{
  private:
    // Descriptions of options
    boost::program_options::options_description m_cmdline_options;
    boost::program_options::options_description m_config_options;

    // Variable storage
    boost::program_options::variables_map m_vm;

  public:
    //! Default constructor
    config();

  public:
    //! Parse command-line arguments
    config &parse(int argc, const char *argv[]);

    //! Parse a configuration file
    config &parse(const std::string &filepath);

    //! Check if a key was set by parse
    bool is_set(const std::string &key) const;

    //! Retrieve the value of a key
    template <typename T>
    const T &value(const std::string &key) const
    {
        return m_vm.at(key).as<const T &>();
    }

    //! Print command-line help to os
    void cmdline_help(std::ostream &os, const char *progname) const;

    //! Print configuration help to os
    void config_help(std::ostream &os) const;
};

void write_default_config(const std::string &path);

}; // namespace conf
}; // namespace droid0

#endif /* CONF_CONFIG_HPP */
