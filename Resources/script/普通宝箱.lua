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
local namespace = "普通宝箱"
local package = register(namespace)
--保旧环境
local _G = _G
--重置环境
setfenv(1, package)

-- 全局属性
名称 = "普通宝箱"
类型 = "宝箱"
物品 = {
金钱 = 300,
布头巾 = 1,
青铜盔甲 = 1,
}
形象 = {
帧池 = "M001",
帧集 = "1-1.png,1-2.png",
}
描述 = "只是一个再普通不过的箱子而已,没啥值钱的呦~"
-- 事件响应函数
function onSearched(item)
_G.Item.setSpriteFrame(item, "1-2.png")
end
