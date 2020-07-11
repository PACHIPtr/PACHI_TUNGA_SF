--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest servis begin
	state start begin
		function TeleportIsAvailable()
		local maps = { 5, 7, 25, 27, 28, 45, 47, 79, 81, 112, 206, 211, 212, 213 } 
			if pc.in_dungeon() then
				return true
			end
			for i = 1, table.getn(maps) do
				if pc.get_map_index() == maps[i] then
					return true
				end
			end
			return true
		end
		
		
			
		when 40003.use begin
		
		local menu = {}
		local sub_menu = {}
			
		if servis.TeleportIsAvailable() then
			table.insert(menu,"Iþýnlanma")
			table.insert(sub_menu,1)
		end
			
		table.insert(menu,"Kapat")
		table.insert(sub_menu,-1)
		
		local menu_list = select_table(menu)

			if sub_menu[menu_list] == -1 then
				return
			end
			
			if sub_menu[menu_list] == 1 then
				if pc.get_level() < 5 then
					say_title("Iþýnlanma: ")
					say("")
					say_reward("Bu iþlemi yapabilmek için 5 seviye olmalýsýn.")
					return
				end
			
				local s = select("Þehir Bölgeleri","Orman Bölgeleri ","Örümcek Zindaný ","Sürgün Maðarasý (Lv.75~90)","Diðer Bölgeler ","Farm Bölgeleri ","Farm Bölgeleri 2 ","Karanlýk Ejderha Kayasý(+90) ","Büyülü Orman(+95) ","Sihirli Orman(+115) ","Vazgeç ")
				if s == 1 then
					local ss = select("Kýrmýzý Bayrak 1. Köy","Kýrmýzý Bayrak 2. Köy","Sarý Bayrak 1. Köy","Sarý Bayrak 2. Köy","Mavi Bayrak 1. Köy","Mavi Bayrak 2. Köy","Vazgeç ")
					local warps_1 = { 465800,958200, 357400,877200, 52100,154100, 137100,239400, 965000,273900, 867800,240700 }	
					if ss == 7 then
						return
					end
					pc.warp(warps_1[2*ss-1],warps_1[2*ss])
				elseif s == 2 then
					local sss = select("Hayalet Orman (Lv.60~75)","Kýzýl Orman Giriþ (Lv.65~75)","Kýzýl Orman Sonu (Lv.65~75)","Vazgeç ")
					local warps_2 = { 288700,5700, 1119900,70800, 1118100,8600 }
					if sss == 4 then
						return
					end
					pc.warp(warps_2[2*sss-1],warps_2[2*sss])
				elseif s == 3 then
					local zindan = select("Örümcek Zindaný 1 (Lv.45~55)","Örümcek Zindaný 2 (Lv.55~75)","Vazgeç ")
					local warps_3 = { 59800,497300, 704100,521900 }
					if zindan == 3 then
						return
					end
					pc.warp(warps_3[2*zindan-1],warps_3[2*zindan])
				elseif s == 4 then
					pc.warp(284400,810700)
				elseif s == 5 then
					local ssss = select("Seungryong Vadisi (Lv.34~50)","Sohan Daðý (Lv.49~65)","Yongbi Çölü (Lv.37~50)","Doyyumhwan (Lv.45~72)","Devler Diyarý ","Þeytan Kulesi (Lv.40~69)","Þeytan Katakombu (Lv.75~80)","Vazgeç ")
					local warps_4 = { 331400,755600, 436400,215600, 296800,547400, 600800,687400, 829900,763300, 590500,110500, 591100,99300 }
					if ssss == 8 then
						return
					end
					pc.warp(warps_4[2*ssss-1],warps_4[2*ssss])
				elseif s == 6 then
					local ssss = select("Seungryong Vadisi (Farm 1) ","Seungryong Vadisi (Farm 2) ","Seungryong Vadisi (Farm 3) ","Doyyumhwan (Farm 1) ","Doyyumhwan (Farm 2) ","Doyyumhwan (Farm 3) ","Vazgeç ")
					local warps_5 = { 395300,716000, 292900,713500, 336000,779500, 634600,687200, 652500,663900, 724800,657800 }
					if ssss == 7 then
						return
					end
					pc.warp(warps_5[2*ssss-1],warps_5[2*ssss])
				elseif s == 7 then
					local sssss = select("Yýlan Vadisi (Farm 1) ","Yýlan Vadisi (Farm 2) ","Sohan Daðý (Farm) ","Kýzýl Orman (Farm 1) ","Kýzýl Orman (Farm 2) ","Kýzýl Orman (Farm 3) ","Vazgeç ")
					local warps_5 = { 1061300,787700, 1106200,785900, 443300,233300, 1096800,21500, 1077600,15700, 1099800,22300 }
					if sssss == 7 then
						return
					end
					pc.warp(warps_5[2*sssss-1],warps_5[2*sssss])
				elseif s == 8 then
					local beta = select("Nefrit Körfezi (Lv.90~120)", "Ejderha Atesi Burnu (Lv.90~120)", "Gautama Uçurumu (Lv.90~120)", "Yýldýrým Daðlarý (Lv.90~120)", "Vazgeç ")
					local warps_beta = {1049600,1510400, 1024000,1664000, 1177600,1664000, 1126400,1510400}
					if pc.get_level() < 90 then
						syschat("Seviyen 90 olmadan bu haritaya giriþ yapamazsýn.")
						return
					end
					if beta == 5 then
						return
					end
					pc.warp(warps_beta[2*beta-1],warps_beta[2*beta])
				elseif s == 9 then
					local buyulu = select("Büyülü Orman (95~120)","Vazgeç ")
					local warps_buyulu = { 817100,1505000 }
					if pc.get_level() < 95 then
						syschat("Seviyen 95 olmadan bu haritaya giriþ yapamazsýn.")
						return
					end
					if buyulu == 2 then
						return
					end
					pc.warp(warps_buyulu[2*buyulu-1],warps_buyulu[2*buyulu])
				elseif s == 10 then
					local sihirli = select("Sihirli Orman (115~120)","Vazgeç ")
					local warps_sihirli = { 7926800,9916300 }
					if pc.get_level() < 115 then
						syschat("Seviyen 115 olmadan bu haritaya giriþ yapamazsýn.")
						return
					end
					if sihirli == 2 then
						return
					end
					pc.warp(warps_sihirli[2*sihirli-1],warps_sihirli[2*sihirli])
				end
			end	
		end
	end
end