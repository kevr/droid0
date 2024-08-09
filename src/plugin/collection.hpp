// SPDX-License-Identifier: MIT
#ifndef PLUGIN_COLLECTION_HPP
#define PLUGIN_COLLECTION_HPP

#include <string>
#include <utility>
#include <vector>

namespace droid0::plugin
{

using string_pair = std::pair<std::string, std::string>;

class collection
{
  private:
    std::string m_dir;
    std::vector<string_pair> m_plugins;

  public:
    //! Default constructor
    collection() = default;

    //! Value constructor
    collection(const std::string &dir, std::vector<std::string> il);

    //! Copy constructor
    collection(const collection &other);

    //! Copy assignment
    collection &operator=(const collection &other);

  public:
    std::vector<string_pair>::iterator begin();
    std::vector<string_pair>::const_iterator cbegin() const;

    std::vector<string_pair>::iterator end();
    std::vector<string_pair>::const_iterator cend() const;
};

}; // namespace droid0::plugin

#endif /* PLUGIN_COLLECTION_HPP */
