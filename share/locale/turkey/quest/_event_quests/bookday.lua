--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest bookday begin
	state start begin
		when login with game.get_event_flag("worldbookday") == 1 begin
			send_letter("*Dünya Kitap Günü!")
			local v=find_npc_by_vnum(20094)
			if v!=0 then
				target.vid("__TARGETWBD__", v, "HongHae")
		    	end
		end
		
		when button or info begin
			----"123456789012345678901234567890123456789012345678901234567890"|
			say("Sevgili oyuncular, bu gün dünya kitap günü.")
			say("Ve elinizdeki artmýþ beceri kitaplarýnýzý ")
			say("Hong-Hae'ye götürerek takas edebileceksiniz.")
			say("O size rastgele bir beceri kýlavuzu verecek.")
			say("")
			say_reward("Dünya Kitap Gününüz kutlu olsun!")
			say("")
		end
		
		when 20094.take with item.vnum == 50300 and game.get_event_flag("worldbookday") == 1 begin
			say_title("Hong-Hae")
			say("")
			say("Bu kitabý takas etmek istediðine eminmisin ?")
			say("Sadece rastgele bir beceri kýlavuzu vereceðim.")
			say("")
			local answ = select("Evet", "Hayýr")
			if answ == 1 then
				local num = number(1,44)
				local tomes = {50401,50402,50403,50404,50405,50416,50417,50418,50419,50420,50431,50432,50433,50434,50435,50446,50447,50448,50449,50450,50461,50462,50463,50464,50465,50466,50476,50477,50478,50479,50480,50481,50496,50495,50494,50493,50492,50491,50506,50507,50508,50509,50510,50511}
				item.remove()
				pc.give_item2(tonumber(tomes[num]),1)
			end
		end
		
		when 20094.click with game.get_event_flag("worldbookday") == 1 begin
			target.delete("__TARGETWBD__")
			say_title("Hong-Hae")
			say("")
			say("Dünya Kitap Günün Kutlu Olsun!")
			say("")
			say("Eðer bana bir beceri kitabý verirsen")
			say("Sana rastgele bir beceri kýlavuzu vereceðim.")
			say("")
		end
	end
end	