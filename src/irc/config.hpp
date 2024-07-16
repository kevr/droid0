#ifndef IRC_CONFIG_HPP
#define IRC_CONFIG_HPP

#include <boost/program_options.hpp>
#include <memory>

namespace droid0::irc
{

class config
{
  private:
    boost::program_options::options_description m_desc;
    std::shared_ptr<boost::program_options::variables_map> m_vm;

  public:
    config(const std::string &config_file);
    config(const config &other);
    config &operator=(const config &other);

  public:
    bool is_set(const std::string &key);

    template <typename T>
    T value(const std::string &key) const
    {
        return m_vm->at(key).as<T>();
    }

    std::string server_address() const;
    std::string server_port() const;
    std::string nick() const;
    std::string user() const;
    std::string realname() const;
    std::string channels() const;
};

}; // namespace droid0::irc

#endif /* IRC_CONFIG_HPP */
