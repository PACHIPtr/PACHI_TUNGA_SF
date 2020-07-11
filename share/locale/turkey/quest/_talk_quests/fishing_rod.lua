--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest fishing_rod begin
	state start begin
		when 12000.take begin
			if item.get_vnum() >= 27833 and item.get_vnum() <= 27862 then
				local reward_vnum = item.get_vnum()+30
				pc.give_item2(reward_vnum)
				if item.get_count() > 1 then
					item.set_count(item.get_count()-1)
				else
					item.remove()
				end
			end
		end
	end
end