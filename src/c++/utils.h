#pragma once

#include <string>
#include <leaf/leaf.h>

namespace qmlbind20::utils
{
  using std::string;
  using std::string_view;

  auto check_name_and_version(string_view name, int version_major, int version_minor) noexcept(false) -> void;
} // namespace qmlbind20::utils

namespace qmlbind20::utils
{
  inline auto check_name_and_version(const string_view name, const int version_major,
                                     const int version_minor)
  noexcept(false) -> void
  {
    using std::invalid_argument;

    if(name.empty())
      throw invalid_argument("module name cannot be empty");
    if(version_major < 0)
      throw invalid_argument("module version major cannot be negative");
    if(version_minor < 0)
      throw invalid_argument("module version minor cannot be negative");
  }
}
