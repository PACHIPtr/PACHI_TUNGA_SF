--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest make_wonso begin
    state start begin
		when 20008.chat."Fasulyeli Pirinçli Tatlı yapmak istiyorum." with game.get_event_flag("newyear_wonso")>0 begin
			if pc.is_busy() == true then
				say_title("Octavio: ")
				say("Meşgul durumda iken bu işlemi gerçekleştiremezsin.")
			elseif pc.is_dead() == true then
				say_title("Octavio: ")
				say("Ölü durumda iken bu işlemi gerçekleştiremezsin.")
			elseif pc.count_item(50016) < 1 and pc.count_item(50019) < 1 then
				say_title("Octavio: ")
				say("Envanterinde yeterli malzeme bulamadım.")
			else
				say_title("Octavio:")
				say("Tamam, Fasulyeli malzeme ve pirinçli krema ile tatlı yapacağım.")
				pc.removeitem(50016,1)
				pc.removeitem(50019,1)
				pc.give_item2(50020,1)
			end
		end
		when 20008.chat."Şekerli Pirinçli Tatlı yapmak istiyorum." with game.get_event_flag("newyear_wonso")>0 begin
			if pc.is_busy() == true then
				say_title("Octavio: ")
				say("Meşgul durumda iken bu işlemi gerçekleştiremezsin.")
			elseif pc.is_dead() == true then
				say_title("Octavio: ")
				say("Ölü durumda iken bu işlemi gerçekleştiremezsin.")
			elseif pc.count_item(50016) < 1 and pc.count_item(50019) < 1 then
				say_title("Octavio: ")
				say("Envanterinde yeterli malzeme bulamadım.")
			else
				say_title("Octavio:")
				say("Tamam, Şekerli malzeme ve pirinçli krema ile tatlı yapacağım.")
				pc.removeitem(50017,1)
				pc.removeitem(50019,1)
				pc.give_item2(50021,1)
			end
		end
		when 20008.chat."Meyveli Pirinçli Tatlı yapmak istiyorum." with game.get_event_flag("newyear_wonso")>0 begin
			if pc.is_busy() == true then
				say_title("Octavio: ")
				say("Meşgul durumda iken bu işlemi gerçekleştiremezsin.")
			elseif pc.is_dead() == true then
				say_title("Octavio: ")
				say("Ölü durumda iken bu işlemi gerçekleştiremezsin.")
			elseif pc.count_item(50016) < 1 and pc.count_item(50019) < 1 then
				say_title("Octavio: ")
				say("Envanterinde yeterli malzeme bulamadım.")
			else
				say_title("Octavio:")
				say("Tamam, Meyveli malzeme ve pirinçli krema ile tatlı yapacağım.")
				pc.removeitem(50018,1)
				pc.removeitem(50019,1)
				pc.give_item2(50022,1)
			end
		end
		when 20008.chat."Nasıl tatlı yapılır?" with game.get_event_flag("newyear_wonso")>0 begin
			say_title("Octavio: ")
			say("Eğer canavarları avlarsanız,[ENTER] sık sık tatlı yapmak için gerekli malzemelerden bulacaksınız.[ENTER] Burada tatlı prinçli krema ve üç tür malzeme bulacaksınız.[ENTER] Tatlı malzemeleri ve prinçli krema ile [ENTER] tatlı yapabilirim.[ENTER] Yediğiniz zaman bir şeyler olacağı söyleniyor.[ENTER] Ne olacağını merak ediyorum.")
		end
		function have_item()
			if 	pc.count_item(50016)>0 or
				pc.count_item(50017)>0 or
				pc.count_item(50018)>0 or
				pc.count_item(50019)>0 or
				pc.count_item(50020)>0 or
				pc.count_item(50021)>0 or
				pc.count_item(50022)>0 then
				return true
			end
			return false
		end
    end
end
