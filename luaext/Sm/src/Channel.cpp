/*****************************************************************
 * @文件名称:Channels.h
 * @创建人:dnp
 * @创建日期:2024 February
 * @简述: 主线程中的通道集合,全局共享变量
*******************************************************************/
#include "../include/Channel.h"
#include "../include/Channels.h"
#include <vector>
#include "../include/Utils.h"


namespace Pcs {
	namespace Sm {
		/**
		 * @创建人 dnp
		 * @简介 对用户提交的脚本进行检查,这里主要是禁用协程和io
		 * @参数 s 脚本
		 * @返回值
		 */
		static std::string validateScript(std::string s) {
			if (s.empty()) {
				return "脚本不能为空";
			}

			int idx = s.find("coroutine."); //在a中查找b.
			if (idx != std::string::npos) //不存在。
			{
				return "脚本禁止使用协程";
			}

			idx = s.find("io."); //在a中查找b.
			if (idx != std::string::npos) //不存在。
			{
				return "脚本禁止使用IO";
			}
			return "";
		}

		/**
		 * @创建人 dnp
		 * @简介 构造函数
		 * @参数 channelId 通道ID
		 */
		Channel::Channel(int channelId) {
			channelId = channelId;
			_script = new Script(channelId);
			_cmd = new Cmd(channelId);
			_botState = new BotStates();
			_state = new ChannelStateModel();
			_debug = new Debug();
		}

		/**
		 * @创建人 dnp
		 * @简介 获取当前通道ID
		 * @返回值 通道ID
		 */
		int Channel::getChannelId() {
			return channelId;
		}

		/**
		* @创建人 dnp
		* @简介 设置是否继续
		* @参数 canContinue 是否能继续(下一步)
		*/
		void Channel::setContinue(bool canContinue)
		{
			this->_debug->setContinue(canContinue);
		}

		/**
		* @创建人 dnp
		* @简介 是否能够继续执行
		* @返回值 true 继续/ false 等待
		*/
		bool Channel::canContine()
		{
			return _debug->canContine();
		}

		/**
		 * @创建人 dnp
		 * @简介 是否是调试模式
		 */
		bool Channel::isDebug() {
			return _debug->isDebug();
		}

		/**
		 * @创建人 dnp
		 * @简介 是否启动调试模式
		 * @参数 enable true 启用调试/false 禁用调试
		 * @返回值 是否操作成功
		 */
		bool Channel::setDebugMode(bool enable) {
			_debug->setMode(enable);			
			return true;
		}


		/**
		 * @创建人 dnp
		 * @简介 下一个断点
		 * @返回值
		 */
		bool Channel::nextBreakpoint() {
			return _debug->nextBreakpoint();
		}

		/**
		* @创建人 dnp
		* @简介  下一行(如果下一行未非系统函数则无效)
		* @参数 channel 通道ID
		* @返回值 是否设置成功
		*/
		bool Channel::toContinue() {
			return _debug->toContinue();
		}

		/**
		 * @创建人 dnp
		 * @简介 获取当前指令所在的行
		 * @返回值  行号
		 */
		std::tuple<int, bool> Channel::getDebugInfo() {
			return std::tuple<int, bool>(_debug->getLine(),_debug->isDebug());
		}

		/**
		* @创建人 dnp
		* @简介 记录当前行号
		* @参数 line 当前行号
		*/
		void Channel::logCurLine(int line)
		{
			_debug->logCurLine(line);
		}


		/**
		* @创建人 dnp
		* @简介 设置断点
		* @参数 linenos 断点所在行号列表
		* @返回值 是否操作从成功
		*/
		bool Channel::setBreakpoints(std::vector<int> linenos) {
			return _debug->setBreakpoints(linenos);
		}

		/**
		* @创建人 dnp
		* @简介 获取指定行号的代码.
		* @参数 rowno 行号
		* @返回值 该行的代码
		*/
		std::string Channel::getScriptRow(int rowno)
		{
			return _script->getScriptRow(rowno);
		}

		/**
		* @创建人 dnp
		* @简介 设置错误信息
		* @参数 hasErr 是否包含错误
		* @参数 err 错误信息
		*/
		void Channel::setErr(bool hasErr,std::string err)
		{
			_script->setErr(hasErr,err);
		}
		/**
		 * @创建人 dnp
		 * @简介 获取脚本状态
		 * @返回值 (脚本Id,脚本状态)
		 */
		int Channel::getScriptId() {
			return _script->getScriptId();
		}

		/**
		* @创建人 dnp
		* @简介 是否是新加入的脚本
		* @返回值
		*/
		bool Channel::isNewScript()
		{
			return _script->isNewScript();
		}

		/**
		 * @创建人 dnp
		 * @简介 获取脚本
		 * @返回值 脚本实例
		 */
		ScriptModel Channel::getScript() {
			return _script->getScript();
		}

		/**
		* @创建人 dnp
		* @简介 设置脚本状态
		* @参数 stauts 脚本状态
		*/
		void Channel::setScriptStatus(EnumExecuteStatus status) {
			_script->setStatus(status);
		}

		/**
		* @创建人 dnp
		* @简介执行脚本。脚本遵循本文档规范。该函数调用后, 设置  脚本 共享变量。
		* @参数 channel 通道 ID。
		* @参数 _script 脚本内容
		* @返回值 是否设置成功
		*/
		std::tuple<int, std::string> Channel::updateScript(std::string s) {
			std::string errMsg = validateScript(s);
			if (!errMsg.empty()) {
				return std::tuple<int, std::string>(-1, errMsg);
			}

			long scriptId = _script->update(s);
			_cmd->reset();
			return std::tuple<int, std::string>(scriptId,"");
		}

		/**
		 * @创建人 dnp
		 * @简介 设置prst值
		 * @参数 signal PRST的值
		 * @返回值 是否设置成功
		 */
		bool Channel::setPrst(EnumPrst signal) {
			return _script->setPrst(signal);
		}

		/**
		 * @创建人 dnp
		 * @简介 LC设置(全局)关键速率 (非Nc全局速率,Nc那边的有plc设定,这里设定作用是为运动控制函数提供参数, set_tail_speed同理)
		 * @参数 v 关键速率 [1,100]
		 * @参数 a 关节角加速度，单位（系统设定加速度的百分比%），取值范围（0,100]。
		 * @返回值
		 */
		bool Channel::setJointsSpeed(int v, int a) {
			_state->jointV = v;
			_state->jointA = a;
			return true;
		}

		/**
		 * @创建人 dnp
		 * @简介 PLC设置(全局)末端速度
		 * @参数 v 末端速度 mm/s
		 * @参数 a 末端加速度，单位（mm/s2）。
		 * @返回值  是否设置成功
		 */
		bool Channel::setTailSpeed(double v, double a) {
			_state->tcpV = v;
			_state->tcpA = a;
			return true;
		}

		/**
		 * @创建人 dnp
		 * @简介 PLC获取通道状态
		 * @参数 channel 通道 ID。
		 * @返回值  ChannelStateModel实例
		 */
		ChannelStateModel* Channel::getChannelState() {
			return _state;
		}

		/**
		 * @创建人 dnp
		 * @简介 获取psrt值
		 * @返回值 psrt
		 */
		EnumPrst Channel::getPrst() {
			return _script->getPrst();
		}

		/**
		 * @创建人 dnp
		 * @简介 创建或更新工件坐标系
		 * @参数 name 坐标系名称
		 * @参数 offset 坐标系偏移
		 * @返回值  是否设置成功
		 */
		bool Channel::updateWobj(std::string name, std::vector<double> offset) {
			return false;
		}

		/**
		 * @创建人 dnp
		 * @简介 创建或更新工件末端参数
		 * @参数 name 工具名称
		 * @参数 offset 工具偏移
		 * @参数 shape 工具形状
		 * @返回值 是否设置成功
		 */
		bool Channel::updateTool(std::string name, std::vector<double> offset, std::vector<double> shape) {
			return false;
		}

		/**
		 * @创建人 dnp
		 * @简介 设置当前使工具名称
		 * @参数 name 工具名称
		 * @返回值 是否设置成功
		 */
		bool Channel::useTool(std::string name) {
			_state->tool = name;
			return false;
		}

		/**
		 * @创建人 dnp
		 * @简介 设置当前坐标系
		 * @参数 name 坐标系名称
		 * @返回值 是否设置成功
		 */
		bool Channel::useWobj(std::string name) {
			_state->wobj = name;
			return false;
		}

		/**
		* @创建人 dnp
		* @简介 获取工件坐标系名称.
		* @返回值
		*/
		std::string Channel::getWobjName()
		{
			return _state->wobj.val();
		}

		/**
		* @创建人 dnp
		* @简介 获取工件坐标系名称.
		* @返回值
		*/
		std::string Channel::getToolName()
		{
			return _state->tool.val();
		}

		/**
		* @创建人 dnp
		* @简介 PLC更新 gBotState 的值
		* @参数 joints 关节角度
		* @参数 tcps 末端位置
		* @参数 robState 机器人状态
		* @参数 robTaskState 机器人任务状态
		* @参数 safetyState 安全控制器状态
		* @返回值 是否设置成功
		*/
		bool Channel::updateBotStatus(std::vector<double> joints, std::vector<double> tcps, int robState, int robTaskState, int safetyState) {
			return _botState->update(joints, tcps, robState, robTaskState, safetyState);
		}
		/**
		 * @创建人 dnp
		 * @简介 内部获取外部机械臂状态
		 * @返回值  机械臂状态
		 */
		BotStateModel Channel::getBotStatus() {
			return _botState->getBotStatus();
		}

		/**
		 * @创建人 dnp
		 * @简介 PLC查询控制指令
		 * @参数 channel 通道 ID。
		 * @返回值 控制指令
		 */
		CmdModel Channel::getCmd() {
			return _cmd->getCmd();
		}

		/**
		 * @创建人 dnp
		 * @简介 PLC更新指令状态
		 * @参数 cmdId 控制指令ID
		 * @参数 status 指令目标状态
		 * @返回值 是否设置成功
		 */
		bool Channel::setCmdStatus(int cmdId,EnumExecuteStatus status) {
			return _cmd->setStatus(status);
		}

		/**
		 * @创建人 dnp
		 * @简介 executor更新指令
		 * @参数 cmd 新指令
		 * @返回值 是否设置成功
		 */
		int Channel::updateCmd(CmdModel c) {
			return _cmd->update(c, _state);
		}

		/**
		 * @创建人 dnp
		 * @简介 获取指令状态
		 * @返回值 (指令id,指令状态)
		 */
		std::tuple<int, int> Channel::getCmdStatus() {
			return _cmd->getCmdStatus();
		}

		/**
		* @创建人 dnp
		* @简介 记录脚本日志内容
		* @参数 lg 日志内容
		*/
		void Channel::setLog(std::string lg) {
			_state->scriptLog = lg;
		}

		/**
		* @创建人 dnp
		* @简介 获取脚本执行到的行
		* @返回值 行号
		*/
		int Channel::getCurLine()
		{
			return _debug->getCurLine();
		}

		/**
		* @创建人 dnp
		* @简介 更新预定义位置.
		* @参数 p 位姿
		*/
		void Channel::updatePrePosition(int idx,std::vector<double> p)
		{
			D6Param pos = { idx,p };
			prePositions.push(pos);
		}

	

		/**
		* @创建人 dnp
		* @简介 获取预定义位姿.
		* @返回值 位姿
		*/
		D6Param Channel::getPrePosition()
		{
			if (prePositions.empty()) {
				return D6Param();
			}

			D6Param p = prePositions.front();
			prePositions.pop();
			return p;
		}

		/**
		* @创建人 dnp
		* @简介 固定参数是否发生改变.
		* @返回值 是否发生改变
		*/
		bool Channel::prePositionChanged()
		{
			return !prePositions.empty();
		}

	}
}