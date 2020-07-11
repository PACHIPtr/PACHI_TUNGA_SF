--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest zodiac_milbon begin
	state start begin
		when 20470.chat."Zodyak Tapýnaðý" begin
			local s = select("Uyanýþ Prizmasý ","Tapýnak Kurallarý ","Define Sandýklarýný Bozdur ","Sýralama Listesi ","Köye Geri Dön ","Kapat ")
			if s == 1 then
				say("Henüz aktif deðil!")
			elseif s == 2 then
				say("Henüz aktif deðil!")
			elseif s == 3 then
				setskin(NOWINDOW)
				command("open_cz")
			elseif s == 4 then
				say("Henüz aktif deðil!")
			elseif s == 5 then
				pc.go_home()
			elseif s == 6 then
				return
			end
		end
	end
end
