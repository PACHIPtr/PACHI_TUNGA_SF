--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest Nephrit_dungeon begin
	state start begin
		when 20093.chat."Nefrit Magarasý 2 " with game.get_event_flag("enable_Nephrit_dungeon") == 0 begin
			addimage(35,35,'nefrit.tga')
			if party.is_leader() or party.is_party() then
				say("Nefrit Maðarasý:")
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
				addimage(35,35,'nefrit.tga')
				say("Nefrit Maðarasý:")
				say("")
				say("")
				say("")
				say("")
				say("")
				say("Sadece 1. kanaldan giriþ yapabilirsin.")
				return
			end
			
			say(mob_name(20093))
			say("")
			say("")
			say("")
			say("")
			say("")
			say("Nefrit Magarasý yeraltýndaki en tehlikeli[ENTER]yaratýklarýn bulundugu bir yeraltý zindaný[ENTER]giriþ yapabilmek için 110 seviyenin üstünde olman gerekiyor")
			say_item(item_name(31337),31337, "")
			wait()
			say(mob_name(20093))
			addimage(35,35,'nefrit.tga')
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
					say(mob_name(20093))
					addimage(35,35,'nefrit.tga')
					say("")
					say("")
					say("")
					say("")
					say("")
					say("Henüz ýþýnlanamazsýn.")
					say("")
					return
				elseif pc.count_item(31337) < 1 then
					say(mob_name(20093))
					addimage(35,35,'nefrit.tga')
					say("")
					say("")
					say("")
					say("")
					say("")
					say("Envanterinde yeterli miktarda yok:")
					say(string.format("%s.", item_name(31337)))
					say("")
					return
				elseif pc.level < 30 or pc.level > 75 then
					say(mob_name(20093))
					addimage(35,35,'nefrit.tga')
					say("")
					say("")
					say("")
					say("")
					say("")
					say("Seviyen giriþ gereksinimlerini karþýlamýyor. ")
					say("Gereksinimler:")
					say(string.format("Minimum Seviye: %d.",30))
					say(string.format("Maksimum Seviye: %d.",75))
					say("")
					return
				elseif get_time() < pc.getqf("nc_player_time_110") then
					say(mob_name(20093))
					addimage(35,35,'nefrit.tga')
					say("")
					say("")
					say("")
					say("")
					say("")
					say("Soguma sureniz henuz dolmadi!")
					say("Kalan süre: "..dungeon_get_duration(pc.getqf("nc_player_time_110") - get_global_time()).."")
					say("")
					if (game.get_event_flag("enable_time_Nephrit") == 1) then
						say("Beta Server; süreyi sýfýrlamak mý istiyorsun?")
						if select("Evet","Hayýr") == 1 then
							pc.setqf("nc_player_time_110",0)
							return
						end
					end
					return
				elseif game.get_event_flag("enable_Nephrit_dungeon") == 1 then
					say(mob_name(20093))
					addimage(35,35,'nefrit.tga')
					say("")
					say("")
					say("")
					say("")
					say("")
					say("Nefrit Magarasý henüz aktif durunca deðildir.")
					say("")
					return
				else
					pc.remove_item(31337, 1)
					pc.setqf("nc_player_time_110", get_time()+60*60*2)
					pc.setqf("join_Nephrit_dung", 1)
					d.join(249)
				--	d.new_jump(249, 21144 * 100, 17115 * 100)
				end
			end
		end
		
		when 20093.chat."Maðara Süre sýfýrla " with (game.get_event_flag("Nephrit_dung_time") == 1) begin
			local select_timer = select("Sýfýrla", "Sýfýrlama")
			if select_timer == 1 then
				pc.setqf("nc_player_time_110", 0)
			end
		end
		
		-------------------------
		when login with pc.get_map_index() == 249 begin
			pc.set_warp_location(67, 2894, 66)
			d.exit()
		end
		
		when logout with pc.get_map_index() >= 2490000 and pc.get_map_index() < 2499999 begin
			------------ Dungeon Turn Back System ----------------------
			pc.setf("Nephrit","dungeon_turn",1)
			------------------------------------------------------------
		end
		
		---------------------------- Floor 1 Start ------------------------------

		when login with pc.get_map_index() >= 2490000 and pc.get_map_index() < 2499999 begin
			if pc.getqf("join_Nephrit_dung") > 0 then
				pc.setqf("join_Nephrit_dung", 0)
				d.setqf2("Nephrit_dungeon","last_exit_time",d.get_map_index())
				d.setf("level", 1)
				d.regen_file("data/dungeon/Nephrit_dungeon/regen_1_1.txt")
				server_timer('nclogintime1', 5, d.get_map_index())
				server_timer ("ncfloor1_2_eli_check_timer", 5,d.get_map_index())
			end
		--	cncchat("OpenNephritCoolTime")
			--cncchat("NephritCoolTimeSetFloor 1")
		end

		when kill with pc.get_map_index() >= 2490000 and pc.get_map_index() < 2499999 and npc.get_race() == 4972 begin
			d.setf("4972_kill",1)
		end

		when ncfloor1_2_eli_check_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() <= 5 then
					d.zodiac_notice_clear()
					d.zodiac_notice("(2.Görev) Tüm canavarlarý ve metin taþlarýný yok et ! ")
					d.notice("<Lanetli Ucube> Hahahaha piyonlarýmý yenmeyi baþardýn, ")
					d.notice("<Lanetli Ucube> Bana ulaþmak için sana gönderdiðim Kayalara karþý gücünü kanýtla ! ")
					d.notice("(Onlarý haritanýn 4 farklý bölgesinde bulabilirsin)")
					d.clear_regen()
					d.regen_file("data/dungeon/Nephrit_dungeon/regen_1_1.txt")
					d.regen_file("data/dungeon/Nephrit_dungeon/regen_1_2.txt")
					server_timer ("ncfloor1_3_eli_check_timer", 5,d.get_map_index())
				else
					server_timer ("ncfloor1_2_eli_check_timer_2", 5,d.get_map_index())
				end
			end
		end
		when ncfloor1_2_eli_check_timer_2.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() <= 5 then
					d.zodiac_notice_clear()
					d.zodiac_notice("(2.Görev) Tüm canavarlarý ve metin taþlarýný yok et ! ")
					d.notice("<Lanetli Ucube> Hahahaha piyonlarýmý yenmeyi baþardýn, ")
					d.notice("<Lanetli Ucube> Bana ulaþmak için sana gönderdiðim Kayalara karþý gücünü kanýtla ! ")
					d.notice("(Onlarý haritanýn 4 farklý bölgesinde bulabilirsin)")
					d.clear_regen()
					d.regen_file("data/dungeon/Nephrit_dungeon/regen_1_1.txt")
					d.regen_file("data/dungeon/Nephrit_dungeon/regen_1_2.txt")
					server_timer ("ncfloor1_3_eli_check_timer", 5,d.get_map_index())
				else
					server_timer ("ncfloor1_2_eli_check_timer", 5,d.get_map_index())
				end
			end
		end
		
		
		when ncfloor1_3_eli_check_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() <= 5 then
					d.zodiac_notice_clear()
					d.zodiac_notice("(3.Görev) Tüm Metinleri ve Patronlarý yok et !")
					d.notice("<Lanetli Ucube> Bana ulaþmayý kolaymý sanýyorsun ?")
					d.notice("<Lanetli Ucube> Gücünü kanýtlamak için elçilerimi yenmelisin !")
					d.notice("(Onlarý haritanýn 4 farklý bölgesinde bulabilirsin)")
					d.clear_regen()
					d.regen_file("data/dungeon/Nephrit_dungeon/regen_1_2.txt")
					server_timer ("ncfloor1_4_eli_check_timer", 5,d.get_map_index())
					d.spawn_mob(4931,613,434)
					d.spawn_mob(4931,499,388)
					d.spawn_mob(4931,412,269)
					d.spawn_mob(4931,602,644)
				else
					server_timer ("ncfloor1_3_eli_check_timer_2", 5,d.get_map_index())
				end
			end
		end
		when ncfloor1_3_eli_check_timer_2.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() <= 5 then
					d.zodiac_notice_clear()
					d.zodiac_notice("(3.Görev) Tüm Metinleri ve Patronlarý yok et !")
					d.notice("<Lanetli Ucube> Bana ulaþmayý kolaymý sanýyorsun ?")
					d.notice("<Lanetli Ucube> Gücünü kanýtlamak için elçilerimi yenmelisin !")
					d.notice("(Onlarý haritanýn 4 farklý bölgesinde bulabilirsin)")
					d.clear_regen()
					d.regen_file("data/dungeon/Nephrit_dungeon/regen_1_2.txt")
					d.spawn_mob(4931,613,434)
					d.spawn_mob(4931,499,388)
					d.spawn_mob(4931,619,649)
					d.spawn_mob(4931,602,644)
					server_timer ("ncfloor1_4_eli_check_timer", 5,d.get_map_index())
				else
					server_timer ("ncfloor1_3_eli_check_timer", 5,d.get_map_index())
				end
			end
		end
---------------------------- Kat 4 ------------------------------
		when ncfloor1_4_eli_check_timer.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() <= 5 then
					d.zodiac_notice_clear()
					d.zodiac_notice("(4.Görev) Patronla yüzleþ !")
					d.notice("<Lanetli Ucube> Demek beni istiyorsun ? ")
					d.clear_regen()
					d.spawn_mob(4972,609,629)
				else
					server_timer ("ncfloor1_4_eli_check_timer_2", 5,d.get_map_index())
				end
			end
		end

		when ncfloor1_4_eli_check_timer_2.server_timer begin
			if d.select(get_server_timer_arg()) then
				if d.count_monster() <= 5 then
					d.zodiac_notice_clear()
					d.zodiac_notice("(4.Görev) Patronla yüzleþ !")
					d.clear_regen()
					d.spawn_mob(4972,609,629)
				else
					server_timer ("ncfloor1_4_eli_check_timer", 5,d.get_map_index())
				end
			end
		end

		---------------------------- Dungeon End ------------------------------
			
		when kill with pc.get_map_index() >= 2490000 and pc.get_map_index() < 2499999 and npc.get_race() == 4972 begin
			clear_server_timer('nclogintime1',d.get_map_index())
			clear_server_timer('nclogintime2',d.get_map_index())
			clear_server_timer('nclogintime3',d.get_map_index())
			clear_server_timer('nclogintime4',d.get_map_index())
			clear_server_timer('nclogintime5',d.get_map_index())
			clear_server_timer('nclogintime6',d.get_map_index())
			clear_server_timer('nclogintimeend',d.get_map_index())
			clear_server_timer('nclogintime5',d.get_map_index())
			
			d.zodiac_notice_clear()
			d.zodiac_notice("Nefrit Magarasi baþarýlý þekilde fethedildi! 2 dakika içerisinde dýþarý gönderileceksiniz.")
			timer("exit_Nephrit", 60*2)
		end
		
		---------------------------- Floor 3 End ------------------------------
		
		----------------- Dungeon Time Control ---------------------
		when nclogintime1.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("(1.Görev) Tüm canavarlarý yok et! - (Zindaný tamamlamak için 45 dakika vaktin kaldý)")
				server_timer('nclogintime2', 10*60,d.get_map_index())
				clear_server_timer("nclogintime1",d.get_map_index())
			end
		end
		when nclogintime2.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamak için 35 dakika vaktin kaldý.")
				server_timer('nclogintime3', 10*60,d.get_map_index())
				clear_server_timer("nclogintime2",d.get_map_index())
			end
		end
		when nclogintime3.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamak için 25 dakika vaktin kaldý.")
				server_timer('nclogintime4', 10*60,d.get_map_index())
				clear_server_timer("nclogintime3",d.get_map_index())
			end
		end
		when nclogintime4.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamak için 15 dakika vaktin kaldý.")
				server_timer('nclogintime5', 10*60,d.get_map_index())
				clear_server_timer("nclogintime4",d.get_map_index())
			end
		end
		when nclogintime5.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamak için 5 dakika vaktin kaldý.")
				server_timer('nclogintime6', 4*60,d.get_map_index())
				clear_server_timer("nclogintime5",d.get_map_index())
			end
		end

		when nclogintime6.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamak için 1 dakika vaktin kaldý.")
				server_timer('nclogintimeend', 60,d.get_map_index())
				clear_server_timer("nclogintime6",d.get_map_index())
			end
		end
		when nclogintimeend.server_timer begin
			if d.select(get_server_timer_arg()) then
				d.zodiac_notice_clear()
				d.zodiac_notice("Zindaný tamamlamakta baþarýsýz oldun. 5 saniye içerisinde dýþarý ýþýnlanýyorsun.")
				clear_server_timer("nclogintimeend",d.get_map_index())
				timer("exit_Nephrit", 5)
			end
		end

		when exit_Nephrit.timer begin
			d.setqf2("Nephrit_dungeon","last_exit_time",d.get_map_index())
			d.exit_all_to_start_position()
		end
		----------------- Dungeon Time Control ---------------------
	end
end