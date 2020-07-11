--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest training_mount begin
	state start begin
		function check_list(material, pc_level, horse_level)
			if pc.count_item(material) <= 0 then 
				say_title(""..mob_name(20349).."")
				say("Üzgünüm, parþömen olmadan sana yardýmcý olamam. ")
				return 0
			elseif pc.level <  pc_level then
				say_title(""..mob_name(20349).."")
				say("Üzgünüm, sana yardýmcý olamam.[ENTER]Bunun için en azýndan Asker At lisansýna sahip olmalýsýn. ") 
				return 0
			elseif 	horse.get_level()< horse_level then
				say_title(""..mob_name(20349).."")
				say(string.format("Binek geliþtirmek için uygun mühür yok.[ENTER]En azýndan Asker At sahibi olmalýsýn.",horse_level)) 
				return 0
			else
				return 1
			end
		end
		
		function extraTime(typ)
			local n = number (1,100)
			if typ == 1 then
				if n <= 50 then
					return 86400
				elseif n <= 80 then
					return 86400*2
				elseif n <=95 then
					return 86400*3
				else
					return 86400*4
				end
			elseif typ == 2 then
				if n <= 30 then
					return 0
				elseif n <= 70 then
					return 86400
				elseif n <=90 then
					return 86400*2
				else
					return 86400*3
				end
			end
		end
		
		when 20349.chat."Yeni bir bineðe binmek istiyorum! " begin
			say_title(""..mob_name(20349).."")
			say("Þiiþt! Hala at sýrtýnda mý gidiyorsun?[ENTER]Ýnanamýyorum! diye bir þey var, hiç duymadýn mý? ")
			say("")
			wait()
			say_title(""..mob_name(20349).."")
			say("Ehlileþtirilmiþ yaban hayvanlarýný savaþ için[ENTER]eðitebilmen için derin binicilik bilgisine sahip[ENTER]olmalýsýn. Ayrýca þu eþyalar da sana lazým[ENTER]olacak: bineðini yabani binek hayvaný yapmak için[ENTER]evrim parþömeni; bineðini cesur binek hayvaný[ENTER]yapmak için de mutasyon parþömeni gerekir.")
			say("")
			wait()
			say_title(""..mob_name(20349).."")
			say("Daha fazla sorularýn olursa dilediðin zaman[ENTER]yanýma gelebilirsin. Sana elimden geldiði kadar[ENTER]yardýmcý olacaðým. ")
			say("")
			set_state(main_point)
		end
	end

	state main_point begin
		when 20349.chat."Bineðimi geliþtirmek istiyorum! " begin
			say_title(""..mob_name(20349).."")
			say("Demek bineðini geliþtirmek istiyorsun, öyle mi?[ENTER]Nasýl yapýldýðýný biliyor musun?[ENTER]Hadi gel anlatayým. ")
			say("")
			wait()
			say_title(""..mob_name(20349).."")
			say("Bineðini geliþtirip, yabani binek hayvaný[ENTER]yapabilmen için Evrim Parþömeni arayýþýna[ENTER]çýkmalýsýn. Sonra yaban hayvanýný eðitip, cesur[ENTER]binek hayvaný yapabilmek için de Mutasyon[ENTER]Parþömeni'ni bulmalýsýn. Tehlikeli devlerin[ENTER]yaþadýðý çok uzak bir diyarda saklý[ENTER]olduklarý söylenir! ")
			say("")
			wait()
			say_title(""..mob_name(20349).."")
			say("Bana o parþömenlerden birini getirirsen bineðini[ENTER]geliþtireceðim. Getirdin mi yoksa? ")

			local s=select("Evet, bineðimi geliþtirmek istiyorum! ","Hayýr, þimdi deðil. ")

			if 2==s then
				return
			end
			if pc.count_item(50067) >0  or pc.count_item(50068)>0 then 
				say_title(""..mob_name(20349).."")
				say("Ah, parþömenlerden birini getirmiþsin.[ENTER]Hangisini getirdin?[ENTER]Evrim parþömeniyle tecrübesiz yavru[ENTER]hayvanýný savaþ için eðitiyorsun. Sonra da[ENTER]mutasyon parþömeniyle geliþtirip cesur binek[ENTER]hayvaný yapabiliyorsun. ")
			else 
				say_title(""..mob_name(20349).."")
				say("Üzgünüm, parþömen olmadan sana yardýmcý olamam. ")
				return
			end

			local s=select("Evrim Parþömeni bende. ","Mutasyon Parþömeni bende. ","Parþömen getirmedim. ")
			if 1==s  then 
				if training_mount.check_list(50067,25,11) == 1 then 
					say_title(""..mob_name(20349).."")
					say("Ah iþte evrim parþömeni! Binek hayvaný mührüne[ENTER]bir göz atayým. Sahtesiyle beni kandýrmaya[ENTER]çalýþan nicelerini gördüm! Ýlk önce gerçek olup[ENTER]olmadýðýna bakacaðým. Ver bakalým.[ENTER](Mührü, Seyis'in üzerine sürükle.) ")
				end
			elseif 2==s   then
				if training_mount.check_list(50068,45,20) == 1 then 
					say_title(""..mob_name(20349).."")
					say("Ah iþte mutasyon parþömeni! Binek hayvaný mührüne[ENTER]bir göz atayým. Sahtesiyle beni kandýrmaya[ENTER]çalýþan nicelerini gördüm! Ýlk önce gerçek olup[ENTER]olmadýðýna bakacaðým. Ver bakalým.[ENTER]Mührü, Seyis'in üzerine sürükle.) ")
				end
			elseif 3==s then
				say_title(""..mob_name(20349).."")
				say("Parþömen bulduðunda bana geri gel.[ENTER] ")
				say("")
				return
			end
		end --when


		when 20349.take with 
                52001 <= item.vnum and item.vnum <= 52005 or 
                52016 <= item.vnum and item.vnum <= 52020 or 
                52031 <= item.vnum and item.vnum <= 52035 or 
                52046 <= item.vnum and item.vnum <= 52050 or 
                52061 <= item.vnum and item.vnum <= 52065 or 
                52076 <= item.vnum and item.vnum <= 52080 or 
                52091 <= item.vnum and item.vnum <= 52095 or
				52106 <= item.vnum and item.vnum <= 52110 begin
			if pc.is_riding() == true then
				say_title(""..mob_name(20349).."")
				say("Konuþurken bineðinden iner misin lütfen?[ENTER]Boynum tutuldu.[ENTER]Durmadan yukarý bakarak konuþmak zor[ENTER]oluyor. ")
				return
			end
			if count_item_range(38100, 38102) >= 1 then
				say_title(""..mob_name(20349).."")
				say("Bu þifali bitki saf enerji ile güçlü[ENTER]bineðinizim ömrünü bir kaç günlüðüne uzatýr.[ENTER]Senin için ne yapabilirim?[ENTER]Bineðini geliþtirmek ya da süresini uzatmak ister misin? ")
				local choice = select("Uzat ", "Geliþtir ", locale.cancel)
				if choice == 3 then
					return
				elseif choice == 1 then
					local option_select = select("7 Gün ", "15 Gün ", "30 Gün ", locale.cancel)
					local information = {}
					if option_select == 4 then
						return
					end
					if option_select == 1 then
						if pc.count_item(38100) >= 1 then
							information = {86400*7,38100}
						else
							say_title(""..mob_name(20349).."")
							say("Seçtiðin süreye uygun atýþtýrmalýðýn ne[ENTER]yazýk ki yok. Ama bunu dönen paraya[ENTER]týklayarak alabilirsin. ")
							return
						end
					elseif option_select == 2 then
						if pc.count_item(38101) >= 1 then
							information = {86400*15,38101}
						else
							say_title(""..mob_name(20349).."")
							say("Seçtiðin süreye uygun atýþtýrmalýðýn ne[ENTER]yazýk ki yok. Ama bunu dönen paraya[ENTER]týklayarak alabilirsin. ")
							return
						end
					elseif option_select == 3 then
						if pc.count_item(38102) >= 1 then
							information = {86400*30, 38102}
						else
							say_title(""..mob_name(20349).."")
							say("Seçtiðin süreye uygun atýþtýrmalýðýn ne[ENTER]yazýk ki yok. Ama bunu dönen paraya[ENTER]týklayarak alabilirsin. ")
							return
						end
					end
					local remain_time =item.get_socket(0)
					item.set_socket(0,remain_time+information[1])
					pc.remove_item(information[2],1)
					return
				end
			end
			if training_mount.check_list(50067,25,11) == 1 then
				say_title(""..mob_name(20349).."")
				say("Binek hayvaný mührünün ahý gitmiþ vahý kalmýþ.[ENTER]Bekle yenisiyle deðiþtireyim.[ENTER]Buyur alabilirsin.[ENTER]Þimdi yabani binek hayvanýný çaðýrabilirsin.[ENTER]Mutasyon parþömenini keþfeder[ENTER]keþfetmez de yanýma dön.[ENTER]Sana daha güçlü bir binek hayvaný mührü[ENTER]hazýrlayacaðým. ")
				wait()
				say_title(""..mob_name(20349).."")
				say("Bu mühür, dönüþmüþ bineðini çaðýrmaný saðlar.[ENTER]Ona gözün gibi bak, deðeri paha biçilemez! ")
				local remain_time =item.get_socket(0)
				local old_vnum =item.get_vnum()
				local new_vnum =item.get_vnum()+5
				local extra = training_mount.extraTime(1)
				pc.give_item2_select(new_vnum,1)
				item.set_socket(0,remain_time+extra)
				pc.remove_item(old_vnum,1)
				pc.remove_item(50067,1)
			end
		end

		when 20349.take with 52006 <= item.vnum and item.vnum <= 52010 or 
							52021 <= item.vnum and item.vnum <= 52025 or 
							52036 <= item.vnum and item.vnum <= 52040 or 
							52051 <= item.vnum and item.vnum <= 52055 or 
							52081 <= item.vnum and item.vnum <= 52085 or 
							52066 <= item.vnum and item.vnum <= 52070 or
							52096 <= item.vnum and item.vnum <= 52100 or
							52111 <= item.vnum and item.vnum <= 52115 begin
			if pc.is_riding() == true then
				say_title(""..mob_name(20349).."")
				say("Konuþurken bineðinden iner misin lütfen?[ENTER]Boynum tutuldu.[ENTER]Durmadan yukarý bakarak konuþmak zor[ENTER]oluyor. ")
				return
			end
			
			if count_item_range(38100, 38102) >= 1 then
				say_title(""..mob_name(20349).."")
				say("Bu þifali bitki saf enerji ile güçlü[ENTER]bineðinizim ömrünü bir kaç günlüðüne uzatýr.[ENTER]Senin için ne yapabilirim?[ENTER]Bineðini geliþtirmek ya da süresini uzatmak ister misin? ")
				local choice = select ("Uzat ", "Geliþtir ", locale.cancel)
				if choice == 3 then
					return
				elseif choice == 1 then
					local option_select = select("7 Gün ", "15 Gün ", "30 Gün ", locale.cancel)
					local information = {}
					if option_select == 4 then
						return
					end
					if option_select == 1 then
						if pc.count_item(38100) >= 1 then
							information = {86400*7,38100}
						else
							say_title(""..mob_name(20349).."")
							say("Seçtiðin süreye uygun atýþtýrmalýðýn ne[ENTER]yazýk ki yok. Ama bunu dönen paraya[ENTER]týklayarak alabilirsin. ")
							return
						end
					elseif option_select == 2 then
						if pc.count_item(38101) >= 1 then
							information = {86400*15,38101}
						else
							say_title(""..mob_name(20349).."")
							say("Seçtiðin süreye uygun atýþtýrmalýðýn ne[ENTER]yazýk ki yok. Ama bunu dönen paraya[ENTER]týklayarak alabilirsin. ")
							return
						end
					elseif option_select == 3 then
						if pc.count_item(38102) >= 1 then
							information = {86400*30, 38102}
						else
							say_title(""..mob_name(20349).."")
							say("Seçtiðin süreye uygun atýþtýrmalýðýn ne[ENTER]yazýk ki yok. Ama bunu dönen paraya[ENTER]týklayarak alabilirsin. ")
							return
						end
					end
					
					local remain_time =item.get_socket(0)
					item.set_socket(0,remain_time+information[1])
					pc.remove_item(information[2],1)
					return
				end
			end
			if training_mount.check_list(50068,45,20) == 1 then
				say_title(""..mob_name(20349).."")
				say("Ýþte sana bir müddet önce verdiðim binek hayvaný[ENTER]mührü. Ama bayaðý kullanýlmýþ. Onu bana geri ver,[ENTER]sana daha güçlü bir binek hayvaný mührü[ENTER]hazýrlayacaðým. ")
				wait()
				say_title(""..mob_name(20349).."")
				say("Bu mühür, dönüþmüþ bineðini çaðýrmaný saðlar.[ENTER]Ona gözün gibi bak, deðeri paha biçilemez! ")
				local remain_time =item.get_socket(0)
				local old_vnum =item.get_vnum()
				local new_vnum =item.get_vnum()+5
				local extra = training_mount.extraTime(1)
				pc.give_item2_select(new_vnum,1)
				item.set_socket(0,remain_time+extra)
				pc.remove_item (old_vnum,1)
				pc.remove_item (50068,1)
				
			end
		end

		when 20349.take with 52011 <= item.vnum and item.vnum <= 52015 or 
                            52026 <= item.vnum and item.vnum <= 52031 or
                            52041 <= item.vnum and item.vnum <= 52045 or
                            52056 <= item.vnum and item.vnum <= 52061 or
                            52086 <= item.vnum and item.vnum <= 52091 or
                            52071 <= item.vnum and item.vnum <= 52075 or
                            52101 <= item.vnum and item.vnum <= 52105 or
							52116 <= item.vnum and item.vnum <= 52120
                            begin
			if pc.is_riding() == true then
				say_title(""..mob_name(20349).."")
				say("Konuþurken bineðinden iner misin lütfen?[ENTER]Boynum tutuldu.[ENTER]Durmadan yukarý bakarak konuþmak zor[ENTER]oluyor. ")
				return
			end
			
			say_title(""..mob_name(20349).."")
			say("Mutasyon Parþömeni büyük bir güce sahiptir. Cesur[ENTER]bineðinin ömrünü dört güne kadar uzatabilir. Bir[ENTER]deneme sana 200.000 Yang'a mal olur. Ancak bunun[ENTER]için bineðin çok yorulmuþ olmalý ve ömrü bir[ENTER]günden daha az olmalý. Ayrýca tabi biraz da[ENTER]þansýn olmalý! Eðer uzatma baþarýsýz olursa ona[ENTER]güçlü bir atýþtýrmalýk yedir. Bunu dönen paranýn[ENTER]arkasýnda bulabilirsin. ")
			local choice = select ("Uzat ", locale.cancel)
			if choice == 2 then
				return
			elseif choice == 1 then
				local option_select = select("7 Gün ", "15 Gün ", "30 Gün ", "Mutasyon Parþömeni + 200k Yang ", locale.cancel)
				local information = {}
				if option_select == 5 then
					return
				end
				if option_select == 1 then
					if pc.count_item(38100) >= 1 then
						information = {86400*7,38100}
					else
						say_title(""..mob_name(20349).."")
						say("Seçtiðin süreye uygun atýþtýrmalýðýn ne[ENTER]yazýk ki yok. Ama bunu dönen paraya[ENTER]týklayarak alabilirsin. ")
						return
					end
				elseif option_select == 2 then
					if pc.count_item(38101) >= 1 then
						information = {86400*15,38101}
					else
						say_title(""..mob_name(20349).."")
						say("Seçtiðin süreye uygun atýþtýrmalýðýn ne[ENTER]yazýk ki yok. Ama bunu dönen paraya[ENTER]týklayarak alabilirsin. ")
						return
					end
				elseif option_select == 3 then
					if pc.count_item(38102) >= 1 then
						information = {86400*30, 38102}
					else
						say_title(""..mob_name(20349).."")
						say("Seçtiðin süreye uygun atýþtýrmalýðýn ne[ENTER]yazýk ki yok. Ama bunu dönen paraya[ENTER]týklayarak alabilirsin. ")
						return
					end
				elseif option_select == 4 then
					local remain_time = item.get_socket(0)
					local run_time = remain_time-get_time()
					if (remain_time-get_time()) > 86400 then
						say_title(""..mob_name(20349).."")
						say("Güçlü bineðin henüz çok güçlü.[ENTER]Mutasyon parþömeni burada hiçbir etki saðlamaz.[ENTER]Bir günden az ömrü kaldýðýnda yine gel. ")
						if  is_test_server()  then
							item.set_socket(0, get_time() + 86460)
						end
						return
					end
					if pc.count_item(50068) >= 1 and pc.get_gold() >= 200000 then
						information = {training_mount.extraTime(2),50068}
						pc.changegold(-200000)
					else
						say_title(""..mob_name(20349).."")
						say("Ne yazýk ki sana yardým edemem.[ENTER]Ya Mutasyon Parþömeni yanýnda deðil[ENTER]ya da 200.000 Yang'ýn yok. ")
						return
					end
				end
				
				local remain_time = item.get_socket(0)
				item.set_socket(0,remain_time+information[1])
				pc.remove_item(information[2],1)
				return
			end
			
		end
		
		when 20349.chat."At becerileri kullanabilir miyim? "  begin
			say_title(""..mob_name(20349).."")
			say("At becerilerini kullanmak istiyorsun, öyle mi?[ENTER]Senin de isteklerin bitmiyor Gerçi onu ben bile[ENTER]sterim... Neyse, onu öðrenebilmek için bir takým[ENTER]kurallara uymalýsýn. ")
			wait()
			say_title(""..mob_name(20349).."")
			say("Ýlk olarak at binmeyi öðrenmelisin, aksi takdirde[ENTER]senin için çok tehlikeli olabilir. Bunun dýþýnda,[ENTER]savaþta kullanmaya elveriþli bir bineðin olmasý[ENTER]gerekir. Ancak o zaman at becerilerini kullanacak[ENTER]durumunda olur. ")
			wait()
			say_title(""..mob_name(20349).."")
			say("Bineðin, at becerilerini kullanabilecek durumda[ENTER]mý? Deðilse eðitime izin veremeyeceðim. Ne[ENTER]yapmayý düþünüyorsun? ")
			local s=select(locale.yes,locale.no)
			if 2==s then
				return
			end
			say_title(""..mob_name(20349).."")
			say("Anlýyorum. Þimdi de gücünü ve binicilik bilgini[ENTER]tahmin edeceðim. Bekle bir saniye. ")
			wait()
			if pc.count_item(50068) >0  then
				if horse.get_level()>1 then 
					say_title(""..mob_name(20349).."")
					say("Korkarým bineðin henüz hazýr deðil. ")
					wait()
					say("Savaþmaya daha hevesli olduðunda tekrar gel.")
				else 
					say_title(""..mob_name(20349).."")
					say("Ata binmek çok tehlikelidir.[ENTER]Ata binmeyi öðrendiðin zaman gel.")
					return
				end
			else
				say_title(""..mob_name(20349).."")
				say("Henüz hazýr deðilsin.[ENTER]Hazýr olduðunda tekrar gel. ")
				return
			end	
		end
	end
end
