#include "../include/Script.h"
#include <iostream>
#include "../../Models/Enums.h"
#include "../../Models/Models.h"
#include "../include/Utils.h"

namespace Pcs {
	namespace Sm {
		using namespace Model;

		Script::Script(int channelId) {
			this->channelId = channelId;
			script.channel = channelId;
		}

		/**
		 * @创建人 dnp
		 * @简介 获取脚本状态
		 * @返回值 (脚本Id,脚本状态)
		 */
		int Script::getScriptId() {
			return script.id;
		}

		bool Script::isNewScript()
		{
			return isNew;
		}

		/**
		 * @创建人 dnp
		 * @简介 获取脚本
		 * @返回值 脚本实例
		 */
		const ScriptModel& Script::getScript() {
			return script;
		}

		/**
		 * @创建人 dnp
		 * @简介 获取Prst值
		 * @返回值
		 */
		EnumPrst Script::getPrst() {
			return script.prst;
		}

		/**
		* @创建人 dnp
		* @简介 设置脚本状态
		* @参数 stauts 脚本状态
		*/
		void Script::setStatus(EnumExecuteStatus status) {
			script.status = status;
			if (status == EnumExecuteStatus::terminated or status ==EnumExecuteStatus::running) {
				isNew = false;
			}
		}

		/**
		 * @创建人 dnp
		 * @简介 更新脚本模型内容
		 * @参数 script 脚本内容
		 */
		long Script::update(std::string& s) {
			std::string content = "channel=" + std::to_string(channelId) + "\n" + s;
			script.content = content; // 脚本首行追加通道信息
			script.id = Utils::GetTsMs();
			script.hasErr = false;
			script.prst = EnumPrst::normal;
			script.status = EnumExecuteStatus::notstart;
			isNew = true;
			script.scriptRows = Utils::split(content, "\n"); // 分割成行
			return script.id;
		}

		/**
		 * @创建人 dnp
		 * @简介 更新Prst
		 * @参数 value psrt的值
		 */
		bool Script::setPrst(EnumPrst value) {
			script.prst = value;		
			return true;
		}


		/**
		* @创建人 dnp
		* @简介 设置错误信息
		* @参数 hasErr 是否包含错误
		* @参数 err 错误信息
		*/
		void Script::setErr(bool hasErr,std::string err) {
			script.hasErr = hasErr;
			script.errMsg = err;
		}

		/**
		* @创建人 dnp
		* @简介 获取指定行号的代码.
		* @参数 rowno 行号
		* @返回值 该行的代码
		*/
		std::string Script::getScriptRow(int rowno)
		{
			if (script.scriptRows.size() > rowno) {
				return script.scriptRows[rowno];
			}
			return "";
		}
	}
}