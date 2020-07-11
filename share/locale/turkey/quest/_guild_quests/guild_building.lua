--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest guild_building begin
    state start begin
        when 20040.click begin
            say_title("Emlakçý:")
                say("")
                say("Ben bu arsanýn satýþ temsilcisiyim. Ýstilacý ")
                say("yaratýklar, orta kýtayý talan etmeden önce arsa")
                say("parselleme yoktu, herkesin topraðý vardý ve")
                say("insanlar mutluydu... Her neyse, arsayý lonca")
                say("bölgesi olarak kullanabilmeleri için sadece lonca")
                say("liderlerine satýyorum. Arsa almak ister misin?")
                say("")

            local s = select(locale.yes, locale.no)

            if s == 1 then
                if not pc.is_guild_master() then
                    say_title("Emlakçý:")
                        say("")
                        say("Sana zaten söylemiþtim; arsalarý herkese")
                        say("satmýyorum. Umarým bu defa beni anlamýþsýndýr.")
						say("")
                elseif building.has_land(pc.get_guild()) then
                    say_title("Emlakçý:")
                        say("")
                        say("Her Loncanýn en fazla bir arazisi olabilir.")
                        say("")
                else
                    local land_id = building.get_land_id(pc.get_map_index(), pc.get_x()*100, pc.get_y()*100)

                    if land_id == 0 then
                        say_title("Emlakçý:")
                        say("")
                        say("Sipariþ hatasý oldu.")
                    else
                        local price, owner, guild_level_limit = building.get_land_info(land_id)
                        say_title("Emlakçý:")
                        say("")
                        say("Bu araziyi almak için ")
                        say("lonca derecesinin "..guild_level_limit.." olmasý ")
                        say("gerekiyor. Arazinin fiyatý "..price..locale.gold..".")

                        if guild.level(pc.get_guild()) < guild_level_limit then
                            say("")
                                say("Senin loncan daha bu derceye gelmedi.")
                                say("Özür dilerim.")
                        else
                            say("")
                                say("Arazi bedelini Yang olarak mý ödemek istiyorsun?")

                            s = select(locale.yes, locale.no)

                            if s == 1 then
                                local price, owner, guild_level_limit = building.get_land_info(land_id)

                                if owner!= 0 then
                                    say_title("Emlakçý:")
                                say("")
                                        say("Bu arazi baþka loncaya satýldý. ")
                                        say("Özür dilerim.")
                                elseif pc.gold < price then
                                    say_title("Emlakçý:")
                                say("")
                                        say("Ben Emlakçýyým, fiyatý ben belirlemiyorum.")
                                        say("Bana daha çok Yang getir.")
                                else
                                    pc.changegold(-price)
                                    building.set_land_owner(land_id, pc.get_guild())
                                    notice(guild.name(pc.get_guild()).." Loncasý araziyi aldý!")
                                end
                            else
                                say_title("Emlakçý:")
                            say("")
                                say("Ne yapacaðýna karar verdiðinde tekrar gel. Önce")
                    say("sen gel, önce sen al, kaçýrma.")
                            end
                        end
                    end
                end
            else
            say_title("Emlakçý:")
                    say("")
                    say("Ne yapacaðýna karar verdiðinde tekrar gel Önce")
                    say("sen gel, önce sen al, kaçýrma.")
            end
        end
    end
end
