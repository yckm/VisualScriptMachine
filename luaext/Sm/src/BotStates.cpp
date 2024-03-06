#include "../include/BotStates.h"
#include <iostream>
#include "../../Models/Enums.h"
#include "../../Models/Models.h"
#include "../include/Utils.h"

namespace Pcs {
	namespace Sm {
		using namespace Model;
		/**
		 * @创建人 dns
		 * @简介 更新来自plc机器人的状态
		 * @参数 bot 机器人状态
		 * @参数 joints 关节角度
		 * @参数 tcps tcp
		 * @参数 robState 机器人状态
		 * @参数 robTaskState 任务状态
		 * @参数 safetyState 安全状态
		 */
		static void updateStatus(BotStateModel& bot, std::vector<double> joints, std::vector<double> tcps, int robState, int robTaskState, int safetyState) {
			bot.joints = joints;
			bot.tcp = tcps;

			bot.robState = robState;
			bot.robTaskState = robTaskState;
			bot.safetyState = safetyState;
			bot.ts = Utils::GetTsMs();
		}

		/**
		* @创建人 dns
		* @简介 PLC更新 gBotState 的值
		* @参数 joints 关节角度
		* @参数 tcps 末端位置
		* @参数 robState 机器人状态
		* @参数 robTaskState 机器人任务状态
		* @参数 safetyState 安全控制器状态
		* @返回值 是否设置成功
		*/
		bool BotStates::update(std::vector<double> joints, std::vector<double> tcps, int robState, int robTaskState, int safetyState) {
			try {
				if (_bid == 0) {
					updateStatus(botState1, joints, tcps, robState, robTaskState, safetyState);
					_bid = 1;
				}
				else {
					updateStatus(botState0, joints, tcps, robState, robTaskState, safetyState);
					_bid = 0;
				}				
				return true;
			}
			catch (const std::exception e) {
				std::cout << e.what() << std::endl;
				return false;
			}
		}
		/**
		 * @创建人 dns
		 * @简介 内部获取外部机械臂状态
		 * @返回值  机械臂状态
		 */
		const BotStateModel& BotStates::getBotStatus() {
			return _bid == 0 ? botState0 : botState1;
		}
	}
}