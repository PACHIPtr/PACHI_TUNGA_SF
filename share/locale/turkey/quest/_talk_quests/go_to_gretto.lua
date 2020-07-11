--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest go_to_gretto begin
	state start begin
		when login or levelup with pc.level >0 begin
			set_state(zone_enter)
		end
	end
		
	state zone_enter begin
		when 20093.chat."Giriş izni iste" begin
            if pc.level < 75 then
                        say_title("Koe-Pung:")
                        say("")
                        ---                                                   l
                        say("75. seviyenin altında oldugun için")
                        say("seni buz zindanına ışınlayamam..")
                        return
                end
                        say_title("Koe-Pung:")
			say("Dur! Ben Ejderha Mezhebinden Koe-Pung,")
			say("bu kapının bekçisiyim. Sadece eğer bana")
			say("Kan Taşını gösterirsen ve mezhebin")
			say("iznini alırsan geçebilirsin!")
			wait()
			if pc.count_item(30190)>0 then
				say_title(""..pc.name.."")
				say("Adım "..pc.name.." ve Ejderha Mezhebinin iradesi")
				say("altında buradayım! Bunu kanıtlamak için ")
				say("kutsal Kan Taşım var. Şimdi geri çekil!")
				say("")
				say("")
				pc.remove_item(30190)
		wait()
			say_title(""..mob_name(20093).."")
			say("Ejderha Tanrı seni seçti.")	
			say("O her zaman seni izliyor olmalı!")	
			wait()
			pc.warp(10000, 1213200)				
			else
                        say_title("Koe-Pung:")
				say("Kan Taşına sahip olursan")
				say("tekrar gel.")
				return
			end		
		end

	end

end

