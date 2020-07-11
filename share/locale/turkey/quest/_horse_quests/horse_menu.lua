--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest horse_menu begin
    state start begin
		function horse_menu()
			if horse.is_mine() then
				say("Atýmla ne yapmak istiyorsun?")
				say("")
				local s = 6
				if horse.is_dead() then
					s = select( "Atý Çaðýr", "Atý Yolla", "Hiçbir þey (pencereyi kapat)")
					if s == 1 then s = 0
					elseif s == 2 then s = 3
					elseif s == 3 then return
					end
				else
					s = select(
					"Besle", "Bin", "Gönder", "Atýn karakteri gözleniyor", 
					"Atý isimlendirmek", "Hiçbirþey (pencereyi kapat)")
				end
				if s == 0 then
					horse.revive()
				elseif s == 1 then
					local food = horse.get_grade() + 50054 - 1
					if pc.countitem(food) > 0 then
						pc.removeitem(food, 1)
						horse.feed()
					else
						say("Bunun için aþaðýdakilere ihtiyacýnýz var: "..item_name(food).."")
						say("")
					end
				elseif s == 2 then
					horse.ride()
				elseif s == 3 then
					horse.unsummon()
				elseif s == 4 then
					say("Atýn þu anki gücü: "..horse.get_health_pct().."%")
					say("Atýn mevcut dayanýklýlýðý: "..horse.get_stamina_pct().."%")
					say("")
				elseif s == 5 then
					if pc.countitem("71110") < 1 then
						say("Bunun için aþaðýdakilere ihtiyacýnýz var: "..item_name("71110").."")
						say("")
						return
					end
					local old_horse_name = horse.get_name() ;
					say_title("Atý isimlendir")
					say("At þekeri ile atýna yeni bir isim ")
					say("verebilirsin.")
					say("")
					if string.len(old_horse_name) == 0 then
						say_reward("Atýn henüz adý yok.")
					else
						say_reward("Atýn þuan ki adý "..old_horse_name..".")
					end
					say("")
					say("Yeni bi at ismi gir:")
					say("")
					local horse_name = input()
					if string.len(horse_name) < 2 then
						say_title("Atý isimlendir")
						say("At ismi çok kýsa.")
						say("")
						return
					elseif string.len(horse_name) > 12 then
						say_title("Atý isimlendir")
						say("At ismi çok uzun.")
						say("")
						return
					end
					local ret = horse.set_name(horse_name)
					say_title("Atý isimlendir")
					if ret == 0 then
						say_reward("Bu ismi kullanamazsýn!")
						say("")
					elseif ret == 1 then
						say_reward("Bu ismi kullanamazsýn!")
						say("")
					elseif ret == 2 then
						pc.remove_item("71110",1)
						say("Artýk atýnýn yeni bir ismi var!")
						say("")
					end
				end
			end
		end
		when 20030.click begin horse_menu.horse_menu() end
		when 20101.click begin horse_menu.horse_menu() end
		when 20102.click begin horse_menu.horse_menu() end
		when 20103.click begin horse_menu.horse_menu() end
		when 20104.click begin horse_menu.horse_menu() end
		when 20105.click begin horse_menu.horse_menu() end
		when 20106.click begin horse_menu.horse_menu() end
		when 20107.click begin horse_menu.horse_menu() end
		when 20108.click begin horse_menu.horse_menu() end
		when 20109.click begin horse_menu.horse_menu() end
    end
end
