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
	CLOG_INFO("lua脚本执行结果 %d",ret);
	CLOG_INFO("width = %d, height = %d", w, h);
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
		CLOG_ERR("Error Msg is %s.", lua_tostring(L, -1));
		return -1;
	}

	/**
	 * 1. 取lua中的值
	 */
	lua_getglobal(L, "width");
	lua_getglobal(L, "height");
	if (!lua_isnumber(L, -2)) {
		CLOG_ERR("'width' should be a number");
		return -2;
	}
	if (!lua_isnumber(L, -1)) {
		CLOG_ERR("'height' should be a number");
		return -3;
	}
	*w = lua_tointeger(L, -2);
	*h = lua_tointeger(L, -1);

	/**
	 *  2. 调用函数流程, 参数2个int值
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
	CLOG_INFO("结果 %d", sum);

	/**
	 * 3. 获得lua表例子.
	 * http://cn.cocos2d-x.org/tutorial/show?id=1223
	 */
	//从Lua里面取得me这个table，并压入栈
	lua_getglobal(L, "me");
	if (!lua_istable(L, -1)) {
		CLOG_ERR("error! me is not a table");
	}

	// 关闭
	lua_close(L);
	return 0;
}

