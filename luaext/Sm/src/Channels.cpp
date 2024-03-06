/*****************************************************************
 * @文件名称:Channels.h
 * @创建人:dns
 * @创建日期:2024 February
 * @简述: 主线程中的通道集合,全局共享变量
*******************************************************************/
#include "../include/Channels.h"
namespace Pcs {
	namespace Sm {
		std::thread Channels::threads[Channels::channel_num];
		std::vector<Channel*> Channels::channels = {}; // 通道

		/**
		 * @创建人 dns
		 * @简介 追加新通道
		 * @参数 channel 通道
		 */
		void Channels::append(Channel* channel) {
			for (int i = 0; i < channels.size(); i++) {
				if (channels[i]->getChannelId() == channel->getChannelId())
				{
					return;
				}
			}
			channels.push_back(channel);
		}

		/**
		 * @创建人 dns
		 * @简介 获取通道
		 * @参数 channelId 通道ID
		 * @返回值 通道
		 */
		Channel* Channels::getChannel(int channelId) {
			return channels[channelId];
		}
	}
}