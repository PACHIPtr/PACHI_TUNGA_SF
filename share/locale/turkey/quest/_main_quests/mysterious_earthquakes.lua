--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]

quest mysterious_earthquakes begin
	state start begin
		when login or levelup or enter with pc.get_level() >=55 begin
			 set_state(information)
		end
	end

	state information begin
		when letter begin
			send_letter("&Depremlerin sýrrý ")
			local v=find_npc_by_vnum(20011)
			if 0==v then
			else
				target.vid("__TARGET__", v, "Uriel")
			end
		end
					
		when button or info begin
			say_title("Yongbi Çölü sarsýntýlarý ")
			say("")
			say("Uriel zor durumda! Git ve yapabileceðin bir")
			say("þey var mý öðren.")
		end

		when __TARGET__.target.click or 20011.chat."Depremlerin sýrrý " begin
			target.delete("__TARGET__")
			say_title("Yongbi çölü sarsýntýlarý ")
			say("")
			say("Merhaba cesur savaþçý!")
			say("Nakajima ve çetesi son Yongbi çölü ziyaretlerinde")
			say("ilginç olaylar yaþamýþlar. Anllattýklarýna göre")
			say("örümcek zindaný yakýnlarýna ulaþtýklarý zaman")
			say("daha ileriye gidememiþler. Sarsýntýlar o kadar")
			say("þiddetliymiþ. Düþünebiliyor musun? Son 20.000")
			say("yýldýr bu topraklarda en ufak bir deprem bile")
			say("olmuyor!")
			wait()
			say_title("Yongbi çölü sarsýntýlarý ")
			say("")
			say("Bu çok ilginç... Belki arkadaþým Pung-Ho'ya benim")
			say("için bir ziyarette bulunabilirsin. Kendisi örümcek")
			say("zindanýnda yaþýyor ve gerçekten çok sevdiðim bir")
			say("dostum. Bu deprem hikayelerini duyduktan sonra")
			say("onun için çok endiþeleniyorum. Umarým bir þey")
			say("olmamýþtýr. Bunu benim için yapar mýsýn?")
			say("Karþýlýðýnda seni ödüllendireceðim.")
			say("")
			local acceptq=select("Evet", "Hayýr")
			if acceptq == 1 then
				say_title(pc.get_name()..":")
				say("")
				say("Uriel adýna üzüldüm. Ona yardým etmeliyim.")		
				say("")
				say_reward("Geçiþ bileti kazandýn.")
				pc.give_item2(71095, 1)
				set_state(accept)		
			elseif acceptq == 2 then
				say_title(pc.get_name()..":")
				say("")
				say("Kendimi tehlikeye atmak istemiyorum.")		
				say("Üzgünüm ama bunu yapamam.")
				say("")
				wait()
			end		
		end		
	end	
		
	state accept begin
		when letter begin
			send_letter("&Depremlerin sýrrý ")
			local x=find_npc_by_vnum(20089)
			if 0==x then
			else
				target.vid("__TARGET2__", x, "Pung-Ho")
			end
		end
			
		when button or info begin
			say_title("Yongbi çölü sarsýntýlarý ")
			say("")
			say("Örümcek zindanýna git ve Pung-Ho hayatta mý ")
			say("öðren. Daha sonra durumunu Uriel'e bildir.")
			say("")
			wait()
		end
			
		when 20011.chat."Yongbi çölü sarsýntýlarý " begin		
			say_title("Uriel:")		
			say("")		
			say("Ne bu kadar erken mi döndün?")		
			say("...")
			say("Ah... Depremlerden çok korktun ve fikrini")
			say("deðiþtirdin deðil mi?")
			say("")
			say_reward("Görevi iptal etmek istiyor musun?")
			say("")
			local tooweak=select("Evet", "Hayýr")		
			if tooweak == 2 then		
				say_title("Uriel:")		
				say("")				
				say("Bana yardým ettiðin için çok teþekkür ederim.")
				say("Acele et! Onun baþýna bir þey gelmesinden")
				say("korkuyorum.")		
				say("")		
			else		
				say_title("Uriel:")		
				say("")				
				say("Korkularýný anlýyorum fakat bu bir ölüm kalým")
				say("meselesi! Senden baþkasýna güvenemem. Tek")		
				say("umudum sensin!")
				say("")		
				local tooweak2=select("Peki, bunu yapacaðým!", "Üzgünüm ama yapamam.")		
				if tooweak2 == 1 then	
					say_title("Uriel:")
					say("")		
					say("Çok teþekkür ederim. Ýyi þanslar!")
					say("")
					wait()
				elseif tooweak2 == 2 then
					say_title(""..pc.get_name()..":")
					say("")		
					say("Örümcek zindanýna yaklaþtýðýmda sarsýntýlar")
					say("beni çok korkuttu. Çok üzgünüm ama bunu")
					say("yapamam.")
					say("")
					say_reward("Görev baþarýsýz.")
					say("")		
					set_state(__FAILURE__)		
				end		
			end		
		end
	
		when __TARGET2__.target.click or 20089.chat."Pung-Ho, iyi misin?" begin
			target.delete("__TARGET2__")
			say_title("Pung-Ho:")
			say("")
			say("Merhaba sana nasýl yardým edebilirim?")
			say("...")
			----"123456789012345678901234567890123456789012345678901234567890"|
			say("Ah, Seni Uriel gönderdi çünkü benim için")
			say("endiþelendi öyle mi? Hahaha, Eski dostum Uriel!")
			say("Ona endiþelenmemesini söyle. Bu zindanýn duvarlarý ")
			say("kaya gibi!")
			say("")
			wait()
			say_title("Pung-Ho:")
			say("")
			say("Depremlerin nedenini merak ediyor musun?")
			say("Pekala, sana daha sonra anlatýrým. Þimdi Uriel'e")
			say("dön ve benim turp gibi olduðumu söyle!")
			say("")
			wait()
			set_state(back_to_uriel)
		end
	end
	
	state back_to_uriel begin
		when letter begin
			send_letter("&Uriel'e dön")
			local u=find_npc_by_vnum(200811)
			if 0==u then
			else
				target.vid("__TARGET3__", u, "Uriel")
			end
		end
			
		when button or info begin
			say_title("Uriel'e dön")
			say("")
			say("Uriel'e dön ve güzel haberi ona ver!")
			say("")
		end
			
		when __TARGET__.target.click or 20011.chat."Pung Ho'nun durumu hakkýnda" begin
			target.delete("__TARGET__")
			say_title(""..pc.get_name()..":")
			say("")
			say("Endiþelenmene gerek yok Uriel!")
			say("Pung-Ho çok iyi durumda. Örümcek zindaný ")
			say("onun güvenliði için yeterince saðlam.")
			say("")
			wait()
			say_title("Uriel:")
			say("")
			say("Oh! Bana yardým ettiðin için çok teþekkür")
			say("ederim. Senin için küçük bir hediyem var.")
			say("")
			say_reward("10.000.000 Yang kazandýn.")
			say_reward("Elmas Matkap kazandýn.")
			say("")
			pc.change_money(10000000)
			pc.give_item2(81102, 1)
			clear_letter()		
			set_state(__COMPLETE__)
			q.done()
		end		
	end		
		
	state __COMPLETE__ begin		
	end		
	
	state __FAILURE__ begin		
	end		
end	