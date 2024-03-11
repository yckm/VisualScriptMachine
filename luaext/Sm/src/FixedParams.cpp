#pragma once
#include "../include/FixedParams.h"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cstdio>
#include "../include/log.h"
#include "../include/Channels.h"
#include <set>

#ifndef SQLFIXPARAM
#define SQLFIXPARAM "CREATE TABLE IF NOT EXISTS D6Params (	ch	INTEGER NOT NULL,	idx	INTEGER NOT NULL,	v0	NUMERIC NOT NULL DEFAULT 0,	v1	NUMERIC NOT NULL DEFAULT 0,	v2	NUMERIC NOT NULL DEFAULT 0,	v3	NUMERIC NOT NULL DEFAULT 0,	v4	NUMERIC NOT NULL DEFAULT 0,	v5	NUMERIC NOT NULL DEFAULT 0,	PRIMARY KEY(ch,idx));"
#endif // !SQLFIXPARAM



namespace Pcs {
	namespace Sm {
		

		std::string FixedParams::sqlPath = "Sm.db";
		std::vector<std::map<int, std::vector<double>>> FixedParams::prePosList(CHANNEL_NUM);
		sqlite3* FixedParams::db=nullptr;

		void FixedParams::fillLossParams(std::vector<std::set<int>>& pIdx)
		{
			// 检查每个通道0~99P是是否存在
			std::set<int> s;
			std::vector<std::tuple<int, int>> lossIdx; // 丢失了的 <channel,idx>
			for (int ch = 0; ch < CHANNEL_NUM; ch++) {

				for (int i = 0; i < 100; i++) {
					s.insert(i);
				}
				std::vector<int> diff(100);
				auto iter = set_difference(s.begin(), s.end(), pIdx[ch].begin(), pIdx[ch].end(), diff.begin());
				diff.resize(iter - diff.begin());
				for (auto& idx : diff) {
					lossIdx.push_back(std::tuple<int, int>(ch, idx));
				}
			}

			if (!lossIdx.empty()) { // 插入数据库
				std::string sql = "INSERT into D6Params(ch,idx,v0,v1,v2,v3,v4,v5) VALUES (0,0,1,2,3,4,5,6);";
				sqlite3_exec(db, "BEGIN;", 0, 0, 0);
				for (int i = 0; i < lossIdx.size(); i++)
				{
					int channel = std::get<0>(lossIdx[i]);
					int idx = std::get<1>(lossIdx[i]);
					std::string sql = "INSERT into D6Params(ch,idx,v0,v1,v2,v3,v4,v5) VALUES (" + std::to_string(channel) + "," + std::to_string(idx) + ",0,0,0,0,0,0);";
					sqlite3_exec(db, sql.c_str(), 0, 0, 0);
					prePosList[channel].insert({ idx,std::vector<double>(6)});
				}
				sqlite3_exec(db, "COMMIT;", 0, 0, 0);
			}

		}

		/**
		 * @创建人 dnp
		 * @简介 载入数据到内存.
		 * @返回值 是否打开成功
		 */
		bool FixedParams::init() {

			int rc = sqlite3_open(sqlPath.c_str(), &db);
			if (rc != SQLITE_OK) {
				Log << "打开数据库失败 " << sqlite3_errmsg(db);
				db = nullptr;
				return false;
			}

			// 创建数据库表
			rc = sqlite3_exec(db, SQLFIXPARAM, 0, 0, 0);
			if (rc != SQLITE_OK) {
				Log << "创建表失败 " << sqlite3_errmsg(db);
				return 1;
			}
			

			std::vector<std::set<int>> pIdx(CHANNEL_NUM); // 用来检查Idx是否有缺失
			
			// 检查数据是否存在,不存在创建数据
			const char* queryDataSQL = "SELECT ch,idx,v0,v1,v2,v3,v4,v5 FROM D6Params;";
			sqlite3_stmt* statement;

			if (sqlite3_prepare_v2(db, queryDataSQL, -1, &statement, nullptr) == SQLITE_OK) {				
				while (sqlite3_step(statement) == SQLITE_ROW) {// 遍历结果集	
					int channel = sqlite3_column_int(statement, 0);
					int idx = sqlite3_column_int(statement, 1);
					std::vector<double> pms = {
						sqlite3_column_double(statement, 2),
							sqlite3_column_double(statement,3),
							sqlite3_column_double(statement, 4),
							sqlite3_column_double(statement, 5),
							sqlite3_column_double(statement, 6),
							sqlite3_column_double(statement, 7)
					};
					prePosList[channel].insert({ idx,pms });
					pIdx[channel].insert(idx);
				}
				sqlite3_finalize(statement);// 释放资源
			}

			fillLossParams(pIdx); // 填补缺失项

			return true;
		}

		/**
		 * @创建人 dnp
		 * @简介 关闭数据库.
		 * @返回值
		 */
		bool FixedParams::close() {
			if (db != nullptr) {
				sqlite3_close(db);
			}
			return false;
		}

		/**
		 * @创建人 dnp
		 * @简介 更新值.
		 * @参数 channel 通道
		 * @参数 idx 索引 (0开始)
		 * @参数 d6 长度为6的double数组
		 * @返回值 是否更新成功
		 */
		bool FixedParams::update(int channel, int idx, std::vector<double> d6) {
			

			const char* updateDataSQL = "UPDATE D6Params SET v0=?,v1=?,v2=?,v3=?,v4=?,v5=? where ch=? and idx=?;";
			sqlite3_stmt* statement;

			if (sqlite3_prepare_v2(db, updateDataSQL, -1, &statement, nullptr) == SQLITE_OK) {				
				sqlite3_bind_double(statement, 1, d6[0]);
				sqlite3_bind_double(statement, 2, d6[1]);
				sqlite3_bind_double(statement, 3, d6[2]);
				sqlite3_bind_double(statement, 4, d6[3]);
				sqlite3_bind_double(statement, 5, d6[4]);
				sqlite3_bind_double(statement, 6, d6[5]);
				sqlite3_bind_int(statement, 7, channel);
				sqlite3_bind_int(statement,8, idx);


				// 执行语句
				if (sqlite3_step(statement) != SQLITE_DONE) {
					Log << "更新P变量失败";
					return false;
				}

				// 释放资源
				sqlite3_finalize(statement);
			}
			prePosList[channel][idx] = d6;
			return true;
		}

		/**
		 * @创建人 dnp
		 * @简介 获取通道固定参数列表.
		 * @参数 channel 通道id
		 * @返回值 参数列表
		 */
		std::vector<D6Param> FixedParams::getList(int channel) {
			std::vector<D6Param> list;
			for (auto itr = prePosList[channel].begin(); itr != prePosList[channel].end(); ++itr) {
				// 使用 itr->first 和 itr->second 访问键和值
				D6Param dp;// = new D6Param();
				dp.idx = itr->first;
				dp.params = itr->second;
				list.push_back(dp);
			}
			return list;
		}

		/**
		* @创建人 dnp
		* @简介 获取指定索引的值.
		* @参数 channel 通道
		* @参数 idx 索引
		* @返回值 参数值
		*/
		std::vector<double> FixedParams::get(int channel, int idx)
		{
			if (channel > prePosList.size() - 1 or idx>99 or idx<0) {
				return std::vector<double>(6);
			}
			return prePosList[channel][idx];
		}
	}
}