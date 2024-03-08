#pragma once
#include <iostream>
#include "../../Models/Enums.h"
#include "../../Models/Models.h"

namespace Pcs {
	namespace Sm {
		using namespace Model;
		class Cmd {
		private:
			int curCmdId = 0;
			int channelId;
			CmdModel  cmd;
		public:
			Cmd(int channelId);
			/**
			 * @创建人 dnp
			 * @简介 PLC查询控制指令
			 * @参数 channel 通道 ID。
			 * @返回值 控制指令
			 */
			const CmdModel& getCmd();

			/**
			 * @创建人 dnp
			 * @简介 PLC更新指令状态
			 * @参数 status 指令目标状态
			 * @返回值 是否设置成功
			 */
			bool setStatus(EnumExecuteStatus status);

			/**
			 * @创建人 dnp
			 * @简介 executor更新指令
			 * @参数 cmd 新指令
			 * @返回值 是否设置成功
			 */
			int update(CmdModel newcmd, ChannelStateModel* state);

			/**
			 * @创建人 dnp
			 * @简介 获取指令状态
			 * @返回值 (指令id,指令状态)
			 */
			std::tuple<int, int> getCmdStatus();

			/**
			 * @创建人 dnp
			 * @简介 近指令设置为"没有指令"
			 */
			void reset();
		};
	}
}
