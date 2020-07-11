--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest crystal_dungeon1 begin
	state start begin
		--Functions
		function settings()
			return
			{
				["map_index"] = 9,
				["spawm_pos"] = {
					[1] = {21272, 29576},
					[2] = {663+20992, 248+29440},
					[3] = {241+20992, 639+29440},
					[4] = {406+20992, 648+29440},
					[5] = {776+20992, 740+29440},
				},
				["level_check"] = {
					["minimum"] = 85,
					["maximum"] = 120
				},
				["keys"] = {19985, 19986, 19987},
				["metin_stones"] = {8411, 8412, 8413, 8414},
			};
		end
		
		function spawn_boss(level)
			local boss_settings = {
				[3] = {948, 741, 191},
				[5] = {949, 431, 544},
				[7] = {950, 773, 681}
				};
			
			return d.spawn_mob(boss_settings[level][1], boss_settings[level][2], boss_settings[level][3])
		end
		
		function spawn_metins_4thfloor()
			local setting = crystal_dungeon1.settings()
			local r = number (1,8)
			if r == 1 then
				d.spawn_mob(setting["metin_stones"][3], 250, 659)
				d.spawn_mob(setting["metin_stones"][2], 239, 714)
				d.spawn_mob(setting["metin_stones"][2], 257, 745)
				d.spawn_mob(setting["metin_stones"][2], 282, 777)
				d.spawn_mob(setting["metin_stones"][2], 321, 767)
				d.spawn_mob(setting["metin_stones"][2], 371, 770)
				d.spawn_mob(setting["metin_stones"][2], 397, 737)
				d.spawn_mob(setting["metin_stones"][2], 410, 714)
			elseif r == 2 then
				d.spawn_mob(setting["metin_stones"][2], 250, 659)
				d.spawn_mob(setting["metin_stones"][3], 239, 714)
				d.spawn_mob(setting["metin_stones"][2], 257, 745)
				d.spawn_mob(setting["metin_stones"][2], 282, 777)
				d.spawn_mob(setting["metin_stones"][2], 321, 767)
				d.spawn_mob(setting["metin_stones"][2], 371, 770)
				d.spawn_mob(setting["metin_stones"][2], 397, 737)
				d.spawn_mob(setting["metin_stones"][2], 410, 714)
			elseif r == 3 then
				d.spawn_mob(setting["metin_stones"][2], 250, 659)
				d.spawn_mob(setting["metin_stones"][2], 239, 714)
				d.spawn_mob(setting["metin_stones"][3], 257, 745)
				d.spawn_mob(setting["metin_stones"][2], 282, 777)
				d.spawn_mob(setting["metin_stones"][2], 321, 767)
				d.spawn_mob(setting["metin_stones"][2], 371, 770)
				d.spawn_mob(setting["metin_stones"][2], 397, 737)
				d.spawn_mob(setting["metin_stones"][2], 410, 714)
			elseif r == 4 then
				d.spawn_mob(setting["metin_stones"][2], 250, 659)
				d.spawn_mob(setting["metin_stones"][2], 239, 714)
				d.spawn_mob(setting["metin_stones"][2], 257, 745)
				d.spawn_mob(setting["metin_stones"][3], 282, 777)
				d.spawn_mob(setting["metin_stones"][2], 321, 767)
				d.spawn_mob(setting["metin_stones"][2], 371, 770)
				d.spawn_mob(setting["metin_stones"][2], 397, 737)
				d.spawn_mob(setting["metin_stones"][2], 410, 714)
			elseif r == 5 then
				d.spawn_mob(setting["metin_stones"][2], 250, 659)
				d.spawn_mob(setting["metin_stones"][2], 239, 714)
				d.spawn_mob(setting["metin_stones"][2], 257, 745)
				d.spawn_mob(setting["metin_stones"][2], 282, 777)
				d.spawn_mob(setting["metin_stones"][3], 321, 767)
				d.spawn_mob(setting["metin_stones"][2], 371, 770)
				d.spawn_mob(setting["metin_stones"][2], 397, 737)
				d.spawn_mob(setting["metin_stones"][2], 410, 714)
			elseif r == 6 then
				d.spawn_mob(setting["metin_stones"][2], 250, 659)
				d.spawn_mob(setting["metin_stones"][2], 239, 714)
				d.spawn_mob(setting["metin_stones"][2], 257, 745)
				d.spawn_mob(setting["metin_stones"][2], 282, 777)
				d.spawn_mob(setting["metin_stones"][2], 321, 767)
				d.spawn_mob(setting["metin_stones"][3], 371, 770)
				d.spawn_mob(setting["metin_stones"][2], 397, 737)
				d.spawn_mob(setting["metin_stones"][2], 410, 714)
			elseif r == 7 then
				d.spawn_mob(setting["metin_stones"][2], 250, 659)
				d.spawn_mob(setting["metin_stones"][2], 239, 714)
				d.spawn_mob(setting["metin_stones"][2], 257, 745)
				d.spawn_mob(setting["metin_stones"][2], 282, 777)
				d.spawn_mob(setting["metin_stones"][2], 321, 767)
				d.spawn_mob(setting["metin_stones"][2], 371, 770)
				d.spawn_mob(setting["metin_stones"][3], 397, 737)
				d.spawn_mob(setting["metin_stones"][2], 410, 714)
			elseif r == 8 then
				d.spawn_mob(setting["metin_stones"][2], 250, 659)
				d.spawn_mob(setting["metin_stones"][2], 239, 714)
				d.spawn_mob(setting["metin_stones"][2], 257, 745)
				d.spawn_mob(setting["metin_stones"][2], 282, 777)
				d.spawn_mob(setting["metin_stones"][2], 321, 767)
				d.spawn_mob(setting["metin_stones"][2], 371, 770)
				d.spawn_mob(setting["metin_stones"][2], 397, 737)
				d.spawn_mob(setting["metin_stones"][3], 410, 714)
			end
		end
		
		function make_dungeon()
			local setting = crystal_dungeon1.settings()
			
			d.new_jump_party(setting["map_index"], setting["spawm_pos"][1][1], setting["spawm_pos"][1][2])
			d.setf("lair_check", 1)
			d.setf("level_four_login", 0)
			crystal_dungeon1.get_regens(1)
		end

		function get_regens(level)
			local regens = {
				[1] = "data/dungeon/crystal_dungeon/regen_1.txt",
				[2] = "data/dungeon/crystal_dungeon/regen_2.txt",
				[3] = "data/dungeon/crystal_dungeon/regen_3.txt",
				[4] = "data/dungeon/crystal_dungeon/regen_4a.txt",
				[5] = "data/dungeon/crystal_dungeon/regen_4b.txt",
				[6] = "data/dungeon/crystal_dungeon/regen_5.txt",
				[7] = "data/dungeon/crystal_dungeon/regen_5_metins.txt"};
			
			return d.set_regen_file(regens[level])
		end
		
		function party_get_member_pids()
			local pids = {party.get_member_pids()}
			
			return pids
		end
		
		------
		------[Dungeon enter]
		------
		when 9215.chat."Kristal Zindan" begin
			addimage(35,35,'kristal.tga')
			local settings = crystal_dungeon1.settings()
			
			say(mob_name(9215))
			say("")
			
			if not party.is_party() then
				addimage(35,35,'kristal.tga')say("")say("")say("")say("")

				say("Bu zindana giriþ yapabilmen için,")
				say("bir grupta bulunman gerekiyor .")
				return
			end
			
			if not party.is_leader() then
				addimage(35,35,'kristal.tga')say("")say("")say("")say("")

				say("Bu zindana giriþ yapabilmen için,")
				say("bir grubun lideri olmalýsýn .")
				return
			end
			
			if party.get_near_count() < 2 then
				addimage(35,35,'kristal.tga')say("")say("")say("")say("")

				say("Bu zindana giriþ yapabilmen için,")
				say("bir grupta bulunmalýsýnýz.")
				return
			end
			
			local levelCheck, maximlevelCheck, itemCheck, durationCheck = true, true, true, true
			local noDurationMembers, noLevelMembers, noitemCheckMembers = {},{},{}

			for i, pid in next, crystal_dungeon1.party_get_member_pids(), nil do
				q.begin_other_pc_block(pid)
				
				if pc.get_level() < settings["level_check"]["minimum"] then
					table.insert(noLevelMembers, pc.get_name())
					levelCheck = false
				end
				if pc.get_level() > settings["level_check"]["maximum"] then
					table.insert(noLevelMembers, pc.get_name())
					maximlevelCheck = false
				end
				if pc.count_item(31335) <= 0 then
					table.insert(noitemCheckMembers, pc.get_name())
					itemCheck = false
				end
				
				if get_time() < pc.getqf("cy_player_time_duration") then
					table.insert(noDurationMembers, pc.get_name())
					durationCheck = false
				end
				
				q.end_other_pc_block()
			end
			
			if not levelCheck then
				addimage(35,35,'kristal.tga')say("")say("")say("")say("")

				say("Bu zindana giriþ yapabilmen için,")
				say("grubun her üyesi")
				say(string.format("Minimum seviye: %s.", settings["level_check1"]["minimum"]))
				say("")
				say("Grubunuzdaki bazý üyeler bu þartý yerine getirmiyor !")
				for i, name in next, noLevelMembers, nil do
					say(color(1,1,0), "    "..name)
				end
				return
			end
			
			if not maximlevelCheck then
				addimage(35,35,'kristal.tga')say("")say("")say("")say("")
	
				say("Bu zindana giriþ yapabilmen için,")
				say("grubun her üyesi ")
				say(string.format("Minimum seviye:   %s.", settings["level_check1"]["maximum"]))
				say("")
				say("Grubunuzdaki bazý üyeler bu þartý yerine getirmiyor !")
				for i, name in next, noLevelMembers, nil do
					say(color(1,1,0), "    "..name)
				end
				return
			end
			
			if not itemCheck then
				-- Quest window dungeon image
				addimage(35,35,'kristal.tga')say("")say("")say("")say("")

				say("Bu zindana giriþ yapabilmen için,")
				say("grubun her üyesinin zindan biletine")
				say("sahip olmasý gerekiyor bir üyenin")
				say("Envanterinde yeterli miktarda yok:")
				say_item(item_name(31335),31335, "")
				for i, name in next, noitemCheckMembers, nil do
					say(color(1,1,0), "    "..name)
				end
				return
			end
			
			if not durationCheck then
				-- Quest window dungeon image
				addimage(35,35,'kristal.tga')say("")say("")say("")say("")

				say("Bu zindana giriþ yapabilmen için,")
				say("grubun her üyesinin soguma süresinin")
				say("sona ermiþ olmasý gerekiyor")
				say("soguma süresini tamamlamamýþ olanlar var:")

				for i, name in next, noDurationMembers, nil do
					say(color(1,1,0), "    "..name)
				end
				return
			end
			if (pc.get_channel_id() == 2 or pc.get_channel_id() == 3 or pc.get_channel_id() == 4 or pc.get_channel_id() == 5 or pc.get_channel_id() == 6) then
				addimage(35,35,'kristal.tga')say("")say("")say("")say("")
				say("Sadece 1. kanaldan giriþ yapabilirsin.")
				return
			end

			
			for i, pid in next, crystal_dungeon1.party_get_member_pids(), nil do
				q.begin_other_pc_block(pid)
				pc.remove_item(31335, 1)
				pc.setqf("cy_player_time_duration", get_time()+60*60*3)
				q.end_other_pc_block()
			end

			addimage(35,35,'kristal.tga')say("")say("")say("")say("")

			say("Kristal zindana ýþýnlanýyorsun,")
			say("'Devam' dedikten sonra bütün üyeler ýþýnlanacaklar .")
			wait()
			crystal_dungeon1.make_dungeon()
		end
		when 9215.chat."Maðara Süre sýfýrla " with (game.get_event_flag("cy_dung_time") == 1) begin
			local select_timer = select("Sýfýrla", "Sýfýrlama")
			if select_timer == 1 then
				pc.setqf("cy_player_time_duration", 0)
			end
		end
		----
		----Dungeon start
		----
		when login begin
			local mid = pc.get_map_index()
			local setting = crystal_dungeon1.settings()
			if mid == setting["map_index"] then
				if not pc.in_dungeon() then
					warp_to_village()
				end
			end
		end
		when logout with pc.get_map_index() >= 90000 and pc.get_map_index() < 99999 begin
			------------ Dungeon Turn Back System ----------------------
			pc.setf("Crystal","dungeon_turn",1)
			------------------------------------------------------------
		end
		
		
		when kill with npc.get_race() == 940 or npc.get_race() == 941 or npc.get_race() == 942 begin
			local count, check = d.getf("mobs") + 1, false;
			local setting = crystal_dungeon1.settings()
			
			if d.getf("lair_check") == 1 then
				d.zodiac_notice_clear()
				d.zodiac_notice("Haritadaki bütün canavarlarý temizle !")
						--83
				if count < 82 and check == false then
					d.setf("mobs", count)
					local c = 82-d.getf("mobs")
					d.zodiac_notice_clear()
					d.zodiac_notice("Haritadaki bütün canavarlarý temizle ! Kalan yaratýk: "..c.."")
				else
					if d.getf("metin_stone") == 0 then
						d.clear_regen()
						d.kill_all()
						d.spawn_mob(setting["metin_stones"][1], 261, 276)
						d.spawn_mob(setting["metin_stones"][1], 242, 303)
						d.spawn_mob(setting["metin_stones"][1], 293, 305)
						d.zodiac_notice_clear()
						d.zodiac_notice("(1.2 Görev) Metinleri yoket !")
						d.notice(string.format(" %s Belirdi onlarý yoket ! ", mob_name(8411)))
						d.setf("metin_stone", 1)
					end
					check = true
				end
			end
		end
		
		when kill with npc.get_race() == 8411 and pc.in_dungeon() begin
			local count, check_metin1 = d.getf("metins") + 1, false;
			local setting = crystal_dungeon1.settings()
			
			if d.getf("lair_check") == 1 then
				if count < 3 and check_metin1 == false then
					d.setf("metins", count)
				else
					if d.getf("metin_stone1") == 0 then
						d.notice("Harika! Ýlk görevi baþarý ile tamamladýn, sonraki kata gönderiliyorsun !")
						d.kill_all()
						d.setf("lair_check", 2)
						d.setf("metin_stone1", 1)
						cleartimer("level_timer")
						timer("level_timer", 7)
					end
					check_metin1 = true
				end
			end
		end
		
		
		when 942.kill or 943.kill or 944.kill begin
			local count, check_2 = d.getf("mobs_2") + 1, false
			local setting = crystal_dungeon1.settings()
			
			if count < 72 and check_2 == false then
				d.setf("mobs_2", count)
				local w = 72-d.getf("mobs_2")
				d.zodiac_notice_clear()
				d.zodiac_notice("Haritadaki bütün canavarlarý temizle ! Kalan yaratýk: "..w.."")
			else
				if d.getf("crystal_seal") == 0 then
					game.drop_item(setting["keys"][1], 1)
					d.setf("crystal_seal", 1)
				end
				check_2 = true
			end
		end
		
		
		when 9210.take with item.vnum == 19985 and pc.in_dungeon() begin
			local setting = crystal_dungeon1.settings()
			local n = number (1,2)
			
			if n == 1 then
				d.clear_regen()
				pc.remove_item(setting["keys"][1], 1)
				d.zodiac_notice_clear()
				d.zodiac_notice("Mühür anýtý açmadý, Þansýný denemeye devam et!")	
				d.notice("Bu iyi bir mühür deðil .")
				d.notice("Git ve iyi bir tane ara !")
				d.spawn_mob(9211, 753, 172)
				timer("effect_purge", 10)
				d.setf("mobs_2", 1)
				crystal_dungeon1.get_regens(2)
				d.setf("crystal_seal", 0)
			else
				d.notice("Güzel bir mühür !")
				d.kill_unique(9210)
				d.clear_regen()
				d.kill_all()
				d.setf("lair_check", 3)
				pc.remove_item(setting["keys"][1], 1)
				cleartimer("level_timer")
				timer("level_timer", 12)
			end
		end
		
		when 940.kill or 943.kill or 944.kill or 945.kill begin
			local count, check3 = d.getf("mobs_3") + 1, false;
			
			if d.getf("lair_check") == 3 then
						--105
				if count < 104 and check3 == false then
					d.setf("mobs_3", count)
					local q = 105-d.getf("mobs_3")
					d.zodiac_notice_clear()
					d.zodiac_notice("Haritadaki bütün canavarlarý temizle ! Kalan yaratýk: "..q.."")
				else
					if d.getf("crystal_boss1") == 0 then
						d.kill_all()
						d.clear_regen()
						d.zodiac_notice_clear()
						d.zodiac_notice("Ejderhanýn yavrusunu yoket ! ")
						timer("third_boss", 10)
						d.setf("third_boss", 1)
						d.setf("crystal_boss1", 1)
					end
					check3 = true
				end
			end
		end
		
		when 948.kill begin
			local setting = crystal_dungeon1.settings()
			
			if d.getf("lair_check") == 3 then
				d.clear_regen()
				d.kill_all()
				d.setf("lair_check", 4)				
				d.notice("Kristal Canavar öldürüldü !")
				d.notice("30 saniye içinde sizi bir sonraki seviyeye ýþýnlayacaðýz .")
				cleartimer("level_four_timer")
				cleartimer("level_timer")
				timer("level_timer", 10)
			else
				return
			end
		end
		
		when 8412.kill begin
			local setting = crystal_dungeon1.settings()
			
			if d.getf("lair_check") == 4 then
				game.drop_item(setting["keys"][2],1)
			end				
		end
		
		when 8413.kill begin
			local setting = crystal_dungeon1.settings()
			
			if d.getf("lair_check") == 4 then
				game.drop_item(setting["keys"][3],1)
			end
		end
		
		when 9212.take with item.vnum == 19986 and pc.in_dungeon() begin
			local setting = crystal_dungeon1.settings()
			pc.remove_item(setting["keys"][2], 1)
			d.spawn_mob(9213, 405, 687)
			timer("wait_eff1", 2)
		end
		
		when 9212.take with item.vnum == 19987 and pc.in_dungeon() begin
			local setting = crystal_dungeon1.settings()
			pc.remove_item(setting["keys"][3], 1)
			d.spawn_mob(9213, 405, 687)
			timer("wait_eff2", 2)
		end
		
		when 944.kill or 945.kill or 946.kill begin
			local count, check_4 = d.getf("mobs_5") + 1, false;
			local setting = crystal_dungeon1.settings()
			
			if d.getf("lair_check") == 5 then
						--75
				if count < 74 and check_4 == false then
					d.setf("mobs_5", count)
					local q = 74-d.getf("mobs_5")
					d.zodiac_notice_clear()
					d.zodiac_notice("Haritadaki bütün canavarlarý temizle ! Kalan yaratýk: "..q.."")
				else
					if d.getf("crystal_dino") == 0 then
						d.clear_regen()
						d.kill_all()
						timer("crystal_dino", 10)
						d.setf("crystal_dino", 1)
					end
					check_4 = true
				end
			end
		end
		
		when 949.kill with pc.in_dungeon() begin
			local setting = crystal_dungeon1.settings()
			
			if d.getf("lair_check") == 5 then
				d.clear_regen()
				d.kill_all()
				d.setf("lair_check", 6)				
				d.notice("Kristal Dino öldürüldü !")
				d.notice("10 saniye içinde sizi bir sonraki seviyeye ýþýnlayacaðýz .")
				cleartimer("level_timer")
				timer("level_timer", 10)
			else
				return
			end
		end

		when 8414.kill with pc.in_dungeon() begin
			local count, check_5 = d.getf("metins_6") + 1, false;
			local setting = crystal_dungeon1.settings()
			
			if d.getf("lair_check") == 6 then
				if count < 7 and check_5 == false then
					d.setf("metins_6", count)
					local t = 7-d.getf("metins_6")
					d.zodiac_notice_clear()
					d.zodiac_notice("Metinleri yok et ve Kristal Ejderha ile yüzleþ! Kalan Metin: "..t.."")
				else
					if d.getf("crystal_dragon") == 0 then
						d.zodiac_notice_clear()
						d.zodiac_notice(string.format("%s göründü ! onu maðlub et !", mob_name(950)))
						d.notice("Kýsa sürede tüm metin taþlarý kýrýldý !")
						d.notice("Kristal Ejderha yakýnda görünecek !")
						cleartimer("level_six_timer")
						cleartimer("level_timer")
						d.kill_all()
						d.setf("lair_check", 7)
						d.setf("crystal_dragon", 1)
						cleartimer("level_timer")
						timer("level_timer", 7)
					end
					check_5 = true
				end
			end
		end

		when 950.kill with pc.in_dungeon() begin
			local setting = crystal_dungeon1.settings()
			
			if d.getf("lair_check") == 7 then
				d.clear_regen()
				d.kill_all()
				d.notice("Kristal Ejderha öldürüldü !")
				d.notice("Zindaný baþarýyla tamamladýn !")
				d.notice("3 dakika içinde ýþýnlanacaksýn .")
				cleartimer("level_timer")
				timer("final", 3 * 60)
			else
				return
			end
		end

		---
		--- Dungeon end
		---
		
		---
		--- TIMERS
		---		
		when effect_purge.timer begin
			d.kill_unique(9211)
		end
		
		when third_boss.timer begin
			local setting = crystal_dungeon1.settings()
			
			if d.getf("third_boss") == 1 then
				crystal_dungeon1.spawn_boss(3)
				d.setf("third_boss", 0)
			else
				return
			end
		end
		
		when crystal_dino.timer begin
			local setting = crystal_dungeon1.settings()
			
			if d.getf("crystal_dino") == 1 then
				crystal_dungeon1.spawn_boss(5)
				d.zodiac_notice_clear()
				d.zodiac_notice(string.format("%s belirdi ! Onunla mücadele et", mob_name(949)))
				d.setf("crystal_dino", 0)
			else
				return
			end
		end
		
		when wait_eff1.timer begin
			d.notice("Bu iyi bir taþ deðil !")
			d.notice("Þansýný tekrar dene .")			
		end
		
		when wait_eff2.timer begin
			d.notice("Bu taþ çok deðerli !")
			d.notice("Bir sonraki göreve gönderiliyorsun .")
			d.kill_unique(9212)
			d.clear_regen()
			d.kill_all()
			d.setf("lair_check", 5)
			cleartimer("level_timer")
			timer("level_timer", 12)
		end
		
		when level_four_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.set_warp_location(41, 9694, 2786)
				d.notice("Zindan sonuna kadar gidilmedi !")
				d.notice("Zaman doldu !")
				
				server_timer("exit_before", 10,d.get_map_index())
			end
		end
		
		when level_six_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.set_warp_location(41, 9694, 2786)
				d.notice("Zindan sonuna kadar gidilmedi !")
				d.notice("Zaman doldu  !")
				
				server_timer("exit_before2", 10,d.get_map_index())
			end
		end

		when final.timer begin
			clear_server_timer("level_six_timer",d.get_map_index())
			clear_server_timer("final_warp",d.get_map_index())
			clear_server_timer("level_four_timer",d.get_map_index())
			d.exit_all_to_start_position()
		end

		when level_timer.timer begin
			local get_flag = d.getf("lair_check")
			local setting = crystal_dungeon1.settings()
			
			d.clear_regen()
			d.kill_all()
			
			if get_flag == 2 then
				d.jump_all (setting["spawm_pos"][2][1], setting["spawm_pos"][2][2])
				crystal_dungeon1.get_regens(2)
				d.spawn_mob(9210, 753, 172)
				d.setf("lair_check", 2)
				d.zodiac_notice_clear()
				d.zodiac_notice("Dikkat! Canavarlarý Temizle anahtarý bul ve anýtý yoket !")
			end
			if get_flag == 3 then
				d.jump_all (setting["spawm_pos"][2][1], setting["spawm_pos"][2][2])
				crystal_dungeon1.get_regens(3)
				d.setf("lair_check", 3)
				d.zodiac_notice_clear()
				d.zodiac_notice("Dikkat! Canavarlarý Temizle !")
			end
			if get_flag == 4 then
				d.jump_all (setting["spawm_pos"][3][1], setting["spawm_pos"][3][2])
				crystal_dungeon1.get_regens(4)
				d.setf("lair_check", 4)
				d.zodiac_notice_clear()
				d.zodiac_notice("Dikkat! Metinlerden kristal magma elde et ve Kristal Kuleye ver !")
				crystal_dungeon1.spawn_metins_4thfloor()
			end
			if get_flag == 5 then
				d.jump_all (setting["spawm_pos"][4][1], setting["spawm_pos"][4][2])
				crystal_dungeon1.get_regens(5)
				d.zodiac_notice_clear()
				d.zodiac_notice("Dikkat! Canavarlarý Temizle !")
				d.setf("lair_check", 5)
			end
			if get_flag == 6 then
				d.jump_all (setting["spawm_pos"][5][1], setting["spawm_pos"][5][2])
				crystal_dungeon1.get_regens(6)
				crystal_dungeon1.get_regens(7)
				d.zodiac_notice_clear()
				d.zodiac_notice("Dikkat! Metinleri temizle ve Kristal Ejderhayý çaðýr !")
				d.setf("lair_check", 6)
			end
			if get_flag == 7 then
				crystal_dungeon1.spawn_boss(7)
				d.zodiac_notice_clear()
				d.zodiac_notice("Patronu katlettin! Görev tamamlandý !")
				d.setf("lair_check", 7)
			end
		end
	end
end
