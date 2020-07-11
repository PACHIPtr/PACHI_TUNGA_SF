--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest guild_war_join begin
    state start begin
        when letter begin
            local e = guild.get_any_war()

            if e != 0 and pc.get_war_map() == 0 then
                setskin(NOWINDOW)
                makequestbutton("Giriþ izni iste")
            end
        end

        when button begin
            local e = guild.get_any_war()

            if e == 0 then
                say("Savaþ bitti bile.")
            else
                say(guild.name(e).." loncasý ile sen de savaþmak istiyor musun?")

                local s = select("Evet","Hayýr")

                if s == 1 then
                    guild.war_enter(e)
                else
                    setskin(NOWINDOW)
                    makequestbutton("Giriþ izni iste")
                end
            end
        end
    end
end
