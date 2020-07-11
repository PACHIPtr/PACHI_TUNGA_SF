--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest cube_opener_list begin
	state start begin
		when 20383.chat."Arýndýrma Penceresini Aç " or 20016.chat."Zihin Taþý Penceresi Aç " or 30129.chat."Event Penceresini Aç " or 20421.chat."Kask Penceresini Aç " or 20406.chat."Kuþak Penceresini Aç " or 20413.chat."Geliþtirme Penceresini Aç " begin
			say("Senin için birþeyleri yükseltebilirim...")
			say("Eþyayý sürükle ve yeni bir eþya oluþtur.")
			command("cube open")
		end
		when 20091.chat."Güçlü Silah Üretimi " begin
			setskin(NOWINDOW)
			command("cube open")
		end
		when 20092.chat."Ekipman Üretimi " begin
			setskin(NOWINDOW)
			command("cube open")
		end
	end
end