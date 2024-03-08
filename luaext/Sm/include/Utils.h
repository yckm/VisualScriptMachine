#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cstdio>

#ifndef PI
#define PI 3.14159265358979
#endif // !PI



namespace Pcs {
	namespace Sm {
		class Utils
		{
		public:
			/**
			 * @创建人 dnp
			 * @简介 double转string并保留3位小数
			 * @参数 val 数字
			 * @返回值 数字字符串
			 */
			static std::string doubleToString(const double& val)
			{
				char* chCode;
				chCode = new char[20];
				sprintf_s(chCode, sizeof(chCode), "%.3f", val);
				std::string str(chCode);
				delete[]chCode;
				return str;
			}

			/// <summary>
			/// 字符串数组皮拼接为字符串
			/// </summary>
			/// <param name="strs">数组列表</param>
			/// <returns>拼接后的字符串</returns>
			static std::string Arr2String(std::vector< std::string>& strs, std::string spliter = ",") {
				std::string strings("");
				for (auto& s : strs) {
					strings += s + spliter;
				}
				strings.erase(strings.length() - 1, 1);
				return strings;
			}

			/// <summary>
			/// double数组变成string数组
			/// </summary>
			/// <param name="arr">double数组</param>
			/// <returns>字符串数组</returns>
			static std::string DoubleArr2String(std::vector<double> arr)
			{
				if (arr.size() == 0)
				{
					return " () ";
				}

				std::string str = " ( ";
				for (auto d : arr)
				{
					str += doubleToString(d) + ",";
				}

				str.erase(str.length() - 1, 1);
				return str + " ) ";
			}

			/**
			 * @创建人:dnp
			 * @简述:单精度数组转双精度数组
			 * @参数:arr 单精度数组
			 * @返回值:算进度速度
			 * **/
			static std::vector<double> toArrd(std::vector<float>& arr) {
				std::vector<double> arrd;
				for (const auto& v : arr) {
					arrd.push_back(v);
				}
				return arrd;
			}

			/// <summary>
			/// 获取当前时间戳(毫秒)
			/// </summary>
			/// <returns>时间戳(毫秒)</returns>
			static long GetTsMs() {
				auto now = std::chrono::high_resolution_clock::now();
				auto nano_time_point = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
				auto epoch = nano_time_point.time_since_epoch();
				return  std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
			}

			/**
			 * @创建人 dnp
			 * @简介 分割字符串
			 * @参数 str 待分割的字符串
			 * @参数 pattern 分割符
			 * @返回值 分割后的字符串列表
			 */
			static std::vector<std::string> split(std::string& str, const std::string& pattern) {
				std::vector<std::string> result;
				std::string::size_type begin, end;

				end = str.find(pattern);
				begin = 0;

				while (end != std::string::npos) {
					if (end - begin != 0) {
						result.push_back(str.substr(begin, end - begin));
					}
					begin = end + pattern.size();
					end = str.find(pattern, begin);
				}

				if (begin != str.length()) {
					result.push_back(str.substr(begin));
				}
				return result;
			}

			/**
			 * @创建人 dnp
			 * @简介 角度转弧度
			 * @参数 degree 角度
			 * @返回值 弧度
			 */
			static double toRad(double degree) {
				return PI * degree / 180.0;
			}

			/**
			 * @创建人 dnp
			 * @简介 弧度转角度
			 * @参数 rad 弧度
			 * @返回值 角度
			 */
			static double toDegree(double rad) {
				return 180 * rad / PI;
			}
		};
	}
}