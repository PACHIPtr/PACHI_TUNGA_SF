--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest support_system begin
    state start begin
        function get_pet_info(itemVnum)
            pet_info_map = {
            --  [ITEM VNUM] MOB_VNUM, DEFAULT NAME, buff_idx, spawn_effect_idx
                [81001]     = { 34077, "Şaman (Ejderha Gücü)", 0},
				[81002]		= { 34077, "Şaman (İyileştirme)", 0},
				[81003]		= { 34077, "Şaman (Ejderha Gücü)", 0},
				[81004]		= { 34077, "Şaman (İyileştirme)", 0},
				[81005]		= { 34077, "Şaman (Ejderha Gücü)", 0},
            }

            itemVnum = tonumber(itemVnum)

            return pet_info_map[itemVnum]
        end
		function get_spawn_effect_file(idx)
			effect_table = {
				[0] = nil,
				[1] = "d:\\\\ymir work\\\\effect\\\\etc\\\\appear_die\\\\npc2_appear.mse",
			}
			return effect_table [idx]
		end
        when 81001.use or 81002.use or 81003.use or 81004.use or 81005.use begin
			 local any_info = support_system.get_pet_info(item.vnum)

            if null != any_info then

                local mobVnum = any_info[1]
                local petName = any_info[2]
				local spawn_effect_file_name = support_system.get_spawn_effect_file(any_info[3])

                if true == supports.is_summon(mobVnum) then
					if spawn_effect_file_name != nil then
						supports.spawn_effect (mobVnum, spawn_effect_file_name)
					end
                    supports.unsummon(mobVnum)
					item.set_socket(1, 0);
                else
                    if supports.count_summoned() < 1 then
                        supports.summon(mobVnum, petName, false)
                    else
                        syschat("Zaten bir destek şamanı çağırmışsın.")
                    end
					if spawn_effect_file_name != nil then
						supports.spawn_effect (mobVnum, spawn_effect_file_name)
					end
                end
            end
        end
    end
end
