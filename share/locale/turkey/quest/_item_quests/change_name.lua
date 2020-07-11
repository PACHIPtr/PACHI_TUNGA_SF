--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest change_name begin
	state start begin
		when 71055.use begin
		say_title("Ýsim izi ")
			if pc.is_married() then
				say("Evli olduðun için isim deðiþtiremezsin.")
				say("")
				return
			end

			if pc.is_polymorphed() then
				say("Dönüþmüþ halde isim deðiþtiremezsin.")
				say("")
				return
			end

			if pc.has_guild() then
				say("Bir loncanýn üyesiyken isim deðiþtiremezsin.")
				say("")
				return
			end

			if party.is_party() then
				say("Ýsim deðiþtirmek için gruptan çýkmalýsýn.")
				say("")
				return
			end

			if pc.get_level() < 50 then
				say("Ýsim deðiþtirmek için yeterli seviyede deðilsin.")
				say("50. seviyeye ulaþtýðýnda tekrar dene.")
				return
			end

			if get_time() < pc.getqf("next_time") then
				say("Henüz isim deðiþtiremezsin.")
				say("")
				say_reward("Tekrar isim deðiþtirebileceðin tarih:")
				say_reward(string.format("%s", time_to_str(pc.getqf("next_time"))))

				if is_test_server() == true then
					say("Test serverdayken bu süre sýfýrlanýr.")
					say("")
				else
					return
				end
			end

			say("Yeni ismini aþaðýdaki kutuya yaz.") 

			local name = pc.name
			local str = input()

			local ret = pc.change_name(str)
			
			if string.len(str) > 12 then
			say_title("Ýsim izi")
			say("Ýsim çok uzun.")
			return
			end
			
			if ret == 0 then
			say_title("Ýsim izi")
				say("Ýsim geçerli deðil. Baþka bir isim")
				say("yaz. Türkçe veya özel karakter kullanma.")
				say("Lütfen oyuna tekrar giriþ yap.")

				char_log(0, "CHANGE_NAME", "HAVE NOT RE-LOGIN")
			elseif ret == 1 then
			say_title("Ýsim izi")
				say("Bir sorun var.")
				say("Daha sonra tekrar dene.")
				say("")
				
				char_log(0, "CHANGE_NAME", "ITEM USE PROBLEM")
			elseif ret == 2 then
			say_title("Ýsim izi")
				say("Ýsim geçerli deðil. Baþka bir isim")
				say("yaz. Türkçe veya özel karakter kullanma.")
				say("")

				char_log(0, "CHANGE_NAME", "CAN NOT USE NAME")
			elseif ret == 3 then
			say_title("Ýsim izi")
				say("Bu ismi baþka bir oyuncu kullanýyor.")
				say("Kendine baþka bir isim bul.")
				say("")

				char_log(0, "CHANGE_NAME", "ALREADY USING NAME")
			elseif ret == 4 then
			say_title("Ýsim izi")
				say("Ýsim deðiþtirme gerçekleþti.")
				say("Oyuna tekrar giriþ yap.")
				say("")

				pc.remove_item("71055",1)

				pc.setqf("next_time", get_time() + time_hour_to_sec(1*1))

				char_log(0, "CHANGE_NAME", "SUCCESS: from "..name.." to "..str)
			else
				say("Bir hata oluþtu.")

				char_log(0, "CHANGE_NAME", "UNKNOWN NAME")
			end
		end
	end
end

