--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest cube begin
    state start begin
        when 20018.chat."Þifalý Bitkiler Bilimi" begin
			say_title("Baek-Go:")
			say("")
			--
			say("Merhaba, nasýlsýn?")
			say("Seni güclendirecek yada iyileþtirecek")
			say("iksirler yaratmakla ilgileniyormusun?")
			say("Bir þifacý olarak kendimi")
			say("iksirlere adadým.")
			say("Birçok tarif buldum.")
			say("Onlarý denemek istermisin?")
			say("")
			wait()
			setskin(NOWINDOW)
			command("cube open")

		end


        when 20017.chat."Þifalý Bitkiler Bilimi" begin
            say_title("Yu-Hwan:")
			say("")
			---
            say("Bitki bilimi heryerde biliniyor. Ýlgilenir")
			say("misin? Sana bazý reçeteler gösterebilirim.")
            say("Bunlarla çok daha iyi iksirler yaratabilirsin.")
            wait()
            setskin(NOWINDOW)
            command("cube open")

        end


        when 20022.chat."Gizli tarif" begin
            say_title("Huahn-So:")
			say("")
			--
            say("Gizli bir aile tarifi buldum.")
            say("Bununla neredeyse zarar verilemez olacaksýn!")
            say("Hahaha")
            say("Sanýrým ülkede bunu bilen tek kiþi")
			say("benim. Bu iksirlerle neredeyse sana hiç ")
            say("birþey olmaz.")
            say("Ýksirleri denemek istermisin?")
            say("")
            wait()
            setskin(NOWINDOW)
            command("cube open")

        end
		
		when 20383.take or 20421.take or 20470.take or 60003.take or 20015.take or 20016.take or 20091.take begin
			setskin(NOWINDOW)
			command("cube open")
		end
		
	end
end
