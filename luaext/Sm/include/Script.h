#pragma once
#include <iostream>
#include "../../Models/Enums.h"
#include "../../Models/Models.h"

namespace Pcs {
	namespace Sm {
		using namespace Model;
		class Script
		{
		private:
			ScriptModel script; // 脚本
			int channelId;
			bool isNew= false; // 是否是新脚本
		public:
			Script(int channelId);

			/**
			 * @创建人 dns
			 * @简介 获取脚本状态
			 * @返回值 (脚本Id,脚本状态)
			 */
			int getScriptId();

			/**
			 * @创建人 dns
			 * @简介 是否是新加入的脚本
			 * @返回值 
			 */
			bool isNewScript();

			/**
			 * @创建人 dns
			 * @简介 获取脚本
			 * @返回值 脚本实例
			 */
			const ScriptModel& getScript();

			/**
			 * @创建人 dns
			 * @简介 获取Prst值
			 * @返回值
			 */
			EnumPrst getPrst();

			/**
			 * @创建人 dns
			 * @简介 设置脚本状态
			 * @参数 stauts 脚本状态
			 */
			void setStatus(EnumExecuteStatus status);

			/**
			 * @创建人 dns
			 * @简介 更新脚本模型内容
			 * @参数 script 脚本内容
			 */
			long update(std::string& s);

			/**
			 * @创建人 dns
			 * @简介 更新Prst
			 * @参数 value psrt的值
			 */
			bool setPrst(EnumPrst value);

			/**
			 * @创建人 dns
			 * @简介 设置错误信息
			 * @参数 hasErr 是否包含错误
			 * @参数 err 错误信息
			 */
			void setErr(bool hasErr, std::string err);
		};
	}
}
