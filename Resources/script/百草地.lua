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
--ע�����
local namespace = "�ٲݵ�"
local package = register(namespace)
--���ɻ���
local _G = _G
--���û���
setfenv(1, package)

��ͼ = "M001";
���� = "�����"

��Ʒ = {
	{
		���� = "��ͨ����",
		λ�� = "1744,862.5",
	},
	{
		���� = "��ͨ����",
		λ�� = "1704,862.5",
	},
}

���� = {
	{
		���� = "��ݶ�",
		λ�� = "1704,862.5",
	},
	{
		���� = "��ݶ�",
		λ�� = "1744,862.5",
	},
}

������� = {
	��ݶ� = 0.3
}

NPC = {
	��ݶ� = {
		λ�� = "1744,862.5",
	},
}

function onEnterSceneTransitionDidFinish()
--local item = _G.Item.create("��ͨ����")
--_G.Item.setPos(item, 1744, 862.5)
--local entity = _G.Entity.create("��ݶ�")
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
