#include <qmlbind20/qmlbind20.h>

using std::runtime_error;

namespace qmlbind20
{
  module_::module_(const string_view name, const int version_major, const int version_minor)
      : m_name(name)
      , m_version_major(version_major)
      , m_version_minor(version_minor)
  {
    if(m_name.empty())
      throw runtime_error("module name cannot be empty");
    if(m_version_major < 0)
      throw runtime_error("module version major cannot be negative");
    if(m_version_minor < 0)
      throw runtime_error("module version minor cannot be negative");

    qmlRegisterModule(name.data(), version_major, version_minor);
    llog::debug("registered qml module: {}@{}.{}", name, version_major, version_minor);
  }

  auto module_::name() const -> string_view { return this->m_name; }
  auto module_::version() const -> string { return fmt::format("{}.{}", this->m_version_major, this->m_version_minor); }
  auto module_::version_major() const -> int { return this->m_version_major; }
  auto module_::version_minor() const -> int { return this->m_version_minor; }
} // namespace qmlbind20
