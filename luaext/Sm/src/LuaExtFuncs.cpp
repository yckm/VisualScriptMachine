/*****************************************************************
 * @文件名称:LuaExtFuncs.h
 * @创建人:dnp
 * @创建日期:2024 February
 * @简述: 脚本机系统函数
*******************************************************************/
#include "../include/LuaExtFuncs.h"
#include "../include/Func.h"
#include <string>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <vector>
#include "../include/log.h"
#include "../include/Signals.h"

namespace Pcs {
	namespace Sm {
		/**
		 * @创建人 dnp
		 * @简介 获取channelId
		 * @参数 lua lua状态机
		 * @返回值 通道ID
		 */
		static int getChId(lua_State* lua) {
			lua_getglobal(lua, "channel"); // 获取通道ID
			int channelId = lua_tonumber(lua, -1);
			return channelId;
		}
#pragma region share
		/**
		 * @创建人 dnp
		 * @简介 获取lua传给c++的double数组
		 * @参数 lua状态
		 * @参数 paramIndexInLua 数组在栈中的位置
		 * @返回值 double数组
		 */
		static std::vector<double> lua2arrd(lua_State* lua, int paramIndexInLua) {
			std::vector<double> arr;
			luaL_checktype(lua, paramIndexInLua, LUA_TTABLE);

			lua_pushnil(lua);
			while (lua_next(lua, paramIndexInLua) != 0) {
				arr.push_back(lua_tonumber(lua, -1));
				lua_pop(lua, 1);
			}
			return arr;
		}

		// Function to convert a Lua table to a string
		static std::string ConvertTableToString(lua_State* lua, int tableIndex) {
			std::string result;

			// Check if the value at 'tableIndex' is a table
			if (lua_istable(lua, tableIndex)) {
				lua_pushnil(lua); // Push a nil key to start the traversal
				while (lua_next(lua, tableIndex) != 0) {
					// Get the key and value
					int keyType = lua_type(lua, -2);
					int valueType = lua_type(lua, -1);

					// Handle different value types (e.g., integer, string, etc.)
					if (valueType == LUA_TNUMBER) {
						result += std::to_string(lua_tonumber(lua, -1));
					}
					else if (valueType == LUA_TSTRING) {
						result += lua_tostring(lua, -1);
					}
					else if (valueType == LUA_TTABLE) {
						// Recursively handle nested tables
						result += ConvertTableToString(lua, lua_gettop(lua));
					}

					// Add a separator (e.g., comma) between key-value pairs
					result += ", ";

					// Pop the1 value, but keep the key for the next iteration
					lua_pop(lua, 1);
				}
			}

			return result;
		}

#pragma endregion

#pragma region 影响到机械臂的操纵都需要进行 "设置指令-等待治理状态"的操作


		

		/**
		 * @创建人 dnp
		 * @简介 执行运动函数
		 * @参数 lua lua状态
		 * @参数 func 运动函数
		 * @返回值
		 */
		static int runMoveFunc(lua_State* lua, EnumExtFunc func,std::string funcName) {
			Func fn(lua, func, 1);
			if (!fn.validate()) {
				return 0;
			}

			std::vector<double> params = lua2arrd(lua, 1);
			//Log <<"line:" << fn.getLine() << "【before " <<  funcName << "" << Utils::DoubleArr2String(params) << " 】";

			if (params.size() != 6) {
				std::string err = funcName + " 参数错误";
				lua_pushstring(lua, err.c_str());
				lua_error(lua);
				return 0; // 退出脚本执行
			}

			CmdModel cmd;
			cmd.id = 0;
			cmd.func = func;
			cmd.params = params;
			cmd.wobj = fn.getChannel()->getWobjName();
			cmd.tool = fn.getChannel()->getToolName();
			
			fn.start(cmd);
			return 0;
		}

#pragma endregion

#pragma region 实现
		/**
		 * @创建人 dnp
		 * @简介 等待手动介入, 完成手动操作后函数返回。如果设置了超时时间, 即使手动介入未结束函数也返回。
		 * @参数 lua lua虚拟机
		 * @返回值 返回参数个数
		 */
		int LuaExtFuncs::wait(lua_State* lua) {
			Func fn(lua, EnumExtFunc::none, 1);
			if (!fn.validate()) {
				return 0;
			}
			Log << "line:" << fn.getLine() << " wait()";

			long st = Utils::GetTsMs();
			int s = lua_tonumber(lua, 1);

			Channel* channel = fn.getChannel();
			channel->setPrst(EnumPrst::puase); // 通过prst暂停方式实现等待

			// 进行等待
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(EPOLLTIME)); // 睡眠10ms
				if (s > 0 and Utils::GetTsMs() - st > s * 1000) {
					channel->setPrst(EnumPrst::normal);
					return 0;
				}

				if (fn.isPrstNormal()) {
					channel->setPrst(EnumPrst::normal);
					return 0;
				}

				if (!fn.checkPrst()) {
					return 1; // 检查prsk
				}

			}

			return 0;
		}

		/**
		 * @创建人 dnp
		 * @简介 获取关节角度列表。
		 * @参数 lua lua虚拟机
		 * @返回值 返回参数个数
		 */
		int LuaExtFuncs::get_joints(lua_State* lua) {
			Func fn(lua, EnumExtFunc::none, 0);
			if (!fn.validate()) {
				return 0;
			}
			Log << "line:" << fn.getLine() << " get_joints()";

			BotStateModel botState = fn.getChannel()->getBotStatus();

			// 将数组压入Lua栈
			lua_newtable(lua);
			for (size_t i = 0; i < 6; ++i) {
				lua_pushinteger(lua, i + 1); // 压入表的key值（从1开始）
				lua_pushinteger(lua, botState.joints[i]); // 压入key对应的value值
				lua_settable(lua, -3); // 设置表中的元素
			}
			return 1;
		}

		/**
		 * @创建人 dnp
		 * @简介 获取当前 PT 的位置和姿态。
		 * @参数 lua lua虚拟机
		 * @返回值 返回参数个数
		 */
		int LuaExtFuncs::get_tcp(lua_State* lua) {
			Func fn(lua, EnumExtFunc::none, 0);
			if (!fn.validate()) {
				return 0;
			}
			Log << "line:" << fn.getLine() << " get_tcp()";
			BotStateModel botState = fn.getChannel()->getBotStatus();

			// 将数组压入Lua栈
			try {
				lua_newtable(lua);
				for (size_t i = 0; i < 6; ++i) {
					lua_pushinteger(lua, i+1);
					lua_pushnumber(lua, botState.tcp[i]);
					lua_settable(lua, -3);
				}
			}
			catch (std::exception e) {
				std::cout << e.what() << std::endl;
			}
			
			return 1;
		}

		/**
		 * @创建人 dnp
		 * @简介 获取机器人的状态,包括 ﻿ 机器人状态 ， 任务状态 ﻿、﻿ 碰撞状态 ﻿
		 * @参数 lua lua虚拟机
		 * @返回值 返回参数个数
		 */
		int LuaExtFuncs::get_state(lua_State* lua) {
			Func fn(lua, EnumExtFunc::none, 0);
			if (!fn.validate()) {
				return 0;
			}
			Log << "line:" << fn.getLine() << " get_state()";
			BotStateModel botState =fn.getChannel()->getBotStatus();
			std::vector<int> states = { botState.robState,botState.robTaskState,botState.safetyState };

			// 将数组压入Lua栈
			lua_newtable(lua);
			for (size_t i = 0; i < 3; ++i) {
				lua_pushinteger(lua, i + 1); // 压入表的key值（从1开始）
				lua_pushnumber(lua, states[i]); // 压入key对应的value值
				lua_settable(lua, -3); // 设置表中的元素
			}
			return 1;
		}

#pragma region 运动设置函数

		/**
		 * @创建人 dnp
		 * @简介 创建或更新工件坐标系。
		 * @参数 lua lua虚拟机
		 * @返回值 返回参数个数
		 */
		int LuaExtFuncs::update_wobj(lua_State* lua) {
			Func fn(lua, EnumExtFunc::tool, 2);
			if (!fn.validate()) {
				return 0;
			}

			const char* name = lua_tostring(lua, 1);
			std::vector<double> offset = lua2arrd(lua, 2);

			CmdModel cmd{};
			cmd.func = EnumExtFunc::update_wobj;
			cmd.params = offset;
			
			fn.start(cmd);
			fn.getChannel()->useWobj(name);
			return true;
		}

		/**
		 * @创建人 dnp
		 * @简介 设置当前使用的 PT。
		 * @参数 lua lua虚拟机
		 * @返回值 返回参数个数
		 */
		int LuaExtFuncs::use_tool(lua_State* lua) {			
			Func fn(lua, EnumExtFunc::tool, 1);
			if (!fn.validate()) {
				return 0;
			}

			std::string toolName = lua_tostring(lua, 1);

			CmdModel cmd{};
			cmd.func = EnumExtFunc::use_tool;
			cmd.tool = toolName;
			fn.start(cmd);

			fn.getChannel()->useTool(toolName);
			return 0;
		}

		/**
		 * @创建人 dnp
		 * @简介 设置当前使用的坐标。
		 * @参数 lua lua虚拟机
		 * @返回值 返回参数个数
		 */
		int LuaExtFuncs::use_wobj(lua_State* lua) {		
			Func fn(lua, EnumExtFunc::use_wobj, 1);
			if (!fn.validate()) {
				return 0;
			}

			std::string wobjName = lua_tostring(lua, 1);

			CmdModel cmd{};
			cmd.func = EnumExtFunc::use_wobj;
			cmd.wobj = wobjName;
			
			fn.start(cmd);

			fn.getChannel()->useWobj(wobjName);
			return 0;
		}

		/**
		 * @创建人 dnp
		 * @简介 设置全局关节角速度倍率。对关节运动函数生效 。关节角速度倍率，单位（系统设定速度的百分比%），取值范围（0,100]。
		 * @参数 lua lua虚拟机
		 * @返回值 返回参数个数
		 */
		int LuaExtFuncs::set_joints_speed(lua_State* lua) {
			Func fn(lua, EnumExtFunc::none, 2);
			if (!fn.validate()) {
				return 0;
			}
			Log << "line:" << fn.getLine() << " set_joints_speed()";
			int v = lua_tonumber(lua, 1);
			int a = lua_tonumber(lua, 2);
			fn.getChannel()->setJointsSpeed(v, a);
			return 0;
		}

		/**
		 * @创建人 dnp
		 * @简介 设定 PT 直线运动速度。 对关直线动函数生效。v﻿：PT 直线运动速度,单位 mm/s。
		 * @参数 lua lua虚拟机
		 * @返回值 返回参数个数
		 */
		int LuaExtFuncs::set_tail_speed(lua_State* lua) {
			Func fn(lua, EnumExtFunc::none, 2);
			if (!fn.validate()) {
				return 0;
			}
			Log << "line:" << fn.getLine() << " set_tail_speed()";
			double v = lua_tonumber(lua, 1);
			double a = lua_tonumber(lua, 2);
			fn.getChannel()->setTailSpeed(v, a);
			return 0;
		}

		/**
		 * @创建人 dnp
		 * @简介 终止械臂运动并退出当前脚本。
		 * @参数 lua lua虚拟机
		 * @返回值 返回参数个数
		 */
		int LuaExtFuncs::exit(lua_State* lua) {
			Log << "exit()";
			lua_pushstring(lua, "正常退出");
			lua_error(lua);
			return 0;
		}

		/**
		 * @创建人 dnp
		 * @简介 控制机械臂从当前状态，按照关节运动的方式移动到 ﻿ 目标关节角 ﻿ 状态。范围 [-360°，360°]。
		 * @参数 lua lua虚拟机
		 * @返回值 返回参数个数
		 */
		int LuaExtFuncs::movej(lua_State* lua) {
			return runMoveFunc(lua, EnumExtFunc::movej,"movej");
		}


		/**
		 * @创建人 dnp
		 * @简介 在当前坐标系下, 控制机械臂从当前状态，按照关节运动的方式移动到 PT 目标位置 。
		 * @参数 lua lua虚拟机
		 * @返回值 返回参数个数
		 */
		int LuaExtFuncs::movep(lua_State* lua) {
			return runMoveFunc(lua, EnumExtFunc::movep,"movep");
		}

		/**
		* @创建人 dnp
		* @简介 按tcp进行运动
		*/
		int LuaExtFuncs::move_tcp(lua_State* lua)
		{
			return runMoveFunc(lua, EnumExtFunc::move_tcp, "move_tcp");
		}

		/**
		 * @创建人 dnp
		 * @简介 在当前坐标系下, 将 PT按直线运动到指定位姿。
		 * @参数 lua lua虚拟机
		 * @返回值 返回参数个数
		 */
		int LuaExtFuncs::movel(lua_State* lua) {
			return runMoveFunc(lua, EnumExtFunc::movel,"moveL");
		}

		/**
		 * @创建人 dnp
		 * @简介 控制机械臂从当前状态，按照关节运动的方式移动一个增量。
		 * @参数 lua lua虚拟机
		 * @返回值 返回参数个数
		 */
		int LuaExtFuncs::movej_d(lua_State* lua) {
			return runMoveFunc(lua, EnumExtFunc::movej_d,"movej_d");
		}

		/**
		 * @创建人 dnp
		 * @简介 在当前坐标系下, 控制机械臂从当前状态，按照关节运动的方式将 PT 移动一个增量。
		 * @参数 lua lua虚拟机
		 * @返回值 返回参数个数
		 */
		int LuaExtFuncs::movep_d(lua_State* lua) {
			return runMoveFunc(lua, EnumExtFunc::movep_d,"movep_d");
		}

		/**
		 * @创建人 dnp
		 * @简介 在当前坐标系下, 将 PT按直线运动一个增量。
		 * @参数 lua lua虚拟机
		 * @返回值 返回参数个数
		 */
		int LuaExtFuncs::movel_d(lua_State* lua) {
			return runMoveFunc(lua, EnumExtFunc::movel_d,"moveL_d");
		}
#pragma endregion

		/**
		* @创建人 dnp
		* @简介 控制工具执行指定动作 .该函数不直接控制工具, 而是设置  共享工具变量 (3.2.2) 。
		* @参数 lua lua虚拟机
		* @返回值 返回参数个数
		*/
		int LuaExtFuncs::tool(lua_State* lua) {
			Func fn(lua, EnumExtFunc::tool, 2);
			if (!fn.validate()) {
				return 0;
			}

			int tool = lua_tointeger(lua, 1);
			int action = lua_tointeger(lua, 2);
			CmdModel cmd{};
			cmd.func = EnumExtFunc::tool;
			cmd.toolId= tool;
			cmd.toolAction = action;
			
			fn.start(cmd);
			return 0;
		}


		/**
		* @创建人 dnp
		* @简介 角度转弧度
		* @参数 lua lua虚拟机
		* @返回值 返回参数格式
		*/
		int LuaExtFuncs::torad(lua_State* lua) {
			Func fn(lua, EnumExtFunc::none, 1);
			if (!fn.validate()) {
				return 0;
			}

			int degree = lua_tonumber(lua, 1);
			double v = degree * 3.14159265358979 / 180.0;
			lua_pushnumber(lua,v);
			return 1;
		}

		/**
		* @创建人 dnp
		* @简介 日志记录
		* @参数 lua lua虚拟机
		* @返回值 返回参数格式
		*/
		int LuaExtFuncs::luaLog(lua_State* lua) {
			Func fn(lua, EnumExtFunc::none, 1);
			if (!fn.validate()) {
				return 0;
			}

			std::string lg ;
			switch (lua_type(lua, 1))
			{
			case LUA_TTABLE:
				lg = "{"+ ConvertTableToString(lua, 1)+"}";
				break;
			case LUA_TNUMBER:
				lg = lua_tostring(lua, 1);
				break;
			case LUA_TSTRING:
				lg = lua_tostring(lua, 1);
				break;
			case LUA_TBOOLEAN:
				lg =lua_toboolean(lua, 1)==1?"true":"false";
				break;
			default:
				lg = "暂不支持该类型";
				break;
			}		

			lg ="line:"+ std::to_string(fn.getLine()) + "  " + lg;
			fn.getChannel()->setLog(lg);
			Log << lg;			
			return 0;
		}
#pragma endregion

#pragma region 信号量
		/**
		* @创建人 dnp
		* @简介 发送通道信号
		*/
		int LuaExtFuncs::send_signal(lua_State* lua)
		{
			Func fn(lua, EnumExtFunc::none, 2);
			if (!fn.validate()) {
				return 0;
			}		

			const char* key = lua_tostring(lua, 1);
			std::string k = key;
			int idx = std::stoi(k.substr(1));
			int signal = lua_tointeger(lua, 2);

			if (!Signals::send(idx, signal)) {
				lua_pushstring(lua, "信号量名称错误");
				lua_error(lua);
				return 1;
			}

			lua_pushstring(lua, "Success");
			return 1;
		}

		/**
		* @创建人 dnp
		* @简介 等待通道信号
		*/
		int LuaExtFuncs::wait_signal(lua_State* lua)
		{
			Func fn(lua, EnumExtFunc::none, 2);
			if (!fn.validate()) {
				return 0;
			}

			const char* key = lua_tostring(lua, 1);
			std::string k = key;
			int idx = std::stoi(k.substr(1));
			int timeout = lua_tointeger(lua, 2); // 超时时间

			long st = Utils::GetTsMs();
			int signal = 0;

			// 进行等待
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(EPOLLTIME)); // 睡眠10ms
				if (timeout > 0 and Utils::GetTsMs() - st > timeout * 1000) {
					lua_pushinteger(lua, 0); 
					return 1;
				}

				signal = Signals::get(idx);
				if (signal > 0) {
					lua_pushinteger(lua, signal);
					return 1;
				}

				if (!fn.checkPrst()) {
					lua_pushinteger(lua, 0); 
					return 1; // 检查prsk
				}

			}
			lua_pushinteger(lua, 0); 
			return 1;
		}
#pragma endregion
		/**
		* @创建人 dnp
		* @简介 当fixparams发生变化时候更新lua状态中的变量.
		* @返回值 是否发生了变化
		*/
		bool LuaExtFuncs::updateFixParams(Channel* channel,lua_State* lua)
		{
			if (!channel->prePositionChanged()) {
				return false;
			}
			D6Param d6 = channel->getPrePosition();

			std::string name = "P" + std::to_string(d6.idx);
			// 获取要更新的 table。
			lua_getglobal(lua, name.c_str());

			// 创建一个新的 Lua 表。
			lua_newtable(lua);

			// 将要设置的值压入堆栈。
			for (int i = 0; i < 6; i++) {
				lua_pushnumber(lua, d6.params[i]);
			}

			// 将表压入堆栈。
			lua_pushvalue(lua, -1);

			// 调用 lua_settable() 函数。
			lua_settable(lua, -3);
			return true;
		}
	}
}