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
local namespace = "�����"
local package = register(namespace)
--���ɻ���
local _G = _G
--���û���
setfenv(1, package)

-- ȫ������
���� = "�����"
���� = "����"
���� = 1
���� = 0
���� = 150
���� = 100
���� = 24
�� = 21
���� = 35
���� = 32
��Ʒ = {
	��Ǯ = 500,
}

���� = {
	֡�� = "magic",
	���羢�� = "55-1.png,55-2.png,55-3.png,55-4.png,55-5.png,55-6.png,55-7.png,55-8.png,55-9.png,55-10.png,55-11.png,55-12.png,55-13.png,55-14.png,55-15.png",
}

�﹥ = {
	֡�� = "physical",
	���� = "33-1.png,33-8.png,33-9.png,33-10.png,33-11.png,33-1.png",
}

ս��״̬ = {
	֡�� = "physical",
	���� = "33-1.png",
	���� = "33-4.png",
	�ܴ� = "33-5.png",
	���� = "33-2.png",
	���� = "33-3.png",
}

���� = {
	֡�� = "player",
	�� = "2-7.png,2-8.png,2-9.png,2-7.png",
	�� = "2-1.png,2-2.png,2-3.png,2-1.png",
	�� = "2-4.png,2-5.png,2-6.png,2-4.png,",
	�� = "2-10.png,2-11.png,2-12.png,2-10.png",
}
���� = "�Ҿ�������,�Ҿ����й⻷,����զ�ذ�~"
