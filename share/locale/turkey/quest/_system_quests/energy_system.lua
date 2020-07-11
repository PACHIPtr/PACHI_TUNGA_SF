--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest energy_system begin
	state start begin
		when 20001.chat." Yeni bir teknik! " begin
			say_title(mob_name(20001)) 
			say("Baþardým![ENTER]Nihayet yepyeni bir teknik geliþtirebildim.[ENTER]Eþyalarý tekrar iþleyerek onlardan saf enerji elde etmenin yolunu buldum. Ne þahane! ")
			wait()
			say_title(mob_name(20001)) 
			say("Eþya benim bulduðum teknikle tahrip edilir ve[ENTER]enerji parçalarýna ayrýlýr. Bu parçalarýn 30 tane [ENTER]tanesi birleþtirilince de güçlü bir enerji kristali elde edilir! Bir mücevher içerisinde toplanmýþ saf ve filtrelenmiþ bir enerji.[ENTER]Bu güç tüm teçhizatýna iþler. Ýlgini çekiyor mu?")
			wait()
			say_title(mob_name(20001)) 
			say("Avlanýrken bulduðun silah, taký ve kýyafet gibi[ENTER]teçhizat malzemelerini bana getir.[ENTER]Onlarý iþleyip [ENTER]enerji parçalarýna ayýracaðým. [ENTER]Krallýðýmýzýn [ENTER]geleceði ve umudu bu tekniðe baðlý. Yenilmez olacaðýz! ")
			setstate(can_make)
		end
	end

	state can_make begin
		function setting () 
			return
			{
				["prob_acc_table"] = 
				{
					["65to75"] = {30,55,70,80,90,95,97,98,99,100},
					["76to85"] = {20,40,60,75,85,91,96,98,99,100},
					["upto85"] = {10,25,45,65,80,88,94,97,99,100}
				},
				["item_num_table"] ={0,1,2,3,4,6,8,10,12,15},
				["energy_stone"] = 51001,
				["charging_stone"] = 51002
			}
		end	
		function getItemNum ( str, r )
			local setting = energy_system.setting()
			for i = 1, 10 do
				if r < setting.prob_acc_table[str][i] then
					return setting.item_num_table[i]
				end
			end
			return 0
		end

		when 20001.chat." Enerji Parçasý " begin
			say_title(mob_name(20001)) 
			say("Yapabildin mi? Enerji parçasý elde edebildin mi?[ENTER]Simya yardýmýyla parçalayabileceðim daha fazla[ENTER]eþya getir bana.Tekniðim henüz mükemmelleþmedi.[ENTER]Bu yüzden ne kadar parça elde edileceðine dair[ENTER]bir garanti veremeyeceðim.")
			wait()
			say_title(mob_name(20001)) 
			say("Tek bir þart var:[ENTER]Kendi seviyen ve eþyanýn seviyesi[ENTER]en az 65 olmalý. Hým bir göz atalým bakalým... ")
			wait()
			if pc.get_level() < 65 then 
				say_title(mob_name(20001)) 
				say("Henüz yeterince güçlü deðilsin![ENTER]En az 65. seviyede olmalýsýn. ")
			else
				say_title(mob_name(20001)) 
				say("Ah süper! Güçlü ve tecrübelisin. Ýþlenecek[ENTER]eþyayý ver bakalým. ")
			end
		end

		when 20001.take begin
			if pc.get_level() < 65 then 
				say_title(mob_name(20001)) 
				say("65. seviye'ye ulaþtýðýnda gel.")
				return
			end
			local item_vnum = item.vnum
			local levelLimit = item.get_level_limit(item_vnum)
			local setting = energy_system.setting()
			
			if levelLimit == nil then
				say_title(mob_name(20001)) 
				say("Bu öðe tekniðime uygun deðil. ")
				wait()
			elseif item.get_type() == ITEM_WEAPON and item.get_sub_type() == WEAPON_ARROW then
				say_title (mob_name(20001)) 
				say ("Bu öðe tekniðime uygun deðil. ")
				wait()
			elseif item.is_sealed() == true then
				say_title(mob_name(20001))
				say ("Ruha baðlanmýþ bir öðeyi enerji parçasýna dönüþtüremezsin.")
				wait()
			elseif item.is_basic() == true then
				say_title(mob_name(20001))
				say ("Temel (Baþlangýç) eþyalarýný enerji parçasýna dönüþtüremezsin.")
				wait()
			elseif levelLimit < 65 then
				say_title(mob_name(20001)) 
				say("Bu öðe yeterli seviyeye sahip deðil.")
			else
				say_title(mob_name(20001)) 
				say(item_name(item_vnum))
				say("Bu itemi enerji parçasýna dönüþtürmek istermisin? ")
				local s = select (" Evet! "," Hayýr, þimdi deðil!")
				if s == 1 then
					item.remove()
					local r = number(1, 100)
					local n
					if levelLimit >= 65 and levelLimit <= 75 then
						n = energy_system.getItemNum ("65to75",r)
					elseif levelLimit > 75 and levelLimit <= 85 then
						n = energy_system.getItemNum ("75to85",r)
					else
						n = energy_system.getItemNum ("upto85",r)
					end
					if (n == 0) then
						say_title(mob_name(20001)) 
						say("Ne yazýkki iþe yaramadý.[ENTER]Enerji kristali üretemedik. ")
					else
						pc.give_item2(setting.energy_stone, n)
						say_title(mob_name(20001)) 
						say(string.format("Harika! %d Enerji parçasý oluþturuldu..[ENTER]Hadi bakalým. ",n))
					end
				end
			end
		end

		when 20001.chat."Enerji Kristali Üret " begin
			local setting = energy_system.setting()
			local need = 30
			say_title(mob_name(20001)) 
			say(string.format("Enerji parçalarý tükendi. Sende var mý?[ENTER]Bu kadar sýký çalýþma ve pervasýzca riski[ENTER]hayatýnýzý koymak gerekir.[ENTER]Enerji Kristali oluþturmak için 30 tane[ENTER]Enerji Parçasý gerekir. ",need))
			wait()
			
			if pc.get_level() < 65 then 
				say_title(mob_name(20001)) 
				say("Henüz yeterince güçlü deðilsin![ENTER]En az 65. seviyede olmalýsýn. ")
				return
			end
			
			if pc.count_item(setting.energy_stone) < need then
				say_title(mob_name(20001)) 
				say(string.format("Yeterince Enerji Parçasý yok.[ENTER]Malesef Enerji Kristali üretemem.[ENTER]Enerji Parçalarýný tamamladýðýn zaman gel. ",need))
				return
			else
				say_title(mob_name(20001)) 
				say(string.format("Çok güzel. Gerekli Enerji Parçalarý sende var.[ENTER]Þimdi Enerji Kristali oluþturabilirim.[ENTER]Lütfen biraz bekleyin. Enerji Kristali oluþturmayý[ENTER]deneyeceðim, ama baþarýlý olacaðýnýnýn garantisini[ENTER]veremem. ", need))
				wait()
			end
			
			local charge = 1000
			say_title(mob_name(20001)) 
			say(string.format("Her þey hazýr durumda. Parçalarý birleþtirip[ENTER]enerji kristali üretebiliriz. Ama bende bir[ENTER]þekilde ekmeðimi kazanmalýyým. Tamir[ENTER]aletleri de gökten inmiyor yani.[ENTER]Masraflarý karþýlamak için 1000 yang[ENTER]ödemeni istiyorum. Ödemeye hazýr mýsýn?", charge))
			local s = select (" Elbette! "," Hayýr, þimdi deðil. ")
			if s == 2 then
				say_title(mob_name(20001)) 
				say("Tamam, kararýna saygý duyuyorum.[ENTER]Fikrini deðiþtirdiðinde gel.")
				return
			end

			if pc.get_gold() < charge then
				say_title(mob_name(20001)) 
				say("Üzgünüm, yeterli ücrete sahip[ENTER]olduðunda tekrar gel.")
				return
			end
			
			if pc.count_item (setting.energy_stone) < need then
				return
			end			

			pc.change_gold(-charge)
			pc.remove_item(setting.energy_stone, need)

			if pc.getqf("hasExperience") == 0 then
				say_title(mob_name(20001)) 
				say("Ýþte enerki kristalin. Sadece ilk giriþimde[ENTER]baþarýlý olunur hep. Simya saðý solu belli[Enter]olmayan bir bilim. Ufak tefek farklýlýklar[ENTER]baþarýsýzlýða neden olabilir.Yani[ENTER]deneylerde genelde küçük de olsa baþarýsýzlýða[ENTER]uðrama riski bulunur. ")
				pc.give_item2 (setting.charging_stone, 1)
				pc.setqf("hasExperience", 1)
				return
			end

			local r = number (1, 100)
			if r > 30 then
				say_title(mob_name(20001)) 
				say("Ne yazýkki iþe yaramadý.[ENTER]Enerji kristali üretemedik.")
				return
			end
			say_title(mob_name(20001)) 
			say("Harika! Ýþte enerki kristalin.[ENTER]Enerjiyi hissedebiliyor musun? ")
			pc.give_item2(setting.charging_stone, 1)

		end
	end
end
