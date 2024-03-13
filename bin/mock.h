#pragma once

#include <qobject.h>
#include <qstring.h>

namespace mock
{
  class QtMock : public QObject
  {
    Q_OBJECT
    Q_PROPERTY(int dummy READ dummy WRITE setDummy NOTIFY dummyChanged FINAL)

    public:
      explicit QtMock(QObject* parent = nullptr) : QObject(parent) {}
      virtual ~QtMock() override = default;

      int dummy() const
      {
        return this->m_dummy;
      }

      void setDummy(int dummy)
      {
        if(this->m_dummy == dummy)
          return;
        this->m_dummy = dummy;
        emit this->dummyChanged();
      }

      Q_SIGNAL void dummyChanged();

      Q_INVOKABLE static QString addPrefix(const QString& str, const QString& prefix)
      {
        return prefix + str;
      }

    private:
      int m_dummy = 0;
  };
}