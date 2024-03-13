// ReSharper disable CppHiddenFunction
// ReSharper disable CppHidingFunction

#pragma once

#include <string>
#include <concepts>
#include <fmt/color.h>
#include <leaf/global.h>
#include <leaf/utils/rtti.h>
#include <qqml.h>

namespace qmlbind20
{
  using std::string;
  using std::string_view;
  using std::unique_ptr;
  using std::shared_ptr;
  using namespace leaf::types;

  [[nodiscard]] auto remove_namespace(string_view type_name) -> string;

  template <typename T>
  concept TQObject = std::is_base_of_v<QObject, T>;

  class component
  {
    public:
      explicit component(string_view component_name);
      virtual ~component() { delete this->m_underlying; }

      [[nodiscard]] auto name() const -> string_view;
      [[nodiscard]] auto underlying_metaobject() const -> expected<QObject*, string>;

    protected:
      string m_name;
      QObject* m_underlying;
  };

  template <TQObject T>
  class inherited_component final : public component
  {
    public:
      explicit inherited_component();
      virtual ~inherited_component() final = default;

      [[nodiscard]] auto underlying_metaobject() const -> expected<T*, string>;
  };

  class module
  {
    public:
      module(string_view name, int version_major, int version_minor);

      [[nodiscard]] auto name() const -> string_view;
      [[nodiscard]] auto version() const -> string;
      [[nodiscard]] auto version_major() const -> int;
      [[nodiscard]] auto version_minor() const -> int;

      auto component(const qmlbind20::component& c) const -> void;

      template <TQObject T>
      auto inherited_component() const -> void;

    protected:
      string m_name;
      int m_version_major;
      int m_version_minor;
  };

  template <std::invocable T>
  auto guaranteed_call(T&& func) -> decltype(func())
  {
    [[maybe_unused]] volatile auto d = &func;
    return func();
  }
}

namespace qmlbind20
{
  template <TQObject T> inherited_component<T>::inherited_component()
    : component(qmlbind20::remove_namespace(leaf::utils::type_name<T>()))
  {
    this->m_underlying = new T(nullptr);
  }

  template <TQObject T> auto inherited_component<T>::underlying_metaobject() const -> expected<T*, string> { return qobject_cast<T*>(this->m_underlying); }

  template <TQObject T>
  auto module::inherited_component() const -> void
  {
    const auto c = qmlbind20::inherited_component<T>();
    if(const auto m = c.underlying_metaobject(); not m)
      throw std::runtime_error(m.error());
    qmlRegisterType<T>(
      this->name().data(),
      this->version_major(),
      this->version_minor(),
      c.name().data()
    );
    llog::debug("registered inherited qml component: {}",
      fmt::format(fg(fmt::color::medium_violet_red) | fmt::emphasis::bold, "{}", c.name())
    );
  }
} // namespace qmlbind20
