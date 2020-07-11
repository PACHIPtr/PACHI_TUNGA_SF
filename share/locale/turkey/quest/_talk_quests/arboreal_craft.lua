--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest arboreal_craft begin
	state start begin
		when 20413.chat."Geliþtirme Penceresini Aç " with pc.get_level() >= 90 begin
			setskin(NOWINDOW)
			command("cube open")
		end
	end
end