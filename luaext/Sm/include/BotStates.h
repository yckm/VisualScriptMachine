#pragma once
#include <iostream>
#include "../../Models/Models.h"

namespace Pcs {
	namespace Sm {
		using namespace Model;
		class BotStates
		{
		private:
			BotStateModel botState0;// 机器人状态
			BotStateModel botState1;// 机器人状态
			int _bid = 0;
		public:
			/**
			 * @创建人 dnp
			 * @简介 PLC更新 gBotState 的值
			 * @参数 joints 关节角度
			 * @参数 tcps 末端位置
			 * @参数 robState 机器人状态
			 * @参数 robTaskState 机器人任务状态
			 * @参数 safetyState 安全控制器状态
			 * @返回值 是否设置成功
			 */
			bool update(std::vector<double> joints, std::vector<double> tcps, int robState, int robTaskState, int safetyState);
			/**
			 * @创建人 dnp
			 * @简介 内部获取外部机械臂状态
			 * @返回值  机械臂状态
			 */
			const BotStateModel& getBotStatus();
		};
	}
}
