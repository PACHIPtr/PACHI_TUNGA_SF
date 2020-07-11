--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest sash begin
	state start begin
		when 20406.chat."Bir omuz kuþaðý nedir?" with utils.is_feature_acce_enabled() == true begin
			say("Omuz kuþaðý sana iki imkan saðlar:")
			say("Kombinasyon ve emiþ.")
			say("")
			say("Kombinasyon ayný derecedeki iki kuþakla")
			say("gerçekleþtirilebilir. Kombine edilen iki kuþak")
			say("daha deðerli bir kuþak meydana getirir.")
			say("")
			say("Emiþ iþleminde bir silah veya zýrh üzerinde")
			say("bulunan bonuslar")
			say("çeþitli oranlarla omuz kuþaðýna aktarýlýr.")
			say("")
			say("Emiþ oraný (%) kuþaðýn derecesine baðlýdýr. Bonus")
			say("iþlemi için seçilen silah ve zýrh geri")
			say("döndürülemez þekilde yok edilir.")
		end
		
		when 20406.chat."Kombinasyon" with utils.is_feature_acce_enabled() == true begin
			say("Ýki kuþaðý kombine mi etmek istiyorsun?")
			say("")
			local confirm = select("Evet", "Hayýr")
			if confirm == 2 then
				return
			end
			
			setskin(NOWINDOW)
			pc.open_sash(true)
		end
		
		when 20406.chat."Bonus Emiþi" with utils.is_feature_acce_enabled() == true begin
			say("Silah veya zýrhýndan bonus mu emmek istersin?")
			say("")
			local confirm = select("Evet", "Hayýr")
			if confirm == 2 then
				return
			end
			
			setskin(NOWINDOW)
			pc.open_sash(false)
		end
	end
end