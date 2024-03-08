#pragma once
#include <iostream>
#include "../../Models/Enums.h"
#include "../../Models/Models.h"
#include "Utils.h"
#include "Script.h"
#include "Cmd.h"
#include "BotStates.h"
#include "Debug.h"

namespace Pcs {
	namespace Sm {
		using namespace Model;
		class Channel
		{
		private:
			Script* _script; // 脚本
			Cmd* _cmd;// 控制指令
			BotStates* _botState;// 机器人状态
			ChannelStateModel* _state;// 通道状态
			Debug* _debug;// 调试信息
			int channelId;// 通道id			

		public:
			Channel(int channelId);

			/**
			 * @创建人 dnp
			 * @简介 获取当前通道ID
			 * @返回值  通道ID
			 */
			int getChannelId();

#pragma region 调试
			/**
			* @创建人 dnp
			* @简介 设置是否继续
			* @参数 canContinue 是否能继续(下一步)
			*/
			void setContinue(bool canContinue);

			/**
			 * @创建人 dnp
			 * @简介 是否能够继续执行
			 * @返回值 true 继续/ false 等待
			 */
			bool canContine();

			/**
			 * @创建人 dnp
			 * @简介 是否是调试模式
			 */
			bool isDebug();

			/**
			 * @创建人 dnp
			 * @简介 是否启动调试模式
			 * @参数 enable true 启用调试/false 禁用调试
			 * @返回值 是否操作成功
			 */
			bool setDebugMode(bool enable);


			/**
			 * @创建人 dnp
			 * @简介 下一个断点
			 * @返回值
			 */
			bool nextBreakpoint();

			/**
			 * @创建人 dnp
			 * @简介  下一行(如果下一行未非系统函数则无效)
			 * @参数 channel 通道ID
			 * @返回值 是否设置成功
			 */
			bool toContinue();

			/**
			 * @创建人 dnp
			 * @简介 获取当前指令所在的行
			 * @返回值  行号
			 */
			std::tuple<int, bool> getDebugInfo();

			/**
			 * @创建人 dnp
			 * @简介 记录当前行号
			 * @参数 line 当前行号
			 */
			void logCurLine(int line);


			/**
			 * @创建人 dnp
			 * @简介 设置断点
			 * @参数 linenos 断点所在行号列表
			 * @返回值 是否操作从成功
			 */
			bool setBreakpoints(std::vector<int> linenos);

			/**
			 * @创建人 dnp
			 * @简介 获取指定行号的代码.
			 * @参数 rowno 行号
			 * @返回值 该行的代码
			 */
			std::string getScriptRow(int rowno);
#pragma endregion

#pragma region _script

			/**
			 * @创建人 dnp
			 * @简介 设置错误信息
			 * @参数 hasErr 是否包含错误
			 * @参数 err 错误信息
			 */
			void setErr(bool hasErr,std::string err);

			/**
			 * @创建人 dnp
			 * @简介 获取脚本状态
			 * @返回值 (脚本Id,脚本状态)
			 */
			int getScriptId();

			/**
			 * @创建人 dnp
			 * @简介 是否是新脚本
			 * @返回值 
			 */
			bool isNewScript();

			/**
			 * @创建人 dnp
			 * @简介 获取脚本
			 * @返回值 脚本实例
			 */
			ScriptModel getScript();

			/**
			* @创建人 dnp
			* @简介 设置脚本状态
			* @参数 stauts 脚本状态
			*/
			void setScriptStatus(EnumExecuteStatus status);

			/**
			* @创建人 dnp
			* @简介执行脚本。脚本遵循本文档规范。该函数调用后, 设置  脚本 共享变量。
			* @参数 channel 通道 ID。
			* @参数 _script 脚本内容
			* @返回值 < 脚本ID, 错误信息>
			*/
			std::tuple<int,std::string> updateScript(std::string s);

			/**
			 * @创建人 dnp
			 * @简介 设置prst值
			 * @参数 signal PRST的值
			 * @返回值 是否设置成功
			 */
			bool setPrst(EnumPrst signal);

			/**
			 * @创建人 dnp
			 * @简介 LC设置(全局)关键速率 (非Nc全局速率,Nc那边的有plc设定,这里设定作用是为运动控制函数提供参数, set_tail_speed同理)
			 * @参数 v 关键速率 [1,100]
			 * @参数 a 关节角加速度，单位（系统设定加速度的百分比%），取值范围（0,100]。
			 * @返回值
			 */
			bool setJointsSpeed(int v, int a);

			/**
			 * @创建人 dnp
			 * @简介 PLC设置(全局)末端速度
			 * @参数 v 末端速度 mm/s
			 * @参数 a 末端加速度，单位（mm/s2）。
			 * @返回值  是否设置成功
			 */
			bool setTailSpeed(double v, double a);

			/**
			 * @创建人 dnp
			 * @简介 PLC获取通道状态
			 * @参数 channel 通道 ID。
			 * @返回值  ChannelStateModel实例
			 */
			ChannelStateModel* getChannelState();

			/**
			 * @创建人 dnp
			 * @简介 获取psrt值
			 * @返回值 psrt
			 */
			EnumPrst getPrst();
			/**
			 * @创建人 dnp
			 * @简介 创建或更新工件坐标系
			 * @参数 name 坐标系名称
			 * @参数 offset 坐标系偏移
			 * @返回值  是否设置成功
			 */
			bool updateWobj(std::string name, std::vector<double> offset);

			/**
			 * @创建人 dnp
			 * @简介 创建或更新工件末端参数
			 * @参数 name 工具名称
			 * @参数 offset 工具偏移
			 * @参数 shape 工具形状
			 * @返回值 是否设置成功
			 */
			bool updateTool(std::string name, std::vector<double> offset, std::vector<double> shape);

			/**
			 * @创建人 dnp
			 * @简介 设置当前使工具名称
			 * @参数 name 工具名称
			 * @返回值 是否设置成功
			 */
			bool useTool(std::string name);

			/**
			 * @创建人 dnp
			 * @简介 设置当前坐标系
			 * @参数 name 坐标系名称
			 * @返回值 是否设置成功
			 */
			bool useWobj(std::string name);

			/**
			 * @创建人 dnp
			 * @简介 获取工件坐标系名称.
			 * @返回值 
			 */
			std::string getWobjName();

			/**
			 * @创建人 dnp
			 * @简介 获取工件坐标系名称.
			 * @返回值 
			 */
			std::string getToolName();
#pragma endregion

#pragma region 外部机器人状态
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
			bool updateBotStatus(std::vector<double> joints, std::vector<double> tcps, int robState, int robTaskState, int safetyState);
			/**
			 * @创建人 dnp
			 * @简介 内部获取外部机械臂状态
			 * @返回值  机械臂状态
			 */
			BotStateModel getBotStatus();
#pragma endregion

#pragma region 设置指令
			/**
			 * @创建人 dnp
			 * @简介 PLC查询控制指令
			 * @参数 channel 通道 ID。
			 * @返回值 控制指令
			 */
			CmdModel getCmd();

			/**
			 * @创建人 dnp
			 * @简介 PLC更新指令状态
			 * @参数 cmdId 控制指令ID
			 * @参数 status 指令目标状态
			 * @返回值 是否设置成功
			 */
			bool setCmdStatus(int cmdId,EnumExecuteStatus status);

			/**
			 * @创建人 dnp
			 * @简介 executor更新指令
			 * @参数 _cmd 新指令
			 * @返回值 是否设置成功
			 */
			int updateCmd(CmdModel c);

			/**
			 * @创建人 dnp
			 * @简介 获取指令状态
			 * @返回值 (指令id,指令状态)
			 */
			std::tuple<int, int> getCmdStatus();

			/**
			 * @创建人 dnp
			 * @简介 记录脚本日志内容
			 * @参数 lg 日志内容
			 */
			void setLog(std::string lg);


			/**
			 * @创建人 dnp
			 * @简介 获取脚本执行到的行
			 * @返回值 行号
			 */
			int getCurLine();
#pragma endregion
		};
	}
}