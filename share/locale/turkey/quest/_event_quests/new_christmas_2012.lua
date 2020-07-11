--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest new_christmas_2012 begin
	state start begin
	    function has_santa()
			return (game.get_event_flag("ex_xmas_drop") > 0)
		end

		when 20126.chat."Ha ha ha! " with new_christmas_2012.has_santa() begin
			say_title(mob_name(20126))
			say ("Ah, sensin![ENTER]Görüþmeyeli uzun zaman oldu.[ENTER]Ozamandan, bu zamana bir yýl geçti, ho ho![ENTER]Tekrar köyde olmamýn nedenini belki merak ediyorsundur..[ENTER]Bekle, anlatayým… ")
			wait()
			say_title(mob_name(20126))
			say ("Bu yýl Noel'i burada kutlamaya[ENTER]karar verdim. O yüzden buradayým. ")
			wait()
			say_title(mob_name(20126))
			say ("Herhangi bir sorun varsa[ENTER]bana sorabilirsin. Mutlu noeller! ")
			wait()
			set_quest_state("new_christmas_2012_nog", "nog_first_give")
			set_quest_state("new_christmas_2012_sock", "baby_dear_give")
			set_state ("__COMPLETE__")
		end
	end
	state __COMPLETE__ begin
		when 20126.chat."init christmas quest" with is_test_server() or pc.is_gm() begin
			set_state ("start")
			set_quest_state("new_christmas_2012_nog", "start")
			set_quest_state("new_christmas_2012_sock", "start")
			say ("init complete")
		end
	end
end
