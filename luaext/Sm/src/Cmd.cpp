#include "../include/Cmd.h"
#include <iostream>
#include <tuple>
#include "../../Models/Enums.h"
#include "../../Models/Models.h"
#include "../include/Utils.h"

namespace Pcs {
	namespace Sm {
		using namespace Model;
		
		Cmd::Cmd(int channelId) {
			this->channelId = channelId;
		}

		/**
		 * @创建人 dns
		 * @简介 PLC查询控制指令
		 * @参数 channel 通道 ID。
		 * @返回值 控制指令
		 */
		const CmdModel& Cmd::getCmd() {
			return cmd;
		}

		/**
		 * @创建人 dns
		 * @简介 PLC更新指令状态
		 * @参数 cmdId 控制指令ID
		 * @参数 status 指令目标状态
		 * @返回值 是否设置成功
		 */
		bool Cmd::setStatus(EnumExecuteStatus status) {
			cmd.status = status;
			return true;
		}

		/**
		 * @创建人 dns
		 * @简介 executor更新指令
		 * @参数 cmd 新指令
		 * @返回值 是否设置成功
		 */
		int Cmd::update(CmdModel newcmd, ChannelStateModel* state) {
			curCmdId++;

			cmd.id = curCmdId;
			cmd.func = newcmd.func;
			cmd.params = newcmd.params;

			// 运动函数才进行运动参数的设置
			if (newcmd.func < EnumExtFunc::update_wobj) {
				if (newcmd.func == EnumExtFunc::movel or newcmd.func == EnumExtFunc::movel_d) {
					cmd.v = state->tcpV;
					cmd.a = state->tcpA;
				}
				else {
					cmd.v = state->jointV;
					cmd.a = state->jointA;
				}
			}

			cmd.tool = newcmd.tool;
			cmd.wobj = newcmd.wobj;
			cmd.status = EnumExecuteStatus::notstart;

			cmd.toolId = newcmd.toolId;
			cmd.toolAction = newcmd.toolAction;

			return curCmdId;
		}

		/**
		 * @创建人 dns
		 * @简介 获取指令状态
		 * @返回值 (指令id,指令状态)
		 */
		std::tuple<int, int> Cmd::getCmdStatus() {
			return std::tuple<int, int>(cmd.id,cmd.status);
		}


		/**
		 * @创建人 dns
		 * @简介 近指令设置为"没有指令"
		 */
		void Cmd::reset() {
			cmd.id = -1;
		}
	}
}