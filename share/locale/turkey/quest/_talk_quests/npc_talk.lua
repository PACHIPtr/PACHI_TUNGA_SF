--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest npc_talk begin
    state start begin
        when 20354.click begin
            say_title("Þehir Bekçisi:")
            say("Sessiz ve disiplinli ol. Kendine hakim ol.")
            say("Herhangi bir sorun baþlatma. Yoldaþlarýna saygýlý ")
            say("olmayý öðrenmelisin. Anladýn mý? ")
            say("")
            say("")
        end
        when 20008.chat."Mmm, burasý harika kokuyor..." begin
            say_title("Restoran sahibi Octavio:")
            say("")
            say("Hoþgeldin! Restoranýmýz uluslar arasý bir zincirin")
            say("parçasýdýr. Gel ve bizi bir dene.")
            say("içeri gelin ve bizi deneyin.")
            say("")

            if math.random(2) == 1 then
                wait()
                say_title(""..pc.get_name()..":")
                say("")
                say("Büyük bir restoran için, dekor")
                say("þaþýrtýcý derecede basit.")
                say("")
            end
        end
        when 20005.chat."Bana çömlekçilik hakkýnda bir þeyler anlat." begin
            say_title("Çömlekçi Yonah:")
            say("Çömlekçiliði seviyorum. Ellerinle çamuru ")
            say("þekillendirmek ve bir þey yaratmak çok tatmin")
            say("edici... Genelde basit þeyler yapýyorum.")
            say("Doðudaki daðlarýn eteklerinde yöntemler çok ")
            say("daha geliþmiþ. Bir defasýnda, orada bazý ")
            say("çömlekçiler görmüþ ve çok etkilenmiþtim. ")
            say("Zamaným olduðunda deneyler yapýyorum. Ama hala")
			say("ustalýklarýný çözemedim. ")
        end
		when 20005.chat."Yaþýna göre çok hayat dolusun..." begin
            say_title("Çömlekçi Yonah:")
            say("Ben gerçekten yaþlý bir adamým ancak hala")
            say("saðlýklý ve dincim. Çünkü sýrrým var.")
            say("Gango Kökü Suyu ve Tue Mantarýndan yapýlan ")
            say("ömür uzatýcý bir karýþým buldum. Eðer bu")
            say("karýþýmdan istersen, malzemeleri Yu-Hwan'a ")
            say("götürmelisin, senin için o iksiri yapacaktýr. ")
            say("")
			say("")
        end
        when 20017.chat."Git, onu daðlara söyle... " begin
            say_title("Müzisyen Yu-Hwan:")
            say("")
            say("...Tepelere ve her yere...")
            say("Sen de müziði sever misin? Burada her tür ")
            say("parçayý söylerim, savaþa ")
            say("dahil olmuyorum.Ancak eðer ülkem beni")
            say("korumazsa, sevdiðim bu iþi yapmaya devam")
            say("edemem.")
        end
        when 20023.chat."Neden bu kadar çok okuyorsun? " begin
            say_title("Soon:")
            say("")
            say("Ah, Maceracý.Dünya sorunlarla dolu ve tek")
            say("kurtuluþ yolu bilgi. Her zaman kitaplarýna saygý ")
            say("göstermelisin. Onlar sana kötü zamanlarda yardým ")
            say("ederler. Ýnan bana!")
            say("")
            say("")
        end
		when 20023.chat."Çocuklarýn var mý? " begin
            say_title("Soon:")
            say("Aslýnda hiç çocuk istemiyordum, ancak... Sihir")
            say("enerjimi arttýrmak için Kaki Çiçekleri ")
            say("toplayýp doðru þekilde hazýrlýyorum. Bunu")
            say("Baek-Go buldu. 100 adet iksir yapmak için bir")
            say("tane çiçeðe ihtiyacýn var. Ancak Kaki")
            say("Çiçeðinin bir de yan etkisi var. Eðer bir")
            say("kadýn bu çiçekten yapýlmýþ bir kolye")
			say("takarsa kýsa zamanda oðlu olur. Aramýzda ")
			say("kalsýn, karým utanmazca kullandý bunu.")
		end

        when 20020.chat."Siyah Rüzgar Çetesi'nin bir kurbaný " begin
            say_title("Hain Balso:")
            say("DÝkkatli ol! þu anda Siyah Rüzgar Grubu")
            say("savaþçýlarý göz açtýrmýyor. En iyisi bu görevi")
            say("ertelemek olacak. Hoþçakal...")
            say("")
                wait()
                say_title("Bilgi:")
                say_reward("Siyah Rüzgar Grubu'nun Sýrrý - görevi")
                say_reward("þu anda henüz hazýr deðil...")
                say("")
                say("")
            end
		 when 20020.chat."Neden insanlar sana hain diyor?" begin
            say_title("Hain Balso:")
            say("Bir savaþtan kaçtým ve Siyah Rüzgar Çetesine")
            say("katýldým. Yoldaþlarým bu yüzden beni asla")
            say("affetmiyorlar. Sonradan Siyah Rüzgar Çetesinin")
            say("masum insanlarý öldürdüðünü anladým ve")
            say("hemen onlarý terk ettim. Korkunçtu! ")
            say("")
		if math.random(2) == 1 then
                wait()
			say_pc_name()
			say("Masum insanlarýn vebalini mi taþýyorsun?")
			say("Öyleyse ismini gerçekten hak ediyorsun. Onlarý ")
			say("güzel bir hayattan kopartmýþsýn. ")
				wait()
			say_title("Hain Balso:")
			say("Burasý adaletsiz bir dünya. Ama artýk asla")
			say("kötü bir þey yapmayacaðým. ")
			say("Ýnsanlarýn beni hain olarak çaðýrmasýný ")
			say("istemiyorum.")
			say("Bana biraz Yang verebilir misin?")
        end
		end
		when 20020.chat."Bu kadar yara izi nasýl oldu?" begin
            say_title("Hain Balso:")
            say("Çok iyi bir savaþçý deðilim ve savaþlarda")
            say("çok sýk yaralanýrým. Dövüþlerde ")
            say("yaralandýktan sonra Yu-Hwan'a beni yaralardan")
            say("korumasý için bir iksir yapmasýný ")
			say("söylemiþtim. Bana Çan Çiçeði ve Leylaktan")
			say("oluþan bir iksir yaptý anca pek iþe ")
			say("yaramadý. Belki de benim beceriksizliðim. Senin")
			say("iþine yarayacaktýr.")
			say("")
			end

        when 20011.chat."Sana neden bilge diyorlar?" begin
            say_title("Bilge Uriel:")
            say("")
            say("Bu engin dünyada öðrenecek çok þey var. Cehalet")
            say("ve silahlý kuvvetleri birleþtirmek çok berbat bir")
            say("karýþým. Cahil askerlerle yaþamak istemiyorsun")
			say("deðil mi? Bilgelik güçtür!")
            say("")
        end

        when 20018.chat."Kaynaklarým senin ilaç yaptýðýný söyledi. " begin
            say_title("Baek-Go:")
            say("Elimden geleni yapýyorum. Bu tehlikeli dünyada")
            say("bir çok insan her gün yaralanýyor. Bizim o")
            say("kadar ilaç ve doktorumuz yok. Zaman çok zor!")
            say("Bana daha sonra yardým edebilir misin?")
            say("")
        end

        when 20019.chat."Av iþleri nasýl gidiyor?" begin
            say_title("Avcý Yang-Shin:")
            say("")
            say("Hmm, Metin Taþlarýnýn düþmesiyle birlikte")
            say("hayvanlar vahþileþti. Bugünlerdei tavuk")
            say("avlamak bile seni öldürebilir. Avcýlarýn iþi ")
            say("çok zor. Neyse ki ben en iyilerden biriyim!")
            say("")
        end
		when 20019.chat."Bu dikkatini neye borçlusun? " begin
            say_title("Avci Yang-Shin:")
            say("Ben baþarýlý bir okçuyum çünkü ")
            say("Çan Çiçeði Suyu ve Leylak karýþtýrýrsan elde")
            say("edeceðin iksir senin niþan alma kabiliyetini")
            say("inanýlmaz artýrýr. Eðer bu saydýklarýmý ")
            say("Yu-Hwan'a götürürsen senin için iksiri")
            say("yapacaktýr.")
        end
		
		 when 20006.chat."Ne güzel bir çiçek tacý... " begin
            say_title("Mirine:")
            say("Ben kardeþimle birlikte þeftali çiçeði ")
            say("topluyorum. Ama bugün yalnýzdým. Çok güzel")
            say("deðiller ama özel etkileri var. Eðer sen")
            say("de onlardan birini alýp ")
			say("Baek-Go'ya götürebilirsen, seni")
			say("çevikleþtirecek bir iksir yapabilir.")
        end
        when 20006.chat."Kardeþin nerede?" begin
            say_title("Mirine:")
            say("Bugünlerde kardeþim çok seyahat ediyor, onu")
            say("artýk göremiyorum ve endiþeleniyorum...")
            say("")
        end

        when 20012.chat."Pirinç pastasý mý satýyorsun?" begin
            say_title("Pirinç hamuru satýcýsý Yu-Rang:")
            say("Savaþ yüzünden, pirinç hamuru satmak çok")
            say("zorlaþtý. Haydutlara çok teþekkür etmek lazým,")
            say("sayelerinde bütün müþterilerimi kaybettim. Sadece")
            say("artýk hayatýmý sürdürebilmek için ne yapacaðýmý ")
            say("bilmiyorum.")
        end
		
		when 20012.chat."Senden kimler alýþveriþ yapýyor?" begin
            say_title("Pirinç hamuru satýcýsý Yu-Rang:")
            say("Pastalarýmýn çoðunu seyyahlara satýyorum.")
			say("Bir gün, gizemli bir süvari")
			say("geldi ve benden pirinç pastasý almak")
			say("istediðini söyledi. Fakat bana para yerine Zin")
			say("Suyunun reçetesini verdi. Bu iksir saldýrý gücünü ")
			say("arttýrabiliyormuþ.")
			say("Reçeteyi Huahn-So'ya sattým. Ona")
			say("Sim-suyu ve Alp gülü ")
			say("götürürsen senin için Zin Suyu yapabilir.")
        end

        when 20021.chat."Neden bu kadar üzgünsün?" begin
            say_title("Ariyoung:")
            say("Yeni evli çift olarak tatlý rüyalarýmýzýn")
            say("merhametsizce paramparça edileceðini bilmeliydim.")
            say("Ühü, ühü...lütfen bu cinayetin intikamýný al.")
            say("")
        end
		when 20021.chat."Savaþta çok kiþi öldü..." begin
            say_title("Ariyoung:")
            say("Savaþta birçok adam kaybettik, biz kadýnlar")
            say("olarak toplandýk ve Huahn-So'dan savunmamýzý ")
            say("güçlendirecek olan Dok-suyunu geliþtirmesini")
            say("istedik. Uzun araþtýrmalardan")
			say("sonra, eðer Dok-suyuna Alp Gülü eklenirse çok ")
			say("daha güçlü olacaðýný buldu. Kendisine git, o")
			say("Sambo-suyu yapabiliyor. Gerekli þeyleri götürmeyi")
			say("de unutma.")
        end
		when 20003.chat."Siz ikiniz kimsiniz?" begin
            say_title("Ah-Yu:")
            say("Evladým! Kocam savaþta düþtü, artýk ben kendime")
            say("bakmak zorundayým.Artýk çocuðuma ve kendime ben")
            say("bakmaya çalýþýyorum. Bo-suyu ile hayatýmý ")
			say("geliþtirebileceðimi öðrendim. Yu-Hwan'a 100")
			say("Kaki Çiçeði Suyu ve Tue Mantarý götürmeliyim ki")
			say("bana ondan yapsýn. ")
			say("Þimdi bir þekilde bu malzemeleri bulmalýyým...")
			say("")
        end
        when 20002.chat."Senin kocan kim?" begin
            say_title("Aranyo:")
            say("")
            say("Benim kocam olacak adam, kitaplarýyla evli. O")
            say("artýk benimle ilgilenmiyor. Ama ben çirkin bir")
            say("kadýn deðilim. ")
            say("")
        end
        when 20010.chat."Ýþler nasýl?" begin
            say_title("Seyyar Satýcý:")
            say("")
            say("Savas baþladýðýndan beri iþlerim")
            say("çok kötü gitmeye baþladý. ")
            say("Daha önceleri tüccarlar birliði ")
            say("çok güçlüydü, fakat þimdi her þey ")
            say("tersine döndü. Vahþi hayvanlara,")
            say("Savasçýlara ve Siyah Rüzgar çetesine")
            say("daha fazla aldýrmamazlýk edemiyoruz.")
            say("Sanki bizi kullanýyorlar...")
        end
		when 20010.chat."Evlilik Dükkaný " begin
			setskin(NOWINDOW)
			npc.open_shop()
		end
		when 20042.chat."Evlilik Dükkaný " begin
			setskin(NOWINDOW)
			npc.open_shop()
		end
        when 20014.chat."Neden moralin bozuk?" begin
            say_title("Taurean:")
            say("")
            say("Harang kötü bir kýz! Ondan nefret ediyorum! Ben")
            say("zayýfým ve o hep bana çatýyor!")
            say("")
        end
        when 20024.chat."Þu anda ne yapýyorsun?" begin
            say_title("Harang:")
            say("Taurean'ý bekliyorum.Beraber oyun oynarýz ve")
            say("çok da kavga ederiz. Ne korkak ama! Eðer kendi")
            say("isteðiyle gelmezse onu çok fena yapacaðým! ")
            say("")
        end
		when 20024.chat."Taurean senden korkuyor." begin
            say_title("Harang:")
            say("Taurean benden daha güçlü. Bu yüzden onu")
            say("yenmek için bir yol düþündüm. Beyaz bir")
            say("Ninja Yöng Suyu ile Dutu karýþtýrýrsam")
            say("saldýrý hýzýmýn artacaðýný söyledi.")
			say("Sonunda Taurean'ý yenebileceðim! Eðer sen de")
			say("hýzlý saldýrmak istiyorsan, malzemeleri")
			say("Huahn-So'ya götür. Sana iksir yapacaktýr.")
        end
		when 20086.chat."Ayaðýna ne oldu?" begin
			say_title("Handu-Up:")
			say("")
			say("Ýyileþiyor. Yürürken bir Gango Köküne")
			say("takýldým ve ayaðým burkuldu. Neyseki doktor")
			say("Baek-Go'yu tanýyorum, beni muayene etti ve bir")
			say("tavsiye verdi: Küçük yaralarý iyileþtirmek")
			say("için Gango Kökü suyu kullanabilirmiþim.")
			say("Böylece suçlu kökleri çaya karýþtýrdým ve")
			say("yol açtýklarý acýyý dindirdiler...")
			say("")
			end
		when 20368.chat."3. seviyeye geçiþ " begin
			say_title("Seyyar Satýcý:")
			say("Þeytanýn Katakomb'una hoþgeldin.") 
			say("Buraya kadar gelmek basit, burdan") 
			say("sonrasýný görmek istiyorsan güçlü bir") 
			say("grup liderine ihtiyacýn olacak.") 
			say("Ýlerlemek istiyorsan 8 adet Küçülen Kafa") 
			say("bulundurmalýsýn. O küçük þeytani baþcýklar") 
			say("sende mevcutsa Kaplumbaða Kayalýðý ") 
			say("seni bir sonraki seviyeye taþýyacaktýr.") 
			say("Bu eþyayý dönen paranýn altýndan ya da") 
			say("Þeytan Kulesi'ndeki Azrail'den elde") 
			say("edebilirsin. Unutma 8 adet baþçýk olmadan") 
			say("geçmenize izin verilmeyecektir.")
		end
		when 20084.chat."Araþtýrma Penceresi " begin
			setskin(NOWINDOW)
			cmdchat("OpenBiologWnd")
		end
	end
end
