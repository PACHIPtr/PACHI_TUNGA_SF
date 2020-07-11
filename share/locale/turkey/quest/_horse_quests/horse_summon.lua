--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest horse_summon begin
        state start begin

				function get_horse_summon_prob_pct()
						local skill_level=pc.get_skill_level(131)
						if skill_level==1 then
								return 15
						elseif skill_level==2 then
								return 20
						elseif skill_level==3 then
								return 30
						elseif skill_level==4 then
								return 40
						elseif skill_level==5 then
								return 50
						elseif skill_level==6 then
								return 60
						elseif skill_level==7 then
								return 70
						elseif skill_level==8 then
								return 80
						elseif skill_level==9 then
								return 90
						elseif skill_level>=10 then
								return 100
						end
						return 10
				end


                when 20349.chat."Yeni At Resmi" with horse.get_grade()==1 and pc.countitem("50051")<1 begin
                        say_title("Seyis:")
                        say("")
                        ----"12345678901234567890123456789012345678901234567890"|
                        say("Binici resmini kaybettin.")
                        say("Bana 10.000 Yang verirsen sana ")
                        say("yenisini verebilirim.")
                        say("")
                        local b=select("Al", "Alma")
                        if 1==b then
                                if pc.money>=10000 then
                                        pc.changemoney(-10000)
                                        say_title("Seyis:")
                                        say("")
                                        ----"12345678901234567890123456789012345678901234567890"|
                                        say("Bu yeni binici resmin.")
                                        say("Tekrar kaybetme!")
                                        pc.give_item2("50051", 1)
                                else
                                        say_title("Seyis:")
                                        say("")
                                        ----"12345678901234567890123456789012345678901234567890"|
                                        say("Yeterli Yang'ýn yok.")
                                        say("")
                                end
                        elseif 2==b then
                        else
                                say("UNBEKANNTE TASTE ["..b.."]")
                        end
                end
                when 20349.chat."Yeni Zýrhlý At Kitabý " with horse.get_grade()==2 and pc.countitem("50052")<1 begin
                        say_title("Seyis:")
                        say("")
                        ----"12345678901234567890123456789012345678901234567890"|
                        say("Zýrhlý At Kitabýný kaybettin!")
                        say("50.000 Yang'a sana yenisini ")
                        say("verebilirim.")
                        say("")
                        local b=select("Kabul", "Vazgeç ")
                        if 1==b then
                                if pc.money>=50000 then
                                        pc.changemoney(-50000)
                                        say_title("Ahýr Görevlisi:")
                                        say("")
                                        -----------"12345678901234567890123456789012345678901234567890"|
                                        say("Bu Yeni Zýrhlý At Kitabýn.")
                                        say("Tekrar kaybetme!")
                                        pc.give_item2("50052", 1)
                                else
                                        say("Yeterli Yang'ýn yok.")
                                end
                        elseif 2==b then
                        else
                                say("UNBEKANNTE TASTE ["..b.."]")
                        end
                end
                when 20349.chat."Yeni Asker At Kitabý " with horse.get_grade()==3 and pc.countitem("50053")<1 begin
                        say_title("Seyis:")
                        say("")
                        ----"12345678901234567890123456789012345678901234567890"|
                        say("Asker At Kitabýný kaybettin!")
                        say("100.000 Yang'a sana yenisini verebilirim.")
                        say("")
                        local b=select("Kabul", "Vazgeç ")
                        if 1==b then
                                if pc.money>=100000 then
                                        pc.changemoney(-100000)
                                        say_title("Bilgi:")
                                        say("")
                                        ----"12345678901234567890123456789012345678901234567890"|
                                        say("Bu yeni Asker At Kitabýn.")
                                        say("Onu yine kaybetme!")
                                        say("")
                                        pc.give_item2("50053", 1)
                                else
                                        say_title("Seyis:")
                                        say("")
                                        ----"12345678901234567890123456789012345678901234567890"|
                                        say("Yeterli Yang'ýn yok.")
                                end
                        elseif 2==b then
                        else
                                say("UNBEKANNTE TASTE ["..b.."]")
                        end
                end
                when 50051.use with horse.get_grade()==0 begin
					syschat("Acemi atýný çaðýrabilmek için acemi at görevini yapman gerek.")
                end
                when 50051.use with horse.get_grade()==1 begin
					if horse.is_summon() == true then
						syschat("Zaten acemi atý çaðýrmýþsýn.")
						return
					end
					if pc.get_mount_vnum() != 0 then
						syschat("Zaten bir binek hayvanýna biniyorsun.")
						return
					end
					if number(1, 100)<=horse_summon.get_horse_summon_prob_pct() then
						horse.summon()
						syschat("Acemi at çaðýrýldý.")
					else
						syschat("Acemi at çaðýrýlamadý.")
					end
				end
                when 50051.use with horse.get_grade()==2 begin
					syschat("Zýrhlý atýný çaðýrabilmek için Zýrhlý At Kitabý lazým.")
                end
                when 50051.use with horse.get_grade()==3 begin
					syschat("Uzman seviyedeki atlarý çaðýrmak için Asker At Kitabý lazým.")
                end
                when 50052.use with horse.get_grade()==0 begin
					syschat("Orta seviyedeki atý çaðýrman için acemi at görevini yapman lazým.")
                end
                when 50052.use with horse.get_grade()==1 begin
					syschat("Acemi atý çaðýrman için at resmi lazým.")
                end
                when 50052.use with horse.get_grade()==2 begin
					if horse.is_summon() == true then
						syschat("Zaten zýrhlý atý çaðýrmýþsýn.")
						return
					end
					if pc.get_mount_vnum() != 0 then
						syschat("Zaten bir binek hayvanýna biniyorsun.")
						return
					end
					if number(1, 100)<=horse_summon.get_horse_summon_prob_pct() then
						horse.summon()
						syschat("Zýrhlý at çaðýrýldý.")
					else
						syschat("Zýrhlý at çaðýrýlamadý.")
					end
				end
                when 50052.use with horse.get_grade()==3 begin
					syschat("Asker atý çaðýrman için Asker At Kitabý lazým.")
                end
                when 50053.use with horse.get_grade()==0 begin
					syschat("Ýleri seviyedeki atý çaðýrman için acemi at görevini yapman lazým.")
                end
                when 50053.use with horse.get_grade()==1 begin
					syschat("Acemi atý çaðýrman için at resmi lazým.")
                end
                when 50053.use with horse.get_grade()==2 begin
					syschat("Orta seviyedeki atý çaðýrman için Zýrhlý At Kitabý lazým.")
                end
                when 50053.use with horse.get_grade()==3 begin
					if horse.is_summon() == true then
						syschat("Zaten asker atý çaðýrmýþsýn.")
						return
					end
					if pc.get_mount_vnum() != 0 then
						syschat("Zaten bir binek hayvanýna biniyorsun.")
						return
					end
					if number(1, 100)<=horse_summon.get_horse_summon_prob_pct() then
						horse.summon()
						syschat("Asker at çaðýrýldý.")
					else
						syschat("Asker at çaðýrýlamadý.")
					end
				end
	end
        state __COMPLETE__ begin
                when enter begin
                        q.done()
                end
        end
end
