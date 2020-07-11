--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest devilcatacomb_zone begin
	state start begin

	function setting()
		return
		{

		["base"]={["x"] = 3072  , ["y"]= 12032 ,},
		["floor1_s"] = { ["x"] = 20, ["y"] = 20, },
		["floor1_e"] = { ["x"] = 500, ["y"] = 470,},
		["floor2_s"] = { ["x"] = 510, ["y"] = 18, },
		["floor2_e"] = { ["x"] = 978, ["y"] = 470,},
		["floor1_entry"] = {["x"] = 73, ["y"] = 63,},
		["floor2_entry"] = {["x"] = 550, ["y"] = 45,},
		["floor3_entry"] = {["x"] = 1378, ["y"] = 249,},
		["floor4_entry"] = {["x"] = 70, ["y"] = 592,},
		["floor5_entry"] = {["x"] = 846, ["y"] = 898},
		["floor6_entry"] = {["x"] = 1362, ["y"] = 666},
		["floor7_entry"] = {["x"] = 73, ["y"] = 1156},

		["map_idx"] = 216,
		["rag"] = 30311,

		["devil_king"] = 2591,

		["reapers_credit1"] = 30319,
		["reapers_credit2"] = 30320,
		["reapers_credit3"] = 76002,
		["reapers_credit4"] = 79001,

		["floor3_stone"] = 8038,  
		["floor3_stone_pos"] = {
				{1366,	150},
				{1366,	351},
				{1234,	365},
				{1234,	140},
				{1150,	135},
				{1130,	365},
				{1135,	253}
		},

		["mob_regen_file_path"] = "data/dungeon/devilcatacomb/",
		["floor4_regen_file"] = {"devilcatacomb_floor4_regen1.txt",
		                       "devilcatacomb_floor4_regen2.txt",
							   "devilcatacomb_floor4_regen3.txt",
							   "devilcatacomb_floor4_regen4.txt",
							   "devilcatacomb_floor4_regen5.txt"} ,

		["credit_stone"] = 30101,
		["credit_stone_pos"] = {307,323},
		["floor2_stone"] = 30103,
		["floor2_stone_pos"] = {741,217},
		["floor2_merchant_npc"] = 20368,
		["floor2_merchant_npc_pos"] = {734,214},
		
		["floor4_npc"] = 30104,
		["floor4_npc_pos"] = {500, 717},

		["devil_king_pos"] = {{673,829},{691,638},{848,568},{1026,642},{1008,862}},

		["unlock_stone"] = 30312,
		["floor5_stone"] = 30102,
		["floor5_stone_pos"] = {848, 735},

		["devil_great_king"] = 2597 ,
		["devil_great_king_pos"] = {1303,704},
		["devil_emperor"] = 2598,
		["devil_emperor_pos"] = {74,1103},

		["dc2_door_set_size"] = {4,2,3,2},
		["dc2_door_set1"] = {
								{{30111,    566,    117,    5},{30112,  562,    311,    5,},{30118, 663,    434,    7},{30119,  881,    434,    7}},
								{{30116,    743,    390,    3},{30119,  612,    251,    1,}},
								{{30113,    654,    211,    5},{30111,  707,    338,    7,},{30112, 775,    336,    7}},
								{{30117,    733,    294,    3},{30113,  694,    271,    1,}}
							},
		["dc2_door_set2"] = {
								{{30115,    942,    141,    5},{30116,  942,    245,    5},{30117,  942,    321,    5},{30115,  763,    64, 7}},
								{{30118,    643,    116,    3},{30114,  900,    167,    1}},
								{{30114,    850,    293,    5},{30113,  715,    164,    7},{30114,  817,    162,    7}},
								{{30111,    802,    277,    1},{30112,  800,    241,    1}}
							},
		}
	end
		
		when logout or disconnect begin
			local setting = devilcatacomb_zone.setting()
			local idx = pc.get_map_index()
			if idx >= (setting.map_idx * 10000) and idx < ((setting.map_idx + 1) * 10000) then
				if pc.getqf ("katakombfix") == 1 then
					pc.setqf ("katakombfix",0)
				end
			end
		end
		
		when login begin
		
			if pc.getqf ("katakombfix") == 1 then
				pc.setqf ("katakombfix",0)
			end
			
			local setting = devilcatacomb_zone.setting()
			local idx = pc.get_map_index()
			if idx >= (setting.map_idx * 10000) and idx < ((setting.map_idx + 1) * 10000) then
--				pc.set_warp_location(65, 5914, 992)         -----------¾Æ±Íµ¿±¼ ÁÂÇ¥·Î
				local level = d.getf("level")
				-- warp locationÀÌ 0,0 ÀÌ¸é ½Ã½ºÅÛ »óÀ¸·Î, Á¾·á ½Ã, Á¾·á À§Ä¡¸¦ ½ÃÀÛ À§Ä¡·Î ÀúÀåÇÑ´Ù.
				d.set_warp_location (0, 0, 0)
					
				if level == 2 then
					say("3. kata ulaşmak için bu kapıların mühürlerini sök!")
					say("60 dakikan daha var.")
					pc.setqf ("katakombfix",1)
				elseif level >= 3 then
					if pc.getqf ("katakombfix") != 1 then
						chat("Karakter atıp bug yaptığın için köye ışınlanacaksın !")
						warp_to_village()
						return
					end
					
				elseif level == pc.getqf ("level") then
--					say ("³«¿ÀµÇÁö ¾Ê±â À§ÇØ µ¿·áµé°ú ÇÕ·ùÇÏ½Ê½Ã¿À.")
					--say (locale.devilcatacomb_follow_peer)
				else
--					say (locale.devilcatacomb_dropout)
--					say ("´ç½ÅÀº ³«¿ÀµÇ¾ú½À´Ï´Ù. ¿ë½ÅÀÇ ÈûÀ» ºô¾î Àá½Ã µÚ µ¿±¼ ¹ÛÀ¸·Î ÀÌµ¿ÇÕ´Ï´Ù.")
--					timer ("devilcatacomb_ban_timer", 5)
				end
			end
			if idx == setting.map_idx then
				pc.setf("devilcatacomb_zone","last_exit_time", get_global_time())
				-- 2ÃşÀÌ»óÀÌ¸é, ÂÑ°Ü³ª
				if pc.get_x() < setting.floor1_s.x + setting.base.x
					or pc.get_y() < setting.floor1_s.y + setting.base.y
					or pc.get_x() > setting.floor1_e.x + setting.base.x
					or pc.get_y() > setting.floor1_e.y + setting.base.y
				then
					-- ¾Æ±Íµ¿±¼ ÀÔ±¸·Î
--					if not is_test_server() then
						--say (locale.devilcatacomb_dropout)
						timer ("devilcatacomb_ban_timer", 5)
--					end
				else 
					-- ¾Æ±Íµ¿±¼ ÀÔ±¸·Î
--					pc.set_warp_location(65, 5914, 992)
					say("Ruh Kristali Anahtarı'nı ele geçirmek için ")
					say("yaratıkları yok et. Bu ")
					say("sana sonraki katın kapısını açar. ")
					pc.remove_item(30311, pc.count_item(30311))
					pc.remove_item(30312, pc.count_item(30312))
				end
			end
		end

		when logout begin
			local idx = pc.get_map_index()
			local setting = devilcatacomb_zone.setting()
			if idx == setting.map_idx or idx >= (setting.map_idx * 10000) and idx < ((setting.map_idx + 1) * 10000) then
				pc.setf("devilcatacomb_zone","last_exit_time", get_global_time())
			end
		end

		when devilcatacomb_ban_timer.timer begin
			pc.warp((5914)*100, (992)*100, 65)
		end

		when warp_timer.timer begin
			local level = d.getf("level")
			local level_switch = {
				[3] = "Doğru Metin Taşı'nı bul ve onu yok et!",
				[4] = "5. aşamaya ulaşmak için labirentten çıkış yolunu bul!",
				[5] = "Tartarus'u yen ve Surat Totemi'ni al.",
				[6] = "Kharoon ve cellatlarını yen!",
			}
			local t = 3600 -(get_global_time() - d.getf("enter_time"))
			local t_r = math.mod (t, 60)
			local t_m = (t - t_r) / 60
			d.notice ( level_switch[level] )
			d.notice ( "" ..t_m.." dakikan daha var.")
			d.set_warp_location (0, 0, 0)
		end

		when 20367.chat."Şeytan Katakombu'na gider" with pc.get_map_index() == 65 begin
			addimage(35,35,'catacomb.tga')
			local setting = devilcatacomb_zone.setting()
			if pc.get_level() < 75 then
				say_title("Katakomb Bekçisi:")
				say("")
				say("")
				say("")
				say("")
				say("")
                say("75. seviyenin altında olduğun için ")
                say("Katakomb'a giremezsin...")
			--elseif pc.getf("deviltower_zone","9_done") == 0 then
			--	say(locale.devilcatacomb_man_say_deviltop_clear)
			--elseif ((get_global_time() - pc.getf("devilcatacomb_zone","last_exit_time")) < 1800) then
			--	say(locale.devilcatacomb_man_say_see_you_later)
			else
				say_title("Katakomb Bekçisi:")
				addimage(35,35,'catacomb.tga')
				say("")
				say("")
				say("")
				say("")
				say("")
				say("Şeytan Katakombu'na girmek")
				say("istiyor musun?")
                local s = select("Evet, bırak içeri gireyim!","Hayır, girmek istemiyorum.")
				if s == 1 then
					pc.warp ((setting.floor1_entry.x + setting.base.x )* 100, (setting.floor1_entry.y + setting.base.y) * 100)
				end
			end
		end
		
		when 20367.chat."Karanlık Tapınağa dön" with pc.get_map_index() == 216 begin
			say_title("Katakomb Bekçisi:")
			say("")
			say("Savaşmaktan vazgeçip Karanlık Tapınağa dönmek")
			say("istediğine emin misin?")
			say("")
			local s = select("Evet, çok güçsüzüm.", "Hayır, savaşmaya devam edeceğim!")
			if s == 1 then
				pc.warp(591400, 99900)
			end
		end
	
		-- ¸ó½ºÅÍ°¡ Á×À¸¸é ¹ß»ıÇÏ´Â ÀÌº¥Æ®.
		when kill begin
			local setting = devilcatacomb_zone.setting()
			-- ¾Æ±Íµ¿±¼ 1ÃşÀÌ¸é 1% È®·ü·Î ±İ½Ã°ÇÀ» µå¶øÇÑ´Ù.
			if pc.get_x() > setting.floor1_s.x + setting.base.x
					and pc.get_y() > setting.floor1_s.y + setting.base.y
					and pc.get_x() < setting.floor1_e.x + setting.base.x
					and pc.get_y() < setting.floor1_e.y + setting.base.y then
				local j = number(1, 100)
				if j == 1 then
					game.drop_item (setting.rag, 1)
				end
			end
		end

	function spawn_doors ()
		local setting = devilcatacomb_zone.setting()
		for i = 1, 4 do
			local mob_set = setting.dc2_door_set1[i]
			local set_size = setting.dc2_door_set_size[i]
			for j = 1, set_size do
				d.set_unique (i.."_1_"..j, d.spawn_mob_dir (mob_set[j][1], mob_set[j][2], mob_set[j][3], mob_set[j][4]))
			end
			mob_set = setting.dc2_door_set2[i]
			for j = 1, set_size do
				d.set_unique (i.."_2_"..j, d.spawn_mob_dir (mob_set[j][1], mob_set[j][2], mob_set[j][3], mob_set[j][4]))
			end
		end
	end
		-- ±İ½Ã°ÇÀ» ³ªÂûÀÎÁõ¼®¿¡ µå·¡±×ÇÏ¸é 2ÃşÀ¸·Î º¸³½´Ù. 2ÃşºÎÅÍ´Â ´øÀüÀÌ´Ù.
		when 30101.take  begin
			local setting = devilcatacomb_zone.setting()
		
			if item.vnum == setting.rag then 
				if not party.is_party() then
					say("Katakombun daha alt aşamalarına geçmek için")
					say("güçlü bir gruba ihtiyacın var.")
					say("")
					return
				elseif party.is_in_dungeon() then
					say("Senin grubun zaten zindanda.")
					say("")
					return
				end

				item.remove()
				say("Mühürlü kapı açıldı. Acele et,")
                say("aşağı iniyoruz.")
                wait()
                say("Asıl macera şimdi başlıyor. Şeytan ")
                say("Katakombu'nu keşfetmek için sadece")
                say("bir saatin var.")
				wait()
				d.new_jump_party (setting.map_idx, setting.floor2_entry.x + setting.base.x  , setting.floor2_entry.y + setting.base.y )
				d.setqf2("devilcatacomb_zone","level", 2)
				d.setf ("level",2)
				--d.regen_file (setting.mob_regen_file_path.."dc_2f_regen.txt")
				d.regen_file(get_locale_base_path().."/map/metin2_map_devilcatacomb/base2_regen.txt")
				--d.regen_file(get_locale_base_path().."/map/metin2_map_devilcatacomb/base2_npc.txt")
				
				-- d.spawn_mob (setting.floor2_merchant_npc, setting.floor2_merchant_npc_pos[1], setting.floor2_merchant_npc_pos[2])
				d.spawn_mob (setting.floor2_stone, setting.floor2_stone_pos[1], setting.floor2_stone_pos[2])
				
				devilcatacomb_zone.spawn_doors ()
				server_timer('devilcatacomb_45m_left_timer',60 * 15, d.get_map_index())
				d.setf("enter_time", get_global_time())
			end	
		end
	
		when devilcatacomb_45m_left_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.notice ("45 dakikan daha var.")
				server_timer('devilcatacomb_30m_left_timer', 60 * 15, get_server_timer_arg())
			end
		end

		when devilcatacomb_30m_left_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.notice ("30 dakikan daha var.")
				server_timer('devilcatacomb_15m_left_timer', 60 * 15, get_server_timer_arg())
			end
		end
	
		when devilcatacomb_15m_left_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.notice ("15 dakikan daha var.")
				server_timer('devilcatacomb_5m_left_timer', 60 * 10, get_server_timer_arg())
			end
		end

		when devilcatacomb_5m_left_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.notice ("5 dakikan daha var.")
				server_timer('devilcatacomb_1m_left_timer', 60 * 4, get_server_timer_arg())
			end
		end
		
		when devilcatacomb_1m_left_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.notice ("1 dakikan daha var.")
				server_timer ("devilcatacomb_0m_left_timer", 60 * 1, get_server_timer_arg())
			end
		end
		
		when devilcatacomb_0m_left_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.notice("Süre bitti. Kendi güvenliğin için Devil's Catacomb'dan dışarı ışınlanacaksın.")
				d.set_warp_location (65, 5914, 992)
				server_timer('devilcatacomb_exit_timer', 7, get_server_timer_arg())
			end
		end
	
	function is_dc2_doors_clear ()
		local setting = devilcatacomb_zone.setting()
		local clear = true
		for i = 1, 4 do
			local set_clear = true
			local mob_set = setting.dc2_door_set1[i]
			local set_size = setting.dc2_door_set_size[i]
			for j = 1, set_size do
				set_clear = set_clear and d.is_unique_dead (i.."_1_"..j)
			end
			if not set_clear then
				set_clear = true
				mob_set = setting.dc2_door_set2[i]
				for j = 1, set_size do
					set_clear = set_clear and d.is_unique_dead (i.."_2_"..j)
				end
			end
			clear = clear and set_clear
		end

		return clear
	end
	
		-- ÇöÈ²¼®À» Å¬¸¯ÇÏ¸é 3Ãş ÁøÀÔ ¿©ºÎ¸¦ ¼±ÅÃÇÒ ¼ö ÀÖ°í
		-- ÁøÀÔ½Ã »ç½ÅÀÇÁõÇ¥¸¦ °Ë»çÇÑ´Ù.
		-- »ç½ÅÀÇ ÁõÇ¥°¡ ¾ø´Ù¸é ¹ÛÀ¸·Î ¿öÇÁµÈ´Ù.
		when 30103.click begin
			local setting = devilcatacomb_zone.setting()
			
			if d.getf("devilcatacomb_floor2_stone_clicked") == 0 then
				--if not devilcatacomb_zone.is_dc2_doors_clear () and not is_test_server() then
					--say_title ("The Devil´s Catacomb:")
					--say ("You first have to kill each door.")
					--say ("")
					--wait ()
					--return
				--end
				say("Kaplumbağa Kayalığı'na yaklaştıkça ")
				say("bazı gravürler dikkat çekiyor... ")
				wait()
				say("...Sadece Küçülen Kafa'ya sahip olanlar")
				say("bir sonraki kademeye geçebilirler. ")
				say("Küçülen Kafa'ya sahip değilsen yer yüzüne")
				say("ışınlanacaksın.")
				wait()
				local s = select("Evet, geçeceğim!", "Hayır, geçmek istemiyorum.")
				if s == 1 then
					if d.getf("devilcatacomb_floor2_stone_clicked") == 0 then
						d.setf("devilcatacomb_floor2_stone_clicked", 1)
						d.setqf2("devilcatacomb_zone","last_exit_time", get_global_time())
						d.set_item_group ("reapers_credit", 4, setting.reapers_credit4, 1, setting.reapers_credit3, 1, setting.reapers_credit2, 1, setting.reapers_credit1, 1)
						d.say_diff_by_item_group ("reapers_credit", "Küçülen Kafa'ya sahipsin ve artık 3. kata[ENTER]ışınlanabilirsin.[ENTER][ENTER]", "Küçülen Kafa'ya sahip değilsin.[ENTER]Şimdi Şeytan Katakombunun dışına[ENTER]ışınlanacaksın.[ENTER][ENTER]")
						timer ("devilcatacomb_floor3_enter_timer", 7)
					end
				end
			else
				say_title("Devil´s Catacomb:")
				say ("")
				say ("Lütfen bekle.")
				say ("")
			end
		end
		
		when devilcatacomb_floor3_enter_timer.timer begin
			local setting = devilcatacomb_zone.setting()
			d.set_warp_location (65, 5914, 992)
			d.exit_all_by_item_group ("reapers_credit")
			d.delete_item_in_item_group_from_all ("reapers_credit",1)
			d.purge ()
			d.jump_all (setting.floor3_entry.x + setting.base.x , setting.floor3_entry.y + setting.base.y)
			d.setf("level",3)
			d.setqf2("devilcatacomb_zone","level", 3)
			--d.regen_file (setting.mob_regen_file_path.."dc_3f_regen.txt")
			d.regen_file(get_locale_base_path().."/map/metin2_map_devilcatacomb/base3_regen.txt")
			
			timer("warp_timer",3)
			
			local position = setting.floor3_stone_pos
		
			local n = number(1,7)
			for i = 1, 7 do
				if (i != n)
				then
					d.set_unique("fake"..i, d.spawn_mob( setting.floor3_stone, position[i][1], position[i][2]))
				end
			end
		
			local vid = d.spawn_mob( setting.floor3_stone, position[n][1], position[n][2])
			d.set_unique ("real",vid)
			if (is_test_server()) then
				-- real stone pos
				chat (position[n][1],position[n][2])
			end
	
		end

		when kill with npc.get_race() == 8038 begin
			local setting = devilcatacomb_zone.setting()
			if d.is_unique_dead("real") then
				d.notice ("Gerçek Metin Taşı'nı yok ettin! Sonraki kata geçiyorsun.")
				d.setf ("bug_devam",0)
				d.setf ("level",4)
				d.setqf2("devilcatacomb_zone","level", 4)
				timer ("devilcatacomb_floor4_enter_timer", 3)
			else
				d.notice ("Yanlış Metin Taşı'nı yok ettin. Doğrusunu bul!")
			end
		end

		when devilcatacomb_floor4_enter_timer.timer begin
			local setting = devilcatacomb_zone.setting()
			local i = number (1, 4)
			timer("warp_timer",3)
			d.purge ()
			d.jump_all (setting.floor4_entry.x + setting.base.x , setting.floor4_entry.y + setting.base.y)
			--d.regen_file (setting.mob_regen_file_path.."dc_4f_regen.txt")
			--d.regen_file (setting.mob_regen_file_path..setting.floor4_regen_file[i])
			d.regen_file(get_locale_base_path().."/map/metin2_map_devilcatacomb/base4_regen.txt")
			d.spawn_goto_mob(70,673 , 94,844)
				if i == 1 then
				d.spawn_goto_mob(94,839 , 70,673)
				d.spawn_goto_mob(141,865 , 369,893)
				d.spawn_goto_mob(364,893 , 136,865)
				d.spawn_goto_mob(431,935 , 194,867)
				d.spawn_goto_mob(194,862 , 426,935)
				d.spawn_goto_mob(243,853 , 277,637)
				d.spawn_goto_mob(282,637 , 243,858)
				d.spawn_goto_mob(181,667 , 323,779)
				d.spawn_goto_mob(328,779 , 499,811)
				d.spawn_goto_mob(131,668, 345,560)
				d.spawn_goto_mob(345,55 , 131,663)
				d.spawn_goto_mob(180,566 , 401,563)
				d.spawn_goto_mob(401,558 , 185,566)
				d.spawn_goto_mob(296,568 , 67,810)
				d.spawn_goto_mob(67,815 , 291,568)
				d.spawn_goto_mob(140,822 , 356,717)
				d.spawn_goto_mob(351,717 , 140,817)
				d.spawn_goto_mob(189,711 , 414,712)
				d.spawn_goto_mob(419,712 , 189,716)
				d.spawn_goto_mob(274,712 , 369,776)
				d.spawn_goto_mob(364,776 , 274,717)
				d.spawn_goto_mob(224,779 , 366,814)
				d.spawn_goto_mob(366,809 , 229,779)
				d.spawn_goto_mob(274,842 , 352,839)
				d.spawn_goto_mob(347,839 , 274,837)
				d.spawn_goto_mob(429,841 , 70,577)
				d.spawn_goto_mob(55,864 , 55,920)
				d.spawn_goto_mob(50,920 , 60,864)
				d.spawn_goto_mob(144,919 , 90,943)
				d.spawn_goto_mob(90,948 , 139,919)
				d.spawn_goto_mob(312,852 , 406,876)
				d.spawn_goto_mob(406,871 , 312,857)
				elseif i == 2 then
				d.spawn_goto_mob(70,673 , 312,857)
				d.spawn_goto_mob(312,852 , 70,668)
				d.spawn_goto_mob(431,935 , 90,943)
				d.spawn_goto_mob(90,948 , 426,935)
				d.spawn_goto_mob(144,919 , 424,841)
				d.spawn_goto_mob(429,841 , 291,568)
				d.spawn_goto_mob(296,568 , 70,577)
				d.spawn_goto_mob(181,667 , 499,811)
				d.spawn_goto_mob(131,668, 345,560)
				d.spawn_goto_mob(345,55 , 131,663)
				d.spawn_goto_mob(180,566 , 401,563)
				d.spawn_goto_mob(401,558 , 185,566)
				d.spawn_goto_mob(282,637 , 67,810)
				d.spawn_goto_mob(67,815 , 277,637)
				d.spawn_goto_mob(140,822 , 189,716)
				d.spawn_goto_mob(189,711 , 140,817)
				d.spawn_goto_mob(274,712 , 229,779)
				d.spawn_goto_mob(224,779 , 274,717)
				d.spawn_goto_mob(328,779 , 274,837)
				d.spawn_goto_mob(274,842 , 323,779)
				d.spawn_goto_mob(351,717 , 194,867)
				d.spawn_goto_mob(194,862 , 356,717)
				d.spawn_goto_mob(419,712 , 243,858)
				d.spawn_goto_mob(243,853 , 414,712)
				d.spawn_goto_mob(364,776 , 366,814)
				d.spawn_goto_mob(366,809 , 369,776)
				d.spawn_goto_mob(347,839 , 70,673)
				d.spawn_goto_mob(94,839 , 60,864)
				d.spawn_goto_mob(55,864 , 94,844)
				d.spawn_goto_mob(141,865 , 55,920)
				d.spawn_goto_mob(50,920 , 136,865)
				d.spawn_goto_mob(364,893 , 406,876)
				d.spawn_goto_mob(406,871 , 369,893)
				elseif i == 3 then
				d.spawn_goto_mob(70,673 , 312,857)
				d.spawn_goto_mob(312,852 , 70,668)
				d.spawn_goto_mob(431,935 , 274,717)
				d.spawn_goto_mob(274,712 , 426,935)
				d.spawn_goto_mob(224,779 , 140,817)
				d.spawn_goto_mob(140,822 , 229,779)
				d.spawn_goto_mob(189,711 , 94,844)
				d.spawn_goto_mob(94,839 , 189,716)
				d.spawn_goto_mob(144,919 , 424,841)
				d.spawn_goto_mob(429,841 , 499,811)
				d.spawn_goto_mob(131,668 , 185,566)
				d.spawn_goto_mob(180,566 , 131,663)
				d.spawn_goto_mob(296,568 , 277,637)
				d.spawn_goto_mob(282,637 , 291,568)
				d.spawn_goto_mob(181,667 , 345,560)
				d.spawn_goto_mob(345,555 , 186,667)
				d.spawn_goto_mob(401,558 , 67,810)
				d.spawn_goto_mob(67,815 , 401,563)
				d.spawn_goto_mob(328,779 , 274,837)
				d.spawn_goto_mob(274,842 , 323,779)
				d.spawn_goto_mob(351,717 , 369,776)
				d.spawn_goto_mob(364,776 , 356,717)
				d.spawn_goto_mob(419,712 , 366,814)
				d.spawn_goto_mob(366,809 , 414,712)
				d.spawn_goto_mob(347,839 , 70,577)
				d.spawn_goto_mob(55,864 , 55,920)
				d.spawn_goto_mob(50,920 , 60,864)
				d.spawn_goto_mob(141,865 , 90,943)
				d.spawn_goto_mob(90,948 , 136,865)
				d.spawn_goto_mob(194,862 , 369,893)
				d.spawn_goto_mob(364,893 , 194,867)
				d.spawn_goto_mob(243,853 , 406,876)
				d.spawn_goto_mob(406,871 , 243,858)
				elseif i == 4 then
				d.spawn_goto_mob(131,668 , 67,810)
				d.spawn_goto_mob(67,815 , 185,566)
				d.spawn_goto_mob(282,637 , 274,717)
				d.spawn_goto_mob(274,712 , 277,637)
				d.spawn_goto_mob(274,842 , 94,844)
				d.spawn_goto_mob(94,839 , 274,837)
				d.spawn_goto_mob(144,919 , 499,811)
				d.spawn_goto_mob(70,673 , 345,560)
				d.spawn_goto_mob(345,555 , 70,668)
				d.spawn_goto_mob(401,558 , 70,577)
				d.spawn_goto_mob(296,568 , 186,667)
				d.spawn_goto_mob(181,667 , 291,568)
				d.spawn_goto_mob(140,822 , 189,716)
				d.spawn_goto_mob(189,711 , 140,817)
				d.spawn_goto_mob(224,779 , 323,779)
				d.spawn_goto_mob(328,779 , 229,779)
				d.spawn_goto_mob(351,717 , 352,839)
				d.spawn_goto_mob(347,839 , 356,617)
				d.spawn_goto_mob(419,712 , 424,841)
				d.spawn_goto_mob(429,841 , 414,712)
				d.spawn_goto_mob(364,776 , 366,814)
				d.spawn_goto_mob(366,809 , 369,776)
				d.spawn_goto_mob(55,864 , 194,867)
				d.spawn_goto_mob(194,862 , 60,864)
				d.spawn_goto_mob(141,865 , 90,943)
				d.spawn_goto_mob(90,948 , 136,865)
				d.spawn_goto_mob(50,920 , 369,893)
				d.spawn_goto_mob(364,893 , 55,920)
				d.spawn_goto_mob(243,853 , 406,876)
				d.spawn_goto_mob(406,871 , 243,858)
				d.spawn_goto_mob(312,852 , 426,935)
				d.spawn_goto_mob(431,935 , 312,857)
				end
			
			if (is_test_server()) then
				d.notice (setting.floor4_regen_file[i])
			end
			d.spawn_mob (setting.floor4_npc, setting.floor4_npc_pos[1], setting.floor4_npc_pos[2])
		end

		-- devilcatacomb_floor4_stone´Â Å¬¸¯ ½Ã 4Ãş¿¡¼­ 5ÃşÀ¸·Î ÀÌµ¿½ÃÅ°´Â npcÀÌ´Ù.
		when 30104.click begin
			local setting = devilcatacomb_zone.setting()
			local n = number (1,5)
			say("Başardın! Labirentin çıkışı burada. 5. kata in.")
			wait()
			if d.getf("bug_devam") == 1 then
				say_reward("Bu işlem daha önce yapıldı!")
				return
			end
			if (is_test_server()) then
				chat (setting.devil_king_pos[n][1], setting.devil_king_pos[n][2])
			end
			d.setf ("bug_devam",1)
			d.purge ()
			d.jump_all (setting.floor5_entry.x + setting.base.x, setting.floor5_entry.y + setting.base.y )
			d.setf ("level",5)
			d.setqf2("devilcatacomb_zone","level", 5)
			timer("warp_timer",3)
			d.regen_file(get_locale_base_path().."/map/metin2_map_devilcatacomb/base5_regen.txt")
			d.spawn_mob (setting.devil_king, setting.devil_king_pos[n][1], setting.devil_king_pos[n][2])
			d.spawn_mob (setting.floor5_stone, setting.floor5_stone_pos[1], setting.floor5_stone_pos[2])
		end
		
		-- ¾Æ±Í¿Õ(2591)ÀÌ Á×À¸¸é ºÀÀÎÇØÃ¼¼® µå¶ø
		when kill with npc.get_race() == 2591 begin
			local setting = devilcatacomb_zone.setting()
			game.drop_item (setting.unlock_stone, 1)
		end

		-- »ç½Å±İÁ¦ºñ(30102)¿¡ ±İÁ¦ÇØÃ¼¼®ºÀÀ» µå·¡±×ÇÏ¸é ¸ğµç ¸ó½ºÅÍ¸¦ Á×ÀÌ°í
		-- 6ÃşÀ¸·Î ÀÌµ¿ÇÏ´Â Å¸ÀÌ¸Ó¸¦ µ¿ÀÛ½ÃÅ²´Ù.
		when 30102.take with item.vnum == 30312 begin
			local setting = devilcatacomb_zone.setting()
				item.remove ()
				d.notice("Şeytan Katakombu'nun derinliklerinden tüyler ")
				d.notice("ürperten bir çığlık duyuldu. Kharoon uyandı! ")
				-- 6ÃşÀ¸·Î ÀÌµ¿
				d.clear_regen()
				d.purge()
				timer("devilcatacomb_floor6_timer",2)
		end

		when devilcatacomb_floor6_timer.timer begin
			local setting = devilcatacomb_zone.setting()
			d.setf("level",6)
			d.setqf2("devilcatacomb_zone","level", 6)
			d.jump_all (setting.floor6_entry.x + setting.base.x, setting.floor6_entry.y + setting.base.y )
			--d.regen_file (setting.mob_regen_file_path.."dc_6f_regen.txt")
			d.regen_file(get_locale_base_path().."/map/metin2_map_devilcatacomb/base6_regen.txt")
			d.spawn_mob (setting.devil_great_king, setting.devil_great_king_pos[1], setting.devil_great_king_pos[2])
			timer("warp_timer",3)
		end
		
		-- ¾Æ°É(2597)ÀÌ Á×À¸¸é, 6ÃşÀÇ Å¸ÀÌ¸Ó1ÀÌ µ¿ÀÛÇÑ´Ù.
		when kill with npc.get_race() == 2597 begin
			server_timer ("floor6_eli_check_timer_1", 5,d.get_map_index())
		end

		-- 6ÃşÀÇ Å¸ÀÌ¸Ó1Àº ´øÀüÀÇ ¸ğµç ¸ó½ºÅÍ ¼ıÀÚ¸¦ º¸°í,
		-- 0ÀÌ¸é 7ÃşÀ¸·Î ¿öÇÁÇÏ´Â Å¸ÀÌ¸Ó¸¦ µ¿ÀÛÇÑ´Ù.
		-- ¹Ù·Î 7ÃşÀ¸·Î °¡Áö ¾Ê´Â ÀÌÀ¯´Â, À¯Àú¿¡°Ô ÁØºñÇÒ ½Ã°£À» ÁÖ±â À§ÇØ¼­ÀÌ´Ù.
		-- 1ÀÌ¸é Å¸ÀÌ¸Ó2¸¦ µ¿ÀÛ½ÃÅ²´Ù.
		-- Å¸ÀÌ¸Ó1À» recursiveÇÏ°Ô µ¿ÀÛ½ÃÅ°Áö ¾ÊÀº °ÍÀº,
		-- ±×·² °æ¿ì resetÇÑ Å¸ÀÌ¸Ó1ÀÌ µ¿ÀÛÇÏÁö ¾Ê¾Ò±â ¶§¹®ÀÌ´Ù.
		when floor6_eli_check_timer_1.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() == 0 then
					say_in_map(get_server_timer_arg(), color256(255, 230, 186).."The Devil´s Catacomb:[ENTER]"..color256(196, 196, 196).."Kharoon'u yendin! Şimdi Şeytan Katakombu'nun en[ENTER]son katına ışınlanacaksın.")
					server_timer ("devilcatacomb_floor7_timer", 3, get_server_timer_arg())
				else
					server_timer ("floor6_eli_check_timer_2", 5, get_server_timer_arg())
				end
			end
		end

		when floor6_eli_check_timer_2.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() == 0 then
					say_in_map(get_server_timer_arg(), color256(255, 230, 186).."Kharoon'u yendin! Şimdi Şeytan Katakombu'nun en[ENTER]son katına ışınlanacaksın.")
					server_timer("devilcatacomb_floor7_timer", 3, get_server_timer_arg())
				else
					server_timer ("floor6_eli_check_timer_1", 5, get_server_timer_arg())
				end
			end
		end
		
		when devilcatacomb_floor7_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				local setting = devilcatacomb_zone.setting()

				d.setf("level",7)
				d.setqf2("devilcatacomb_zone","level", 7)
				d.jump_all (setting.floor7_entry.x + setting.base.x, setting.floor7_entry.y + setting.base.y )
				--d.regen_file (setting.mob_regen_file_path.."dc_7f_regen.txt")
				d.regen_file(get_locale_base_path().."/map/metin2_map_devilcatacomb/base7_regen.txt")
				d.spawn_mob (setting.devil_emperor, setting.devil_emperor_pos[1], setting.devil_emperor_pos[2])
				server_timer ("devilcatacomb_floor7_say_timer", 2, get_server_timer_arg())
			end	
		end

		when devilcatacomb_floor7_say_timer.server_timer begin
			if d.select(get_server_timer_arg()) then

				local t = 3600 -(get_global_time() - d.getf("enter_time"))
				local t_r = math.mod (t, 60)
				local t_m = (t - t_r) / 60
				d.notice("Azrail'i yen!")
				d.notice ("".. t_m.." dakikan daha var.")
				d.set_warp_location (0, 0, 0)
			end
		end

		-- 6Ãş°ú ¸¶Âù°¡Áö·Î »ç¸¶ÈÖ(2598)ÀÌ Á×À¸¸é
		-- 7ÃşÀÇ Å¸ÀÌ¸Ó°¡ ÀÛµ¿ÇÑ´Ù.
		when kill with npc.get_race() == 2598 begin
			--notice_all(pc.get_name().."'s party has defeated Lord Gahnasel!")
			server_timer ("floor7_eli_check_timer_1", 5, d.get_map_index())
		end
		
		-- 7ÃşÀÇ Å¸ÀÌ¸Ó´Â 6Ãş Å¸ÀÌ¸Ó¿Í ºñ½ÁÇÏ¸ç,
		-- ¸ó½ºÅÍ ¼ıÀÚ°¡ 0ÀÌ¸é exit Å¸ÀÌ¸Ó¸¦ ¹ß»ı½ÃÅ²´Ù.
		-- ¹Ù·Î exitÇÏÁö ¾Ê´Â ÀÌÀ¯´Â, À¯Àúµé¿¡°Ô ÁØºñÇÒ ½Ã°£À» ÁÖ±â À§ÇØ¼­ÀÌ´Ù.
		when floor7_eli_check_timer_1.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() == 0 then
					d.set_warp_location (65, 5914, 992)
					d.setqf2("devilcatacomb_zone","done", 1)
					say_in_map(get_server_timer_arg(),"Azrail yenildi. Şeytan Katakombu'na nihayet[ENTER]huzur hakim. Şimdi yer yüzüne ışınlanacaksın. ")

					clear_server_timer ("devilcatacomb_0m_left_timer", get_server_timer_arg())
					server_timer("devilcatacomb_exit_timer", 60, get_server_timer_arg())
				else
					server_timer ("floor7_eli_check_timer_2", 5, get_server_timer_arg())
				end
			end
		end

		when floor7_eli_check_timer_2.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() == 0 then
					d.set_warp_location (65, 5914, 992)
					d.setqf2("devilcatacomb_zone","done", 1)
					say_in_map(get_server_timer_arg(),"Azrail yenildi. Şeytan Katakombu'na nihayet[ENTER]huzur hakim. Şimdi yer yüzüne ışınlanacaksın. ")
	
					clear_server_timer ("devilcatacomb_0m_left_timer", get_server_timer_arg())
					server_timer("devilcatacomb_exit_timer", 30, get_server_timer_arg())
				else
					server_timer ("floor7_eli_check_timer_1", 5, get_server_timer_arg())
				end
			end
		end

		when devilcatacomb_exit_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.setqf2("devilcatacomb_zone","level", 0)
				d.setqf2("devilcatacomb_zone","last_exit_time", get_global_time())
				clear_server_timer("devilcatacomb_5m_left_timer", get_server_timer_arg())
				clear_server_timer("devilcatacomb_1m_left_timer", get_server_timer_arg())
				clear_server_timer("devilcatacomb_0m_left_timer", get_server_timer_arg())
				clear_server_timer("floor6_eli_check_timer_1", get_server_timer_arg())
				clear_server_timer("floor6_eli_check_timer_2", get_server_timer_arg())
				clear_server_timer("floor7_eli_check_timer_1", get_server_timer_arg())
				clear_server_timer("floor7_eli_check_timer_2", get_server_timer_arg())
				clear_server_timer("devilcatacomb_floor7_say_timer", get_server_timer_arg())
				clear_server_timer("devilcatacomb_floor7_timer", 3, get_server_timer_arg())
				d.clear_regen()
				d.exit_all()
			end
		end
	end  --state
end   --quest

