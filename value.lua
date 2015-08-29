
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
	--[[
	data["t"]: 总数据
	data["ivalue"] : 整型数据
	data["fvalue"] :浮点型数据
	data["name"]: 名称:只读
--]]
	print_r(data);
	-- 修改/计算
	-- data["fvalue"]= data["fvalue"]/1.732;
	-- data["fvalue"]=2.3;
	data["name"]="222";
	data["t"]=data["ivalue"] + data["fvalue"];
	-- 通过全局变量输出

	outData=data;
	-- return string.format("做个测试 \r\n a.name = %s \r\n a.ivalue = %d \n a.fvalve = %f",
	-- data.name,data.ivalue,data.fvalue);
	return 0;
end

function print_r ( t )
	local print_r_cache={}
	local function sub_print_r(t,indent)
		if (print_r_cache[tostring(t)]) then
			print(indent.."*"..tostring(t))
		else
			print_r_cache[tostring(t)]=true
			if (type(t)=="table") then
				for pos,val in pairs(t) do
					if (type(val)=="table") then
						print(indent.."["..pos.."] => "..tostring(t).." {")
						sub_print_r(val,indent..string.rep(" ",string.len(pos)+8))
						print(indent..string.rep(" ",string.len(pos)+6).."}")
					elseif (type(val)=="string") then
						print(indent.."["..pos..'] => "'..val..'"')
					else
						print(indent.."["..pos.."] => "..tostring(val))
					end
				end
			else
				print(indent..tostring(t))
			end
		end
	end
	if (type(t)=="table") then
		print(tostring(t).." {")
		sub_print_r(t,"  ")
		print("}")
	else
		sub_print_r(t,"  ")
	end
	print()
end
