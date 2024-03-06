#include "../include/Debug.h"
#include <iostream>
#include "../../Models/Enums.h"
#include "../../Models/Models.h"
#include "../include/Utils.h"
#include "../include/log.h"

namespace Pcs {
	namespace Sm {
		using namespace Model;

		/**
		* @创建人 dns
		* @简介 设置是否继续
		* @参数 canContinue 是否能继续(下一步)
		*/
		void Debug::setContinue(bool canContinue)
		{
			this->_canContinue = canContinue;
		}

		/**
		* @创建人 dns
		* @简介 是否能够继续执行
		* @参数 line 当前行号
		* @返回值 true 继续/ false 等待
		*/
		bool Debug::canContine() {
			if (!_isDebugMode) {
				return false;
			}

			// 如果按断点进行的话,未到达断点前忽略单步执行. 到达断点所在行后, 继续按单步执行
			if (_targetPointLine > _curLine) {
				return true;
			}

			return _canContinue;
		}

		/**
		* @创建人 dns
		* @简介 记录当前行号
		* @参数 line 当前行号
		*/
		void Debug::logCurLine(int line)
		{
			Log << "logCurLine " << line;
			_curLine = line;
		}

		/**
		* @创建人 dns
		* @简介 获取当前行号
		* @参数 line 当前行号
		*/
		int Debug::getCurLine()
		{
			return _curLine;
		}

		/**
		 * @创建人 dns
		 * @简介 设置模式(是否调试)
		 * @参数 isDebug 是否进行调试
		 */
		bool Debug::setMode(bool isDebug) {
			_isDebugMode = isDebug;
			_targetPointLine = -1;
			_canContinue = false;
			_breakpointLines = std::vector<int>();
			_curLine = -1;
			return true;
		}

		/**
		 * @创建人 dns
		 * @简介 设置断点
		 * @参数 linenos 断点所在行号列表
		 */
		bool  Debug::setBreakpoints(std::vector<int> linenos) {
			_breakpointLines = linenos;
			return true;
		}

		/**
		 * @创建人 dns
		 * @简介 是否是调试模式
		 * @返回值 是否是调试模式
		 */
		bool Debug::isDebug() {
			return _isDebugMode;
		}

		/**
		 * @创建人 dns
		 * @简介 将行号设置下一个断点
		 */
		bool  Debug::nextBreakpoint() {
			std::vector<int> bkl = _breakpointLines.val();
			for (int i = 0; i < bkl.size(); i++) {
				if (bkl[i] > _targetPointLine) {
					_targetPointLine = bkl[i];
					return true;
				}
			}

			_targetPointLine = -1;
			return false;
		}

		/**
		* @创建人 dns
		* @简介  下一行(如果下一行未非系统函数则无效)
		* @参数 channel 通道ID
		* @返回值 是否设置成功
		*/
		bool Debug::toContinue() {
			_canContinue = true;
			return true;
		}

		/**
		 * @创建人 dns
		 * @简介 获取当前行号
		 * @返回值  当前行号
		 */
		int  Debug::getLine() const {
			return _targetPointLine;
		}

	}
}