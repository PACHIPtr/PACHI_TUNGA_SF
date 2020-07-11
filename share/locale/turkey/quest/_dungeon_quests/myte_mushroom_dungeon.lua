--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest mushroom_dungeon begin
	state start begin
		function setting()
		return {
				["base"]				=		{["x"] = 21000, ["y"]= 1000},
				-- STEP 1 clear floor from the all monsters, after jump all to froor 2 and move to STEP 2
				["floor_1"]				=		{["x"] = 269, ["y"] = 144},	-- . 269 144	10841
				["floor_2"]				=		{["x"] = 778, ["y"] = 151},	-- . 778 151	10842
				-- STEP 2 kill monsters and find 5 souls, after collect all 5 souls jump all to floor 3 and move to STEP 3
				["floor_3"]				=		{["x"] = 158, ["y"] = 811},	-- . 158 811	10843
				-- STEP 3 kill all metin stones and semi boss, after kill teleport to floor 4 and move to STEP 5
				["floor_4"]				=		{["x"] = 473, ["y"] = 581},	-- . 740 830	10844
				


				-- STEP 5 on the start of floor 4 respawn 6 metin stones on random position[1-2], after kill all move to step 6
				["floor_4_metin_1"]		=		{{527, 555},{695, 643}},
				["floor_4_metin_2"]		=		{{588, 614},{626, 591}},
				["floor_4_metin_3"]		=		{{618, 569},{574, 574}},	
				["floor_4_metin_4"]		=		{{654, 619},{522, 596}},
				["floor_4_metin_5"]		=		{{730, 600},{545, 624}},
				["floor_4_metin_6"]		=		{{781, 627},{644, 581}},
		
				 -- STEP 6 respawn only 4x boss after kill each one, after kill 4x move to step 7
				["semi_boss"]		=		{{326, 210},{217, 213},{832, 217},{725, 220},{158, 811},{261, 809},{761, 748},{762, 792},{719, 791},{718, 747}},

				-- STEP 7 after kill 4x semi boss respawn final boss
				["final_boss"]		=		{["x"] = 739, ["y"] = 831},
			}
		end

		when login begin
			if (pc.get_map_index() >= 2440000 and pc.get_map_index() < 2450000) then
				if pc.getqf("join_mashroom_dung") > 0 then
					pc.setqf("join_mashroom_dung", 0)
					d.zodiac_notice_clear()
					-- d.notice("Bir sonraki katta ulaşmak için. ")
					d.zodiac_notice("Bir sonraki kata ulaşmak için tüm canavarları yok et !")
					d.setqf2("mashroom_dung","last_exit_time", get_global_time())
					d.setf("level", 1)
					d.regen_file("data/dungeon/mushroom_dungeon_regens/regen_1.txt")
					server_timer('mushroom1', 5,d.get_map_index())
					server_timer ("floor1_eli_check_timer", 5,d.get_map_index())
				end
			end
			local idx = pc.get_map_index()
			if idx >= (244 * 10000) and idx < ((244 + 1) * 10000) then
				local level = d.getf("level")
				--d.set_warp_location (0, 0, 0)
			end
			if idx == 244 then
				pc.setf("mashroom_dung","last_exit_time", get_global_time())
				pc.warp(1114400,53400)
			end
		end

		when logout begin
			local idx = pc.get_map_index()
			if idx == 244 or idx >= (244 * 10000) and idx < ((244 + 1) * 10000) then
				--pc.setf("mashroom_dung","last_exit_time", get_global_time())
				------------ Dungeon Turn Back System ----------------------
				pc.setf("mushroom","dungeon_turn",1)
				------------------------------------------------------------
			end
		end
		when 20410.chat."Lanetli Orman (Bireysel)" begin
			addimage(35,35,'mantar.tga')
			if party.is_party() then
				say("")
				say("")
				say("")
				say("")
				say("")
				say_reward("Bu zindana grup halinde giremezsin")
				say_reward("lütfen önce bulundugun grupdan ayril !")
				return
			end
			if (pc.get_channel_id() == 2 or pc.get_channel_id() == 3 or pc.get_channel_id() == 4 or pc.get_channel_id() == 5 or pc.get_channel_id() == 6) then
				say("Lanetli Orman:")
				addimage(35,35,'mantar.tga')
				say("")
				say("")
				say("")
				say("")
				say("")
				say("Sadece 1. kanaldan giriş yapabilirsin.")
				return
			end
			-- sys_log(0,"mushroom_dungeon quest open")

			say(mob_name(20410))
			say("")
			say("")
			say("")
			say("")
			say("")
			say("Lanetli Orman karanlık güçlerin tapınağı")
			say("haline gelmiş büyülü bir mekan. Giriş yapabilmek")
			say("için 100. seviyenin üzerinde olman gerekiyor.")
			say("")
			say_item(item_name(31322),31322, "")
			wait()
			say(mob_name(20410))
			addimage(35,35,'mantar.tga')
			say("")
			say("")
			say("")
			say("")
			say("")
			say(string.format("%s:", item_name(31322)))
			say("Giriş yaptığında eşya yok olacak. ")
			say("")
			say_reward("Devam etmek istiyor musun?")
			say("")
			local s = select("Evet", "Hayır")
			-- ------------ Dungeon Turn Back System ----------------------
			-- pc.remove_turn_back_dungeon()
			-- ------------------------------------------------------------
			if s == 1 then
				if not pc.can_warp() then
					say(mob_name(20410))
					addimage(35,35,'mantar.tga')
					say("")
					say("")
					say("")
					say("")
					say("")
					say("")
					say("Henüz ışınlanamazsın.")
					say("")
					return
				end
				if get_time() < pc.getqf("player_dung_65") then
					if (game.get_event_flag("enable_time_lanet") == 1) then
						say("Beta Server; süreyi sıfırlamak mı istiyorsun?")
						say("Kalan süre: "..dungeon_get_duration(pc.getqf("player_dung_65") - get_global_time()).."")
						if select("Evet","Hayır") == 1 then
							pc.setqf("player_dung_65",0)
							return
						end
					end
					say(mob_name(20410))
					addimage(35,35,'mantar.tga')
					say("")
					say("")
					say("")
					say("")
					say("")
					say("Soguma süren henüz dolmadi !")
					say("Kalan süre: "..dungeon_get_duration(pc.getqf("player_dung_65") - get_global_time()).."")
					return
				end
				if pc.get_level() < 100 or pc.get_level() > 120 then
					say(mob_name(20410))
					addimage(35,35,'mantar.tga')
					say("")
					say("")
					say("")
					say("")
					say("")
					say("Giris yapmak için seviyen yetersiz.")
					say(string.format("Minimum Seviye: %d.",100))
					say(string.format("Maksimum Seviye: %d.",120))
					return
				end	
				
				if pc.count_item(31322) < 1 then
					say(mob_name(20410))
					addimage(35,35,'mantar.tga')
					say("")
					say("")
					say("")
					say("")
					say("")
					say("Envanterinde yeterli miktarda yok:")
					say_reward(string.format("%s.", item_name(31322)))
					say("")
					return
				end
				if game.get_event_flag("mashroom_dung_close") == 1 then
					say(mob_name(20410))
					addimage(35,35,'mantar.tga')
					say("")
					say("")
					say("")
					say("")
					say("")
					say("Lanetli Orman henüz aktif durumda değildir.")
					say("")
					return
				else
					pc.setqf("player_dung_65", get_time()+60*60*2)
					pc.remove_item(31322, 1)
					pc.setqf("join_mashroom_dung", 1)
					--d.join(244)
					d.new_jump(244, 21270 * 100, 1147 * 100)
				end
			end
		end -- when
		
		when 20410.chat."Süre sıfırla " with (game.get_event_flag("mashroom_dung_time") == 1) begin
			local select_timer = select("Sıfırla", "Sıfırlama")
			if select_timer == 1 then
				pc.setqf("player_dung_65", 0)
			end
		end

		when floor1_eli_check_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() <= 5 then
					d.zodiac_notice_clear()
					-- d.notice("Bütün canavarları yokettin!")
					d.zodiac_notice("10 saniye içerisinde bir sonraki kata gönderileceksin !")
					server_timer ("mushroom_dungeon_end_first", 10,d.get_map_index())
					clear_server_timer("floor1_eli_check_timer",d.get_map_index())
				else
					server_timer ("floor1_eli_check_timer_2", 10,d.get_map_index())
					clear_server_timer("floor1_eli_check_timer",d.get_map_index())
				end
			end
		end

		when floor1_eli_check_timer_2.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() <= 5 then
					-- d.notice("Bütün canavarları yokettin!")
					-- d.notice("10 Saniye içerisinde ışınlanacaksın!")
					d.zodiac_notice_clear()
					-- d.notice("Bütün canavarları yokettin!")
					d.zodiac_notice("10 saniye içerisinde bir sonraki kata gönderileceksin !")
					server_timer("mushroom_dungeon_end_first", 10,d.get_map_index())
					clear_server_timer("floor1_eli_check_timer_2",d.get_map_index())
				else
					server_timer ("floor1_eli_check_timer", 10,d.get_map_index())
					clear_server_timer("floor1_eli_check_timer_2",d.get_map_index())
				end
			end
		end
		
		when mushroom_dungeon_end_first.server_timer begin
			if d.select(get_server_timer_arg()) then
				local setting = mushroom_dungeon.setting()
				d.setf("level",2)
				d.jump_all (setting.floor_2.x + setting.base.x, setting.floor_2.y + setting.base.y )
				d.set_regen_file("data/dungeon/mushroom_dungeon_regens/regen_2.txt")
				d.zodiac_notice_clear()
				-- d.notice("Bütün canavarları yokettin!")
				d.zodiac_notice("Tüm canavarları yok ederek 5 savaşcının ruhunu serbest bırak !")
				cmdchat("MashRoomCoolTimeSetFloor 2")
				clear_server_timer("mushroom_dungeon_end_first",d.get_map_index())
			end	
		end

		-- END of LEVEL 1

		-- START of LEVEL 2
		function warrior_soul()
			local soul = d.getf("warrior_soul")
			local soul_names = {
				[1] = {"Bu toprakları koruyan bir savaşcının ruhunu keşfettiniz ! - Kalan(4)"},
				[2] = {"Bu toprakları koruyan bir savaşcının ruhunu keşfettiniz ! - Kalan(3)"},
				[3] = {"Bu toprakları koruyan bir savaşcının ruhunu keşfettiniz ! - Kalan(2)"},
				[4] = {"Bu toprakları koruyan bir savaşcının ruhunu keşfettiniz ! - Kalan(1)"},
				[5] = {"Bu toprakları koruyan bir savaşcının ruhunu keşfettiniz ! - Kalan(0)"},
			}
			d.zodiac_notice_clear()
				-- d.notice("Bütün canavarları yokettin!")
			-- d.zodiac_notice("Tüm canavarları yok ederek 5 savaşcının ruhunu serbest bırak !")
			d.zodiac_notice(string.format("%s.", soul_names[soul][1]))
		end

		when kill with pc.get_map_index() >= 2440000 and pc.get_map_index() < 2450000 and npc.get_race() == 4963 and d.getf("level") == 2 begin
			local KILL_COUNT_FOR_DROP_KEY = 30
			local n =d.getf("count_to_kill") + 1
			d.setf("count_to_kill", n)		
            if n == KILL_COUNT_FOR_DROP_KEY then
				local soul = d.getf("warrior_soul") + 1
				d.setf("warrior_soul", soul)
                mushroom_dungeon.warrior_soul()
                d.setf("count_to_kill", 0)
				if soul >= 5 then

					d.zodiac_notice_clear()
					d.zodiac_notice("10 saniye içerisinde bir sonraki kata gönderileceksin !")
					d.kill_all()
					d.clear_regen()
					d.setf("level", 3)
					timer("third", 5)
				end
            end
		end
		-- END of LEVEL 2

		when third.timer begin
			local setting = mushroom_dungeon.setting()
			d.jump_all (setting.floor_3.x + setting.base.x, setting.floor_3.y + setting.base.y )
			d.regen_file("data/dungeon/mushroom_dungeon_regens/regen_3.txt")
			d.zodiac_notice_clear()
			d.zodiac_notice("Bir sonraki aşamaya geçmek için büyülü mantar metinlerini yok et.")
			cmdchat("MashRoomCoolTimeSetFloor 3")
			d.spawn_mob(8073,186,832)
			d.spawn_mob(8073,178,823)
			d.spawn_mob(8073,178,811)
			d.spawn_mob(8073,178,798)
			d.spawn_mob(8073,186,789)
			d.spawn_mob(8073,231,788)
			d.spawn_mob(8073,239,797)
			d.spawn_mob(8073,238,811)
			d.spawn_mob(8073,239,823)
			d.spawn_mob(8073,230,833)
		end

		when kill with npc.get_race() == 8073 and pc.get_map_index() >= 2440000 and pc.get_map_index() < 2450000 and d.getf("kill_metins") < 10 and d.getf("level") == 3 begin
			d.setf("kill_metins", d.getf("kill_metins")+1)
			--d.notice("Mantar Metini Katledildi!")
			if d.getf("kill_metins") >= 10 then
				d.setf("level", 4)
				d.zodiac_notice_clear()
				d.zodiac_notice("Bir sonraki aşamaya geçmek için Büyülü Mantar Elçilerini yok et!.")
				timer("fourth", 5)
			end
		end

		when fourth.timer begin
			d.spawn_mob(4966,156,812)
			d.spawn_mob(4966,260,808)
			d.spawn_mob(4966,206,862)
			cmdchat("MashRoomCoolTimeSetFloor 4")
		end

		when kill with npc.get_race() == 4966 and pc.get_map_index() >= 2440000 and pc.get_map_index() < 2450000 and d.getf("kill_mini_bosses") < 3 and d.getf("level") == 4 begin
			d.setf("kill_mini_bosses", d.getf("kill_mini_bosses")+1)
			d.zodiac_notice_clear()
			d.zodiac_notice("Bir büyülü mantar elçisi yok edildi.")
			if d.getf("kill_mini_bosses") >= 3 then
				d.setf("level", 5)
				d.kill_all()
				d.clear_regen()
				d.zodiac_notice_clear()
				d.zodiac_notice("10 saniye içerisinde bir sonraki kata gönderileceksin !")
				timer("fifth", 5)
			end
		end

		when fifth.timer begin
			local setting = mushroom_dungeon.setting()
			-- d.notice("Bütün mantar metinlerini etkisiz hale getirin.")
			d.zodiac_notice_clear()
			d.zodiac_notice("Bütün mantar metinlerini etkisiz hale getirin !")
			d.jump_all (setting.floor_4.x + setting.base.x, setting.floor_4.y + setting.base.y )
			d.set_regen_file("data/dungeon/mushroom_dungeon_regens/regen_4.txt")

			local n1 = number (1,2)
			local n2 = number (1,2)
			local n3 = number (1,2)
			local n4 = number (1,2)
			local n5 = number (1,2)
			local n6 = number (1,2)
			d.spawn_mob (8073, setting.floor_4_metin_1[n1][1], setting.floor_4_metin_1[n1][2])
			d.spawn_mob (8073, setting.floor_4_metin_2[n2][1], setting.floor_4_metin_2[n2][2])
			d.spawn_mob (8073, setting.floor_4_metin_3[n3][1], setting.floor_4_metin_3[n3][2])
			d.spawn_mob (8073, setting.floor_4_metin_4[n4][1], setting.floor_4_metin_4[n4][2])
			d.spawn_mob (8073, setting.floor_4_metin_5[n5][1], setting.floor_4_metin_5[n5][2])
			d.spawn_mob (8073, setting.floor_4_metin_6[n6][1], setting.floor_4_metin_6[n6][2])
			cmdchat("MashRoomCoolTimeSetFloor 5")
		end

		when kill with npc.get_race() == 8073 and pc.get_map_index() >= 2440000 and pc.get_map_index() < 2450000 and d.getf("kill_metins_sd") < 6 and d.getf("level") == 5 begin
			d.setf("kill_metins_sd", d.getf("kill_metins_sd")+1)
			if d.getf("kill_metins_sd") > 1 then
				-- d.notice("Metin taşı katledildi!")
				d.zodiac_notice_clear()
				-- d.zodiac_notice("Bir metin taşı katledildi ! - Kalan metin taşı: %d")
				d.zodiac_notice(string.format("Bir metin taşı katledildi ! - Kalan metin taşı: %d", (6-d.getf("kill_metins_sd"))))
			-- else
				-- d.notice("Son metin taşı katledildi!")
			end
			if d.getf("kill_metins_sd") >= 6 then
				d.setf("level", 6)
				-- d.notice("Tüm metin taşlarını etkisiz hale getirdin.")
				-- d.notice("Bir sonraki dalgayı tamamlamak için.")
				-- d.notice("Büyülü mantar kahramanlarını öldür.")
				d.zodiac_notice_clear()
				d.zodiac_notice("Büyülü mantar kahramanlarını etkisiz hale getirin !")
				timer("sixth", 5)
			end
		end

		when sixth.timer begin
			local setting = mushroom_dungeon.setting()
			d.spawn_mob(4965,718,792)
			d.spawn_mob(4965,718,747)
			d.spawn_mob(4965,710,757)
			d.spawn_mob(4965,709,769)

			d.spawn_mob(4965,709,782)
			d.spawn_mob(4965,770,757)
			d.spawn_mob(4965,770,769)
			d.spawn_mob(4965,771,782)

			d.spawn_mob(4965,802,240)
			d.spawn_mob(4965,757,241)
			d.spawn_mob(4965,757,196)

			d.spawn_mob(4965,186,833)
			d.spawn_mob(4965,186,789)
			d.spawn_mob(4965,230,789)
			d.spawn_mob(4965,230,833)

			d.spawn_mob(4965,setting.semi_boss[1][1],setting.semi_boss[1][2])
			d.spawn_mob(4965,setting.semi_boss[2][1],setting.semi_boss[2][2])
			d.spawn_mob(4965,setting.semi_boss[3][1],setting.semi_boss[3][2])
			d.spawn_mob(4965,setting.semi_boss[4][1],setting.semi_boss[4][2])
			d.spawn_mob(4965,setting.semi_boss[5][1],setting.semi_boss[5][2])
			d.spawn_mob(4965,setting.semi_boss[6][1],setting.semi_boss[6][2])
			d.spawn_mob(4965,setting.semi_boss[7][1],setting.semi_boss[7][2])
			d.spawn_mob(4965,setting.semi_boss[8][1],setting.semi_boss[8][2])
			cmdchat("MashRoomCoolTimeSetFloor 6")
		end

		when kill with npc.get_race() == 4965 and pc.get_map_index() >= 2440000 and pc.get_map_index() < 2450000 and d.getf("kill_semi_final") < 8 and d.getf("level") == 6 begin
			d.setf("kill_semi_final", d.getf("kill_semi_final")+1)
			if d.getf("kill_semi_final") > 1 then
				-- d.notice("Büyülü Mantar Kahramanı Yenildi!")
				d.zodiac_notice_clear()
				d.zodiac_notice(string.format("Büyülü mantar kahramanlarını etkisiz hale getirin ! - Kalan : %d", (8-d.getf("kill_semi_final"))))
			-- else
				-- d.notice("Son Büyülü Mantar Kahramanı Yenildi..")
			end
			if d.getf("kill_semi_final") >= 8 then
				d.setf("level", 7)
				-- d.notice("Bütün mantar kahramanlarını öldürdün!")
				-- d.notice("Lanetli Ormannın tamamını fethetmek için.")
				-- d.notice("Büyülü Mantar Efsanesini yok et.")
				d.zodiac_notice_clear()
				d.zodiac_notice("Lanetli Ormannın fethetmek için Büyülü Mantar Efsanesini yok et.")
				timer("seventh", 5)
			end
		end

		when seventh.timer begin
			local setting = mushroom_dungeon.setting()
			d.spawn_mob(4969,setting.final_boss.x,setting.final_boss.y)
			cmdchat("MashRoomCoolTimeSetFloor 7")
		end

		when kill with npc.get_race() == 4969 and pc.get_map_index() >= 2440000 and pc.get_map_index() < 2450000 and d.getf("level") == 7 begin
			timer("exitmsh", 3*60)
			d.zodiac_notice_clear()
			d.zodiac_notice("Tebrikler zindan tamamlandı 3 dakika içerisinde dışarı gönderileceksin.")
		end

		when mushroom1.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindanı tamamlamak için 45 dakika vaktin kaldı.")
				server_timer('mushroom2', 10*60,d.get_map_index())
				clear_server_timer("mushroom1",d.get_map_index())
			end
		end
		when mushroom2.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindanı tamamlamak için 35 dakika vaktin kaldı.")
				server_timer('mushroom3', 10*60,d.get_map_index())
				clear_server_timer("mushroom2",d.get_map_index())
			end
		end
		when mushroom3.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindanı tamamlamak için 25 dakika vaktin kaldı.")
				server_timer('mushroom4', 10*60,d.get_map_index())
				clear_server_timer("mushroom3",d.get_map_index())
			end
		end
		when mushroom4.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindanı tamamlamak için 15 dakika vaktin kaldı.")
				server_timer('mushroom5', 10*60,d.get_map_index())
				clear_server_timer("mushroom4",d.get_map_index())
			end
		end
		when mushroom5.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindanı tamamlamak için 5 dakika vaktin kaldı.")
				server_timer('mushroom6', 4*60,d.get_map_index())
				clear_server_timer("mushroom5",d.get_map_index())
			end
		end

		when mushroom6.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindanı tamamlamak için 1 dakika vaktin kaldı.")
				server_timer('mushroomend', 60,d.get_map_index())
				clear_server_timer("mushroom6",d.get_map_index())
			end
		end
		when mushroomend.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindanı tamamlamakta başarısız oldun. 10 saniye içerisinde dışarı ışınlanıyorsun.")
				server_timer('mushroomfail', 10,d.get_map_index())
				clear_server_timer("mushroomend",d.get_map_index())
			end
		end
		when mushroomfail.server_timer begin
			if d.select(get_server_timer_arg()) then
				clear_server_timer("mushroomfail",d.get_map_index())
				d.exit_all_to_start_position()
			end
		end

		when exitmsh.timer begin
			d.setqf2("mashroom_dung","last_exit_time", get_global_time())
			d.exit_all_to_start_position()
		end
	end
end