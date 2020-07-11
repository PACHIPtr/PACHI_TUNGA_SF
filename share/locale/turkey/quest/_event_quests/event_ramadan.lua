--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest event_ramadan begin
	state start begin
		function is_running()
			return (game.get_event_flag("ex_ramadan_drop") > 0)    --npc spawn
		end
		function reward_period()
			return (game.get_event_flag("ramazan_etkinlik_bitti") > 0)    --reward menu appear
		end

			
		function setting()
			return {  
			["bread"]	= 30315,
			["candy"]	= 50183,
			["plate"]	= 30316,
			["iftar"]	= 30317,
			["fruit"]	= 30318,
			["box"]	    = 50182,
			["horse1"]	= 71131,
			["horse2"]	= 71132,
			["horse3"]	= 71133,
			["horse4"]	= 71134,
			["historian"]	= 33002,
			["beggar1"]	    = 33003, 
			["beggar2"]	    = 33004,
			["beggar3"]	    = 33005, 
			["beggar4"]	    = 33006, 
			["beggar5"]	    = 33007 
			}
		end
		
		when 33002.chat."Tarihçi mi? " with pc.getf("event_ramadan","first_notice")==0 and event_ramadan.is_running()  begin
			local setting = event_ramadan.setting()
			say_title("Tarihçi: ")
			say("")
			say("Geçmişi olmayanın geleceği olmaz![ENTER]Sana nasıl yardımcı olabilirim? ")
			wait()

			say_pc_name()
			say("Pardon, tanışıyor muyuz? ")
			wait()

			say_title("Tarihçi: ")
			say("Afedersin, kendimi tanıtmayı unutmuşum. Bir[ENTER]tarihçi olarak gezip görmediğim yer yoktur. Eğer[ENTER]tarihle ilgili bir şey bilmek istersen, çekinmeden[ENTER]sorabilirsin. ")
			wait()
			
			say_pc_name()
			say("Seninle tanıştığıma çok memnun oldum. Artık sık sık[ENTER]ziyaretine gelirim. ")
			

			pc.setqf("first_notice",1)
		end  
	
	when 33002.chat."Tatlı yiyelim tatlı konuşalım  " with pc.getf("event_ramadan","first_notice")==1 and event_ramadan.is_running() begin
		
		local setting = event_ramadan.setting()
		if pc.count_item(setting.bread) == 0  then
			say_title("Tarihçi: ")
			say("Tatlı gördün mü hiç?[ENTER]Baklava tatlısı derler.")
			local s=select("Evet, gördüm. " ,"Maalesef. ")
			if 2==s then
				say_title("Tarihçi: ")
				say("Tüh. Neyse, tekrar görüşmek üzere. ")
				return
			end
			say_title("Tarihçi: ")
			say("Demek ne olduğunu biliyorsun?[ENTER]Bu tatlıları istiyorsan,[ENTER]bana on tane simit getirmelisin.  ")
			wait()
			say_title("Tarihçi: ")
			say("10 simit karşılığında sana bir tatlı verebilirim.[ENTER]Anlaştık mı? ")
			local s=select("Seve seve. ","Uğraşamam. ")
			if 2==s then
				say_title("Tarihçi: ")
				say("Tüh. Neyse, tekrar görüşmek üzere. ")
			return
			end
			say_title("Tarihçi: ")
			say("Her 10 simit karşılığında sana bir tane tatlı[ENTER]vereceğim. ")
		elseif	pc.count_item(setting.bread) > 0 and  pc.count_item(setting.bread) < 10  then
			say_title("Tarihçi: ")
			say("10 simit bulduğunda gel. ")
		elseif	pc.count_item(setting.bread) >= 10  then
			say_title("Tarihçi: ")
			say("Ahh harikasın 10 simit buldun demek.[ENTER]İşte sana nefis baklava tatlısı.[ENTER]Afiyet olsun. ")
			wait()	
			say_title("Tarihçi: ")
			say("Her 10 simit karşılığında sana bir tane tatlı[ENTER]vereceğim. ")
			pc.give_item2(setting.candy,1)
			pc.remove_item(setting.bread,10)
		end  --if
		if not event_ramadan.is_running() then
			npc.purge()
		end
	end	  --when
		when 33002.chat."Enfes Simit " with pc.getqf("first_notice")== 1 and event_ramadan.is_running() begin
			local setting = event_ramadan.setting()
			say_title("Tarihçi: ")
			say("Bana getirdiğin simitle karnımı doyurdum.")
			say("Burada o kadar aç dilenci var ki, onlara iftar[ENTER]yemeği versen çok sevap kazanırsın. ")
			say("Tevazu gösterip alçak gönüllü davranmalısın.[ENTER]İftar yemeğini değerli bir tabakta vermen gerekir. ")
			say_item_vnum(setting.plate)
			wait()
			say_title("Tarihçi: ")
			say("Bu nefis simit için ne kadar teşekkür etsem[ENTER]azdır! Çok yardımsever ve merhametli bir insansın.[ENTER]Tuttuğun altın olsun, gönlün neşeyle dolsun!")
		end	

		when 33003.chat." Açım... " or 33004.chat." Açım... " or 33005.chat." Açım... " or 33006.chat." Açım... " or 33007.chat." Açım... " begin
			local setting = event_ramadan.setting()
			if pc.count_item(setting.iftar) == 0  then 
				say_title(" Niyetli Vatandaş: ")
				say("")
				say("Gerçekten çok açım.[ENTER]Bana merhamet et.[ENTER]Lütfen bana iftar yemeği ver, yoksa açlıktan öleceğim. ")

			elseif pc.count_item(setting.iftar) > 0  then 
				
				say_title(" Niyetli Vatandaş: ")
				say("")
				say("Aaa, bu iftar yemeği değil mi? Simitler ne kadar[ENTER]da nefis kokuyor...Karnımı doyurursan ömür boyu sana dua edeceğim.")
				local s = select("Tabi, buyur. ","Malesef veremem. ")
				if 2==s then
					say_title(" Niyetli Vatandaş: ")
					say("")
					say("Çok teşekkür ederim. Hakikaten çok yardımsever ve merhametli bir insanmışsın. ")
					return
				end
				-- reward
				say_title(" Niyetli Vatandaş: ")
				say("")
				say("Bu iyiliğini hiç [ENTER]unutmayacağım. Bütün duaların kabul olsun. ")
				
				pc.give_item2(setting.box,1)
				pc.remove_item(setting.iftar,1)
				end --if

				if not event_ramadan.is_running() then
					npc.purge()
				end
			end	
		
		when 33002.chat." Bereket Meyvesi " with  pc.getf("event_ramadan","fig_exchange")==0 and event_ramadan.is_running() begin
			local setting = event_ramadan.setting()
			if get_time() > pc.getqf("duration") then
				say_title("Tarihçi: ")
				say("")
				say("Senin karnını doyurmadım mı ben bugün?!  ")
				say_reward("Bereket meyvesini 6 saatte bir alabilirsin.")
				pc.give_item2(setting.fruit)
				pc.setqf("duration",get_time()+60*60*6) 			--24h
			else	
				say_title("Tarihçi: ")
				say("Bak sana[ENTER]çok önemli bir şey söyleyeceğim, kulağını dört aç:[ENTER]Sabreden derviş muradına ermiş! ")
			end
		end

		when 33002.chat."Bereket Meyvesi getirdim " with event_ramadan.reward_period() and  pc.getf("event_ramadan","fig_exchange")==0 and event_ramadan.is_running() begin
			local setting = event_ramadan.setting()
			say_title("Tarihçi: ")
			say("")
			say("Bu kadar az mı?[ENTER]Üzgünüm, bu kadar az meyve karşılığında sadece[ENTER]at verebilirim. Daha sabırlı olmalıydın. ")
			wait()
			if  pc.count_item(setting.fruit)>= 5 then		--more than 5
				say_title("Tarihçi: ")
				say("")
				say(string.format("%d tane Bereket Meyvesi var. ", pc.count_item(setting.fruit)))
				say("Hımm, çok fazla değil ama olsun.[ENTER]Karşılığını alacaksın.[ENTER]Beni daha fazla ziyaret etmeliydin. ")
				local s=select("Lütfen, al. ", "Hayır. ")
				if 2==s then 
					say_title("Tarihçi: ")
					say("")
					say("İyice düşün... Ben bir süre burada değilim. ")
					return
				end
				say_title("Tarihçi: ")
				say("")
				say("Cömertliğin için teşekkür ederim! ")
				say_reward("Ödül: Kara Yağız Atı ")
				say("")
				if pc.count_item(setting.fruit)>= 5 and pc.count_item(setting.fruit)<= 10 then
					pc.give_item2(setting.horse1,1)
				elseif pc.count_item(setting.fruit)>= 11 and pc.count_item(setting.fruit)<= 19 then
					pc.give_item2(setting.horse2,1)
				elseif pc.count_item(setting.fruit)>= 20 and pc.count_item(setting.fruit)<= 27 then
					pc.give_item2(setting.horse3,1)
				elseif pc.count_item(setting.fruit)>= 28 then
					pc.give_item2(setting.horse4,1)
				end
				pc.remove_item(setting.fruit,pc.count_item(setting.fruit))
				pc.setqf("fig_exchange",1)
			else										--less  than  5
				say_title("Tarihçi: ")
				say("")
				say("5 taneden az mı? Bununla fazla bir şey yapamam. Belki başka zaman. ")
			end
	end
	when 30316.use begin
		local setting = event_ramadan.setting()
		if pc.count_item(setting.bread) >= 10 then
			pc.remove_item(setting.bread, 10)
			pc.remove_item(setting.plate, 1)
			pc.give_item2(setting.iftar, 1)
		end
	end

	end --state
end		--quest