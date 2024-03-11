#pragma once
#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include <cstdio>
#include <set>
#include "../../Models/Models.h"
#include "../../sqlite/sqlite3.h"



namespace Pcs {
	namespace Sm {
		using namespace Pcs::Model;
		class FixedParams
		{
		private:
			static std::string sqlPath;
			static std::vector< std::map<int,std::vector<double>>> prePosList;
			static sqlite3* db;

			/**
			 * @创建人 dnp
			 * @简介 检查并填补缺失数据.
			 * @参数 pIdx 存在的变量
			 */
			static void fillLossParams(std::vector<std::set<int>>& pIdx);

		public:
			/**
			 * @创建人 dnp
			 * @简介 载入数据到内存.
			 * @返回值 是否打开成功
			 */
			static bool init();

			/**
			 * @创建人 dnp
			 * @简介 关闭数据库.
			 * @返回值 
			 */
			static bool close();

			/**
			 * @创建人 dnp
			 * @简介 更新值.
			 * @参数 channel 通道
			 * @参数 idx 索引 (0开始)
			 * @参数 d6 长度为6的double数组
			 * @返回值 是否更新成功
			 */
			static bool update(int channel, int idx, std::vector<double> d6);

			/**
			 * @创建人 dnp
			 * @简介 获取通道固定参数列表.
			 * @参数 channel 通道id
			 * @返回值 参数列表
			 */
			static std::vector<D6Param> getList(int channel);
		
		};
	}
}