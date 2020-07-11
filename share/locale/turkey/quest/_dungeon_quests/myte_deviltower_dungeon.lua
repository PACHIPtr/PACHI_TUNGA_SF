--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest deviltower_zone begin
    state start begin
        when login begin
            if pc.get_map_index() == 66 then
                if pc.get_x() < 2048+88 or pc.get_y() < 6656+577 or pc.get_x() > 2048+236 or pc.get_y() > 6656+737 then
                    --pc.warp(590500, 110500)
                end
                -- pc.set_warp_location(65, 5905, 1105)
            elseif pc.get_map_index() >= 660000 and pc.get_map_index() < 670000 then
                -- pc.set_warp_location(65, 5905, 1105)				
               -- deviltower_zone.register_player(pc.get_vid())
            end
        end

	when logout begin
		if pc.count_item(30300) >= 1 then
			pc.remove_item(30300, pc.count_item(30300))
		end
		if pc.count_item(30302) >= 1 then
			pc.remove_item(30302, pc.count_item(30302))
		end				
		if pc.get_map_index() >= 660000 and pc.get_map_index() < 670000 then
			-- deviltower_zone.unregister_player(pc.get_vid())		
			------------ Dungeon Turn Back System ----------------------
			pc.setf("devil_tower","dungeon_turn",1)
			------------------------------------------------------------
		end
	end
    when 20348.chat."Þeytan Kulesi " begin
	    if pc.get_level() < 40 then
			say_title("Þeytan Kulesi Bekçisi:")
			say(locale.deviltower_man_say_you_cant)
	    else
			say_title("Þeytan Kulesi Bekçisi:")
			addimage(35,35,'kule.tga')
			say("")
			say("")
			say("")
			say("")
			say(locale.deviltower_man_say)
			local secim = select(locale.kulesecim,locale.kulesecimiki,locale.kulesecimuc,"Vazgeçtim")
			if secim == 1 then
				------------ Dungeon Turn Back System ----------------------
				--pc.remove_turn_back_dungeon()
				------------------------------------------------------------
				pc.warp(216500,727000)
			elseif secim == 2 then
			
				say_npc()
				say("Eðer 50.000.000 yang verirsen seni hiçbir")
				say("þeyle uðraþtýrmadan 2.Kat'a gönderebilirim.")
				say("ister misin?")
				local icsecimiki = select("Girmek istiyorum","Girmek istemiyorum.")
				if icsecimiki ==1 then
					if pc.get_money() < 50000000 then
						say_npc()
						say_reward("Hay aksi!") 
						say("Seni içeri alamam envanterinde yeterli") 
						say("yang bulamadým bana 50.000.000 yang getirmelisin.") 
						return 
					end
					--[[if pc.is_busy() then
						say_npc()
						say_reward("Hay aksi!")							
						say("Diðer penceleri kapatmadan buraya giremezsin.")
						say("Ticaret, NPC, Depo gibi.")
						say("Ek olarak 10 saniye beklemen gerekebilir.")
						return
					end		--]]
					pc.change_money(-50000000) 
					--d.new_jump(66, special.devil_tower[1][1], special.devil_tower[1][2])
					------------ Dungeon Turn Back System ----------------------
					--pc.remove_turn_back_dungeon()
					------------------------------------------------------------
					d.new_jump(66, special.devil_tower[1][1] * 100, special.devil_tower[1][2] * 100)
					d.regen_file("data/dungeon/deviltower2_regen.txt")
					d.set_warp_at_eliminate(4, d.get_map_index(), special.devil_tower[2][1], special.devil_tower[2][2], "data/dungeon/deviltower3_regen.txt")
				end
			elseif secim == 3 then
				say_npc()
				say("Eðer 75.000.000 yang verirsen seni ve grubunu hiçbir")
				say("þeyle uðraþtýrmadan 2.Kat'a gönderebilirim.")
				say("Ýster misin?")
				local icsecimgrup = select("Girmek Ýstiyorum","Girmek Ýstemiyorum.")
				if icsecimgrup ==1 then
					if pc.get_money() < 75000000 then
						say_npc()
						say_reward("Hay aksi!") 
						say("Seni içeri alamam envanterinde yeterli") 
						say("yang bulamadým bana 75.000.000 yang getirmelisin.") 
						return 
					end
					if not party.is_party() then
						say_npc()
						say_reward("Heey "..pc.get_name().."")
						say("Bu iþlemi yapmak istiyorsan önce grup olmalýsýn")
						say("unutma kulede insanlar deðiþebilir kendine")
						say("güvenilir bir grup kurmaný tavsiye ederim")
						return
					end
					if not party.is_leader() then
						say_npc()
						say_reward("Heey "..pc.get_name().."")
						say("Cesaretini takdir ediyorum ama")
						say("yalnýzca grup lideri giriþ talebi yapabilir.")
						return
					end
					--[[if pc.is_busy() then
						say_npc()
						say_reward("Hay aksi!")							
						say("Diðer penceleri kapatmadan buraya giremezsin.")
						say("Ticaret, NPC, Depo gibi.")
						say("Ek olarak 10 saniye beklemen gerekebilir.")
						return
					end		--]]
					------------ Dungeon Turn Back System ----------------------
					--pc.remove_turn_back_dungeon()
					------------------------------------------------------------
					pc.change_money(-75000000) 
					d.new_jump_party(66, special.devil_tower[1][1], special.devil_tower[1][2])
					d.regen_file("data/dungeon/deviltower2_regen.txt")
					d.set_warp_at_eliminate(4, d.get_map_index(), special.devil_tower[2][1], special.devil_tower[2][2], "data/dungeon/deviltower3_regen.txt")
				end
			end
	    end
    end
        when kill with npc.get_race() == 8015  begin
            timer("devil_stone1_1", 8)
        end
        when devil_stone1_1.timer begin
            d.new_jump_all(66, special.devil_tower[1][1], special.devil_tower[1][2])
            d.regen_file("data/dungeon/deviltower2_regen.txt")
            d.set_warp_at_eliminate(4, d.get_map_index(), special.devil_tower[2][1], special.devil_tower[2][2], "data/dungeon/deviltower3_regen.txt")
        end
        when kill with npc.get_race() == 1091 begin
            d.set_warp_at_eliminate(4, d.get_map_index(), special.devil_tower[3][1], special.devil_tower[3][2], "data/dungeon/deviltower4_regen.txt")
            d.check_eliminated()
        end
        function get_4floor_stone_pos()
	    local positions = 
			{
				{368, 629}, {419, 630}, {428, 653}, {422, 679},
				{395, 689}, {369, 679}, {361, 658},
			}
            for i = 1, 6 do
                local j = number(i, 7)
                if i != j then
                    local t = positions[i];
                    positions[i] = positions[j];
                    positions[j] = t;
                end
            end
            return positions
        end
        when kill with pc.in_dungeon() and pc.get_map_index() >= 660000 and pc.get_map_index() < 670000 and npc.get_race() == 8016 begin
            d.setf("level", 4)
            local positions = deviltower_zone.get_4floor_stone_pos()
            for i = 1, 6 do
                d.set_unique("fake" .. i , d.spawn_mob(8017, positions[i][1], positions[i][2]))
            end
            local vid = d.spawn_mob(8017, positions[7][1], positions[7][2])
            d.set_unique("real", vid)
	    server_loop_timer('devil_stone4_update', 10, pc.get_map_index())
	    server_timer('devil_stone4_fail1', 5*60, pc.get_map_index())

            d.notice("4. Kat Seçenek Kapýsýndasýn.");
			d.notice("Gözlerini ve kulaklarýný aldatacak bir çok sahte");
			d.notice("Metin Taþý mevcut. 15 dakika içerisinde");
			d.notice("gerçek Metin Taþý'ný bul ve yok et!");
			d.notice("Kapýdan geçmenin tek yolu budur.");
        end

        when devil_stone4_fail1.server_timer begin
            if d.select(get_server_timer_arg()) then
                d.notice("10 dakika kaldý ")
		server_timer('devil_stone4_fail2', 5*60, get_server_timer_arg())
            end
        end

        when devil_stone4_fail2.server_timer begin
            if d.select(get_server_timer_arg()) then
               d.notice("5 dakika kaldý ")
		server_timer('devil_stone4_fail', 5*60, get_server_timer_arg())
            end
        end

        when devil_stone4_fail.server_timer begin
            if d.select(get_server_timer_arg()) and d.getf("level") == 4 then
                d.notice("Zaman doldu! ")
                d.exit_all()
            end
        end

        when devil_stone4_update.server_timer begin
            if d.select(get_server_timer_arg()) then
                if not d.is_unique_dead("real") then
                    for i = 1, 6 do
                        if d.getf("fakedead" .. i) == 0 then
                            if d.unique_get_hp_perc("fake" .. i) < 50 then
                                d.purge_unique("fake" .. i)
                                d.setf("fakedead" .. i, 1)
                                d.notice("Sahte Metin taþlarý yok oldu...");
                            end
                        end
                    end
                else
                    server_timer("devil_stone4_end", 5, get_server_timer_arg())
                    
                    d.notice("Mükemmel bir duyma yeteneðine ve iyi bir");
					d.notice("içgüdüye sahip olduðunu kanýtlayarak");
					d.notice("gerçek Metin Taþý'ný yok ettin!");
					d.notice("Þimdi 5.kata gireceksin.")
                    d.purge()
                end
            else
		server_timer('devil_stone4_stop_timer', 1, get_server_timer_arg())
            end
        end

        when devil_stone4_stop_timer.server_timer begin
	    clear_server_timer('devil_stone4_update', get_server_timer_arg())
        end

        when devil_stone4_end.server_timer begin
            if d.select(get_server_timer_arg()) then
			clear_server_timer('devil_stone4_update', get_server_timer_arg())
			clear_server_timer('devil_stone4_fail1', get_server_timer_arg())
			clear_server_timer('devil_stone4_fail2', get_server_timer_arg())
			clear_server_timer('devil_stone4_fail', get_server_timer_arg())

                d.setf("level", 5)
                d.setf("stone_count", 5)
                
                d.jump_all(special.devil_tower[4][1], special.devil_tower[4][2])
                d.notice("5.Kattaki Mühürlü Kapý'dasýn. Bir sonraki kata ")
				d.notice("çýkmak için beþ Eski Mühür'ü açman gerek. Bu alan ")
				d.notice("sana saldýrmak için bekleyen canavarlarla dolu.")
				d.notice("Bazýlarýnýn üzerinde anahtar var. Yeteri kadar ")
				d.notice("Kilit Açma Taþý elde eder ve tüm mühürleri ")
				d.notice("açarsan 6. kata çýkabilirsin. Ama bil ki sadece ")
				d.notice("20 dakikan var!")

                
		server_timer('devil_stone5_fail1', 5*60, get_server_timer_arg())
		clear_server_timer('devil_stone4_update', get_server_timer_arg())

                d.set_regen_file("data/dungeon/deviltower5_regen.txt")

                d.spawn_mob(20073, 421, 452)
                d.spawn_mob(20073, 380, 460)
                d.spawn_mob(20073, 428, 414)
                d.spawn_mob(20073, 398, 392)
                d.spawn_mob(20073, 359, 426)
            end
        end
        when devil_stone5_fail1.server_timer begin
            if d.select(get_server_timer_arg()) then
               d.notice("15 dakika kaldý ")
		server_timer('devil_stone5_fail2', 5*60, get_server_timer_arg())
            end
        end

        when devil_stone5_fail2.server_timer begin
            if d.select(get_server_timer_arg()) then
                d.notice("10 dakika kaldý ")
		server_timer('devil_stone5_fail3', 5*60, get_server_timer_arg())
            end
        end

        when devil_stone5_fail3.server_timer begin
            if d.select(get_server_timer_arg()) then
                d.notice("5 dakika kaldý ")
		server_timer('devil_stone5_fail', 5*60, get_server_timer_arg())
            end
        end

        when devil_stone5_fail.server_timer begin
            if d.select(get_server_timer_arg()) and d.getf("level") == 5 then
		d.notice("Zaman doldu!")
		d.exit_all()
            end
        end

		when kill with npc.get_race() == 8023 begin
			game.drop_item(50084, 1)
		end
		
        when kill with pc.in_dungeon() and d.getf("level") == 5 and npc.get_race() == 1062 begin
            local KILL_COUNT_FOR_DROP_KEY = 50
            local n =d.getf("count") + 1
            d.setf("count", n)
            if n == KILL_COUNT_FOR_DROP_KEY then
                game.drop_item(50084, 1)
                d.setf("count", 0)
            end
        end
        when devil_stone5.take with item.vnum == 50084 begin
            npc.purge()
            item.remove()
            d.setf("stone_count", d.getf("stone_count") - 1)
            if d.getf("stone_count") <= 0 then
            	d.clear_regen()
            	d.kill_all()
                d.notice("Mührü açtýn. 6. Kata çýkýyorsunuz!")

		clear_server_timer('devil_stone5_fail1', get_server_timer_arg())
		clear_server_timer('devil_stone5_fail2', get_server_timer_arg())
		clear_server_timer('devil_stone5_fail3', get_server_timer_arg())
		clear_server_timer('devil_stone5_fail', get_server_timer_arg())
		
                d.setf("level", 6)
                d.jump_all(special.devil_tower[5][1], special.devil_tower[5][2])
				d.regen_file("data/dungeon/deviltower6_regen.txt")
		
                d.notice("Þeytan Krala ulaþmak için tüm ")
                d.notice("þeytanlarý öldür!")

            else
                d.notice("Mührü açtýn! "..d.getf("stone_count").." kaldý.")
            end
        end

        when kill with npc.get_race() == 1092 begin
            d.kill_all()
            d.check_eliminated()
            local reward_alchemist = {20074, 20075, 20076}
            d.spawn_mob(reward_alchemist[number(1,3)], 425, 216);
            d.setqf("can_refine", 1)
        end

   
	when 20074.chat."Üst katlar" or 20075.chat."Üst katlar" or 20076.chat."Üst katlar" with pc.in_dungeon() and pc.get_map_index() >= 660000 and pc.get_map_index() < 670000 begin
		if not npc.lock() then
            say("")
            say("Þu anda baþka biriyle konuþuyorum!")
            say("Biraz bekle ya da sonra tekrar gel!")
            return
        end
		say_title(mob_name(npc.get_race()))
		say("Kulenin 7. katýna girmek mi istiyorsun?")
		say("Bu katla mücadele etmek muhteþem beceriler ve")
		say("büyük bir yazgý gerektirir.")
		say("Sadece 75. seviyeye ulaþtýysan")
		say("seni oraya ýþýnlayabilirim.")
		wait()
			if pc.level >=75 then
			say_title(mob_name(npc.get_race()))
			say("Üst kata çýkmak için")
			say("yeterli deneyime ve güce sahipsin.")
			say("Girebilirsin!")
			say("")
			local gec = select("Git!","Vazgeç ","Kuleyi terket!")
				if gec == 1 then
				timer("devil_jump_7",6)
				npc.unlock()
				npc.purge()
				return
				elseif gec == 2 then
					npc.unlock()
				return
				elseif gec == 3 then
				npc.unlock()
				pc.warp(590500, 110500)
				end
			else
			say_title(mob_name(npc.get_race()))
			say("Üzgünüm sadece")
			say("75. seviyeye ulaþanlarý bir")
			say("üst kata alabilirim.")
			say("")
			npc.unlock()
			wait()
			pc.warp(590500, 110500)
			return
			end
		end

   when devil_jump_7.timer begin
		d.clear_regen()

		d.spawn_mob(8018, 639, 658)
		d.spawn_mob(8018, 611, 637)
		d.spawn_mob(8018, 596, 674)
		d.spawn_mob(8018, 629, 670)

		d.setf("level", 7)

		d.jump_all(2048+590, 6656+638)
	end
	
	when kill with pc.in_dungeon() and pc.get_map_index() >= 660000 and pc.get_map_index() < 670000 and npc.get_race() == 8018 begin
		local cont = d.getf("7_stone_kill") + 1
		d.setf("7_stone_kill", cont)

		if cont >= 4 then
			d.setf("7_stone_kill", 0)
			d.set_regen_file("data/dungeon/deviltower7_regen.txt")
        end
	end

	when kill with pc.in_dungeon() and pc.get_map_index() >= 660000 and pc.get_map_index() < 670000 and npc.get_race() == 8019 begin
		game.drop_item(30300, 1)
	end

	when 30300.use with pc.in_dungeon() and pc.get_map_index() >= 660000 and pc.get_map_index() < 670000 begin
		pc.remove_item("30300", 1)

		local pct = number(1,8)

		if pct == 1 then
			game.drop_item(30302, 1)
			d.clear_regen()
		else
			game.drop_item(30301, 1)
			
		end
	end

	when 30302.use with pc.in_dungeon() and pc.get_map_index() >= 660000 and pc.get_map_index() < 670000 begin
		-- 8ÃþÀ¸·Î ¿öÇÁ

			if d.getf( "level" ) != 7 then
				pc.remove_item( "30302", 1)
				return
			end
			
		say("Bu haritayla bir sonraki kata çýkabilirim!")
		pc.remove_item("30302", 1)
	    timer("devil_jump_8", 6)
		d.clear_regen()
	end
						
	--------------------------------------------
	-- 8Ãþ ÁøÀÔºÎÅÍ 9Ãþ ±îÁö
	--------------------------------------------
	when devil_jump_8.timer begin
		d.setf("level", 8)
		d.jump_all(2048+590, 6656+403)
		d.set_regen_file("data/dungeon/deviltower8_regen.txt")
		d.spawn_mob(20366, 640, 460)
		local _count = pc.count_item(30302)
		pc.remove_item(30302,_count)
		
	end

	when kill with pc.in_dungeon() and pc.get_map_index() >= 660000 and pc.get_map_index() < 670000 and npc.get_race() == 1040 begin
		if d.getf("level") < 9 then
			local pct1 = number(1, 10)
			if pct1 == 1 then
				local pct2 = number(1, 4)
				if pct2 == 1 then
					-- ÁøÂ¥ ¿­¼è µå·Ó
					game.drop_item(30304, 1)
				else
					-- °¡Â¥ ¿­¼è µå·Ó
					game.drop_item(30303, 1)
				end
			else
				return
			end
		end
	end

	when 20366.take with item.vnum == 30304 begin
		-- ºÀÀÎ ÇØÃ¼ 9Ãþ ¤¡¤¡
		npc.purge()
		item.remove()
		timer("devil_jump_9", 8)
	end

	---------------------------------------------
	-- 9Ãþ ÁøÀÔºÎÅÍ ¿Ï·á±îÁö
	---------------------------------------------
	when devil_jump_9.timer begin
		d.setf("level", 9)
		d.jump_all(2048+590, 6656+155)
		d.regen_file("data/dungeon/deviltower9_regen.txt")
	end

	when kill with pc.in_dungeon() and pc.get_map_index() >= 660000 and pc.get_map_index() < 670000 and npc.get_race() == 1093 begin
		d.kill_all()
		timer("devil_end_jump", 30)
	end

	when devil_end_jump.timer begin
	    d.exit_all()
	end
	function register_player(vid)
		local player_count = d.getf("player_count")
		player_count = player_count + 1
		d.setf("player_count", tonumber(player_count))
		d.setf(string.format("player%d", player_count), tonumber(vid))
	end	
	function unregister_player(vid)
		local player_count = d.getf("player_count")
		local found = false
		for i = 1, player_count, 1 do
			if found == true then
				d.setf(string.format("player%d", tonumber(i)), d.getf(string.format("player%d", i+1)))
			end
			p = d.getf("player"..tostring(i))
			if p == vid then
				i = i -1
				found = true
			end
		end	
		if found == true then
			d.setf("player_count", tonumber(player_count - 1))
		end
	end	
	function get_players()
		local players = {}
		local player_count = d.getf("player_count")
		for i = 1, player_count, 1 do
			players[i] = d.getf("player"..tostring(i))
		end
		return players
	end
	function give_dc_access(pindex)
		local self_checked = false
		local self = pc.select(d.getf("player"..tostring(pindex)))
		--pc.setqf("dcenter", 1)
		pc.setf("deviltower_zone", "dcenter", 1)
		pc.select(self)
	end
end
end
