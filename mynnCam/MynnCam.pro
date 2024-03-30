QT += quick multimedia qml

CONFIG += c++11


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        MyFilter.cpp \
        MyFilterRunable.cpp \
        MyImageProvider.cpp \
        main.cpp \
        mynn.cpp \
        mynnwrapper.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

HEADERS += \
    MyFilter.h \
    MyFilterRunable.h \
    MyImageProvider.h \
    mynn.h \
    mynnwrapper.h



win32{

# EigenLib

INCLUDEPATH += C:\work\Spielwiese\EigenLib\eigen-3.4.0

# OpenCV

OPENCV_PATH = C:\work\Spielwiese\OpenCV-MinGW-Build-OpenCV-4.5.2-x64

INCLUDEPATH += $${OPENCV_PATH}\include

LIBS += $${OPENCV_PATH}\x64\mingw\bin\libopencv_core452.dll
LIBS += $${OPENCV_PATH}\x64\mingw\bin\libopencv_highgui452.dll
LIBS += $${OPENCV_PATH}\x64\mingw\bin\libopencv_imgcodecs452.dll
LIBS += $${OPENCV_PATH}\x64\mingw\bin\libopencv_imgproc452.dll
LIBS += $${OPENCV_PATH}\x64\mingw\bin\libopencv_features2d452.dll
LIBS += $${OPENCV_PATH}\x64\mingw\bin\libopencv_calib3d452.dll


deployment.files += $$PWD/android-sources/assets/who.csv
deployment.files += $$PWD/android-sources/assets/wih.csv
deployment.path = $$OUT_PWD/assets
INSTALLS += deployment
}



unix{

# EigenLib
INCLUDEPATH += /home/thomas/Spielwiese/eigen-3.4.0

#OpenCV
INCLUDEPATH += /usr/include/opencv2
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_features2d -lopencv_calib3d -lopencv_videoio


deployment.files += $$PWD/android-sources/assets/who.csv
deployment.files += $$PWD/android-sources/assets/wih.csv
deployment.path = $$OUT_PWD/assets
INSTALLS += deployment


}

android{

        QT +=androidextras

        # EigenLib
        INCLUDEPATH += /home/thomas/Spielwiese/eigen-3.4.0

        ANDROID_TARGET_ARCH = arm64-v8a

        OPENCV_ANDROID_SDK_PATH = /home/thomas/Spielwiese/OpenCV-android-sdk_4_5_4/



        INCLUDEPATH += $${OPENCV_ANDROID_SDK_PATH}/sdk/native/jni/include/


        LIBS += -L$${OPENCV_ANDROID_SDK_PATH}/sdk/native/libs/arm64-v8a -lopencv_java4

        LIBS += \
                -L$${OPENCV_ANDROID_SDK_PATH}/sdk/native/staticlibs/arm64-v8a \
                -lopencv_dnn\
                -lopencv_ml\
                -lopencv_objdetect\
                -lopencv_photo\
                -lopencv_stitching\
                -lopencv_video\
                -lopencv_calib3d\
                -lopencv_features2d\
                -lopencv_highgui\
                -lopencv_flann\
                -lopencv_videoio\
                -lopencv_imgcodecs\
                -lopencv_imgproc\
                -lopencv_core

        ANDROID_EXTRA_LIBS =$${OPENCV_ANDROID_SDK_PATH}/sdk/native/libs/arm64-v8a/libopencv_java4.so



        ANDROID_PACKAGE_SOURCE_DIR += $$PWD/android-sources/assets



        deployment.files += $$PWD/android-sources/assets/who.csv
        deployment.files += $$PWD/android-sources/assets/wih.csv
        deployment.path = /assets
        INSTALLS += deployment

        DISTFILES += \
            android-sources/assets/AndroidManifest.xml \
            android-sources/assets/build.gradle \
            android-sources/assets/dogs.jpg \
            android-sources/assets/gradle/wrapper/gradle-wrapper.jar \
            android-sources/assets/gradle/wrapper/gradle-wrapper.properties \
            android-sources/assets/gradlew \
            android-sources/assets/gradlew.bat \
            android-sources/assets/res/values/libs.xml \
            android-sources/assets/who.csv \
            android-sources/assets/wih.csv

}

