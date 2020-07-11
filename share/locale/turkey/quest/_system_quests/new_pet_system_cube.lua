--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest PetSystemIncubator begin
	state start begin
		when 55401.use or 55402.use or 55403.use or 55404.use or 55405.use or 55406.use or 55407.use or 55408.use or 55409.use or 55410.use begin
			newpet.EggRequest(item.get_vnum())
			cmdchat(string.format("OpenPetIncubator %d ", (item.get_vnum()-55401)))
		end
	end
end