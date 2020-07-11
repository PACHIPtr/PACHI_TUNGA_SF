--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest midgard_dungeon begin
	state start begin
		when 20505.chat."Ruhlar Magarasý " with game.get_event_flag("enable_midgard_dungeon") == 0 begin
			addimage(35,35,'ruhlar.tga')
			if party.is_leader() or party.is_party() then
				say_reward("Bu zindana grup halinde giremezsin")
				say_reward("lütfen önce bulundugun grupdan ayril !")
				return
			end
			if (pc.get_channel_id() == 2 or pc.get_channel_id() == 3 or pc.get_channel_id() == 4 or pc.get_channel_id() == 5 or pc.get_channel_id() == 6) then
				say("Ruhlar Maðarasý:")
				addimage(35,35,'ruhlar.tga')
				say("")
				say("")
				say("")
				say("")
				say("")
				say("Sadece 1. kanaldan giriþ yapabilirsin.")
				return
			end
			-- sys_log(0,"midgard_dungeon quest open")
			
			say(mob_name(20505))
			addimage(35,35,'ruhlar.tga')
			say("")
			say("")
			say("")
			say("")
			say("")
			say("Ruhlar Magarasý yeraltýndaki en tehlikeli[ENTER]yaratýklarýn bulundugu bir yeraltý zindaný[ENTER]giriþ yapabilmek için 110 seviyenin üstünde olman gerekiyor")
			say_item(item_name(31324),31324, "")
			wait()
			say(mob_name(20505))
			addimage(35,35,'ruhlar.tga')
			say("")
			say("")
			say("")
			say("")
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
					say(mob_name(20505))
					addimage(35,35,'ruhlar.tga')
					say("")
					say("")
					say("")
					say("")
					say("")
					say("Henüz ýþýnlanamazsýn.")
					say("")
					return
				elseif pc.count_item(31324) < 1 then
					say(mob_name(20505))
					addimage(35,35,'ruhlar.tga')
					say("")
					say("")
					say("")
					say("")
					say("")
					say("Envanterinde yeterli miktarda yok:")
					say(string.format("%s.", item_name(31324)))
					say("")
					return
				elseif pc.level < 110 or pc.level > 120 then
					say(mob_name(20505))
					addimage(35,35,'ruhlar.tga')
					say("")
					say("")
					say("")
					say("")
					say("")
					say("Seviyen giriþ gereksinimlerini karþýlamýyor. ")
					say("Gereksinimler:")
					say(string.format("Minimum Seviye: %d.",110))
					say(string.format("Maksimum Seviye: %d.",120))
					say("")
					return
				elseif get_time() < pc.getqf("md_player_time_110") then
					say(mob_name(20505))
					addimage(35,35,'ruhlar.tga')
					say("")
					say("")
					say("")
					say("")
					say("")
					say("Soguma sureniz henuz dolmadi!")
					say("Kalan süre: "..dungeon_get_duration(pc.getqf("md_player_time_110") - get_global_time()).."")
					say("")
					if (game.get_event_flag("enable_time_midgard") == 1) then
						say("Beta Server; süreyi sýfýrlamak mý istiyorsun?")
						if select("Evet","Hayýr") == 1 then
							pc.setqf("md_player_time_110",0)
							return
						end
					end
					return
				elseif game.get_event_flag("enable_midgard_dungeon") == 1 then
					say(mob_name(20505))
					addimage(35,35,'ruhlar.tga')
					say("")
					say("")
					say("")
					say("")
					say("")
					say("Ruhlar Magarasý henüz aktif durumda deðildir.")
					say("")
					return
				else
					pc.remove_item(31324, 1)
					pc.setqf("md_player_time_110", get_time()+60*60*2)
					pc.setqf("join_midgard_dung", 1)
					d.new_jump(507, 21144 * 100, 17115 * 100)
				end
			end
		end
		
		when 20505.chat."Maðara Süre sýfýrla " with (game.get_event_flag("midgard_dung_time") == 1) begin
			local select_timer = select("Sýfýrla", "Sýfýrlama")
			if select_timer == 1 then
				pc.setqf("md_player_time_110", 0)
			end
		end
		
		-------------------------
		when login with pc.get_map_index() == 507 begin
			pc.set_warp_location(67, 2894, 66)
			d.exit()
		end
		
		when logout with pc.get_map_index() >= 5070000 and pc.get_map_index() < 5079999 begin
			------------ Dungeon Turn Back System ----------------------
			pc.setf("midgard","dungeon_turn",1)
			------------------------------------------------------------
		end
		
		---------------------------- Floor 1 Start ------------------------------

		when login with pc.get_map_index() >= 5070000 and pc.get_map_index() < 5079999 begin
			if pc.getqf("join_midgard_dung") > 0 then
				pc.setqf("join_midgard_dung", 0)
				d.setqf2("midgard_dungeon","last_exit_time",d.get_map_index())
				d.setf("level", 1)
				d.regen_file("data/dungeon/midgard_dungeon/regen_1_1.txt")
				server_timer('mdlogintime1', 5, d.get_map_index())
				server_timer ("mdfloor1_2_eli_check_timer", 5,d.get_map_index())
			end
		--	cmdchat("OpenMidgardCoolTime")
			--cmdchat("MidgardCoolTimeSetFloor 1")
		end

		when kill with pc.get_map_index() >= 5070000 and pc.get_map_index() < 5079999 and npc.get_race() == 4900 begin
			d.setf("4900_kill",1)
		end

		when mdfloor1_2_eli_check_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() <= 5 then
					d.zodiac_notice_clear()
					d.zodiac_notice("(2.Görev) Tüm canavarlarý ve metin taþlarýný yok et !")
					--cmdchat("MidgardCoolTimeSetFloor 2")
					d.jump_all (143+21000, 115+17000)
					d.clear_regen()
					d.regen_file("data/dungeon/midgard_dungeon/regen_1_1.txt")
					d.regen_file("data/dungeon/midgard_dungeon/regen_1_2.txt")
					server_timer ("mdfloor1_3_eli_check_timer", 5,d.get_map_index())
					--clear_server_timer("mdfloor1_2_eli_check_timer",d.get_map_index())
					--clear_server_timer("mdfloor1_2_eli_check_timer_2",d.get_map_index())
				else
					server_timer ("mdfloor1_2_eli_check_timer_2", 5,d.get_map_index())
				end
			end
		end
		when mdfloor1_2_eli_check_timer_2.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() <= 5 then
					d.zodiac_notice_clear()
					d.zodiac_notice("(2.Görev) Tüm canavarlarý ve metin taþlarýný yok et !")
					--cmdchat("MidgardCoolTimeSetFloor 2")
					d.jump_all (143+21000, 115+17000)
					d.clear_regen()
					d.regen_file("data/dungeon/midgard_dungeon/regen_1_1.txt")
					d.regen_file("data/dungeon/midgard_dungeon/regen_1_2.txt")
					server_timer ("mdfloor1_3_eli_check_timer", 5,d.get_map_index())
					--clear_server_timer("mdfloor1_2_eli_check_timer",d.get_map_index())
					--clear_server_timer("mdfloor1_2_eli_check_timer_2",d.get_map_index())
				else
					server_timer ("mdfloor1_2_eli_check_timer", 5,d.get_map_index())
				end
			end
		end
		
		
		when mdfloor1_3_eli_check_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() <= 5 then
					d.zodiac_notice_clear()
					d.zodiac_notice("(3.Görev) Tüm canavarlarý ve patron'u yok et !")
					--cmdchat("MidgardCoolTimeSetFloor 3")
					d.jump_all (143+21000, 115+17000)
					d.clear_regen()
					d.regen_file("data/dungeon/midgard_dungeon/regen_1_1.txt")
					server_timer ("mdfloor2_eli_check_timer", 5,d.get_map_index())
					d.spawn_mob(4900,359,276)
					--clear_server_timer("mdfloor1_3_eli_check_timer",d.get_map_index())
					--clear_server_timer("mdfloor1_3_eli_check_timer_2",d.get_map_index())
				else
					server_timer ("mdfloor1_3_eli_check_timer_2", 5,d.get_map_index())
				end
			end
		end
		when mdfloor1_3_eli_check_timer_2.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() <= 5 then
					d.zodiac_notice_clear()
					d.zodiac_notice("(3.Görev) Tüm canavarlarý ve patron'u yok et !")
					--cmdchat("MidgardCoolTimeSetFloor 3")
					d.jump_all (143+21000, 115+17000)
					d.clear_regen()
					d.regen_file("data/dungeon/midgard_dungeon/regen_1_1.txt")
					d.spawn_mob(4900,359,276)
					server_timer ("mdfloor2_eli_check_timer", 5,d.get_map_index())
					--clear_server_timer("mdfloor1_3_eli_check_timer",d.get_map_index())
					--clear_server_timer("mdfloor1_3_eli_check_timer_2",d.get_map_index())
				else
					server_timer ("mdfloor1_3_eli_check_timer", 5,d.get_map_index())
				end
			end
		end
		
		when mdfloor1_4_eli_check_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() <= 5 and d.getf("4900_kill") == 1 then
					d.zodiac_notice_clear()
					d.zodiac_notice("(4.Görev) Tüm canavarlarý yok et !")
					--cmdchat("MidgardCoolTimeSetFloor 4")
					d.clear_regen()
					d.jump_all (737+21000, 164+17000)
					d.regen_file("data/dungeon/midgard_dungeon/regen_2_1.txt")
					server_timer ("mdfloor2_eli_check_timer", 5,d.get_map_index())
					--clear_server_timer("mdfloor1_4_eli_check_timer",d.get_map_index())
					--clear_server_timer("mdfloor1_4_eli_check_timer_2",d.get_map_index())
				else
					server_timer ("mdfloor1_4_eli_check_timer_2", 5,d.get_map_index())
				end
			end
		end
		
		when mdfloor1_4_eli_check_timer_2.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() <= 5  and d.getf("4900_kill") == 1 then
					d.zodiac_notice_clear()
					d.zodiac_notice("(4.Görev) Tüm canavarlarý yok et !")
					--cmdchat("MidgardCoolTimeSetFloor 4")
					d.clear_regen()
					d.jump_all (737+21000, 164+17000)
					d.regen_file("data/dungeon/midgard_dungeon/regen_2_1.txt")
					server_timer ("mdfloor2_eli_check_timer", 5,d.get_map_index())
					--clear_server_timer("mdfloor1_4_eli_check_timer",d.get_map_index())
					--clear_server_timer("mdfloor1_4_eli_check_timer_2",d.get_map_index())
				else
					server_timer ("mdfloor1_4_eli_check_timer", 5,d.get_map_index())
				end
			end
		end
		---------------------------- Floor 1 End ------------------------------
		
		
		
		---------------------------- Floor 2 Start ------------------------------
		when mdfloor2_eli_check_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() <= 5 then
					d.setf("lock_stone_new", 4)
					d.clear_regen()
					d.jump_all (737+21000, 164+17000)
					d.regen_file("data/dungeon/midgard_dungeon/regen_2_2.txt")
					d.spawn_mob(20404,662,474)
					d.spawn_mob(20404,846,492)
					d.spawn_mob(20404,853,377)
					d.spawn_mob(20404,851,220)
					d.setf("md2_level",2)
					d.zodiac_notice_clear()
					d.zodiac_notice("(4.Görev) Anahtar düþürerek tüm sandýklarý yok et! - Kalan Sandýk : "..d.getf("lock_stone_new").."")
					--cmdchat("MidgardCoolTimeSetFloor 4")
					--server_loop_timer('dm_104_server_timer', 60*3, pc.get_map_index())
					--clear_server_timer("mdfloor2_eli_check_timer",d.get_map_index())
					--clear_server_timer("mdfloor2_eli_check_timer_2",d.get_map_index())
				else
					server_timer ("mdfloor2_eli_check_timer_2", 5,d.get_map_index())
				end
			end
		end
		when mdfloor2_eli_check_timer_2.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() <= 5 then
					d.setf("lock_stone_new", 4)
					d.clear_regen()
					d.jump_all (737+21000, 164+17000)
					d.regen_file("data/dungeon/midgard_dungeon/regen_2_2.txt")
					d.spawn_mob(20404,662,474)
					d.spawn_mob(20404,846,492)
					d.spawn_mob(20404,853,377)
					d.spawn_mob(20404,851,220)
					d.setf("md2_level",2)
					d.zodiac_notice_clear()
					d.zodiac_notice("(4.Görev) Anahtar düþürerek tüm sandýklarý yok et! - Kalan Sandýk : "..d.getf("lock_stone_new").."")
					--cmdchat("MidgardCoolTimeSetFloor 4")
					--clear_server_timer("mdfloor2_eli_check_timer",d.get_map_index())
					--clear_server_timer("mdfloor2_eli_check_timer_2",d.get_map_index())
				else
					server_timer ("mdfloor2_eli_check_timer", 5,d.get_map_index())
				end
			end
		end


		when kill with pc.get_map_index() >= 5070000 and pc.get_map_index() < 5079999 and npc.get_race() == 4904 begin
			local killset = d.getf("kill_104") + 1
			d.setf("kill_104", killset)
			if d.getf("kill_104") >= 70 then
				game.drop_item(30368,1)
				d.setf("kill_104", 0)
			end
		end

		when 20404.take with pc.get_map_index() >= 5070000 and pc.get_map_index() < 5079999 and item.vnum == 30368 begin
			d.setf("lock_stone_new", d.getf("lock_stone_new")-1)
			if d.getf("lock_stone_new") <= 0 then
				d.clear_regen()
				d.kill_all()
				d.clear_regen()
				d.jump_all (846+21000, 851+17000)
				d.setf("md2_level",3)
				d.setf("kill_1192",4)
				d.regen_file("data/dungeon/midgard_dungeon/regen_3_1.txt")
				d.spawn_mob(4910,750,893)
				d.spawn_mob(4910,676,829)
				d.spawn_mob(4910,675,889)
				d.spawn_mob(4910,758,828)
				d.zodiac_notice_clear()
				d.zodiac_notice("(5.Görev) Tüm patronlarý ve yaratýklarý yok et! - Kalan Patron : "..d.getf("kill_1192").."")
				d.setf("lock_stone_new", 0)
				return
			end
			if d.getf("lock_stone_new") == 2 then 
				d.setf("kill_104", 0)
				d.regen_file("data/dungeon/midgard_dungeon/regen_2_2.txt")
			end
			npc.purge()
			item.remove()
			d.zodiac_notice_clear()
			d.zodiac_notice("(4.Görev) Anahtar düþürerek tüm sandýklarý yok et! - Kalan Sandýk : "..d.getf("lock_stone_new").."")
			if d.getf("lock_stone_new") <= 0 then
				d.clear_regen()
				d.jump_all (846+21000, 851+17000)
				d.setf("md2_level",3)
				d.setf("kill_1192",4)
				d.regen_file("data/dungeon/midgard_dungeon/regen_3_1.txt")
				d.spawn_mob(4910,750,893)
				d.spawn_mob(4910,676,829)
				d.spawn_mob(4910,675,889)
				d.spawn_mob(4910,758,828)
				d.zodiac_notice_clear()
				d.zodiac_notice("(5.Görev) Tüm patronlarý ve yaratýklarý yok et! - Kalan Patron : "..d.getf("kill_1192").."")
				d.setf("lock_stone_new", 0)
				return
			end
			--cmdchat("MidgardCoolTimeSetFloor 4")
		end

		---------------------------- Floor 2 End ------------------------------
		
		when kill with pc.get_map_index() >= 5070000 and pc.get_map_index() < 5079999 and npc.get_race() == 4910 begin
			d.setf("kill_1192", d.getf("kill_1192")-1)
			if d.getf("kill_1192") > 0 then
				d.zodiac_notice_clear()
				d.zodiac_notice("(5.Görev) Tüm patronlarý ve yaratýklarý yok et! - Kalan Patron : "..d.getf("kill_1192").."")
				--cmdchat("MidgardCoolTimeSetFloor 5")
				pc.aggregate_monster()
			end
			
			if d.getf("kill_1192") <= 0 then
				d.kill_all()
				d.clear_regen()
				d.jump_all (383+21000, 815+17000)
				d.setf("md2_level",5)
				d.zodiac_notice_clear()
				d.zodiac_notice("(6.Görev) Doðru patronu bulana kadar kesmeye devam et!")
				--cmdchat("MidgardCoolTimeSetFloor 6")
				d.regen_file("data/dungeon/midgard_dungeon/regen_4_1.txt")
				d.spawn_mob(4911,298,877)
			end
		end
		
		---------------------------- Floor 3 Start ------------------------------
		
		when kill with pc.get_map_index() >= 5070000 and pc.get_map_index() < 5079999 and npc.get_race() == 4911 begin
			local per = number(1,100)
			if per <= 25 then
				d.clear_regen()
				d.kill_all()
				d.jump_all (383+21000, 815+17000)
				d.zodiac_notice_clear()
				d.zodiac_notice("(7.Görev) Metin taþlarýný keserek anahtar düþür ve anýtý parçala!")
				--cmdchat("MidgardCoolTimeSetFloor 7")
				d.setf("md2_level",7)
				d.spawn_mob(30151,295,886)
				d.spawn_mob(8163,297,903)
				d.spawn_mob(8163,347,872)
				d.spawn_mob(8163,301,861)
				d.spawn_mob(8163,334,905)
				d.setf("stone_count",4)
				--clear_server_timer("mdfloor5_1_eli_check_timer",d.get_map_index())
			else
				d.clear_regen()
				d.kill_all()
				d.zodiac_notice_clear()
				d.regen_file("data/dungeon/midgard_dungeon/regen_4_1.txt")
				d.zodiac_notice("(6.Görev) Yanlýþ patronu buldun tekrar kes!")
				d.spawn_mob(4911,298,877)
			end
		end
		
		
		when kill with pc.get_map_index() >= 5070000 and pc.get_map_index() < 5079999 and npc.get_race() == 8163 begin
			d.setf("stone_count", d.getf("stone_count")-1)
			if d.getf("monument_30151") == 0 then
				game.drop_item(30368,1)
			end
		end

		
		when 30151.take with pc.get_map_index() >= 5070000 and pc.get_map_index() < 5079999 and item.vnum == 30368 begin
			item.remove()
			if d.getf("monument_30151") == 1 then return end
			local per = number(1,100)
			if per <= 85 then
				if d.getf("stone_count") == 0 then
					d.spawn_mob(8163,297,903)
					d.spawn_mob(8163,347,872)
					d.spawn_mob(8163,301,861)
					d.spawn_mob(8163,334,905)
					d.setf("stone_count",4)
				end
				d.zodiac_notice_clear()
				d.zodiac_notice("(7.Görev) Mühür anýtý açarken parçalara ayrýldý, tekrar dene!")
			else
				npc.purge()
				d.clear_regen()
				d.kill_all()
				d.zodiac_notice_clear()
				d.zodiac_notice("(8.Görev) Patronu yok et ve haritayý tamamla!")
				--cmdchat("MidgardCoolTimeSetFloor 8")
				d.setf("monument_30151",1)
				d.spawn_mob(4912,319,890)
			end
		end
			
		when kill with pc.get_map_index() >= 5070000 and pc.get_map_index() < 5079999 and npc.get_race() == 4912 begin
			clear_server_timer('mdlogintime1',d.get_map_index())
			clear_server_timer('mdlogintime2',d.get_map_index())
			clear_server_timer('mdlogintime3',d.get_map_index())
			clear_server_timer('mdlogintime4',d.get_map_index())
			clear_server_timer('mdlogintime5',d.get_map_index())
			clear_server_timer('mdlogintime6',d.get_map_index())
			clear_server_timer('mdlogintimeend',d.get_map_index())
			clear_server_timer('mdtimefail',d.get_map_index())
			clear_server_timer('mdlogintime5',d.get_map_index())
			
			d.zodiac_notice_clear()
			d.zodiac_notice("Ruhlar Magarasi baþarýlý þekilde fethedildi! 2 dakika içerisinde dýþarý gönderileceksiniz.")
			timer("exit_midgard", 60*2)
		end
		
		---------------------------- Floor 3 End ------------------------------
		
		----------------- Dungeon Time Control ---------------------
		when mdlogintime1.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("(1.Görev) Tüm canavarlarý yok et! - (Zindaný tamamlamak için 45 dakika vaktin kaldý)")
				server_timer('mdlogintime2', 10*60,d.get_map_index())
				clear_server_timer("mdlogintime1",d.get_map_index())
			end
		end
		when mdlogintime2.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamak için 35 dakika vaktin kaldý.")
				server_timer('mdlogintime3', 10*60,d.get_map_index())
				clear_server_timer("mdlogintime2",d.get_map_index())
			end
		end
		when mdlogintime3.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamak için 25 dakika vaktin kaldý.")
				server_timer('mdlogintime4', 10*60,d.get_map_index())
				clear_server_timer("mdlogintime3",d.get_map_index())
			end
		end
		when mdlogintime4.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamak için 15 dakika vaktin kaldý.")
				server_timer('mdlogintime5', 10*60,d.get_map_index())
				clear_server_timer("mdlogintime4",d.get_map_index())
			end
		end
		when mdlogintime5.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamak için 5 dakika vaktin kaldý.")
				server_timer('mdlogintime6', 4*60,d.get_map_index())
				clear_server_timer("mdlogintime5",d.get_map_index())
			end
		end

		when mdlogintime6.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamak için 1 dakika vaktin kaldý.")
				server_timer('mdlogintimeend', 60,d.get_map_index())
				clear_server_timer("mdlogintime6",d.get_map_index())
			end
		end
		when mdlogintimeend.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamakta baþarýsýz oldun. 5 saniye içerisinde dýþarý ýþýnlanýyorsun.")
				server_timer('mdtimefail', 5,d.get_map_index())
				clear_server_timer("mdlogintimeend",d.get_map_index())
			end
		end
		when mdtimefail.server_timer begin
			clear_server_timer('mdlogintime1',d.get_map_index())
			clear_server_timer('mdlogintime2',d.get_map_index())
			clear_server_timer('mdlogintime3',d.get_map_index())
			clear_server_timer('mdlogintime4',d.get_map_index())
			clear_server_timer('mdlogintime5',d.get_map_index())
			clear_server_timer('mdlogintime6',d.get_map_index())
			clear_server_timer('mdlogintimeend',d.get_map_index())
			clear_server_timer('mdtimefail',d.get_map_index())
			clear_server_timer('mdlogintime5',d.get_map_index())
			if d.select(get_server_timer_arg()) then
				d.exit_all_to_start_position()
			end
		end

		when exit_midgard.timer begin
			d.setqf2("midgard_dungeon","last_exit_time",d.get_map_index())
			d.exit_all_to_start_position()
		end
		----------------- Dungeon Time Control ---------------------
	end
end