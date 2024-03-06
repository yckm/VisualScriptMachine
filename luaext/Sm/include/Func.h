#pragma once
/*****************************************************************
 * @文件名称:Func.h
 * @创建人:dns
 * @创建日期:2024 February 
 * @简述: 系统函数
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
		class Func
		{
		private:
			int paramNum; // 参数个数
			EnumExtFunc func; // 函数类型
			std::string funcName; // 函数名称
			int lineno; // 行号
			lua_State* lua; // lua状态
			Channel* channel; // 通道
			int channelId;
		public:
			Func(lua_State* lua, EnumExtFunc func, int paramNum);


			/**
			 * @创建人 dns
			 * @简介 验证函数参数
			 * @返回值 是否验证通过 true 通过 ; false 未通过
			 */
			bool validate();

			/**
			 * @创建人 dns
			 * @简介 验证prst状态
			 * @返回值
			 */
			bool checkPrst();

			/**
			 * @创建人 dns
			 * @简介 运行函数
			 * @参数 cmd 待执行的指令
			 * @参数 enableDebug 是否允许调试
			 * @返回值 是否成功运行
			 */
			bool start(CmdModel cmd);


			/**
			 * @创建人 dns
			 * @简介 获取通道
			 * @返回值 通道
			 */
			Channel* getChannel();

			/**
			 * @创建人 dns
			 * @简介 是否是Prst::normal
			 * @返回值  
			 */
			bool isPrstNormal();

			/**
			 * @创建人 dns
			 * @简介 获取函数所在行
			 * @返回值 行号
			 */
			int getLine();
		};
	}
}

