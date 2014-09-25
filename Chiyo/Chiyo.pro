include(paths.pri)

message()
message(==============================)

TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    ChiyoPlugins \
    ChiyoGUI

CHIYO_SUPPORTED_LANGUAGES = zh_CN en_US

for(lang, CHIYO_SUPPORTED_LANGUAGES) {
    TRANSLATIONS += ChiyoGUI/lang/$${lang}.ts
}

message(Languages loaded: $${CHIYO_SUPPORTED_LANGUAGES})
