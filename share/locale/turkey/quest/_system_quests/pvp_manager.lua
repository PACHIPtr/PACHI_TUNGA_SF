--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest pvp_manager begin
	state start begin
		when 9006.chat."GM:PvP Turnuvasýna Iþýnlan " with pc.is_gm() == true begin
			say("Haritaya gitmek istiyor musun?")
			say("")
			local a = select(locale.yes,locale.no)
			if a == 1 then	
				pc.warp(6585800,6585800)
			end
		end
		when login with game.get_event_flag("vs_ac") == 1 begin
			send_letter("*Sayokan Turnuvasý ")
		end
		when button or info begin
			addimage(20, 12, "sayokan.tga")
			say("")
			say("")
			say("")
			say("")
			say("Sevgili oyuncular, Sayokan Turnuvasý ")
			say("baþlamýþtýr. Sende turnuvaya katýlarak. ")
			say("eþsiz ödüller elde edebilirsin")
			say("")
			say_reward("Myte2 Takýmýn.")
			send_letter("*Sayokan Turnuvasý ")
		end
		when 9006.chat."GM:PvP Turnuvasýný Baþlat " with pc.is_gm() == true begin
			if not pc.is_gm() then
				setskin(NOWINDOW)
				return
			end
			say_reward("Turnuvayý baþlatmak istiyor musun?")
			local s = select("Turnuvayý baþlat","Turnuvayý kapat","Vazgeç ")
			if s == 1 then
				game.set_event_flag("pvp_minlevel",55)
				game.set_event_flag("pvp_maxlevel",120)
				game.set_event_flag("pvp_price_count", 1)
				game.set_event_flag("pvp_price_vnum", 80017)
				pvp_manager.open_registrations()
				game.set_event_flag("vs_ac",1)
				server_timer("vs_kayit",10*60)
				notice_all("Sayokan Turnuvasý kayýtlarý 10 dakika sonra kapanacak.")
			else
				game.set_event_flag("vs_ac",0)
				notice_all("Sayokan Turnuvasý sona erdi.")
			end
		end
		when vs_kayit.server_timer begin
			pvp_manager.close_registrations()
			notice_all("Turnuva Ödülü "..game.get_event_flag("pvp_price_count").."x "..item_name(game.get_event_flag("pvp_price_vnum")).." (vnum: "..game.get_event_flag("pvp_price_vnum")..")")
		end
				
		when login with pc.get_map_index() == 500 and (pvp_attenders == nil or pvp_attenders[pvp_manager.get_my_attender_id()] == nil or (pvp_attenders[pvp_manager.get_my_attender_id()].fight_mode != 1 and pvp_attenders[pvp_manager.get_my_attender_id()].fight_mode != 2)) begin
			if pvp_attenders == nil then
				pvp_manager.guard_check_position()
				timer("pvp_guard_1", 10)
				return
			end
			if pvp_attenders[pvp_manager.get_my_attender_id()] == nil then
				pvp_manager.guard_check_position()
				timer("pvp_guard_1", 10)
				return
			end
			if pvp_attenders[pvp_manager.get_my_attender_id()].fight_mode != 1 and pvp_attenders[pvp_manager.get_my_attender_id()].fight_mode != 2 then
				pvp_manager.guard_check_position()
				timer("pvp_guard_1", 10)
			end
		end
		when pvp_guard_1.timer begin
			if pc.get_map_index() != 500 then
				return
			end
			pvp_manager.guard_check_position()
			timer("pvp_guard_2", 10)
		end
		when pvp_guard_2.timer begin
			if pc.get_map_index() != 500 then
				return
			end
			pvp_manager.guard_check_position()
			timer("pvp_guard_1", 10)
		end
		function guard_check_position()
			if pc.get_local_x() >= 84 and pc.get_local_y() >= 84 and pc.get_local_x() <= 116 and pc.get_local_y() <= 116 then --arena 1
				pvp_manager.guard_reset()
			end
			if pc.get_local_x() >= 84 and pc.get_local_y() >= 136 and pc.get_local_x() <= 116 and pc.get_local_y() <= 168 then --arena 2
				pvp_manager.guard_reset()
			end
			if pc.get_local_x() >= 136 and pc.get_local_y() >= 84 and pc.get_local_x() <= 168 and pc.get_local_y() <= 116 then --arena 3
				pvp_manager.guard_reset()
			end
			if pc.get_local_x() >= 136 and pc.get_local_y() >= 136 and pc.get_local_x() <= 168 and pc.get_local_y() <= 168 then --arena 4
				pvp_manager.guard_reset()
			end
		end
		
		function guard_reset()
			notice_in_map(""..pc.get_name().." Arenadan uzaklaþtýrýldý!")
			pc . remove_item ( "40002" , 200 )
			pc.warp(344000,502500)
		end
		
		when 9006.chat."PvP Turnuvasýna Katýl " with game.get_event_flag("pvp") == 1 begin
			say_title("PvP Turnuvasýna Katýl:")
			if party.is_party() == true then
				say("Grup ile giremezsin.")
				return
			end
				say("Ne yapmak istiyorsun?")
				local s = select("PvP Arenasýna Iþýnlan " , "Kapat " ) 
				if s == 1 then
				pc.warp(6598400,6589800)
				else return
			end
		end
		
		when 20078.chat."Þehre Dön " with pc.get_map_index() == 500 begin
			say_title("Savaþ Ustasý: ")
			say("")
			say("Arenadan ayrýlýp þehre dönmek istiyormusun?")
			say("")
			local s = select("Evet ", "Hayýr ")
			if s == 1 then
				warp_to_village()
			end
		end
		when 20078.chat."Turnuva Bölgesini Temizle " with pc.get_map_index() == 500 and pc.is_gm() begin	
			say_title("PvP Turnuvasý : ")
			say("")
			say("Bölgeyi temizlemek istiyor musun?")
			say("")
			local temizle = select(locale.yes,locale.no)
			if temizle == 1 then
				warp_all_to_village(500,10)
			end
		end
		when 20078.chat."GM: PvP Turnuvasý Ýptal Et " with pc.get_map_index() == 500 and pc.is_gm() and game.get_event_flag("pvp") != 0 begin
			say_title("GM: PvP Turnuvasý Ýptal Et:")
			say("PvP Turnuvasý iptal etmek istiyormusun?")
			say("")
			local s = select("Evet ", "Hayýr ")
			if s == 1 then
				game.set_event_flag("pvp", 0)
				pvp_attenders = nil
				notice_all("Turnuva sona ermiþtir!")
				say_title("PvP Turnuvasý iptal edildi!")
				say("PvP Turnuvasý iptal edildi!")
				say("")
				if pvp_manager_callback != nil then
					pvp_manager_callback()
				end
			end
		end

		when 20078.chat."GM: PvP Turnuvasýný baþlat " with pc.get_map_index() == 500 and pc.is_gm() and game.get_event_flag("pvp") == 0 and ae_pvp_tournament == nil begin
			say_title("PvP Turnuvasýný baþlat:")
			say("Lütfen minimum level sýnýrýný gir (Min: 15):")
			local str_min_level = input()
			say_title("PvP Turnuvasýný baþlat:")
			local min_level = tonumber(str_min_level)
			if min_level == nil then
				say("Numara girilmedi.")
				say("")
				return
			end
			if tonumber(str_min_level) < 15 then
				min_level = 15
			end
			say("Minimum level : "..tostring(min_level)..".")
			say("lütfen maksimum level sýnýrýný gir:")
			local str_max_level = input()
			say_title("PvP Turnuvasýný baþlat:")
			local max_level = tonumber(str_max_level)
			if max_level == nil then
				say("Numara girilmedi.")
				say("")
				return
			end
			if tonumber(str_max_level) < min_level then
				say("Maks level min levelden yüksek olmalý.")
				say("")
				return
			end
			
			game.set_event_flag("pvp_minlevel", min_level)
			game.set_event_flag("pvp_maxlevel", max_level)
			
			say("Minimum Level: "..tostring(min_level).." olarak ayarlandý.")
			say("Maksimum Level: "..tostring(max_level).." olarak ayarlandý.")
			say("")
			local s = select("Baþlat ", "Ýptal et ")
			if s == 2 then
				return
			end
			
			say_title("PvP Turnuvasýný baþlat:")
			say("")
			say("PvP Turnuvasý baþladý.")
			say("")
			say_reward("Level Limiti: "..game.get_event_flag("pvp_minlevel").." - "..game.get_event_flag("pvp_maxlevel"))
			pvp_manager.open_registrations()
		end
		function open_registrations()
			game.set_event_flag("pvp", 1)
			attender_count = 0
			game.set_event_flag("pvp_tournament", get_global_time())
			pvp_attenders = nil
			pvp_attenders = { }
			
			arena = {
						-- arena 1
						{
							{ 113, 100 },
							{ 88, 100 },
							0,
							"SLEEPING",
							0,
						},
						-- arena 2
						{
							{ 113, 152 },
							{ 88, 152 },
							0,
							"SLEEPING",
							0,
						},
						-- arena 3
						{
							{ 140, 152 },
							{ 164, 152 },
							0,
							"SLEEPING",
							0,
						},
						-- arena 4
						{
							{ 164, 100 },
							{ 140, 100 },
							0,
							"SLEEPING",
							0,
						},
					}
			
			notice_all("PvP Turnuvasý baþlamýþtýr.")
			notice_all("Giriþler için "..mob_name(9006).."'e gidin.")
			notice_all("Level limiti: "..game.get_event_flag("pvp_minlevel").." - "..game.get_event_flag("pvp_maxlevel").."")
		end
		when 20078.chat."Turnuvaya Katýl " with pc.get_map_index() == 500 and game.get_event_flag("pvp") == 1 begin
			say_title(mob_name(20078)..":")
			if party.is_party() == true then	
				say("Grubun varken katýlamazsýn.")
				say("Grubunu boz öyle gir.")	
				return
			end
			if game.get_event_flag("pvp_tournament") == pc.getqf("pvp_tournament") then
				say("Turnuvaya zaten kayýt yaptýrdýn.")
				say("")
				return
			end
			if pc.get_level() < 15 then
				say("Levelin çok düþük.")
				say("")
				return
			end
			if pc.get_level() < game.get_event_flag("pvp_minlevel") or pc.get_level() > game.get_event_flag("pvp_maxlevel") then
				say("Levelin  "..game.get_event_flag("pvp_minlevel").." ile "..game.get_event_flag("pvp_maxlevel").." arasýnda olmasý gerekli.")
				say("")
				return
			end
			say("Budokana hoþgeldin. Turnuvada senin gibi ")
			say("oyuncularla savaþacaksýn.")
			say("")
			say("Karþýna çýkan her oyuncuyu öldür.")
			say("")
			say("Bu turnuvaya katýlmak istiyormusun?")
			say("")
			say_reward("Kayýt Ücreti : 15.000.000 Yang ")
			say("")
			local s = select("Evet, katýlmak istiyorum ", "Hayýr, ben bir korkaðým ")
			say_title(mob_name(20078)..":")
			if s == 1 then
				if pc.get_gold() < 15000000 then	
					say_title("PvP Turnuvasý ")
					say("")
					say(15000000-pc.get_gold().." yang eksiðin var.")
					say("Paraný tamamlayýnca turnuvaya girebilirsin.")
					return
				end
				say("Turnuva baþladýðý zaman seni çaðýracaðým.")
				say("")
				say("Ýyi þanslar evlat!")
				say("")
				
				pvp_attenders[attender_count+1] = {name = pc.getname(), level = pc.get_level(), partner = nil, winner = true, fight_mode = 0, arena = 0}
				pc.setqf("attender_id", attender_count+1)
				pc.setqf("pvp_tournament", game.get_event_flag("pvp_tournament"))
				attender_count = attender_count + 1
				pc.change_gold(-15000000)
				
				if achievement_manager != nil then
					achievement_manager.on_participatepvpevent()
				end
				
			elseif s == 2 then
				say("Çok zayýf gözüküyorsun evlat ")
				say("diðer oyunculara karþý hiç þansýn olamaz.")
				say("")
			end
		end
		when 20078.chat." PvP Turnuvasý için yapýlmýþ kayýtlar " with pc.get_map_index() == 500 and pc.is_gm() and game.get_event_flag("pvp") == 1 begin
			say_title(" PvP Turnuvasý ")
			say("Þuana kadar "..tostring(attender_count).." oyuncu kayýt oldu.")
			if ae_pvp_tournament != nil then
				say("")
				return
			end
			say(" Kayýtlarý kapamak istiyormusun? ")
			say("")
			local s = select("Kayýtlarý kapat ", "Kayýt olmaya devam etsinler ")
			if s == 1 then
				say_title("Kayýtlarý Kapat:")
				say("Kayýtlar kapandý.")
				say("")
				pvp_manager.close_registrations()
			end
		end
		
		function close_registrations()
			if attender_count == 0 then
				notice_all("Giriþler kapandý , kayýt yaptýramazsýn.")
				game.set_event_flag("pvp", 0)
				pvp_attenders = nil
				if pvp_manager_callback != nil then
					pvp_manager_callback()
				end
				return
			end
			game.set_event_flag("pvp", 2)
			pvp_manager.make_duel_list()
			duel_list_index = 0
			playerless_mode = false
			notice_all("PvP Turnuvasý baþlýyor!")
			pvp_manager.refresh_arenas()
		end
		
		when 20078.chat."Þimdi kiminle düello yapacaðým?" with pc.get_map_index() == 500 and game.get_event_flag("pvp") == 2 and game.get_event_flag("pvp_tournament") == pc.getqf("pvp_tournament") begin
			say_title(mob_name(20078)..":")
			if pvp_manager.get_duel_partner() == nil then
				say("Þuanda sana uygun aday yok.")
				say("")
				say("Eðer sana uygun aday bulamazsak bir sonraki ")
				say("rounda geçeceksin.")
				return
			end
			say("Düello yapacaðýn kiþi:")
			say("")
			say_reward(""..pvp_manager.get_duel_partner().."")
		end
		

		function make_duel_list()
			duel_list = { }
			local i = 0
			for ai = 1, attender_count, 1 do
				if pvp_attenders[ai] != nil and pvp_attenders[ai].partner == nil and pvp_attenders[ai].winner == true then
					local partner_id = pvp_manager.findpartner(tostring(pvp_attenders[ai].name), pvp_attenders[ai].level)
					if partner_id != nil then
						pvp_attenders[ai].partner = tostring(pvp_attenders[partner_id].name)
						pvp_attenders[partner_id].partner = tostring(pvp_attenders[ai].name)
						duel_list[i] = {fighter_id1 = ai, fighter_id2 = partner_id}
						i = i + 1
					end
				end
			end
			
			for ai = 1, attender_count, 1 do
				if pvp_attenders[ai] != nil and pvp_attenders[ai].partner == nil and pvp_attenders[ai].winner == true then
					local partner_id = pvp_manager.findpartner_for_partnerless(tostring(pvp_attenders[ai].name), pvp_attenders[ai].level)
					if partner_id != nil then
						pvp_attenders[ai].partner = tostring(pvp_attenders[partner_id].name)
						pvp_attenders[partner_id].partner = tostring(pvp_attenders[ai].name)
						duel_list[i] = {fighter_id1 = ai, fighter_id2 = partner_id}
						i = i + 1
					end
				end
			end
		end
		
		
		function findpartner(name, level)
			local closest_level = nil
			local partner_id = nil
			for ai = 1, attender_count, 1 do
				if pvp_attenders[ai] != nil and pvp_attenders[ai].winner == true and tonumber(pvp_attenders[ai].level) <= level + 30 and tonumber(pvp_attenders[ai].level) >= level - 30 and tostring(pvp_attenders[ai].name) != name and pvp_attenders[ai].partner == nil and pvp_manager.is_player_in_map(tostring(pvp_attenders[ai].name)) == true then
					if closest_level == nil then
						closest_level = pvp_attenders[ai].level
						partner_id = ai
					elseif tonumber(pvp_attenders[ai].level) == tonumber(level) then
						closest_level = pvp_attenders[ai].level
						partner_id = ai
					elseif tonumber(level) < tonumber(closest_level) and tonumber(pvp_attenders[ai].level) < tonumber(closest_level) then
						closest_level = pvp_attenders[ai].level
						partner_id = ai
					elseif tonumber(level) > tonumber(closest_level) and tonumber(pvp_attenders[ai].level) > tonumber(closest_level) then
						closest_level = pvp_attenders[ai].level
						partner_id = ai
					end
				end
			end
			return partner_id
		end
		
		function findpartner_for_partnerless(name, level)
			local closest_level = nil
			local partner_id = nil
			for ai = 1, attender_count, 1 do
				if pvp_attenders[ai] != nil and pvp_attenders[ai].winner == true and tostring(pvp_attenders[ai].name) != name and pvp_attenders[ai].partner == nil and pvp_manager.is_player_in_map(tostring(pvp_attenders[ai].name)) == true then
					if closest_level == nil then
						closest_level = pvp_attenders[ai].level
						partner_id = ai
					elseif pvp_attenders[ai].level == level then
						closest_level = pvp_attenders[ai].level
						partner_id = ai
					elseif tonumber(level) < tonumber(closest_level) and pvp_attenders[ai].level < closest_level then
						closest_level = pvp_attenders[ai].level
						partner_id = ai
					elseif tonumber(level) > tonumber(closest_level) and pvp_attenders[ai].level > closest_level then
						closest_level = pvp_attenders[ai].level
						partner_id = ai
					end
				end
			end
			return partner_id
		end
		
		function is_player_in_map(name)
			if pc.getname() == name then
				return true
			end
			local myname = pc.getname()
			local target = find_pc_by_name(name)
			local t = pc.select(target)
			if pc.getname() == myname then
				pc.select(t)
				return false
			end
			pc.select(t)
			return true
		end
		
		
		when 20078.chat."GM: Arenayý Temizle" with pc.is_gm() and game.get_event_flag("pvp") == 2 begin
			for aArena = 1, 4, 1 do
				arena[aArena][3] = 0
				arena[aArena][4] = "SLEEPING"
			end
			pvp_manager.refresh_arenas()
		end
		
		-- not working
		--[[
		when 20078.chat."GM: Düelloyu bitir " with pc.is_gm() and game.get_event_flag("pvp") == 2 begin
			say_title("Düelloyu bitir:")
			say("Hangi arenadaki düelloyu bitireceksin?")
			local s = select("Arena 1", "Arena 2", "Arena 3", "Arena 4", "Kapat")
			
			if s == 5 then
				return
			end
			
			local fighter1 = nil
			local fighter2 = nil
			for ai = 1, attender_count, 1 do
				if pvp_attenders[ai].arena == s and pvp_attenders[ai].fight_mode != 0 then
					if fighter1 == nil then
						fighter1 = ai
					else
						fighter2 = ai
					end
				end
			end
			
			if fighter1 == nil then
				say_title("Düelloyu bitir:")
				say("Bu arenada þuan düello yok.")
				say("")
				return
			end
			
			say_title(" Düelloyu bitir:")
			say("Düelloyu bitirirsen ")
			say(""..s..". Arenadaki düelloyu kim kazanacak?")
			local w = select(tostring(pvp_attenders[fighter1].name), tostring(pvp_attenders[fighter1].name), " Kapat ")
			if w == 3 then
				return
			end
			
			if w == 1 then
				pvp_attenders[fighter2].winner = false
				notice_in_map(tostring(pvp_attenders[fighter1].name).." , "..tostring(pvp_attenders[fighter2].name).." karþýsýndaki düelloyu kazandý!", pc.get_map_index())
				pvp_manager.local_pc_warp(tostring(pvp_attenders[fighter1].name, 126, 40))
				pvp_manager.local_pc_warp(tostring(pvp_attenders[fighter2].name, 126, 40))
			else
				pvp_attenders[fighter1].winner = false
				notice_in_map(tostring(pvp_attenders[fighter2].name).." , "..tostring(pvp_attenders[fighter1].name).." karþýsýndaki düelloyu kazandý!", pc.get_map_index())
				pvp_manager.local_pc_warp(tostring(pvp_attenders[fighter1].name, 126, 40))
				pvp_manager.local_pc_warp(tostring(pvp_attenders[fighter2].name, 126, 40))
			end
			
			pvp_attenders[fighter1].fight_mode = 0
			pvp_attenders[fighter2].fight_mode = 0
			
			
			arena[s][3] = 0
			pvp_manager.refresh_arenas()
			
			say_title(" Düelloyu bitir: ")
			say(" Düello bitti. ")
		end
		--]]
		
		when 20078.chat."GM: PvP Turnuva Ödülü " with pc.is_gm() begin
			say_title("Ödül:")
			say("Þuanki Ödül "..game.get_event_flag("pvp_price_count").."x "..item_name(game.get_event_flag("pvp_price_vnum")).." (vnum: "..game.get_event_flag("pvp_price_vnum")..")")
			say("")
			local s = select("Ödülü Deðiþtir " , "Kapat ")
			if s == 2 then
				return
			end
			say_title("Ödül:")
			say("Yeni ödülün kodunu gir:")
			say("")
			local vnum = input()
			say_title("Ödül:")
			say("Yeni ödülün adetini gir:")
			say("")
			local count = input()
			game.set_event_flag("pvp_price_count", count)
			game.set_event_flag("pvp_price_vnum", vnum)
			say_title("Ödül deðiþtirildi:")
			say("Ödül deðiþtirildi.")
			say("Yeni ödül : "..count.."x "..item_name(vnum).." (vnum: "..vnum..")")
		end
		
		function refresh_arenas()
			for aArena = 1, 4, 1 do
				if duel_list[duel_list_index] == nil then
					if playerless_mode == true then
						pvp_manager.init_next_round()
					else
						pvp_manager.fight_playerless()
					end
					return
				end
				if arena[aArena][3] == 0 then
					arena[aArena][3] = 1
					pvp_manager.fight_init(duel_list[duel_list_index].fighter_id1, duel_list[duel_list_index].fighter_id2, aArena)
				end
			end
		end
		

		function fight_init(pid1, pid2, arena_id)
			duel_list_index = duel_list_index + 1
			if pvp_manager.is_player_in_map(tostring(pvp_attenders[pid1].name)) == false then
				notice_in_map(""..tostring(pvp_attenders[pid1].name).." adlý oyuncu oyunda olmadýðýndan diskalifiye edildi.", pc.get_map_index())
				pvp_attenders[pid1].winner = false
				pvp_attenders[pid2].partner = nil
				arena[arena_id][3] = 0
				pvp_manager.refresh_arenas()
				return
			end
			if pvp_manager.is_player_in_map(tostring(pvp_attenders[pid2].name)) == false then
				notice_in_map(""..tostring(pvp_attenders[pid2].name).." adlý oyuncu oyunda olmadýðýndan diskalifiye edildi.", pc.get_map_index())
				pvp_attenders[pid2].winner = false
				pvp_attenders[pid1].partner = nil
				arena[arena_id][3] = 0
				pvp_manager.refresh_arenas()
				return
			end
			
			notice_in_map(tostring(pvp_attenders[pid1].name).." ve "..tostring(pvp_attenders[pid2].name).." 'nýn düellosu birazdan baþlayacak!", pc.get_map_index())
			
			--[[
			local f1_accept = ask_fight(tostring(pvp_attenders[pid1].name), tostring(pvp_attenders[pid2].name))
			if f1_accept != CONFIRM_OK then
				notice_in_map(" "..tostring(pvp_attenders[pid1].name).." has not took on the fight against "..tostring(pvp_attenders[pid2].name)..". ", pc.get_map_index())
				pvp_attenders[pid1] = nil
				pvp_attenders[pid2].partner = nil
				pvp_manager.refresh_arenas()
				return
			end
			
			local f2_accept = confirm(find_pc_by_name(tostring(pvp_attenders[pid2].name)), "Trittst du den Kampf gegen "..tostring(pvp_attenders[pid1].name).. " an?", 15)
			if f2_accept != CONFIRM_OK then
				notice_in_map(" "..tostring(pvp_attenders[pid2].name).." has not took on the fight against "..tostring(pvp_attenders[pid1].name)..". ", pc.get_map_index())
				pvp_attenders[pid2] = nil
				pvp_attenders[pid1].partner = nil
				pvp_manager.refresh_arenas()
				return
			end
			]]
			
			pvp_attenders[pid1].fight_mode = 1
			pvp_attenders[pid2].fight_mode = 1
			pvp_attenders[pid1].arena = arena_id
			pvp_attenders[pid2].arena = arena_id
			
			pvp_manager.local_pc_warp(tostring(pvp_attenders[pid1].name), arena[arena_id][1][1], arena[arena_id][1][2])
			pvp_manager.local_pc_warp(tostring(pvp_attenders[pid2].name), arena[arena_id][2][1], arena[arena_id][2][2])

		end
		
		when login with pc.get_map_index() == 500 and game.get_event_flag("pvp") == 2 and pvp_manager.get_fight_mode() == 1 begin
			affect.add(apply.MOV_SPEED, -10000, 2)
			affect.add_collect(apply.HP_REGEN, 5000, 60)
			affect.add_collect(apply.SP_REGEN, 5000, 60)
			pvp_manager.set_fight_mode(2)
			if arena[pvp_manager.get_arena()][3] == 2 then
				arena[pvp_manager.get_arena()][5] = 60
				loop_timer("fight_start_countdown", 1)
			else
				timer("partner_timeout", 40)
			end
			arena[pvp_manager.get_arena()][3] = 2
		end		
		
		when partner_timeout.timer begin
			if pvp_manager.is_player_in_map(pvp_manager.get_duel_partner()) == true or game.get_event_flag("pvp") != 2 or pvp_manager.get_fight_mode() != 2 then
				return
			end
			pvp_manager.set_fight_mode(3)
			if arena[pvp_manager.get_arena()][3] == 3 then
				return
			end
			arena[pvp_manager.get_arena()][3] = 3
			notice_in_map(pc.getname().." , "..pvp_manager.get_duel_partner().." karþýsýndaki düelloyu kaybetti!", pc.get_map_index())
			pvp_attenders[pvp_manager.get_attender_id_by_name(pvp_manager.get_duel_partner())].fight_mode = 3
			pvp_attenders[pvp_manager.get_my_attender_id()].winner = true
			pvp_attenders[pvp_manager.get_attender_id_by_name(pvp_manager.get_duel_partner())].winner = false
			pc.warp_local(pc.get_map_index(), 12600, 4000)
		end

		when fight_start_countdown.timer begin
			if arena[pvp_manager.get_arena()][5] == -1 then
				return
			end
			local partner_vid = find_pc_by_name(pvp_manager.get_duel_partner())
			local my_vid = pc.get_vid()
			if arena[pvp_manager.get_arena()][5] == 0 then
				arena[pvp_manager.get_arena()][5] = -1
				pc.select(partner_vid)
				command("pvp "..my_vid)
				pc.select(my_vid)
				command("pvp "..partner_vid)
				notice_in_map(""..pc.getname().."  ile "..pvp_manager.get_duel_partner().."  arasýndaki düello baþladý!", pc.get_map_index())
				timer("fight_start_countdown_shutdown", 1)
			else
				pc.select(partner_vid)
				if math.mod(arena[pvp_manager.get_arena()][5], 5) == 0 then
					if tostring(arena[pvp_manager.get_arena()][5]) == "60" then
						local f = "pvp"
						pc.setf(f,"item_block",0)
						if pc.get_job() == 2 or pc.get_job() == 6 then
							pc.setf(f,"block_cozme",1)
						end
						syschat("Ýtemlerini deðiþtirmek için 60 saniyen var.")
						syschat("60 saniye sonunda item deðiþtiremeyeceksin.")
					elseif tostring(arena[pvp_manager.get_arena()][5]) == "15" then
						local f = "pvp"
						pc.setf(f,"item_block",1)
						if pc.get_job() == 2 or pc.get_job() == 6 then
							pc.setf(f,"block_cozme",0)
						end
						syschat("Ýtem deðiþtirme süresi sona erdi.")
					end
					syschat("Düello "..tostring(arena[pvp_manager.get_arena()][5]).." saniye içinde baþlayacak.")
					-- pc . remove_item ( "27003" , 20000000 )

				end
				affect.add(apply.MOV_SPEED, -10000, 2)
				pc.select(my_vid)
				if math.mod(arena[pvp_manager.get_arena()][5], 5) == 0 then
					if tostring(arena[pvp_manager.get_arena()][5]) == "60" then
						local f = "pvp"
						pc.setf(f,"item_block",0)
						syschat("Ýtemlerini deðiþtirmek için 60 saniyen var.")
						syschat("60 saniye sonunda item deðiþtiremeyeceksin.")
					elseif tostring(arena[pvp_manager.get_arena()][5]) == "15" then
						local f = "pvp"
						pc.setf(f,"item_block",1)
						syschat("Ýtem deðiþtirme süresi sona erdi.")
					end
					syschat("Düello "..tostring(arena[pvp_manager.get_arena()][5]).." saniye içinde baþlayacak.")
					-- pc . remove_item ( "27003" , 20000000 )

				end
				affect.add(apply.MOV_SPEED, -10000, 2)
				arena[pvp_manager.get_arena()][5] = arena[pvp_manager.get_arena()][5] - 1
			end
		end
		
		
		when login with pc.get_map_index() != 500 begin
			local f = "pvp"
			pc.setf(f,"item_block",0)
			if pc.get_job() == 2 or pc.get_job() == 6 then
				pc.setf(f,"block_cozme",0)
			end
		end
		
		when login with pc.get_map_index() == 500 begin
			if pc.get_job() == 2 or pc.get_job() == 6 then
				local f = "pvp"
				pc.setf(f,"block_cozme",0)
			end
		end

		when fight_start_countdown_shutdown.timer begin
			cleartimer("fight_start_countdown")
		end
		
		when logout with game.get_event_flag("pvp") == 2 and pc.get_map_index() == 500 and pvp_manager.get_fight_mode() == 2 begin
			pvp_manager.set_fight_mode(3)
			arena[pvp_manager.get_arena()][3] = 3
			notice_in_map(""..pvp_manager.get_duel_partner().." , "..pc.getname().." karþýsýndaki düelloyu kazandý!", pc.get_map_index())
			pvp_attenders[pvp_manager.get_attender_id_by_name(pvp_manager.get_duel_partner())].fight_mode = 3
			pvp_attenders[pvp_manager.get_my_attender_id()].winner = false
			pvp_attenders[pvp_manager.get_attender_id_by_name(pvp_manager.get_duel_partner())].winner = true
			
			pvp_manager.local_pc_warp(pvp_manager.get_duel_partner(), 126, 40)
		end
		
		when kill with game.get_event_flag("pvp") == 2 and pc.get_map_index() == 500 and npc.is_pc() and pvp_manager.get_fight_mode() == 2 begin
			pvp_manager.set_fight_mode(3)
			if arena[pvp_manager.get_arena()][3] == 3 then
				return
			end
			arena[pvp_manager.get_arena()][3] = 3
			notice_in_map(""..pc.getname().." , "..pvp_manager.get_duel_partner().." karþýsýndaki düelloyu kazandý!", pc.get_map_index())
			pvp_attenders[pvp_manager.get_attender_id_by_name(pvp_manager.get_duel_partner())].fight_mode = 3
			pvp_attenders[pvp_manager.get_my_attender_id()].winner = true
			pvp_attenders[pvp_manager.get_attender_id_by_name(pvp_manager.get_duel_partner())].winner = false
			
			timer("fight_end", 5)
		end
		
		when fight_end.timer begin
			pc.warp_local(pc.get_map_index(), 12600, 4000)
			pvp_manager.local_pc_warp(pvp_manager.get_duel_partner(), 126, 40)
		end
		
		when letter with pc.get_map_index() == 500 and game.get_event_flag("pvp") == 2 and pvp_manager.get_fight_mode() == 3 begin
			pvp_manager.set_fight_mode(0)
			
			if pvp_attenders[pvp_manager.get_my_attender_id()].winner == true then
				arena[pvp_manager.get_arena()][3] = 0
				pvp_manager.refresh_arenas()
			end
		end
		
		function fight_playerless()
			for aArena = 1, 4, 1 do
				if arena[aArena][3] != 0 then
					return
				end
			end
			playerless_mode = true

			
			pvp_manager.make_duel_list()
			pvp_manager.refresh_arenas()
		end
		
		function init_next_round()
			for aArena = 1, 4, 1 do
				if arena[aArena][3] != 0 then
					return
				end
			end
			playerless_mode = false
			
			local winner_count = 0
			--[[local]] winner_id = 0
			for ai = 1, attender_count, 1 do
				pvp_attenders[ai].partner = nil
				pvp_attenders[ai].fight_mode = 0
				if pvp_attenders[ai] != nil and pvp_attenders[ai].winner == true then
					winner_count = winner_count + 1
					winner_id = ai
				end
			end
			
			if winner_count == 1 then
				notice_all(""..pvp_attenders[winner_id].name.." turnuvayý kazandý!")
				notice_all(""..pvp_attenders[winner_id].name.." ,"..game.get_event_flag("pvp_price_count").." tane "..item_name(game.get_event_flag("pvp_price_vnum")).." kazandý.")
				local target = find_pc_by_name(pvp_attenders[winner_id].name)
				local t = pc.select(target)
				
				if achievement_manager != nil then
					achievement_manager.on_winpvpevent()
				end
				
				pc.give_item2(game.get_event_flag("pvp_price_vnum"), game.get_event_flag("pvp_price_count"))
				pc.select(t)
				notice_all("PvP Turnuvasý bitti.")
				game.set_event_flag("pvp", 0)
				--pvp_attenders = nil
				if pvp_manager_callback != nil then
					pvp_manager_callback()
				end
				return
			end
			
			
			pvp_manager.make_duel_list()
			duel_list_index = 0
			if winner_count == 2 then
				notice_all("Final roundu baþlýyor.")
			elseif winner_count <= 4 then
				notice_all("Yarý final roundu baþlýyor.")
			else
				notice_all("Round baþlýyor.")
			end
			pvp_manager.refresh_arenas()
		end
		
		function local_pc_warp(name, x, y)
			local target = find_pc_by_name(name)
			local t = pc.select(target)
			if x == 126 and y == 40 then
				local f = "pvp"
				pc.setf(f,"item_block",0)
			end
			pc.warp_local(pc.get_map_index(), x*100, y*100)
			pc.select(t)
			if x == 126 and y == 40 then
				local f = "pvp"
				pc.setf(f,"item_block",0)
			end
		end
		
		function setqf_by_name(name, flag, value)
			local target = find_pc_by_name(name)
			local t = pc.select(target)
			pc.setqf(flag, value)
			pc.select(t)
		end
		
		function getqf_by_name(name, flag)
			local target = find_pc_by_name(name)
			local t = pc.select(target)
			local rflag = pc.getqf(flag)
			pc.select(t)
			return rflag
		end
		
		function get_attender_id_by_name(name)
			for ai = 1, attender_count, 1 do
				if tostring(pvp_attenders[ai].name) == name then
					return ai
				end
			end
			return 0
		end
		
		function get_my_attender_id()
			return pvp_manager.get_attender_id_by_name(pc.get_name())
		end
		
		function get_duel_partner()
			if pvp_attenders[pvp_manager.get_my_attender_id()] != nil and pvp_attenders[pvp_manager.get_my_attender_id()].partner != nil then
				return pvp_attenders[pvp_manager.get_my_attender_id()].partner
			else
				return nil
			end
		end
		
		function get_fight_mode()
			if pvp_attenders != nil and pvp_manager.get_my_attender_id() != 0 and pvp_attenders[pvp_manager.get_my_attender_id()] != nil and pvp_attenders[pvp_manager.get_my_attender_id()].fight_mode != nil then
				return pvp_attenders[pvp_manager.get_my_attender_id()].fight_mode
			else
				return 0
			end
		end
		
		function set_fight_mode(fmode)
			pvp_attenders[pvp_manager.get_my_attender_id()].fight_mode = fmode
		end
		
		function get_arena()
			return pvp_attenders[pvp_manager.get_my_attender_id()].arena
		end
		
		function set_arena(id)
			pvp_attenders[pvp_manager.get_my_attender_id()].arena = id
		end
		
		function ask_fight(asker, target)
			return confirm(find_pc_by_name(target), "Trittst du den Kampf gegen "..asker.. " an?", 15)
		end
		
		
	end
end
