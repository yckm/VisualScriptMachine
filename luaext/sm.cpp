// CcreateDll.cpp : 定义 DLL 应用程序的导出函数
#ifndef CHANNEL_NUM
#define CHANNEL_NUM 1 // 通道数
#endif

#include "sm.h"
#include <iostream>
#include "Sm/include/PcsExecutor.h"
#include "Sm/include/Channel.h"
#include "Sm/include/Utils.h"
#include "Sm/include/Channels.h"
#include "mutex"
#include <string>
#include "glog/logging.h"
#include <io.h>
#include <direct.h>
#include "Sm/include/log.h"
#include "Sm/include/FixedParams.h"

using namespace Pcs::Sm;

#ifndef CHARLEN
#define CHARLEN 256
#endif // !CHARLEN

static std::once_flag once_flag;

static void _initLogger() {
	google::InitGoogleLogging("scriptvm");

	std::string prefix = "smlog";
	if (_access(prefix.c_str(), 0) == -1)	//如果文件夹不存在
		_mkdir(prefix.c_str());				//则创建


	// 设置特定严重级别的日志的输出目录和前缀
	google::SetLogDestination(google::GLOG_INFO, "smlog\\svm_info_");
	google::SetLogDestination(google::GLOG_WARNING, "smlog\\svm_warn_");
	google::SetLogDestination(google::GLOG_ERROR, "smlog\\svm_err_");
	google::SetLogDestination(google::GLOG_FATAL, "smlog\\svm_fatal_");

	google::SetLogFilenameExtension(".log");
	google::SetStderrLogging(google::GLOG_INFO);
	FLAGS_v = 1;
	FLAGS_logbufsecs = 0;
	FLAGS_max_log_size = 10;
	FLAGS_colorlogtostderr = true;
}

/**
 * @创建人 dnp
 * @简介 初始化给c#提供的变量
 */
static void _init_charp_share_params() {
	// 初始化对c#提供的返回结构化相关的指针
	ExternChannelStateModel* ecs = new  ExternChannelStateModel();
	ecs->funcName = (char*)malloc(CHARLEN * sizeof(char));  // 预先申请动态空间
	ecs->tool = (char*)malloc(CHARLEN * sizeof(char));
	ecs->wobj = (char*)malloc(CHARLEN * sizeof(char));
	ecs->err = (char*)malloc(CHARLEN * 4 * sizeof(char));
	ecs->log = (char*)malloc(CHARLEN * 4 * sizeof(char));
	externalChannelStateLs.push_back(ecs);

	ExternCmd* ec = new ExternCmd();
	ec->wobj = (char*)malloc(CHARLEN * sizeof(char));
	ec->tool = (char*)malloc(CHARLEN * sizeof(char));
	extenalCmdLs.push_back(ec);

	Resp* resp = new Resp();
	resp->msg = (char*)malloc(CHARLEN * 4 * sizeof(char));
	std::string msg = "Success";
	strcpy_s(resp->msg, CHARLEN * 4, msg.c_str());
	externalRespLs.push_back(resp);
}

/**
* @创建人 dnp
* @简介 创建通道
*/
bool  initScriptMachine() {
	std::call_once(once_flag, []() {
		_initLogger();
		FixedParams::init();

		for (int channelId = 0; channelId < CHANNEL_NUM; channelId++) {
			Channels::append(new Channel(channelId));
			Channels::threads[channelId] = std::thread(&PcsExecutor::Start, new PcsExecutor(), channelId);
			_init_charp_share_params();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		Log << "创建通道数 : " << CHANNEL_NUM;
		});
	return true;
}

/**
	 * @创建人 dnp
	 * @简介执行脚本。脚本遵循本文档规范。该函数调用后, 设置  脚本 共享变量。
	 * @参数 chId 通道 ID。
	 * @参数 script 脚本内容
	 * @返回值 是否设置成功
	 */
Resp* executeScript(int chId, const char* script) {
	Log << "bridge.executeScript() \r\n" << script;
	Resp* resp = externalRespLs[chId];
	Channel* channel = Channels::getChannel(chId);

	std::tuple<int, std::string> idMsg =channel ->updateScript(script);
	long scriptId = std::get<0>(idMsg);
	if (scriptId<0) {
		resp->isOK = false;
		strcpy_s(resp->msg, CHARLEN * 4, std::get<1>(idMsg).c_str());
		return resp;
	}

	return resp;	
}

/**
 * @创建人 dnp
 * @简介
 * @参数 channel 通道 ID。
 * @参数 signal PRST的值
 * @返回值 是否设置成功
 */
bool  prst(int channel, int signal) {
	Log << "【bridge.prst("+std::to_string(signal) +")】";
	return  Channels::getChannel(channel)->setPrst(static_cast<Pcs::Model::EnumPrst>(signal));
}

/**
 * @创建人 dnp
 * @简介 PLC 设置指令为完成状态
 * @参数 channel 通道 ID。
 * @参数 cmdId 控制指令ID
 * @参数 status 指令状态
 * @返回值 是否设置成功
 */
bool setCmdStatus(int channel, int cmdId, int status) {
	Log << "bridge.setCmdTerminated()";
	return  Channels::getChannel(channel)->setCmdStatus(cmdId, static_cast<Pcs::Model::EnumExecuteStatus>(status));
}

/**
* @创建人 dnp
* @简介 PLC更新 gBotState 的值
* @参数 channel 通道ID
* @参数 rts [机器人状态,机器人任务状态,机器人安全控制器状态]
* @参数 joints 关节角度
* @参数 tcp 末端位置
* @返回值 是否设置成功
*/
bool  updateBotStatus(int channel, States* rts, Joints* joints, Tcps* tcp){
	std::vector<double> js;
	std::vector<double> tcps;
	for (int i = 0; i < 6; i++) {
		js.push_back(Utils::toDegree(joints->values[i]));		
	}

	for (int i = 0; i < 3; i++) {
		tcps.push_back(tcp->values[i] * 1000);		
	}
	for (int i = 0; i < 3; i++) {
		tcps.push_back(Utils::toDegree(tcp->values[i + 3]));
	}

	return  Channels::getChannel(channel)->updateBotStatus(js,tcps, rts->values[0], rts->values[1], rts->values[2]);
}

/**
 * @创建人 dnp
 * @简介 PLC获取通道状态,搞到数据一般不使用,所以得到数据就保存原样,方便用户查看
 * @参数 channel 通道 ID。
 * @返回值  ChannelStateModel实例
 */
ExternChannelStateModel*  getChannelState(int channelId) {
	Channel* channel = Channels::getChannel(channelId);

	Pcs::Model::ChannelStateModel* ch = channel->getChannelState();
	Pcs::Model::ScriptModel script = channel->getScript();

	ExternChannelStateModel* c = externalChannelStateLs[channelId];
	c->channel = ch->channel; // 通道号

	std::string tool = ch->tool.val();
	std::string wobj = ch->wobj.val();
	std::string funcName = script.funcName.val();
	std::string lg = ch->scriptLog.val();

	strcpy_s(c->tool, CHARLEN, tool.c_str());
	strcpy_s(c->wobj, CHARLEN, wobj.c_str());
	strcpy_s(c->funcName, CHARLEN, funcName.c_str());
	strcpy_s(c->log, CHARLEN, lg.c_str());
	
	c->status = script.status; // 当前脚本的状态
	c->jointV = ch->jointV;// 速率为30%
	c->tcpV = ch->tcpV ;// 100mm/s
	
	c->jointA = ch->jointA;
	c->tcpA = ch->tcpA;

	c->hasErr = script.hasErr?1:0;

	// 错误信息
	std::string err = script.errMsg.val();
	strcpy_s(c->err, CHARLEN * 4, err.c_str());

	c->line = channel->getCurLine();
	return c;
}

/**
 * @创建人 dnp
 * @简介 PLC查询控制指令
 * @参数 channel 通道 ID。
 * @返回值 控制指令
 */
ExternCmd*  getCmd(int channel) {
	Pcs::Model::CmdModel cmd = Channels::getChannel(channel)->getCmd();
	ExternCmd* c = extenalCmdLs[channel];
	
	c->a = cmd.a;
	c->v = cmd.v; 
	c->func = cmd.func;
	c->id = cmd.id;
	c->status = cmd.status;

	std::string tool = cmd.tool.val();
	std::string wobj = cmd.wobj.val();
	strcpy_s(c->tool, CHARLEN, tool.c_str());
	strcpy_s(c->wobj, CHARLEN, wobj.c_str());

	c->channel = channel;
	c->values = new double[6];

	std::vector<double> params = cmd.params.val();
	for (int i = 0; i < 6; i++) {
		c->values[i] = params[i];
	}

#pragma region 转换单位
	// movej 加速度和速度为[1,100] ,不用变, 只需要转换姿态即可
	if (cmd.func >= EnumExtFunc::movej and cmd.func <= EnumExtFunc::movej_d) {
		for (int i = 0; i < 6; i++) {
			c->values[i] = Utils::toRad(c->values[i]);
		}
	}

	// movel
	if (cmd.func >= EnumExtFunc::movel and cmd.func <= EnumExtFunc::move_tcp) {
		c->a = c->a / 1000.0;
		c->v = c->v / 1000.0;
		for (int i = 0; i < 3; i++) {
			c->values[i] = c->values[i] / 1000.0;
			c->values[i + 3] = Utils::toRad(c->values[i + 3]);
		}
	}

	// movej_pose 加速度和速度为[1,100]
	if (cmd.func >= movep and cmd.func <= movep_d) {
		for (int i = 0; i < 3; i++) {
			c->values[i] = c->values[i] / 1000.0;
			c->values[i + 3] = Utils::toRad(c->values[i + 3]);
		}
	}
#pragma endregion


	// 控制工器具的数据
	c->toolId = cmd.toolId;
	c->toolAction = cmd.toolAction;

	return c;
}

#pragma region 调试
/**
 * @创建人 dnp
 * @简介  启用或禁用调试
 * @参数 enable true 启用调试/false 禁用调试
 * @返回值 是否设置成功
 */
bool setDebugMode(int channel, bool enable) {
	Log << "bridge.setDebugMode()";
	return Channels::getChannel(channel)->setDebugMode(enable);
}

/**
 * @创建人 dnp
 * @简介  下一个可用断点
 * @返回值 是否设置成功
 */
bool nextBreakpoint(int channel) {
	Log << "bridge.nextBreakpoint()";
	return Channels::getChannel(channel)->nextBreakpoint();
}

/**
 * @创建人 dnp
 * @简介  下一行(如果下一行未非系统函数则无效)
 * @参数 channel 通道ID
 * @返回值 是否设置成功
 */
bool nextStep(int channel) {
	Log << "bridge.nexLine()";
	return Channels::getChannel(channel)->toContinue();
}

/**
 * @创建人 dnp
 * @简介 设置断点
 * @参数 channel 通道ID
 * @参数 linenos 断点所在行列表
 * @参数 size 断点的数量
 * @返回值 是否执行成功
 */
bool setBreakpoints(int channel, int* linenos, int size) {
	Log << "bridge.setBreakpoints()";
	std::vector<int> lines;
	for (int i = 0; i < size; i++) {
		lines.push_back(linenos[i]+1);
	}
	return Channels::getChannel(channel)->setBreakpoints(lines);

}
#pragma endregion

/**
 * @创建人 dnp
 * @简介  更新使用的工件坐标系.
 */
void useWobj(int channel, const char* wobj) {
	Channels::getChannel(channel)->useWobj(wobj);
}
