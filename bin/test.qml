import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import io.qmlbind20.test 1.0

ApplicationWindow
{
    width: 300
    height: 300
    visible: true
    color: "magenta"

    QtMock { id: mock; dummy: 1337 }

    Text {
        text: mock.addPrefix("World", "Hello, ")
    }

    Text {
        id: propertytest
        anchors.centerIn: parent
        text: mock.dummy
    }

    Button {
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: propertytest.bottom
            topMargin: 10
        }
        onPressed: mock.dummy = mock.dummy + 1
    }
}