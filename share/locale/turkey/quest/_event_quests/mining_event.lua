--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest miningevent begin
	state start begin
		function buildRanking()
			if get_time() < pc.getqf("mining_event_time") then
				say_title("  "..mob_name(20015))
				say("")
				say("Madencilik sýralamasýný 5 dakikada bir açabilirsin.")
				say("")
				return
			else
				say_title("  "..mob_name(20015))
				say("")
				say("Madencilik sýralamasý: ")
				pc.setqf("mining_event_time", get_time()+60*5)
				local res1, rankingPositions = mysql.query("SELECT * FROM log.mining_event ORDER BY total_points DESC LIMIT 10;")

				for saye = 1, 10 do
					if rankingPositions[saye].name != 0 then
						say(""..saye..".\t"..rankingPositions[saye].name.." - "..rankingPositions[saye].total_points.." Puan.")
					else
						say("" ..saye..".\tBoþ.")
					end
				end
				say("")
				say("")
			end
		end
		
		when 20015.chat."Madencilik Etkinliði" with game.get_event_flag("mining_event") == 1 begin
			say_title("   "..mob_name(20015))
			say("")
			say("Oh, sonunda buradasýn. Bende seni arýyordum.")
			say("Geleneksel madencilik etkinlikleri baþladý!")
			say("maden kazarak puan toplayabilecek ve")
			say("etkinlik sonunda ejderha parasý kazanabileceksin.")
			say("")
			say_reward("350 Puan karþýlýðý: 5 Ejderha Parasý ")
			say_reward("700 Puan karþýlýðý: 10 Ejderha Parasý ")
			say_reward("1000 Puan karþýlýðý: 15 Ejderha Parasý ")
			say_reward("Toplanan madencilik puaný: "..pc.getqf("currentMiningPoint").." Puan")
		end
		when 20015.chat."Madencilik Sýralamasý " with game.get_event_flag("mining_event") == 1 begin
			miningevent.buildRanking()
		end
		when 20015.chat."Madencilik Ödülleri " with game.get_event_flag("mining_event") == 2 begin
			say_title("   "..mob_name(20015))
			say("")
			say("Hoþgeldin. Ne kadar puan topladýysan")
			say("o kadar fazla ejderha parasý alacaksýn.")
			say_reward("Toplanan madencilik puaný: "..pc.getqf("currentMiningPoint").." Puan")
			say_reward("Devam etmek istiyor musun?")
			local miningSelect = select("Elbette!","Daha sonra.")
			if miningSelect == 1 then
				local miningSelectCash = select("350 Puan - 5 EP ", "700 Puan - 10 EP ", "1000 Puan - 15 EP ", "Vazgeç ")
				local miningPoints = pc.getqf("currentMiningPoint")
				if miningSelectCash == 1 then
					if miningPoints < 350 then
						say_title("   "..mob_name(20015))
						say("")
						say("Yeterli miktarda madencilik puanýna sahip deðilsin.")
					elseif pc.is_busy() == true then
						say_title("   "..mob_name(20015))
						say("")
						say("Meþgul durumda iken bu iþlemi gerçekleþtiremezsin.")
					elseif pc.is_dead() == true then
						say_title("   "..mob_name(20015))
						say("")
						say("Ölü durumda iken bu iþlemi gerçekleþtiremezsin.")
					else
						local result = pc.charge_cash(5, "cash")
						if result == true then
							say_title("   "..mob_name(20015))
							say("")
							say("Etkinlik ödülü olarak 5 ejderha parasý kazandýn.")
							pc.setqf("currentMiningPoint", pc.getqf("currentMiningPoint")-350)
						end
					end
				elseif miningSelectCash == 2 then
					if miningPoints < 700 then
						say_title("   "..mob_name(20015))
						say("")
						say("Yeterli miktarda madencilik puanýna sahip deðilsin.")
					elseif pc.is_busy() == true then
						say_title("   "..mob_name(20015))
						say("")
						say("Meþgul durumda iken bu iþlemi gerçekleþtiremezsin.")
					elseif pc.is_dead() == true then
						say_title("   "..mob_name(20015))
						say("")
						say("Ölü durumda iken bu iþlemi gerçekleþtiremezsin.")
					else
						local result = pc.charge_cash(10, "cash")
						if result == true then
							say_title("   "..mob_name(20015))
							say("")
							say("Etkinlik ödülü olarak 10 ejderha parasý kazandýn.")
							pc.setqf("currentMiningPoint", pc.getqf("currentMiningPoint")-700)
						end
					end
				elseif miningSelectCash == 3 then
					if miningPoints < 1000 then
						say_title("   "..mob_name(20015))
						say("")
						say("Yeterli miktarda madencilik puanýna sahip deðilsin.")
					elseif pc.is_busy() == true then
						say_title("   "..mob_name(20015))
						say("")
						say("Meþgul durumda iken bu iþlemi gerçekleþtiremezsin.")
					elseif pc.is_dead() == true then
						say_title("   "..mob_name(20015))
						say("")
						say("Ölü durumda iken bu iþlemi gerçekleþtiremezsin.")
					else
						local result = pc.charge_cash(15, "cash")
						if result == true then
							say_title("   "..mob_name(20015))
							say("")
							say("Etkinlik ödülü olarak 15 ejderha parasý kazandýn.")
							pc.setqf("currentMiningPoint", pc.getqf("currentMiningPoint")-1000)
						end
					end
				else
				end
			else
			end
		end
	end
end