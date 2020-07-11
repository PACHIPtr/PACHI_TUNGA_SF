--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest alter_of_power begin
        state start begin
                when 20077.click with npc.get_guild() == pc.get_guild() and pc.is_guild_master() begin
                        ---                                                   l
                        say_title("Yaholo:")
                        say("")
                        say("Benim adým  Yaholo, Güç Mimberi bekçisiyim.")
                        say("Güç Mimberi bir bina, loncalara")
                        say("üyeleri için yer saðlýyor.")
                        say("Loncan büyüdükçe")
                        say("daha büyük güç mimberi kurabilirsin.")
                        say("Ve daha çok üye bulabilirsin.")
                        say("")

                        if pc.getqf("build_level") == 0 then
                                pc.setqf("build_level", guild.level(pc.get_guild()))
                        end

                        wait()

                        if pc.getqf("build_level") < guild.level(pc.get_guild()) or guild.level(pc.get_guild()) >= 20 then
                                ---                                                   l
                                say_title("Yaholo:")
                                say("")
                                say("Loncan son zamanlarda çok güçlendi.")
                                say("Güç Mimberi'ni büyütmek istiyor musun?")
                                say("")
                                say("Gerekli malzemeler:")
                                say("")
                                say_reward("25 milyon Yang")
                                say_reward("10 Temel Taþý ")
                                say_reward("15 Kontrplak")
                                say_reward("10 Agaç Kökü ")
                                say("")

                                local s = select("Güç Mimberi'ni büyüt.", "Güç Mimberi'ni büyütme.")

                                if s == 1 then
                                        if pc.count_item(90010) >= 10 and pc.count_item(90012) >= 15 and pc.count_item(90011) >= 10 and pc.get_gold() >= 25000000 then
                                                say_title("Yaholo:")
                                                say("")
                                                say("Güç Mimberin büyütülüyor.")
                                                building.reconstruct(14062)
                                                pc.setqf("build_level", guild.level(pc.get_guild()))
                                                char_log(0, "GUILD_BUILDING", "alter_of_power 14062 constructed")

                                                pc.change_gold(-25000000)
                                                pc.remove_item("90010", 10)
                                                pc.remove_item("90011", 10)
                                                pc.remove_item("90012", 15)
                                        else
                                                say_title("Yaholo:")
                                                say("")
                                                say("Yeterli malzeme yok. Þu an minberi")
                                                say("büyütemem.")
                                                say("")
                                        end
                                elseif s == 2 then
                                say_title("Yaholo:")
                                say("")
                                say("Fikrin deðiþirse tekrar gel.")
                                say("")
                                end
                        else
                                say_title("Yaholo:")
                                say("")
                                say("Bence, loncaný güçlendir.")
                                say("")
                        end
                end

                when 20078.click with npc.get_guild() == pc.get_guild() and pc.is_guild_master() begin
                        say_title("Yaholo:")
                        say("")
                        say("Lonca derecen ne kadar yüksek olursa,")
                        say("o kadar büyük güç mimberin olabilir.")
                        say("Derecen artýnca, loncan da")
                        say("güçleniyor.")
                        say("")

                        wait()
                        say_title("Yaholo:")
                        say("")

                        if pc.getqf("build_level") < guild.level(pc.get_guild()) or guild.level(pc.get_guild()) >= 20 then
                                say("Oh, loncan þimdi çok daha güçlü! ")
                                say("Senin güç mimberini büyüteceðim.")
                                say("Gereken malzemeler:")
                                say("")
                                say_reward("30 milyon Yang")
                                say_reward("15 Temel Taþý ")
                                say_reward("20 Kontrplak")
                                say_reward("20 Agaç Kökü ")
                                say("")

                                local s = select("Güç Mimberini büyüt.", "Güç Mimberini büyütme.")

                                if s == 1 then
                                        if pc.count_item(90010) >= 15 and pc.count_item(90012) >= 20 and pc.count_item(90011) >= 15 and pc.get_gold() >= 30000000 then
                                                say_title("Yaholo:")
                                                say("")
                                                say("Bu Mimberi ")
                                                say("büyüteceðim.")
                                                say("")
                                                building.reconstruct(14063)
                                                pc.setqf("build_level", guild.level(pc.get_guild()))
                                                char_log(0, "GUILD_BUILDING", "alter_of_power 14063 constructed")

                                                pc.change_gold(-30000000)
                                                pc.remove_item("90010", 15)
                                                pc.remove_item("90011", 20)
                                                pc.remove_item("90012", 20)
                                        else
                                                say_title("Yaholo:")
                                                say("")
                                                say("Yeterli malzeme yok. Þu an minberi")
                                                say("büyütemem.")
                                                say("")
                                        end
                                elseif s == 2 then
                                        say_title("Yaholo:")
                                        say("")
                                        say("Sonra bir daha gel.")
                                        say("")
                                end
                        end
                end

                when 20079.click with npc.get_guild() == pc.get_guild() and pc.is_guild_master() begin
                        say_title("Yaholo:")
                        say("")
                        say("")
                        say("Güç Mimberi'ni henüz büyütemezsin.")
                        say("")
                        say("")
                end

                when 20077.click or 20078.click or 20079.click with npc.get_guild() == pc.get_guild() and pc.is_guild_master()!= true begin
                        say_title("Yaholo:")
                        say("Güç sunaðý loncana daha çok üye kabul edebilmeni")
                        say("saðlar.")
                end
        end
end
