--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest blood_stone begin
    state start begin
        when 20095.chat."Maðaranýn Ýçi" begin
            local delay = 86400
            if get_global_time() - pc.getqf("time") < delay and pc.getqf("time") != 0 then
                local zeit = delay - (get_global_time() - pc.getqf("time"))
                say("Kan Taþýný sana günde sadece bir kez yapabilirim!")
                say("Kalan süre "..zeit.." saniye!")
                return
            end
            if pc.get_level() < 75 then
                say("Kan Taþý kazanmak için çok zayýfsýn!")
                say_reward("Seviyen 75'e ulaþtýðýnda tekrar gel!")
                return
            end
            say("Merhaba!")
            say("Kapýdan geçmek için Kan Taþý'na mý ihtiyacýn var?")
            say("Öyleyse Ateþ Diyarýndan yedi")
            say("Fosil Gözyaþý ve dört parça Obsidyen getir!")
            local s= select("Hadi Gidelim!","Hayýr, Teþekkürler.")
            if s==1 then
                say_title("Maðaranýn Ýçi")
                say("Ateþ Diyarýna git ve Ejderha Tanrý Baljit")
                say("Elvedin'in Fosil Gözyaþlarýný bul. Ayný ")
                say("zamanda Alevleri öldürerek Obsidyen")
                say("toplamalýsýn.")
                say("")
                say_reward("Gereken Fosil Gözyaþý: 7")
                say_reward("Gereken Obsidyen: 4")
                setstate(vulkangas)
            else
                say("Ýyi bir karar!")
                say("Görüþürüz..")
            end
        end
    end
    state vulkangas begin
        when letter begin
            send_letter("Obsidyen Ara")
        end
        when info or button begin
            say_title("Maðaranýn Ýçi")
            say("4 adet Obsidyen toplamalýsýn.")
            say("Obsidyenleri Alevlerden elde edebilirsin.")
        end
        when kill begin
		if npc.get_race() == 2204 then
            local pct = number(1,20)
            if pct == 1 then
                pc.give_item2(30188,1)
        	  send_letter("Bu alevden Obsidyen buldun!")
            end
			end
            if pc.count_item(30188) >= 4 then
        	send_letter("Gereken tüm Obsidyeni topladýn!")
                say("Tebrikler!")
                say("Þimdi Sýrada Fosil Gözyaþý var!")
                say("Haritada beliren koordinatlardan toplamalýsýn.")
                setstate(vertrnen)
        end
        end
    end
    state vertrnen begin
        when letter begin
            send_letter("Fosil Gözyaþlarýný topla!")
                    local tropfen = {
                        { 1333, 785},
                        { 900,730},
                        { 661,717},
                        { 543,821},
                        { 460,771},
                        { 379,685},
                        { 480,623},
                        { 428,468},
                        { 327,359},
                        { 224,539},
                        { 175,727},
                        { 145,900},
                        { 69,1000},
                        { 176,1150},
                        { 438,1112},
                        { 646,1015},
                        { 790,1183},
                        { 1028,1052},
                        { 1326, 1028},
                        { 1376,1198},
                        { 1301,1388},
                        { 903,1399},
                        { 539, 1338},
                        { 574, 361},
                        { 846, 455},
                        { 1154, 498},
                        { 1222, 440},
                        { 1425,500},
                        { 1283,347},
                        { 1430,123},
                        { 1100,159},
                        { 787, 1083},
                        { 610,175},
                        { 500, 278},
                        { 390,125},
                }
                local r =  number(1,30)
                target.pos("p1", tropfen[r][1], tropfen[r][2], 62, "Fosil Gözyaþý ")
                local r =  r+1
                target.pos("p2", tropfen[r][1], tropfen[r][2], 62, "Fosil Gözyaþý ")
                local r =  r+1
                target.pos("p3", tropfen[r][1], tropfen[r][2], 62, "Fosil Gözyaþý ")
                local r =  r+1
                target.pos("p4", tropfen[r][1], tropfen[r][2], 62, "Fosil Gözyaþý ")
                local r =  r+1
                target.pos("p5", tropfen[r][1], tropfen[r][2], 62, "Fosil Gözyaþý ")
                local r =  r+1
                target.pos("p6", tropfen[r][1], tropfen[r][2], 62, "Fosil Gözyaþý ")
                local r =  r+1
                target.pos("p7", tropfen[r][1], tropfen[r][2], 62, "Fosil Gözyaþý ")
                local r =  r+1
                pc.setqf("tränen",7)
        end
        when button or info begin
            say_title("Maðaranýn Ýçi")
            say("Þimdi 7 tane Fosil Gözyaþýna ihtiyacýn var!")
            say("Harita üzerindeki iþaretli noktalarý bul.")
        end
        when p1.target.arrive begin
        target.delete("p1")
        say_title("Fosil Gözyaþý ")
        say("Burada Ejderha Tanrý'nýn yeryüzüne düþen")
        say("Gözyaþlarýndan biri var.")
        pc.give_item2(30186,1)
        pc.setqf("tränen",pc.getqf("tränen")-1)
        if pc.getqf("tränen") == 0 then
        send_letter("Yeteri kadar gözyaþý topladýn!")
            setstate(rw)
        end
        end
        when p2.target.arrive begin
        target.delete("p2")
        say_title("Fosil Gözyaþý ")
        say("Burada Ejderha Tanrý'nýn yeryüzüne düþen")
        say("Gözyaþlarýndan biri var.")
        pc.give_item2(30186,1)
        pc.setqf("tränen",pc.getqf("tränen")-1)
        if pc.getqf("tränen") == 0 then
        send_letter("Yeteri kadar gözyaþý topladýn!")
            setstate(rw)
        end
        end
        when p3.target.arrive begin
        target.delete("p3")
        say_title("Fosil Gözyaþý ")
        say("Burada Ejderha Tanrý'nýn yeryüzüne düþen")
        say("Gözyaþlarýndan biri var.")
        pc.give_item2(30186,1)
        pc.setqf("tränen",pc.getqf("tränen")-1)
        if pc.getqf("tränen") == 0 then
        send_letter("Yeteri kadar gözyaþý topladýn!")
            setstate(rw)
        end
        end
        when p4.target.arrive begin
        target.delete("p4")
        say_title("Fosil Gözyaþý ")
        say("Burada Ejderha Tanrý'nýn yeryüzüne düþen")
        say("Gözyaþlarýndan biri var.")
        pc.give_item2(30186,1)
        pc.setqf("tränen",pc.getqf("tränen")-1)
        if pc.getqf("tränen") == 0 then
        send_letter("Yeteri kadar gözyaþý topladýn!")
            setstate(rw)
        end
        end
        when p5.target.arrive begin
        target.delete("p5")
        say_title("Fosil Gözyaþý ")
        say("Burada Ejderha Tanrý'nýn yeryüzüne düþen")
        say("Gözyaþlarýndan biri var.")
        pc.give_item2(30186,1)
        pc.setqf("tränen",pc.getqf("tränen")-1)
        if pc.getqf("tränen") == 0 then
        send_letter("Yeteri kadar gözyaþý topladýn!")
            setstate(rw)
        end
        end
        when p6.target.arrive begin
        target.delete("p6")
        say_title("Fosil Gözyaþý ")
        say("Burada Ejderha Tanrý'nýn yeryüzüne düþen")
        say("Gözyaþlarýndan biri var.")
        pc.give_item2(30186,1)
        pc.setqf("tränen",pc.getqf("tränen")-1)
        if pc.getqf("tränen") == 0 then
        send_letter("Yeteri kadar gözyaþý topladýn!")
            setstate(rw)
        end
        end
        when p7.target.arrive begin
        target.delete("p7")
        say_title("Fosil Gözyaþý ")
        say("Burada Ejderha Tanrý'nýn yeryüzüne düþen")
        say("Gözyaþlarýndan biri var.")
        pc.give_item2(30186,1)
        pc.setqf("tränen",pc.getqf("tränen")-1)
        if pc.getqf("tränen") == 0 then
        send_letter("Yeteri kadar gözyaþý topladýn!")
            setstate(rw)
        end
        end
    end
    state rw begin
        when letter begin
            send_letter("Maðaranýn Ýçi")
        end
        when button or info begin
            say_title("Maðaranýn Ýçi")
            say("Gözyaþlarý ve Obsidyen ile Seon-Hae'ye dön.")
            say("")
            say("Seon-Hae burada: 1. Köy")
        end
        when 20095.chat."Maðaranýn Ýçi" begin
        if pc.count_item(30186) >= 7 and pc.count_item(30188)>= 4 then
        pc.remove_item(30186,77)
        pc.remove_item(30188,44)
        pc.give_item2(30189,1)
            say_title("Maðaranýn Ýçi")
        say("iþte, vazoyu al ve Bahar-Taraji'nin sesini ara.")
        setstate(blut)
        else
        say("Obsidyen veya Fosil Gözyaþý eksik!")
        end
    end
    end
    state blut begin
        when letter begin
            send_letter("Maðaranýn Ýçi")
            träne = {
            { 197, 214},
            { 326, 302},
            { 527, 90},
            { 616, 257},
            { 702, 262},
            { 674, 38},
        }
        local r2 =  number(1,6)
        target.pos("träne", träne[r2][1], träne[r2][2], 68, "")
            end
        when info or button begin
            say_title("Maðaranýn Ýçi")
            say("Kýrmýzý Ormanda Bahar-Taraji'nin sesini ara.")
            say("Tanrýçanýn ruhunu göremesen bile, ruhunun")
            say("varlýðýný hissedebileceksin.")
        end
        when träne.target.arrive begin
            target.delete("träne")
            say_title("Maðaranýn Ýçi")
            say("Rüzgar sessiz bir fýsýltýyý getiriyor:")
            say("Güvenli bir yere git... Buarada konuþamayýz...")
            say("Hayaletle konuþmak için göreve týkla.")
            wait()
            if pc.count_item(30189) >0 then
                pc.remove_item(30189,1)
                pc.give_item2(30187,1)
            say_title("Maðaranýn Ýçi")
                say("Uzaktan, öldürülen Tanrýça Bahar-Taraji'nin sesi")
                say("duyuluyor...")
                say("")
                say(""..pc.name.." Seni tanýyorum.")
                say("")
                say("Bana aðabeyimin gözyaþlarýný getirdin ve ödül")
                say("olarak kanýmý kazandýn, savaþmaya devam")
                say("edebilirsin.")
                setstate(stein)
            else
                say("Tanrýça Gözyaþlarý sende deðil!")
                say("Onu bulduðun zaman tekrar beni bul.")
                            träne = {
            { 197, 214},
            { 326, 302},
            { 527, 90},
            { 616, 257},
            { 702, 262},
            { 674, 38},
        }
        local r2 =  number(1,6)
        target.pos("träne", träne[r2][1], träne[r2][2], 68, "")
            end
        end
    end
    state stein begin
            when letter begin
                send_letter("Maðaranýn Ýçi")
            end
            when button or info begin
            say_title("Maðaranýn Ýçi")
                say("Seon-Hae'ye Bahar-Taraji'nin kanýný götür.!")
                say("")
                say("Seon-Hae burada : 1. Köy")
                say()
            end
            when 20095.chat."Maðaranýn Ýçi" begin
                if pc.count_item(30187) <= 0 then
                    say("Kan Damlasý sende deðil!")
                else
                    pc.remove_item(30187,1)
                    pc.give_item2(30190,1)
            say_title("Maðaranýn Ýçi")
                    say("Bahar-Taraji'nin kanýný taþa çevireceðim. Ýþine!")
                    say("yarayacaktýr.")
                    say("")
                    say_reward("Sana günde sadece bir kez Kan Taþý yapabilirim.")
                    say_reward("Aksi takdirde Koe-Pung þüphelenir.")
                    clear_letter()
                    pc.setqf("time",get_global_time())
                    setstate(start)
                end
            end
        end
    end 