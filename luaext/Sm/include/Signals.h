#pragma once
/*****************************************************************
 * @文件名称:Signals.h
 * @创建人:董新强
 * @创建日期:2024 March 
 * @简述: 通道信号量
*******************************************************************/
#include <iostream>
#include <vector>

namespace Pcs {
    namespace Sm {
        class Signals
        {
        public:
            /**
             * @创建人 dnp
             * @简介 等待信号.
             * @参数 idx 信号编号
             * @返回值 信号值
             */
            static int get(int idx);

            /**
             * @创建人 dnp
             * @简介 发送信号.
             * @参数 idx 信号编号
             * @参数 signal 信号
             * @返回值
             */
            static bool send(int idx, int signal);

        private:
            static std::vector<int> signals;

        };
    }
}