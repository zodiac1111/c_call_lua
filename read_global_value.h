#ifndef READ_H
#define READ_H
//#include <lua5.2/lualib.h>

#if 1 /// clog debug
/// 前缀CL  c log
#  include <time.h>
#  include <errno.h>
#  define CL_ATTR_RED "\033[31m"
#  define CL_ATTR_GREEN "\033[32m"
#  define CL_ATTR_YELLOW "\033[33m"
#  define CL_ATTR_BLUE "\033[34m"
#  define CL_ATTR_PURPLE "\033[35m"
#  define CL_ATTR_REV "\033[7m"
#  define CL_ATTR_UNDERLINE "\033[4m"
#  define CL_ATTR_END "\033[0m"
/// *** 自定义输出的换行, 空/ NL 等 *****
#  define CL_LINE_NULL ""
#  define CL_LINE_END_WIN "\r\n"
#  define CL_LINE_END_LINUX "\n"
#  define CL_LINE_END_MAC "\r"
#  define CL_NEWLINE CL_LINE_END_LINUX
#  define CLOG_DEBUG(fmt, ...) \
		fprintf(stdout,\
		"I "CL_ATTR_PURPLE"%s (%s:%d):%s[%d] "CL_ATTR_END fmt CL_NEWLINE\
		,__FUNCTION__,__FILE__, __LINE__ \
		,strerror(errno) ,errno\
		,##__VA_ARGS__ )
#  define CLOG_INFO(fmt, ...) \
		fprintf(stdout,\
		"I "CL_ATTR_GREEN"%s (%s:%d):%s[%d] "CL_ATTR_END fmt CL_NEWLINE\
		,__FUNCTION__,__FILE__, __LINE__ \
		,strerror(errno) ,errno\
		,##__VA_ARGS__ )
#  define CLOG_WARN(fmt, ...) \
		fprintf(stdout,\
		"E "CL_ATTR_YELLOW"%s (%s:%d):%s[%d] "CL_ATTR_END fmt CL_NEWLINE\
		,__FUNCTION__,__FILE__, __LINE__ \
		,strerror(errno) ,errno\
		,##__VA_ARGS__ )
#  define CLOG_ERR(fmt, ...) \
		fprintf(stdout,\
		"E " CL_ATTR_RED "%s (%s:%d):%s[%d] "CL_ATTR_END fmt CL_NEWLINE\
		,__FUNCTION__,__FILE__, __LINE__ \
		,strerror(errno) ,errno\
		,##__VA_ARGS__ )
#else
#  define CLOG_DEBUG(fmt, ...)
#  define CLOG_INFO(fmt, ...)
#  define CLOG_WARN(fmt, ...)
#  define CLOG_ERR(fmt, ...)
#endif /** clog debug */
typedef struct {
	char* name;
	int ivalue;
	float fvalue;
	int t;
} D;
int load(const char* fname, int* w, int* h);
void stackDump(lua_State* L);
int transData(lua_State* L, D *d);
int out(lua_State* L, D *d);
int in(lua_State* L, D *d);
int pdata(D *d);
#endif
