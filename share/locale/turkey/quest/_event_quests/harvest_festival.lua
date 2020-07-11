--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest harvest_festival begin
	state start begin
	
		function reset_flags()
			if game.get_event_flag("harvest_festival") == 0 then
				q.setstate("information")
				pc.setqf("harvest_festival_count", 0)
				pc.setqf("harvest_festival_mob_complete", 0)
				pc.setqf("cake_count", 0)
				pc.setqf("rice_count", 0)
				pc.setqf("mob_kill1_count",0)
				pc.setqf("mob_kill2_count",0)
				pc.setqf("mob2_info", 0)
				pc.setqf("mob1_info", 0)
			end
		
		end
		
		function kill_action(mob_vnum, kill_state)
						local kill_count
			if kill_state == 1 then
				kill_count = pc.getqf("mob_kill1_count") + 1
				pc.setqf("mob_kill1_count", kill_count)
				kill_finish = 30
			else
				kill_count = pc.getqf("mob_kill2_count") + 1
				pc.setqf("mob_kill2_count", kill_count)
				kill_finish = 60
			end
			

			if kill_count < 0 then
				return
			end

			if kill_count >= kill_finish then
				q.set_counter("Kalan Sayı ", 0)
			else 
				q.set_counter("Kalan Sayı ", kill_finish - kill_count)
			end

			if kill_count == kill_finish then
				local random_rice = harvest_festival.get_random_rice(2)
				notice_multiline(string.format("Yağmacıları yendin ve %s pirinç patlağı[ENTER]aldın. Şimdi Handu-Up'a geri dön.", random_rice), notice)
				local cake_count = pc.getqf("cake_count") + random_rice
				pc.setqf("cake_count", cake_count)

				pc.setqf("harvest_festival_mob_complete", 1)
				clear_letter()
				if kill_state == 2 then
					q.setstate("information")
				end
			end
		end
		
		function get_random_rice(type)
			if (type == 1) then
				if (pc.get_level() >= 90) then
					random_rice = number(4, 7)
				elseif (pc.get_level() >= 70) then
					random_rice = number(3, 6)
				elseif (pc.get_level() >= 50) then
					random_rice = number(2, 4)
				elseif (pc.get_level() >= 30) then
					random_rice = number(1, 3)
				elseif (pc.get_level() >= 10) then
					random_rice = number(1, 2)
				else
					random_rice = number(1, 2)
				end -- end of if
			elseif (type == 2) then
				if (pc.get_level() >= 90) then
					random_rice = number(7, 10)
				elseif (pc.get_level() >= 70) then
					random_rice = number(6, 8)
				elseif (pc.get_level() >= 50) then
					random_rice = number(4, 6)
				elseif (pc.get_level() >= 30) then
					random_rice = number(2, 5)
				elseif (pc.get_level() >= 10) then
					random_rice = number(2, 4)
				else
					random_rice = number(2, 3)
				end -- end of if
			end
			
			return random_rice
			
		end

		when login with pc.get_level() > 0 begin
			set_state(information)
			pc.setqf("harvest_festival_count", 0)
			pc.setqf("harvest_festival_mob_complete", 0)
			pc.setqf("cake_count", 0)
			pc.setqf("rice_count", 0)
	  end -- end of when
  end -- end of state

	-- Event Start
	state information begin	
		when letter with game.get_event_flag("harvest_festival")>0  begin
			local v= find_npc_by_vnum(20086)
			if 0==v then
			else
				target.vid("__TARGET__", v, "Wonda-Rim: ")
			end
		end

		when login with game.get_event_flag("harvest_festival") == 0 begin
			harvest_festival.reset_flags()
		end

		-- Glutinous rice in exchange for a gift
		when 20087.chat."Pirinç patlağı değiş tokuşu " with game.get_event_flag("harvest_festival")>0 begin
			local rice_count = pc.getqf("rice_count")

			say_title("Wonda-Rim: ")
			say(string.format("%s tane pirinç patlağı topladın! 30 tane[ENTER]topladığında bana gelip değerli bir[ENTER]eşya karşılığında değiş tokuş edebilirsin.[ENTER]Şimdi kendine bir ödül seçmek ister misin?  ", rice_count))
			local reward_set = select("Evet, lütfen! ", "Hayır, daha sonra.")

			if reward_set == 2 then
				say_title("Wonda-Rim: ")
				say("Fikrin değiştiğinde tekrar gel. ")
				return
			end -- end of if	
		
			say_title("Wonda-Rim: ")
			say("Hangi ödülü istersin? ")

			-- local harvest_item_set = select("15x Glut",  "15x Gl","35x Perle","35x Gluttrank","35x Gltes","Maybe later")
			local harvest_item_set = select("Kırmızı Abanoz Sandık (55 Pirinç Pat.) ",
											"İhtişamlı Anaboz Sandık (50 Pirinç Pat) ",
											"Ay Özütü (O) (45 Pirinç Pat) ",
											"Sarı Abanoz Sandık (30 Pirinç Pat) ",
											"Diğer ödüller ",
											"İptal ")

			if harvest_item_set == 6 then
				return
			end -- end of ifa

			local harvest_item_set_2 = 0	

			if harvest_item_set == 5 then
				harvest_item_set_2 = select("Açık Yeşil Abanoz Sandık (65 Pirinç Pat) ",
											"Yeşil Abanoz Sandık (75 Pirinç Pat) ",
											"Güneş Özütü (O) (60 Pirinç Pat) ",
											"Mavi Abanoz Sandık (75 Pirinç Pat) ",
											"Koyu Kır. Abanoz Sandık (80 Pirinç Pat) ",
											"İptal ")
			end

			if harvest_item_set_2 == 6 then
				return
			end

			local harvest_item_name = ""
			local harvest_rice_count = 0
			
			
			if harvest_item_set == 1 then
				harvest_item_name = "Kırmızı Abanoz Sandık (55 Pirinç Pat.) "
				harvest_rice_count = 55 
			elseif harvest_item_set == 2 then
				harvest_item_name = "İhtişamlı Anaboz Sandık (50 Pirinç Pat) "
				harvest_rice_count = 50 
			elseif harvest_item_set == 3 then
				harvest_item_name = "Ay Özütü (O) (45 Pirinç Pat) "
				harvest_rice_count = 45
			elseif harvest_item_set == 4 then
				harvest_item_name = "Sarı Abanoz Sandık (30 Pirinç Pat) "
				harvest_rice_count = 30
			elseif harvest_item_set_2 == 1 then
				harvest_item_name = "Açık Yeşil Abanoz Sandık (65 Pirinç Pat) "
				harvest_rice_count = 65 
			elseif harvest_item_set_2 == 2 then
				harvest_item_name = "Yeşil Abanoz Sandık (75 Pirinç Pat) "
				harvest_rice_count = 75
			elseif harvest_item_set_2 == 3 then
				harvest_item_name = "Güneş Özütü (O) (60 Pirinç Pat) "
				harvest_rice_count = 60 
			elseif harvest_item_set_2 == 4 then
				harvest_item_name = "Mavi Abanoz Sandık (75 Pirinç Pat) "
				harvest_rice_count = 75
			elseif harvest_item_set_2 == 5 then
				harvest_item_name = "Koyu Kır. Abanoz Sandık (80 Pirinç Pat) "
				harvest_rice_count = 80
			end -- end of if	

			-- Check glutinous rice amount
			if rice_count < harvest_rice_count then
				say_title("Wonda-Rim: ")
				say("Bu ödül için yeterince pirinç patlağına[ENTER]sahip değilsin. Daha fazla pirinç patlağı[ENTER]getir ve istediğin ödülü seç! ")
				return
			end -- end of if	

			say_title("Wonda-Rim: ")
			say("Akıllı bir seçim yap. ")
			say("")
			say(string.format("Teşekkür olarak %s pirinç patlağı karşılığında[ENTER]sana şunu veriyorum: İhtişamlı Anaboz Sandık (%s pirinç patl.) ",harvest_rice_count, harvest_item_name))
			say("Bu eşyayı almak ister misin? ")

			local item_get_set = select("Evet, isterim! ", "İptal ")

			if item_get_set == 2 then
				return
			end -- end of if

			-- System to hand out the items
			if harvest_item_set == 1 then
				pc.setqf("rice_count", rice_count - harvest_rice_count)	
				pc.give_item2("50109", 1)
			elseif harvest_item_set == 2 then
				pc.setqf("rice_count", rice_count - harvest_rice_count)	
				pc.give_item2("50110", 1)
			elseif harvest_item_set == 3 then
				pc.setqf("rice_count", rice_count - harvest_rice_count)	
				pc.give_item2("72728", 1)
			elseif harvest_item_set == 4 then
				pc.setqf("rice_count", rice_count - harvest_rice_count)	
				pc.give_item2("50111", 1)
			elseif harvest_item_set_2 == 1 then
				pc.setqf("rice_count", rice_count - harvest_rice_count)
				pc.give_item2("50112", 1)
			elseif harvest_item_set_2 == 2 then
				pc.setqf("rice_count", rice_count - harvest_rice_count)
				pc.give_item2("50113", 1)
			elseif harvest_item_set_2 == 3 then
				pc.setqf("rice_count", rice_count - harvest_rice_count)	
				pc.give_item2("72724", 1)
			elseif harvest_item_set_2 == 4 then
				pc.setqf("rice_count", rice_count - harvest_rice_count)	
				pc.give_item2("50114", 1)
			elseif harvest_item_set_2 == 5 then
				pc.setqf("rice_count", rice_count - harvest_rice_count)	
				pc.give_item2("50115", 1)
			end -- end of if

			say_reward(string.format("%s aldın. ",harvest_item_name))
			say("Eğer, daha fazla Prinç Patlağı getirmek[ENTER]istersen beni burada bulabilirsin. ")
			
		end -- end of when
		
		when 20086.chat."Yağmacıları kovala! " with game.get_event_flag("harvest_festival")>0 begin
			target.delete("__TARGET__")
			local quest_count = pc.getqf("harvest_festival_count")
			local quest_complete = pc.getqf("harvest_festival_mob_complete")

			-- When you finished all the quests
			if quest_count >= 10 then
				say_title("Handu-Up: ")
				say("Bizi büyük sıkıntıdan kurtardın! ")
				return
			end -- end of if

			-- If you came back for this dish
			if quest_complete == 1 then
				pc.setqf("harvest_festival_count", quest_count + 1)
				pc.setqf("harvest_festival_mob_complete", 0)
				pc.setqf("mob_kill1_count", 0)
				pc.setqf("mob_kill2_count", 0)
				
				local quest_count = pc.getqf("harvest_festival_count")
				
				say_title("Handu-Up: ")
				say("Teşekkür ederim. ")
				wait()

				-- Getting the Glutinous Rice
				local rice_count = pc.getqf("rice_count")
				local cake_count = pc.getqf("cake_count")
				pc.setqf("rice_count", rice_count + cake_count)
				pc.setqf("cake_count", 0)
				
				say_title("Handu-Up: ")
				say(string.format("Zaten %s Pirinç Patlağı ele geçirdin! ", cake_count))
				wait()
				say_title("Handu-Up: ")
				say("Karşılığında bir ödül alacaksın tabiki. ")
				wait()

				if quest_count >= 10 then
					char_log(0, "EVENT_RECORD", "harvest_festival clear")
					say_title("Handu-Up: ")
					say("Yağmacıları kovalayıp stoklarımızı bize gerigetirdiğin[ENTER]için sana minnettarız![ENTER]Sana çok borçluyuz.[ENTERSen olmasaydın büyük sıkıntılar yaşayacaktık! ")

					return
				else
					say_title("Handu-Up: ")
					say("Stoklarımızın hala büyük kısmı eksik.[ENTER]Bize yardım etmeye devam edecek misin? ")
				end -- end of if

				local replay_set = select("Elbette! ", "Daha sonra yardım ederim. ")

				if replay_set == 2 then
					return
				end	-- end of if

				local current_count = quest_count + 1

				say_title("Handu-Up: ")
				say("Seni burada bekleyeceğim. ")
				say(string.format("Görev: %s[ENTER]Bu görevi toplam on kere kabul edebilirsin.", current_count))
				
				set_state(mob_kill1)

				return
			end -- end of if

			-- If you stop and do the quest again
			if quest_count > 0 then
				say_title("Handu-Up: ")
				say(string.format("Görev: %s[ENTER]Bu görevi toplam on kere kabul edebilirsin.", quest_count))

				local replay_set = select("Size yardım edeceğim! ","Malesef yardım edemeyeceğim. ")

				if replay_set == 2 then
					return
				end	-- end of if

				local current_count = quest_count + 1
				say_title("Handu-Up: ")
				say(string.format("Cesur bir savaşçısın. Devam et![ENTER]Görev: %s[ENTER]Bu görevi toplam on kere kabul edebilirsin.", current_count))
				set_state(mob_kill1)

				return
			end -- end of if

			-- here we must put the different mobnames
			if pc.get_level() >= 81 then
				pc.setqf("mob1_info", 6)
				mob1_name = "Cehennem Köpeği "
			elseif pc.get_level() >= 61 then	
				pc.setqf("mob1_info", 5)
				mob1_name = "Alev Hayaleti "
			elseif pc.get_level() >= 41 then	
				pc.setqf("mob1_info", 4)
				mob1_name = "Karanlık Fanatik "
			elseif pc.get_level() >= 21 then	
				pc.setqf("mob1_info", 3)
				mob1_name = "Yabani Piyade "
			elseif pc.get_level() >= 11 then	
				pc.setqf("mob1_info", 2)
				mob1_name = "Beyaz Yeminli Asker "
			elseif pc.get_level() <= 10 then	
				pc.setqf("mob1_info", 1)
				mob1_name = "Alfa Mavi Kurt "
			end -- end of if
			
					
			-- When you start for the 1st time
			say_title("Handu-Up: ")
			say("Bize yardım et lütfen! ")
			say(string.format("Köyümüz %s tarafından baskına uğradı![ENTER]Hırsızlar tüm pirinç patlağı stoklarımızı çaldı.[ENTER]Çaresiz kaldık! Şimdi ne yer ne içeriz?[ENTER]Yağmacıları kovala ve ellerinden ganimetleri al.[ENTER]Seni bolca mükafatlandıracağız. ", mob1_name))

			local main_set = select("Size yardım edeceğim! ","Malesef yardım edemeyeceğim. ")
			if main_set == 2 then
				say_title("Handu-Up: ")
				say("Yardım etmezsen aç kalacağız. Fikrini[ENTER]değitirdiğinde beni burada bulabilirsin. ")
				return
			end -- end of if

			local current_count = quest_count + 1

			say_title("Handu-Up: ")
			say(string.format("Cesur bir savaşçısın. Devam et![ENTER]Görev: %s[ENTER]Bu görevi toplam on kere kabul edebilirsin.", current_count))
			set_state(mob_kill1)

		end -- end of when
	end -- end of state

	
	state mob_kill1 begin
		when letter begin
			send_letter("Yağmacıları kovala! ")
			q.set_counter("Kalan Sayı ", 30 -  pc.getqf("mob_kill1_count"))
		end -- end of when
		
		when login with game.get_event_flag("harvest_festival") == 0 begin
			harvest_festival.reset_flags()
		end
		
		when info or button begin
			local mob1_info = pc.getqf("mob1_info")
			if mob1_info == 6 then
				mob1_name = "Cehennem Köpeği "
			elseif mob1_info == 5 then	
				mob1_name = "Alev Hayaleti "
			elseif mob1_info == 4 then	
				mob1_name = "Karanlık Fanatik "
		   elseif mob1_info == 3 then	
				mob1_name = "Yabani Piyade "
			elseif mob1_info == 2 then	
				mob1_name = "Beyaz Yeminli Asker "
			elseif mob1_info == 1 then	
				mob1_name = "Alfa Mavi Kurt "
			end -- end of ifa

			say_title("Yağmacıları kovala! ") 
			say(string.format("30 %s öldürdükten sonra Handu-Up'a geri dön. ", mob1_name))

			local gogo_set = select("Elbette! ","Pes ediyorum. ")

		   if gogo_set == 2 then
				clear_letter()
				set_state(information)
			end -- end of if

		end -- end of when

		when 20086.chat."Yağmacı kovalamacası " with game.get_event_flag("harvest_festival")>0 begin
			local kill_count = pc.getqf("mob_kill1_count")
			if kill_count < 30 then
				say_title("Handu-Up: ")
				say("Bize stoklarımızı geri getir lütfen! ")
			else
				say_title("Handu-Up: ")
				say("Sen gittikten sonra köyümüz yine yağmalandı! ")
				local mob2_name = ""
				if pc.get_level() >= 91 then
					pc.setqf("mob2_info", 8)
					mob2_name = "Cehennem Deniz Anası "
				elseif pc.get_level() >= 81 then
					pc.setqf("mob2_info", 7)
					mob2_name = "Zombi "
				elseif pc.get_level() >= 71 then
					pc.setqf("mob2_info", 6)
					mob2_name = "Alev Savaşçı "
				elseif pc.get_level() >= 61 then	
					pc.setqf("mob2_info", 5)
					mob2_name = "Kurbağa Generali "
				elseif pc.get_level() >= 51 then	
					pc.setqf("mob2_info", 4)
					mob2_name = "Karanlık Haberci "
				elseif pc.get_level() >= 41 then	
					pc.setqf("mob2_info", 3)
					mob2_name = "Mistik Arahan Savaşçısı "
				elseif pc.get_level() >= 31 then	
					pc.setqf("mob2_info", 2)
					mob2_name = "Yabani Uşak "
				elseif pc.get_level() >= 20 then	
					pc.setqf("mob2_info", 1)
					mob2_name = "Kahverengi Ayı "
				elseif pc.get_level() >= 10 then
					pc.setqf("mob2_info", 9)
					mob2_name = "Ayı "
				else
					pc.setqf("mob2_info", 10)
					mob2_name = "Kırmızı Yaban Domuzu "
				end -- end of if

				say(string.format("Bu sefer %s tarafından baskına uğradık.[ENTER]Bizi ortada bırakma şimdi lütfen! ", mob2_name))
				local sub_set = select("Bana güvenebilirsiniz! ","Ödülümü istiyorum. ")
				
				if sub_set == 2	then
					say_title("Handu-Up: ")
					say("Yardımların için teşekkür ederiz. Şimdi[ENTER]Wonda-Rim'e git ve ele geçirilen pirinç[ENTER]patlaklarının karşılığında ödülünü al. ")
					set_state(information)
					return
				end
				say_title("Handu-Up: ")
				say(string.format("%s için ava git![ENTER]Ama bu tehlikeli yalnız başına gitme.[ENTER]Grup olarak giderseniz düşmanı yenebilirsiniz. ", mob2_name))
				set_state(mob_kill2)
			end
		end	-- end of when
		
		when 20087.click with game.get_event_flag("harvest_festival")>0 begin
			say_title("Wonda-Rim: ")
			say("Yağmacıları kovala ve ellerinden ganimetleri al.[ENTER]Seni bolca mükafatlandıracağız! ")
		end	-- end of when
		
		when kill begin
			if npc.get_race() == 2502 and  pc.getf("harvest_festival","mob1_info") == 6 then
				harvest_festival.kill_action(2502,1)		
			end
		end
		
		when kill begin
			if npc.get_race() == 2202 and pc.getf("harvest_festival","mob1_info") == 5 then
				harvest_festival.kill_action(2202,1)		
			end
		end
		
		when kill begin
			if npc.get_race() == 701 and  pc.getf("harvest_festival","mob1_info") == 4 then
				harvest_festival.kill_action(701,1)		
			end
		end
		
		when kill begin
			if npc.get_race() == 501 and  pc.getf("harvest_festival","mob1_info") == 3 then
				harvest_festival.kill_action(501,1)		
			end
		end
		
		when kill begin
			if npc.get_race() == 301 and pc.getf("harvest_festival","mob1_info") == 2 then
				harvest_festival.kill_action(301,1)
			end
		end
		
		when kill begin
			if npc.get_race() == 105 and  pc.getf("harvest_festival","mob1_info") == 1 then
				harvest_festival.kill_action(105,1)		
			end
		end
	end

	state mob_kill2 begin

		when letter begin
			send_letter("Yağmacıları kovala! ")
			q.set_counter("Kalan Sayı ", 60 -  pc.getqf("mob_kill2_count"))
		end -- end of when
		
		when login with game.get_event_flag("harvest_festival") == 0 begin
			harvest_festival.reset_flags()
		end
	  
		when info or button begin
			local mob2_info = pc.getqf("mob2_info")
			if mob2_info == 10 then
				mob2_name = "Kırmızı Yaban Domuzu "
			elseif mob2_info == 9 then
				mob2_name = "Ayı "
			elseif mob2_info == 8 then
				mob2_name = "Cehennem Deniz Anası "
			elseif mob2_info == 7 then
				mob2_name = "Zombi "
			elseif mob2_info == 6 then
				mob2_name = "Alev Savaşçı "
			elseif mob2_info == 5 then
				mob2_name = "Kurbağa Generali "
			elseif mob2_info == 4 then
				mob2_name = "Karanlık Haberci "
			elseif mob2_info == 3 then
				mob2_name = "Mistik Arahan Savaşçısı "
			elseif mob2_info == 2 then
				mob2_name = "Yabani Uşak "
			elseif mob2_info == 1 then
				mob2_name = "Kahverengi Ayı "
			end -- end of if

			say_title("Yağmacıları kovala! ")
			say(string.format("60 %s öldür ve Handu-Up'a geri dön. ",mob2_name))

			if party.is_party() then
				say("Yağmacıları kovalaması için birilerini[ENTER]arıyoruz. ")
			else
				say("Yağmacıları kovalamak için bir grup[ENTER]oluşturmalısın. Birlikte daha çabuk halledersiniz. ")
			end -- end of if

			local gogo_set = select("Size yardım edeceğim! ","Pes ediyorum. ")

			if gogo_set == 2 then
				clear_letter()
				set_state(information)
			end -- end of if

		end -- end of when
	 
	 
		when 20086.chat."Prinç Patlağı değiş tokuşu " with game.get_event_flag("harvest_festival")>0 begin

			local quest_count = pc.getqf("harvest_festival_count")
			local quest_complete = pc.getqf("harvest_festival_mob_complete")

			-- When you have finished all the quests
			if quest_count >= 10 then
				say_title("Handu-Up: ")
				say("Yağmacıları kovalayıp stoklarımızı bize geri[ENTER]getirdiğin için sana minnettarız![ENTER]Sana çok borçluyuz.[ENTERSen olmasaydın büyük sıkıntılar yaşayacaktık! ")
				set_state(information)
				return
			end -- end of if

			-- If you come back for the dish
			if quest_complete == 1 then
				pc.setqf("harvest_festival_count", quest_count + 1)
				pc.setqf("harvest_festival_mob_complete", 0)
				pc.setqf("mob_kill1_count", 0)
				pc.setqf("mob_kill2_count", 0)
				
				local quest_count = pc.getqf("harvest_festival_count")
				
				say_title("Handu-Up: ")
				say("Harika! ")
				wait()

				-- Getting Glutinous Rice
				local rice_count = pc.getqf("rice_count")
				local cake_count = pc.getqf("cake_count")
				pc.setqf("rice_count", rice_count + cake_count)
				pc.setqf("cake_count", 0)
				
				say_title("Handu-Up: ")
				say(string.format("Zaten %s Pirinç Patlağı ele geçirdin! ",rice_count))
				wait()
				
				if quest_count >= 10 then
					char_log(0, "EVENT_RECORD", "harvest_festival clear")
					say_title("Handu-Up: ")
					say("Yağmacıları kovalayıp stoklarımızı bize geri[ENTER]getirdiğin için sana minnettarız![ENTER]Sana çok borçluyuz.[ENTERSen olmasaydın büyük sıkıntılar yaşayacaktık! ")
					set_state(information)
					return
				else
					say_title("Handu-Up: ")
					say("Stoklarımızın hala büyük kısmı eksik.[ENTER]Bize yardım etmeye devam edecek misin? ")
				end -- end of if

				local replay_set = select("Size yardım edeceğim! ","Malesef yardım edemeyeceğim. ")

				if replay_set == 2 then
					set_state(information)
					return
				end	-- end of if

				local current_count = quest_count + 1

				say_title("Handu-Up: ")
				say(string.format("Cesur bir savaşçısın. Devam et![ENTER]Görev: %s[ENTER]Bu görevi toplam on kere kabul edebilirsin.",current_count))
				set_state(mob_kill1)
				return
			end -- end of if
		end -- end of when
		
		when 20087.click with game.get_event_flag("harvest_festival")>0 begin
			say("Wonda-Rim: ")
			say("Yardım etmezsen aç kalacağız.")
		end -- end of when

		when kill begin
			if npc.get_race() == 108 and  pc.getf("harvest_festival","mob2_info") == 10 and party.is_party() then
				harvest_festival.kill_action(108,2)
			end
		end

		when kill begin
			if npc.get_race() == 109 and  pc.getf("harvest_festival","mob2_info") == 9 and party.is_party() then
				harvest_festival.kill_action(109,2)
			end
		end

		when kill begin
			if npc.get_race() == 113 and  pc.getf("harvest_festival","mob2_info") == 1 and party.is_party() then
				harvest_festival.kill_action(113,2)
			end
		end

		when kill begin
			if npc.get_race() == 502 and pc.getf("harvest_festival","mob2_info") == 2  and party.is_party() then 
				harvest_festival.kill_action(502,2)
			end
		end	

		when kill begin
			if npc.get_race() == 703 and  pc.getf("harvest_festival","mob2_info") == 3 and party.is_party()  then
				harvest_festival.kill_action(703,2)
			end
		end

		when kill begin
			if npc.get_race() == 707 and  pc.getf("harvest_festival","mob2_info") == 4 and party.is_party()  then
				harvest_festival.kill_action(707,2)
			end
		end
	
		when kill begin
			if npc.get_race() == 1305 and pc.getf("harvest_festival","mob2_info") == 5 and party.is_party()  then
				harvest_festival.kill_action(1305,2)
			end
		end

		when kill begin
			if npc.get_race() == 2205 and pc.getf("harvest_festival","mob2_info") == 6  and party.is_party() then
				harvest_festival.kill_action(2205,2)
			end
		end

		when kill begin
			if npc.get_race() == 2501 and  pc.getf("harvest_festival","mob2_info") == 7  and party.is_party() then
				harvest_festival.kill_action(2501,2)
			end
		end

		when kill begin
			if npc.get_race() == 2503 and  pc.getf("harvest_festival","mob2_info") == 8  and party.is_party() then
				harvest_festival.kill_action(2503,2)
			end
		end
	end 
end 

