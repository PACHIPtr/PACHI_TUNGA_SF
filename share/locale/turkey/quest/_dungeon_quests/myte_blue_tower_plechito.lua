--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest devil_tower2_plechito begin
	state start begin
		--[[ FUNCTION STATEMENT BEGIN ]]
		function get_regens(level)
			local regens = {
				[1] = "data/dungeon/devil_tower2_regens/floor_1.txt",
				[2] = "data/dungeon/devil_tower2_regens/floor_2.txt",
				[3] = "data/dungeon/devil_tower2_regens/floor_3.txt",
				[4] = "data/dungeon/devil_tower2_regens/floor_4.txt",
				[5] = "data/dungeon/devil_tower2_regens/floor_5.txt",
				[6] = "data/dungeon/devil_tower2_regens/floor_6.txt",
				[7] = "data/dungeon/devil_tower2_regens/floor_7.txt",
				[8] = "data/dungeon/devil_tower2_regens/floor_8.txt"};
			
			return d.set_regen_file(regens[level])
		end
		
		function spawn_boss(level)
			local boss_settings = {
				[2] = {806, 382, 191},
				[4] = {817, 394, 394},
				[7] = {1095, 209, 609},
				[8] = {816, 564, 613}};
			
			return d.spawn_mob(boss_settings[level][1], boss_settings[level][2], boss_settings[level][3])
		end
		
		function settings()
			return
			{
				["map_index"] = 18,
				["spawm_pos"] = {
					[1] = {21086, 22099},
					[2] = {332+21000, 109+22000},
					[3] = {598+21000, 96+22000},
					[4] = {330+21000, 360+22000},
					[5] = {166+21000, 333+22000},
					[6] = {166+21000, 333+22000},
					[7] = {212+21000, 540+22000},
					[8] = {560+21000, 546+22000}
				},
				["level_check1"] = {
					["minimum"] = 40,
					["maximum"] = 120
				},
				["keys"] = {30330, 30330}
			}
		end
		
		function party_get_member_pids()
			local pids = {party.get_member_pids()}
			
			return pids
		end
		
		function make_dungeon()
			local setting = devil_tower2_plechito.settings()
			
			d.new_jump_party(setting["map_index"], setting["spawm_pos"][1][1], setting["spawm_pos"][1][2])
			d.setf("lair_check1", 1)
			devil_tower2_plechito.get_regens(1)
		end
		--[[ FUNCTION STATEMENT END ]]
		
		--[[ TOWER STATEMENT BEGIN ]]
		
		when login begin
			local mid = pc.get_map_index()
			local setting = devil_tower2_plechito.settings()
			local get_flag = d.getf("lair_check1")
			
			if get_flag == 2 then
				if d.getf("level_two_login") == 0 then
					d.setf("level_two_login", 1)
					clear_server_timer("level_two_timer",d.get_map_index())
					server_timer("level_two_timer", 10 * 60, d.get_map_index())
				end
			end
			
			if mid == setting["map_index"] then
				if not pc.in_dungeon() then
					warp_to_village()
				end
			end
		end
		
		when logout with pc.get_map_index() >= 180000 and pc.get_map_index() < 189999 begin
			------------ Dungeon Turn Back System ----------------------
			pc.setf("devil_tower2","dungeon_turn",1)
			------------------------------------------------------------
		end
		
		when kill with npc.get_race() == 801 or npc.get_race() == 802 or npc.get_race() == 803 or npc.get_race() == 804 or npc.get_race() == 805 begin
			local count, check = d.getf("mobs") + 1, false;
			
			if d.getf("lair_check1") == 1 then
				if count < 650 and check == false then
					d.setf("mobs", count)
					local c = 650-d.getf("mobs")
					d.zodiac_notice_clear()
					d.zodiac_notice("(1.Görev)Dikkat! Dalga dalga gelecek olan iblisleri temizle! Kalan yaratýk: "..c.."")
				else
					if d.getf("metin_stone") == 0 then
						d.clear_regen()
						d.kill_all()
						d.spawn_mob(8401, 146, 151)
						d.zodiac_notice_clear()
						d.zodiac_notice(string.format("(1.2 Görev) %s Ortaya Çýktý ! Onu bul ve ruhu serbest býrak  !", mob_name(8401)))
						d.setf("metin_stone", 1)
					end
					check = true
				end
			end
		end
		
		when kill with npc.get_race() == 8401 and pc.in_dungeon() begin
			local setting = devil_tower2_plechito.settings()
			
			if d.getf("lair_check1") == 1 then
				d.setf("lair_check1", 2)
				cleartimer("level_timer1")
				timer("level_timer1", 3)
			end
			if d.getf("lair_check1") == 3 then
				local items = {setting["keys"][1], setting["keys"][2]}
				local chance = number(1, 3)

				if chance <= 2 then
					game.drop_item(items[1], 1)
				elseif chance == 3 then
					game.drop_item(items[2], 1)
				end
			end
		end
		
		when 9201.take with item.vnum == 30330 and pc.in_dungeon() begin
			local setting = devil_tower2_plechito.settings()
			pc.remove_item(30330, 1)

			d.notice("Doðru anahtarý buldun ! sonraki aþamaya geçiyorsun !")
			npc.purge()
			d.clear_regen()
			d.kill_all()
			d.setf("lair_check1", 4)
			cleartimer("level_timer1")
			timer("level_timer1", 3)
		end
		
		when kill with npc.get_race() == 805 or npc.get_race() == 804 or npc.get_race() == 803 or npc.get_race() == 809 or npc.get_race() == 810 begin
			local count, check_2 = d.getf("mobs_level_2") + 1, false;
			
			if d.getf("lair_check1") == 2 then
				if count < 60 and check_2 == false then
					d.setf("mobs_level_2", count)
				else
					if d.getf("boss_check1") == 0 then
						d.clear_regen()
						d.kill_all()
						d.zodiac_notice_clear()
						d.zodiac_notice(string.format("(2.1 Görev) %s belirdi bul ve yok et !", mob_name(806)))
						devil_tower2_plechito.spawn_boss(2)
						d.setf("boss_check1", 1)
					end
					check_2 = true
				end
			end
		end
		
		when kill with npc.get_race() == 806 and pc.in_dungeon() begin
			d.notice(string.format("%s baþarý ile yok edildi !", mob_name(806)))
			d.notice("Bir sonraki göreve hazýrlan  !")
			d.setf("lair_check1", 3)
			cleartimer("level_timer1")
			cleartimer("level_two_timer")
			timer("level_timer1", 3)
		end
		
		when kill with npc.get_race() == 807 or npc.get_race() == 808 or npc.get_race() == 809 or npc.get_race() == 812 begin
			local count, check_3 = d.getf("mobs_level_3") + 1, false;
			
			if d.getf("lair_check1") == 4 then
				if count < 30 and check_3 == false then
					d.setf("mobs_level_3", count)
				else
					if d.getf("boss_check1_2") == 0 then
						d.clear_regen()
						d.kill_all()
						devil_tower2_plechito.spawn_boss(4)
						d.zodiac_notice_clear()
						d.zodiac_notice(string.format("(4.1 Görev) %s a belirdi! Onu yoket !", mob_name(817)))
						d.setf("boss_check1_2", 1)
					end
					check_3 = true
				end
			end
		end
		
		when kill with npc.get_race() == 817 and pc.in_dungeon() begin
			d.notice(string.format("Harika !  %s'yi yok ettin bir sonraki aþamaya geçiliyor ! ", mob_name(817)))
			d.setf("lair_check1", 5)
			cleartimer("level_timer1")
			timer("level_timer1", 3)
		end
		
		when kill with npc.get_race() == 8402 and pc.in_dungeon() begin
			local chance = number(1, 4)
			
			if chance < 2 then
				if d.getf("first_root") == 0 then
					d.zodiac_notice_clear()
					d.zodiac_notice("(5.Görev) Doðru metin yok edildi ! Kalan doðru metin: 1 !")	
					d.setf("first_root", 1)
				else	
					d.notice("Son doðru metin taþýný buldun !")
					d.notice("Bir sonraki aþamada canavarlarý öldürerek kapýlarýn kilidini açmalýsýn!")
					d.setf("lair_check1", 6)
					cleartimer("level_timer1")
					timer("level_timer1", 3)
				end
			else
				d.zodiac_notice_clear()
				d.zodiac_notice(string.format("(5. Görev) Yanlýþ %s yokettin !", mob_name(8402)))	
			end
		end
		
		when kill with npc.get_race() == 812 or npc.get_race() == 813 or npc.get_race() == 814 begin
			local count = d.getf("mobs_level_4") + 1;
			local purge_count = d.getf("purge_count")
			
			if d.getf("lair_check1") == 6 then
				if count < 10 then
					d.setf("mobs_level_4", count)
				else
					if purge_count < 10 then
						d.setf("purge_count", purge_count+1)
						d.zodiac_notice_clear()
						d.zodiac_notice(string.format("Canavar gruplarýný yok et ! Kalan grup %s !", 10-d.getf("purge_count")+1))
						d.setf("mobs_level_4", 0)
					else
						d.notice("Tüm anýtlarý serbest býraktýn !")
						d.notice("Bir sonraki seviyede tüm canavarlarý öldür ve Mavi Ölüm sana görünecek !")
						d.setf("lair_check1", 7)
						cleartimer("level_timer1")
						timer("level_timer1", 3)
					end
				end
			end
		end
		
		when kill with npc.get_race() == 811 or npc.get_race() == 814 or npc.get_race() == 815 begin
			local count, check_4 = d.getf("mobs_level_5") + 1, false;
			
			if d.getf("lair_check1") == 7 then
				if count < 10 and check_4 == false then
					d.setf("mobs_level_5", count)
				else
					if d.getf("boss_check1_4") == 0 then
						d.clear_regen()
						d.kill_all()
						devil_tower2_plechito.spawn_boss(7)
						d.zodiac_notice_clear()
						d.zodiac_notice(string.format("(7.1 Görev) %s göründü ! onu maðlub et !", mob_name(1095)))
						d.setf("boss_check1_4", 1)
					end
					check_4 = true
				end
			end
		end
		
		when kill with npc.get_race() == 1095 and pc.in_dungeon() begin
			d.notice(string.format("Ýyi iþ! Grubun %s karþý kazandý !", mob_name(1095)))
			d.notice("Bu zindandaki son patronu maðlub et !")
			d.clear_regen()
			d.kill_all()
			d.setf("lair_check1", 8)
			cleartimer("level_timer1")
			timer("level_timer1", 15)
			d.zodiac_notice_clear()
			d.zodiac_notice(string.format("15 Saniye içerisinde bir sonraki kata gönderileceksiniz."))
		end
		
		when kill with npc.get_race() == 816 and pc.in_dungeon() begin
			d.zodiac_notice_clear()
			d.zodiac_notice(string.format("%s Maðlub edildi , Tebrikler!", mob_name(816)))
			notice_all(string.format("%s grubu Mavi Ölüm Kulesini baþarý ile tamamladý!", pc.get_name()))
			d.notice("2 dakika içide ýþýnlanýyorsun !")
			d.clear_regen()
			d.kill_all()
			d.setf("lair_check1", 0)
			timer("final1", 60*2)
		end
		
		--[[ TOWER STATEMENT END ]]
		
		--[[ TIMER STATEMENT BEGIN ]]
		
		when level_timer1.timer begin
			local get_flag = d.getf("lair_check1")
			local setting = devil_tower2_plechito.settings()
			
			d.clear_regen()
			d.kill_all()
			
			if get_flag == 2 then
				d.jump_all (setting["spawm_pos"][2][1], setting["spawm_pos"][2][2])
				devil_tower2_plechito.get_regens(2)
				d.setf("lair_check1", 2)
				d.zodiac_notice_clear()
				d.zodiac_notice("(2.Görev) Bu aþamadaki canavarlarý 10 dakika içerisinde yok et !")
			end
			if get_flag == 3 then
				d.jump_all (setting["spawm_pos"][3][1], setting["spawm_pos"][3][2])
				devil_tower2_plechito.get_regens(3)
				d.setf("lair_check1", 3)
				d.zodiac_notice_clear()
				d.zodiac_notice("(3.Görev)Metinden Maat Taþý düþür ve Þeytani anýta sürükleyerk üst kata geç.")
			end
			if get_flag == 4 then
				d.jump_all (setting["spawm_pos"][4][1], setting["spawm_pos"][4][2])
				devil_tower2_plechito.get_regens(4)
				d.setf("lair_check1", 4)
				d.zodiac_notice_clear()
				d.zodiac_notice("(4.Görev) Bütün yaratýklarý temizle ve Patronu çaðýr !")
			end
			if get_flag == 5 then
				d.jump_all (setting["spawm_pos"][5][1], setting["spawm_pos"][5][2])
				devil_tower2_plechito.get_regens(5)
				d.setf("lair_check1", 5)
				d.zodiac_notice_clear()
				d.zodiac_notice("(5. Görev) Doðru metin taþýný bulana kadar metinleri yoket !")
			end
			if get_flag == 6 then
				d.jump_all (setting["spawm_pos"][6][1], setting["spawm_pos"][6][2])
				devil_tower2_plechito.get_regens(6)
				d.setf("lair_check1", 6)
				d.zodiac_notice_clear()
				d.zodiac_notice("(6. Görev) Dalga Dalga canavarlarý yok et !")
			end
			if get_flag == 7 then
				d.jump_all (setting["spawm_pos"][7][1], setting["spawm_pos"][7][2])
				devil_tower2_plechito.get_regens(7)
				d.setf("lair_check1", 7)
				d.zodiac_notice_clear()
				d.zodiac_notice("(7. Görev) Canavarlarý temizle ve sana görünecek Mavi Ölümü yoket !")
			end
			if get_flag == 8 then
				d.jump_all (setting["spawm_pos"][8][1], setting["spawm_pos"][8][2])
				devil_tower2_plechito.get_regens(8)
				d.setf("lair_check1", 8)
				d.zodiac_notice_clear()
				d.zodiac_notice("(Son Görev) Þeytaný Kurt'u yok et !")
			end
		end
		
		when level_two_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.set_warp_location(41, 9694, 2786)
				d.notice("Zindan tamamlanamadý !")
				d.notice("Zaman doldu !")
				--notice_all(string.format("%s grubu Mavi Ölüm Kulesi yapmaya çalýþmakta baþarýsýz oldu!", pc.get_name()))
				server_timer('fail_bt1', 5,d.get_map_index())
				--timer("final1", 60*1)
				clear_server_timer("level_two_timer",d.get_map_index())
			end
		end
		
		when fail_bt1.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.exit_all_to_start_position()
			end
		end
		
		when final1.timer begin
			clear_server_timer("level_two_timer",d.get_map_index())
			d.exit_all_to_start_position()
		end
		
		--[[ TIMER STATEMENT END ]]
		
		--[[ CHAT BEGIN ]]
		when 9201.chat."Mavi Ölüm Kulesi " begin
			-- Quest window dungeon image
			addimage(35,35,'deneme31.tga')
			local settings = devil_tower2_plechito.settings()
			
			say(mob_name(20348))
			say("")
			
			if not party.is_party() then
				-- Quest window dungeon image
				addimage(35,35,'deneme31.tga')say("")say("")say("")say("")say("")

				say("Bu zindana giriþ yapabilmen için,")
				say("bir grupta bulunman gerekiyor .")
				return
			end
			
			if not party.is_leader() then
				-- Quest window dungeon image
				addimage(35,35,'deneme31.tga')say("")say("")say("")say("")

				say("Mavi ölüm kulesine girmek istiyorsanýz,")
				say("bir grubun lideri olmalýsýn .")
				return
			end
			
			if party.get_near_count() < 2 then
				-- Quest window dungeon image
				addimage(35,35,'deneme31.tga')say("")say("")say("")say("")

				say("Mavi ölüm kulesine girmek istiyorsanýz ")
				say("bir grupta bulunmalýsýnýz.")
				return
			end
			
			local levelCheck, maximlevelCheck, itemCheck, durationCheck = true, true, true, true
			local noDurationMembers, noLevelMembers, noitemCheckMembers = {},{},{}
			
			for i, pid in next, devil_tower2_plechito.party_get_member_pids(), nil do
				q.begin_other_pc_block(pid)
				
				if pc.get_level() < settings["level_check1"]["minimum"] then
					table.insert(noLevelMembers, pc.get_name())
					levelCheck = false
				end
				if pc.get_level() > settings["level_check1"]["maximum"] then
					table.insert(noLevelMembers, pc.get_name())
					maximlevelCheck = false
				end
				
				if pc.count_item(31336) <= 0 then
					table.insert(noitemCheckMembers, pc.get_name())
					itemCheck = false
				end
				
				if get_time() < pc.getqf("bt_player_time_duration") then
					table.insert(noDurationMembers, pc.get_name())
					durationCheck = false
				end

				q.end_other_pc_block()
			end
			
			if not levelCheck then
				-- Quest window dungeon image
				addimage(35,35,'deneme31.tga')say("")say("")say("")say("")

				say("Mavi ölüm kulesine girmek istiyorsanýz,")
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
				-- Quest window dungeon image
				addimage(35,35,'deneme31.tga')say("")say("")say("")say("")

				say("Mavi ölüm kulesine girmek istiyorsanýz,")
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
				addimage(35,35,'deneme31.tga')say("")say("")say("")say("")

				say("Bu zindana giriþ yapabilmen için,")
				say("grubun her üyesinin zindan biletine")
				say("sahip olmasý gerekiyor bir üyenin")
				say("Envanterinde yeterli miktarda yok:")
				say_item(item_name(31336),31336, "")
				for i, name in next, noitemCheckMembers, nil do
					say(color(1,1,0), "    "..name)
				end
				return
			end

			
			if not durationCheck then
				-- Quest window dungeon image
				addimage(35,35,'deneme31.tga')say("")say("")say("")say("")

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
				addimage(35,35,'deneme31.tga')say("")say("")say("")say("")
				say("Sadece 1. kanaldan giriþ yapabilirsin.")
				return
			end

			for i, pid in next, devil_tower2_plechito.party_get_member_pids(), nil do
				q.begin_other_pc_block(pid)
				pc.remove_item(31336, 1)
				pc.setqf("bt_player_time_duration", get_time()+60*60*2)
				q.end_other_pc_block()
			end
			
			-- Quest window dungeon image
			addimage(35,35,'deneme31.tga')say("")say("")say("")say("")say("")

			say("Mavi ölüm kalesine ýþýnlanýyorsun,")
			say("'Devam' dedikten sonra bütün üyeler ýþýnlanacaklar .")
			wait()
			devil_tower2_plechito.make_dungeon()
		end
		when 9201.chat."Maðara Süre sýfýrla " with (game.get_event_flag("bt_dung_time") == 1) begin
			local select_timer = select("Sýfýrla", "Sýfýrlama")
			if select_timer == 1 then
				pc.setqf("bt_player_time_duration", 0)
			end
		end
		--[[ CHAT END ]]
	end
end