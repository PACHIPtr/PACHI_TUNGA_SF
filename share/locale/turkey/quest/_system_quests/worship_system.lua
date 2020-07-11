--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest sevap begin
	state start begin
		function sevap_odul_list(sayi)
			sevap_oduller =  
			{  
				[1] = {38055}, 
				[2] = {71161},
				[3] = {52105},	
				[4] = {71123},	
				[5] = {71129},	
			}
			sayi = tonumber(sayi)
			toplam = table.getn(sevap_oduller)
			return sevap_oduller[sayi][1], toplam
		end
        when button or info begin
			say_title("Merhaba "..pc.get_name())
			local sevap_puani = pc.get_worship_point()
			say_reward("Mevcut Sevap Puanın : "..sevap_puani)
			say("Ne yapmak istiyorsun ?")
			say("")
			local menu = select("Bilgi Al ", "Ödüller ", "Ödül Al ", "Kapat ")
			if menu == 4 then
				send_letter("Sevap Sistemi ")
			elseif menu == 1 then
				say("Her 5 sevap puanında ")
				say("1 ödül kazanmanı sağlar.")
				wait()
				send_letter("Sevap Sistemi ")
			elseif menu == 2 then
				say("Şansa bağlı olarak aşağıdaki ödüllerden birini kazanacaksın.")
				local deger, sayi = sevap.sevap_odul_list(1)
				if sayi > 10 then
					say_size(350,400)
				end
				for i=1, sayi,1 do
					local deger1, sayi1 = sevap.sevap_odul_list(i)
					say(i.." : "..item_name(deger1))					
				end
				wait()
				send_letter("Sevap Sistemi ")
			elseif menu == 3 then
				if sevap_puani < 5 then
					say("Ödül alabilmek için en az 5 ")
					say("sevap puanına sahip olmalısın.")
					wait()
					send_letter("Sevap Sistemi ")
					return
				end
				say("Ödül almak istiyor musun ?")
				say("")
				local al = select(locale.yes,locale.no)
				if al == 1 then
					local hediyemm, sayi = sevap.sevap_odul_list(1)
					local sans = number(1,sayi)
					local hediyem, sayim = sevap.sevap_odul_list(sans)
					say_item_vnum(hediyem)
					say(item_name(hediyem).." kazandın.")
					pc.give_item2(hediyem,1)
					pc.change_worship_point(-5)
				end
				wait()
				send_letter("Sevap Sistemi ")
			end
		end
		when 20022.chat."Abdest Al " with pc.get_map_index() == 1 or pc.get_map_index() == 21 or pc.get_map_index() == 41 begin -------------- Şadırvan Görevlisi
			say_title("Merhaba "..pc.get_name())
			say("")
			say("Demek abdest almak istiyorsun?")
			say("Abdest almak için önce niyet etmek önemlidir.")
			say_reward("Aşağıdaki boşluğa Niyet ettim abdest almaya yazın.")
			local yaz = tostring(input(''))
			if yaz == "Niyet ettim abdest almaya" then
				say("Abdestini aldın artık namaz kılabilirsin.")
				say("Abdestin varken 30 dakika içerisinde namaz kılabilirsin.")
				pc.setqf("abdest",get_time()+30*60)
			else
				say("Her şeyden önce doğru niyet önemlidir.")
				say("Niyet yazısını doğru yazın.")
				return
			end
		end
		
		when 20087.chat."Namaz Kıl " begin -------------- İmam
			say_title("Merhaba "..pc.get_name())
			say("")
			if pc.getqf("abdest") < get_time() then
				say("Abdestsiz namaz kılamazsın.")
				say("Git önce abdest al.")
				return
			end
			if pc.getqf("namaz") == 1 then
				say("Bu vaktin namazını zaten kıldın.")
				return
			end
			if game.get_event_flag("namaz") < get_time() then
				say("Namaz vakti daha gelmedi.")
				return
			end
			say("Demek namaz kılmak istiyorsun?")
			say("Namaz kılmak için önce niyet etmek önemlidir.")
			say_reward("Aşağıdaki boşluğa Niyet ettim namaz kılmaya yazın.")
			local yaz = tostring(input(''))
			if yaz == "Niyet ettim namaz kılmaya" then
				say("Allah kabul etsin.")
				say("1 Sevap puanı kazandın.")
				say("")
				pc.change_worship_point(1)
				pc.setqf("namaz",1)
				say_reward("Toplam Sevap Puanın : "..pc.get_worship_point())
			else
				say("Her şeyden önce doğru niyet önemlidir.")
				say("Niyet yazısını doğru yazın.")
				return
			end
		end
		when login begin
			loop_timer("ezan_bak",40)
			send_letter("Sevap Sistemi ")
		end
		when ezan_bak.timer begin
			local saat = tonumber(os.date("%H"))
			local dakika = tonumber(os.date("%M"))
			local ezan_vakit = {
			---------[sıra] = {saat,dakika}--------------
				[1]		= {14,26},
				[2]		= {14,02}, 
				[3]		= {17,10},
				[4]		= {20,35},
				[5]		= {22,37},
			}
			for i=1, table.getn(ezan_vakit),1 do
				if tonumber(ezan_vakit[i][1]) == saat and tonumber(ezan_vakit[i][2]) == dakika then
					syschat("Allahu Ekber Allahu Ekber")
					syschat("Ezan sesi tüm diyarda yankılanıyor tüm cemaat abdestini alsın namaza bekleniyor.")
					game.set_event_flag("namaz",get_time()+30*60)
					cmdchat("ezancal")
				end	
			end
			if game.get_event_flag("namaz") < get_time() then
				pc.setqf("namaz",0)
			end
		end
	end
end
			