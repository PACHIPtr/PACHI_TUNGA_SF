--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest oxevent_manager begin
	state start begin
		when 20011.chat."OX-Yarýþmasý " begin
			say_title("Uriel:")
			say("Hey Evet Sen.")
			say("Sen gerçekten çok zeki görünüyorsun.")
			say("Yarýþmanýn ismi Ox Yarýþmasý. ") 
			say("Sorular sorulacak cevap vereceksin,")
			say("bilirsen süpriz bir hediye kazanacaksýn.")

			if game.get_event_flag("oxevent_status") == 0 then
				wait()
				say_title("Uriel:")
				say("Yarýþmaya katýlmana izin veriyorum.")
				say("Ýstersen ayrýca izleyici de olabilirsin.")
				say("Baþlama zamaný henüz kararlaþtýrýlmadý.")
				say("Zaman geldiðinde seni bilgilendireceðim.")
				say("Tetikte kal!")
			elseif game.get_event_flag("oxevent_status") == 1 then
				wait()
				say_title("Uriel:")
				say("Yarýþmaya katýlmak istiyormusun?")

				local s = select("Evet", "Hayýr", "Ýzlemek istiyorum.")

				if s == 1 then
				if game.get_event_flag("oxevent_status") == 1 then
					if pc.get_level() < 1 then
					say_title("Uriel:")
					say("OX-Yarýþmasýna katýlabilmen için en az")
					say("1. seviyede olmalýsýn.")
					say("")
					elseif pc.is_riding() then
					say_title("Uriel:")
					say("Binek veya at kullanýrken OX yarýþmasýna giremezsin.")
					else
					pc.warp(896500, 24600)
					end
				elseif game.get_event_flag("oxevent_status") == 2 then
				say("Yarýþma baþladý, giremezsin!")
				end
				elseif s == 3 then
					say_title("Uriel:")
					say("Ýzleyici olarak mý katýlmak istiyorsun?")
					say("Tamam, ýþýnlýyorum seni.")

					wait()

					pc.warp(896300, 28900)
				end
			elseif game.get_event_flag("oxevent_status") == 2 then
				
					say_title("Uriel:")
					say("Yarýþma baþladý, giremezsin!")
					say("Ancak izleyici olarak katýlabilirsin.")

					local s = select("Katýl", "Katýlma")
					if s == 1 then
						say_title("Uriel:")
						say("Tamam, ýþýnlýyorum seni.")

						wait()

						pc.warp(896300, 28900)
					end
			end
		end

		when 20358.chat."OX-Yarýþmasý " with pc.is_gm() begin

			local stat = oxevent.get_status()

			if stat == 0 then
				
				say_title("OX Çiçekleri: ")
				say("Yarýþmayý baþlatmak istiyormusun? ")


				local s = select("Evet", "Hayýr ")

				if s == 1 then
					local v = oxevent.open()

					if v == 0 then
						say_title("OX Çiçekleri: ")
						say("OX-Yarýþmasý zaten baþlatýlmýþ. ")
					elseif v == 1 then
						say_title("OX Çiçekleri:")
						say("Ox-Yarýþmasý baþarýyla baþlatýldý. ")
						notice_all("Ox Yarýþmasý baþladý! Uriel'den giriþ yapabilirsin. ")
					else
						say_title("OX Çiçekleri: ")
						say("Bir hata meydana geldi. ")
					end
				end
			elseif stat == 1 then
					say_title("OX Çiçekleri:")
					say("Ox-Yarýþmasýnda "..oxevent.get_attender().." kiþi var. ")
					say("Yarýþmayý baþlatmak istiyormusun? ")

					local s = select("Evet", "Hayýr ")

					if s == 1 then
						oxevent.close()

						say_title("OX Çiçekleri: ")
						say("Giriþler kapatýldý.")
						say("Yarýþma baþlamasý için ayný iþlemi yapýn! ")
					end
			elseif stat == 2 then

					say_title("OX Çiçekleri:")
					say("Ne Yapmak Ýstiyorsunuz?")

					local s = select("Yarýþmayý baþlat ", "Ýtem ver ", "Yarýþmayý sonlandýr ","Konuþmayý Engelle", "Kapat")

					if s == 1 then
						local v = oxevent.quiz(1, 30)

						if v == 0 then
							say_title("OX Çiçekleri: ")
							say("Yarýþma baþlatýlamadý! ")
						elseif v == 1 then
							say_title("OX Çiçekleri: ")
							say("Soru gönderildi! ")
						else
							say_title("OX Çiçekleri: ")
							say("Zaten bir soru sordunuz! ")
						end
					elseif s == 2 then
						say_title("OX Çiçekleri: ")
						say("Verilecek itemin kodunu gir. ")

						local item_vnum = input()

						say_title("OX Çiçekleri: ")
						say("Verilecek itemin adetini gir. ")

						local item_count = input()

						oxevent.give_item(item_vnum, item_count)

						say_title("OX Çiçekleri: ")
						say(""..item_count.." adet "..item_name(item_vnum)..", "..oxevent.get_attender().." adlý ")
						say("oyuncuya verildi.")
					elseif s == 3 then
						oxevent.end_event()

						notice_all("Ox Yarýþmasý sona erdi.")

						say_title("OX Çiçekleri: ")
						say("Ox-Yarýþmasý bitirildi. ")
					elseif s == 4 then
						say_title("OX Çiçekleri: ")
						say("Ne yapmak istiyorsunuz? ")
						local s = select("Konuþmayý Engelle ","Konuþma Engelini Aç ","Ýptal ")
						if s == 1 then
						say_title("OX Çiçekleri: ")
						say("OX yarýþmasýnda konuþma engellendi. ")
						game.set_event_flag("ox_chat",1)
						elseif s == 2 then
						say("OX yarýþmasýnda konuþma engeli açýldý. ")
						game.set_event_flag("ox_chat",0)
						elseif s == 3 then
							return
						end
					end
			elseif stat == 3 then
					say_title("OX Çiçekleri: ")
					say("Lütfen bekleyiniz. ")
				else
					say("Bir hata oluþtu! ")
				end
			end
			
			

		when login with pc.get_map_index() == 113 and not pc.is_gm() begin
			if pc.is_mount() then
				pc.unmount()
				horse.unsummon()
			end
		end
	end
end

