#pragma once
/*****************************************************************
 * @文件名称:LuaExtFuncs.h
 * @创建人:dnp
 * @创建日期:2024 February
 * @简述: 脚本机系统函数
*******************************************************************/
#include <string>
extern "C" {
	#include "../../lua/lua.h"
	#include "../../lua/lualib.h"
	#include "../../lua/lauxlib.h"
	#include "../../lua/lua.hpp"
}
#include "Channels.h"
namespace Pcs {
	namespace Sm {
		class LuaExtFuncs
		{
		public:
			/**
			 * @创建人 dnp
			 * @简介 等待手动介入, 完成手动操作后函数返回。如果设置了超时时间, 即使手动介入未结束函数也返回。
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数个数
			 */
			static int wait(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 获取关节角度列表。
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数个数
			 */
			static int get_joints(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 获取当前 PT 的位置和姿态。
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数个数
			 */
			static int get_tcp(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 获取机器人的状态,包括 ﻿ 机器人状态 ， 任务状态 ﻿、﻿ 碰撞状态 ﻿
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数个数
			 */
			static int get_state(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 创建或更新工件坐标系。
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数个数
			 */
			static int update_wobj(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 设置当前使用的 PT。
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数个数
			 */
			static int use_tool(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 设置当前使用的坐标。
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数个数
			 */
			static int use_wobj(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 设置全局关节角速度倍率。对关节运动函数生效 。关节角速度倍率，单位（系统设定速度的百分比%），取值范围（0,100]。
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数个数
			 */
			static int set_joints_speed(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 设定 PT 直线运动速度。 对关直线动函数生效。v﻿：PT 直线运动速度,单位 mm/s。
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数个数
			 */
			static int set_tail_speed(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 终止械臂运动并退出当前脚本。
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数个数
			 */
			static int exit(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 控制机械臂从当前状态，按照关节运动的方式移动到 ﻿ 目标关节角 ﻿ 状态。范围 [-360°，360°]。
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数个数
			 */
			static int movej(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 在当前坐标系下, 控制机械臂从当前状态，按照关节运动的方式移动到 PT 目标位置 。
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数个数
			 */
			static int movep(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 在当前坐标系下, 将 PT按直线运动到指定位姿。
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数个数
			 */
			static int movel(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 控制机械臂从当前状态，按照关节运动的方式移动一个增量。
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数个数
			 */
			static int movej_d(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 在当前坐标系下, 控制机械臂从当前状态，按照关节运动的方式将 PT 移动一个增量。
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数个数
			 */
			static int movep_d(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 在当前坐标系下, 将 PT按直线运动一个增量。
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数个数
			 */
			static int movel_d(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 控制工具执行指定动作 .该函数不直接控制工具, 而是设置  共享工具变量 (3.2.2) 。
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数个数
			 */
			static int tool(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 角度转弧度
			 * @参数 lua lua虚拟机
			 * @返回值 返回参数格式
			 */
			static int torad(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 日志记录
			 */
			static int luaLog(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 按tcp进行运动
			 */
			static int move_tcp(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 发送通道信号
			 */
			static int send_signal(lua_State* lua);

			/**
			 * @创建人 dnp
			 * @简介 等待通道信号
			 */
			static int wait_signal(lua_State* lua);

		};
	}
}
