--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest kids_day_quiz begin
    state start begin
        when 20041.chat."GM: Bilgi Yarýþmasý Drop Deðiþtir " with pc.is_gm() begin
            local low_value=game.get_event_flag("kids_day_drop")
            local high_value=game.get_event_flag("kids_day_drop_high")
            say_title("Bulmaca Kutusu Düþme Yüzdesi:")
            say(string.format("Düþük seviyeler için yüzde : 1/%d ", low_value))
            say(string.format("Yüksek seviyeler için yüzde  : 1/%d ", high_value))
            local s=select("Düþük Seviyeler Ýçin Drop Ayarla ", "Yüksek Seviyeler Ýçin Drop Ayarla ", "Vazgeç ")
            if s==1 then
                say_title("Düþük seviyeler için yüzde deðiþikliði :")
                say(string.format("Düþük seviyede yapýlacak deðiþiklik 1/%d -> ", low_value))
                local v=tonumber(input())
                if v<300 then
                    say("300'ün altýnda deðer yazamazsýn!")
                end

                say(string.format("Düþük seviyede yapýlacak deðiþiklik 1/%d -> 1/%d ", low_value, v))
                local s=select("Deðiþtir ", "Vazgeç ")
                if s==1 then
                    say("Deðiþtirildi!")
                    game.set_event_flag("kids_day_drop", v)
                else
                    say("Ýptal Edildi.")
                end
            elseif s==2 then
                say_title("Yüksek seviyeler için yüzde deðiþikliði :")
                say(string.format("Düþük seviyede yapýlacak deðiþiklik 1/%d -> ", high_value))
                local v=tonumber(input())
                if v<300 then
                    say("300'ün altýnda deðer yazamazsýn!")
                end

                say(string.format("Düþük seviyede yapýlacak deðiþiklik 1/%d -> 1/%d ", high_value, v))
                local s=select("Deðiþtir ", "Vazgeç ")
                if s==1 then
                    say("Deðiþtirildi!")
                    game.set_event_flag("kids_day_drop_high", v)
                else
                    say("Ýptal Edildi.")
                end
            end
        end
        when 50034.use begin
            say_title("Bilgi Yarýþmasý! ")
            say("")
            ----"123456789012345678901234567890123456789012345678901234567890"|
            say("Bilgi yarýþmasýna hoþ geldin! Eðer 3 soruyu ")
            say("doðru cevaplarsan ödül kazanacaksýn. ")
            say("")
            say("Hazýr mýsýn? ")
            say("")
            local s=select("Hadi Baþlayalým! ", "Vazgeçtim. ")
            if 2==s then
                return
            end

            --pc.remove_item(50034, 1)
            item.remove()

            local size=3
            local chance=pc.getqf("chance")

            -- Chance!
            if 0==chance then
                pc.setqf("chance", number(3, 5))
            elseif pc.getqf("success_count")==chance then
                pc.setqf("success_count", 0)
                pc.setqf("chance", number(3, 5))
                size=5
            end

            say_title("Bilgi Yarýþmasý! ")

		say("")
		----"123456789012345678901234567890123456789012345678901234567890"|
		say(""..size.." soruyu doðru cevaplarsan ")
		say("ödül kazanacaksýn. ")

            wait()

            for i=1, size+1 do

                local quiz_index=number(1, table.getn(locale.quiz))
                local quiz_data=locale.quiz[quiz_index]

                say_title("Bilgi Yarýþmasý #"..i.." ")
                say("")
                say(quiz_data[1])
                say("")
                local s=select(quiz_data[2], quiz_data[3], quiz_data[4], quiz_data[5])
                if s==quiz_data[6] then
                    if i==size then
                        say_title("Bilgi Yarýþmasý! ")
                        say("")
                        say("Tüm sorularý doðru cevapladýn, ")
                        say("iþte ödülün! ")
                        say("")

                        pc.setqf("success_count", pc.getqf("success_count")+1)

                        if 5==size then
                            say_item_vnum(50036)
                            say_reward("Mor hediye sandýðý kazandýn. ")
                            pc.give_item2(50036)
                        else
                            say_item_vnum(50035)
                            say_reward("Sarý hediye sandýðý kazandýn. ")
                            pc.give_item2(50035)
                        end
                        say("")
                        return
                    else
                        say_title("Bilgi Yarýþmasý! ")
                        say("")
                        say_reward("Doðru! ")
                        say("")
                        say("Bir sonraki soruya geçiliyor. ")
                        say("")
                        wait()
                    end
                else
                    say_title("Bilgi Yarýþmasý! ")
                    say("")
                    say("Yanlýþ cevap! ")
                    say("")
                    say("Üzgünüm, daha sonra tekrar dene! ")
                    say("") 
                    return
                end
            end

        end
    end
end
