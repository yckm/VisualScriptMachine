#pragma once
/*****************************************************************
 * @文件名称:Debug.h
 * @创建人:dnp
 * @创建日期:2024 February 
 * @简述: 调试模块. 启用调试,默认单步运行脚本.如果设置断点,可直接跳转至设置行号.
*******************************************************************/
#include <iostream>
#include "../../Models/Enums.h"
#include "../../Models/Models.h"

namespace Pcs {
	namespace Sm {
		using namespace Model;
		class Debug
		{
		private:
			int _curLine = -1;// 当前行号
			bool _isDebugMode = false; // 是否是调试模式
			// 设置了 _targetPointLine 后,在当前行到达 _targetPointLine之前,相当于要将_canContinue设置为true
			int _targetPointLine = -1;// 目标断点行号
			PValue<std::vector<int>> _breakpointLines; // 断点设置的行(暂时先不用,默认每行都设置了断点)
			bool _canContinue = false;// 是否能够继续			
		public:

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
			 * @简介 记录当前行号
			 * @参数 line 当前行号
			 */
			void logCurLine(int line);

			/**
			 * @创建人 dnp
			 * @简介 获取当前行号
			 * @返回值 line 当前行号
			 */
			int getCurLine();



			/**
			 * @创建人 dnp
			 * @简介 设置模式(是否调试)
			 * @参数 isDebug 是否进行调试
			 */
			bool setMode(bool isDebug);

			/**
			 * @创建人 dnp
			 * @简介 设置断点
			 * @参数 linenos 断点所在行号列表
			 */
			bool setBreakpoints(std::vector<int> linenos);

			/**
			 * @创建人 dnp
			 * @简介 是否是调试模式
			 * @返回值 是否是调试模式
			 */
			bool isDebug();

			/**
			 * @创建人 dnp
			 * @简介 plc调用该方法,增加1导致行号和当前正在执行的行号不一致,从而进入下一行执行
			 */
			bool nextBreakpoint();

			/**
			 * @创建人 dnp
			 * @简介  下一行(如果下一行未非系统函数则无效)
			 * @返回值 是否设置成功
			 */
			bool toContinue();


			/**
			 * @创建人 dnp
			 * @简介 获取当前行号
			 * @返回值  当前行号
			 */
			int getLine() const;
		};
	}
}
