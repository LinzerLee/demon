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

-- ͨ��wrap֮��Ķ���ֻ��ʹ��;���ʳ�Ա
function WrapEventObject(table, id)
    -- �������滻��Ԫ��
	local mt = {}
	mt.__call = function(table, eventobject, ...)
		table.func(eventobject.id, ...)
	end

	-- ȥ��create����
	table["create"] = nil

	-- �����к����滻Ϊ��
	for k,v in pairs(table) do
		if type(v) == "function" then
			table[k] = { func = v }
			setmetatable(table[k], mt)
		end
	end


	-- ���id����
	table["id"] = id

	return table
end

eo = WrapEventObject(eo, 2)
eo:dialog("HHH", "NNN")
