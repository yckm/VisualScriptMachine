#include "../include/Func.h";
#include "../include/log.h"

namespace Pcs {
	namespace Sm {
		using namespace Model;

#pragma region 局部函数
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

		/**
		 * @创建人 dnp
		 * @简介 获取当前函数的执行信息
		 * @参数 lua  lua状态
		 * @返回值 (当前执行函数所在的行号,函数名称)
		 */
		static std::tuple<int, std::string> _getDebugInfo(lua_State* lua) {
			int lineno = 0;
			std::string func = "";

			int level = 0;
			lua_Debug ar;

			while (lua_getstack(lua, level, &ar)) {
				lua_getinfo(lua, "nSl", &ar);
				if (ar.what == "main") {
					lineno = ar.currentline;
				}
				if (ar.what == "C") {
					func = ar.name;
				}
				level++;
			}

			return std::tuple<int, std::string>(lineno, func);
		}
#pragma endregion


		Func::Func(lua_State* lua, EnumExtFunc func, int paramNum)
		{
			this->func = func;			
			this->paramNum = paramNum;
			this->lua = lua;

			lua_getglobal(lua, "channel"); // 获取通道ID
			channelId = lua_tonumber(lua, -1);
			lua_pop(lua,1);
			this->channel = Channels::getChannel(channelId);		
			std::tuple<int, std::string> lineName = _getDebugInfo(lua);		
			this->lineno = std::get<0>(lineName);
			this->funcName = std::get<1>(lineName);

			channel->setContinue(false); // 设置断点
			// channel->logCurLine(lineno); // 记录行号
		}

		/**
		* @创建人 dnp
		* @简介 验证函数参数
		* @返回值 是否验证通过 true 通过 ; false 未通过
		*/
		bool Func::validate()
		{
			if (paramNum == 0) {
				return checkPrst();
			}

			int  n = lua_gettop(lua);
			if (n != paramNum) {				
				std::string tip = funcName + " 参数错误";
				lua_pushstring(lua, tip.c_str());
				lua_error(lua);
				return false;
			}
			return checkPrst();
		}

		/**
		* @创建人 dnp
		* @简介 验证prst状态和是否有新脚本
		* @返回值
		*/
		bool Func::checkPrst()
		{
			if (channel->isNewScript()) {
				channel->setPrst(EnumPrst::normal);
				Log << "发现新脚本,旧脚本退出";
				lua_pushstring(lua, "正常退出");
				lua_error(lua);
				return false;
			}

			// 处理prst
			EnumPrst  prst = channel->getPrst();

			if (prst == EnumPrst::terminate) {
				channel->setPrst(EnumPrst::normal);
				Log << "<<<< checkPrst terminate >>>>";
				lua_pushstring(lua, "正常退出");
				lua_error(lua);
				return false;
			}

			if (prst == EnumPrst::stop) {
				channel->setPrst(EnumPrst::normal);
				Log << "<<<< checkPrst stop >>>>";
				channel->setPrst(EnumPrst::normal);
				lua_pushstring(lua, "停止当前指令");
				return false;
			}

			return true;
		}

		/**
		* @创建人 dnp
		* @简介 运行函数
		* @参数 cmd 待执行的指令
		* @参数 enableDebug 是否允许调试
		* @返回值 是否成功运行
		*/
		bool Func::start(CmdModel cmd) {
			// 打印
			std::vector<double> params = cmd.params.val();
			std::string paramsStr = "  ( ";
			for (int i = 0; i < params.size(); i++) {
				paramsStr = paramsStr + std::to_string(params[i]) + " , ";
			}

			int cmdId = channel->updateCmd(cmd); // 更新指令状态,等待plc执行指令
			Log <<"【Pcs to Plc】 " << funcName << " : " << paramsStr  << " ID: " << cmdId << "  line" << getLine();
			
#pragma region 等待完成信号
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(EPOLLTIME)); // 睡眠10ms

				if (!checkPrst()) {
					return 0;
				}

				std::tuple<int, int> idStatus = channel->getCmdStatus();
				// 指令不同了,直接返回
				if (std::get<0>(idStatus) != cmdId) {
					return true;
				}

				int cmdStatus = std::get<1>(idStatus);
				if (cmdStatus == EnumExecuteStatus::notstart or cmdStatus == running) {
					continue; // 指令正在运行的话继续查询
				}

				Log << "Signals::waitRobSignal cmd finished";
				return true;
			}
#pragma endregion

			return true;
		}

		/**
		* @创建人 dnp
		* @简介 获取通道
		* @返回值 通道
		*/
		Channel* Func::getChannel()
		{
			return channel;
		}

		/**
		* @创建人 dnp
		* @简介 是否是Prst::normal
		* @返回值
		*/
		bool Func::isPrstNormal()
		{
			return channel->getPrst()==EnumPrst::normal;
		}

		/**
		* @创建人 dnp
		* @简介 获取函数所在行
		* @返回值 行号
		*/
		int Func::getLine()
		{
			return lineno-1;
		}
	}
};
