--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest pet_system begin
    state start begin
        function get_pet_info(itemVnum)
            pet_info_map = {
            --  [ITEM VNUM] MOB_VNUM, DEFAULT NAME, buff_idx, spawn_effect_idx
                [53001]     = { 34001, " - Anka Kuþu ", 0},
                [53002]     = { 34002, " - Ren Geyiði Yavrusu ", 0},
                [53003]     = { 34003, " - Buz Ankasý ", 0},
                [53005]     = { 34004, " - Yavru Azrail ", 0},
				[53006]		= { 34009, " - Yavru Azrail(altýn) ", 0},
                [53007]     = { 34010, " - Bambi ", 0},
				[53008]		= { 34011, " - Knuud ", 0},
				[53009]		= { 34012, " - Bao Bao ", 0},
                [53010]     = { 34008, " - Leonidas ", 0},
                [53011]     = { 34007, " - Khan ", 0},
                [53012]     = { 34005, " - Porki ", 0},
                [53013]     = { 34006, " - Rufus ", 0},
				[53014]     = { 34013, " - Cellatcýk ", 0},
				[53015]     = { 34014, " - Cellatcýk(altýn) ", 0},
				[53016]     = { 34015, " - Boney ", 0},
				[53017]     = { 34016, " - Yeþim Ankasý ", 0},
                [38200]     = { 34006, " - Rufus ", 0},
                [38201]     = { 34006, " - Rufus ", 0},
				[53018]     = { 34017, " - Kar Leonidas ", 0},
				[53019]     = { 34018, " - Arktik Khan ", 0},
				[53020]     = { 34019, " - Kutup Porki ", 0},
				[53021]     = { 34020, " - Ayaz Rufus ", 0},
				[53022]     = { 34021, " - Sheldon ", 0},
				[53023]     = { 34022, " - Cooper ", 0},
				[53024]     = { 34023, " - Panter Yavrusu ", 0},
				[53025]     = { 34024, " - Leopar Yavrusu ", 0},
				[53218]		= { 34023, " - Panter Yavrusu ", 0},
				[53219]		= { 34023, " - Panter Yavrusu ", 0},
				[53220]		= { 34024, " - Leopar Yavrusu ", 0},
				[53221]		= { 34024, " - Leopar Yavrusu ", 0},
				[53222]		= { 34026, " - Balkabaðý Kafasý ", 0},
				[53223]		= { 34027, " - Küçük Süpürgeci ", 0},
				[53224]		= { 34028, " - Kar Tanesi ", 0},
				[53225]		= { 34029, " - Çancýk ", 0},
				[53226]		= { 34030, " - Cupido ", 0},
				[53227]		= { 34031, " - Öfkeli Kalp ", 0},
				[53228]		= { 34033, " - Hilal ", 0},
				[53229]		= { 34032, " - Hilal ", 0},
				[53230] 	= { 34034, " - Beyaz Kuzucuk ", 0},
				[53231]		= { 34035, " - Mavi Koç ", 0},
				[53232]		= { 34039, " - Yavru Huashin ", 0},
				[53233]		= { 34055, " - Bruce ", 0},
				[53234]		= { 34056, " - Wayne ", 0},
				[53235]		= { 34057, " - Robin ", 0},
				[53236]		= { 34058, " - Valentino ", 0},
				[53237]		= { 34059, " - Valentinia ", 0},
				[53238]		= { 34060, " - Valentino ", 0},
				[53239]		= { 34061, " - Valentinia ", 0},
				[53240]		= { 34063, " - Ay Feneri ", 0},
				[53241]		= { 34062, " - Ay Feneri ", 0},
				[53242]		= { 34066, " - Bonsa ", 0},
				[53243]		= { 34066, " - Bonsa ", 0},
				[53244]		= { 34067, " - Bayan Jiangshi ", 0},
				[53245]		= { 34068, " - Bay Jiangshi ", 0},
				[53246]		= { 34069, " - Genç Jiangshi ", 0},
				[53247]		= { 34070, " - Kýrmýzý Elf ", 0},
				[53248]		= { 34071, " - Yeþil Elf ", 0},
				[53249]		= { 34072, " - Kýrmýzý Elf ", 0},
				[53250]		= { 34084, " - Donni ", 0},
				[53251]		= { 34085, " - Frank ", 0},
				[53253]		= { 34086, " - Hui ", 0},
				[53254]		= { 34087, " - Buh ", 0},
				[53255]		= { 34088, " - Grusella ", 0},
				[53256]		= { 34066, " - Bonsa ", 0},
				[53258]		= { 34090, " - Renkli Fener ", 0},
				[53259]		= { 34091, " - Yeþil Fener ", 0},
				[53260]		= { 34092, " - Osvald Bey ", 0},
				[53261]		= { 34093, " - Cobblepot Bey ", 0},
				[53262]		= { 34094, " - Chesterfield Bey ", 0},
				[53263]		= { 34095, " - Griffy ", 0},
				[53264]		= { 34096, " - Nogaj ", 0},
				[53268]		= { 34100, " - Lycan Maddäus ", 0},
				[53269]		= { 34101, " - Lycaninho ", 0},
				[53270]		= { 34102, " - Küçük Catenaccio ", 0},
				[56004]		= { 20523, " - Yeþil Ejder ", 0},
				[56005]		= { 20524, " - Tanrýça ", 0},
				[56006]		= { 20525, " - Usta ", 0},
				[56007]		= { 20526, " - Bilge Kedi ", 0},
				[56008]		= { 20527, " - Antaras ", 0},
				[56009]		= { 20528, " - Zýrhlý Kaplan ", 0},
				[56010]		= { 20529, " - Kanatlý Aslan ", 0},
				[40497]		= { 34150, " - Hilal ", 0},
            }

            itemVnum = tonumber(itemVnum)

            return pet_info_map[itemVnum]
        end
		function get_spawn_effect_file(idx)
			effect_table = {
				[0] = nil,
				[1] = "d:\\\\ymir work\\\\effect\\\\etc\\\\buff\\\\buff_item3.mse",
			}
			return effect_table [idx]
		end
		when 38200.use or 38201.use or 53001.use or 53002.use or 53003.use or 53015.use or 53005.use or 53006.use or 53007.use or 53008.use or 53009.use or 53010.use or 53011.use or 53012.use or 53016.use or 53014.use or 53013.use or 53017.use or 53018.use or 53019.use or 53020.use or 53021.use or 53022.use or 53023.use or 53024.use or 53025.use or 53222.use or 53223.use or 53224.use or 53225.use or 53226.use or 53227.use or 53228.use or 53229.use or 53230.use or 53231.use or 53232.use or 53233.use or 53234.use or 53235.use or 53236.use or 53237.use or 53238.use or 53239.use or 53240.use or 53241.use or 53242.use or 53243.use or 53244.use or 53245.use or 53246.use or 53247.use or 53248.use or 53249.use or 53250.use or 53251.use or 53256.use or 53218.use or 53219.use or 53220.use or 53221.use or 53253.use or 53254.use or 53255.use or 53258.use or 53259.use or 53260.use or 53261.use or 53262.use or 53263.use or 53264.use or 53268.use or 53269.use or 56004.use or 53270.use or 56005.use or 56006.use or 56007.use or 56008.use or 56009.use or 56010.use begin
			local pet_info = pet_system.get_pet_info(item.get_vnum())

            if null != pet_info then

                local mobVnum = pet_info[1]
                local petName = pet_info[2]
				local spawn_effect_file_name = pet_system.get_spawn_effect_file(pet_info[3])
				
                if true == pet.is_summon(mobVnum) then
					if spawn_effect_file_name != nil then
						pet.spawn_effect (mobVnum, spawn_effect_file_name)
					end
                    pet.unsummon(mobVnum)
					pc.setqf("pet_item_vnum",0)
                else
                    if pet.count_summoned() < 1 then
                        pet.summon(mobVnum, petName, false)
						if item.get_socket(2) == 0 then item.set_socket(2, 1) end
						pc.setqf("pet_item_vnum",item.get_vnum())
                    else
                        syschat(" Zaten bir evcil hayvan çaðýrdýn. ")
                    end
					if spawn_effect_file_name != nil then
						pet.spawn_effect(mobVnum, spawn_effect_file_name)
					end
                end -- if pet.is_summon
            end  -- if null != pet_info
        end -- when
    end -- state
end -- quest
