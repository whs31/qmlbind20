#include <qmlbind20/qmlbind20.h>

namespace qmlbind20
{
  auto remove_namespace(const string_view type_name) -> string {
    return string(type_name.substr(type_name.find_last_of(':') + 1));
  }
} // namespace qmlbind20
