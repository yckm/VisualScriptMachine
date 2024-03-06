// CcreateDll.cpp : 定义 DLL 应用程序的导出函数

#pragma once
//#ifdef API_DECLSPEC
//#else
//#define API_DECLSPEC extern "C" _declspec(dllimport) //指的是允许将其给外部调用
//#endif

#include "vector"

#ifndef EXPDLL
#define EXPDLL extern "C" _declspec(dllimport)
#endif

/**
 * @创建人 dns
 * @简介 一般操作返回值
 */
EXPDLL struct Resp {
	bool isOK = true; // 是否成功
	char* msg ;
};


EXPDLL struct Tcps
{
	double values[];
};
EXPDLL struct Joints
{
	double values[];
};
EXPDLL struct States
{
	int values[];
};


/**
 * @创建人 dns
 * @简介 通道状态
 */
EXPDLL struct ExternChannelStateModel {
	int channel; // 通道号
	char* tool; // 当前工具
	char* wobj; // 当前坐标系
	int status; // 当前脚本的状态 EnumExecuteStatus
	int jointV;// 速率为30%
	double tcpV;// 100mm/s
	int jointA; // 关节加速比率[1,100]
	double tcpA;// 末端加速度 mm/s2
	int line; // 正在执行指令所在行号
	char* funcName;// 当前正在执行函数的名称
	int hasErr = 0; // 是否有错误
	char* err;// 错误信息
	char* log;// 由lua中log产生的内容
};

EXPDLL struct ExternCmd {
	int id; // 指令 ID
	int channel;// 通道ID
	int func;  // EnumExtFunc
	double* values;
	double v;  // 速度
	double a;  // 加速度
	char* tool;  // 工具
	char* wobj; // 工件坐标系
	int status;// 指令状态
	int toolId ; // 工具ID
	int toolAction ; // 工具动作
};

// 预先分配内存,返回给.net时不在新创建,还是需要和通道对应
std::vector< ExternChannelStateModel*> externalChannelStateLs;
std::vector< ExternCmd*> extenalCmdLs;
std::vector<Resp*> externalRespLs;

#pragma region explore to lc
/**
* @创建人 dns
* @简介 初始化脚本机 (该方法必须在主线程中调用,请勿重复调用)
*/
EXPDLL bool initScriptMachine();

/**
 * @创建人 dns
 * @简介执行脚本。脚本遵循本文档规范。该函数调用后, 设置  脚本 共享变量。
 * @参数 channel 通道 ID。
 * @参数 script 脚本内容
 * @返回值 是否设置成功
 */
EXPDLL Resp* executeScript(int channel, const char* script);
/**
 * @创建人 dns
 * @简介
 * @参数 channel 通道 ID。
 * @参数 signal PRST的值 Pcs::Model::EnumPrst
 * @返回值 是否设置成功
 */
EXPDLL bool prst(int channel, int  signal);
/**
 * @创建人 dns
 * @简介 PLC 设置指令为完成状态
 * @参数 channel 通道 ID。
 * @参数 cmdId 控制指令ID
 * @参数 status 指令状态
 * @返回值 是否设置成功
 */
EXPDLL bool setCmdStatus(int channel, int cmdId,int status);
/**
 * @创建人 dns
 * @简介 PLC更新 BotState 的值
 * @参数 channel 通道 ID。
 * @返回值  是否设置成功
 */
EXPDLL bool updateBotStatus(int channel, States* rts, Joints* joints, Tcps* tcp);
/**
 * @创建人 dns
 * @简介 PLC获取通道状态
 * @参数 channel 通道 ID。
 * @返回值  ChannelStateModel实例
 */
EXPDLL  ExternChannelStateModel* getChannelState(int channel);
/**
 * @创建人 dns
 * @简介 PLC查询控制指令
 * @参数 channel 通道 ID。
 * @返回值 控制指令
 */
EXPDLL  ExternCmd* getCmd(int channel);

#pragma endregion

#pragma region 调试
/**
 * @创建人 dns
 * @简介  启用或禁用调试
 * @参数 channel 通道ID
 * @参数 enable true 启用调试/false 禁用调试
 * @返回值 是否设置成功
 */
EXPDLL bool setDebugMode(int channel, bool enable);

/**
 * @创建人 dns
 * @简介  下一个可用断点
 * @参数 channel 通道ID
 * @返回值 是否设置成功
 */
EXPDLL bool nextBreakpoint(int channel);

/**
 * @创建人 dns
 * @简介  下一步(如果下一行未非系统函数则无效)
 * @参数 channel 通道ID
 * @返回值 是否设置成功
 */
EXPDLL bool nextStep(int channel);


/**
 * @创建人 dns
 * @简介 设置断点
 * @参数 channel 通道ID
 * @参数 linenos 断点所在行列表
 * @参数 size 断点的数量
 * @返回值 是否执行成功
 */
EXPDLL bool setBreakpoints(int channel , int* linenos, int size);

#pragma endregion


