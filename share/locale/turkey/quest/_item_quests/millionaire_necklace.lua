--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest ticaret_duyur begin
	state start begin
		when 70019.use begin
		if pc . count_item ( 70019 ) < 1 then 
		say_title ( "Bug Kontrol" ) 
		say ( "Envanterindeki Milyonerin kolyesi kaybolmuþ." ) 
		return 
		end
		
		if pc.get_level() < 80 then 
		say("80. seviye olmadan bu eþyayý kullanamazsýn." ) 
		return 
		end
		
	    local kalan = pc.getqf("ticaret_sure") - get_time()
        if kalan > 0 then
        say_title("Milyonerin Kolyesi")
        say("")
        say("30 dakikada 1 bu kolyeyi kullanabilirsin.")
        say("")
        say_reward("Kalan süre: "..kalan.." saniye.")
        say("")
        else
        say_title("Milyonerin Kolyesi")
        say("")
        say("Ne yapmak istiyorsun?")
        say("")
        local panpa = select("Eþya satacaðým.","Eþya arýyorum.","Hiçbir þey")
        if panpa == 1 then
            say_title("Milyonerin Kolyesi")
            say("")
            say("Satmak istediðin eþyanýn adýný yaz.")
            say("")
            local esya = input()
                if esya == "" then
                    say_title("Milyonerin Kolyesi")
                    say("")
                    say("Ne satacaðýný bilmiyor musun?")
                    say("")
                else
                    say_title("Milyonerin Kolyesi")
                    say("")
                    say("Satýþ þeklini seç.")
                    say("")
                    local s = select("Takasla satmak istiyorum.","Yang karþýlýðý satmak istiyorum.","Vazgeç ")
                        if s == 1 then
                            if pc.get_empire() == 1 then
                                say_title("Milyonerin Kolyesi")
                                say("")
                                say("Duyuru þu þekilde olacak.")
                                say("")
                                say_reward(""..esya..", takasla satýlacaktýr.")
                                say_reward("Teklif vermek için Shinso ülkesinden")
                                say_reward(""..pc.get_name().." ile görüþün.")
                            elseif pc.get_empire() == 2 then
                                say_title("Milyonerin Kolyesi")
                                say("")
                                say("Duyuru þu þekilde olacak.")
                                say("")
                                say_reward(""..esya..", takasla satýlacaktýr.")
                                say_reward("Teklif vermek için Chunjo ülkesinden")
                                say_reward(""..pc.get_name().." ile görüþün.")
                            elseif pc.get_empire() == 3 then
                                say_title("Milyonerin Kolyesi")
                                say("")
                                say("Duyuru þu þekilde olacak.")
                                say("")
                                say_reward(""..esya..", takasla satýlacaktýr.")
                                say_reward("Teklif vermek için Jinno ülkesinden")
                                say_reward(""..pc.get_name().." ile görüþün.")
                            end
                                say("")
                                say("Onaylýyor musun?")
                                say("")
                            local ss = select("Onaylýyorum.","Vazgeçtim.")
                                if ss == 1 then
                                    if pc.get_empire() == 1 then
                                        notice_all(""..esya..", takasla satýlacaktýr. Teklif vermek için")
                                        notice_all("Shinso ülkesinden "..pc.get_name().." ile görüþün.")
                                        pc.remove_item("70019",1)
                                        pc.setqf("ticaret_sure",get_time() + 1800)
                                    elseif pc.get_empire() == 2 then
                                        notice_all(""..esya..", takasla satýlacaktýr. Teklif vermek için")
                                        notice_all("Chunjo ülkesinden "..pc.get_name().." ile görüþün.")
                                        pc.remove_item("70019",1)
                                        pc.setqf("ticaret_sure",get_time() + 1800)
                                    elseif pc.get_empire() == 3 then
                                        notice_all(""..esya..", takasla satýlacaktýr. Teklif vermek için")
                                        notice_all("Jinno ülkesinden "..pc.get_name().." ile görüþün.")
                                        pc.remove_item("70019",1)
                                        pc.setqf("ticaret_sure",get_time() + 1800)
                                    end
                                elseif ss == 2 then
                                    return
                                end
                        elseif s == 2 then
                            say_title("Milyonerin Kolyesi")
                            say("")
                            say("Eþyanýn fiyatýný aþaðýdaki kutuya yaz.")
                            say("")
                            say_reward("Örnek: 500000")
                            say("")
                            local fiyat = tonumber(input(""))
                                if fiyat == "" then
                                    say_title("Milyonerin Kolyesi")
                                    say("")
                                    say("Eþya fiyatýný yazmadýn. Yoksa bedavaya mý ")
                                    say("vermek istiyorsun?")
                                    say("")
                                elseif fiyat < 9999999999999999 then
                                    say_title("Milyonerin Kolyesi")
                                    say("")
                                    say("Duyuru þu þekilde olacak.")
                                    say("")
                                    if pc.get_empire() == 1 then
                                        say_reward(""..esya..", satýlýktýr. Fiyatý: "..fiyat.." Yang")
                                        say_reward("Satýn almak için Shinso ülkesinden "..pc.get_name().."")
                                        say_reward("ile görüþün.")
                                    elseif pc.get_empire() == 2 then
                                        say_reward(""..esya..", satýlýktýr. Fiyatý: "..fiyat.." Yang")
                                        say_reward("Satýn almak için Chunjo ülkesinden "..pc.get_name().."")
                                        say_reward("ile görüþün.")
                                    elseif pc.get_empire() == 3 then
                                        say_reward(""..esya..", satýlýktýr. Fiyatý: "..fiyat.." Yang")
                                        say_reward("Satýn almak için Jinno ülkesinden "..pc.get_name().."")
                                        say_reward("ile görüþün.")
                                    end
                                    say("")
                                    say("Onaylýyor musun?")
                                    say("")
                                    local sss = select("Onaylýyorum.","Vazgeçtim.")
                                        if sss == 1 then
                                            if pc.get_empire() == 1 then
                                                notice_all(""..esya..", satýlýktýr. Fiyatý: "..fiyat.." Yang")
                                                notice_all("Satýn almak için Shinso ülkesinden "..pc.get_name().." ile görüþün.")
                                                pc.remove_item("70019",1)
                                                pc.setqf("ticaret_sure",get_time() + 1800)
                                            elseif pc.get_empire() == 2 then
                                                notice_all(""..esya..", satýlýktýr. Fiyatý: "..fiyat.." Yang")
                                                notice_all("Satýn almak için Chunjo ülkesinden "..pc.get_name().." ile görüþün.")
                                                pc.remove_item("70019",1)
                                                pc.setqf("ticaret_sure",get_time() + 1800)
                                            elseif pc.get_empire() == 3 then
                                                notice_all(""..esya..", satýlýktýr. Fiyatý: "..fiyat.." Yang")
                                                notice_all("Satýn almak için Jinno ülkesinden "..pc.get_name().." ile görüþün.")
                                                pc.remove_item("70019",1)
                                                pc.setqf("ticaret_sure",get_time() + 1800)
                                            end
                                        elseif sss == 2 then
                                            return
                                        end
                                    end
                                else
                                say_title("Milyonerin Kolyesi")
                                say("")
                                say("Bir hata oluþtu. Fiyat rakamlardan oluþmalý.")
                                say("")
                                end
                            end
                            elseif panpa == 2 then
                            say_title("Milyonerin Kolyesi")
                            say("")
                            say("Aradýðýn eþyanýn adýný yaz.")
                            local esya2 = input()
                            if esya2 == "" then
                            say_title("Milyonerin Kolyesi")
                            say("")
                            say("Ne aradaðýný bilmiyor musun?")
                            else
                            say_title("Milyonerin Kolyesi")
                                    say("")
                                    say("Duyuru þu þekilde olacak.")
                                    say("")
                                    if pc.get_empire() == 1 then
                                        say_reward(""..esya2.." alýnacaktýr. ")
                                        say_reward("Shinso ülkesinden "..pc.get_name().."")
                                        say_reward("ile görüþün.")
                                    elseif pc.get_empire() == 2 then
                                        say_reward(""..esya.." alýnacaktýr.")
                                        say_reward("Chunjo ülkesinden "..pc.get_name().."")
                                        say_reward("ile görüþün.")
                                    elseif pc.get_empire() == 3 then
                                        say_reward(""..esya2.." alýnacaktýr.")
                                        say_reward("Jinno ülkesinden "..pc.get_name().."")
                                        say_reward("ile görüþün.")
                                    end
                                    say("")
                                    say("Onaylýyor musun?")
                                    say("")
                                    local ssss = select("Onaylýyorum.","Vazgeçtim.")
                                    if ssss == 1 then
                                    if pc.get_empire() == 1 then
                                        notice_all(""..esya2.." alýnacaktýr. ")
                                        notice_all("Shinso ülkesinden "..pc.get_name().." ile görüþün.")
                                    elseif pc.get_empire() == 2 then
                                        notice_all(""..esya2.." alýnacaktýr.")
                                        notice_all("Chunjo ülkesinden "..pc.get_name().." ile görüþün.")
                                    elseif pc.get_empire() == 3 then
                                        notice_all(""..esya2.." alýnacaktýr.")
                                        notice_all("Jinno ülkesinden "..pc.get_name().." ile görüþün.")
                                    end
                                    pc.remove_item("70019",1)
                                    pc.setqf("ticaret_sure",get_time() + 1800)
                                    elseif ssss == 2 then
                                    return
                                    end
                                    end
                                end
                            end
                        end
                    end
                end  