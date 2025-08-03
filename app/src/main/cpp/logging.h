#pragma once

#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "BBLOG", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "BBLOG", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARNING, "BBLOG", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "BBLOG", __VA_ARGS__)
