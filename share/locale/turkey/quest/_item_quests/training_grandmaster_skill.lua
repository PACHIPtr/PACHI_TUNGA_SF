--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest training_grandmaster_skill begin
    state start begin
        when 50513.use begin
            say_title("Grand Masterlarýn Beceri Eðitimi ")
            if pc.get_skill_group() == 0 then
                ---                                                   l
                say("Henüz beceri eðitimine baþlamadýn.")
  
                return
			elseif pc.getf("skill_reset2","skill_bind") + get_time() > 0 then
				say("Becerilerin mühürlenmiþ durumda.")
				return
            end


            local result = training_grandmaster_skill.BuildGrandMasterSkillList(pc.get_job(), pc.get_skill_group())

            local vnum_list = result[1]
            local name_list = result[2]

            if table.getn(vnum_list) == 0 then
                ---                                                   l
                say("Grand Master seviyesinde olan ")
                say("bir becerin yok.")
                return
            end
            ---                                                   l
            say("Grand Master seviyesini artýrmak istediðin")
            say("beceriyi seç. ")
            local menu_list = {}
            table.foreach(name_list, function(i, name) table.insert(menu_list, name) end)
            table.insert(menu_list, "Kapat")

            local s=select_table(menu_list)
            if table.getn(menu_list) == s then
                return
            end

            local skill_name=name_list[s]
			local skill_vnum=vnum_list[s]
            local skill_level = pc.get_skill_level(skill_vnum)
            local cur_alignment = pc.get_real_alignment()
            local need_alignment = 1000+500*(skill_level-30)

			--test_chat("Güncel Derece:" .. cur_alignment.."! ") 
			--test_chat("Gereken Derece: " .. need_alignment .."! ")  
			
			syschat("Güncel Derece:" .. cur_alignment.."! ") 
			syschat("Gereken Derece: " .. need_alignment .."! ")  

			local title = string.format("%s grand master beceri eðitimi ",skill_name)

			say_title(string.format("%s",title))
            say("")
			say("Grand Master becerisi için sýralama puaný ") 
			say("harcanýr.Yani sýralama puanýn negatif deðere ") 
			say("düþebilir. ")  
            say("")

            if cur_alignment<-19000+need_alignment then
                say_reward("Eðitim için yeterli derece yok.")
				say("")
                return
            end

            if cur_alignment<0 then
                say_reward(string.format("Gereken sýralama puaný: %d -> %d ", need_alignment, need_alignment*2))
				say_reward("Bu da demek oluyor ki, grand master becerilerini ") 
				say_reward("yükseltmek için sýralama puaný pozitif olan bir ")
				say_reward("kiþiye göre iki kat puan harcamalýsýn.")
                need_alignment=need_alignment*2
				elseif cur_alignment<need_alignment then
			say_reward(string.format("Gereken sýralama puaný: %d " , need_alignment )) 
			say_reward("Eðer þimdi eðitim alýrsan, sýralama puanýn ") 
			say_reward("negatif deðere düþecek. ") 
            else
                say_reward(string.format("Gereken sýralama puaný:: %d ", need_alignment ))
            end
            say("")

            local s=select("Devam", "Vazgeç ")
            if s==2 then
                return
            end


            if cur_alignment>=0 and cur_alignment<need_alignment then
                say_title(string.format("%s Confirm", title))
				say("")
				say("Eðer þimdi grand master beceri eðitimini ") 
				say("denersen, sýralama puanlarýnýn azalmasý ve ") 
				say("negatif deðere düþmesi mümkün olabilir.Eðer hala ")
				say("eðitimi istiyorsan aþaðýda gördüðün cümleyi yaz.") 
				say_reward("Eðitim istiyorum.")  
				say("Eðer eðitim istemiyorsan, 'ENTER' tuþuna bas.")
				local s = input () 
                if s!="Eðitim istiyorum." then
                    return
                end
            end
			if pc.count_item(50513) < 1 then
				say("Hey!")
				say("Ruh taþýn olmadan nasýl öðreneceksin ")
				say("becerilerini.")
				say("")
				say_reward("Git ve ruh taþý bul.")
				return
			end

            if get_time() < pc.getqf("next_time") then -- and not is_test_server() then
                if pc.is_skill_book_no_delay() then
                    pc.remove_skill_book_no_delay()
                else
					say_title("Grand Masterlarýn Beceri Eðitimi ")
                    return
                end
            end

            pc.setqf("next_time", get_time()+time_hour_to_sec(number(8, 12)))

            if need_alignment>0 then
				if pc.count_item(50513) < 1 then
						say("Hey!")
						say("Ruh taþýn olmadan nasýl öðreneceksin ")
						say("becerilerini.")
						say("")
						say_reward("Git ve ruh taþý bul.")
				else
					if pc.learn_grand_master_skill(skill_vnum) then
						pc.change_alignment(-need_alignment)
						say_title(string.format("%sBaþarýlý ", title))
						if 40 == pc.get_skill_level(skill_vnum) then
							say(string.format("%s Perfect Master oldu.", skill_name))
						else
							say(string.format("%s ulaþtýðý seviye %d." , skill_name , skill_level - 30+1+1)) 
						end
						say("")
						say_reward("Seviyeni baþarý ile yükselttin!") 
						say_reward(string.format("%d sýralama puaný kullandýn.", need_alignment))
						say("")
					else
						say_title(string.format("%s [ENTER]Baþarýsýz ", title))
						say("Becerini geliþtiremedin.")
						say("")
						say_reward("Bir miktar sýralama puaný kaybettin ve bir Ruh Taþý kullandýn.")
						say("")
						pc.change_alignment(-number(need_alignment/3, need_alignment/2))
					end
				end
            end
            pc.remove_item(50513,1)
        end
		
        function BuildGrandMasterSkillList(job, group)
            GRAND_MASTER_SKILL_LEVEL = 30
            PERFECT_MASTER_SKILL_LEVEL = 40
            local skill_list = special.active_skill_list[job+1][group]
            local ret_vnum_list = {}
            local ret_name_list = {}
            table.foreach(skill_list,
            function(i, skill_vnum)
                local skill_level = pc.get_skill_level(skill_vnum)
                if skill_level >= GRAND_MASTER_SKILL_LEVEL and skill_level < PERFECT_MASTER_SKILL_LEVEL then
                    table.insert(ret_vnum_list, skill_vnum)
                    local name=locale.GM_SKILL_NAME_DICT[skill_vnum]
                    if name == nil then name=skill_vnum end
                    table.insert(ret_name_list, name)
                end
            end)
            return {ret_vnum_list, ret_name_list}
        end
    end
end
