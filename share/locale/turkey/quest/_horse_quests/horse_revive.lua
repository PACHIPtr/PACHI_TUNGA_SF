--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest horse_revive begin
        state start begin
                when 20349.chat."Atýmý canlandýrmak istiyorum." with horse.get_grade()==3 and horse.is_dead() and pc.countitem("50059")<1 begin
                        say_title("Seyis:")
                        say("")
                        say("Atýný sadece, Güçlü Maymun zindanýndan ")
                        say("bulabileceðin bitkilerle canlandýrabilirsin. ")
                        say("Fakat, bunu baþarmak gerçekten zor. Çünkü bitkiler ")
                        say("maymunlar tarafýndan korunuyor. ")
                        say("Maymunlar çok sinirli. Bitkileri onlardan çalmak ")
                        say("isteyen, herkesi maymuna dönüþtürüyorlar. ")
                        if pc.countitem("50058")>=1 then
                                say_title("Seyis: ")
                                say("")
                                say("Bilgi: Normal Maymun zindanýndan toplanan ")
                                say("bitkiler sadece normal geliþimdeki atlar içindir. ")
                                say("")
                        elseif pc.countitem("50057")>=1 then
                                say_title("Seyis: ")
                                say("")
                                say("Bilgi: Zayýf Maymun zindanýndan toplanan ")
                                say("bitkiler sadece acemi atlar içindir. ")
                        end
                end
                when 20349.chat."Atýmý canlandýrmak istiyorum." with horse.get_grade()==3 and horse.is_dead() and pc.countitem("50059")>=1 begin
                        say("Bu bitkiler Güçlü Maymun zindanýndan toplandý. ")
                        say("Senin atýný canlandýracaklar. Haydi atý çaðýralým! ")
                        wait()
                        say("Bu otlarý ata yedir ve bekle. ")
                        say("[DELAY value=400]. . .[/DELAY] ")
                        horse.summon()
                        wait()
                        say("Atýn canlanmaya baþladý. ")
                        say("Gelecekte, atýný beslemeyi ihmal etme. ")
                        if pc.countitem("50059")>=1 then
                                pc.removeitem("50059", 1)
                                horse.revive()
                        end
                end
                when 20349.chat."Atýmý canlandýrmak istiyorum. " with horse.get_grade()==2 and horse.is_dead() and pc.countitem("50058")<1 begin
                        say("Atini sadece Normal Maymun zindanýndan bulabileceðin ")
                        say("bitkilerle canlandýrabilirsin. ")
                        say("Bitkileri maymunlar koruduklarý için, ")
                        say("bu gerçekten çok zor. Maymunlar çok öfkeli. ")
			say("Bitkileri almak isteyen herkesi ")
			say("lanetleyerek maymuna çeviriyorlar.. ")
			say("")
                        if pc.countitem("50059")>=1 then
                                say("Bilgi: Güçlü Maymun zindanýndan olan bitkiler sadece ileri düzeydeki atlar içindir. ")
                        elseif pc.countitem("50057")>=1 then
                                say("Bilgi: Zayýf Maymun zindanýndan olan bitkiler sadece acemi atlar içindir. ")
                        end
                end
                when 20349.chat."Atýmý canlandýrmak istiyorum." with horse.get_grade()==2 and horse.is_dead() and pc.countitem("50058")>=1 begin
                        say("Bu bitkiler Normasl Maymun zindanýndan toplandý. ")
			say("Senin atýný canlandýracaklar. ")
                        say("Haydi önce atý çaðýralým. ")
                        wait()
                        say("Bitkileri ata yedir ve bekle! ")
                        say("[DELAY value=400]. . .[/DELAY]")
                        horse.summon()
                        wait()
                        say("Atin canlanmaya baþlýyor. ")
                        say("Gelecekte onu beslemeyi unutma. ")
                        if pc.countitem("50058")>=1 then
                                pc.removeitem("50058", 1)
                                horse.revive()
                        end
                end
                when 20349.chat."Atimi canlandýrmak istiyorum. " with horse.get_grade()==1 and horse.is_dead() and pc.countitem("50057")<1 begin
                        say("Atýný sadece (1) Hasun Zindanýndan ")
			say("toplanacak bitkiler ile canlandýrabilirsin. ")
                        say("Bitkiler maymunlar tarafýndan ")
			say("korunduðu için bu gerçekten zor bir görev. ")
                        say("gerçekten zor bir görev. ")
			say("Maymunlar öfkeli! Bitkileri toplamak ")
                        say("isteyen herkesi lanetleyerek maymuna ")
			say("dönüþtürüyorlar. ")
                        if pc.countitem("50059")>=1 then
                                say("Bilgi: (3) Joongsun Zindanýndan toplanan bitkiler sadece ileri düzeydeki atlar içindir. ")
                        elseif pc.countitem("50057")>=1 then
                                say("Bilgi: (1) Hasun Zindanýndan toplanan bitkiler acemi atlar içindir. ")
                        end
                end
                when 20349.chat."Atýmý canlandýrmak istiyorum. " with horse.get_grade()==1 and horse.is_dead() and pc.countitem("50057")>=1 begin
                        say("Bu bitkiler (1) Hasun Zindanýndan toplandý. ")
			say("Atýný tekrar canlandýracaklar. ")
                        say("Haydi atýný çaðýralým! ")
                        wait()
                        say("Ata bitkileri yedir ve bekle. ")
                        say("[DELAY value=400]. . .[/DELAY]")
                        horse.summon()
                        wait()
                        say("Atýn canlanmaya baþladý. ")
                        say("Gelecekte atýný beslemeyi unutma. ")
                        if pc.countitem("50057")>=1 then
                                pc.removeitem("50057", 1)
                                horse.revive()
                        end
                end
        end
        state __COMPLETE__ begin
                when enter begin
                        q.done()
                end
        end
end
