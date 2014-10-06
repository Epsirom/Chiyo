#ifndef CHIYOGUI_GLOBAL_H
#define CHIYOGUI_GLOBAL_H

#include <QString>

#define AVAILABLE_OPEN_FILE_TYPES "Supported Image Files(*.jpg;*.jpeg;*.bmp;*.png)"";;All Files(*.*)"
#define AVAILABLE_SAVE_FILE_TYPES "Jpeg(*.jpg)"";;BMP(*.bmp)"";;PNG(*.png)"";;All Files(*.*)"

typedef enum chiyo_log_type {
    CHIYO_LOG_INFO = 0,
    CHIYO_LOG_WARNING = 1,
    CHIYO_LOG_ERROR = 2
} ChiyoLogType;

void chiyoLog(const QString &log);
void chiyoWarning(const QString &log);
void chiyoError(const QString &log);

#endif // CHIYOGUI_GLOBAL_H
