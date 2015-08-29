#include <stdio.h>
#include <string.h>
///  指定的是5.2的版本
#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h> //luaL_newstate();

#include "read_global_value.h"

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
	lua_getglobal(L,"add");  
	lua_pushinteger(L, *w);
	lua_pushinteger(L, *h);
	lua_call(L, 2, 1);
	int sum = (int)lua_tointeger(L, -1);
	lua_pop(L, 1);
	printf("结果 %d\n",sum);
}

