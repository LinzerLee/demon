--fix require bug
function require(package)
package = ScriptInterface.getFullPathForFileName(package)
local fd = io.open(package, "r")
local c = fd:read("*all")
loadstring(c)()
fd:close()
end

require "script/Base.lua"
--ע�����
local namespace = "EventObject"
local package = register(namespace)
--���ɻ���
local _G = _G
--���û���
setfenv(1, package)

function onEnterSceneTransitionDidFinish()
end

function onEnterScene()
local player = _G.Player.create("�����")
local item = _G.Item.create("��ͨ����")
_G.Item.setPos(item, 1744, 862.5)
local entity = _G.Entity.create("��ݶ�")
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
