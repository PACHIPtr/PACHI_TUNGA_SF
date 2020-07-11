--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest marriage_manage begin
    state start begin
        when oldwoman.chat."Evlenmek istiyorum" with not pc.is_engaged_or_married() begin
            if not npc.lock() then
                say_title("Yaþlý Kadýn:")
                say("")
                say("Þu anda baþka bir düðün var!")
                say("Biraz bekle ya da sonra tekrar gel!")
                return
            end
            if pc.level < 75 then
                say_title("Yaþlý Kadýn:")
                say("")
                say("Evlenmek için çok gençsin...")
                say("Yeterli sorumluluðu kazanmalýsýn ve henüz")
                say("hazýr deðilsin. Genç insanlar çabuk ayrýlýrlar.")
                say("Bunu tasvip etmiyorum. Git ve")
                say("biraz daha tecrübe kazan.")
                say("")
                say_title("Bilgi:")
                say("")
                say_reward("75.seviyeden sonra evlenebilirsin. ")
                say("")
                npc.unlock()
                return
            end

            local m_ring_num = pc.countitem(70301)
            local m_has_ring = m_ring_num > 0
            if not m_has_ring then
                say_title("Yaþlý Kadýn:")
                say("")
                say("Nikah yüzüðü olmadan mý evlenmek istiyorsun?")
                say("")
                say_item("Nikah yüzüðü ", 70302, "")
                say("Önce, söz yüzüðü almalýsýn. Ancak ondan")
                say("sonra evlenebilirsin. ")
                say("")
                npc.unlock()

                return
            end

            local m_sex = pc.get_sex()
            if not marriage_manage.is_equip_wedding_dress() then
                say_title("Yaþlý Kadýn:")
                say("")
                say("Bu sekilde evlenmek istediðine gerçekten")
                say("emin misin? Hayatta sadece bir kere")
                say("evleniyorsun, en azýndan buna uygun giyinmelisin.")
                say("")

                if m_sex==0 then
                    say_item("Smokin", marriage_manage.get_wedding_dress(pc.get_job()), "")
                    say_reward("Eðer evlenmek istiyorsan bir smokin ")
                    say_reward("giymelisin.")
                else
                    say_item("Gelinlik", marriage_manage.get_wedding_dress(pc.get_job()), "")
                    say_reward("Eðer evlenmek istiyorsan gelinlik")
                    say_reward("giymelisin.")
                end
                say("")
                npc.unlock()
                return
            end

            local NEED_MONEY = 1000000
            if pc.get_money() < NEED_MONEY then
                say_title("Yaþlý Kadýn:")
                say("")
                say("Biraz Yang'a ihtiyacýn var. ")
                say("Yeterli Yang'ýn yok mu? Daha fazla Yang")
                say("kazan, en az 1 Milyon. ")
                say("")
                say_reward(string.format(" %d Yang'a ihtiyacýn var.", NEED_MONEY/10000))
                say("")
                npc.unlock()
                return
            end

            say_title("Yaþlý Kadýn:")
            say("")
            say("Þimdi yeterince olgunsun. ")
            say("Ýyi görünüyorsun. Kiminle evlenmek istiyorsun?")
            say("")
            say_reward("Kiþinin adýný yaz. ")

            local sname = input()
            if sname == "" then
                say_title("Yaþlý Kadýn:")
                say("")
                say("Heyecanlý mýsýn ismi ")
                say("yazamadýn? Tekrar dene.")
                say("")
                npc.unlock()
                return
            end

            local u_vid = find_pc_by_name(sname)
            local m_vid = pc.get_vid()
            if u_vid == 0 then
                say_title("Yaþlý Kadýn:")
                say("")
                say("Adýný bilmiyor musun?")
                say("Hala kararlý mýsýn? ")
                say("Evlenmek istediðine emin misin?")
                say("")
                say_reward(string.format("%s online deðil.", sname))
                say("")
                npc.unlock()
                return
            end

            if not npc.is_near_vid(u_vid, 10) then
                say_title("Yaþlý Kadýn:")
                say("")
                say("Eþin yakýnda olmak zorunda , isteðini ")
                say("ancak o zaman kabul edebilirim. ")
                say("Eþini buraya getir. ")
                say("")
                say_reward(string.format("%s çok uzakta.", sname))
                say("")
                npc.unlock()
                return
            end

            local old = pc.select(u_vid)
            local u_level = pc.get_level()
            local u_job = pc.get_job()
            local u_sex = pc.get_sex()
            local u_name = pc.name
            local u_gold = pc.get_money()
            local u_married = pc.is_married()
            local u_has_ring = pc.countitem(70301) > 0
            local u_wear = marriage_manage.is_equip_wedding_dress()
            pc.select(old)
            local m_level = pc.get_level()

            if u_vid == m_vid then
                say_title("Yaþlý Kadýn:")
                say("")
                say("Senin adýn deðil, eþinin adý. ")
                say("")
                say_reward("Eþinin adýný yaz.")
                say("")
                npc.unlock()
                return
            end

            if u_sex == m_sex then
                say_title("Yaþlý Kadýn:")
                say("")
                say("Özür dilerim.")
                say("")
                say("Ayný cinsteki bir insanla evlenemezsin.")
                say("")
                npc.unlock()
                return
            end

            if u_married then
                say_title("Yaþlý Kadýn:")
                say("")
                say("Evlenmek istedigin kiþi zaten evli ,")
                say("bunu biliyor muydun?")
                say("Yalnýz birini bul.")
                say("")
                say_reward(string.format("%s evli.", sname))
                say("")
                npc.unlock()
                return
            end

            if u_level < 75 then
                say_title("Yaþlý Kadýn:")
                say("")
                say("Kiþi yeterince olgun deðil. ")
                say("Eþin de en az 75.seviyede olmalý.")
                say("")
                npc.unlock()
                return
            end

            if m_level - u_level > 15 or u_level - m_level > 15 then
                say_title("Yaþlý Kadýn:")
                say("")
                say("Siz uyumlu bir çift deðilsiniz.")
                say("Bu evliliði onaylayamam.")
                say("")
                say("Seviye farký 15'den fazla olmamalý,")
                say("bu yüzden evlenemiyorsunuz.")
                say("")
                npc.unlock()
                return
            end

            if not u_has_ring then
                if m_ring_num >= 2 then
                    say_title("Yaþlý Kadýn:")
                    say("")
                    say("þimdi yüzükleri deðiþtirin.")
                    say("")
                else
                    say("Yaþlý Kadýn:")
                    say("")
                    say("Evlilik hayatýnýzdaki en önemli karardýr.")
                    say("En azýndan eþine nikah yüzüðü ")
                    say("getirmiþ olmalýsýn.")
                    say("")
                end

                say_item("Nikah yüzüðü ", 70302, "")
                say_title("Yaþlý Kadýn:")
                say("")
                say("Eþinin de nikah yüzüðü olmalý.")
                say("hazýr mý?")
                say("")
                npc.unlock()
                return
            end

            if not u_wear then
                say_title("Yaþlý Kadýn:")
                say("")
                say("Eþin evlilik kýyafetý giymemiþ.")
                say("Onunla burda durmaktan sýkýlmýyor musun?")
                say("Hmm?")
                say("")
                if u_sex==0 then
                    say_title("Yaþlý Kadýn:")
                    say("")
                    say_item("Smokin ", marriage_manage.get_wedding_dress(u_job), "")
                    say("Eþinin Smokin giymesi")
                    say("gerekiyor.")
                else
                    say_title("Yaþlý Kadýn:")
                    say("")
                    say_item("Gelinlik", marriage_manage.get_wedding_dress(u_job), "")
                    say("Eþinin Gelinlik giymesi ")
                    say("gerekiyor.")
                end
                say("")
                npc.unlock()
                return
            end


            local ok_sign = confirm(u_vid, pc.name.."Evlenmek istiyor musun?", 30)
            if ok_sign == CONFIRM_OK then
                local m_name = pc.name
                if pc.get_gold()>=NEED_MONEY then
                    pc.change_gold(-NEED_MONEY)

                    pc.removeitem(70301, 1)
                    pc.give_item2(70302, 1)
                    local old = pc.select(u_vid)
                    pc.removeitem(70301, 1)
                    pc.give_item2(70302, 1)
                    pc.select(old)

                    say_title("Yaþlý Kadýn:")
                    say("")
                    say("Peki...biz hazýrýz.")
                    say("Þimdi baþlayabiliriz. Sizi Aþýklar")
                    say("Adasýna göndereceðim. Umarým uzun ve")
                    say("mutlu bir evliliðiniz olur. Tebrikler..")
                    say("")
                    say("Daha sonra otomatik olarak Aþýklar Adasý'na")
                    say("gönderileceksiniz.")
                    say("")
                    wait()
                    setskin(NOWINDOW)
                    marriage.engage_to(u_vid)
                end
            else
                say_title("Yaþlý Kadýn:")
                say("")
                say("Eþin seninle evlenmek istemiyor.")
                say("Öncelikle bunu açýklýða kavuþturun. ")
                say("")
            end
            say("")
            npc.unlock()
        end

        when oldwoman.chat."Nikah salonuna geri dön." with pc.is_engaged() begin
            say_title("Yaþlý Kadýn:")
            say("")
            say("Neden hala burdasýn? ")
            say("Eþin seni ariyor. ")
            say("Seni oraya göndereceðim. ")
            say("")
            wait()
            setskin(NOWINDOW)
            marriage.warp_to_my_marriage_map()
        end


        when 9011.chat."Evlilik için izin" with pc.is_engaged() and marriage.in_my_wedding() begin
            if not npc.lock() then
                say_title("Nikah Memuru:")
                say("")
                say("Eþinle konuþuyorum. Biraz bekle. ")
                say("")
                return
            end
            say_title("Nikah Memuru:")
            say("")
            say("Ben bir evlilik asistanýyým.")
            say("ilk olarak eþinin ismini")
            say("yaz.")

            local sname = input()
            local u_vid = find_pc_by_name(sname)
            local m_vid = pc.get_vid()

            if u_vid == 0 then
                say_title("Nikah Memuru:")
                say("")
                say("Böyle bir isim kayýtlý deðil, ")
                say("Yazdýðýný tekrar gözden geçir. ")
                say("")
                say_reward(string.format("%s online deðil.", sname))
                say("")
                npc.unlock()
                return
            end

            if not npc.is_near_vid(u_vid, 10) then
                say_title("Nikah Memuru:")
                say("")
                say("Özür dilerim. Eþini buraya getir,")
                say("Onu bilgilendirmeli ve")
                say("incelemeliyim.")
                say("")
                say_reward(string.format("%s burada olmalý.", sname))
                say("")
                npc.unlock()
                return
            end

            if u_vid == m_vid then
                say_title("Nikah Memuru:")
                say("")
                say("Lütfen kendi adýný yazma. ")
                say("")
                say("Eþinin adýný yazmalýsýn.")
                say("")
                npc.unlock()
                return
            end

            if u_vid != marriage.find_married_vid() then
                say_title("Nikah Memuru:")
                say("")
                say("Bir þeyler yanlýþ. Bu doðru insan deðil.")
                say("Kontrol ettin mi?")
                say("")
                npc.unlock()
                return
            end

            local ok_sign = confirm(u_vid, pc.name.."Evlenmek istiyor musun?", 30)
            if ok_sign != CONFIRM_OK then
                say_title("Nikah Memuru:")
                say("")
                say("Eþin evlilik için hazýr deðil.")
                say("Birbirinizle konuþun.")
                say("")
                npc.unlock()
                return
            end
            say_title("Nikah Memuru:")
            say("")
            say("Þimdi her þey uygun, baþlayabiliriz.")

            marriage.set_to_marriage()

            say("")
            say("Güzel bir düðündü.")
            say("")
            npc.unlock()
        end

        function give_wedding_gift()
            local male_item = {71072, 71073, 71074}
            local female_item = {71069, 71070, 71071}
            if pc.get_sex() == MALE then
                pc.give_item2(male_item[number(1, 3)], 1)
            else
                pc.give_item2(female_item[number(1, 3)], 1)
            end
        end

        when 9011.chat."Düðün marþýný çal" with
                    (pc.is_engaged() or pc.is_married()) and
                marriage.in_my_wedding() and
                not marriage.wedding_is_playing_music() begin
            marriage.wedding_music(true, "wedding.mp3")
            setskin(NOWINDOW)
        end
        when 9011.chat."Düðünü marþýný durdur" with
                    (pc.is_engaged() or pc.is_married()) and
                marriage.in_my_wedding() and
                marriage.wedding_is_playing_music() begin
            marriage.wedding_music(false, "default")
            setskin(NOWINDOW)
        end
        when 9011.chat."Gece efekti" with
            pc.is_married() and
            marriage.in_my_wedding() begin
            marriage.wedding_dark(true)
            setskin(NOWINDOW)
        end

        when 9011.chat."Kar yaðdýr" with pc.is_married() and marriage.in_my_wedding() begin
            marriage.wedding_snow(true)
            setskin(NOWINDOW)
        end

        when 9011.chat."Düðünü bitir" with pc.is_married() and marriage.in_my_wedding() begin
            if not npc.lock() then
                say_title("Nikah Memuru:")
                say("")
                say("Biraz bekle, Eþinle konuþuyorum.")
                say("")
                return
            end

            say_title("Nikah Memuru:")
            say("")
            say("Düðünü bitirmek istiyor musun?")
            say("")
            local s = select("Evet","Hayýr")
            if s == 1 then
                local u_vid = marriage.find_married_vid()
                if u_vid == 0 then
                    say_title("Nikah Memuru:")
                    say("")
                    say("Eþinin düðüne devam etmeyi kabul etmesi gerekli.")
                    say("")
                    say("düðün devam edemiyor çünkü eþin")
                    say("online deðil.")
                    say("")
                    npc.unlock()
                    return
                end
                say_title("Nikah Memuru:")
                say("")
                say("Eþinin onayý için beklemeliyiz.")
                say("Henüz cevabý almadýk.")
                say("")
                local ok_sign = confirm(u_vid, "düðüne devam etmek istiyor musun? ", 30)
                if ok_sign == CONFIRM_OK then
                    marriage.end_wedding()
                else
                    say_title("Nikah Memuru:")
                    say("")
                    say("Eþin onaylamadý!")
                    say("")
                end

                npc.unlock()
            end
        end


        when         11000.chat."Bosanma " or
                    11002.chat."Bosanma " or
                11004.chat."Bosanma " with pc.is_married() begin

            if not marriage_manage.check_divorce_time() then
                return
            end

            local u_vid = marriage.find_married_vid()
            if u_vid == 0 or not npc.is_near_vid(u_vid, 10) then
                say_title("Köy Gardiyaný:")
                say("")
                say("Bir eþin olmadan boþanamazsýn.")
                say("")
                return
            end

            say_title("Köy Gardiyaný:")
            say("")
            say("Yasal bir ayrýlýk için")
            say("500.000 Yang ve eþinin")
            say("onayý gerekli.")
            say("Gerçekten boþanmak istiyor musun? ")
            say("")

            local MONEY_NEED_FOR_ONE = 500000
            local s = select("Evet", "Hayýr")

            if s == 1 then
                local m_enough_money = pc.gold > MONEY_NEED_FOR_ONE
                local m_have_ring = pc.countitem(70302) > 0

                local old = pc.select(u_vid)
                local u_enough_money = pc.gold > MONEY_NEED_FOR_ONE
                local u_have_ring = pc.countitem(70302) > 0
                pc.select(old)

                if not m_have_ring then
                    say("Nikah yüzüðünü getir.")
                    return;
                end
                if not u_have_ring then
                    say("Eþinin nikah yüzüðü yanýnda deðil.")
                    return;
                end

                if not m_enough_money then
                    say_title("Köy Gardiyaný:")
                    say("")
                    say("Bosanmak için yeterli Yang'a sahip deðilsin.")
                    say("")
                    say_reward(string.format("Boþanmak için %d Yang gerekli.", MONEY_NEED_FOR_ONE/10000))
                    say("")
                    return;
                end
                if not u_enough_money then
                    say_title("Köy Gardiyaný:")
                    say("")
                    say("Boþanmak için yeterli Yang'a sahip deðilsin.")
                    say("")
                    say_reward("Boþanmak için 500.000 Yang'a ihtiyacýn var.")
                    say("")
                    return;
                end
                say_title("Köy Gardiyaný:")
                say("")
                say("Gerçekten bosanmak istiyor musun? Bu çok büyük")
                say("bir hata olabilir.")
                say("")
                say("Gerçekten istiyor musun?")
                say("")

                local c=select("Evet", "Hayýr")
                if 2 == c then
                    say_pc_name()
                    say("")
                    say("Fikrimi deðiþtirdim.")
                    say("Bosanmak istemiyorum.")
                    say("")
                    wait()
                    say_title("Köy Gardiyaný:")
                    say("")
                    say("Umarým huzurlu ve uzun bir evliliðiniz olur.")
                    say("")
                    say_reward("Boþanma gerçekleþmedi.")
                    say("")
                    return
                end

                local ok_sign = confirm(u_vid, pc.name.."Boþanmak istediðine emin misin?", 30)
                if ok_sign == CONFIRM_OK then

                    local m_enough_money = pc.gold > MONEY_NEED_FOR_ONE
                    local m_have_ring = pc.countitem(70302) > 0

                    local old = pc.select(u_vid)
                    local u_enough_money = pc.gold > MONEY_NEED_FOR_ONE
                    local u_have_ring = pc.countitem(70302) > 0
                    pc.select(old)

                    if m_have_ring and m_enough_money and u_have_ring and u_enough_money then
                        pc.removeitem(70302, 1)
                        pc.change_money(-MONEY_NEED_FOR_ONE)

                        local old = pc.select(u_vid)
                        pc.removeitem(70302, 1)
                        pc.change_money(-MONEY_NEED_FOR_ONE)
                        pc.select(old)

                        say_title("Köy Gardiyaný:")
                        say("")
                        say("Eþinden ayrýldýn. Artýk evli deðilsin.")
                        say("Belki de doðru olan budur. Belki de")
                        say("mutluluðu böyle bulursun.")
                        say("")
                        say_reward("Boþanma gerçekleþti.")
                        say("")
                        marriage.remove()
                    else
                        say_title("Köy Gardiyaný:")
                        say("")
                        say("Evraklarýnýz tam deðil.")
                        say("Lütfen sonra tekrar gelin.")
                        say("")
                        say_reward("Boþanma iptal edildi.")
                        say("")
                    end
                else
                    say_title("Köy Gardiyaný:")
                    say("")
                    say("Eþiniz bosanmak istemiyor.")
                    say("Önce anlaþma saðlayýn.")
                    say("")
                    say_reward("Boþanma iptal edildi.")
                    say("")
                end
            end
        end

        when         11000.chat."Nikah Yüzüðü Silindi" or
                    11002.chat."Nikah Yüzüðü Silindi" or
                11004.chat."Nikah Yüzüðü Silindi" with
                        not pc.is_married()         and
                        pc.count_item(70302)>0
                        begin
                say_title("Köy Gardiyaný:")
                say("")
                say("Kötü hatýralarý bir an önce ")
                say("unutmalýsýn.")
                say("")
                say_reward("Nikah yüzüðü silindi.")
                pc.remove_item(70302)
        end

        when         11000.chat."Tek taraflý boþanma" or
                    11002.chat."Tek taraflý boþanma" or
                11004.chat."Tek taraflý boþanma" with pc.is_married() begin

            if not marriage_manage.check_divorce_time() then
                return
            end

            say_title("Köy Gardiyaný:")
            say("")
            say("Tek taraflý boþanma için 1 milyona ihtiyacýn var.")
            say("Boþanmak istiyor musun?")
            say("")

            local s = select("Evet", "Hayýr")

            local NEED_MONEY = 1000000
            if s == 2 then
                return
            end

            if pc.money < NEED_MONEY then
                say_title("Köy Gardiyaný:")
                say("")
                say("Boþanmak için yeterli Yang yok.")
                say("")
                return
            end

            say_title("Köy Gardiyaný:")
            say("Bir yuva kurmak kolay deðildir. Gerçekten emin")
            say("olmadan böyle büyük bir karar vermemelisin.")
            say("Gerçekten boþanmak istiyor musun?")

            local c = select("Evet ,istiyorum.","Tekrar düþünmeliyim.")

            if c == 2 then
                say_title("Köy Gardiyaný:")
                say("")
                say("Bu daha iyi.")
                say("Eðer bunu gerçekten istersen, sonra tekrar gel.")
                say("")
                return
            end

            pc.removeitem(70302, 1)
            pc.change_gold(-NEED_MONEY)

            marriage.remove()

            say_title("Köy Gardiyaný:")
            say("")
            say("Peki tamam.")
            say("Umarým doðru olaný yapýyorsundur!")
            say("")
            say_reward("Tek taraflý boþanma gerçekleþti.")
            say("")
        end

        when oldwoman.chat."Evlilik töreni listesi" with not pc.is_engaged() begin
            local t = marriage.get_wedding_list()
            if table.getn(t) == 0 then
                say_title("Yaþlý Kadýn:")
                say("")
                say("Þu an devam eden nikah töreni yok.")
                say("")
            else
                -- chat(table.getn(t))
                local wedding_names = {}
                table.foreachi(t, function(n, p) wedding_names[n] = p[3].."ile"..p[4].."evleniyor." end)
                wedding_names[table.getn(t)+1] = locale.confirm
                local s = select_table(wedding_names)

                if s != table.getn(wedding_names) then
                    marriage.join_wedding(t[s][1], t[s][2])
                end
            end
        end
        when 9011.click with not pc.is_engaged() and not pc.is_married() begin
            say_title("Nikah Memuru:")
            say("")
            say("Bu sizin düðününüz!")
            say("iyi eðlenceler.")
            say("")
        end

        function check_divorce_time()

            local DIVORCE_LIMIT_TIME = 86400

            if is_test_server() then
                DIVORCE_LIMIT_TIME = 60
            end

            if marriage.get_married_time() < DIVORCE_LIMIT_TIME then
                say_title("Köy Gardiyaný:")
                say("")
                say("Daha imzandaki mürekkep yeterince kurumadý!")
                say("Kendine biraz zaman ver.")
                say("")
                return false
            end

            return true
        end

        function is_equip_wedding_dress()
            local a = pc.get_armor()
            return a >= 11901 and a <= 11904
        end
        function get_wedding_dress(pc_job)
                if 0==pc_job then
                    return 11901
                elseif 1==pc_job then
                    return 11903
                elseif 2==pc_job then
                    return 11902
                elseif 3==pc_job then
                    return 11904
                else
                    return 0;
                end
        end
    end
end
