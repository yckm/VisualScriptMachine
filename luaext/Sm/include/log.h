#pragma once
#include "glog/logging.h"
#include <io.h>
#include <direct.h>

#ifndef Log
#define Log LOG(INFO)
#endif // !Log

#ifndef Err
#define Err LOG(ERROR)
#endif // !Err

#ifndef EPOLLTIME
#define EPOLLTIME 1 //  内部循环时间
#endif // !EPOLLTIME

#ifndef CHANNEL_NUM
#define CHANNEL_NUM 1 //  通道数
#endif // !EPOLLTIME

