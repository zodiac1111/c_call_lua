
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
me = { name = "Alice", ["age"] = 23}

-------- 例子 4 ----------
function main(data)
	-- 调试时打开,打印输入的数据
	-- print("in");
	-- print_r(data);

	-- 修改/计算
	-- data["name"] = "John"; -- 一般不需要修改名称,需要注意宿主语言的缓冲区大小
	data["fvalue"] = data["fvalue"] * 1000; -- 缩放等操作
	data["t"] = data["ivalue"] + data["ivalue2"]; -- 求和等算术操作
	data["ivalue"] = data["ivalue"] + 200; -- 偏置等操作
	data["ivalue"]=foo(data["ivalue"]);
	-- 调试时打开,打印输出的数据
	-- print("output");
	-- print_r(data);
	return 0, data;
endlocal

-- 简单内部函数例子, 输入值+1后返回
function foo(a)
	return a+1;
end

---- 辅助函数,打印table
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
