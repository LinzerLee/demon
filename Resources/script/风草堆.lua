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
local namespace = "��ݶ�"
local package = register(namespace)
--���ɻ���
local _G = _G
--���û���
setfenv(1, package)

-- ȫ������
���� = "��ݶ�"
���� = "����"
���� = 1
���� = 2
���� = 40
���� = 56
���� = 14
�� = 5
���� = 12
���� = 36
��Ʒ = {
	��Ǯ = 2,
	��ɽ����Ҷ = 1,
}

͵�� = {
	������ = 1,
}

������ = 1

���� = {
	֡�� = "monster",
	�� = "471-5.png,471-6.png,471-7.png,471-8.png",
	�� = "471-9.png,471-10.png,471-11.png,471-12.png",
	�� = "471-13.png,471-14.png,471-15.png,471-16.png,",
	�� = "471-1.png,471-2.png,471-3.png,471-4.png",
}

���� = {
	֡�� = "magic",
	���羢�� = "55-1.png,55-2.png,55-3.png,55-4.png,55-5.png,55-6.png,55-7.png,55-8.png,55-9.png,55-10.png,55-11.png,55-12.png,55-13.png,55-14.png,55-15.png",
}

�﹥ = {
	֡�� = "physical",
	���� = "100-1.png,100-2.png,100-3.png,100-4.png,100-5.png,100-6.png,100-7.png,100-8.png,100-9.png,100-10.png,100-11.png",
}

ս��״̬ = {
	֡�� = "physical",
	���� = "100-1.png",
	���� = "33-4.png",
	�ܴ� = "33-5.png",
	���� = "33-2.png",
	���� = "33-3.png",
}

������ = 1
׷���� = 3
�ٶ� = 0.5

��� = {
	��ݶ� = "1~3"
}

ս����ͼ = "B001"
���� = "��...,��ֻ�Ǹ�������?"

local path = {
	"��","��","��","��",
	"��","��","��","��",
	"��","��","��","��",
	"��","��","��","��",
}

local index = 0
local sum = 0
-- �¼���Ӧ����
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
			entity:dialog("��Һ�,���Ƿ�ݶ�", "lqf-10.png")
			entity:dialog("�~Ŷ~��~~~")
			entity:dialog("��Ҫ����㣬�¸£�����")
			
			_G.GameMainScene.battle(entity.id, "B001")
		end
	)
	_G.coroutine.resume(thread)
end
