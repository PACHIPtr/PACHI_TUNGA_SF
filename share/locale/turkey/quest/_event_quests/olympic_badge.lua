--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest olympic_badge begin
	state start begin
		function has_it()
			local a = pc.count_item(30265)
			local b = pc.count_item(30266)
			local c = pc.count_item(30267)
			local d = pc.count_item(30268)
			local e = pc.count_item(30269)

			local perfect = a > 0 and b > 0 and  c > 0 and d > 0 and e > 0 
			local notyet = a >0 or b > 0 or  c > 0 or d > 0 or  e > 0 
			if perfect then 
				return 1
			elseif notyet  then
				return 0
			end
		end
		when login with game.get_event_flag("ex_olympic_drop") >= 1  begin
			set_state(information)
		end
	end

	state information begin
		when letter with pc.level>=30 and game.get_event_flag("ex_olympic_drop") >= 1 begin
			local v = find_npc_by_vnum(20017)
			if 0 != v then
				target.vid("__TARGET__", v, "")
			end
			send_letter("Kahramanlýk Madalyasý ")
		end

		when info or button with pc.level>=30 and game.get_event_flag("ex_olympic_drop") >= 1 begin
			say("")
			say("Yu-Hwan'ýn sana ihtiyacý var.[ENTER]Bir an önce onun yanýna gitmelisin.[ENTER]Onu 2.köyde bulabilirsin. ")
			say("")
		end
		when __TARGET__.target.click or 20017.chat."Kahramanlýk Madalyasý "	with  game.get_event_flag("ex_olympic_drop") >= 1 and pc.level>=30 begin
			target.delete("__TARGET__")

			say("")
			say_title(mob_name(20017))
			say(" Yardým! ")
			say(" Sefil hýrsýzlar...[ENTER]Dün gece benimle kaldýlar.[ENTER]Kahramanlýk Madalyasýný çaldýlar.O madalya imparatorluk [ENTER]savaþýnda büyük babama zafer ödülü[ENTER]olarak verilmiþti. [ENTER]Ama þimdi o madalya çalýndý ve[ENTER]hýrsýzlar kayýp oldu! ")
			wait()
			say("")
			say_title(mob_name(20017))
			say("O madalya ailem için paha biçilmez![ENTER]ne yapmalýyým? ")
			say(" Bana yardým edecek misin? ")
			local s = select(locale.yes, locale.no)
			if 2 == s then
				say(" Þimdi bana bir daha hayýr dersen[ENTER]sana bir daha yardým teklifinde[ENTER]bulunmayacaðým.Emin misin? ")
				local a = select(locale.yes, locale.no)
				if 2 == a then
					say_title(mob_name(20017))
					say("Zamanýn yok anlýyorum! Daha uygun bir zamanda[ENTER]gel.Ama beni çok bekletme! ")
					say("")
					return
				end
				say("")
				say_title(mob_name(20017))
				say("Ne yazýk ki çok geç! Artýk bana yardým edemezsin.[ENTER]Sana izlediðin yolda baþarýlar! ")
				set_state(__GIVEUP__)
				return
			end	
			
			say("")
			say_title(mob_name(20017))
			say("Beni soyan canavarlarý avla! Ama[ENTER] çok dikkatli olmalýsýn.Onlar çok güçlü.[ENTER]30.Seviyeden sonra bana Kahramanlýk Madalyasý[ENTER]getirir isen, seni ödüllendireceðim. ")
			set_state(step1)
		end	
	end

	state step1 begin
		when letter with pc.level>=30 begin
			send_letter("Kahramanlýk Madalyasý ")
		end

		when info or button with pc.level>=30 begin
			say("")
			say("Kahramanlýk Madalyasý kayboldu.[ENTER]Bul ve Yu-Hwan'a geri dön. ")
			say("")
		end
		
		when login or enter with game.get_event_flag("ex_olympic_drop") == 0 begin
			test = pc.getqf("medal_done")
			if test != 0 then
				pc.setqf("medal_done", 0)
			end
		end
		
		when 20017.chat."Kahramanlýk Madalyasý " with  game.get_event_flag("ex_olympic_drop") >= 1 and pc.level>=30 begin
			say_title(mob_name(20017))
			say("Sonunda! Geri döndün![ENTER]Kahramanlýk Madalyasý'ný bulduðunu söyle bana.")
			wait()
			if  count_item_range(30265,30269)== 0  then
				say("Oh, hayýr! Elin boþ mu geldin?[ENTER]Kendimi umutsuz hissetmeye baþladým![ENTER]Sen bile bana yardýmcý olamazsan[ENTER]derdime kim derman olur? Lütfen[ENTER]git ve onu benim için ara! ")
				return
			elseif olympic_badge.has_it() ==0  then
				say("Kahramanlýk Madalya'mý buldun![ENTER]Ama bekle... bu sadece bir parçasý.[ENTER]Beþ bölüme ayrýlmýþ olan parçalarýndan[ENTER]eksikler var! Bana tüm[ENTER]parçalarý getir lütfen! [ENTER]Belki sonra yeniden bir madalya[ENTER]oluþturabiliriz! ")
				return
			else 
				say_title(mob_name(20017))
				say("Bu berbat suçlular benim deðerli[ENTER]eþyamý çaldýlar![ENTER]Ama sen bana bütün parçalarý getirerek[ENTER]yeniden bir Madalya yapmama olanak[ENTER]saðlýyorsun.Biraz bekle tek tek[ENTER]parçalarý birleþtiril madalya yapmayý[ENTER]deneyeceðim...")
				wait()	
				pc.remove_item(30265)
				pc.remove_item(30266) 
				pc.remove_item(30267) 
				pc.remove_item(30268) 
				pc.remove_item(30269) 
				if pc.getqf ("medal_done") == 0 then
					say_title(mob_name(20017))
					say("Bir milyon kere teþekkür[ENTER]Bana yeniden madalya yapmam için[ENTER]bütün parçalarý getirdin.[ENTER]Ve madalyamý yaptým.Teþekkürler[ENTER]savaþçý! Ýþte ödülün. ")
					-- show items
					say_item_vnum(71158)
					--say_item_vnum(71159)
					
					pc.give_item2(71158)
					pc.setqf("medal_done", 1)
					return
				end   --if
				
				say_title(mob_name(20017))
				say("Bir milyon kere teþekkür[ENTER]Bana yeniden madalya yapmam için[ENTER]bütün parçalarý getirdin.[ENTER]Ve madalyamý yaptým.Teþekkürler[ENTER]savaþçý! Ýþte ödülün. ")
				say_item_vnum(50249)
				pc.give_item2(50249)
			end    --if 

		end	   --when 

		end   --state

	state __GIVEUP__ begin
	end
end  --quest
