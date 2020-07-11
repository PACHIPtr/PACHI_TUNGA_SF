--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest reset_scroll begin
    state start begin
                when 71002.use begin
                        ---                                                   l
                        say_title("Statü Sýfýrlama Kaðýdý: ")
                        say("")
                        say("Bu kaðýt her þeyi unutmayý mümkün kýlýyor.")
                        say("Bu senin tüm statü puanlarýný sýfýrlayacak.")
                        say("Geri dönüþü mümkün deðil. ")
                        say("")
                        say("Kullanmak istiyor musun?")
                        say("")

                        local s = select("Evet", "Hayýr")
                        if 2 == s then
                                return
                        end

                        say_title("Statünü Sýfýrla Onay:")
                        say("")
                        say("Tüm statü puanlarýn sýfýrlanacak ve ")
                        say("baþtan baþlayacaksýn. ")
                        say("")
                        say("Statü sýfýrlama iþlemini onaylýyor musun?")
                        say("")

                        local c = select("Onaylýyorum", "Sýfýrlamayý iptal et")
                        if 2 == c then
                                return
                        end

                        char_log(0, "RESET_ALL", "USE_ITEM(71002)")
                        pc.remove_item(71002)

                        char_log(0, "RESET_ALL", "RESET_STAT_POINTS")
                        pc.reset_point()

                        --char_log(0, "RESET_ALL", "RESET_SKILL_POINTS")
                        --pc.clear_skill()
                        --pc.set_skill_group(0)
                        char_log(0, "RESET_ALL", "RESET_END")
                end

                when 71003.use begin

                        ----------                                                   l
                        say_title("Beceri Sýfýrlama Kaðýdý: ")
                        say_reward("Seçilen beceri sýfýrlanacak,")
                        say_reward("puanlarýný tekrar düzenleyebileceksin.")
                        say_reward("Þu ana kadar topladýðýn tüm puanlarýný ")
                        say_reward("kaybedeceksin ve seçtiðin beceri için ")
                        say_reward("kullandýðýn puanýn sadece 17'sini ")
                        say_reward("geri kazanacaksýn.")

                        local result = BuildSkillList(pc.get_job(), pc.get_skill_group())

                        local vnum_list = result[1]
                        local name_list = result[2]

                        if table.getn(vnum_list) < 2 then
                                say("Sýfýrlanacak beceri yok.")
                                say("")
                                return
                        end

                        say("Sýfýrlamak istediðin beceriyi seç. ")
                        say("")

                        local i = select_table(name_list)

                        if table.getn(name_list) == i then
                                return
                        end

                        local name = name_list[i]
                        local vnum = vnum_list[i]

                        say_title("Beceri Sýfýrlama Kaðýdý:")
                        say_reward("Seçilen beceri sýfýrlanacak,")
                        say_reward("puanlarýný tekrar düzenleyebileceksin.")
                        say_reward("Þu ana kadar topladýðýn tüm puanlarýný ")
                        say_reward("kaybedeceksin ve seçtiðin beceri için ")
                        say_reward("kullandýðýn puanýn sadece 17'sini ")
                        say_reward("geri kazanacaksýn.")

                        say(string.format("%s becerini sýfýrlamak istiyor musun?", name))
                        say("")

                        local s = select("Evet", "Hayýr")
                        if 2 == s then
                                return
                        end

                        say_title("Becerini Sýfýrla Onay:")
                        say("")
                        say("Beceri sýfýrlama iþlemini onaylýyor musun?")
                        say("")

                        local c = select("Onaylýyorum", "Sýfýrlamayý iptal et")

                        if 2 == c then
                                return
                        end

                        char_log(0, "RESET_ONE_SKILL", "USE_ITEM(71003)")
                        pc.remove_item(71003)

                        char_log(0, "RESET_ONE_SKILL", string.format("RESET_SKILL[%s]", name))
                        pc.clear_one_skill(vnum)

                        char_log(0, "RESET_ONE_SKILL", "APPLY_17MASTER_BONUS")
                        pc.setqf("force_to_master_skill", 1)

                        say_title("Beceri Sýfýrlama Kaðýdý:")
                        say(string.format("%s Becerisi Sýfýrlandý.", name))
                        say("")

                        ---                                                   l
                        say("")
                end
        end
end
