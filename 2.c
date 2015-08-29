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
}D;
D data={
	'a'
	,42
,3.1415
};

int main()
{
    lua_State* L = luaL_newstate();
    int w,h;
    load(L,"./value.lua",&w,&h);
    printf("width = %d, height = %d\n",w,h);
    lua_close(L);
    return 0;
}

void load(lua_State* L, const char* fname, int* w, int* h) 
{
    if (luaL_loadfile(L,fname) || lua_pcall(L,0,0,0)) {
        printf("Error Msg is %s.\n",lua_tostring(L,-1));
        return;
    }
    lua_getglobal(L,"width");
    lua_getglobal(L,"height");
    if (!lua_isnumber(L,-2)) {
        printf("'width' should be a number\n" );
        return;
    }
    if (!lua_isnumber(L,-1)) {
        printf("'height' should be a number\n" );
        return;
    }
    *w = lua_tointeger(L,-2);
    *h = lua_tointeger(L,-1);

	// 调用函数流程, 参数2个int值
// 参考 http://codingnow.cn/language/1530.html
	lua_getglobal(L,"add");  //获得全局变量,函数也是变量
	lua_pushinteger(L, *w); //入栈1
	lua_pushinteger(L, *h); //入栈2
	// void lua_call (lua_State *L, int nargs, int nresults);
	lua_call(L, 2, 1); // 调用
	int sum = (int)lua_tointeger(L, -1); // 结果是 -1,单个返回值.保存结果到c
	lua_pop(L, 1); // 出栈
	printf("结果 %d\n",sum);
}

