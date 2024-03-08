#pragma once
/*****************************************************************
 * @文件名称:PcsExecutor.h
 * @创建人:dnp
 * @创建日期:2024 February
 * @简述: PCSExecutor 脚本机执行模块, 该模块只是一个简单的循环。在脚本执行过程中,脚本机的控制交由LuaExtFunc进行.当没有脚本在执行的时候,控制由PcsExecutor接管。
*******************************************************************/#pragma once
extern "C" {
#include "../../lua/lua.h"
#include "../../lua/lualib.h"
#include "../../lua/lauxlib.h"
#include "../../lua/lua.hpp"
}
#include "Channel.h"

namespace Pcs {
	namespace Sm {
		class PcsExecutor
		{
		protected:
			int channelId; // 绑定通道ID
			int curScriptId = 0;// 当前脚本ID
			Channel* channel = nullptr; // 通道
			lua_State* lua = nullptr; // lua虚拟机
		public:
			/**
			 * @创建人 dnp
			 * @简介 启动通道
			 * @参数 channelId 通道ID
			 */
			void Start(int channelId);

		private:
			/**
			 * @创建人 dnp
			 * @简介 执行脚本
			 */
			void executeScript();

			/**
			 * @创建人 dnp
			 * @简介 初始化lua虚拟机并注册函数
			 * @返回值 lua虚拟机
			 */
			void initLua();
		};
	}
}