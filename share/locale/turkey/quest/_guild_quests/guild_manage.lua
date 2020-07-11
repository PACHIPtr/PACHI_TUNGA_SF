--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest guild_building begin
        state start begin
                when guild_man1.chat."GM: Yeniden kaydolma.." or guild_man2.chat."GM: Yeniden kaydolma.." or guild_man3.chat."GM: Yeniden kaydolma.."
                        with pc.is_gm() begin

                        say_title("Köy Gardiyaný: ")
                        say("")
                        say("Bilgi istiyor isen, kayýtlý ")
                        say("kiþinin ismini yaz.")
                        say("")
                        local u_name = input()
                        local u_vid=find_pc_by_name(u_name)

                        if u_vid==0 then
                                say_title("Bu isim kayýtlý deðil.")
                                say(u_name)
                                return
                        end

                        local old_vid = pc.select(u_vid)
                        u_withdrawTime=pc.getqf("new_withdraw_time")
                        u_withdrawTime=pc.getqf("new_withdraw_time")
                        pc.select(old_vid)

                        withdrawDelay=game.get_event_flag("guild_withdraw_delay")
                        disbandDelay=game.get_event_flag("guild_disband_delay")

                        say_title("Köy Gardiyaný:")
                        say("")
                        say("En erken katýlýþ tarihini")
                        say("kontrol et.")

                        if u_withdrawTime>0 then
                                say("Geçen zaman: "..time_to_str(u_withdrawTime))
                                say("(Kalan zaman: ".. withdrawDelay.." Gün)")
                                say("")
                        end

                        if u_disbandTime>0 then
                                say("kayýp olmaya zaman: "..time_to_str(u_disbandTime))
                                say("(kalan zaman: ".. disbandDelay.." Gün)")
                                say("")
                        end

                        local retryTime1 = u_withdrawTime + withdrawDelay*86400
                        local retryTime2 = u_withdrawTime + disbandDelay*86400

                        local retryTime = 0
                        if retryTime1 > retryTime2 then
                                retryTime = retryTime1
                        else
                                retryTime = retryTime2
                        end

                        local curTime = get_time()
                        if curTime < retryTime then
                                say("Yeni denemeye, kalan zaman: "..time_to_str(retryTime))
                                say("(Kalan zaman: "..((retryTime-curTime)/3600).." saat)")
                                say("")

                                if is_test_server() then
                                        local s=select("Hemen intibak et", "Kapat")
                                        if s==1 then
                                                local old_vid = pc.select(u_vid)
                                                local curTime=get_time()
                                                pc.setqf("new_withdraw_time", curTime-withdrawDelay*86400)
                                                pc.setqf("new_withdraw_time", curTime-disbandDelay*86400)
                                                pc.select(old_vid)
                                        end
                                end
                        else
                                say("Hemen kayýt mümkün.");
                                say("")
                        end
                end
                when    guild_man1.chat."GM: Loncanýn imkanlarýna bak" or
                        guild_man2.chat."GM: Loncanýn imkanlarýna bak" or
                        guild_man3.chat."GM: Loncanýn imkanlarýna bak"
                        with pc.is_gm() begin
						setskin(NOWINDOW)
                        test_chat("pc.has_guild: "..bool_to_str(pc.has_guild()))
                        test_chat("pc.is_guildmaster: "..bool_to_str(pc.isguildmaster()))
                        test_chat("pc.empire: "..pc.empire)
                        test_chat("npc.empire: "..npc.empire)
                end
                when guild_man1.chat."Loncadan çýk " or
                        guild_man2.chat."Loncadan çýk " or
                        guild_man3.chat."Loncadan çýk "
                        with pc.hasguild() and not pc.isguildmaster() begin
                        -- ??
                        say_title("Köy Gardiyaný:")
                        say("")
                        say("Olduðun loncadan çýkmak mý istiyorsun?")
                        say("Herhalde orada arkadaþ bulamadýn. ")
                        say("Nasýl istersen.")
                        say("Loncadan çikmak istediðine emin misin?")
                        say("")
                        local s = select("Evet", "Hayýr")
                        if s==1 then
                                say_title("Köy Gardiyaný:")
                                say("")
                                say("Tamam.")
                                say("Artýk loncada deðilsin. ")
                                say("")
                                say("")
                                pc.remove_from_guild()
                                pc.setqf("new_withdraw_time",get_global_time())
                        end
                end

                when guild_man1.chat."Loncayý kapat" or
                        guild_man2.chat."Loncayý kapat" or
                        guild_man3.chat."Loncayý kapat"
                        with pc.hasguild() and pc.isguildmaster() begin
                        -- ??
						if guild.get_any_war() == true then
							say_title("Köy Gardiyaný: ")
							say("")
							say("Lonca savaþta iken loncaný kapatamazsýn.")
							setskin(NOWINDOW)
							return
						end
                        say_title("Köy Gardiyaný:")
                        say("")
                        say("Ne?")
                        say("[DELAY value;150]        [/DELAY]")
                        say("O kadar zahmetle bu loncayý ")
                        say("kurdum ve sen onu þimdi kapatmak mý istiyorsun?")
                        say("Bütün hatýralarýn ve dostlarýn ")
                        say("sonsuza kadar kaybolacak!")
                        say("")
                        say("Loncayý kapatmak istediðine emin misin?")
                        say("")
                        local s = select("Evet", "Hayýr")
                        if s==1 then
                        say_title("Köy Gardiyaný:")
                        say("")
                        say("Tamam.")
                        say("Loncayý kapattým.")
                        say("")
                        say("")
                        pc.destroy_guild()
                        pc.setqf("new_disband_time", get_global_time())
                        pc.setqf("new_withdraw_time", get_global_time())
                        end
                end

                when guild_man1.chat."Yeni Lonca kur" or
                        guild_man2.chat."Yeni Lonca kur" or
			guild_man3.chat."Yeni Lonca kur" with not pc.hasguild() or not pc.isguildmaster() begin
			----with (pc.is_gm() or npc.empire == pc.empire)
                        --------------------------------------------------------------
                        local level_limit;
                        level_limit = 40
                        -----------------------------------------------------------------------

                        if pc.hasguild() then
							setskin(NOWINDOW)
                            return
                        end
                        if game.get_event_flag("guild_withdraw_delay")*86400 >
                                get_global_time() - pc.getqf("new_withdraw_time") then
                        say_title("Köy Gardiyaný:")
                        say("")
                        say("Bir loncadan ayrýldýktan sonra, 1 gün beklemelisin.")
                        return
                        end

                        if game.get_event_flag("guild_disband_delay")*86400 >
                                get_global_time() - pc.getqf("new_withdraw_time") then
                        say_title("Köy Gardiyaný: ")
                        say("")
                        say("Bir lonca kapattýn! ")
                        say("Bu yüzden 1 gün beklemelisin.")
                        say_reward("Loncayý kapattýðýn tarih: ")
						say_reward(string.format("%s", time_to_str(pc.getqf("new_withdraw_time"))))
                        return
                        end
                        say_title("Köy Gardiyaný:")
                        say("")
                        say("Yeni lonca kurmak istiyor musun?")
                        say("")
                        say("Yeni lonca kurmak için ")
                        say("en az 40.seviyede olmalýsýn. Ayrýca")
                        say(" 200.000 Yang gerekiyor.")
                        say("")
                        say("Lonca kurmak istiyor musun?")
                        say("")
                        local s = select("Evet", "Hayýr")
                        if s == 2 then
                                return
                        end

                        if pc.get_level() < 40 then
                                say_title("Köy Gardiyaný: ")
                                say("")
                                say_reward("Seviyeniz yeterli deðil.")
                                return
                        end

                        if pc.get_money() < 200000 then
                                say_title("Köy Gardiyaný: ")
                                say("")
                                say_reward("Yeterli miktarda Yang'a sahip deðilsin.")
                                return
                        end

                        game.request_make_guild()
                end
        end
end
