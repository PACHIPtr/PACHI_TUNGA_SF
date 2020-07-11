--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest zodiac_info begin
	state start begin
		when levelup or letter with pc.level >= 20 begin
			send_letter("Elementer Güçler")
			local v = find_npc_by_vnum(20438)
			
			if 0 != v then
				target.vid("__TARGET__", v, mob_name(20438))
			end
		end
		when info or button begin
			say_title("Elementer Güçler:")
			say("[ENTER]Keþiþ Milbon seni arýyor. Galiba sana önemli bir þey[ENTER]anlatacak.")
		end

		when 20438.chat."Elementer Güçler" with pc.level >= 20 begin
			target.delete("__TARGET__")
			say_title("Elementer Güçler:")
				say("")
				say("Hoþ geldin! Bu kadar çabuk gelmen iyi oldu")
				say("")
				say("Zodyak tapýnaðýnýn durumundan haberin var mý?")
				say("Demir ejderin bir zamanlar ateþli nefesinin bir[ENTER]soluðuyla 12 Zodyak bekçisinin onuruna tapýnak[ENTER]tesisini kurduðu söyleniyor. Bekçilerin kutsal[ENTER]salonlarýndan canlý çýkanlarý þan, þöhret ve paha[ENTER]biçilmez hazineler bekliyormuþ.")
				say("Ýlgili bekçilerin tapýnaklarý da yalnýzca belli[ENTER]günlerde açýlýyor. Lütfen bunu unutma.")
				wait()
			say_title("Elementer Güçler:")
				say("")
				say("Miin haritanýn üst sol kenarýnda bulunan küçük[ENTER]küreyi görüyor musun? Animasferler burada[ENTER]sayýlýr. Bu yoðunlaþtýrýlmýþ yaþam enerjisi[ENTER]olmadan bekçilerin kapýlarýndan geçemezsin.[ENTER]Duyduðuma göre Zodyak tapýnaðý için 12 animasfer[ENTER]gerekiyormuþ.")
				say("")
			set_state(state_learning)
		end
	end
	state state_learning begin
		when 20438.chat."Elementer Güçler" begin
			say_title("Elementer Güçler:")
				say("")
				say("Hoþ geldin! Bu kadar çabuk gelmen iyi oldu")
				say("")
				say("Zodyak tapýnaðýnýn durumundan haberin var mý?")
				say("Demir ejderin bir zamanlar ateþli nefesinin bir[ENTER]soluðuyla 12 Zodyak bekçisinin onuruna tapýnak[ENTER]tesisini kurduðu söyleniyor. Bekçilerin kutsal[ENTER]salonlarýndan canlý çýkanlarý þan, þöhret ve paha[ENTER]biçilmez hazineler bekliyormuþ.")
				say("Ýlgili bekçilerin tapýnaklarý da yalnýzca belli[ENTER]günlerde açýlýyor. Lütfen bunu unutma.")
				wait()
			say_title("Elementer Güçler:")
				say("")
				say("Miin haritanýn üst sol kenarýnda bulunan küçük[ENTER]küreyi görüyor musun? Animasferler burada[ENTER]sayýlýr. Bu yoðunlaþtýrýlmýþ yaþam enerjisi[ENTER]olmadan bekçilerin kapýlarýndan geçemezsin.[ENTER]Duyduðuma göre Zodyak tapýnaðý için 12 animasfer[ENTER]gerekiyormuþ.")
				say("")
		end
	end
	state state_1 begin
		when login begin
			set_state(state_learning)
		end
	end
end
