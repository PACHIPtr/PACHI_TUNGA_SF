--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest warehouse begin
        state start begin --Status when nothing is open
			when warehouse_keeper.chat."Depoyu kullanmak istiyor musun?"  begin
				---                                                   l
				say_title("Depo Görevlisi:")
				say("")
				say("Depoyu kullanmak istiyor musun?")
				say("Depoyu kullanmak için biraz Yang ödemelisin.")
				say("Bu gün iyi günümdeyim.")
				say("500 Yang a yapabilirsin.")
				say("Gel ,ucuz, hu? Hehe.")
				say("")
				wait()
				if pc.get_money() < 500 then                                                
					say_title("Depocu:")
					say("")
					say("Ne? Yeterli Yang yok mu?")
					say("O zaman yeterli Yang'ýn olduðunda ")
					say("tekrar gel.")
					say("")
				else
					local s = select("Öde.", "Ödeme.")
					if s == 1 then
						--stash.setlevel(1)
						pc.changegold(-500)
						game.set_safebox_level(1)
						set_state(use)
						say_title("Depocu:")
						say("")
						---                                                   l
						say("Özel depon þu anda açýldý. ")
						say("Eðer onu kullanmak istiyorsan,sadece benimle")
						say("konuþ. ")
						say("Depo þifresi: ")
						say_reward("000000")
						say("Onu güvenliðinden dolayý ")
						say("deðiþtirmelisin.")
						say("")
						say("Belki 15 kutu yeterli olmaz. ")
						say("Ama kim bilir eðer birbirimizi seversek")
						say("belki sana daha fazla boþ alan veririm. Hehe!")
						say("")
					else
						say_title("Depocu:")
						say("")
						---                                                   l
						say("Oh güzel..Mayor istemezse ")
						say("bunu yapmak zorunda deðil..")
						say("")
					end
				end
			end
		end
        state use begin
			when warehouse_keeper.chat."Depoyu aç " begin
				if pc.getqf("open_count") < 3 then
					---                                                   l
					say_title("Depocu:")
					say("")
					say("Hoþgeldin !")
					say("Yang külçeler ne için biliyor musun?")
					say("")
					say("Eðer üstünde Yang taþýrsan ")
					say("bu lütfen beni öldür ")
					say("demek gibi olur. ")
					say("")
					say("Bu yüzden insanlardaki Altýnlarý külçe altýna")
					say("dönüþtürdüm. Sizin için saklýyorum.")
					say("")
					wait()
					say_title("Depocu:")
					say("")
					say("Hým..")
					say("Peki,Bir külçe altýnýn deðerinden daha ")
					say("fazlasýný istiyorum. Ama bu hala adaletli,")
					say("deðil mi? Kötü düþünmek için bir neden yok.")
					say("Biliyorsun dýþarýsý çok ")
					say("tehlikeli.")
					say("")
					wait()
					pc.setqf("open_count", pc.getqf("open_count") + 1)
				end
				setskin(NOWINDOW)
				game.open_safebox()
			end
			when warehouse_keeper.chat."Nesne market deposunu aç " begin
				setskin(NOWINDOW)
				game.open_mall()
			end
			when warehouse_keeper.chat."Gümüþ ve Altýn Bar al " begin
				npc.open_shop()
				say_title("Depocu:")
				say("")
				say("Hýmm..")
				say("Tüm madenler tüketildi.")
				say("Artýk altýn ve gümüþ bulmak zor.")
				say("Yeni bir kaynak buluncaya kadar yeni altýn ve")
				say("gümüþ olmaacak.")
				say("")
			end
        end -- end_of_state: use
end -- end_of_quest
