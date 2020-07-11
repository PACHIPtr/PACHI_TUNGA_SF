--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest meleylair begin
	state start begin
		when 20419.chat."Sung Mahi'nin Ejderhalarý mý? " with pc.get_map_index() == MeleyLair.GetSubMapIndex() begin
			say_title(mob_name(20419))
			say("")
			say("Korkarým çoðu yarýn hayatta olmayacak. Tüm ")
			say("iblislerin tanrýsý Sung Mahi tekrar uyandý.. ve ")
			say("onunla birlikte üç ejderhasý da. Ýnsanlarýn ruhlarýyla ")
			say("besleniyolar. Yeterince yedikten sonra Sung Mahi'nin ")
			say("gücüne güç katýyolar. Bu da.. son demek. Hepimiz için. ")
			say("Buna izin veremeyiz!. ")
			say("Ama cesur bir savaþçýdan çok daha fazlasýna ihtiyaç var. ")
			say("Sadece loncalar bu pullu hayvanla yüzleþebilir. Yardým edebilir misin? ")
			say("Senin kadar cesur maceracýlar tanýyor musun? ")
			wait()
			say_title(mob_name(20419))
			say("")
			say("Ama seni uyarýyorum: ")
			say("Ejderhalara karþý çýkanlarla ilgili ürkütücü hikayeler ")
			say("anlatýlýyor. Asla geri dönmediler... ")
		end
		
		when 20419.chat."Sung Mahi'nin Ejderhalarý ile savaþ! " with pc.get_map_index() == MeleyLair.GetSubMapIndex() and pc.is_guild_master() begin
			say_title(mob_name(20419))
			say("")
			local registered, channel = MeleyLair.IsRegistered()
			if registered then
				say(string.format("Zaten loncanýz kayýtlý.[ENTER] CH %d.[ENTER]'den giriþ yapabilirsiniz. ", channel))
				return
			end
					
			local requirment1, requirment2 = MeleyLair.GetRequirments()
			say("Sung Mahi'nin Ejderhalarý savaþmak için loncanýz kayýt ")
			say("olmalýdýr. ")
			say("")
			say("Kayýt olmak için þunlar gerekir: ")
			say(string.format("- Loncanýz en az %d seviyede olmalýdýr. ", requirment1))
			say(string.format("- Loncanýz en az %d lonca puaný olmalýdýr. ", requirment2))
			say("")
			say("Kayýt yaptýrmak istiyor musun? ")
			local menu = select("Evet ", "Hayýr ")
			if menu == 1 then
				local result1, result2 = MeleyLair.Register()
				if result1 == 0 then
					setskin(NOWINDOW)
					return
				end
				
				local resultMsg = {
								[1] = "Zaten loncanýz kayýtlý.[ENTER] CH %d.[ENTER]'den giriþ yapabilirsiniz. ",
								[2] = "Baþka loncadasýnýz,[ENTER]daha sonra deneyiniz ",
								[3] = "Loncanýz en az %d seviye olmalýdýr. ",
								[4] = "Loncanýz en az %d lonca puaný olmalýdýr. ",
								[5] = "Lonca kayýtlý deðil, [ENTER]Yetkili kiþilerle görüþün. ",
								[6] = "Ejderhalarý yenerseniz,[ENTER]%d lonca puaný iade edilcektir. ",
								[7] = "Sadece bulunduðunuz loncayý kayýt edebilirsiniz. %s. ",
				}
				
				say_title(mob_name(20419))
				say("")
				if result1 != 2 and result1 != 5 then
					if result1 == 7 then
						local hours = string.format("%02.f", math.floor(result2 / 3600));
						local minutes = string.format("%02.f", math.floor(result2 / 60 - (hours * 60)));
						local seconds = string.format("%02.f", math.floor(result2 - hours * 3600 - minutes * 60));
						local timeConv = string.format(hours..":"..minutes..":"..seconds)
						say(string.format(resultMsg[result1], timeConv))
					else
						say(string.format(resultMsg[result1], result2))
					end
				else
					say(resultMsg[result1])
				end
			else
				setskin(NOWINDOW)
				return
			end
		end
		
		when 20419.chat."GM: ODAYI BUGDAN KURTAR " with pc.is_gm() begin
			local sec = select("Meley Bugdan Kurtar","Kapat")
			if sec == 1 then
				say_title(mob_name(20419))
				say_reward("Meley giriþleri tekrar açýldý.")
				game.get_event_flag("meley_lair_dungeon_status",0)
				game.get_event_flag("meley_lair_dungeon_guild",0)
			end
		end
		
		when 20419.chat."Ýn'e Gir! " with pc.get_map_index() == MeleyLair.GetSubMapIndex() and pc.has_guild() begin
			say_title(mob_name(20419))
			say("")
			say("Ýn'e Girmek istiyor musun? ")
			say("")
			local agree = select("Evet ", "Hayýr ")
			
			if agree == 1 then
				local registered, limit = MeleyLair.Enter()
				if not registered and limit == 0 then
					say_title(mob_name(20419))
					say("")
					if pc.is_guild_master() then
						say("Ýn'e girebilmek için [ENTER]loncanýzý kayýt etmeniz gerekmektedir. ")
					else
						say("Lonca lideriniz in'e kayýt ettirmelidir.[ENTER]Ýn'e eriþemiyorsunuz. ")
					end
					
					return
				elseif pc.get_level() < 75 then
					say_title(mob_name(20419))
					say("")
					say(string.format("Giriþ yapabilmek için seviyeniz %s olmalýdýr.", 75))
					return
				elseif not registered and limit > 0 then
					say_title(mob_name(20419))
					say("")
					say(string.format("Ýn'e Bu CH %d.'den eriþebilirsiniz. ", limit))
					return
				elseif limit == 1 then
					say_title(mob_name(20419))
					say("")
					say(string.format("Þuanda odada %d lonca üyesi mevcut.[ENTER]Ýn'e eriþemiyorsun. ", MeleyLair.GetPartecipantsLimit()))
					return
				elseif limit == 2 then
					say_title(mob_name(20419))
					say("")
					say("Bir Hata Oluþtu. Lütfen Yetkili Kiþilere Ulaþýn. ")
					return
				elseif limit == 3 then
					say_title(mob_name(20419))
					say("")
					say("Zindan Zaten Bitti. ")
					return
				elseif limit == 4 then
					say_title(mob_name(20419))
					say("")
					say("Savaþa katýlmak için; ")
					say("oyun seçeneklerinden lonca seçeneðine alýn. ")
					return
				elseif limit == 5 then
					say_title(mob_name(20419))
					say("")
					say("Senden önce bir lonca giriþ yaptý; ")
					say("Odada þuanda bir ekip bulunuyor.")
					return
				elseif limit == 6 then
					say_title(mob_name(20419))
					say("")
					say(string.format("Giriþ yapabilmek için seviyeniz %s olmalýdýr.", 75))
					return
				elseif limit == 7 then
					say_title(mob_name(20419))
					say("")
					say("Envanterinde geçit bileti bulamadým.")
					say_item_vnum(71201)
					return
				end
				

				
				
				return
			else
				setskin(NOWINDOW)
				return
			end
		end
    end
end