--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest PetSystems begin
	state start begin
		function get_pet_info(itemVnum)
			pet_info_map = {
			--  [ITEM VNUM] MOB_VNUM, DEFAULT NAME, buff_idx, spawn_effect_idx
				[55701] = { 34041, " - Küçük Örümcek", 0},
				[55702] = { 34045, " - Küçük Maymun", 0},
				[55703] = { 34049, " - Küçük Razador", 0},
				[55704] = { 34053, " - Küçük Nemere", 0},
				[55705] = { 34036, " - Küçük Ejderha", 0},
				[55706] = { 34064, " - Küçük Meley", 0},
				[55707] = { 34073, " - Küçük Azrail", 0},
				[55708] = { 34075, " - Küçük Cellatçýk", 0},
				[55709] = { 34080, " - Baashido Yavrusu", 0},
				[55710] = { 34082, " - Nessie Yavusu", 0},
			}
			itemVnum = tonumber(itemVnum)
			return pet_info_map[itemVnum]
		end
		
		when 55701.use or 55702.use or 55703.use or 55704.use or 55705.use or 55706.use or 55707.use or 55708.use or 55709.use or 55710.use begin
			local pet_info = PetSystems.get_pet_info(item.vnum)

			if null != pet_info then

				local mobVnum = pet_info[1]
				local petName = pet_info[2]
				

				if true == newpet.is_summon(mobVnum) then					
					newpet.unsummon(mobVnum)
				else
					if newpet.count_summoned() < 1 then
						newpet.summon(mobVnum, petName, false)
					else
						syschat("Zaten bir evcil hayvan çaðýrmýþsýn.")
					end					
				end -- if pet.is_summon
			end  -- if null != pet_info
		end -- when
	end
end