#ifndef READ_H
#define READ_H
//#include <lua5.2/lualib.h>

#if 1 /// clog debug
#  include <time.h>
#  include <errno.h>
#  define CL_RED "\033[31m"
#  define CL_GREEN "\033[32m"
#  define CL_YELLOW "\033[33m"
#  define BLUE "\033[34m"
#  define PURPLE "\033[35m"
#  define CL_ATTR_REV "\033[7m"
#  define CL_ATTR_UNDERLINE "\033[4m"
#  define _COLOR "\033[0m"
#  define CLOG_INFO(fmt, ...) \
		fprintf(stdout,\
		"I "CL_GREEN"%s (%s:%d):%s[%d] "_COLOR fmt "\r\n"\
		,__FUNCTION__,__FILE__, __LINE__ \
		,strerror(errno) ,errno\
		,##__VA_ARGS__ )
#  define CLOG_ERR(fmt, ...) \
		fprintf(stdout,\
		"E " CL_RED "%s (%s:%d):%s[%d] "_COLOR fmt "\r\n"\
		,__FUNCTION__,__FILE__, __LINE__ \
		,strerror(errno) ,errno\
		,##__VA_ARGS__ )
#else
#  define CLOG_INFO(fmt, ...)
#  define CLOG_ERR(fmt, ...)
#endif /** clog debug */

int load(const char* fname, int* w, int* h);

#endif
