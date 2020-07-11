--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest guild_building_npc begin
    state start begin
        when 20044.click begin
            if npc.get_guild() == pc.get_guild() then
                ---                                                   l
                say_title("Silah Ustasý:")
				say("Görünüþe göre sen bizim loncadansýn. Bu sana %10")
				say("baþarý þansý kazandýrýr.")
				say("Yanýsýra bizim üyelerimize %5 fiyat indirimimiz")
				say("olur. Hoþgeldin evlat!")
				say("Ben silahlarý geliþtirebilirim.")
				say("Þimdi bana geliþtirmek istediðin nesneyi ver.")
            else
                ---                                                   l
                say_title("Silah Ustasý:")
                say("Benim adým Stanley. Ben silah geliþtirme ")
                say("sorumlusuyum ve bu iþi köydeki demirci ustasýndan")
                say("daya iyi yaparým! Bana geldiðin zaman")
                say("Bana geldiðin zaman")
                say("geliþtirmedeki baþarý þansýn %10 daha fazla olur.")
                say("Þimdi bana geliþtirmek istediðin silahý ver.")
            end
        end
        when 20045.click begin
            if npc.get_guild() == pc.get_guild() then
                ---                                                   l
                say_title("Zýrh Ustasý:")
				say("Görünüþe göre sen bizim loncadansýn. Bu sana %10")
				say("baþarý þansý kazandýrýr.")
				say("Yanýsýra bizim üyelerimize %5 fiyat indirimimiz")
				say("olur. Hoþgeldin evlat!")
				say("Ben zýrhlarý, kalkanlarý ve kasklarý ")
				say("geliþtirebilirim. Þimdi bana geliþtirmek istediðin")
				say("nesneyi ver.")
            else
                ---                                                   l
                say_title("Zýrh Ustasý:")
                say("Ben Stanton. Ben zýrh geliþtirme sorumlusuyum. ")
                say("Köydeki demirciden daha yetenekliyim: baþarý ")
                say("þansým %10 daha fazla.")
                say("Þimdi bana geliþtirmek istediðin nesneyi ver.")
            end
        end
        when 20046.click begin
            if npc.get_guild() == pc.get_guild() then
                ---                                                   l
                say_title("Kuyumcu: ")
				say("Görünüþe göre sen bizim loncadansýn. Bu sana %10")
				say("baþarý þansý kazandýrýr.")
				say("Yanýsýra bizim üyelerimize %5 fiyat indirimimiz")
				say("olur. Hoþgeldin evlat!")
				say("Ben bilezikleri, küpeleri ve ayakkabýlarý ")
				say("geliþtirebilirim. Þimdi bana geliþtirmek istediðin")
				say("aksesuarý ver.")
            else
                ---                                                   l
                say_title("Kuyumcu:")
                say("Adým Starbuck. Ben aksesuarlarý geliþtirmekten")
                say("sorumluyum. Özel mineral, metal ve tüccarlarla")
                say("çalýþýyorum, haliyle kaliteli oluyor. Köydeki")
                say("demircilerden bu konuda daha iyiyimdir.")
                say("Eðer onlar yerine bana gelirsen, baþarý þansýn %10")
                say("artacaktýr. Ben bilezikleri, küpeleri ve")
                say("ayakkabýlarý geliþtiririm.")
				say("Þimdi bana geliþtirmek istediðin aksesuarý ver.")
            end
        end
    end
end
