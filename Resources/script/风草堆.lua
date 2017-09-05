--fix require bug
function require(package)
	package = ScriptInterface.getFullPathForFileName(package)
	local fd = io.open(package, "r")
	local c = fd:read("*all")
	loadstring(c)()
	fd:close()
end

require "script/Base.lua"
--注册包名
local namespace = "风草堆"
local package = register(namespace)
--保旧环境
local _G = _G
--重置环境
setfenv(1, package)

-- 全局属性
名称 = "风草堆"
类型 = "怪物"
修行 = 1
经历 = 2
体力 = 40
真气 = 56
吉运 = 14
身法 = 5
武术 = 12
防御 = 36
物品 = {
	金钱 = 2,
	华山针树叶 = 1,
}

偷窃 = {
	两面针 = 1,
}

真气数 = 1

形象 = {
	帧池 = "monster",
	北 = "471-5.png,471-6.png,471-7.png,471-8.png",
	南 = "471-9.png,471-10.png,471-11.png,471-12.png",
	西 = "471-13.png,471-14.png,471-15.png,471-16.png,",
	东 = "471-1.png,471-2.png,471-3.png,471-4.png",
}

法攻 = {
	帧池 = "magic",
	疾风劲草 = "55-1.png,55-2.png,55-3.png,55-4.png,55-5.png,55-6.png,55-7.png,55-8.png,55-9.png,55-10.png,55-11.png,55-12.png,55-13.png,55-14.png,55-15.png",
}

物攻 = {
	帧池 = "physical",
	攻击 = "100-1.png,100-2.png,100-3.png,100-4.png,100-5.png,100-6.png,100-7.png,100-8.png,100-9.png,100-10.png,100-11.png",
}

战斗状态 = {
	帧池 = "physical",
	正常 = "100-1.png",
	防御 = "33-4.png",
	受创 = "33-5.png",
	将死 = "33-2.png",
	死亡 = "33-3.png",
}

触发域 = 1
追踪域 = 3
速度 = 0.5

伙伴 = {
	风草堆 = "1~3"
}

战斗地图 = "B001"
描述 = "额...,就只是个描述吗?"

local path = {
	"北","北","北","北",
	"西","西","西","西",
	"南","南","南","南",
	"东","东","东","东",
}

local index = 0
local sum = 0
-- 事件响应函数
function update(entity, dt)
	sum = sum + dt
	index = index % 16 + 1
	if sum > 0.2 then
		_G.Entity.go(entity, path[index])
		sum = 0
	end
end

function onSearched(entity)
	entity = _G.WrapEventObject(_G.Entity, entity)
	local thread = _G.coroutine.create(
		function ()
			entity:dialog("大家好,我是风草堆", "lqf-10.png")
			entity:dialog("喔~哦~噢~~~")
			entity:dialog("我要打败你，嘎嘎！！！")
			
			_G.GameMainScene.battle(entity.id, "B001")
		end
	)
	_G.coroutine.resume(thread)
end
