
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
            -lopencv_objdetect \
            -lstdc++
}

chiyo_plugin {

unix {
    TARGET_OUTPUT_FILE = lib$${TARGET}.$${VERSION}.dylib

    PLUGIN_DIRECTORY = $${OUT_PWD}/../ChiyoGUI/Chiyo.app/Contents/ChiyoPlugins
    exists(PLUGIN_DIRECTORY) {

    } else {
        QMAKE_POST_LINK += mkdir $${PLUGIN_DIRECTORY};
    }
    QMAKE_POST_LINK += cp $${OUT_PWD}/$${TARGET_OUTPUT_FILE} $${PLUGIN_DIRECTORY}/;

}

}

chiyo_framework {

unix {
    TARGET_OUTPUT_FILE = lib$${TARGET}.$${VERSION}.dylib
    TARGET_FRAMEWORK_FILE = lib$${TARGET}.0.dylib
    TARGET_FRAMEWORK_DIRECTORY = $${OUT_PWD}/../ChiyoGUI/Chiyo.app/Contents/Frameworks
    exists(TARGET_FRAMEWORK_DIRECTORY) {

    } else {
        QMAKE_POST_LINK += mkdir $${TARGET_FRAMEWORK_DIRECTORY};
    }
    QMAKE_POST_LINK += cp $${OUT_PWD}/$${TARGET_OUTPUT_FILE} $${TARGET_FRAMEWORK_DIRECTORY}/$${TARGET_FRAMEWORK_FILE};
}

}
