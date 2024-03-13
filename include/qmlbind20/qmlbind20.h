// ReSharper disable CppHiddenFunction
// ReSharper disable CppHidingFunction

#pragma once

#include <string>
#include <list>
#include <array>
#include <concepts>
#include <fmt/color.h>
#include <leaf/global.h>
#include <leaf/utils/rtti.h>
#include <qqml.h>

namespace qmlbind20
{
  using std::list;
  using std::array;
  using std::string;
  using std::string_view;
  using std::unique_ptr;
  using std::shared_ptr;
  using namespace std::string_view_literals;
  using namespace leaf::types;

  constexpr array QT_DEFAULT_FUNCTIONS = {
    "destroyed()"sv,
    "destroyed(QObject*)"sv,
    "objectNameChanged(QString)"sv,
    "deleteLater()"sv,
    "_q_reregisterTimers(void*)"sv
  };
  [[nodiscard]] auto remove_namespace(string_view type_name) -> string;

  template <typename T>
  concept TQObject = std::is_base_of_v<QObject, T>;

  enum class ListingBehavior
  {
    All,
    ExcludeQt
  };

  class basic_component
  {
    public:
      explicit basic_component(string_view component_name);
      virtual ~basic_component() { delete this->m_underlying; }

      [[nodiscard]] auto name() const -> string_view;
      [[nodiscard]] auto underlying_metaobject() const -> expected<QObject*, string>;

    protected:
      string m_name;
      QObject* m_underlying;
  };

  template <TQObject T>
  class inherited_component final : public basic_component
  {
    public:
      explicit inherited_component();
      virtual ~inherited_component() final = default;

      [[nodiscard]] auto underlying_metaobject() const -> expected<T*, string>;
      [[nodiscard]] auto functions(ListingBehavior) const -> list<string>;
      [[nodiscard]] auto properties(ListingBehavior) const -> list<string>;
  };

  class module
  {
    public:
      module(string_view name, int version_major, int version_minor);

      [[nodiscard]] auto name() const -> string_view;
      [[nodiscard]] auto version() const -> string;
      [[nodiscard]] auto version_major() const -> int;
      [[nodiscard]] auto version_minor() const -> int;

      template <TQObject T>
      auto inherited_component() -> module&;

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
    : basic_component(qmlbind20::remove_namespace(leaf::utils::type_name<T>()))
  {
    this->m_underlying = new T(nullptr);
  }

  template <TQObject T>
  auto inherited_component<T>::underlying_metaobject() const -> expected<T *, string> {
    return qobject_cast<T*>(this->m_underlying);
  }

  template <TQObject T>
  auto inherited_component<T>::functions(const ListingBehavior behavior) const -> list<string>
  {
    auto ret = list<string>();
    for(auto i = 0; i < T::staticMetaObject.methodCount(); ++i)
      ret.emplace_back(T::staticMetaObject.method(i).methodSignature().data());
    if(behavior == ListingBehavior::ExcludeQt)
      for(const auto &f : QT_DEFAULT_FUNCTIONS)
        ret.remove_if([&f](const auto &s) { return s == f; });
    return ret;
  }
  template <TQObject T>
  auto inherited_component<T>::properties(ListingBehavior) const -> list<string>
  {
    auto ret = list<string>();
    for(auto i = 0; i < T::staticMetaObject.propertyCount(); ++i)
      ret.emplace_back(T::staticMetaObject.property(i).name());
    return ret;
  }

  template <TQObject T>
  auto module::inherited_component() -> module&
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
      fmt::format(fg(fmt::color::light_golden_rod_yellow) | fmt::emphasis::bold, "{}", c.name())
    );
    llog::trace("\t {} functions:",
      fmt::format(fg(fmt::color::light_golden_rod_yellow) | fmt::emphasis::bold, "{}", c.name())
    );
    for(const auto& f : c.functions(ListingBehavior::ExcludeQt))
      llog::trace("\t - {}",
        fmt::format(fg(fmt::color::cyan) | fmt::emphasis::bold, "{}", f)
      );
    llog::trace("\t {} properties:",
      fmt::format(fg(fmt::color::light_golden_rod_yellow) | fmt::emphasis::bold, "{}", c.name())
    );
    for(const auto& p : c.properties(ListingBehavior::ExcludeQt))
      llog::trace("\t - {}",
        fmt::format(fg(fmt::color::lime_green) | fmt::emphasis::bold, "{}", p)
      );

    return *this;
  }
} // namespace qmlbind20
