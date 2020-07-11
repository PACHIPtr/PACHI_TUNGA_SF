--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest horse_levelup begin
	state start begin
		when 50050.use begin
			if horse.get_level() >= 21 then
				syschat("Atýný daha fazla geliþtiremezsin.")
			elseif horse.is_dead() == true then
				syschat("Atýn ölü durumdayken onu geliþtiremezsin.")
			elseif pc.count_item(50050) < 1 then
				syschat("Envanterinde at madalyonu bulamadým.")
			elseif pc.is_busy() == true then
				syschat("Meþgulken bunu gerçekleþtiremezsin.")
			else
				item.set_count(item.get_count()-1)
				horse.advance()
				syschat(string.format("Atýnýn yeni seviyesi: %s",horse.get_level()))
			end
		end
	end
end