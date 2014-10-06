
# Common settings for OS X
unix {
    QMAKE_MAC_SDK = macosx10.9
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
    QMAKE_CXXFLAGS += -std=c++11
}

use_opencv {
    INCLUDEPATH += /usr/local/Cellar/opencv/2.4.9/include
    LIBS += -L/usr/local/Cellar/opencv/2.4.9/lib \
            -lopencv_core \
            -lopencv_highgui \
            -lopencv_imgproc \
            -lopencv_features2d \
            -lopencv_nonfree \
            -lopencv_flann \
            -lstdc++
}

chiyo_plugin {

unix {
    TARGET_OUTPUT_FILE = lib$${TARGET}.$${VERSION}.dylib

    QMAKE_POST_LINK += cp $${OUT_PWD}/$${TARGET_OUTPUT_FILE} $${OUT_PWD}/../ChiyoGUI/Chiyo.app/Contents/ChiyoPlugins/

}

}
