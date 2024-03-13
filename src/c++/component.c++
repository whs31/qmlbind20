#include <qmlbind20/qmlbind20.h>
#include <utils.h>

using std::make_unique;
using leaf::Err;

namespace qmlbind20
{
  basic_component::basic_component(const string_view component_name)
    : m_name(component_name),
      m_underlying(nullptr)
  {
    utils::check_name_and_version(component_name, 1, 0);
  }

  auto basic_component::name() const -> string_view { return this->m_name; }
  auto basic_component::underlying_metaobject() const -> expected<QObject*, string> {
    if(not this->m_underlying)
      return Err("metaobject is null");
    return this->m_underlying;
  }
} // namespace qmlbind20
