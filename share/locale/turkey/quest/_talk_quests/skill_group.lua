--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest skill_group begin
        state start begin
                when login or levelup with pc.level>=5 and pc.skillgroup ==0 begin
                        set_state(run)
                end
        end
		
        state run begin
                -- Arahan Troop
                when
						skill_group1_1.chat."Bedensel Savaþ eðitimine baþla" or
                        skill_group2_1.chat."Bedensel Savaþ eðitimine baþla" or
                        skill_group3_1.chat."Bedensel Savaþ eðitimine baþla" with pc.skillgroup == 0 begin
                        ----------------------------------------------
                        local pc_job=pc.get_job()
                        if pc_job != 0 then -- !!!
                                ---                                                   l
                                say_title("Yanlýþ Eðitim ")
                                say("")
                                say("Özür dilerim.")
                                say("Sadece Savaþçýlar Bedensel Savaþý seçebilir.")
                                say("")
                                say("")

                                if pc_job==0 then
                                        ---                                                   l
                                        say_reward("Bir Savaþçý,  Zihinsel Savaþý ")
                                        say_reward("veya Bedensel Savaþý ")
                                        say_reward("seçebilir.")
                                elseif pc_job==1 then
                                        say_reward("Bir Ninja, Yakin Dövüþ veya")
                                        say_reward("Uzak Dövüþ eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==2 then
                                        say_reward("Bir Sura, Büyülü Silah")
                                        say_reward("veya Kara Büyü eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==3 then
                                        say_reward("Bir Þaman, Ejderha Gücü eðitimini")
                                        say_reward("veya Ýyileþtirme eðitimini")
                                        say_reward("seçebilir.")
								elseif pc_job==4 then
										say_reward("Bir Lycan, sadece Ýç güdü eðitimini")
										say_reward("seçebilir.")
                                end
                                say("")
                                return
                        end
                        ----------------------------------------------
                        say_title("Bedensel Savaþ eðitimi")
                        ---                                                   l
                        say("Bedensel savaþ eðitimi hýzlý savaþma eðitimi ve")
                        say("metodlarýný, ayrýca geliþmiþ temel saldýrý ")
                        say("tiplerini içerir.")
                        say_reward("Gereksinim: Seviye 5 ve üstü bir savaþçý olmak")
                        say("")


                        ----------------------------------------------
                        local s=select("Baþla", "Daha sonra")

                        if 2 == s then
                                return
                        end

                        if pc.get_job()!=0 or pc.get_skill_group()!=0 then
                                test_chat("npc_is_same_job:"..bool_to_str(npc_is_same_job()))
                                test_chat("pc.get_skill_group:"..pc.get_skill_group())
                                test_chat("pc_job:"..pc.get_job())
                                return
                        end

                        set_state("start")
                        pc.set_skill_group(1) -- !!!
                        pc.clear_skill()
                        char_log(0, "CLEAR_SKILL", "Ögretmen silindikten sonraki dogal yetenek.")
                        ----------------------------------------------

                        say_title("Bedensel savaþ eðitimini seçtin.")
                        ---                                                   l
                        say("Hoþgeldin!")
						
                        if not pc.is_clear_skill_group() then
                                ---                                                   l
								local beceripuan = pc.get_level()
								local beceripuanhesapla = beceripuan-1
                                say("Ders almayý seçtiðin için "..beceripuanhesapla.." yetenek puaný ")
                                say("kazandýn. Bu puanlarý dilediðin herhangi bir")
                                say("yeteneðini geliþtirmek için kullabilirsin.")
                                say_title("Bilgi:")
                                say_reward(""..beceripuanhesapla.." yetenek puaný kazandýn.")
                                say("")
								pc.set_skill_level (1,0)
                                                                pc.set_skill_level (2,0)
                                                                pc.set_skill_level (3,0)
                                                                pc.set_skill_level (4,0)
                                                                pc.set_skill_level (5,0)
								pc.set_skill_level (6,0)
								pc.set_skill_level (121,20)
								pc.set_skill_level (137,20)
								pc.set_skill_level (138,20)
								pc.set_skill_level (139,20)
                        else
                                say_reward("Yeni bir ders seçtiginde,")
                                say_reward("daha fazla tecrübe puani almayacaksin.")
                                say("")
                        end
                        clear_letter()
                end

                -- Byulki Troop
                when
                        skill_group1_2.chat."Zihinsel-Savaþ eðitimine baþla" or
                        skill_group2_2.chat."Zihinsel-Savaþ eðitimine baþla" or
                        skill_group3_2.chat."Zihinsel-Savaþ eðitimine baþla" with pc.skillgroup == 0 
                        begin

                        ----------------------------------------------
                        local pc_job=pc.get_job()
                        if pc_job != 0 then
                                ---                                                   l
                                say_title("Yanlýþ Eðitim ")
                                say("")
                                say("Özür Dilerim.")
                                say("Sadece Savaþçýlar Zihinsel Savaþý ")
                                say("seçebilir.")
                                say("")

                                if pc_job==0 then
                                        ---                                                   l
                                        say_reward("Bir Savaþçý , Zihinsel Savaþý ")
                                        say_reward("veya Bedensel Savaþý ")
                                        say_reward("seçebilir.")
                                elseif pc_job==1 then
                                        say_reward("Bir Ninja, Yakýn Dövüþ veya")
                                        say_reward("Uzak Dövüþ eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==2 then
                                        say_reward("Bir Sura, Büyülü Silah")
                                        say_reward("veya Kara Büyü eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==3 then
                                        say_reward("Bir saman ,Ejderha Gücü eðitimini")
                                        say_reward("veya iyilestirme eðitimini")
                                        say_reward("seçebilir.")
								elseif pc_job==4 then
										say_reward("Bir Lycan, sadece Ýç güdü eðitimini")
										say_reward("seçebilir.")
                                end
                                say("")
                                return
                        end
                        ----------------------------------------------

                        say_title("Zihinsel Savaþ eðitimi")
                        say("")
                        ---                                                   l
                        say("Zihinsel savaþ eðitimi sana düþmanlarýna nasýl")
                        say("güçlü chi-saldýrýlarý yapabileceðini ve kendini")
                        say("düþman saldýrýlarýndan koruyabileceðini öðretir.")
                        say_reward("Gereksinim: Seviye 5 ve üstü bir savaþçý olmak")
                        say("")

                        ----------------------------------------------
                        local s=select("Baþla", "Daha sonra")

                        if 2 == s then
                                return
                        end

                        if pc.get_job()!=0 or pc.get_skill_group()!=0 then
                                test_chat("npc_is_same_job:"..bool_to_str(npc_is_same_job()))
                                test_chat("pc.get_skill_group:"..pc.get_skill_group())
                                test_chat("pc_job:"..pc.get_job())
                                return
                        end

                        set_state("start")
                        pc.set_skill_group(2)
                        pc.clear_skill()
                        char_log(0, "CLEAR_SKILL", "Ögretmen silindikten sonraki dogal yetenek")
                        ----------------------------------------------


                        say_title("Zihinsel savaþ eðitimini seçtin.")
                        ---                                                   l
                        say("Hoþgeldin!")

                        if not pc.is_clear_skill_group() then
                                           ---                                                   l
								local beceripuan = pc.get_level()
								local beceripuanhesapla = beceripuan-1
                                say("Ders almayý seçtiðin için "..beceripuanhesapla.." yetenek puaný ")
                                say("kazandýn. Bu puanlarý dilediðin herhangi bir")
                                say("yeteneðini geliþtirmek için kullabilirsin.")
                                say_title("Bilgi:")
                                say_reward(""..beceripuanhesapla.." yetenek puaný kazandýn.")
                                say("")
								pc.set_skill_level (17,0)
                                pc.set_skill_level (18,0)
                                pc.set_skill_level (19,0)
                                pc.set_skill_level (20,0)
                                pc.set_skill_level (16,0)
								pc.set_skill_level (21,0)
								pc.set_skill_level (121,20)
								pc.set_skill_level (137,20)
								pc.set_skill_level (138,20)
								pc.set_skill_level (139,20)
                        else
                                say_reward("Yeni bir ders seçtiginde,")
                                say_reward("daha fazla tecrübe puani almayacaksin.")
                                say("")
                        end
                        clear_letter()
                end


                -- Bisal Troop
                when
                        skill_group1_3.chat."Yakin Dövüþ eðitimine baþla" or
                        skill_group2_3.chat."Yakin Dövüþ eðitimine baþla" or
                        skill_group3_3.chat."Yakin Dövüþ eðitimine baþla" with pc.skillgroup == 0
                        begin

                        ----------------------------------------------
                        local pc_job=pc.get_job()
                        if pc_job != 1 then -- !!!
                                ---                                                   l
                                say_title("Yanlýþ Eðitim ")
                                say("")
                                say("Özür Dilerim.")
                                say("Sadece Ninjalar Yakýn Dövüþ eðitimini seçebilir.")
                                say("")

                                if pc_job==0 then
                                        ---                                                   l
                                        say_reward("Bir Savaþçý,  Zihinsel Savaþý ")
                                        say_reward("veya Bedensel Savaþý ")
                                        say_reward("seçebilir.")
                                elseif pc_job==1 then
                                        say_reward("Bir Ninja ,Yakýn Dövüþ veya")
                                        say_reward("Uzak Dövüþ eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==2 then
                                        say_reward("Bir Sura, Büyülü Silah")
                                        say_reward("veya Kara Büyü eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==3 then
                                        say_reward("Bir saman, Ejderha Gücü eðitimini")
                                        say_reward("veya iyilestirme eðitimini")
                                        say_reward("seçebilir.")
								elseif pc_job==4 then
										say_reward("Bir Lycan, sadece Ýç güdü eðitimini")
										say_reward("seçebilir.")
                                end
                                say("")
                                return
                        end
                        ----------------------------------------------
                        say_title("Yakýn Dövüþ eðitimi")
                        say("")
                        ---                                                   l
                        say("Yakin Dövüþ eðitiminde ,Ninjalara düþmanlarýnýn ")
                        say("zayýf noktalarýna , nasýl hýzlý ve ölümcül")
                        say("vuruþlar yapabilecekleri öðretilir.")
                        say_reward("Gereksinim: Seviye 5 ve üstü bir savaþçý olmak")
                        say("")

                        ----------------------------------------------
                        local s=select("Baþla", "Daha Sonra")

                        if 2 == s then
                                return
                        end

                        if pc.get_job()!=1 or pc.get_skill_group()!=0 then
                                test_chat("npc_is_same_job:"..bool_to_str(npc_is_same_job()))
                                test_chat("pc.get_skill_group:"..pc.get_skill_group())
                                test_chat("pc_job:"..pc.get_job())
                                return
                        end

                        set_state("start")
                        pc.set_skill_group(1) -- !!!
                        pc.clear_skill()
                        char_log(0, "CLEAR_SKILL", "Ögretmen silindikten sonraki dogal yetenek")
                        ----------------------------------------------

                        say_title("Yakýn Dövüþ eðitimini seçtin.")
                        ---                                                   l
                        say("Hoþgeldin!")

                        if not pc.is_clear_skill_group() then
                                ---                                                   l
								local beceripuan = pc.get_level()
								local beceripuanhesapla = beceripuan-1
                                say("Ders almayý seçtiðin için "..beceripuanhesapla.." yetenek puaný ")
                                say("kazandýn. Bu puanlarý dilediðin herhangi bir")
                                say("yeteneðini geliþtirmek için kullabilirsin.")
                                say_title("Bilgi:")
                                say_reward(""..beceripuanhesapla.." yetenek puaný kazandýn.")
                                say("")
								pc.set_skill_level (31,0)
                                pc.set_skill_level (32,0)
                                pc.set_skill_level (33,0)
                                pc.set_skill_level (34,0)
                                pc.set_skill_level (35,0)
								pc.set_skill_level (36,0)
								pc.set_skill_level (121,20)
								pc.set_skill_level (137,20)
								pc.set_skill_level (138,20)
								pc.set_skill_level (139,20)
								pc.set_skill_level (140,20)
                        else
                                say_reward("Yeni bir ders seçtiðinde,")
                                say_reward("daha fazla tecrübe puaný almayacaksýn.")
                                say("")
                        end
                        clear_letter()
                end


                -- Kangno Troop
                when
                        skill_group1_4.chat."Yakýn Dövüþ eðitimine baþla" or
                        skill_group2_4.chat."Yakýn Dövüþ eðitimine baþla" or
                        skill_group3_4.chat."Yakýn Dövüþ eðitimine baþla" with pc.skillgroup == 0
                        begin

                        ----------------------------------------------
                        local pc_job=pc.get_job()
                        if pc_job != 1 then -- !!!
                                ---                                                   l
                                say_title("Yanlýþ Eðitim ")
                                say("")
                                say("Özür Dilerim.")
                                say("Sadece Ninjalar Uzak Dövüþ eðitimini seçebilir.")
                                say("")

                                if pc_job==0 then
                                        ---                                                   l
                                        say_reward("Bir Savaþçý,  Zihinsel Savaþý ")
                                        say_reward("veya Bedensel Savaþý ")
                                        say_reward("seçebilir.")
                                elseif pc_job==1 then
                                        say_reward("Bir Ninja, Yakýn Dövüþ veya")
                                        say_reward("Uzak Dövüþ eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==2 then
                                        say_reward("Bir Sura, Büyülü Silah")
                                        say_reward("veya Kara Büyü eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==3 then
                                        say_reward("Bir saman ,Ejderha Gücü eðitimini")
                                        say_reward("veya Ýyileþtirme eðitimini")
                                        say_reward("seçebilir.")
								elseif pc_job==4 then
										say_reward("Bir Lycan, sadece Ýç güdü eðitimini")
										say_reward("seçebilir.")
                                end
                                say("")
                                return
                        end
                        ----------------------------------------------

                        say_title("Uzak Dövüþ eðitimi")
                        say("")
                        ---                                                   l
                        say("Uzak Dövüþ eðitiminde ,Ninjalara nasýl ok")
                        say("kullanmalarý gerektiði ve uzak mesafeden")
                        say("düþmanlarýyla dikkatli savaþmalarý öðretilir.")
                        say_reward("Gereksinim: Seviye 5 ve üstü bir savaþçý olmak")
                        say("")

                        ----------------------------------------------
                        local s=select("Baþla", "Daha sonra")

                        if 2 == s then
                                return
                        end

                        if pc.get_job()!=1 or pc.get_skill_group()!=0 then
                                test_chat("npc_is_same_job:"..bool_to_str(npc_is_same_job()))
                                test_chat("pc.get_skill_group:"..pc.get_skill_group())
                                test_chat("pc_job:"..pc.get_job())
                                return
                        end

                        set_state("start")
                        pc.set_skill_group(2) -- !!!
                        pc.clear_skill()
                        char_log(0, "CLEAR_SKILL", "Ögretmen silindikten sonraki dogal yetenek")
                        ----------------------------------------------

                        say_title("Uzak Dövüþ eðitimini seçtin.")
                        ---                                                   l
                        say("Hoþgeldin!")

                        if not pc.is_clear_skill_group() then
                                           ---                                                   l
								local beceripuan = pc.get_level()
								local beceripuanhesapla = beceripuan-1
                                say("Ders almayý seçtiðin için "..beceripuanhesapla.." yetenek puaný ")
                                say("kazandýn. Bu puanlarý dilediðin herhangi bir")
                                say("yeteneðini geliþtirmek için kullabilirsin.")
                                say_title("Bilgi:")
                                say_reward(""..beceripuanhesapla.." yetenek puaný kazandýn.")
                                say("")
								pc.set_skill_level (49,0)
                                pc.set_skill_level (50,0)
                                pc.set_skill_level (48,0)
                                pc.set_skill_level (47,0)
                                pc.set_skill_level (46,0)
								pc.set_skill_level (51,0)
								pc.set_skill_level (121,20)
								pc.set_skill_level (137,20)
								pc.set_skill_level (138,20)
								pc.set_skill_level (139,20)
								pc.set_skill_level (140,20)
                        else
                                say_reward("Yeni bir ders seçtiðinde,")
                                say_reward("daha fazla tecrübe puaný almayacaksýn.")
                                say("")
                        end
                        clear_letter()
                end


                -- Hwanmoo Troop
                when
                        skill_group1_5.chat."Büyülü Silah eðitimine baþla" or
                        skill_group2_5.chat."Büyülü Silah eðitimine baþla" or
                        skill_group3_5.chat."Büyülü Silah eðitimine baþla" with pc.skillgroup == 0
                        begin

                        ----------------------------------------------
                        local pc_job=pc.get_job()
                        if pc_job != 2 then -- !!!
                                ---                                                   l
                                say_title("Yanlýþ Eðitim ")
                                say("")
                                say("Özür Dilerim.")
                                say("Sadece Suralar Büyülü Silah eðitimini")
                                say("seçebilir.")
                                say("")

                                if pc_job==0 then
                                        ---                                                   l
                                        say_reward("Bir Savaþçý,  Zihinsel Savaþý ")
                                        say_reward("veya Bedensel Savaþý ")
                                        say_reward("seçebilir.")
                                elseif pc_job==1 then
                                        say_reward("Bir Ninja, Yakýn Dövüþ veya")
                                        say_reward("Uzak Dövüþ eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==2 then
                                        say_reward("Bir Sura, Büyülü Silah")
                                        say_reward("veya Kara Büyü eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==3 then
                                        say_reward("Bir saman, Ejderha Gücü eðitimini")
                                        say_reward("veya Ýyileþtirme eðitimini")
                                        say_reward("seçebilir.")
								elseif pc_job==4 then
										say_reward("Bir Lycan, sadece Ýç güdü eðitimini")
										say_reward("seçebilir.")
                                end
                                say("")
                                return
                        end
                        ----------------------------------------------

                        say_title("Büyülü Silah eðitimi")
                        say("")
                        ---                                                   l
                        say("Büyü Silah eðitiminde ,zayýflatýlmýþ düþmanlara")
                        say("karþý ,lanetli býçaklarý kullanarak güçlü ")
                        say("darbe vurma yöntemleri öðretilir.")
                        say_reward("Gereksinim: Seviye 5 ve üstü bir savaþçý olmak")
                        say("")

                        ----------------------------------------------
                        local s=select("Baþla", "Daha sonra")

                        if 2 == s then
                                return
                        end

                        if pc.get_job()!=2 or pc.get_skill_group()!=0 then
                                test_chat("npc_is_same_job:"..bool_to_str(npc_is_same_job()))
                                test_chat("pc.get_skill_group:"..pc.get_skill_group())
                                test_chat("pc_job:"..pc.get_job())
                                return
                        end

                        set_state("start")
                        pc.set_skill_group(1) -- !!!
                        pc.clear_skill()
                        char_log(0, "CLEAR_SKILL", "Ögretmen silindikten sonraki dogal yetenek")
                        ----------------------------------------------


                        say_title("Büyülü Silah eðitimini seçtin.")
                        ---                                                   l
                        say("Hoþgeldin!")

                        if not pc.is_clear_skill_group() then
                                           ---                                                   l
								local beceripuan = pc.get_level()
								local beceripuanhesapla = beceripuan-1
                                say("Ders almayý seçtiðin için "..beceripuanhesapla.." yetenek puaný ")
                                say("kazandýn. Bu puanlarý dilediðin herhangi bir")
                                say("yeteneðini geliþtirmek için kullabilirsin.")
                                say_title("Bilgi:")
                                say_reward(""..beceripuanhesapla.." yetenek puaný kazandýn.")
                                say("")
								pc.set_skill_level (61,0)
                                pc.set_skill_level (62,0)
                                pc.set_skill_level (63,0)
                                pc.set_skill_level (64,0)
                                pc.set_skill_level (65,0)
								pc.set_skill_level (66,0)
								pc.set_skill_level (121,20)
								pc.set_skill_level (137,20)
								pc.set_skill_level (138,20)
								pc.set_skill_level (139,20)
                        else
                                say_reward("Yeni bir ders seçtiginde,")
                                say_reward("daha fazla tecrübe puani almayacaksin.")
                                say("")
                        end
                        clear_letter()
                end


                -- Black Horse Troop
                when
                        skill_group1_6.chat."Kara Büyü eðitimine baþla" or
                        skill_group2_6.chat."Kara Büyü eðitimine baþla" or
                        skill_group3_6.chat."Kara Büyü eðitimine baþla" with pc.skillgroup == 0
                        begin

                        ----------------------------------------------
                        local pc_job=pc.get_job()
                        if pc_job != 2 then -- !!!
                                ---                                                   l
                                say_title("Yanlýþ Eðitim ")
                                say("")
                                say("Özür Dilerim.")
                                say("Sadece Suralar Kara Büyü eðitimini")
                                say("seçebilir.")
                                say("")

                                if pc_job==0 then
                                        ---                                                   l
                                        say_reward("Bir Savaþçý,  Zihinsel Savaþý ")
                                        say_reward("veya Bedensel Savaþý ")
                                        say_reward("seçebilir.")
                                elseif pc_job==1 then
                                        say_reward("Bir Ninja, Yakýn Dövüþ veya")
                                        say_reward("Uzak Dövüþ eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==2 then
                                        say_reward("Bir Sura, Büyülü Silah")
                                        say_reward("veya Kara Büyü eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==3 then
                                        say_reward("Bir saman, Ejderha Gücü eðitimini")
                                        say_reward("veya Ýyileþtirme eðitimini")
                                        say_reward("seçebilir.")
								elseif pc_job==4 then
										say_reward("Bir Lycan, sadece Ýç güdü eðitimini")
										say_reward("seçebilir.")
                                end
                                say("")
                                return
                        end
                        ----------------------------------------------

                        say_title("Kara Büyü eðitimi")
                        say("")
                        ---                                                   l
                        say("Kara Büyü eðitiminde, uzak mesafeden ")
                        say("düsmanlarini zayif düsüren  lanet yöntemleri")
                        say("ögretilir.")
                        say_reward("Gereksinim: Seviye 5 ve üstü bir savaþçý olmak")
                        say("")

                        ----------------------------------------------
                        local s=select("Baþla", "Daha sonra")

                        if 2 == s then
                                return
                        end

                        if pc.get_job()!=2 or pc.get_skill_group()!=0 then
                                test_chat("npc_is_same_job:"..bool_to_str(npc_is_same_job()))
                                test_chat("pc.get_skill_group:"..pc.get_skill_group())
                                test_chat("pc_job:"..pc.get_job())
                                return
                        end

                        set_state("start")
                        pc.set_skill_group(2) -- !!!
                        pc.clear_skill()
                        char_log(0, "CLEAR_SKILL", "Ögretmen silindikten sonraki dogal yetenek.")
                        ----------------------------------------------

                        say_title("Kara Büyü eðitimini seçtin.")
                        ---                                                   l
                        say("Hoþgeldin!")

                        if not pc.is_clear_skill_group() then
                                           ---                                                   l
								local beceripuan = pc.get_level()
								local beceripuanhesapla = beceripuan-1
                                say("Ders almayý seçtiðin için "..beceripuanhesapla.." yetenek puaný ")
                                say("kazandýn. Bu puanlarý dilediðin herhangi bir")
                                say("yeteneðini geliþtirmek için kullabilirsin.")
                                say_title("Bilgi:")
                                say_reward(""..beceripuanhesapla.." yetenek puaný kazandýn.")
                                say("")
								pc.set_skill_level (76,0)
                                pc.set_skill_level (77,0)
                                pc.set_skill_level (78,0)
                                pc.set_skill_level (79,0)
                                pc.set_skill_level (80,0)
								pc.set_skill_level (81,0)
								pc.set_skill_level (121,20)
								pc.set_skill_level (137,20)
								pc.set_skill_level (138,20)
								pc.set_skill_level (139,20)
                        else
                                say_reward("Yeni bir ders seçtiginde,")
                                say_reward("daha fazla tecrübe puani almayacaksin.")
                                say("")
                        end
                        clear_letter()
                end


                -- Heavenly Dragon Troop
                when
                        skill_group1_7.chat."Ejderha Gücü eðitimine baþla" or
                        skill_group2_7.chat."Ejderha Gücü eðitimine baþla" or
                        skill_group3_7.chat."Ejderha Gücü eðitimine baþla" with pc.skillgroup == 0
                        begin

                        ----------------------------------------------
                        local pc_job=pc.get_job()
                        if pc_job != 3 then -- !!!
                                ---                                                   l
                                say_title("Yanlýþ Eðitim ")
                                say("")
                                say("Özür Dilerim.")
                                say("Sadece samanlar Ejderha Gücü eðitimini")
                                say("seçebilir.")
                                say("")

                                if pc_job==0 then
                                        ---                                                   l
                                        say_reward("Bir Savaþçý, Zihinsel Savaþý ")
                                        say_reward("veya Bedensel Savaþý ")
                                        say_reward("seçebilir.")
                                elseif pc_job==1 then
                                        say_reward("Bir Ninja, Yakin Dövüþ veya")
                                        say_reward("Uzak Dövüþ eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==2 then
                                        say_reward("Bir Sura, Büyülü Silah")
                                        say_reward("veya Kara Büyü eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==3 then
                                        say_reward("Bir saman, Ejderha Gücü eðitimini")
                                        say_reward("veya iyilestirme eðitimini")
                                        say_reward("seçebilir.")
								elseif pc_job==4 then
										say_reward("Bir Lycan, sadece Ýç güdü eðitimini")
										say_reward("seçebilir.")
                                end
                                say("")
                                return
                        end
                        ----------------------------------------------

                        say_title("Ejderha Gücü eðitimi")
                        say("")
                        ---                                                   l
                        say("Ejderha Gücü eðitiminde, Ejderha Tanrýnýn ")
                        say("güçlü ateþ saldýrýlarýný yönetmeyi ve ")
                        say("dayanýklýlýk büyüleriyle Ejderha Tanrýnýn ")
                        say("korumasýný kullanmayi ögrenirler.")
                        say_reward("Gereksinim: Seviye 5 ve üstü bir savaþçý olmak")
                        say("")

                        ----------------------------------------------
                        local s=select("Baþla", "Daha sonra")

                        if 2 == s then
                                return
                        end

                        if pc.get_job()!=3 or pc.get_skill_group()!=0 then
                                test_chat("npc_is_same_job:"..bool_to_str(npc_is_same_job()))
                                test_chat("pc.get_skill_group:"..pc.get_skill_group())
                                test_chat("pc_job:"..pc.get_job())
                                return
                        end

                        set_state("start")
                        pc.set_skill_group(1) -- !!!
                        pc.clear_skill()
                        char_log(0, "CLEAR_SKILL", "Ögretmen silindikten sonraki dogal yetenek.")
                        ----------------------------------------------

                        say_title("Ejderha Gücü eðitimini seçtin.")
                        ---                                                   l
                        say("Hoþgeldin!")

                        if not pc.is_clear_skill_group() then
                                           ---                                                   l
								local beceripuan = pc.get_level()
								local beceripuanhesapla = beceripuan-1
                                say("Ders almayý seçtiðin için "..beceripuanhesapla.." yetenek puaný ")
                                say("kazandýn. Bu puanlarý dilediðin herhangi bir")
                                say("yeteneðini geliþtirmek için kullabilirsin.")
                                say_title("Bilgi:")
                                say_reward(""..beceripuanhesapla.." yetenek puaný kazandýn.")
                                say("")
								pc.set_skill_level (91,0)
                                pc.set_skill_level (92,0)
                                pc.set_skill_level (93,0)
                                pc.set_skill_level (94,0)
                                pc.set_skill_level (95,0)
								pc.set_skill_level (96,0)
								pc.set_skill_level (121,20)
								pc.set_skill_level (137,20)
								pc.set_skill_level (138,20)
								pc.set_skill_level (139,20)
                        else
                                say_reward("Yeni bir ders seçtiginde,")
                                say_reward("daha fazla tecrübe puani almayacaksin.")
                                say("")
                        end
                        clear_letter()
                end


                -- Thunder Light Troop
                when
                        skill_group1_8.chat."Ýyileþtirme eðitimine baþla" or
                        skill_group2_8.chat."Ýyileþtirme eðitimine baþla" or
                        skill_group3_8.chat."Ýyileþtirme eðitimine baþla" with pc.skillgroup == 0
                        begin

                        ----------------------------------------------
                        local pc_job=pc.get_job()
                        if pc_job != 3 then -- !!!
                                ---                                                   l
                                say_title("Yanlýþ Eðitim ")
                                say("")
                                say("Özür Dilerim.")
                                say("Sadece Þamanlar Ýyileþtirme Eðitimi")
                                say("seçebilir.")
                                say("")

                                if pc_job==0 then
                                        ---                                                   l
                                        say_reward("Bir Savaþçý,  Zihinsel Savaþý ")
                                        say_reward("veya Bedensel Savaþý ")
                                        say_reward("seçebilir.")
                                elseif pc_job==1 then
                                        say_reward("Bir Ninja, Yakýn Dövüþ veya")
                                        say_reward("Uzak Dövüþ eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==2 then
                                        say_reward("Bir Sura, Büyülü Silah")
                                        say_reward("veya Kara Büyü eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==3 then
                                        say_reward("Bir saman ,Ejderha Gücü eðitimini")
                                        say_reward("veya Ýyileþtirme eðitimini")
                                        say_reward("seçebilir.")
								elseif pc_job==4 then
										say_reward("Bir Lycan, sadece Ýç güdü eðitimini")
										say_reward("seçebilir.")
                                end
                                say("")
                                return
                        end

                        ----------------------------------------------

                        say_title("Ýyileþtirme eðitimi")
                        say("")
                        ---                                                   l
                        say("Ýyileþtirme eðitiminde, düþmanlarýný ")
                        say("uyuþturmayý, ve kendi grup üyelerini")
                        say("iyileþtirerek güçlendirmeyi öðrenirler.")
                        say_reward("Gereksinim: Seviye 5 ve üstü bir savaþçý olmak")
                        say("")

                        ----------------------------------------------
                        local s=select("Baþla", "Daha sonra")

                        if 2 == s then
                                return
                        end

                        if pc.get_job()!=3 or pc.get_skill_group()!=0 then
                                test_chat("npc_is_same_job:"..bool_to_str(npc_is_same_job()))
                                test_chat("pc.get_skill_group:"..pc.get_skill_group())
                                test_chat("pc_job:"..pc.get_job())
                                return
                        end

                        set_state("start")
                        pc.set_skill_group(2) -- !!!
                        pc.clear_skill()
                        char_log(0, "CLEAR_SKILL", "Ögretmen silindikten sonraki dogal yetenek.")
                        ----------------------------------------------

                        say_title("Ýyileþtirme eðitimini seçtin.")
                        ---                                                   l
                        say("Hoþgeldin!")

                        if not pc.is_clear_skill_group() then
                                           ---                                                   l
								local beceripuan = pc.get_level()
								local beceripuanhesapla = beceripuan-1
                                say("Ders almayý seçtiðin için "..beceripuanhesapla.." yetenek puaný ")
                                say("kazandýn. Bu puanlarý dilediðin herhangi bir")
                                say("yeteneðini geliþtirmek için kullabilirsin.")
                                say_title("Bilgi:")
                                say_reward(""..beceripuanhesapla.." yetenek puaný kazandýn.")
                                say("")
								pc.set_skill_level (106,0)
                                pc.set_skill_level (107,0)
                                pc.set_skill_level (108,0)
                                pc.set_skill_level (109,0)
                                pc.set_skill_level (110,0)
								pc.set_skill_level (111,0)
								pc.set_skill_level (121,20)
								pc.set_skill_level (137,20)
								pc.set_skill_level (138,20)
								pc.set_skill_level (139,20)
                        else
                                say_reward("Yeni bir ders seçtiginde,")
                                say_reward("daha fazla tecrübe puani almayacaksin.")
                                say("")
                        end
                        clear_letter()
                end
				
                when
                        20402.chat."Ýç güdü eðitimine baþla" with pc.skillgroup == 0
                        begin

                        ----------------------------------------------
                        local pc_job=pc.get_job()
                        if pc_job != 4 then -- !!!
                                ---                                                   l
                                say_title("Yanlýþ Eðitim ")
                                say("")
                                say("Özür Dilerim.")
                                say("Sadece Lycanlar Ýç güdü Eðitimi")
                                say("seçebilir.")
                                say("")

                                if pc_job==0 then
                                        ---                                                   l
                                        say_reward("Bir Savaþçý,  Zihinsel Savaþý ")
                                        say_reward("veya Bedensel Savaþý ")
                                        say_reward("seçebilir.")
                                elseif pc_job==1 then
                                        say_reward("Bir Ninja, Yakýn Dövüþ veya")
                                        say_reward("Uzak Dövüþ eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==2 then
                                        say_reward("Bir Sura, Büyülü Silah")
                                        say_reward("veya Kara Büyü eðitimini")
                                        say_reward("seçebilir.")
                                elseif pc_job==3 then
                                        say_reward("Bir saman ,Ejderha Gücü eðitimini")
                                        say_reward("veya Ýyileþtirme eðitimini")
                                        say_reward("seçebilir.")
								elseif pc_job==4 then
										say_reward("Bir Lycan, sadece Ýç güdü eðitimini")
										say_reward("seçebilir.")
                                end
                                say("")
                                return
                        end

                        ----------------------------------------------

                        say_title("Ýçgüdü eðitimi")
                        say("")
                        ---                                                   l
                        say("Ýçgüdü öðretiminde size olaðanüstü [ENTER]döðüþ sanatýný ögretecegim.")
                        say_reward("Gereksinim: Seviye 5 ve üstü bir savaþçý olmak")
                        say("")

                        ----------------------------------------------
                        local s=select("Baþla", "Daha sonra")

                        if 2 == s then
                                return
                        end

                        if pc.get_job()!=4 or pc.get_skill_group()!=0 then
                                test_chat("npc_is_same_job:"..bool_to_str(npc_is_same_job()))
                                test_chat("pc.get_skill_group:"..pc.get_skill_group())
                                test_chat("pc_job:"..pc.get_job())
                                return
                        end

                        set_state("start")
                        pc.set_skill_group(1) -- !!!
                        pc.clear_skill()
                        char_log(0, "CLEAR_SKILL", "Ögretmen silindikten sonraki dogal yetenek.")
                        ----------------------------------------------

                        say_title("Ýç güdü eðitimini seçtin.")
                        ---                                                   l
                        say("Hoþgeldin!")

                        if not pc.is_clear_skill_group() then
                                           ---                                                   l
								local beceripuan = pc.get_level()
								local beceripuanhesapla = beceripuan-1
                                say("Ders almayý seçtiðin için "..beceripuanhesapla.." yetenek puaný ")
                                say("kazandýn. Bu puanlarý dilediðin herhangi bir")
                                say("yeteneðini geliþtirmek için kullabilirsin.")
                                say_title("Bilgi:")
                                say_reward(""..beceripuanhesapla.." yetenek puaný kazandýn.")
                                say("")
								pc.set_skill_level (170,0)
                                pc.set_skill_level (171,0)
                                pc.set_skill_level (172,0)
                                pc.set_skill_level (173,0)
                                pc.set_skill_level (174,0)
								pc.set_skill_level (175,0)
								pc.set_skill_level (121,20)
								pc.set_skill_level (137,20)
								pc.set_skill_level (138,20)
								pc.set_skill_level (139,20)
                        else
                                say_reward("Yeni bir ders seçtiginde,")
                                say_reward("daha fazla tecrübe puani almayacaksin.")
                                say("")
                        end
                        clear_letter()
                end
        end -- end_of_state: run
end -- end_of_quest
