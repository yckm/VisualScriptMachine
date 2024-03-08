#include "../include/Signals.h"
#include <iostream>

#ifndef SIGNALCNT 
#define SIGNALCNT 16
#endif //  信号量


namespace Pcs {
    namespace Sm {
        std::vector<int> Signals::signals(16);

        /**
        * @创建人 dnp
        * @简介 等待信号.
        * @参数 idx 信号编号
        * @返回值 信号值
        */
        int Signals::get(int idx) {
            if (idx<0 or idx>SIGNALCNT - 1) {
                return 0;
            }

            int sign = signals[idx];
            if (sign > 0) {
                signals[idx] = 0;
            }
            
            return sign;
        }

        /**
        * @创建人 dnp
        * @简介 发送信号.
        * @参数 idx 信号编号
        * @参数 signal 信号
        * @返回值
        */
        bool Signals::send(int idx, int signal) {
            if (idx<0 or idx>SIGNALCNT - 1) {
                return false;
            }
            signals[idx] = signal;
            return true;
        }
    }
}