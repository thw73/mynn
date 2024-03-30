/****************************************************************************
*
* Beschreibung: Konfiguration des Buttons
* 
****************************************************************************/

import QtQuick 2.15

Item {
    id: button

    signal clicked

    property string text
    property color color: "white"

    width : 100
    height: 50

    BorderImage {
        id: buttonImage
        source: "toolbutton.png"
        width: button.width; 
		height: button.height
        border.left:15
        border.top: 4
        border.bottom: 4
        border.right: 15
    }
    MouseArea {
        id: mouseRegion
        anchors.fill: buttonImage
        onClicked: { button.clicked(); }
    }
    Text {
        id: buttonText
        anchors.fill: buttonImage
        anchors.margins: 5
        text: button.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight //Sorgt dafuer, dass der Text nicht ueber den Button gebt
        color: button.color
        font.bold: true
        style: Text.Raised
        styleColor: "black"
        font.pixelSize: 12
    }
}
