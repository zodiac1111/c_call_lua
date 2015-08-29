# 例子来源
# http://www.cnblogs.com/stephen-liu74/archive/2012/04/20/2460634.html
# c调用lua,读取全局变量
# 使用的是lua5.2 使用c而不是c++,使用链接库 liblua5.2

all:
	gcc -llua5.2 2.c -o 2
