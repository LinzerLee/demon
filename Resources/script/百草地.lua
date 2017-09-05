--fix require bug
function require(package)
package = ScriptInterface.getFullPathForFileName(package)
print(package)
local fd = io.open(package, "r")
local c = fd:read("*all")
loadstring(c)()
fd:close()
end

require "script/Base.lua"
--注册包名
local namespace = "百草地"
local package = register(namespace)
--保旧环境
local _G = _G
--重置环境
setfenv(1, package)

地图 = "M001";
主角 = "柳清风"

物品 = {
	{
		名称 = "普通宝箱",
		位置 = "1744,862.5",
	},
	{
		名称 = "普通宝箱",
		位置 = "1704,862.5",
	},
}

怪物 = {
	{
		名称 = "风草堆",
		位置 = "1704,862.5",
	},
	{
		名称 = "风草堆",
		位置 = "1744,862.5",
	},
}

随机怪物 = {
	风草堆 = 0.3
}

NPC = {
	风草堆 = {
		位置 = "1744,862.5",
	},
}

function onEnterSceneTransitionDidFinish()
--local item = _G.Item.create("普通宝箱")
--_G.Item.setPos(item, 1744, 862.5)
--local entity = _G.Entity.create("风草堆")
--_G.Entity.setPos(entity, 1700, 862.5)
end

function onEnterScene()
end

function onExitScene()
end

function onEnterSceneTransitionDidStart()
end

function onSearched(eo)
end

function onEquiped()
end

function onUsed()
end

function update(eo, dt)
end
