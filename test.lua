print("testing dtree from Lua")

dtree = require("dtree")
succ, msg = pcall(dtree.open, "test/device-tree")

if succ then
	local dev = dtree.next()
	while dev ~= false do
		local name = dev.name
		local addr = string.format("0x%x..0x%x", dev.base, dev.high)
		local compat = dev.compat

		print(name .. " : " .. addr)

		for i, v in ipairs(compat) do
			print("", v)
		end

		dev = dtree.next()
	end

	dtree.close()
else
	print("error: " .. msg)
end
