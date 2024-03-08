#pragma once
namespace Pcs {
	namespace Model {
        /**
         * @创建人 dnp
         * @简介 执行状态控制
         */
        enum EnumPrst {
            normal = 0, // 正常运行,不用执行任何操作
            stop = 1, // 停止脚本中只正在执行的那一行指令
            terminate =2, // 终止当前脚本
            puase = 3 // 暂停(this item only use for function "wait"
        };

        /**
         * @创建人 dnp
         * @简介 脚本/指令状态
         */
        enum EnumExecuteStatus {
            notstart = 0,
            running = 1,
            terminated = 2
        };

        enum EnumExtFunc {
            none=-1,// 没有任何函数
            movej=0,
            movej_d = 1,
            movep = 2,            
            movep_d = 3,
            movel = 4,
            movel_d = 5,
            move_tcp=7,
            update_wobj=10,
            use_wobj=11,
            use_tool=12,
            tool = 20 //工具控制函数         
        };
	}
}
