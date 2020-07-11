--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest rarity_refine begin
	state start begin
		when 20008.chat."Eritme Ýþlemi " begin
			say_title("Octavio: ")
			say("")
			say("Silahýna bonuslar kazanýp eþsiz þekilde ")
			say("parlatmak istiyorsun demek...")
			say("heh aradýðýn kiþiye geldin. silahýna ")
			say("belli elementler karþýlýðýnda bonus kazandýrabilirim.")
			say("")
			say_reward("Pencereyi açmak istiyor musun?")
			local result = select("Evet, istiyorum.","Hayýr, istemiyorum.")
			if result == 1 then
				pc.open_rarity_refine()
				setskin(NOWINDOW)
			else
				setskin(NOWINDOW)
			end
		end
	end
end