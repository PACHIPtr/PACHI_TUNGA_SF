--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest item_change_sex begin
	state start begin
		when 71048.use begin 
			if pc.get_level() < 50 then 
				say_title("Değişim Tılsımı ")
				say_reward("Sadece 50. seviye altındakiler cinsiyet")
				say("değiştirebilirler.")
				say("Cinsiyet değiştirmek için seviyen çok düşük.")
				say("50. seviyeye ulaştığında tekrar dene.")
				say("")
				return;
			end
			
			if pc.is_engaged() then
				say_title("Değişim Tılsımı ")
				say_reward("Sadece bekar karakterler cinsiyet")
				say_reward("değiştirebilirler.")

				----"12345678901234567890123456789012345678901234567890"|
				say("Eğer evliysen, cinsiyet değiştirmeden önce")
				say("boşanmak zorundasın.")
				say("")
				return;
			end

			if pc.is_married() then
				say_title("Değişim Tılsımı ")
				say_reward("Sadece bekar karakterler cinsiyet")
				say_reward("değiştirebilirler.")

				----"12345678901234567890123456789012345678901234567890"|
				say("Eğer evliysen, cinsiyet değiştirmeden önce")
				say("boşanmak zorundasın.")
				return;
			end

			if pc.is_polymorphed() then
				say_title("Değişim Tılsımı ")
				say_reward("Dönüşmüş haldeyken tılsımı kullanamazsın.")
				say("")

				say("Dönüşmüş karakterler cinsiyet değiştiremezler.")
				say("")
				return
			end
			
			local WEAR_COSTUME_BODY_INDEX = 19
			local WEAR_COSTUME_HAIR_INDEX = 20
			if pc.get_wear(WEAR_COSTUME_BODY_INDEX) != nil then
				say_title("Değişim Tılsımı ")
				say_reward("Kostüm giyili iken tılsımı kullanamazsın.")
				say("")
				
				say("Kostüm giyen karakterler cinsiyet değiştiremezler.")
				say("")
				return
			end
			
			if pc.get_wear(WEAR_COSTUME_HAIR_INDEX) != nil then
				say_title("Değişim Tılsımı ")
				say_reward("Saç stili giyili iken tılsımı kullanamazsın.")
				say("")
				
				say("Saç stili giyen karakterler cinsiyet değiştiremezler.")
				say("")
				return
			end
			
			if get_time() < pc.getqf("next_time") then
				say_title("Değişim Tılsımı ")
				say_reward("Değişim Tılsımı 3 gün arayla kullanılabilir.")
				say("")	
				----"12345678901234567890123456789012345678901234567890"|
				say("En yakın kullanabileceğin zaman:")
				say("")

				if pc.is_gm() then
					say(string.format("    %s", time_to_str(pc.getqf("next_time"))))
					say("")
					say("GM can reset time for tesing")
					local s = select("Yes", "NO")
					if s == 1 then
						say("Gender change time has been reset")
						pc.setqf("next_time", 0)
					end
				end
				return;

			end

			say("Değişim Tılsımı ")
			say("")
			say("Bu tılsımla cinsiyet değiştirebilirsin.")
			say("Bir kere kullandığında tekrar kullanmak için 3")
			say("gün beklemek zorundasın.")
			wait()

			say("Değişim Tılsımı ")
			say("")
			say("Kullandıktan sonra değişikliklerin görülebilmesi")
			say("için oyundan çıkman gerekir.")
			say("")
			say("Şimdi karar zamanı.")
			say("Cinsiyetini değiştirmek istiyor musun?")
			say("")
			local s=select(
				"Değiştirmek istiyorum.",
				"Değiştirmek istemiyorum.")
			if 1==s then
				say("Değişim Tılsımı ")
			say("")
				say("Hayat puanın ve mana puanın aynı kalacak.")
				say("Sadece görünüşün değişecek.")
				say("")
				say_reward("Yeni cinsiyetin oyuna tekrar girdiğinde görünecek.")
				say("")

				pc.remove_item("71048",1)
				pc.change_sex()

				local m_sex = pc.get_sex()

				if m_sex == 0 then
				    char_log(0, "CHANGE_SEX", "F -> M")
				else
				    char_log(0, "CHANGE_SEX", "M -> F")
				end

			elseif 2==s then
				say("Eğer tereddütün varsa, bunu yapma.")
				say("")
			end
		end
	end
end		

