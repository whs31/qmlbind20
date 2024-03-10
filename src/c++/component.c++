#include <qmlbind20/qmlbind20.h>
#include <utils.h>

namespace qmlbind20
{
  component::component(const string_view name, const int version_major, const int version_minor)
      : m_name(name)
      , m_version_major(version_major)
      , m_version_minor(version_minor)
  {
    utils::check_name_and_version(name, version_major, version_minor);
  }

  auto component::inherit(QObject *object) -> void
  {

  }
} // namespace qmlbind20
