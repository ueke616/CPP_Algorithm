#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cmath>
#include <tuple>
#include <iomanip>
#include <cstdarg>

using namespace std;

namespace Application{
	struct Item{
		float area;
		float distance;
		float label;
	};

	namespace logger{
		#define INFO(...) Application::logger::__printf(__FILE__, __LINE__, __VA_ARGS__)

		void __printf(const char* file, int line, const char* fmt, ...){
			va_list vl;
			va_start(vl, fmt);

			// None   = 0,     // 无颜色配置
            		// Black  = 30,    // 黑色
            		// Red    = 31,    // 红色
            		// Green  = 32,    // 绿色
            		// Yellow = 33,    // 黄色
            		// Blue   = 34,    // 蓝色
            		// Rosein = 35,    // 品红
            		// Cyan   = 36,    // 青色
           		// White  = 37     // 白色
            		/* 格式是： \e[颜色号m文字\e[0m   */
			printf("\e[32m[%s:%d]:\e[0m ", file, line);
			vprintf(fmt, vl);
			printf("\n");
		}
	}
}

