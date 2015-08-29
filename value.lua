
function add(x, y)
 return x + y;
end

function foo(x)
 return 0;
end


--这里是用Lua代码定义的窗口大小的配置信息

--[[
lua中一下数据格式均合法
width = 20e02;
width = 0x10;
width = 1.2; --浮点数因为调用者的关系转化为整形
--]]
width = 200;
height = 300;
