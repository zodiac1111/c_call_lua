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
	ret = load("./value.lua", &w, &h);

/// 输出
	CLOG_INFO("lua脚本执行结果 %d", ret);

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

	CLOG_INFO("例1 width = %d, height = %d", *w , *h );
	//stackDump(L);
	/**
	 *  2. 调用函数流程, 参数2个int值
	 *  参考 http://codingnow.cn/language/1530.html
	 */
#define QTY_PARAM 2 /// 参数数量
#define QTY_RETUN 1 /// 返回值数量
	lua_getglobal(L, "add");     //获得全局变量,函数也是变量
	lua_pushinteger(L, *w);     //入栈1
	lua_pushinteger(L, *h);     //入栈2
	// void lua_call (lua_State *L, int nargs, int nresults);
	// 2个参数,1个返回值
	lua_call(L, QTY_PARAM, QTY_RETUN);     // 调用
	// 结果是 -1,单个返回值.保存结果到c 表示从*栈顶*取得返回值。
	int sum = (int) lua_tointeger(L, -1);
	double ds = lua_tonumber(L, -1);
	//stackDump(L);
	lua_pop(L, QTY_RETUN);     // 出栈 ,只需要弹出返回值
	lua_pop(L, QTY_PARAM);
	CLOG_INFO("例2 调用函数 结果 %d, 浮点数 %lf", sum,ds);

	//stackDump(L);
	/**
	 * 3. 获得lua表例子.名字=字符串,年龄=数字
	 * http://cn.cocos2d-x.org/tutorial/show?id=1223
	 * http://zilongshanren.com/archives/641
	 */
	//从Lua里面取得me这个table，并压入栈
	lua_getglobal(L, "me");
	if (!lua_istable(L, -1)) {
		CLOG_ERR("error! me is not a table");
	}
	lua_getfield(L, -1, "name");
	//输出栈顶的name
	//stackDump(L);
	CLOG_INFO("例3 name = %s", lua_tostring(L, -1));
	//把栈顶元素弹出去
	lua_pop(L, 1);
	//stackDump(L);
	lua_getfield(L, -1, "age");
	//stackDump(L);
	if (!lua_isnumber(L, -1)) {
		CLOG_ERR("'age' should be a number");
		return -3;
	}
	CLOG_INFO("例3 age = %ld", lua_tointeger(L, -1));



	CLOG_INFO("================ 结束 =================");
	stackDump(L);
	// 关闭
	lua_close(L);
	return 0;
}

// 打印lua栈情况
void stackDump(lua_State* L)
{
	CLOG_DEBUG("/==bottum==\\");
	int i = 0;
	int top = lua_gettop(L);
	for (i = 1; i<=top; ++i) {
		int t = lua_type(L, i);
		switch (t) {
		case LUA_TSTRING:
			CLOG_DEBUG("| %8s |", lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN:
			CLOG_DEBUG("| %8s |",
				lua_toboolean(L, i) ? "true " : "false ");
			break;
		case LUA_TNUMBER:
			CLOG_DEBUG("| %8g |", lua_tonumber(L, i));
			break;
		default:
			CLOG_DEBUG("| %8s |", lua_typename(L, t));
			break;
		}
	}
	CLOG_DEBUG("|          |");
}
