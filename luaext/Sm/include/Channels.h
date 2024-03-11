#pragma once
/*****************************************************************
 * @文件名称:Channels.h
 * @创建人:dnp
 * @创建日期:2024 February
 * @简述: 主线程中的通道集合,全局共享变量
*******************************************************************/
#include <vector>
#include "Channel.h"
#include <chrono>
#include <thread>

namespace Pcs {
	namespace Sm {
		class Channels
		{
		private:
			static std::vector<Channel*> channels;

		public:
			static const int channel_num = 1;
			static std::thread threads[channel_num];
			static void append(Channel* channel);
			static Channel* getChannel(int channelId);

			/**
			 * @创建人 dnp
			 * @简介 获取预定义位置变量列表.
			 * @参数 channelId 公道ID
			 * @返回值 预定义位置变量列表
			 */
			static std::vector<D6Param> getFixParamsList(int channelId);
		};
	}
}