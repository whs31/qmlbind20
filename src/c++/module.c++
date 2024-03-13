#include <qmlbind20/qmlbind20.h>
#include <utils.h>

using std::runtime_error;

namespace qmlbind20
{
  module::module(const string_view name, const int version_major, const int version_minor)
      : m_name(name)
      , m_version_major(version_major)
      , m_version_minor(version_minor)
  {
    utils::check_name_and_version(this->m_name, this->m_version_major, this->m_version_minor);

    qmlRegisterModule(name.data(), version_major, version_minor);
    llog::debug("registered qml module: {}{}",
      fmt::format(fg(fmt::color::blanched_almond) | fmt::emphasis::bold, "{}@", name),
      fmt::format(fg(fmt::color::coral) | fmt::emphasis::bold, "{}.{}", version_major, version_minor)
    );
  }

  auto module::name() const -> string_view { return this->m_name; }
  auto module::version() const -> string { return fmt::format("{}.{}", this->m_version_major, this->m_version_minor); }
  auto module::version_major() const -> int { return this->m_version_major; }
  auto module::version_minor() const -> int { return this->m_version_minor; }
} // namespace qmlbind20
