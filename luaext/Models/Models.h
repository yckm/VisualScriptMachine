#pragma once

#include <vector>
#include "Enums.h"

namespace Pcs {
	namespace Model {
#pragma region 系统类型模板类
		template <class T>
		class PValue {
		private:
			T v0;
			T v1;
			int _idx = 0;

		public:
			PValue(){}
			PValue(T t) {
				v0 = t;
			}

			/**
			 * @创建人 dnp
			 * @简介 重载赋值
			 * @参数 value 值
			 * @返回值 
			 */
			const void operator=(const T& value) {
				if (_idx == 0) {
					v1 = value;
					_idx = 1;
				}
				else {
					v0 = value;
					_idx = 0;
				}
			}

			

			/**
			 * @创建人 dnp
			 * @简介 更新值
			 * @参数 value 最新值
			 */
			const void operator=(PValue& value) {
				if (_idx == 0) {
					v1 = value.val();
					_idx = 1;
				}
				else {
					v0 = value.val();
					_idx = 0;
				}
			}



			/**
			 * @创建人 dnp
			 * @简介 获取最新值
			 * @返回值  最新值
			 */
			const T& val() {
				return _idx == 0 ? v0 : v1;
			}
		};
#pragma endregion

		/**
		 * @创建人 dnp
		 * @简介  机械臂状态
		 */
		struct BotStateModel {
			int robState; // 机器人状态
			int robTaskState; // 机器人任务状态
			int safetyState; // 机器人安全控制器状态
			std::vector<double> tcp={ 0,0,0,0,0,0 };// 当前工具末端位置
			std::vector<double> joints= { 0,0,0,0,0,0 };// 当前关键角度
			long ts;// 数据更新时间戳(毫秒)
		};

		/**
		 * @创建人 dnp
		 * @简介 通道状态
		 */
		struct ChannelStateModel {
			int channel; // 通道号
			PValue<std::string> tool= PValue<std::string>("default"); // 当前工具
			PValue<std::string> wobj=PValue<std::string>("default"); // 当前坐标系
			int jointV =90; // 关节速率比 30%
			int tcpV = 100; // 末端速度100mm/s
			double jointA = 30; // 关节加速度比
			double tcpA = 20;// 末端加速度 mm/s2
			PValue< std::string> scriptLog;// 脚本日志
		};

		/**
		 * @创建人 dnp
		 * @简介 指令模型
		 */
		struct CmdModel {
			long id = 0; // 指令 ID
			int channel;/// 通道ID
			int func = EnumExtFunc::none;  // EnumExtFunc
			PValue< std::vector<double>> params  = PValue<std::vector<double>>(std::vector<double>(6, 0)); // 运行参数
			double v=30;  // 速度 
			double a=100;  // 加速度
			PValue< std::string> tool = PValue < std::string>("default");  // 工具
			PValue< std::string> wobj = PValue < std::string>("default"); // 工件坐标系
			EnumExecuteStatus status = EnumExecuteStatus::notstart;// 指令状态
			int toolId = -1; // 工具ID
			int toolAction = -1; // 工具动作
		};

		/**
		 * @创建人 dnp
		 * @简介 脚本模型
		 */
		struct ScriptModel
		{
			int channel; // 所属通道
			long id;// 脚本ID(采用时间戳)
			PValue<std::string> content=PValue<std::string>(""); // 脚本内容
			bool hasErr = false; // 脚本是否包含错误
			PValue<std::string>  errMsg;// 错误消息
			EnumPrst prst; // 当前脚本的Prst
			EnumExecuteStatus status = EnumExecuteStatus::notstart;// 脚本运行状态
			PValue< std::string> funcName = PValue < std::string>("");  // 正在执行的函数名称
			std::vector<std::string> scriptRows; // 将原始脚本通过换行符分割后的,用于日志显示用
		};
	}
}