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
local namespace = "��ͨ����"
local package = register(namespace)
--���ɻ���
local _G = _G
--���û���
setfenv(1, package)

-- ȫ������
���� = "��ͨ����"
���� = "����"
��Ʒ = {
��Ǯ = 300,
��ͷ�� = 1,
��ͭ���� = 1,
}
���� = {
֡�� = "M001",
֡�� = "1-1.png,1-2.png",
}
���� = "ֻ��һ������ͨ���������Ӷ���,ûɶֵǮ����~"
-- �¼���Ӧ����
function onSearched(item)
_G.Item.setSpriteFrame(item, "1-2.png")
end
