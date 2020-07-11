--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest ork_maze begin
	state start begin
		when 20403.chat."Ork Zindaný " begin
			if party.is_leader() or party.is_party() then
				say_reward("Bu zindana grup halinde giremezsin")
				say_reward("lütfen önce bulundugun grupdan ayril !")
				return
			end
			if not pc.get_channel_id() == 1 then
				say_title("Ork Zindani:")
				say("Sadece 1. kanaldan giriþ yapabilirsin.")
				return
			end
			-- sys_log(0,"ork_maze quest open")

			say_title(mob_name(20403))
			say("")
			say("Ork Zindaný yeraltýndaki en tehlikeli[ENTER]yaratýklarýn bulundugu bir yeraltý zindaný[ENTER]giriþ yapabilmek için 40 ila 70 seviye arasýnda olman gerekiyor")
			say_item(item_name(90005),90005, "")
			wait()
			say_title(mob_name(20403))
			say("")
			say("Giriþ yaptýðýnda eþya yok olacak. ")
			say("")
			say_reward("Devam etmek istiyor musun?")
			say("")
			local s = select("Evet", "Hayýr")
			-- ------------ Dungeon Turn Back System ----------------------
			-- pc.remove_turn_back_dungeon()
			-- ------------------------------------------------------------
			if s == 1 then
				if not pc.can_warp() then
					say_title(mob_name(20403))
					say("")
					say("Henüz ýþýnlanamazsýn.")
					say("")
					return
				elseif pc.count_item(90005) < 1 then
					say_title(mob_name(20403))
					say("")
					say("Envanterinde yeterli miktarda yok:")
					say(string.format("%s.", item_name(90005)))
					say("")
					return
				elseif pc.level < 40 or pc.level > 70 then
					say_title(mob_name(20403))
					say("")
					say("Seviyen giriþ gereksinimlerini karþýlamýyor. ")
					say("Gereksinimler:")
					say(string.format("Minimum Seviye: %d.",40))
					say(string.format("Maksimum Seviye: %d.",70))
					say("")
					return
				elseif get_time() < pc.getqf("ork_player_time_110") then
					say_title(mob_name(20403))
					say("")
					say("Soguma sureniz henuz dolmadi!")
					say("Kalan süre: "..dungeon_get_duration(pc.getqf("ork_player_time_110") - get_global_time()).."")
					say("")
					if (game.get_event_flag("enable_time_orkmaze") == 1) then
						say("Beta Server; süreyi sýfýrlamak mý istiyorsun?")
						if select("Evet","Hayýr") == 1 then
							pc.setqf("ork_player_time_110",0)
							return
						end
					end
					return
				elseif game.get_event_flag("enable_orcmaze_dungeon") == 1 then
					say_title(mob_name(20403))
					say("")
					say("Ork Zindaný henüz aktif durumda deðildir.")
					say("")
					return
				else
					pc.remove_item(90005, 1)
					pc.setqf("ork_player_time_110", get_time()+60*60*1)
					pc.setqf("join_orcmaze_dung", 1)
					d.new_jump(245, 12143 * 100, 7273 * 100)
				end
			end
		end

		--d.MapControl
		when login with pc.get_map_index() == 245 begin
			pc.set_warp_location(67, 2894, 66)
			d.exit()
		end
		
		when logout with pc.get_map_index() >= 2450000 and pc.get_map_index() < 2459999 begin
			------------ Dungeon Turn Back System ----------------------
			pc.setf("ork_maze","dungeon_turn",1)
			------------------------------------------------------------
		end

		--d.FloorOne
		when login with pc.get_map_index() >= 2450000 and pc.get_map_index() < 2460000 begin
			if pc.getqf("join_orcmaze_dung") > 0 then
				pc.setqf("join_orcmaze_dung", 0)
				d.setqf2("orcmaze_dungeon","last_exit_time", get_global_time())
				d.setf("orc_level", 1)
				d.regen_file("data/dungeon/orcmaze/regen_1.txt")
				server_timer('orclogintime1', 5, d.get_map_index())
				d.set_unique("door", d.spawn_mob_ac_dir(20387, 236, 229, 195))
				server_timer ("orcfloor1_eli_check_timer", 5,d.get_map_index())
			end
			--cmdchat("OpenMidgardCoolTime")
		end

		--d.FloorOneStart
		when orcfloor1_eli_check_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.getf("floor_one_finish") == 1 then
					d.jump_all (577+12032, 1048+7168)
					local per = number(1,100)
					if per <= 33 then
						d.regen_file("data/dungeon/orcmaze/regen_2_1.txt")
					elseif per >= 34 and per <= 66 then
						d.regen_file("data/dungeon/orcmaze/regen_2_2.txt")
					else
						d.regen_file("data/dungeon/orcmaze/regen_2_3.txt")
					end
					d.setf("floor_two_stone_count", 0)
					d.zodiac_notice_clear()
					d.zodiac_notice("(2. Görev) Üç Sonsuzluk taþý labirentin dört bir yanýna gönderildi. Onlarý bulup yok etmelisiniz!")
				end
			end
		end

		when kill with pc.get_map_index() >= 2450000 and pc.get_map_index() < 2460000 and npc.get_race() == 8116 begin
			local per = number(1,100)
			if d.getf("floor_one_stone_count") < 6 then
				if per <= 10 then
					d.clear_regen()
					d.kill_all()
					d.zodiac_notice_clear()
					d.zodiac_notice("(1.Görev) Doðru metin taþýný buldunuz, bir üst kata ýþýnlanýyorsunuz.")
					d.setf("floor_one_finish", 1)
					server_timer ("orcfloor1_eli_check_timer", 5,d.get_map_index())
				else
					d.zodiac_notice_clear()
					d.zodiac_notice("(1.Görev) Yanlýþ metin taþýný kestiniz!")
					d.setf("floor_one_stone_count", d.getf("floor_one_stone_count") + 1)
					pc.aggregate_monster()
				end
			else
				d.kill_all()
				d.clear_regen()
				d.regen_file("data/dungeon/orcmaze/regen_1.txt")
				d.setf("floor_one_stone_count", 0)
				d.zodiac_notice_clear()
				d.zodiac_notice("(1.Görev) Metin taþlarýnýn hepsi yok oldu ve doðru metin taþýný bulamadýnýz. Metin taþlarý ve yaratýklar yenilendi.")
			end
		end

		when orcfloor2_1_eli_check_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.getf("floor_two_finish") == 1 then
					d.jump_all (386+12032, 1393+7168)
					d.regen_file("data/dungeon/orcmaze/regen_3.txt")
					d.zodiac_notice_clear()
					d.zodiac_notice("(3. Görev) Yolun sonuna gidin ve cehennemle tanýþýn! Onu yenerseniz kilitli kapý sizin için açýlacak.")
					--server_timer('orcfloor3_eli_check_timer', 5,d.get_map_index())
				end
			end
		end

		when kill with pc.get_map_index() >= 2450000 and pc.get_map_index() < 2460000 and npc.get_race() == 692 begin
			d.clear_regen()
			d.kill_all()
			d.zodiac_notice_clear()
			d.zodiac_notice("(3. Görev) Cehennemi yendiniz. Ork anahtarýnýzý kullanarak kapýyý açýn.")
			d.set_unique("door", d.spawn_mob_ac_dir(20387, 236, 229, 195))
			d.setf("floor_three_finish", 1)
			server_timer('orcfloor3_1_eli_check_timer', 5,d.get_map_index())
		end

		when orcfloor3_1_eli_check_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.jump_all (242+12032, 200+7168)
			end
		end

		when orcfloor3_eli_check_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.getf("floor_three_finish") == 1 then
					d.regen_file("data/dungeon/orcmaze/regen_4.txt")
					d.zodiac_notice_clear()
					d.zodiac_notice("(4. Görev) Koridordan gidin ve Orc Vezirini yok edin!")
				end
			end
		end

		when kill with pc.get_map_index() >= 2450000 and pc.get_map_index() < 2460000 and npc.get_race() == 693 begin
			d.setf("floor_four_finish", 1)
			d.clear_regen()
			d.kill_all()
			d.zodiac_notice_clear()
			d.zodiac_notice("(3. Görev) Orc Vezirini yok ettiniz! Boss Odasýna ýþýnlanýyorsunuz.")
			server_timer('orcfloor4_eli_check_timer', 5,d.get_map_index())
		end

		when orcfloor4_eli_check_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.getf("floor_four_finish") == 1 then
					d.jump_all (138+12032, 934+7168)
					d.regen_file("data/dungeon/orcmaze/regen_boss.txt")
					d.zodiac_notice_clear()
					d.zodiac_notice("(4. Görev) Kara Ork Reisi dehþet saçýyor. Onu yok etmelisiniz!")
					-- server_timer('orcfloor_boss_eli_check_timer', 5,d.get_map_index())
				end
			end
		end
		
		when kill with pc.get_map_index() >= 2450000 and pc.get_map_index() < 2460000 and npc.get_race() == 694 begin
			d.setf("floor_boss_finish", 1)
			d.clear_regen()
			d.kill_all()
			d.zodiac_notice_clear()
			d.zodiac_notice("(Boss Görevi) Ork Zindanýný Yok Ettiniz! 2 Dakika Sonra Dýþarý Gönderileceksiniz.")
			timer('exit_orkmaze', 60*2)
		end

		when kill with pc.get_map_index() >= 2450000 and pc.get_map_index() < 2460000 and npc.get_race() == 8001 begin
			if d.getf("floor_two_stone_count") == 0 then
				d.zodiac_notice_clear()
				d.zodiac_notice("(2.Görev) Kesilen metin taþý: 1 - Kalan metin taþý: 2")
				d.setf("floor_two_stone_count", 1)
			elseif d.getf("floor_two_stone_count") == 1 then 
				d.zodiac_notice_clear()
				d.zodiac_notice("(2.Görev) Kesilen metin taþý: 2 - Kalan metin taþý: 1")
				d.setf("floor_two_stone_count", 2)
			else
				d.zodiac_notice_clear()
				d.zodiac_notice("(2.Görev) Metin taþlarýný yok ettiniz! Þimdi canavarlarý öldürüp ork anahtarýný alýn o sizi üst kata ýþýnlayacaktýr.")
				d.setf("floor_two_stone_count", 3)
			end
		end

		when kill with pc.get_map_index() >= 2450000 and pc.get_map_index() < 2460000 and npc.get_race() == 696 or npc.get_race() == 695 begin
			local per = number(600,650)
			if d.count_monster() <= per and d.getf("floor_two_stone_count") == 3 then
				d.zodiac_notice_clear()
				d.zodiac_notice("(2.Görev) Ork Anahtarýný elde ettiniz. Onunla bir üst kata ýþýnlanabilirsiniz.")
				d.clear_regen()
				d.kill_all()
				game.drop_item(50904,1)
				d.setf("use_lock", 1)
			end
		end

		when 50904.use with pc.get_map_index() >= 2450000 and pc.get_map_index() < 2460000 begin
			if d.getf("use_lock") == 1 then
				d.setf("use_lock", 0)
				d.setf("floor_two_finish", 1)
				d.zodiac_notice_clear()
				d.zodiac_notice("(2. Görev) Anahtar sizi 5 saniye sonra ýþýnlayacak.")
				server_timer('orcfloor2_1_eli_check_timer', 5,d.get_map_index())
			else
				syschat("Anahtarý tekrar kullanamazsýnýz. Sadece kilitli olan kapýnýn üzerine sürükleyebilirsiniz.")
			end
		end
		
		when 20387.take with pc.get_map_index() >= 2450000 and pc.get_map_index() < 2460000 and item.vnum == 50904 begin
			d.purge_unique("door")
			d.zodiac_notice_clear()
			d.zodiac_notice("(4. Görev) Kapý kilidini açtýnýz. 5 Saniye sonra Ork Veziri Gönderilecek! Bol Þans")
			server_timer('orcfloor3_eli_check_timer', 5,d.get_map_index())
		end
		--d.TimerControl
		when orclogintime1.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("(1.Görev) Açýk Olan Kapýdan Gidin ve Doðru Metin Taþýný Yok Edin! - (Zindaný tamamlamak için 45 dakika vaktiniz kaldý)")
				server_timer('orclogintime2', 10*60,d.get_map_index())
			end
		end
		when orclogintime2.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamak için 35 dakika vaktiniz kaldý.")
				server_timer('orclogintime3', 10*60,d.get_map_index())
			end
		end
		when orclogintime3.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamak için 25 dakika vaktiniz kaldý.")
				server_timer('orclogintime4', 10*60,d.get_map_index())
			end
		end
		when orclogintime4.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamak için 15 dakika vaktiniz kaldý.")
				server_timer('orclogintime5', 10*60,d.get_map_index())
			end
		end
		when orclogintime5.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamak için 5 dakika vaktiniz kaldý.")
				server_timer('orclogintime6', 4*60,d.get_map_index())
			end
		end

		when orclogintime6.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamak için 1 dakika vaktiniz kaldý.")
				server_timer('orclogintimeend', 60,d.get_map_index())
			end
		end
		when orclogintimeend.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamakta baþarýsýz oldun. 5 saniye içerisinde dýþarý ýþýnlanýyorsunuz.")
				server_timer('orctimefail', 5,d.get_map_index())
			end
		end
		when orctimefail.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.exit_all_to_start_position()
			end
		end

		when exit_orkmaze.timer begin
			d.setqf2("orcmaze_dungeon","last_exit_time", get_global_time())
			d.exit_all_to_start_position()
		end
		--d.TimerControl
	end
end