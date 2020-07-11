--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest gm_info begin
	state start begin
		when login with pc.is_gm() begin
			send_letter("*GM: Bilgi")
			q.set_icon("scroll_open_blue.tga")
			-- local number1 = number(1,10)
			-- local number2 = number(20,25)
			-- cmdchat(string.format("Refresh12ZiTimer %s %s %s %s", number1, number2, 3600, 1200))
		end
        when info or button with pc.is_gm() begin 
			say_title("GM: Bilgi")
			say("Harita Kodu: "..pc.get_map_index().."")
			say("Maksimum Can: "..pc.get_max_hp().." / Mevcut Can: "..pc.get_hp().."")
			say("Maksimum Mana: "..pc.get_max_sp().." / Mevcut Mana: "..pc.get_sp().."")
			say("Seviye: "..pc.get_level().." / Mevcut Deneyim: "..pc.get_exp().."")
        end 
	end
end
