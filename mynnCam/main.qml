

import QtQuick 2.15
import QtQuick.Layouts 1.3
import QtQuick.Window 2.15
import QtMultimedia 5.15
import QtQuick.Controls 2.15

import MyFilterLib 1.0


Window {

    id: window

    visible: true
    width: 640
    height: 480

    title: qsTr("Mein neuronales Netz zur Ziffernerkennung")

    color: "black"

     //Camera Auswahl Propertys
     property var cameras: QtMultimedia.availableCameras
     property int cameraIndex: -1
     property var cameraInfo: null
     property string cameraDisplayName: ""

     //Display Orienierungs Propertys
     property string displayorientation:""  //MobileLandscape" "MobilePortrait"
     property string screenori: Screen.orientation


     onScreenoriChanged:  {
         setMobileDisplayorientation()
     }


     Camera {
          id: camera
      }


     GridLayout{
         id: grid

         columns: 3 //Spalten
         rows: 3    //Zeilen
         anchors.fill: parent


         MyButton {
             id: cameraButton

             Layout.column: 0
             Layout.row: 0
             Layout.alignment: ((displayorientation === "MobilePortrait") ?  (Qt.AlignLeft): (Qt.AlignHCenter | Qt.AlignVCenter))
             Layout.leftMargin: 10

             text: getCameraDisplayName( camera.deviceId )
                   //    visible: !videoFilterMenu.visible
              MouseArea {
                   anchors.fill: parent

                   onClicked: nextCamera()
              }
         }


         Text {
               id: txt
               Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

               color: "white"
               font.pixelSize: 18
               text: "Erkannt: "

               Layout.column:  ((displayorientation === "MobilePortrait") ?  (1): (2))  //true : false
               Layout.row:  ((displayorientation === "MobilePortrait") ?  (0): (1))

               objectName: "myLabel"
          }


        Image {

                id: myImage

                Layout.preferredWidth: 100
                Layout.preferredHeight: 100

                Layout.column: 2
                Layout.row: 0

                Layout.rightMargin: 10
                Layout.topMargin:  10

                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                property bool counter: false

                source: "image://myimg/1"
                asynchronous: false

                cache: false

                function reload()
                {
                      counter = !counter
                      source = "image://myimg/image?id=" + counter //Counter = true oder false
                      //console.log("counter",counter);
                 }
               }



        Rectangle {
                id: rectangle

                color: "black"

                Layout.column: ((displayorientation === "MobilePortrait") ?  (0): (1))
                Layout.row:  ((displayorientation === "MobilePortrait") ?  (1): (0))
                Layout.columnSpan:((displayorientation === "MobilePortrait") ?  (3): (1))
                Layout.rowSpan:  ((displayorientation === "MobilePortrait") ?  (1) : (3))

                Layout.rightMargin: 10
                Layout.leftMargin: 10
                Layout.fillHeight: true
                Layout.fillWidth: true

                VideoOutput {

                    id: myVideoOutput
                    source: camera

                    autoOrientation: true //Bewirkt, dass sich das Bilder der Kamera mitdreht,
                                          //wenn dass SmartPhone gedreht wird.
                                          //Trotzdem muss in der Cpp Methode das Bild zusaetzlich gedreht werden
                    filters: [ filter ]

                    anchors.fill: parent

                    Binding {

                        target: filter

                        property: "videoOutputOrientation"

                        value: myVideoOutput.orientation
                     }
                }
         }


        MyButton {
                text: "Capture "

                Layout.column:  ((displayorientation === "MobilePortrait") ?  (1): (0))
                Layout.row:  ((displayorientation === "MobilePortrait") ?  (2): (1))
                Layout.alignment:  Qt.AlignHCenter | Qt.AlignVCenter

                Layout.bottomMargin:  ((displayorientation === "MobilePortrait") ?  (10): (0))
                Layout.leftMargin: ((displayorientation === "MobilePortrait") ?  (0): (10))
                onClicked: filter.capture(); //Aufruf der CPP Methode: MyFilter::capture()
        }


    }


    //----------------------Verbindungen zu CPP Klassen---------------------

    Connections{
          target: ImageProvider

          function onImageChanged() //getriggert durch Signal imageChanged() in myimageprovider
          {
              myImage.reload()
          }
     }

     Connections{
          target: myClassObj

          function onTextChanged(text) //getriggert durch Signal textChanged() in myClassObj
          {
             txt.text = text
          }
     }

     //----------------------Funktionen---------------------


     function setMobileDisplayorientation()
     {
          if (Qt.platform.os === "android")
          {
              if (Screen.orientation===2 || Screen.orientation===8 )
              {
                  displayorientation= "MobileLandscape";
              }
              else
              {
                  displayorientation  = "MobilePortrait";
              }
          }
          else
          {
              displayorientation = "MobilePortrait";
          }
          //console.log("Displayorientation: " + displayorientation);

     }


     function getCameraDisplayName( cameraId ) {
               const cameras = QtMultimedia.availableCameras;
               if ( !cameras || !cameras.length ) return;
               for ( const cameraInfo of cameras ) {
                   if ( cameraInfo.deviceId === camera.deviceId ) {
                       return cameraInfo.displayName;
                   }
               }
               return null;
     }

     function findCameraIndex( cameraId ) {
               for ( let cameraIndex = 0; cameraIndex < cameras.length; cameraIndex++ ) {
                   if ( cameras[ cameraIndex ].deviceId === cameraId ) {
                       return cameraIndex;
                   }
               }
               return -1;
     }

     function nextCamera() {
               cameras = QtMultimedia.availableCameras;
               if ( !cameras || !cameras.length ) {
                   cameraIndex = -1;
                   cameraInfo = null;
                   cameraDisplayName = "";
                   return;
               }
               cameraIndex = ( cameraIndex + 1 ) % cameras.length;
               cameraInfo = cameras[ cameraIndex ];
               cameraDisplayName = cameraInfo.displayName;
               camera.stop();
               camera.deviceId = cameras[ cameraIndex ].deviceId;
               camera.start();
     }

     Component.onCompleted: {
               cameras = QtMultimedia.availableCameras;
               cameraIndex = findCameraIndex( camera.deviceId );
               if ( cameraIndex === -1 ) return;
               cameraInfo = cameras[ cameraIndex ];
               cameraDisplayName = cameraInfo.displayName;
     }

}



