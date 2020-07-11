--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest yg begin 
    state start begin 
        when 50512.use begin 
            x = {} 
            y = {} 
            z = 1 
            g = 50512 
            yg.BeceriKontrol(pc.get_skill_group()) 
        end     
         
        function BeceriKontrol(beceriDurumu) 
            if beceriDurumu == 0 then 
                syschat("Sistem: Henüz beceriye sahip deðilsiniz.") 
            else 
                yg.SinifBelirleme(pc.get_job()) 
            end 
        end 
        function SinifBelirleme(sinifi) 
            if sinifi == 0 then 
                yg.SavasciSinifi(pc.get_skill_group()) 
            elseif sinifi == 1 then 
                yg.NinjaSinifi(pc.get_skill_group()) 
            elseif sinifi == 2 then 
                yg.SuraSinifi(pc.get_skill_group()) 
            elseif sinifi == 3 then 
                yg.SamanSinifi(pc.get_skill_group())
			elseif sinifi == 4 then
				yg.LycanSinifi(pc.get_skill_group())
            end 
        end 
        function SavasciSinifi(savasciBeceri) 
            if savasciBeceri == 1 then -- Bedensel 
                yg.YetenekDenetim(1,6) 
            elseif savasciBeceri == 2 then -- Zihinsel 
                yg.YetenekDenetim(16,21)  
            end 
        end 
        function NinjaSinifi(ninjaBeceri) 
            if ninjaBeceri == 1 then -- Yakýn dövüþ 
                yg.YetenekDenetim(31,36)  
            elseif ninjaBeceri == 2 then -- Uzak Dövüþ 
                yg.YetenekDenetim(46,51) 
            end 
        end 
        function SuraSinifi(suraBeceri) 
            if suraBeceri == 1 then -- Büyülü Keskinlik 
                yg.YetenekDenetim(61,66)  
            elseif suraBeceri == 2 then -- Karabüyü  
                yg.YetenekDenetim(76,81)  
            end 
        end 
        function SamanSinifi(samanBeceri) 
            if samanBeceri == 1 then -- Ejderha Yardýmý 
                yg.YetenekDenetim(91,96)  
            elseif samanBeceri == 2 then -- Ýyileþtirme 
                yg.YetenekDenetim(106,111) 
            end 
        end
		function LycanSinifi(lycanBeceri)
			if lycanBeceri == 1 then
				yg.YetenekDenetim(170,175)
			end
		end
        function BeceriAdlari(beceriDegeri) 
            beceriDizi = { 
                -- Bedensel 
                [1] = {"Üç Yönlü Kesme"}, 
                [2] = {"Kýlýç Çevirme"}, 
                [3] = {"Öfke"}, 
                [4] = {"Hava Kýlýcý "}, 
                [5] = {"Hamle"}, 
				[6] = {"Yaþama Ýsteði"},
                -- Zihinsel 
                [16] = {"Ruh Vuruþu"}, 
                [17] = {"Þiddetli Vuruþ "}, 
                [18] = {"Güçlü Vuruþ "}, 
                [19] = {"Güçlü Beden"}, 
                [20] = {"Kýlýç Darbesi"}, 
				[21] = {"Kýlýç Çemberi"},
                -- Yakýn dövüþ 
                [31] = {"Suikast"}, 
                [32] = {"Hizli Saldiri"}, 
                [33] = {"Býçak Çevirme"}, 
                [34] = {"Kamuflaj"}, 
                [35] = {"Zehirli Bulut"},
				[36] = {"Sinsi Zehir"},
                -- Uzak dövüþ 
                [46] = {"Tekrarlanan Atýþ "}, 
                [47] = {"Ok Yaðmuru"}, 
                [48] = {"Ateþli Ok"}, 
                [49] = {"Hafif Adým"}, 
                [50] = {"Zehirli Ok"}, 
				[51] = {"Kývýlcým Vuruþu"},
                -- Büyülü Keskinlik 
                [61] = {"Parmak Darbesi"}, 
                [62] = {"Ejderha Dönüþu"}, 
                [63] = {"Büyülü Keskinlik"}, 
                [64] = {"Dehþet"}, 
                [65] = {"Büyülü Zýrh"}, 
                [66] = {"Büyü Çözme"}, 
                -- Karabüyü 
                [76] = {"Karanlýk Vurus"}, 
                [77] = {"Ateþ Vuruþ "}, 
                [78] = {"Ateþ Hayaleti"}, 
                [79] = {"Karanlýk Koruma"}, 
                [80] = {"Hayalet Vuruþ "}, 
                [81] = {"Karanlýk Küre"}, 
                -- Ejderha yardýmý 
                [91] = {"Uçan Týlsým"}, 
                [92] = {"Ejderha Atýþý "}, 
                [93] = {"Kutsama"}, 
                [94] = {"Ejderha Kükremesi"}, 
                [95] = {"Yansýtma"}, 
                [96] = {"Ejderha Yardýmý "}, 
                -- Ýyileþtirme 
                [106] = {"Þimþek Atma"}, 
                [107] = {"Þimþek Çaðýrma"}, 
                [108] = {"Þimþek Pençesi"}, 
                [109] = {"Ýyileþtirme"}, 
                [110] = {"Hýz"}, 
                [111] = {"Yüksek Saldirý "},
				-- Lycan
				[170] = {"Yýrtma "},
				[171] = {"Kurt Nefesi "},
				[172] = {"Kurt Atlayýþý "},
				[173] = {"Kurt Pençesi "},
				[174] = {"Kýrmýzý Kurt Ruhu "},
				[175] = {"Çivit Kurt Ruhu "}
            } 
            return beceriDizi[beceriDegeri][1] 
        end 
        function YetenekDenetim(m,k) 
			if pc.getf("skill_reset2","skill_bind") - get_time() > 0 then
				yg.BeceriMuhurlu()
				return
			end
            for i=m,k,1 do 
                if pc.get_skill_level(i) < 40 and pc.get_skill_level(i) >= 20 then 
                    x[z] = i 
                    y[z] = tostring(yg.BeceriAdlari(i)) 
                    z = z + 1 
                end 
            end 
            if z != 1 then 
				yg.Aciklama() 
                p = tonumber(table.getn(y)) 
                y[p+1] = "Kapat" 
                u = select_table(y)
                if u != p+1 then 
                    pc.set_skill_level(x[u],40) 
					item.remove(g,1)
                    yg.IslemBasarili() 
                end 
            else 
                yg.YetenekTam()
				z = 0
            end 
        end 
        function Aciklama() 
            say_title("[WINDOW_SIZE width;350|height;375]Bilgilendirme:") 
            say("") 
            say("Merhaba oyuncu, "..pc.get_name()) 
            say("M1 ve üstü yeteneðini P seviyesine geliþtirebilirsin.") 
            say("") 
            say_reward("Lütfen yeteneði seç: ") 
            say("") 
        end 
        function IslemBasarili() 
            say_title("Bilgilendirme: ") 
            say("") 
            say("Yeteneðiniz baþarý ile yükseltilmiþtir.") 
            say("") 
            say_reward("Yetenek yükseltme yüzüðü silinmiþtir.") 
            say("")
        end 
        function YetenekTam() 
            syschat("Sistem: Perfect master yapýlabilecek bir beceri yok.") 
        end 
		function BeceriMuhurlu()
			syschat("Sistem: Becerilerin mühürlü iken bunu gerçekleþtiremezsin.")
		end
    end 
end  