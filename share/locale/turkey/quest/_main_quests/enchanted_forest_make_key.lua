--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest enchanted_forest_make_key begin
	state start begin
		when letter with pc.get_map_index() == 302 and pc.get_level() >= 95 begin
		local v = find_npc_by_vnum(20409)
			if v != 0 then
			target.vid("__TARGET__", v, "Orman Bekçisi")
			end
		end
		
		when 20409.chat."Ýlk Keþif" begin
		say_title("Ýlk Keþif")
		say("Yüzün çok tanýdýk geliyor... daha önce hiç ")
		say("karþýlaþmýþ mýydýk? Belki ondandýr. Efsunlu Orman")
		say("hýzla deðiþiyor, duymuþsundur herhalde. Yeni")
		say("belirmelere karþý tek baþýna savaþamayýz. Bize")
		say("yardým eder misin? Bize destek olursan harika")
		say("olur.")
		select("Evet katýlýrým.")
		target.delete("__TARGET__")
		say("Harika!")
		say("Olay þöyle geliþti: Þu köþede bölgeyi araþtýran")
		say("ve büyük tehlikelerle karþýlaþan kardeþlerime")
		say("vereceðim bir ilaç çantasý saklýyordum. Bu")
		say("çantayý bir süredir bulamýyorum. Her yere baktým")
		say("ama sanki yer yarýlmýþ içine girmiþ. Sanýrým En")
		say("Tai Savaþçý'larý bu çantayý yürüttü. Buralarda bu")
		say("savaþçýlardan çok gördüm ve çok garip bir halleri")
		say("vardý... Bu iþin arkasýndakiler onlar mý, lütfen")
		say("öðren.")
		pc.setqf("entai_1",0)
		set_state(kill_1)
		end
	end
	
	state kill_1 begin
		when letter begin
		send_letter("&En Tai Savaþçýsý öldür.")
		loop_timer("kill_1",1)
		q.set_counter("Kalan sayý ", 50 - pc.getqf("entai_1"))
		end
		
		when button or info begin
		say_title("Ýlk Keþif")
		say("Efsunlu Orman'da En Tai Savaþçýsý öldür ve Orman")
		say("Bekçisi'ne geri dön.")
		q.set_counter("Kalan sayý ", 50 - pc.getqf("entai_1"))
		end
		
		when kill with npc.get_race() == 6401 begin
		local count = pc.getqf("entai_1")+1
			if party.is_party() then
			party.setqf("entai_1",count)
			else
			pc.setqf("entai_1",count)
			end
		q.set_counter("Kalan sayý ", 50 - count)
		end
		
		when kill_1.timer begin
		q.set_counter("Kalan sayý ", 50 - pc.getqf("entai_1"))
			if pc.getqf("entai_1") >= 50 then
			notice("En Tai Savaþçýlarý'ný öldürdün. Orman Bekçisi'ne")
			notice("geri dön.")
			cleartimer("kill_1")
			clear_letter()
			pc.setqf("entai_1",0)
			set_state(return_1)
			end
		end
	end
	
	state return_1 begin
		when letter begin
		local v = find_npc_by_vnum(20409)
			if v != 0 then
			target.vid("__TARGET__", v, "Orman Bekçisi")
			end
		send_letter("&Ýlk Keþif")
		end
		
		when button or info begin
		say_title("Ýlk Keþif")
		say("Orman Bekçisi'ni bul ve onunla konuþ.")
		end
		
		when 20409.chat."Ýlk Keþif" begin
		target.delete("__TARGET__")
		say_title("Ýlk Keþif")
		say("Geri döndün demek! En Tai Savaþçýlarý'ndan bir")
		say("þey aldýn mý?")
		say_reward("Her þeyi inceledim. Ama önemli bir þey bulamadým.")
		say("")
		say("Çok tuhaf. Tahminlerimden oldukça emindim...")
		say("Tamam. Her þeyi tekrar enine boyuna düþüneceðim.")
		say("Sonra seni çaðýrýrým. O zamana kadar bekle.")
		say_reward("Tamam.")
		say_reward("776600 Tecrübe puaný alýrsýn.")
		pc.give_exp2("776600")
		clear_letter()
		set_state(quest_2)
		end
	end
	
	state quest_2 begin
		when letter begin
		local v = find_npc_by_vnum(20409)
			if v != 0 then
			target.vid("__TARGET__", v, "Orman Bekçisi")
			end
		send_letter("&Ýlaç çantasýný bul.")
		end
		
		when button or info begin
		say_title("Ýlaç çantasýný bul.")
		say("Orman Bekçisi'ni bul ve onunla konuþ.")
		end
		
		when 20409.chat."Ýlaç çantasýný bul." begin
		say_title(mob_name(npc.get_race()))
		say("Hoþ geldin cesur savaþçý! Büyük bir hata yapmýþým!")
		say("Þimdi eminim: Bu tuhaf yaratýklar her þeyin")
		say("sorumlusu. Büyük ihtimalle bir sihir yapmýþ. O")
		say("gittiðinde ilaç çantasý da kaybolmuþtu. Bu")
		say("büyücü, sihirbaz, þifacý veya her neyse buradan")
		say("çok fazla uzaklaþmýþ olamaz. Lütfen onu bul.")
		select("Tamam, onu arayacaðým.")
		target.delete("__TARGET__")
		say("Çok güzel. Seni burada bekleyeceðim. Kendine")
		say("dikkat et.")
		pc.setqf("entai_killed",0)
		set_state(kill_2)
		end
	end
	
	state kill_2 begin
		when letter begin
		send_letter("&Ýlaç çantasýný ara.")
		loop_timer("kill_2",1)
		end
		
		when button or info begin
		say_title("Ýlaç çantasýný ara.")
		say("En Tai Rahibi'ni öldür ve ilaç çantasýný ele")
		say("geçir.")
		end
		
		when kill with npc.get_race() == 6403 begin
			if number(1,100) <= 100 and pc.getqf("entai_killed") == 0 then
				if party.is_party() then
				party.setqf("entai_killed",1)
				else
				pc.setqf("entai_killed",1)
				end
			end
		end
		
		when kill_2.timer begin
			if pc.getqf("entai_killed") == 1 then
			pc.setqf("entai_killed",0)
			cleartimer("kill_2")
			notice("En Tai Rahibi'nden bir ilaç çantasý ele geçirdin.")
			set_state(return_2)
			end
		end
	end
	
	state return_2 begin
		when letter begin
		local v = find_npc_by_vnum(20409)
			if v != 0 then
			target.vid("__TARGET__", v, "Orman Bekçisi")
			end
		send_letter("&Ýlaç çantasýný bul.")
		end
		
		when button or info begin
		say_title("Ýlaç çantasýný bul.")
		say("Orman Bekçisi'ni bul ve onunla konuþ.")
		end
		
		when 20409.chat."Ýlaç çantasýný ara." begin
		target.delete("__TARGET__")
		say_title(mob_name(npc.get_race()))
		say("Dur bir bakayým. Evet! Bu o! Artýk ilaç çantasýný ")
		say("kardeþlerime gönderebilirim. Sana çok teþekkür")
		say("ederim. Benim için yaptýklarýný asla")
		say("unutmayacaðým.")
		say("Uzun bir süredir dýþarýdayýz. Stoklarýmýz")
		say("gittikçe azalýyor. En iyisi bunlarý onlara hemen")
		say("göndereyim. Bakalým bir eksik var mý. Sonra seni")
		say("çaðýrýrým. Sen bu arada dinlen.")
		say_reward("776600 Tecrübe puaný alýrsýn.")
		pc.give_exp2("776600")
		set_state(quest_3)
		end
	end
	
	state quest_3 begin
		when letter begin
		local v = find_npc_by_vnum(20409)
			if v != 0 then
			target.vid("__TARGET__", v, "Orman Bekçisi")
			end
		send_letter("&Ýlaç çantasýný teslim et")
		end
		
		when button or info begin
		say("Orman Bekçisi'ni bul ve onunla konuþ.")
		end
		
		when 20409.chat."Ýlaç çantasýný teslim et" begin
		say_title(mob_name(npc.get_race()))
		say("Dinlenebildin mi? Ýyi ilaç çantasýna baktým,")
		say("hiçbir eksik yok. Çantayý kardeþlerime")
		say("göndermenin zamaný geldi. Sakýn yolda bir þey")
		say("kaybetme.")
		select("Tabii ki.")
		target.delete("__TARGET__")
		say("Tamam, dikkatli ol!")
		pc.give_item2("31100",1)
		set_state(go_3)
		end
	end
	
	state go_3 begin
		when letter begin
		local v = find_npc_by_vnum(20410)
			if v != 0 then
			target.vid("__TARGET__", v, "Orman Casusu")
			end
		send_letter("&Ýlaç çantasýný teslim et")
		end
		
		when button or info begin
		say_title("Ýlaç çantasýný teslim et")
		say("Ýlaç çantasýný Orman Casusu'na teslim et.")
		end
		
		when 20410.chat."Ýlaç çantasýný teslim et" with pc.count_item("31100") > 0 begin
		target.delete("__TARGET__")
		say_title(mob_name(npc.get_race()))
		say("Kim geldi? Senin ne iþin var burada?")
		say_reward("Beni Orman Bekçisi gönderdi. Sana bunu")
		say_reward("verecekmiþim.")
		say("Lütfen kabalýðýmý hoþ gör. Bana ilaç çantamý ")
		say("getirdiðin için çok teþekkür ederim. Ýlaçlara")
		say("acilen ihtiyacýmýz var. Çantayý götürmeme izin")
		say("ver. Kýsaca bekler misin?")
		say_reward("Tabii ki beklerim.")
		say_reward("Gango Kökü kazandýn. 10Uniteler")
		say_reward("5825000 Tecrübe puaný alýrsýn.")
		pc.remove_item("31100",pc.count_item("31100"))
		pc.give_item2("50724",10)
		pc.give_exp2("5825000")
		set_state(quest_4)
		end
	end
	
	state quest_4 begin
		when letter begin
		local v = find_npc_by_vnum(20410)
			if v != 0 then
			target.vid("__TARGET__", v, "Orman Casusu")
			end
		send_letter("&Ýkinci Keþif")
		end
		
		when button or info begin
		say_title("Ýkinci Keþif")
		say("Orman Casusu'nu bul ve onunla konuþ.")
		end
		
		when 20410.chat."Ýkinci Keþif" begin
		say_title(mob_name(npc.get_race()))
		say("Seni bu kadar beklettiðim için çok üzgünüm.")
		say("Burayý nasýl buldun?")
		say_reward("Tapýnak Bekçisi, Efsunlu Orman'ý araþtýrmada ona")
		say_reward("Ona yardýmcý olmamý istedi. Bana ne yapabileceðimi")
		say_reward("söyler misin?")
		say("Cesaretinden çok etkilendim.")
		say("Bu ormanýn bir çok aðacý taþa dönüþmüþ. Bu")
		say("taþlardan güçlü ve mistik bir enerji yayýlýyor bu")
		say("da çýk büyük bir gürültüye yol açýyor. Bu taþlarý ")
		say("yok etmeme yardým et lütfen.")
		select("Tabii, yaparým.")
		target.delete("__TARGET__")
		pc.setqf("stone_4",0)
		set_state(kill_4)
		end
	end
	
	state kill_4 begin
		when letter begin
		send_letter("&Taþlarý yok et")
		q.set_counter("Kalan sayý ", 5 - pc.getqf("stone_4"))
		loop_timer("kill_4",1)
		end
		
		when button or info begin
		say_title("Taþlarý yok et")
		say("Aðaç Yaratýðý Metinleri'ni yok et ve Orman")
		say("Casusu'na geri dön.")
		q.set_counter("Kalan sayý ", 5 - pc.getqf("stone_4"))
		end
		
		when kill with npc.get_race() == 8059 begin
		local count = pc.getqf("stone_4")+1
			if party.is_party() then
			party.setqf("stone_4",count)
			else
			pc.setqf("stone_4",count)
			end
		q.set_counter("Kalan sayý ", 5 - count)
		end
		
		when kill_4.timer begin
		q.set_counter("Kalan sayý ", 5 - pc.getqf("stone_4"))
			if pc.getqf("stone_4") >= 5 then
			notice("Tüm Aðaç Yaratýðý Metinleri'ni yok ettin. Orman")
			notice("Casusu'na rapor ver.")
			cleartimer("kill_4")
			clear_letter()
			pc.setqf("stone_4",0)
			set_state(return_4)
			end
		end
	end
	
	state return_4 begin
		when letter begin
		local v = find_npc_by_vnum(20410)
			if v != 0 then
			target.vid("__TARGET__", v, "Orman Casusu")
			end
		send_letter("&Ýkinci Keþif")
		end
		
		when button or info begin
		say_title("Ýkinci Keþif")
		say("Orman Casusu'nu bul ve onunla konuþ.")
		end
		
		when 20410.chat."Ýkinci Keþif" begin
		say_title(mob_name(npc.get_race()))
		say("Baþardýn! Harika! Sen Aðaç Yaratýðý Metinleri'ni")
		say("yok ederken ben de buradaki mantar hücrelerinden")
		say("mantar elde ettim. Bakalým bunlardan nasýl")
		say("yararlanabiliriz. Ama önce baþka bir iþle")
		say("ilgilenmeliyiz.")
		say_reward("7766000 Tecrübe puaný alýrsýn.")
		target.delete("__TARGET__")
		pc.give_exp2("7766000")
		clear_letter()
		set_state(quest_5)
		end
	end
	
	state quest_5 begin
		when letter begin
		local v = find_npc_by_vnum(20410)
			if v != 0 then
			target.vid("__TARGET__", v, "Orman Casusu")
			end
		send_letter("&En Tai Þebnemi")
		end
		
		when button or info begin
		say_title("En Tai Þebnemi")
		say("Orman Casusu'nu bul ve onunla konuþ.")
		end
		
		when 20410.chat."En Tai Þebnemi" begin
		say_title(mob_name(npc.get_race()))
		say("Hmm... acaba bu gerçekten sýkça bahsedilen")
		say("efsanevi En Tai þebnemi mi? Efsaneye göre")
		say("insanlar bu þebnemi þeytanlarý defetmek için")
		say("kullanýrmýþ.")
		say("Bu müthiþ sývýyý En Tai Ýmhacý'larýnýn")
		say("kendisinden ele geçirmek en iyisi olacaktýr.")
		say("Bu þebnemi benim için bulur musun? Bunu")
		say("yapabileceðinden eminim.")
		select("Bulurum!")
		target.delete("__TARGET__")
		set_state(kill_5)
		end
	end
	
	state kill_5 begin
		when letter begin
		send_letter("&En Tai þebnemi bul")
		pc.setqf("entai_killed_11",0)
		loop_timer("kill_5",1)
		q.set_counter("Kalan sayý ", 5 - pc.count_item("31097"))
		end
		
		when button or info begin
		say_title("En Tai þebnemi bul")
		say("En Tai Ýmhacýsý öldür ve 5 En Tai þebnemi topla.")
		q.set_counter("Kalan sayý ",pc.count_item("31097"))
		end
		
		when kill with npc.get_race() == 6406 begin
			if number(1,5) == 3 and pc.getqf("entai_killed_11") == 0 then
				if party.is_party() then
				party.setqf("entai_killed_11",1)
				else
				pc.setqf("entai_killed_11",1)
				end
			end
		end
		
		when kill_5.timer with pc.getqf("entai_killed_11") == 1 begin
		pc.setqf("entai_killed_11",0)
		pc.give_item2("31097",1)
		q.set_counter("Kalan sayý ", 5 - pc.count_item("31097"))
			if pc.count_item("31097") == 5 then
			notice("En Tai Ýmhacý'larýndan En Tai þebnemi elde ettin.")
			notice("Bunlarý Orman Casusu'na götür.")
			cleartimer("kill_5")
			clear_letter()
			set_state(return_5)
			end
		end
	end
	
	state return_5 begin
		when letter begin
		local v = find_npc_by_vnum(20410)
			if v != 0 then
			target.vid("__TARGET__", v, "Orman Casusu")
			end
		send_letter("&En Tai Þebnemi")
		end
		
		when button or info begin
		say_title("En Tai Þebnemi")
		say("En Tai Ýmhacý'larýndan En Tai þebnemi elde ettin.")
		say("Bunlarý Orman Casusu'na götür.")
		end
		
		
		when 20410.chat."En Tai Þebnemi" with pc.count_item("31097") >= 5 begin
		target.delete("__TARGET__")
		say_title(mob_name(npc.get_race()))
		say("En Tai Ýmhacýsý'ný bulup En Tai þebnemini elde")
		say("ettin mi? Haklý çýktým yani! Yardýmlarýn")
		say("sayesinde artýk En Tai þebnemini bulduk. Bununla")
		say("gürültüyü durdurabileceðim. Buyur. Çam sakýzý ")
		say("çoban armaðýný. Þimdi Orman Kaþifi'ni bul.")
		say_reward("Yöng Suyu kazandýn. 10Uniteler")
		say_reward("7766000 Tecrübe puaný alýrsýn.")
		pc.remove_item("31097",pc.count_item("31097"))
		pc.give_exp2("7766000")
		set_state(quest_6)
		end
	end
	
	state quest_6 begin
		when letter begin
		local v = find_npc_by_vnum(20411)
			if v != 0 then
			target.vid("__TARGET__", v, "Orman Kaþifi")
			end
		send_letter("&En Tai Hükümdarý'ný öldür")
		end
		
		when button or info begin
		say_title("En Tai Hükümdarý'ný öldür")
		say("Orman Kaþifi'ni bul ve onunla konuþ.")
		end
		
		when 20411.chat."En Tai Hükümdarý'ný öldür" begin
		say_title(mob_name(npc.get_race()))
		say("Sen Orman Casusu'nun bahsettiði cesur savaþçý ")
		say("olmalýsýn. Adýný Orman Casusu'ndan duyduðumdan")
		say("beri dört gözle seni bekliyorum. Þimdi beni çok")
		say("iyi dinle.")
		wait()
		say("Benim görevim ormaný keþfetmek. En Tailerin son")
		say("zamanlarda gittikçe tehlikeli olmaya")
		say("baþladýklarýný fark ettim. Hepimiz için büyük bir")
		say("tehdit oluþturuyorlar. Hükümdarlarýný ")
		say("öldürmelisin.")
		say("Onu bizzat ben öldürmek isterdim ama katýldýðým")
		say("onca savaþtan sonra kýlýcým körleþmiþ. En Tai")
		say("Hükümdarý'ný bul ve bana ondaki deðerli tahtadan")
		say("getir.")
		select("Tamam.")
		target.delete("__TARGET__")
		set_state(kill_6)
		end
	end
	
	state kill_6 begin
		when letter begin
		send_letter("&En Tai Hükümdarý'ný öldür")
		target.pos("__BOSS__", 610, 78, 354, "En Tai Hükümdarý ")
		loop_timer("kill_6",1)
		pc.setqf("boss_1",0)
		end
		
		when button or info begin
		say_title("En Tai Hükümdarý'ný öldür")
		say("En Tai Hükümdarý'ný öldür ve deðerli tahta elde")
		say("et.")
		end
		
		when kill with npc.get_race() == 6407 begin
			if party.is_party() then
			party.setqf("boss_1",1)
			else
			pc.setqf("boss_1",1)
			end
		end
		
		when kill_6.timer with pc.getqf("boss_1") > 0 begin
		target.delete("__BOSS__")
		pc.give_item2("31096",1)
		pc.setqf("boss_1",0)
		notice("En Tai Hükümdarý'ndan deðerli tahta elde ettin.")
		notice("Orman Kaþifi'ne git ve rapor ver.")
		cleartimer("kill_6")
		set_state(return_6)
		end
	end
	
	state return_6 begin
		when letter begin
		local v = find_npc_by_vnum(20411)
			if v != 0 then
			target.vid("__TARGET__", v, "Orman Kaþifi")
			end
		send_letter("&En Tai Hükümdarý'ný öldür")
		end
		
		when button or info begin
		say_title("En Tai Hükümdarý'ný öldür")
		say("En Tai Hükümdarý'ndan deðerli tahta elde ettin.")
		say("Orman Kaþifi'ne git ve rapor ver.")
		end
		
		when 20411.chat."En Tai Hükümdarý'ný öldür" with pc.count_item("31096") > 0 begin
		say_title(mob_name(npc.get_race()))
		say("Ýnanýlmaz! Baþardýn!")
		say("Bu tahtayý mümkün olduðunca kýsa sürede")
		say("analiz etmeye çalýþacaðým.")
		say("")
		say("Sakýncasý yoksa iþim bitine kadar")
		say("bekleyebilir misin?")
		say_reward("11650000 Tecrübe puaný alýrsýn.")
		pc.remove_item("31096",pc.count_item("31096"))
		pc.give_exp2("11650000")
		set_state(quest_7)
		end
	end
	
	state quest_7 begin
		when letter begin
		local v = find_npc_by_vnum(20411)
			if v != 0 then
			target.vid("__TARGET__", v, "Orman Kaþifi")
			end
		send_letter("&Bir aðaç zümrütü ")
		end
		
		when button or info begin
		say_title("Bir aðaç zümrütü ")
		say("Orman Kaþifi'ni bul ve onunla konuþ.")
		end
		
		when 20411.chat."Bir aðaç zümrütü " begin
		say_title(mob_name(npc.get_race()))
		say("Tekrar döndüðüne sevindim. Tahtanýn analizi")
		say("üzerinde uzun zaman geçirdim. Aradýðýmýzýn bu")
		say("olduðundan emindim fakat analiz sýrasýnda")
		say("fark ettim.... Eksik bir þey var. Aðaç ")
		say("zümrütü gerekli. Tekrar En Tai Hükümdarý'ný ")
		say("ziyaret edebilir misin? Bu gerçekten gerekli.")
		wait()
		say("En Tai Hükümdarý'ndan bunu almalýyýz. Bu kadar")
		say("kolay vazgeçmek istemiyoruz.")
		select("Tamam, gideceðim.")
		target.delete("__TARGET__")
		set_state(kill_7)
		end
	end
	
	state kill_7 begin
		when letter begin
		send_letter("&Bir aðaç zümrütü ")
		target.pos("__BOSS__", 610, 78, 213, "En Tai Hükümdarý ")
		loop_timer("kill_7",1)
		pc.setqf("boss_2",0)
		end
		
		when button or info begin
		say_title("Bir aðaç zümrütü ")
		say("En Tai Hükümdarý'ný öldür ve aðaç zümrütü elde")
		say("et.")
		end
		
		when kill with npc.get_race() == 6407 begin
			if party.is_party() then
			party.setqf("boss_2",1)
			else
			pc.setqf("boss_2",1)
			end
		end
		
		when kill_7.timer with pc.getqf("boss_2") > 0 begin
		target.delete("__BOSS__")
		pc.give_item2("31099",1)
		pc.setqf("boss_2",0)
		notice("En Tai Hükümdarý'ndan aðaç zümrütü elde ettin.")
		notice("Orman Kaþifi'ne git ve rapor ver.")
		cleartimer("kill_7")
		set_state(return_7)
		end
	end
	
	state return_7 begin
		when letter begin
		local v = find_npc_by_vnum(20411)
			if v != 0 then
			target.vid("__TARGET__", v, "Orman Kaþifi")
			end
		send_letter("&Bir aðaç zümrütü ")
		end
		
		when button or info begin
		say_title("Bir aðaç zümrütü ")
		say("En Tai Hükümdarý'ndan aðaç zümrütü elde ettin.")
		say("Orman Kaþifi'ne git ve rapor ver.")
		end
		
		when 20411.chat."En Tai Hükümdarý'ný öldür" with pc.count_item("31099") > 0 begin
		target.delete("__TARGET__")
		say_title(mob_name(npc.get_race()))
		say("Baþardýn! Gözlerime inanamýyorum - bir gün")
		say("bunu göreceðimi hiç düþünmemiþtim!")
		say("Þimdi bu aðaç zümrütünü analiz etmek için")
		say("gidiyorum.")
		say("Baþardýk! Yardýmlarýn ve cesaretin için")
		say("teþekkürler. Al bakalým. Bu Orman Perisi taþý.")
		say("En Tai Hükümdarý'ndan aldýðýmýz bu otantik taþ ")
		say("çok büyük bir potansiyele sahip. Jotun Thrym")
		say("ile karþýlaþmak sadece bu taþla mümkün olabilir.")
		say("Onunla karþýlaþmak için Orman Bekçileri Lideri")
		say("ile konuþ, hadi!")
		say_reward("Orman Perisi Taþý kazandýn. 1Uniteler")
		say_reward("11650000 Tecrübe puaný alýrsýn.")
		pc.remove_item("31099",pc.count_item("31099"))
		pc.give_item2("30613",1)
		pc.give_exp2("11650000")
		set_state(start)
		end
	end
end
	