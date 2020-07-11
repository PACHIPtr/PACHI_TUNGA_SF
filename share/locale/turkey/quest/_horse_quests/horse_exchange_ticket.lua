--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest horse_exchange_ticket begin
        state start begin
                when 20349.chat."At madalyonu almak için fiþ!" with pc.countitem("50005")>=1 begin
                        say_title("Seyis:")
                        say("")
                        say("Bu fis at madalyonu almak")
                        say("için. Bu fisi")
                        say("madalyon ile degistirebilirsin.")
                        say("Degistirmek")
                        say("istiyor musun?")
                        say("")
                        local b=select("Degistir.", "Kapat")
                        if 1==b then
                                if pc.countitem("50005")>=1 then
                                        pc.removeitem("50005", 1)
                                        pc.give_item2("50050", 1)
                                        say("Sana At madalyonu verdim.")
                                end
                        elseif 2==b then
                        else
                                say("UNKNOWN BUTTON ["..b.."]")
                        end
                end
        end
        state __COMPLETE__ begin
                when enter begin
                        q.done()
                end
        end
end
