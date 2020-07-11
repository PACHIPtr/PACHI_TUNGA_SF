--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest okey_item_transfer begin
	state start begin
		when kill or login or logout or levelup begin
			if pc.count_item(79505) >= 24 then
				pc.remove_item(79505, 24)
				pc.give_item2(79506)
			end
		end
	end
end
