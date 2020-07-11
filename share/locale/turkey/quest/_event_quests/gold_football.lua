--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest gold_football begin
	state start begin
		when 20017.chat."Altýn Futbol Topu" with pc.getqf("first_seen") == 0 begin
			say_title("Yu-Hwan")
			say("")
			say("20 Adet Futbol Topu karþýlýðý sana 1 adet")
			say("Altýn Futbol Topu vereceðim.")
			say("Altýn Futbol Topu içerisinden seni güçlendirecek")
			say("iksirler ve inciler elde edebileceksin.")
			say_reward("Altýn Futbol Topu istiyorsan burada olacaðým.")
			pc.setqf("first_seen",1)
		end
		when 20017.chat."Altýn Futbol Topu Dönüþtür" with pc.getqf("first_seen") == 1 begin
			say_title("Yu-Hwan")
			say("")
			say_reward("Devam etmek istiyor musun?")
			local s = select("Evet, istiyorum.","Hayýr, istemiyorum.")
			if s == 2 then
				return
			else
				if pc.is_busy() == true then
					say_title("Yu-Hwan")
					say("")
					say_reward("Üzgünümki meþgul görünüyorsun.")
				elseif pc.is_dead() == true then
					say_title("Yu-Hwan")
					say("")
					say_reward("Ölü bir adamdan futbol topu alamam herhalde.")
				elseif pc.count_item(50096) < 20 then
					say_title("Yu-Hwan")
					say("")
					say_reward("Envanterinde yeterli miktarda bulamadým.")
				else
					pc.remove_item(50096, 20)
					pc.give_item2(50265, 1)
				end
			end
		end
	end
end