--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest login_info begin
	state start begin
		when login begin
			syschat("Kanal-"..pc.get_channel_id().." ile baðlýsýn. ")
			-- Events
			if game.get_event_flag("ex_moon_drop") > 0 then
				syschat("<Etkinlik Duyurusu> Ayýþýnðý define sandýðý etkinliði þuanda aktif.")
			end
			if game.get_event_flag("ex_hexagon_drop") > 0 then
				syschat("<Etkinlik Duyurusu> Altýgen hediye paketi etkinliði þuanda aktif.")
			end
			if game.get_event_flag("ex_carving_stone_drop") > 0 then
				syschat("<Etkinlik Duyurusu> Oyma taþý etkinliði þuanda aktif.")
			end
			if game.get_event_flag("ex_ramadan_drop") > 0 then
				syschat("<Etkinlik Duyurusu> Ramazan etkinliði þuanda aktif.")
			end
			if game.get_event_flag("ex_xmas_drop") > 0 then
				syschat("<Etkinlik Duyurusu> Noel etkinliði þuanda aktif.")
			end
			if game.get_event_flag("enable_easter_event") > 0 then
				syschat("<Etkinlik Duyurusu> Paskalya etkinliði þuanda aktif.")
			end
			if game.get_event_flag("ex_football_drop") > 0 then
				syschat("<Etkinlik Duyurusu> Futbol topu etkinliði þuanda aktif.")
			end
			if game.get_event_flag("ex_valentine_drop") > 0 then
				syschat("<Etkinlik Duyurusu> Sevgililer günü etkinliði þuanda aktif.")
			end
			if game.get_event_flag("ex_pet_drop") > 0 then
				syschat("<Etkinlik Duyurusu> Evcil hayvan etkinliði þuanda aktif.")
			end
			if game.get_event_flag("ex_kids_drop") > 0 then
				syschat("<Etkinlik Duyurusu> Bulmaca kutusu etkinliði þuanda aktif.")
			end
			if game.get_event_flag("ex_halloween_drop") > 0 then
				syschat("<Etkinlik Duyurusu> Cadýlar bayramý etkinliði þuanda aktif.")
			end
			if game.get_event_flag("ex_olympic_drop") > 0 then
				syschat("<Etkinlik Duyurusu> Olimpiyat etkinliði þuanda aktif.")
			end
			if game.get_event_flag("cards_event") > 0 then
				syschat("<Etkinlik Duyurusu> Okey kart etkinliði þuanda aktif.")
			end
			if game.get_event_flag("ex_mount_drop") > 0 then
				syschat("<Etkinlik Duyurusu> Binek parþomeni etkinliði þuanda aktif.")
			end
			if game.get_event_flag("alignment_event") > 0 then
				syschat("<Etkinlik Duyurusu> Derece etkinliði þuanda aktif.")
			end
			if game.get_event_flag("2017_year_round_event") > 0 then
				syschat("<Etkinlik Duyurusu> Yýldönümü etkinliði þuanda aktif.")
			end
			if game.get_event_flag("enable_attendance_event") > 0 then
				syschat("<Etkinlik Duyurusu> Patron avcýlarý etkinliði þuanda aktif.")
			end
			if game.get_event_flag("2018_fish_event") > 0 then
				syschat("<Etkinlik Duyurusu> Balýk yapboz etkinliði þuanda aktif.")
			end
			if game.get_event_flag("mining_event") > 0 then
				syschat("<Etkinlik Duyurusu> Madencilik etkinliði þuanda aktif.")
			end
			if game.get_event_flag("harvest_festival") > 0 then
				syschat("<Etkinlik Duyurusu> Yaðmacýlarý avla etkinliði þuanda aktif.")
			end
			if game.get_event_flag("newyear_wonso") > 0 then
				syschat("<Etkinlik Duyurusu> Tatlý yapým etkinliði þuanda aktif.")
			end
			if game.get_event_flag("enable_catch_king_event") > 0 then
				syschat("<Etkinlik Duyurusu> Kralý yakala etkinliði þuanda aktif.")
			end
			if game.get_event_flag("golden_rain_event") > 0 then
				syschat("<Etkinlik Duyurusu> Yang yaðmuru etkinliði þuanda aktif.")
			end
			if game.get_event_flag("super_metin_event") > 0 then
				syschat("<Etkinlik Duyurusu> Süper metin etkinliði þuanda aktif.")
			end
		end
	end
end
