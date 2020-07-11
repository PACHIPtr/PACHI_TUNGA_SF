--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest enchanted_forest begin
    state start begin
		when logout with pc.get_map_index() == 353 begin
			pc.setf("enchanted_forest_zone", "dungeon_enter", 1)
			pc.setqf("limit_activity", get_time() + 10 * 60)
		end
		
		when login with pc.get_map_index() == 353 begin
			if get_time() >= pc.getqf("limit_activity") and pc.getqf("limit_activity") > 0 then
				pc.warp(816700, 1505000)
			else
				pc.setqf("limit_activity", 0)
			end
		end
	end
end