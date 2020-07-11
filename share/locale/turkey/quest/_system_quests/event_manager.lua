--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest event_manager begin
	state start begin
		when letter with pc.is_gm() == true begin
			if pc.is_gm() == true then
				send_letter("*GM: Etkinlik Yönetimi")
			end
		end
		when button or info with pc.is_gm() == true begin
			say_size(350,400)
			if pc.is_gm() == false then
				say_title("Etkinlik Yönetimi:")
				say("")
				say("Yönetici olmadýðýnýz için bunu")
				say("kullanamazsýnýz.")
			else
				say_title("Etkinlik Yönetimi:")
				say("")
				say_reward("Hangi etkinliði açmak istiyorsun?")
				local events = select("Ayýþýðý Define Sandýðý ", "Altýgen Hediye Paketi ", "Oyma Taþ ", "Ramazan Bayramý ", "Noel ", "Paskalya ", "Futbol Topu ", "Sevgililer Günü ", "Evcil Hayvan ", "Bulmaca Kutusu ", "Cadýlar Bayramý ", "Olimpiyat ", "Okey Kart ", "Binek Hayvaný ", "Derece ", "Yýldönümü ", "Patron Avcýlarý ", "Balýk Yapboz ", "Madencilik ", "Yaðmacýlarý Avla ", "Tatlý Yapým ", "Kralý Yakala ", "Yang Yaðmuru ", "Süper Metin ", "Vazgeç ")
				if events == 1 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("ex_moon_drop") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("ex_moon_drop",1)
							game.set_event_time(0, (sure*60))
							notice_all("Ayýþýðý define sandýðý etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("ex_moon_drop",0)
							game.set_event_time(0, 0)
							notice_all("Ayýþýðý define sandýðý etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 2 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("ex_hexagon_drop") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("ex_hexagon_drop",1)
							game.set_event_time(1, (sure*60))
							notice_all("Altýgen hediye paketi etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("ex_hexagon_drop",0)
							game.set_event_time(1, 0)
							notice_all("Altýgen hediye paketi etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 3 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("ex_carving_stone_drop") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("ex_carving_stone_drop",1)
							game.set_event_time(2, (sure*60))
							notice_all("Oyma taþ etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("ex_carving_stone_drop",0)
							game.set_event_time(2, 0)
							notice_all("Oyma taþ etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 4 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("ex_ramadan_drop") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("ex_ramadan_drop",1)
							game.set_event_time(3, (sure*60))
							notice_all("Ramazan bayramý etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("ex_ramadan_drop",0)
							game.set_event_time(3, 0)
							notice_all("Ramazan bayramý etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 5 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("ex_xmas_drop") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("ex_xmas_drop",1)
							game.set_event_time(4, (sure*60))
							notice_all("Noel etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("ex_xmas_drop",0)
							game.set_event_time(4, 0)
							notice_all("Noel etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 6 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("enable_easter_event") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("enable_easter_event",1)
							game.set_event_time(5, (sure*60))
							notice_all("Paskalya etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("enable_easter_event",0)
							game.set_event_time(5, 0)
							notice_all("Paskalya etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 7 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("ex_football_drop") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("ex_football_drop",1)
							game.set_event_time(6, (sure*60))
							notice_all("Futbol topu etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("ex_football_drop",0)
							game.set_event_time(6, 0)
							notice_all("Futbol topu etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 8 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("ex_valentine_drop") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("ex_valentine_drop",1)
							game.set_event_time(7, (sure*60))
							notice_all("Sevgililer günü etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("ex_valentine_drop",0)
							game.set_event_time(7, 0)
							notice_all("Sevgililer günü etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 9 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("ex_pet_drop") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("ex_pet_drop",1)
							game.set_event_time(8, (sure*60))
							notice_all("Evcil hayvan etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("ex_pet_drop",0)
							game.set_event_time(8, 0)
							notice_all("Evcil hayvan etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 10 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("ex_kids_drop") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("ex_kids_drop",1)
							game.set_event_time(9, (sure*60))
							notice_all("Bulmaca kutusu etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("ex_kids_drop",0)
							game.set_event_time(9, 0)
							notice_all("Bulmaca kutusu etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 11 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("ex_halloween_drop") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("ex_halloween_drop",1)
							game.set_event_time(10, (sure*60))
							notice_all("Cadýlar bayramý etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("ex_halloween_drop",0)
							game.set_event_time(10, 0)
							notice_all("Cadýlar bayramý etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 12 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("ex_olympic_drop") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("ex_olympic_drop",1)
							game.set_event_time(11, (sure*60))
							notice_all("Olimpiyat etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("ex_olympic_drop",0)
							game.set_event_time(11, 0)
							notice_all("Olimpiyat etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 13 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("cards_event") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("cards_event",1)
							game.set_event_time(12, (sure*60))
							notice_all("Okey kart etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("cards_event",0)
							game.set_event_time(12, 0)
							notice_all("Okey kart etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 14 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("ex_mount_drop") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("ex_mount_drop",1)
							game.set_event_time(13, (sure*60))
							notice_all("Binek parþomen etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("ex_mount_drop",0)
							game.set_event_time(13, 0)
							notice_all("Binek parþomen etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 15 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("alignment_event") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("alignment_event",1)
							game.set_event_time(14, (sure*60))
							notice_all("Derece etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("alignment_event",0)
							game.set_event_time(14, 0)
							notice_all("Derece etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 16 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("2017_year_round_event") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("2017_year_round_event",1)
							game.set_event_time(15, (sure*60))
							notice_all("Yýldönümü etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("2017_year_round_event",0)
							game.set_event_time(15, 0)
							notice_all("Yýldönümü etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 17 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("enable_attendance_event") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("enable_attendance_event",1)
							game.set_event_time(16, (sure*60))
							notice_all("Patron avcýlarý etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("enable_attendance_event",0)
							game.set_event_time(16, 0)
							notice_all("Patron avcýlarý etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 18 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("2018_fish_event") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("2018_fish_event",1)
							game.set_event_time(17, (sure*60))
							notice_all("Balýk yapboz etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("2018_fish_event",0)
							game.set_event_time(17, 0)
							notice_all("Balýk yapboz etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 19 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("mining_event") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("mining_event",1)
							game.set_event_time(18, (sure*60))
							notice_all("Madencilik etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("mining_event",0)
							game.set_event_time(18, 0)
							notice_all("Madencilik etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 20 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("harvest_festival") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("harvest_festival",1)
							game.set_event_time(19, (sure*60))
							notice_all("Yaðmacýlarý avla etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("harvest_festival",0)
							game.set_event_time(19, 0)
							notice_all("Yaðmacýlarý avla etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 21 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("newyear_wonso") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("newyear_wonso",1)
							game.set_event_time(20, (sure*60))
							notice_all("Yaðmacýlarý avla etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("newyear_wonso",0)
							game.set_event_time(20, 0)
							notice_all("Yaðmacýlarý avla etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 22 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("enable_catch_king_event") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("enable_catch_king_event",1)
							game.set_event_time(21, (sure*60))
							notice_all("Kralý yakala etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("enable_catch_king_event",0)
							game.set_event_time(21, 0)
							notice_all("Kralý yakala etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 23 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("golden_rain_event") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("golden_rain_event",1)
							game.set_event_time(22, (sure*60))
							notice_all("Yang yaðmuru etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("golden_rain_event",0)
							game.set_event_time(22, 0)
							notice_all("Yang yaðmuru etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 24 then
					say_title("Etkinlik Yönetimi:")
					say("")
					if game.get_event_flag("super_metin_event") > 0 then
						say_reward("Etkinlik þu anda aktif!")
					else
						say_reward("Etkinlik þu anda kapalý.")
					end
					say_reward("Etkinlik hakkýnda ne yapmak istiyorsun ?")
					local sec = select("Etkinliði Aktif Et ","Etkinliði Kapat ","Vazgeç ")
					if sec == 1 then
						say_title("Etkinlik Yönetimi:")
						say("")
						say_reward("Ne kadar süre açýk kalacaðýný girin.")
						say_reward("Yazýlacak rakam dakika cinsindendir.")
						local sure = tonumber(input())
						if sure == nil then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure == "" then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Süre boþ býrakýlamaz.")
						elseif sure < 0 then
							say_title("Etkinlik Yönetimi:")
							say("")
							say_reward("Sýfýrdan küçük deðer girilemez.")
						else
							game.set_event_flag("super_metin_event",1)
							game.set_event_time(23, (sure*60))
							notice_all("Süper metin etkinliði aktif!")
							notice_all(string.format("Etkinlik Süresi: %s", LIB_duration(sure*60)))
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					elseif sec == 2 then
						say_title("Etkinlik Yönetimi: ")
						say("")
						say_reward("Etkinliði gerçekten kapatmak istiyor musun?")
						local onay = select("Evet","Hayýr")
						if onay == 1 then
							game.set_event_flag("super_metin_event",0)
							game.set_event_time(23, 0)
							notice_all("Süper metin etkinliði sona erdi!")
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						else
							send_letter("*GM: Etkinlik Yönetimi")
							setskin(NOWINDOW)
						end
					else
						send_letter("*GM: Etkinlik Yönetimi")
						setskin(NOWINDOW)
					end
				elseif events == 25 then
					send_letter("*GM: Etkinlik Yönetimi")
					setskin(NOWINDOW)
				end
			end
		end
	end
end