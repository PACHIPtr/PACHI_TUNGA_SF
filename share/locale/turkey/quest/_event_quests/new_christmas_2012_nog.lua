--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest new_christmas_2012_nog begin
	state start begin
	end
	state nog_first_give begin
		when 20126.chat."Bir sýcak þarap ister misiniz? "  begin
			say_title(mob_name(20126))
			say ("Ýyi bir þarap, bir tutam[ENTER]tarçýn, birkaç karanfil ... Mmmmmh al! ")
			wait()
			say_title(mob_name(20126))
			say ("Bu soðukta bir yudum sýcak þarap, sana iyi gelecektir![ENTER]Sana güç verir ve daha hýzlý hareket edersin.[ENTER]Denemek ister misin?[ENTER]Çok güzel, þimdi geliyor! ")
			wait()
			pc.give_item2 (50216, 6)
			pc.setqf ("last_nog_give_time", get_global_time())
			say_title(mob_name(20126))
			say ("Sýcak þarap sizin için iyi bir[ENTER]tat gibi görünüyor, ha ha![ENTER]Sizi içinizden nasýl ýsýttýðýný[ENTER]hissedebiliyor musunuz? Tadý çok iyi ve meyveli.[ENTER]Mutlu noeller! ")
			set_state(nog_give)
		end
	end
	state nog_give begin
		when 20126.chat."Biraz daha þarap, lütfen! " begin
			say_title(mob_name(20126))
			say ("Ah, bekle![ENTER]Unuttuðum küçük bir þey var![ENTER]Sýcak þarabý günde sadece bir[ENTER]kere verebilirim. Çünkü sýcak þarap[ENTER]20 saat boyunca etki eder. ")
			wait()
			if get_global_time() - pc.getqf ("last_nog_give_time") >= 21600 then
				say_title(mob_name(20126))
				say ("Lütfen! Uzun bir süre beklemelisin.[ENTER]Bu harika içki sizi kanatlandýracak![ENTER]Ha ha ha. Mutlu noeller! ")
				pc.setqf ("last_nog_give_time", get_global_time())
				pc.give_item2 (50216, 6)
			else
				say_title(mob_name(20126))
				say ("Ne, daha mý istiyorsun?[ENTER]Üzgünüm, yeterince þarap içtin.[ENTER]24 saat sonra yeniden içebilirsin.[ENTER]Fazlasý sizin için yararlý olmaz. ")
			end
		end
	end
end

