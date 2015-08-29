#include <stdio.h>
#include <string.h>
#include <assert.h>

///  指定的是5.2的版本
#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h> //luaL_newstate();
#include <lua5.2/lualib.h> // luaL_openlibs
#include "read_global_value.h"

// 模拟数据
D data = {
	.name = "Bob"
	, .ivalue = 42
	, .fvalue = 3.1415
	, .t = 0
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
#if 0 // 启动的方式不同?
	if (luaL_loadfile(L, fname)||lua_pcall(L, 0, 0, 0)) {
		CLOG_ERR("Error Msg is %s.", lua_tostring(L, -1));
		return -1;
	}
#else
	luaL_openlibs(L);
	if (luaL_dofile(L, fname)) {
		CLOG_ERR("Could not load counter module:%s", fname);
		lua_close(L);
		return -1;
	}
#endif

	goto e4;
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
#define QTY_RETUN_E1 1 /// 返回值数量
	CLOG_INFO("例1 width = %d, height = %d", *w, *h);

	/**
	 *  2. 调用函数流程, 参数2个int值
	 *  参考 http://codingnow.cn/language/1530.html
	 */
#define QTY_PARAM 2 /// 参数数量
#define QTY_RETUN 1 /// 返回值数量
	lua_getglobal(L, "add");     //获得全局变量,函数也是变量
	assert(lua_isfunction(L, -1));
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
	//lua_pop(L, QTY_PARAM);
	CLOG_INFO("例2 调用函数 结果 %d, 浮点数 %lf", sum, ds);
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

	/**
	 * 例4 c传递结构体到lua
	 */

	e4:
	CLOG_INFO("====== 输入 ======");
	pdata(&data);
	transData(L, &data);
	CLOG_INFO("====== 输出 ======");
	pdata(&data);
	CLOG_INFO("================ 结束 =================");
	// 关闭
	clean_stack(L);
	lua_close(L);
	return 0;
}
/**
 * 转换数据
 * @param L
 * @param d
 * @return
 */
int transData(lua_State* L, D *d)
{
#define TRANS_PARAM_QTY 1 /// lua函数参数列表数量
#define TRANS_RETVAL_QTY 2 /// 乱函数返回值数量
#define FIRST_RET -(TRANS_RETVAL_QTY)
#define SECOND_RET -(TRANS_RETVAL_QTY-1)
	int lua_run_ret;     /// 转换程序运行成功与否
	int ret = 0;
	/// 1. 输入数据到lua
	in(L, d);
	/// 2. 执行lua函数,转换数据 1个参数 1个返回值
	lua_call(L, TRANS_PARAM_QTY, TRANS_RETVAL_QTY);
	lua_run_ret =
		lua_isnumber(L, FIRST_RET) ? lua_tointeger(L, FIRST_RET) : -1;
	if (lua_run_ret!=0) {
		CLOG_INFO("转换结果 = %d", lua_run_ret);
		ret = -1;
		goto LUA_FUNCTION_RETURN;
	}
	if (!lua_istable(L, SECOND_RET)) {
		CLOG_ERR("error! me is not a table");
		ret = -2;
		goto LUA_FUNCTION_RETURN;
	}
	/// 3. 从lua输出数据
	out(L, d);
	LUA_FUNCTION_RETURN:
	/// lua函数结束,返回值出栈
	lua_pop(L, TRANS_RETVAL_QTY);

	//stackDump(L);
	return ret;
}

#define xstr(s) #s

/**
 * T 类型 取 nil,number,integer,string
 * d 结构体
 * f 结构体成员
 * 组合成为类似 set_number 的宏
 */
#define set(T,L,sp,d,f)\
	lua_push##T(L, d->f);\
	lua_setfield(L, sp, xstr(f));
#define set_number(L,d,f)\
	lua_pushnumber(L, d->f);\
	lua_setfield(L, -2, xstr(f));
/// 与set类似
#define get(T,L,d,f)\
	lua_getfield(L, -1, xstr(f));\
	d->f=lua_to##T(L, -1);\
	lua_pop(L, 1);

#define get_int(L,d,f)\
	lua_getfield(L, -1, xstr(f));\
	d->f=lua_tointeger(L, -1);\
	lua_pop(L, 1);
#define get_string(L,d,f)\
	lua_getfield(L, -1, xstr(f));\
	strcpy(d->f,lua_tostring(L, -1));\
	lua_pop(L, 1);

/**
 * 数据函数,结构体到lua函数参数
 * @param L lua
 * @param d 数据输入
 * @return
 */
int in(lua_State* L, D *d)
{
	int sp = 0;     /// 栈指针指向栈底,一般<=0
	lua_getglobal(L, "main");     /// lua中的转换函数
	sp--;
	/// Checks if top of the Lua stack is a function.
	assert(lua_isfunction(L, sp));
	lua_newtable(L);
	sp--;
	/// 设置具体数据到lua.
	/// ro
	set(string, L, sp, d, name);

	/// wr
	set(integer, L, sp, d, ivalue);
	set(integer, L, sp, d, t);
	set(number, L, sp, d, fvalue);
	//stackDump(L);
	return 0;
}

/**
 * 输出函数, lua函数运行结果到结构体
 * @param L
 * @param d
 * @return
 */
int out(lua_State* L, D *d)
{
	get_string(L, d, name);
	/// 具体数据从lua中读取
	get(integer, L, d, ivalue);
	get(integer, L, d, t);
	get(number, L, d, fvalue);

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
/**
 * 打印结构其
 * @param d
 * @return
 */
int pdata(D *d)
{
	CLOG_INFO("name : %s", d->name);
	CLOG_INFO("i    : %ld", d->ivalue);
	CLOG_INFO("f    : %lf", d->fvalue);
	CLOG_INFO("t    : %ld", d->t);
	return 0;
}
void clean_stack(lua_State* L)
{
	int n = lua_gettop(L);
	if (n!=0) {
		stackDump(L);
		CLOG_WARN("lua 栈有残留!");
	}
	lua_pop(L, n);
}
