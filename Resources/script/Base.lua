function import(package)
	for key, value in pairs (package) do
		if(_G[key] ~= nil) then
			error("Symbol " .. key .. "conflict!")
		end

		_G[key] = value
	end
end

function register(name)
	local package = {}
	_G[name] = package
	
	return package
end

-- 通过wrap之后的对象只能使用:访问成员
function WrapEventObject(table, id)
	local newObject = {}
	-- 做参数替换的元表
	local mt = {}
	mt.__call = function(table, eventobject, ...)
		table.func(eventobject.id, ...)
	end

	-- 将所有函数替换为表添加进新的对象中
	for k,v in pairs(table) do
		if type(v) == "function" then
			newObject[k] = { func = v }
			setmetatable(newObject[k], mt)
		end
	end
	
	-- 去掉create函数
	newObject["create"] = nil

	-- 添加id参数
	newObject["id"] = id

	return newObject
end

function debug_write_file(file ,content)
	local fd = _G.io.open(file, "w+")
	fd:write(content)
	fd:close()
end
