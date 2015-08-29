#include <stdio.h>
#include <string.h>
///  指定的是5.2的版本
#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h> //luaL_newstate();

#include "read_global_value.h"

// 模拟数据
typedef struct {
	char name;
	int ivalue;
	float fvalue;
} D;
D data = {
	'a'
	, 42
	, 3.1415
};

int main()
{

	int w, h;
	int ret;
/// 输入
	ret=load("./value.lua", &w, &h);

/// 输出
	printf("执行结果 %d",ret);
	printf("width = %d, height = %d\n", w, h);
	return 0;
}

int load(const char* fname, int* w, int* h)
{
	// 启动和开启
	lua_State* L = luaL_newstate(); /* 创建Lua接口指针 */
	/* luaL_loadfile 读取lua源文件，仅载入内存而未编译 */
	/* lua_pcall 执行匿名函数，以编译源代码成二进制码 */
	/* 并将全局变量压栈（函数名也是变量）。*/
	/* 这句看似无用，但是不能省 */
	if (luaL_loadfile(L, fname)||lua_pcall(L, 0, 0, 0)) {
		printf("Error Msg is %s.\n", lua_tostring(L, -1));
		return -1;
	}

	/**
	 * 取lua中的值
	 */
	lua_getglobal(L, "width");
	lua_getglobal(L, "height");
	if (!lua_isnumber(L, -2)) {
		printf("'width' should be a number\n");
		return -2;
	}
	if (!lua_isnumber(L, -1)) {
		printf("'height' should be a number\n");
		return -3;
	}
	*w = lua_tointeger(L, -2);
	*h = lua_tointeger(L, -1);

	/**
	 *  调用函数流程, 参数2个int值
	 *  参考 http://codingnow.cn/language/1530.html
	 */
	lua_getglobal(L, "add");     //获得全局变量,函数也是变量
	lua_pushinteger(L, *w);     //入栈1
	lua_pushinteger(L, *h);     //入栈2
	// void lua_call (lua_State *L, int nargs, int nresults);
	// 2个参数,1个返回值
	lua_call(L, 2, 1);     // 调用
	// 结果是 -1,单个返回值.保存结果到c 表示从*栈顶*取得返回值。
	int sum = (int) lua_tointeger(L, -1);
	lua_pop(L, 1);     // 出栈
	printf("结果 %d\n", sum);

	// 关闭
	lua_close(L);
	return 0;
}

