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
local namespace = "EventObject"
local package = register(namespace)
--保旧环境
local _G = _G
--重置环境
setfenv(1, package)

function onEnterSceneTransitionDidFinish()
end

function onEnterScene()
local player = _G.Player.create("柳清风")
local item = _G.Item.create("普通宝箱")
_G.Item.setPos(item, 1744, 862.5)
local entity = _G.Entity.create("风草堆")
_G.Entity.setPos(entity, 1700, 862.5)
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
