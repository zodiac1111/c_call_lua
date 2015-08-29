
-------- 例子 1 ----------
-- c 调用lua 获得全局变量例子
--这里是用Lua代码定义的窗口大小的配置信息

--[[
lua中一下数据格式均合法
width = 20e02;
width = 0x10;
width = 1.2; --浮点数因为调用者的关系转化为整形
--]]

width = 20;
height = 300;


-------- 例子 2 ----------
-- c 调用lua 函数,简单参数例子
function add(x, y)
	return x + y;
end


-------- 例子 3 ----------
-- c 语言获得lua table 例子
me = { name = "Alice", age = 23}
outData={};

-------- 例子 4 ----------
--local ffi = require("ffi");
function main(data)
	--print( "lua 接收 data.name : " .. data["name"]);
	--print( "lua 接收 data.ivalue : " .. data["ivalue"]);
	--print( "lua 接收 data.fvalue : " .. data["fvalue"]);

	-- 修改/计算
	-- data["fvalue"]= data["fvalue"]/1.732;
	-- data["fvalue"]=2.3;
	data["t"]=data["ivalue"] + data["fvalue"];
	-- 通过全局变量输出

	outData=data;
	-- return string.format("做个测试 \r\n a.name = %s \r\n a.ivalue = %d \n a.fvalve = %f",
	-- data.name,data.ivalue,data.fvalue);
	return 0;
end
