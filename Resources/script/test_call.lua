dialog = {
	func = function(entity, content, protrait)
		print(entity.." : "..content)
	end
}
mt = {}
mt.__call = function(table, eventobject, ...)
	table.func(eventobject.id, ...)
end

setmetatable(dialog, mt)

--[[
eo = {
	id = 1,
	dialog = dialog,
}
eo:dialog("HHH", "NNN")
]]

eo = {
	dialog = function(entity, content, protrait)
		print(entity.." : "..content)
	end
}

-- 通过wrap之后的对象只能使用;访问成员
function WrapEventObject(table, id)
    -- 做参数替换的元表
	local mt = {}
	mt.__call = function(table, eventobject, ...)
		table.func(eventobject.id, ...)
	end

	-- 去掉create函数
	table["create"] = nil

	-- 将所有函数替换为表
	for k,v in pairs(table) do
		if type(v) == "function" then
			table[k] = { func = v }
			setmetatable(table[k], mt)
		end
	end


	-- 添加id参数
	table["id"] = id

	return table
end

eo = WrapEventObject(eo, 2)
eo:dialog("HHH", "NNN")
