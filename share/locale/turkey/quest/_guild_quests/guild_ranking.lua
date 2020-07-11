--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest guild_ranking begin
        state start begin
                when
                        guild_man1.chat."Kendi loncanýn sýralamamý göster" or
                        guild_man2.chat."Kendi loncanýn sýralamamý göster" or
                        guild_man3.chat."Kendi loncanýn sýralamamý göster"
                        with pc.hasguild() begin
                        say_title("Köy Gardiyaný:")
						say("Burada loncaný, sýralamada loncana yakýn olan")
                        say("diðer loncalarý görebilirsin: [DELAY value;0]")
                        ---                                                                l
                        say("|Lonca adý |Puaný |Zaferleri |Beraberlikleri |Yenilgileri")
                        say(guild.around_ranking_string().."[/DELAY]")
                end
                when
                        guild_man1.chat."En iyi lonca sýralamasýný göster(1-8)" or
                        guild_man2.chat."En iyi lonca sýralamasýný göster(1-8)" or
                        guild_man3.chat."En iyi lonca sýralamasýný göster(1-8)"
                        with pc.hasguild() begin
                        say_title("Köy Gardiyaný:")
			say("")
                        say("Burada sýralamasý en yüksek sekiz loncayý ve")
                        say("istatistiklerini görebilirsin: [DELAY value;0]")
                        say("|Lonca adý |Puaný |Zaferleri |Beraberlikleri |Yenilgileri")
                        say(guild.high_ranking_string().."[/DELAY]")
                        say("")
                end
                when
                        guild_man1.chat."Kendi lonca puanýmý göster" or
                        guild_man2.chat."Kendi lonca puanýmý göster" or
                        guild_man3.chat."Kendi lonca puanýmý göster"
                        with pc.hasguild() begin
                        say_title("Köy Gardiyaný:")
                        say("")
                        say("Kendi lonca sýralaman "..guild.get_rank()..".")
                        say("Puan miktarý :"..guild.get_ladder_point()..".")
                        say("")
                end
        end
end
