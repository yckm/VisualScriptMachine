#include "../include/PcsExecutor.h"
#include <iostream>
#include "../../Models/Enums.h"
#include "../../Models/Models.h"
#include "../include/Utils.h"
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include "../include/Channels.h"
#include "../include/LuaExtFuncs.h"
#include "../include/log.h"


namespace Pcs {
	namespace Sm {
		using namespace Model;
		/**
		* @创建人 dnp
		* @简介 启动通道
		* @参数 channelId 通道ID
		*/
		void PcsExecutor::Start(int channelId) {
			this->channelId = channelId;
			channel = Channels::getChannel(channelId);
			initLua();

			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(EPOLLTIME)); // 睡眠10ms					
				// 脚本id相同的清空只会出现在脚本执行结束的时候,否则函数卡在执行脚本的地方
				if (curScriptId == channel->getScriptId()) {
					continue;
				}

				channel->setLog(""); // 开始新脚本前清空原来的日志
				executeScript();// 新的脚本 暂停是针对脚本的暂停,如果脚本没有运行,又何必暂停
				channel->setScriptStatus(EnumExecuteStatus::terminated); // 设置脚本结束
			}
		}

		/**
		 * @创建人 dnp
		 * @简介 跟踪行号
		 * @参数 lua lua状态
		 * @参数 debug lua debug
		 */
		static void trace(lua_State* lua, lua_Debug* debug)
		{
			lua_getglobal(lua, "channel"); // 获取通道ID
			int channelId = lua_tonumber(lua, -1);
			lua_pop(lua, 1);

			int curLine = debug->currentline - 1;
			
			#pragma region 断点
			Channel* channel = Channels::getChannel(channelId);
			channel->logCurLine(curLine); // 记录当前行号
			while (channel->isDebug())
			{				
				if (channel->canContine()) {// 如果调试并且没有指明下一步的化继续在此等待
					Log << "跳出该行 Line : " << curLine;
					break; // 跳出循环
				}

				#pragma region prst
				if (channel->isNewScript()) {
					channel->setPrst(EnumPrst::normal);
					Log << "发现新脚本,旧脚本退出";
					lua_pushstring(lua, "正常退出");
					lua_error(lua);
					return;
				}

				// 处理prst
				EnumPrst  prst = channel->getPrst();

				if (prst == EnumPrst::terminate) {
					channel->setPrst(EnumPrst::normal);
					Log << "<<<< checkPrst terminate >>>>";
					lua_pushstring(lua, "正常退出");
					lua_error(lua);
					return ;
				}

				if (prst == EnumPrst::stop) {
					channel->setPrst(EnumPrst::normal);
					Log << "<<<< checkPrst stop >>>>";
					channel->setPrst(EnumPrst::normal);
					lua_pushstring(lua, "停止当前指令");
					return ;
				}
				#pragma endregion

				std::this_thread::sleep_for(std::chrono::milliseconds(EPOLLTIME)); // 睡眠10ms				
			}
			#pragma endregion
		}

		/**
		 * @创建人 dnp
		 * @简介 执行脚本
		 */
		void PcsExecutor::executeScript() {
			channel->setErr(false, "");
			try {
				ScriptModel script = channel->getScript();
				Log << "PcsExecutor::executeScript preId : " << curScriptId << " latest id : " << script.id;

				curScriptId = script.id;
				script.hasErr = false;

				if (script.content.val().empty()) {
					channel->setErr(true, "script is empty");
					Log << "脚本为空";
					return;
				}

				if (luaL_loadstring(lua, script.content.val().c_str()) != LUA_OK) {
					std::string err = lua_tostring(lua, -1);
					channel->setErr(true, err);
					Log << err;
					return;
				}

				channel->setScriptStatus(EnumExecuteStatus::running); // 设置脚本为运行状态				
				channel->logCurLine(-1);
				if (lua_pcall(lua, 0, 0, 0) != LUA_OK) {
					std::string err = lua_tostring(lua, -1);
					if (err != "" and err != "正常退出") {
						channel->setErr(true, err);
						Log << err;
						return;
					}
				}

				channel->setErr(false, "");
			}
			catch (const std::exception e) {
				channel->setErr(false, e.what());
				std::cout << "PcsExecutor::executeScript::channel : " << channelId << "    " << e.what();
			}
		}

		/**
		 * @创建人 dnp
		 * @简介 初始化lua虚拟机并注册函数
		 * @返回值 lua虚拟机
		 */
		void PcsExecutor::initLua() {
			lua = luaL_newstate();
			luaL_openlibs(lua);
			lua_register(lua, "wait", LuaExtFuncs::wait);
			lua_register(lua, "get_joints", LuaExtFuncs::get_joints);
			lua_register(lua, "get_tcp", LuaExtFuncs::get_tcp);
			lua_register(lua, "get_state", LuaExtFuncs::get_state);
			lua_register(lua, "update_wobj", LuaExtFuncs::update_wobj);
			lua_register(lua, "use_tool", LuaExtFuncs::use_tool);
			lua_register(lua, "use_wobj", LuaExtFuncs::use_wobj);
			lua_register(lua, "set_joints_speed", LuaExtFuncs::set_joints_speed);
			lua_register(lua, "set_tail_speed", LuaExtFuncs::set_tail_speed);
			lua_register(lua, "exit", LuaExtFuncs::exit);
			lua_register(lua, "movej", LuaExtFuncs::movej);
			lua_register(lua, "movep", LuaExtFuncs::movep);
			lua_register(lua, "movel", LuaExtFuncs::movel);
			lua_register(lua, "movej_d", LuaExtFuncs::movej_d);
			lua_register(lua, "movep_d", LuaExtFuncs::movep_d);
			lua_register(lua, "movel_d", LuaExtFuncs::movel_d);
			lua_register(lua, "tool", LuaExtFuncs::tool);
			lua_register(lua, "rad", LuaExtFuncs::torad);
			lua_register(lua, "log", LuaExtFuncs::luaLog);
			//lua_register(lua, "movej2", LuaExtFuncs::movej2);	
			lua_register(lua, "move_tcp", LuaExtFuncs::move_tcp);
			lua_register(lua, "send_signal", LuaExtFuncs::send_signal);
			lua_register(lua, "wait_signal", LuaExtFuncs::wait_signal);

			// 注册钩子
			lua_sethook(lua, trace, LUA_MASKLINE, 0); // before
		}
	}
}