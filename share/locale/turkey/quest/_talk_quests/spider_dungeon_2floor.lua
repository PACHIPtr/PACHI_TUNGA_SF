--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest spider_dungeon_2floor begin
	state start begin
		when login or levelup with pc.level >0 begin
			set_state(to_spider_2floor)
		end
	end
		
	state to_spider_2floor begin
		when 20089.chat." Birinci kata dön " begin
			say_title("Pung-Ho: ")
			say("Birinci kata gerçekten dönmek istiyor musun?")
			
			local a= select("Evet ", "Hayýr ")
			if 1==a then
			
				say_title("Pung-Ho: ")
				say("Akýllýca bir seçim.[ENTER]Burada uzun süre kalmak çok tehlikeli.[ENTER]Git ve yaralarýný sar. ")
				wait()

				pc.warp(91700, 525300)
				return
			end

			say_title("Pung-Ho: ")
			say("Nasýl dilersen...[ENTER]Dikkat et![ENTER]Burasý çok tehlikeli.")
		end
	end
end