#pragma once

#include <qobject.h>
#include <qstring.h>

namespace mock
{
  class QtMock : public QObject
  {
    Q_OBJECT

    public:
      explicit QtMock(QObject* parent = nullptr) : QObject(parent) {}
      virtual ~QtMock() override = default;

      Q_INVOKABLE static QString addPrefix(const QString& str, const QString& prefix)
      {
        return prefix + str;
      }
  };
}