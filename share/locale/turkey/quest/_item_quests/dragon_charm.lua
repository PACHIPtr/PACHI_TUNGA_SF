--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest dragon_charm begin
	state start begin
		function kontrol()
			if pc.is_busy() == true then return true end
			return false
		end
		when login begin
			if pc.get_map_index() == 72 or pc.get_map_index() == 73 or pc.get_map_index() == 79 then
				if pc.get_level() < 75 then
					chat("Seviyen 75'in altýnda olduðu için buraya giremezsin.")
					warp_to_village()
				end
			end
		end
		when 30180.use begin
			if pc.count_item(30180) < 1 then 
				say_title("Bug Kontrol") 
				say("Envanterindeki Ejderha Tanrý Týlsýmý kaybolmuþ.") 
				return 
			end
			if pc.get_level() < 75 then syschat("Seviyen yeterli deðil.") return end
			if pc.get_map_index() == 72 and not pc.is_gm() then
				say_title("Ejderha Tanrý Týlsýmý ")
				say("")
				say("Eðer gitmek istediðin yer Sürgün Maðarasý'nýn")
				say("2. katýnda bulunuyorsa, öncelikle 2. kata")
				say("geçmen gerekiyor. Orada týlsýmý kullandýðýn")
				say("zaman farklý seçenekler çýkacak.")
				say("")
				say_reward("Birini seç.")
				local s = select("Karanlýk Tapýnak","Sura Ýskeletleri","Maðaranýn sonu","Vazgeç ")
				if s == 1 then
					say_title("Ejderha Tanrý Týlsýmý ")
					say("")
					say("Karanlýk Tapýnak yakýnlarýna ýþýnlanýyorsun...")
					pc.remove_item("30180",1)
					wait()
					pc.warp(82900,1286200)
				elseif s == 2 then
					say_title("Ejderha Tanrý Týlsýmý ")
					say("Birini seç.")
					local a = select("Sura Ýskeleti-1","Sura Ýskeleti-2","Sura Ýskeleti-3","Sura Ýskeleti-4","Vazgeç ")
					if a == 1 then
						say_title("Ejderha Tanrý Týlsýmý ")
						say("")
						say("Sura Ýskeleti-1 yakýnlarýna ýþýnlanýyorsun...")
						pc.remove_item("30180",1)
						wait()
						pc.warp(20800,1290300)
					elseif a == 2 then
						say_title("Ejderha Tanrý Týlsýmý ")
						say("")
						say("Sura Ýskeleti-2 yakýnlarýna ýþýnlanýyorsun...")
						pc.remove_item("30180",1)
						wait()
						pc.warp(11800,1334300)
					elseif a == 3 then
						say_title("Ejderha Tanrý Týlsýmý ")
						say("")
						say("Sura Ýskeleti-3 yakýnlarýna ýþýnlanýyorsun...")
						pc.remove_item("30180",1)
						wait()
						pc.warp(48300,1340700)
					elseif a == 4 then
						say_title("Ejderha Tanrý Týlsýmý ")
						say("")
						say("Sura Ýskeleti-4 yakýnlarýna ýþýnlanýyorsun...")
						pc.remove_item("30180",1)
						wait()
						pc.warp(42000,1268200)
					elseif a == 5 then
						return
					end
				elseif s == 3 then
					say_title("Ejderha Tanrý Týlsýmý ")
					say("")
					say("Maðaranýn 1. katýnýn sonuna ýþýnlanýyorsun...")
					pc.remove_item("30180",1)
					wait()
					pc.warp(131900,1343700)
				elseif s == 4 then
					return
				end
				
			elseif pc.get_map_index() == 73 and not pc.is_gm() then
				say_title("Ejderha Tanrý Týlsýmý ")
				say("")
				say("Sürgün Maðarasý 2. kattasýn. Gidebileceðin")
				say("yerler aþaðýda. Eðer gitmek istediðin yer")
				say("1. katta ise önce 1. kata dönmelisin.")
				say("")
				say_reward("Birini seç.")
				local ss = select("Komutanlar","Sura Ruhu","Sura Ýskeleti-5","Vazgeç ")
				if ss == 1 then
					say_title("Ejderha Tanrý Týlsýmý ")
					say("Birini seç.")
					local aa = select("Komutan","General (1)","General (2)","General (3)","Vazgeç ")
					if aa == 1 then
						say_title("Ejderha Tanrý Týlsýmý ")
						say("")
						say("Komutanýn yakýnlarýna ýþýnlanýyorsun...")
						pc.remove_item("30180",1)
						wait()
						pc.warp(195800,1220600)
					elseif aa == 2 then
						say_title("Ejderha Tanrý Týlsýmý ")
						say("")
						say("General (1) yakýnlarýna ýþýnlanýyorsun...")
						pc.remove_item("30180",1)
						wait()
						pc.warp(279600,1260800)
					elseif aa == 3 then
						say_title("Ejderha Tanrý Týlsýmý ")
						say("")
						say("General (2) yakýnlarýna ýþýnlanýyorsun...")
						pc.remove_item("30180",1)
						wait()
						pc.warp(191100,1233400)
					elseif aa == 4 then
						say_title("Ejderha Tanrý Týlsýmý ")
						say("")
						say("General (3) yakýnlarýna ýþýnlanýyorsun...")
						pc.remove_item("30180",1)
						wait()
						pc.warp(229700,1317600)
					elseif aa == 5 then
						return
					end
				elseif ss == 2 then
					say_title("Ejderha Tanrý Týlsýmý ")
					say("")
					say("Sura Ruhu yakýnlarýna ýþýnlanýyorsun...")
					pc.remove_item("30180",1)
					wait()
					pc.warp(180100,1220400)
				elseif ss == 3 then
					say_title("Ejderha Tanrý Týlsýmý ")
					say("")
					say("Sura Ýskeleti-5 yakýnlarýna ýþýnlanýyorsun...")
					pc.remove_item("30180",1)
					wait()
					pc.warp(251600,1296000)
				elseif ss == 4 then
					return
				end
			elseif pc.is_gm() then
				local aa = select("Maðaranýn sonu","Sura Ruhu","Komutan","General (1)","General (2)","General (3)","Vazgeç ")
					if aa == 1 then
						say_title("Ejderha Tanrý Týlsýmý ")
						say("")
						say("Maðaranýn 1. katýnýn sonuna ýþýnlanýyorsun...")
						wait()
						pc.warp(131900,1343700)
					elseif aa == 2 then
						say_title("Ejderha Tanrý Týlsýmý ")
						say("")
						say("Sura Ruhu yakýnlarýna ýþýnlanýyorsun...")
						wait()
						pc.warp(180100,1220400)
					elseif aa == 3 then
						say_title("Ejderha Tanrý Týlsýmý ")
						say("")
						say("Komutanýn yakýnlarýna ýþýnlanýyorsun...")
						wait()
						pc.warp(195800,1220600)
					elseif aa == 4 then
						say_title("Ejderha Tanrý Týlsýmý ")
						say("")
						say("General (1) yakýnlarýna ýþýnlanýyorsun...")
						wait()
						pc.warp(279600,1260800)
					elseif aa == 5 then
						say_title("Ejderha Tanrý Týlsýmý ")
						say("")
						say("General (2) yakýnlarýna ýþýnlanýyorsun...")
						wait()
						pc.warp(191100,1233400)
					elseif aa == 6 then
						say_title("Ejderha Tanrý Týlsýmý ")
						say("")
						say("General (3) yakýnlarýna ýþýnlanýyorsun...")
						wait()
						pc.warp(229700,1317600)
					elseif aa == 7 then
						return
					end
			else
				say_title("Ejderha Tanrý Týlsýmý ")
				say("")
				say("Týlsýmý kullanmak için Sürgün Maðarasý ")
				say("içerisinde olman gerekiyor.")
				say("")
			end
		end
	end
end