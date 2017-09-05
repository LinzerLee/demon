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
local namespace = "柳清风"
local package = register(namespace)
--保旧环境
local _G = _G
--重置环境
setfenv(1, package)

-- 全局属性
名称 = "柳清风"
类型 = "主角"
修行 = 1
经历 = 0
体力 = 150
真气 = 100
吉运 = 24
身法 = 21
武术 = 35
防御 = 32
物品 = {
	金钱 = 500,
}

法攻 = {
	帧池 = "magic",
	疾风劲草 = "55-1.png,55-2.png,55-3.png,55-4.png,55-5.png,55-6.png,55-7.png,55-8.png,55-9.png,55-10.png,55-11.png,55-12.png,55-13.png,55-14.png,55-15.png",
}

物攻 = {
	帧池 = "physical",
	攻击 = "33-1.png,33-8.png,33-9.png,33-10.png,33-11.png,33-1.png",
}

战斗状态 = {
	帧池 = "physical",
	正常 = "33-1.png",
	防御 = "33-4.png",
	受创 = "33-5.png",
	将死 = "33-2.png",
	死亡 = "33-3.png",
}

形象 = {
	帧池 = "player",
	北 = "2-7.png,2-8.png,2-9.png,2-7.png",
	南 = "2-1.png,2-2.png,2-3.png,2-1.png",
	西 = "2-4.png,2-5.png,2-6.png,2-4.png,",
	东 = "2-10.png,2-11.png,2-12.png,2-10.png",
}
描述 = "我就是主角,我就是有光环,你能咋地啊~"
