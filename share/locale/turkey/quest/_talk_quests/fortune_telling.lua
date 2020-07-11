--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest fortune_telling begin
        state start begin
                when oldwoman.chat."Senin falcý olduðunu duydum" begin
                        if pc.level<=20 then
                                say_title("Yaþlý kadýn:")
                                say("")
                                ---                                                   l
                                say("Görünüþe göre henüz buralarda yenisin.")
                                say("Neden bu kadar çabuk kýsmetini öðremek istiyorsun?")
                                say("Problemlerin mi var?")
                                say("Kýsmetin bol çalýþma istiyor.")
                                say("Sonra bir daha gel.")
                                say("")
                                return
                        end

                        if get_time()-pc.getqf("time") < 8*60*60 then
                                local passed_time = get_time() - pc.getqf("time")
                                say_title("Yaþlý kadýn:")
                                say("")
                                ---                                                   l
                                say("Hayatýný falcýnýn söylediði gibi mi yaþamak")
                                say("istiyorsun? Onun dedilerini çok ciddiye alma.")
                                say("Mühim olan")
                                say("düþüncelerin ve yüz ifaden,")
                                say("ikisi de iyiyse, sana bir þey olmaz.")
                                say("Hala falýna baktýrmak istiyorsan ")
                                say("yarýn bir daha gel.")
                                say("")
                                wait()

                                local idx = pc.getqf("idx")
                                if idx!=0 then
                                        say_title("Yaþlý kadýn:")
                                        ---                                                   l
                                        say("Dediðim gibi, bugünle ilgili kehanetim þöyle:")
                                        say(locale.fortune_words[idx][1])
                                        wait()

                                        if pc.getqf("talisman") == 0 and special.fortune_telling[idx][5] > 0 then
                                                local talisman_cost = special.fortune_telling[idx][5]
                                                wait()
                                                say_title("Yaþlý kadýn:")
                                                say("")
                                                ---                                                   l
                                                say("Kýsmetsiz olacaksýn, ve daha seni")
                                                say("kötülükten koruyan muskayý almadýn.. Benim")
                                                say("zenginlik muskamý almak ")
                                                say("ister misin?")


                                                local s = select("Evet onu almak istiyorum.","Hayýr almak istemiyorum.")
                                                if s == 1 then
                                                        if pc.gold < talisman_cost then
                                                                say_title("Yaþlý kadýn:")
                                                                say("")
                                                                ---                                                   l
                                                                say("Ýyi biriyim, ama sana muskayý hediye edemem")
                                                                say("uðursuzluktan korunmak istiyorsan")
                                                                say("bana para ver.")
                                                                say("")
                                                                say("")

                                                                pc.setqf("talisman", 0)
                                                        else
                                                                pc.give_item2(70052,1)
                                                                pc.changegold(-talisman_cost)
                                                                pc.setqf("talisman", 1)
                                                        end
                                                else
                                                end
                                        end
                                end
                        end

                        local gold = 7777

                        say_title("Yaþlý kadýn:")
                        say("")
                        ---                                                   l
                        say("Daha o kadar derin inlemek için çok gençsin!")
                        say("Falýna baktýrmak ister misin?")
                        say("")


                        local s = select("Bana kaderimi söyle.", "Benim falýma bakma")

                        if s == 2 then
                        say_title("Yaþlý kadýn:")
                        say("")
                        ---                                                   l
                        say("Doðru. Hayatta en önemli þey, kendi istediðini")
                        say("yapabilmek ve doðru yoldan þaþmamaktýr. Görüþmek ")
                        say("üzere!")
                        return
                        end
						say_title("Yaþlý kadýn:")
                        local n = number(1, 10000)

                        local i
                        local idx
                        for i = 1, table.getn(special.fortune_telling) do
                                if n<=special.fortune_telling[i][1] then
                                        idx = i
                                        break
                                else
                                        n = n - special.fortune_telling[i][1]
                                end
                        end
                        i = idx

                        say(locale.fortune_words[i][2])

                        local t = {}

                        n = 0
                        local j
                        for j = 2,4 do
                                if special.fortune_telling[i][j] != 0 then
                                        n=n+1
                                        t[n] = j
                                end
                        end

                        if n > 0 then
                                n = number(1, n)
                                __give_char_priv(t[n]-1, special.fortune_telling[i][t[n]])
                        end

                        pc.setqf("time", get_time())
                        pc.setqf("idx", i)

                        if special.fortune_telling[i][5]>0 then
                                wait()

                                say(locale.fortune_words[i][3])

                                local talisman_cost = special.fortune_telling[i][5]
                                local s = select("Evet,bir tane istiyorum.","Hayýr, saðol.")
                                if s == 1 then
										say_title("Yaþlý kadýn:")
                                        if pc.gold < talisman_cost then
                                                ---                                                   l
                                                say("Iyi biriyim ama sana muskayý hediye edemem")
                                                say("uðursuzlukdan korunmak istiyorsan")
                                                say("bana onun")
                                                say(" için para ver.")
                                                say("")
                                                say("Fiyatý "..gold.." Yang.")
                                                say("")
                                                pc.setqf("talisman", 0)

                                        else
                                                pc.give_item2(70052,1)
                                                pc.changegold(-talisman_cost)
                                                pc.setqf("talisman", 1)
                                        end

                                elseif s == 2 then
                                        say_title("Yaþlý kadýn:")
                                        say("")
                                        ---                                                   l
                                        say("Gidiyor musun? Belki haklýsýn. Uðursuzluk ve ")
                                        say("üzünç hayatýn bir parçasý.Kendine dikat et")
                                        say("Görüþmek üzere. þayet kararýný deðiþtirirsen,")
                                        say("sonra bir daha gel.")

                                        pc.setqf("talisman", 0)
                                end
                        end

                end -- end_of_when
        end -- end_of_state
end -- end_of_quest
