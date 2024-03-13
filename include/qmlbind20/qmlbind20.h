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

  class component;
  // template <TQObject T> class inherited_component;

  template <TQObject T>
  class inherited_component
  {
    public:
      using type = T;

      explicit inherited_component()
        : m_name(qmlbind20::remove_namespace(leaf::utils::type_name<type>())),
          m_underlying(new type(nullptr))
      {}

      ~inherited_component() { delete this->m_underlying; }

      [[nodiscard]] auto name() const -> string_view { return this->m_name; }
      [[nodiscard]] auto underlying_metaobject() const -> expected<type*, string> {
        return qobject_cast<type*>(this->m_underlying);
      }

    private:
      string m_name;
      QObject* m_underlying;
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
      auto inherited_component() const -> void
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