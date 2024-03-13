#pragma once

#include <string>
#include <concepts>
#include <leaf/global.h>
#include <qqml.h>

namespace qmlbind20
{
  using std::string;
  using std::string_view;
  using std::unique_ptr;
  using std::shared_ptr;
  using namespace leaf::types;

  class component;

  template<typename T>
  concept TQObject = std::is_base_of_v<QObject, T>;

  class module
  {
    public:
      module(string_view name, int version_major, int version_minor);

      [[nodiscard]] auto name() const -> string_view;
      [[nodiscard]] auto version() const -> string;
      [[nodiscard]] auto version_major() const -> int;
      [[nodiscard]] auto version_minor() const -> int;

      auto component(const qmlbind20::component& c) const -> void;

    protected:
      string m_name;
      int m_version_major;
      int m_version_minor;
  };

  class component
  {
    public:
      explicit component(string_view component_name);
      ~component() { delete this->m_underlying; }

      [[nodiscard]] auto name() const -> string_view;
      [[nodiscard]] auto underlying_metaobject() const -> expected<QObject*, string>;

      template <typename T>
      [[nodiscard]] auto underlying_metaobject_t() const -> expected<T*, string>
      {
        if(not this->m_underlying)
          return leaf::Err("metaobject is null");
        return static_cast<T*>(this->m_underlying);
      }

      template <TQObject T>
      auto inherit() -> component&;

    private:
      string m_name;
      QObject* m_underlying;
  };

  template <TQObject T>
  auto component::inherit() -> component& {
    this->m_underlying = new QObject(nullptr);
    return *this;
  }

  template <std::invocable T>
  auto guaranteed_call(T&& func) -> decltype(func())
  {
    [[maybe_unused]] volatile auto d = &func;
    return func();
  }
}