--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest dragonlair begin
	state start begin
		when logout with pc.get_map_index() == 207 begin
				pc.delqf("icerdesin_1_")
				pc.delqf("icerdesin_2_")
				pc.delqf("ejder_sifre")
				pc.delqf("lider")
				pc.delqf("ejder_bug_engel333")
				pc.delqf("ejder_bug_engel33")
				pc.delqf("ejder_bug_engel3")
				pc.delqf("ejder_bug_engel2")
				pc.delqf("ejder_sifre_kontrol")
				pc.setf("dragonlair", "dragonlair_soguma_suresi", get_time() + (60*10))
			pc.warp(180100, 1220400)
			end
		when login with pc.get_map_index() != 207 begin
			if pc.getqf("icerdesin_1_") == 1 or pc.getqf("icerdesin_2_") == 1 or pc.getqf("lider") == 1 then
				pc.delqf("icerdesin_1_")
				pc.delqf("icerdesin_2_")
				pc.delqf("ejder_sifre")
				pc.delqf("lider")
				pc.delqf("ejder_bug_engel333")
				pc.delqf("ejder_bug_engel33")
				pc.delqf("ejder_bug_engel3")
				pc.delqf("ejder_bug_engel2")
				pc.delqf("ejder_sifre_kontrol")
			end
		end
		function arti(tip)
			if tip == 1 then
				addimage(80, 55, "arti.tga")
			end
		end
		function eksi(tip)
			if tip == 1 then
				addimage(80, 59, "eksi.tga")
			end
		end
		function carpi(tip)
			if tip == 1 then
				addimage(80, 55, "carpi.tga")
			end
		end
		function artisayi3(tip)
			if tip == 1 then
				addimage(115, 55, "arti.tga")
			end
		end
		function eksisayi3(tip)
			if tip == 1 then
				addimage(117, 59, "eksi.tga")
			end
		end
		function carpisayi3(tip)
			if tip == 1 then
				addimage(112, 55, "carpi.tga")
			end
		end
		function artisayi4(tip)
			if tip == 1 then
				addimage(150, 55, "arti.tga")
			end
		end
		function eksisayi4(tip)
			if tip == 1 then
				addimage(152, 59, "eksi.tga")
			end
		end
		function carpisayi4(tip)
			if tip == 1 then
				addimage(150, 55, "carpi.tga")
			end
		end
		function arkaplan(tip)
			if tip == 1 then
				addimage(70, 45, "arkaplan.tga")
			end
		end
		function cizgi(tip)
			if tip == 1 then
				addimage(70, 45, "cizgi.tga")
			end
		end
		function resim(tip)
			if tip == 1 then
				addimage(70, 50, "1.tga")
			elseif tip == 2 then
				addimage(70, 50, "2.tga")
			elseif tip == 3 then
				addimage(70, 50, "3.tga")
			end
		end
		function resim2(tip)
			if tip == 1 then
				addimage(90, 50, "1.tga")
			elseif tip == 2 then
				addimage(90, 50, "2.tga")
			elseif tip == 3 then
				addimage(90, 50, "3.tga")
			end
		end
		function resim3(tip)
			if tip == 1 then
				addimage(105, 50, "1.tga")
			elseif tip == 2 then
				addimage(105, 50, "2.tga")
			elseif tip == 3 then
				addimage(105, 50, "3.tga")
			end
		end
		function resim4(tip)
			if tip == 1 then
				addimage(125, 50, "1.tga")
			elseif tip == 2 then
				addimage(125, 50, "2.tga")
			elseif tip == 3 then
				addimage(125, 50, "3.tga")
			end
		end
		function resim5(tip)
			if tip == 1 then
				addimage(140, 50, "1.tga")
			elseif tip == 2 then
				addimage(140, 50, "2.tga")
			elseif tip == 3 then
				addimage(140, 50, "3.tga")
			end
		end
		function resim6(tip)
			if tip == 1 then
				addimage(160, 50, "1.tga")
			elseif tip == 2 then
				addimage(160, 50, "2.tga")
			elseif tip == 3 then
				addimage(160, 50, "3.tga")
			end
		end

		when 30121.chat."Ejderhayý öldürmek istiyorum!" with pc.get_map_index() == 73 begin
			if (get_time() < pc.getf("dragonlair", "dragonlair_soguma_suresi")) then
				local soguma_sure_saniye = (pc.getf("dragonlair", "dragonlair_soguma_suresi") - get_time())
				local soguma_sure_dakika = math.ceil(soguma_sure_saniye / 60)    	
				say_title("Sura Ruhu:")
				say("Odadan yeni çýktýn biraz sakin olmalýsýn")
				say("Seni tekrar alabilmem için "..soguma_sure_dakika.." dakika beklemelisin")
				return
			end

			if (pc.get_channel_id() == 3 or pc.get_channel_id() == 4 or pc.get_channel_id() == 5 or pc.get_channel_id() == 6) then
				say_title("Sura Ruhu:")
				say("Sadece 1. ve 2. kanaldan giriþ yapabilirsin.")
				return
			end
			
			if pc.get_level() < 80 then
				say_title("Sura Ruhu:")
				say("80.seviye altýnda isen ejderha odasýna giremezsin!")
				return
			end
			if game.get_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_") == 0 and pc.count_item(30179) < 1 then
				say_title("Sura Ruhu:")
				say("Gerekli eþya : "..item_name(30179)..":")
				say_item_vnum(30179)
				say("3 adet anahtara ihtiyacýn var.")
				return
			end
			if game.get_event_flag("time_1_"..pc.get_channel_id().."_") - get_time() < 0 and game.get_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_") >= 1 then
				game.set_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_",0)
				purge_area(829200,1059600,844200,1074600)
				warp_all_to_village(207,1)
				setskin(NOWINDOW)
				return
			end					
			if game.get_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_") == 1 then
					pc.setqf("ejder_bug_engel2",get_time()+10)
					say_title("Sura Ruhu:")
					say("Üzgünüm Ejderha Tapýnaðý þuan dolu.")
					say("Eðer þifreyi biliyorsan,")
					say("doðru bir þekilde yaz.")
					say("")
					say_reward("10 saniye süren var!")
					local saniyekalan = (game.get_event_flag("time_1_"..pc.get_channel_id().."_") - get_time())
					local dakikakalan = math.ceil(saniyekalan / 60)    	
					say("")
					say("Mavi Ejderha'nýn geri dönmesine "..dakikakalan.." [ENTER]dakikalýk bir zaman kaldý!") 					
					local sifre = tonumber(input(""))
					if get_time() > pc.getqf("ejder_bug_engel2") then
						say_title("Sura Ruhu:")
						say("10 saniye doldu þansýný tekrar dene.")
						pc.setqf("ejder_bug_engel2",get_time()+0)
						return
					end
					if game.get_event_flag("ejder_sifresi_"..pc.get_channel_id().."_") == sifre then
						say_title("Sura Ruhu:")
						say("")
						if pc.count_item(30179) < 1 then
							say("Girmene izin veremem.")
							say("Gerekli eþya "..item_name(30179)..":")
							say_item_vnum(30179)
							say("3 tane anahtara sahip olduðunda tekrar gel.")
							return
						end
						pc.remove_item("30179",1)
						pc.setqf("ejder_sifre_kontrol",sifre)
						say("Þifre doðru tebrikler!")
						say("Seni içeri gönderiyorum. Onu yenmeden dönme!")
						say("")
						say("Mavi Ejderha'yý "..dakikakalan.." dakika içerisinde kesmelisiniz!")
						say("Yoksa tüm grup üyeleri ejderha odasýndan atýlýr.")
						pc.setqf("icerdesin_"..pc.get_channel_id().."_",1)
						pc.setqf("ejder_bug_engel33",get_time()+10)
						wait()
						if get_time() > pc.getqf("ejder_bug_engel33") then
							say_title("Sura Ruhu:")
							say("10 saniye doldu þansýný tekrar dene.")
							pc.setqf("ejder_bug_engel33",get_time()+0)
							return
						end
						pc.warp(843600,1066900)
					elseif sifre != game.get_event_flag("ejder_sifresi_"..pc.get_channel_id().."_") then
						say_title("Sura Ruhu:")
						say("Üzgünüm þifreyi hatalý girdin.")
						return
					end
					
			end
			if game.get_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_") == 0 and pc.get_channel_id() == 1 and pc.count_item(30179) >= 1 then
			
					say_title("(CH1) Sura Ruhu:")
					say("Ejderha Tapýnaðý'na girmek isteyen ilk kiþi")
					say("sensin. Yeterli sayýda Kývrýk Anahtara sahipsen")
					say("seni içeri býrakacaðým. Ama ilk önce bir þifre")
					say("belirleyeceksin. Bu þifreyi, 30 dakika")
					say("içerisinde savaþ arkadaþlarýna verip onlarýn")
					say("içeri girmesini saðlayabilirsin. Bu anahtarýn")
					say("sadece 30 dakika tesir edeceðini unutma. O zamana")
					say("kadar ejderhayý öldürmüþ olmanýz gerekiyor!")
					say("")
					pc.setqf("ejder_bug_engel3",get_time()+10)
					local hikmet = dragonlair.hazirim()
					if get_time() > pc.getqf("ejder_bug_engel3") then
							say_title("Sura Ruhu:")
							say("10 saniye doldu þansýný tekrar dene.")
							pc.setqf("ejder_bug_engel3",get_time()+0)
							return
						end
						
					if hikmet == true and game.get_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_") == 0 then

						pc.setqf("ejder_bug_engel2",get_time()+10)
						say_title("Sura Ruhu:")
						say_size(350,350)
						say("")
						say("")
						dragonlair.arkaplan(1)
						local islem1 = number(1,3)
						local islem2 = number(1,3)
						local islem3 = number(1,3)
						local sayi1 = math.random(1,3)
						if sayi1 == 1 then
							dragonlair.resim(1)
						elseif sayi1 == 2 then
							dragonlair.resim(2)
						elseif sayi1 == 3 then
							dragonlair.resim(3)
						end
						local sayi2 = math.random(1,3)
						if sayi2 == 1 then
							dragonlair.resim2(1) 
						elseif sayi2 == 2 then
							dragonlair.resim2(2)
						elseif sayi2 == 3 then
							dragonlair.resim2(3)
						end
						local sayi3 = math.random(1,3)
						if sayi3 == 1 then
							dragonlair.resim3(1) 
						elseif sayi3 == 2 then
							dragonlair.resim3(2)
						elseif sayi3 == 3 then
							dragonlair.resim3(3)
						end
						local sayi4 = math.random(1,3)
						if sayi4 == 1 then
							dragonlair.resim4(1) 
						elseif sayi4 == 2 then
							dragonlair.resim4(2)
						elseif sayi4 == 3 then
							dragonlair.resim4(3)
						end
						local sayi5 = math.random(1,3)
						if sayi5 == 1 then
							dragonlair.resim5(1) 
						elseif sayi5 == 2 then
							dragonlair.resim5(2)
						elseif sayi5 == 3 then
							dragonlair.resim5(3)
						end
						local sayi6 = math.random(1,3)
						if sayi6 == 1 then
							dragonlair.resim6(1) 
						elseif sayi6 == 2 then
							dragonlair.resim6(2)
						elseif sayi6 == 3 then
							dragonlair.resim6(3)
						end
						local s1,s2,s3,pinn
						if islem1 == 1 then
							local arti = dragonlair.arti(1)
							s1 = sayi1+sayi2
						elseif islem1 == 2 then
							local arti = dragonlair.arti(1)
							s1 = sayi1+sayi2
						elseif islem1 == 3 then
							local carpi = dragonlair.carpi(1)
							s1 = sayi1*sayi2
						end
						if islem2 == 1 then
							local arti = dragonlair.artisayi3(1)
							s2 = sayi3+sayi4
						elseif islem2 == 2 then
							local arti = dragonlair.artisayi3(1)
							s2 = sayi3+sayi4
						elseif islem2 == 3 then
							local carpi = dragonlair.carpisayi3(1)
							s2 = sayi3*sayi4
						end
						if islem3 == 1 then
							local arti = dragonlair.artisayi4(1)
							s3 = sayi5+sayi6
						elseif islem3 == 2 then
							local carpi = dragonlair.carpisayi4(1)
							s3 = sayi5*sayi6
						elseif islem3 == 3 then
							local carpi = dragonlair.carpisayi4(1)
							s3 = sayi5*sayi6
						end
						pinn = ""..s1..""..s2..""..s3..""
						
							
						local pin1 = number(100,999)
						local pin2 = number(100,999)
						local pin3 = number(100,999)
						local pin4 = number(100,999)
						local pin5 = number(100,999)
						local pin6 = number(100,999)
						local gercek = number(2,7)
						if gercek == 2 then
							pin1 = pinn
						elseif gercek == 3 then
							pin2 = pinn
						elseif gercek == 4 then
							pin3 = pinn
						elseif gercek == 5 then
							pin4 = pinn
						elseif gercek == 6 then
							pin5 = pinn
						elseif gercek == 7 then
							pin6 = pinn
						end
						local cizgi = dragonlair.cizgi(1)
						say("10 saniye süren var!")
						pc.setqf("ejder_bug_engel333",get_time()+2)
						say_reward("[DELAY value;50]                    [/DELAY]") 
						local sifre = select("Vazgeç ",pin1,pin2,pin3,pin4,pin5,pin6)
						if sifre == 8 or sifre != gercek and sifre != 2 and sifre != 3 and sifre != 2 and sifre != 4 and sifre != 5 and sifre != 6 and sifre != 7 then
							return
						end
						if get_time() > pc.getqf("ejder_bug_engel2") then
							say_title("Sura Ruhu:")
							say("10 saniye doldu þansýný tekrar dene.")
							pc.setqf("ejder_bug_engel2",get_time()+0)
							return
						end
						if get_time() < pc.getqf("ejder_bug_engel333") then
							say_title("Sura Ruhu:")
							say("Bu acelen ne? Daha yeni þifreleri verdim.")
							say("Biraz düþünüp öyle seçmelisin þifreyi.")
							pc.setqf("ejder_bug_engel333",get_time()+0)
							return
						end
						if sifre == gercek and pc.count_item(30179)>=1 then
							say_title("Sura Ruhu:")
							say("")
							say("Arzu edersen seni doðrudan Ejderha Maðarasý'na")
							say("götürebilirim. Bana þimdi vermen gerekenler:")
							say("3x:")
							say_item_vnum(30179)
							say_reward("10 saniye süren var!")
							say("")
							pc.setqf("ejder_bug_engel3",get_time()+10)
							local zaxd = dragonlair.alvebirak()
							if get_time() > pc.getqf("ejder_bug_engel3") then
								say_title("Sura Ruhu:")
								say("10 saniye doldu þansýný tekrar dene.")
								pc.setqf("ejder_bug_engel3",get_time()+0)
								return
							end
							if zaxd == false then
								return
							elseif zaxd == true and game.get_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_") == 0 then
								if game.get_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_") != 0 then
									syschat("Ejderha odasýna baþka bir grup girdi.")
									return
								end
								--if pc.is_busy() then
									--return
								--else
									pc.remove_item("30179",1)
									pc.setqf("icerdesin_"..pc.get_channel_id().."_",1)
									pc.setqf("lider",1)
									pc.setqf("ejder_sifre",pinn)
									clear_server_timer("sure_bitti_"..pc.get_channel_id().."_",get_server_timer_arg())
									clear_server_timer("geri_dondu_"..pc.get_channel_id().."_",get_server_timer_arg())
									game.set_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_",1)
									game.set_event_flag("ejder_lonca_"..pc.get_channel_id().."_",pc.get_player_id())
									game.set_event_flag("ejder_sifresi_"..pc.get_channel_id().."_",pinn)
									game.set_event_flag("time_1_"..pc.get_channel_id().."_", get_time()+30*60*1)
									pc.warp(843600, 1066900)
								--end
							elseif zaxd == true and game.get_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_") != 0 then
								say_title("Sura Ruhu:")
								say("Birileri senden önce davrandý. Onlarýn")
								say("odadan çýkmalarýný bekle ya da parolayý ")
								say("biliyorsan söyle.")
							end
						elseif sifre != gercek then
							say_title("Sura Ruhu:")
							say("")
							say("Hatalý giriþ.")
						else
							say_title("Sura Ruhu:")
							say("")
							say("Bir hata oluþtu.")
							say("")
						end
					end
				end
			if game.get_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_") == 0 and pc.get_channel_id() == 2 and pc.count_item(30179) >= 1 then
					say_title("(CH2) Sura Ruhu:")
					say("Ejderha Tapýnaðý'na girmek isteyen ilk kiþi")
					say("sensin. Yeterli sayýda Kývrýk Anahtara sahipsen")
					say("seni içeri býrakacaðým. Ama ilk önce bir þifre")
					say("belirleyeceksin. Bu þifreyi, 30 dakika")
					say("içerisinde savaþ arkadaþlarýna verip onlarýn")
					say("içeri girmesini saðlayabilirsin. Bu anahtarýn")
					say("sadece 30 dakika tesir edeceðini unutma. O zamana")
					say("kadar ejderhayý öldürmüþ olmanýz gerekiyor!")
					say("")
					pc.setqf("ejder_bug_engel3",get_time()+10)
					local hikmet = dragonlair.hazirim()
					if get_time() > pc.getqf("ejder_bug_engel3") then
							say_title("Sura Ruhu:")
							say("10 saniye doldu þansýný tekrar dene.")
							pc.setqf("ejder_bug_engel3",get_time()+0)
							return
						end
					if hikmet == true and game.get_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_") == 0 then

						pc.setqf("ejder_bug_engel2",get_time()+10)
						say_title("Sura Ruhu:")
						say("")
						say("")
						say("")
						dragonlair.arkaplan(1)
						local islem1 = number(1,3)
						local islem2 = number(1,3)
						local islem3 = number(1,3)
						local sayi1 = math.random(1,3)
						if sayi1 == 1 then
							dragonlair.resim(1)
						elseif sayi1 == 2 then
							dragonlair.resim(2)
						elseif sayi1 == 3 then
							dragonlair.resim(3)
						end
						local sayi2 = math.random(1,3)
						if sayi2 == 1 then
							dragonlair.resim2(1) 
						elseif sayi2 == 2 then
							dragonlair.resim2(2)
						elseif sayi2 == 3 then
							dragonlair.resim2(3)
						end
						local sayi3 = math.random(1,3)
						if sayi3 == 1 then
							dragonlair.resim3(1) 
						elseif sayi3 == 2 then
							dragonlair.resim3(2)
						elseif sayi3 == 3 then
							dragonlair.resim3(3)
						end
						local sayi4 = math.random(1,3)
						if sayi4 == 1 then
							dragonlair.resim4(1) 
						elseif sayi4 == 2 then
							dragonlair.resim4(2)
						elseif sayi4 == 3 then
							dragonlair.resim4(3)
						end
						local sayi5 = math.random(1,3)
						if sayi5 == 1 then
							dragonlair.resim5(1) 
						elseif sayi5 == 2 then
							dragonlair.resim5(2)
						elseif sayi5 == 3 then
							dragonlair.resim5(3)
						end
						local sayi6 = math.random(1,3)
						if sayi6 == 1 then
							dragonlair.resim6(1) 
						elseif sayi6 == 2 then
							dragonlair.resim6(2)
						elseif sayi6 == 3 then
							dragonlair.resim6(3)
						end
						local s1,s2,s3,pinn
						if islem1 == 1 then
							local arti = dragonlair.arti(1)
							s1 = sayi1+sayi2
						elseif islem1 == 2 then
							local arti = dragonlair.arti(1)
							s1 = sayi1+sayi2
						elseif islem1 == 3 then
							local carpi = dragonlair.carpi(1)
							s1 = sayi1*sayi2
						end
						if islem2 == 1 then
							local arti = dragonlair.artisayi3(1)
							s2 = sayi3+sayi4
						elseif islem2 == 2 then
							local arti = dragonlair.artisayi3(1)
							s2 = sayi3+sayi4
						elseif islem2 == 3 then
							local carpi = dragonlair.carpisayi3(1)
							s2 = sayi3*sayi4
						end
						if islem3 == 1 then
							local arti = dragonlair.artisayi4(1)
							s3 = sayi5+sayi6
						elseif islem3 == 2 then
							local carpi = dragonlair.carpisayi4(1)
							s3 = sayi5*sayi6
						elseif islem3 == 3 then
							local carpi = dragonlair.carpisayi4(1)
							s3 = sayi5*sayi6
						end
						
						pinn = tonumber(s1..""..s2..""..s3)
						local cizgi = dragonlair.cizgi(1)
						say("10 saniye süren var!")
						local sifre = tonumber(input())
						if sifre < 100 or sifre > 999 or sifre == nil then
							say_title("Sura Ruhu:")
							say("Þifreyi lütfen 3 haneli yaz.")
							return
						end
						if get_time() > pc.getqf("ejder_bug_engel2") then
							say_title("Sura Ruhu:")
							say("15 saniye doldu þansýný tekrar dene.")
							pc.setqf("ejder_bug_engel2",get_time()+0)
							return
						end
						if sifre == pinn and pc.count_item(30179)>=1 then
							say_title("Sura Ruhu:")
							say("")
							say("Arzu edersen seni doðrudan Ejderha Maðarasý'na")
							say("götürebilirim. Bana þimdi vermen gerekenler:")
							say("1x:")
							say_item_vnum(30179)
							say("15 saniye süren var!")
							say("")
							pc.setqf("ejder_bug_engel3",get_time()+15)
							local zaxd = dragonlair.alvebirak()
							if get_time() > pc.getqf("ejder_bug_engel3") then
								say_title("Sura Ruhu:")
								say("10 saniye doldu þansýný tekrar dene.")
								pc.setqf("ejder_bug_engel3",get_time()+0)
								return
							end
							if zaxd == false then
								return
							elseif zaxd == true and game.get_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_") == 0 then
								if game.get_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_") != 0 then
									syschat("Ejderha odasýna baþka bir grup girdi.")
									return
								end
								--if pc.is_busy() then
									--return
								--else
									pc.remove_item("30179",1)
									pc.setqf("icerdesin_"..pc.get_channel_id().."_",1)
									pc.setqf("lider",1)
									pc.setqf("ejder_sifre",pinn)
									clear_server_timer("sure_bitti_"..pc.get_channel_id().."_",get_server_timer_arg())
									clear_server_timer("geri_dondu_"..pc.get_channel_id().."_",get_server_timer_arg())
									game.set_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_",1)
									game.set_event_flag("ejder_lonca_"..pc.get_channel_id().."_",pc.get_player_id())
									game.set_event_flag("ejder_sifresi_"..pc.get_channel_id().."_",pinn)
									game.set_event_flag("time_1_"..pc.get_channel_id().."_", get_time()+30*60*1)
									pc.warp(843600, 1066900)
								--end
							elseif zaxd == true and game.get_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_") != 0 then
								say_title("Sura Ruhu:")
								say("Birileri senden önce davrandý. Onlarýn")
								say("odadan çýkmalarýný bekle ya da parolayý ")
								say("biliyorsan söyle.")
							end
						else
							say_title("Sura Ruhu:")
							say("Þifreyi yanlýþ yazdýn!")
					end
				end
			end
		end
		function hazirim()
			local numara = number(1,2)
			if numara == 1 then
				local s = select("Hazýrým ","Vazgeç ","Vazgeç ")
				if s == 1 then
					return true
				elseif s == 2 then
					return false
				elseif s == 3 then
					return false
				end
			elseif numara == 2 then
				local s = select("Vazgeç ","Hazýrým ","Vazgeç ")
				if s == 1 then
					return false
				elseif s == 2 then
					return true
				elseif s == 3 then
					return false
				end
			end
		end

		function alvebirak()
			local numara = number(1,3)
			if numara == 1 then
				local s = select("Al ve beni içeri býrak ","Vazgeç ","Vazgeç ")
				if s == 1 then
					return true
				else
					return false
				end
			elseif numara == 2 then
				local s = select("Vazgeç ","Al ve beni içeri býrak ","Vazgeç ")
				if s == 2 then
					return true
				else
					return false
				end
			elseif numara == 3 then
				local s = select("Vazgeç ","Vazgeç ","Al ve beni içeri býrak ")
				if s == 3 then
					return true
				else
					return false
				end
			end
		end	
		when login begin
			if pc.getqf("icerdesin_"..pc.get_channel_id().."_") == 1 then
				send_letter("Mavi Ejderha'yý maðlup edin")
			end
		end
		
		when button or info begin
			q.set_clock("Kalan süre:", game.get_event_flag("time_1_"..pc.get_channel_id().."_")-get_time())
			dragonlair.ejder_odasi_kalansure()
		end
		
		when login with pc.get_map_index() == 207 and pc.getqf("icerdesin_"..pc.get_channel_id().."_") != 1 begin
			warp_to_village()
		end
		
		when login with pc.get_map_index() == 207 and pc.getqf("lider") == 1 begin
			local lonca = game.get_event_flag("ejder_lonca_"..pc.get_channel_id().."_")
			local id = pc.get_player_id()
			if lonca != id then
				pc.delqf("icerdesin_1_")
				pc.delqf("icerdesin_2_")
				pc.delqf("ejder_sifre")
				pc.delqf("lider")
				pc.delqf("ejder_bug_engel333")
				pc.delqf("ejder_bug_engel33")
				pc.delqf("ejder_bug_engel3")
				pc.delqf("ejder_bug_engel2")
				pc.delqf("ejder_sifre_kontrol")
				syschat("Þifre sende olmadýðý için Sura Ruhu giriþine atýldýn!")
				pc.give_item2(30179,1)
				pc.warp(180100,1220400)
				return
			end
			purge_area(829200,1059600,844200,1074600)
			regen_in_map(207,"data/dungeon/skia_deliboss.txt")
			notice_all("CH"..pc.get_channel_id()..": "..pc.get_name().." grubu, Mavi Ejderha'yý öldürmeye çalýþýyor!")
			notice(""..pc.getqf("ejder_sifre").." numaralý þifreyi daðýtmaya 30 dakika zamanýn var!")
			server_timer("sure_bitti_"..pc.get_channel_id().."_", 1800,pc.get_map_index())
			dragonlair.ejder_odasi_kalansure()
			pc.setqf("lider",2)
			pc.set_warp_location(73 , 1801, 12204)
		end
		
		when login with pc.get_map_index() == 207 and pc.getqf("icerdesin_"..pc.get_channel_id().."_") == 1 and pc.getqf("lider") != 1 begin		
			if pc.getqf("ejder_sifre_kontrol") != game.get_event_flag("ejder_sifresi_"..pc.get_channel_id().."_") then
				warp_to_village()
			end
			dragonlair.ejder_odasi_kalansure()
			pc.set_warp_location(73 , 1801, 12204)
		end
		
		when kill with pc.get_map_index() == 207 and npc.get_race() == 8031 or npc.get_race() == 8032 or  npc.get_race() == 8033 or npc.get_race() == 8034 begin
			local n =game.get_event_flag("kesilen_metin_"..pc.get_channel_id().."_") + 1
			game.set_event_flag("kesilen_metin_"..pc.get_channel_id().."_", n)
			if n > 3 then
			local t = number(1,4)
				if 1 == t then
				regen_in_map(207,"data/dungeon/skia_boss1.txt")
				elseif 2 == t then
				regen_in_map(207,"data/dungeon/skia_boss2.txt")
				elseif 3 == t then
				regen_in_map(207,"data/dungeon/skia_boss3.txt")
				else
				regen_in_map(207,"data/dungeon/skia_boss4.txt")
				end
			end
		end

		when kill with pc.get_map_index() == 207 and game.get_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_") >= 1 and npc.get_race() == 2493 begin
			game.drop_item_with_ownership("71123",1)
			game.drop_item_with_ownership("71129",1)
			local drop_chance = number(1,10)
				if drop_chance == 5 then
				game.drop_item_with_ownership("71129",1)
				end
			notice_all("CH"..pc.get_channel_id()..": "..pc.get_name().." grubu, Mavi Ejderha'yý öldürdü!")
			game.set_event_flag("kesilen_metin_"..pc.get_channel_id().."_",0)
			clear_server_timer("sure_bitti_"..pc.get_channel_id().."_",get_server_timer_arg())
			server_timer("geri_dondu_"..pc.get_channel_id().."_",60*1, pc.get_map_index())
			game.set_event_flag("ejder_sifresi_"..pc.get_channel_id().."_",number(9999,99999))	
		end
		
		when geri_dondu_1_.server_timer begin
			notice_all("CH"..pc.get_channel_id()..": Mavi Ejderha geri döndü!")
			purge_area(829200,1059600,844200,1074600)
			clear_server_timer("sure_bitti_"..pc.get_channel_id().."_",get_server_timer_arg())
			game.set_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_",0)
			forked.warp_all_in_map("207","73","184500","1220700",1)
		end
		
		when geri_dondu_2_.server_timer begin
			notice_all("CH"..pc.get_channel_id()..": Mavi Ejderha geri döndü!")
			purge_area(829200,1059600,844200,1074600)
			clear_server_timer("sure_bitti_"..pc.get_channel_id().."_",get_server_timer_arg())
			game.set_event_flag("ejder_kullanimda_"..pc.get_channel_id().."_",0)
			forked.warp_all_in_map("207","73","184500","1220700",1)
		end
		
		when sure_bitti_1_.server_timer with game.get_event_flag("time_1_1_") - get_time() <= 0 begin
			notice_all("CH"..pc.get_channel_id()..": Mavi Ejderha geri döndü!")
			purge_area(829200,1059600,844200,1074600)
			forked.warp_all_in_map("207","73","184500","1220700",1)
		end
		
		when sure_bitti_2_.server_timer with game.get_event_flag("time_1_2_") - get_time() <= 0 begin
			notice_all("CH"..pc.get_channel_id()..": Mavi Ejderha geri döndü!")
			purge_area(829200,1059600,844200,1074600)
			forked.warp_all_in_map("207","73","184500","1220700",1)
		end
		when 30121.chat."Süreyi sýfýrla" with pc.is_gm() begin
			say_title("Sura Ruhu;")
			say("Sýfýrlamak istediðinden emin misin ?")
			local cyvercha = select("Evet","Hayýr")
			if cyvercha == 2 then
				return
			elseif cyvercha == 1 then
				notice_all("CH "..pc.get_channel_id()..": mavi ejderha süresi sýfýrlanmýþtýr.")
				game.set_event_flag("time_1_"..pc.get_channel_id().."",get_time() + 600)
				clear_server_timer("sure_bitti_"..pc.get_channel_id().."",get_server_timer_arg())
				server_timer("geri_dondu_"..pc.get_channel_id().."_",60*1, pc.get_map_index())
				forked.warp_all_in_map("207","73","180100","1220400",295)
			end
		end
		when 30121.chat."GM : Soguma süremi sýfýrla" with pc.is_gm() begin
			say_title("Sura Ruhu;")
			say("Sýfýrlamak istediðinden emin misin ?")
			local cyvercha = select("Evet","Hayýr")
			if cyvercha == 2 then
				return
			elseif cyvercha == 1 then
				say("Soguma süren sýfýrlandý.")
				pc.setf("dragonlair", "dragonlair_soguma_suresi", 0)
			end
		end
		function ejder_odasi_kalansure()    
			local saniyekalan = (game.get_event_flag("time_1_"..pc.get_channel_id().."_") - get_time())
            local dakikakalan = math.ceil(saniyekalan / 60)    	
            notice("Mavi Ejderha'yý öldürmeye "..dakikakalan.." dakikalýk bir zaman kaldý!")  
        end
	end
end