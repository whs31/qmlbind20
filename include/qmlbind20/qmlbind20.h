#pragma once

#include <string>
#include <leaf/global.h>
#include <qqml.h>

namespace qmlbind20
{
  using std::string;
  using std::string_view;
  using namespace leaf::types;

  class module_
  {
    public:
      module_(string_view name, int version_major, int version_minor);

      [[nodiscard]] auto name() const -> string_view;
      [[nodiscard]] auto version() const -> string;
      [[nodiscard]] auto version_major() const -> int;
      [[nodiscard]] auto version_minor() const -> int;

    protected:
      string m_name;
      int m_version_major;
      int m_version_minor;
  };

  template<typename T>
  auto guaranteed_call(T&& func) -> decltype(func())
  {
    [[maybe_unused]] volatile auto d = &func;
    return func();
  }
}