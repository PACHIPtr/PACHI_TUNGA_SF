--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest eggs_hunt begin
	state start begin
		function isMetinStone(npcVnum)
			if npcVnum >= 8041 and npcVnum <= 8050 then
				return true
			end
			
			return false
		end
		
		function checkLevelLimit(npcLevel, levelLimit)
			if (npcLevel - levelLimit <= pc.get_level()) and (pc.get_level() <= npcLevel + levelLimit) then
				return true
			end
			
			return false
		end
		
		function CountItemRange(firstVnum, lastVnum)
			local amount = 0
			for item = firstVnum, lastVnum, 1 do
				local i = pc.count_item(item)
				amount = amount + i
			end
			return amount
		end
		
		function RemoveItemRange(amountLeft, firstVnum, lastVnum)
			if eggs_hunt.CountItemRange(firstVnum, lastVnum) < amountLeft then
				return false
			end
			for currentVnum = firstVnum, lastVnum, 1 do
				local currentAmount = pc.count_item(currentVnum)
				if currentAmount > 0 then
					if currentAmount < amountLeft then
						pc.remove_item(currentVnum, currentAmount)
						amountLeft = amountLeft - currentAmount
					else
						pc.remove_item(currentVnum, amountLeft)
						return true
					end
				end
			end
		end
		
		function giveMagicEggReward()
			-- { itemVnum, itemCount }
			rewardTable = {
				{ 70102, 1 }, { 71108, 5 }, { 71026, 1 }, { 72043, 1 }, { 72304, 1 }, 
				{ 27992, 1 }, { 27993, 1 }, { 27994, 1 }, { 71094, 1 }, { 71084, 1 }, 
				{ 71085, 1 }, { 72308, 1 }, { 72724, 1 }, { 72728, 1 }, { 18900, 1 },
				{ 100400, 1 }
			}
			
			local rewardPos = number(1, table.getn(rewardTable)) 
			pc.give_item2(rewardTable[rewardPos][1], rewardTable[rewardPos][2])
		end
		
        function getRewardByLevel()
			-- { itemVnum, itemCount }
			rewardTable = {
				[1] = { -- Level 1 - 18
					{ 27001, 200 }, { 27004, 200 }, { 27100, 20 }, { 27103, 20 }, { 50701, 5 },
					{ 30053, 5 }, { 30073, 5 }, { 30033, 5 }, { 30010, 5 }, { 30032, 5 }, { 30031, 5 }
				},
				[2] = { -- Level 19 - 30
					{ 27002, 200 }, { 27005, 200 }, { 27101, 20 }, { 27104, 20 }, { 50701, 10 },
					{ 50703, 10 }, { 50704, 10 }, { 50702, 10 }, { 50300, 1 }, { 30196, 3 }, { 30090, 5 }
				},
				[3] = { -- Level 31 - 52
					{ 27003, 200 }, { 27006, 200 }, { 27102, 10 }, { 27105, 10 }, 
					{ 50701, 15 }, { 50703, 15 }, { 50704, 15 }, { 50702, 15 }, { 50706, 15 }, { 50707, 15 }, { 50705, 15 }, { 50708, 15 },
					{ 50300, 1 }, { 30195, 3 }, { 30193, 3 }, { 30197, 3 }
				},
				[4] = { -- Level 53 - 74
					{ 27003, 200 }, { 27006, 200 }, { 27102, 10 }, { 27105, 10 }, 
					{ 50701, 15 }, { 50703, 15 }, { 50704, 15 }, { 50702, 15 }, { 50706, 15 }, { 50707, 15 }, { 50705, 15 }, { 50708, 15 },
					{ 50300, 1 }, { 30198, 3 }, { 30199, 3 }
				},
				[5] = { -- Level 75 - 120
					{ 27102, 10 }, { 27105, 10 }, { 50091, 100 }, { 50092, 100 }, { 50093, 100 }, { 50094, 100 },
					{ 50300, 1 }, { 30195, 3 }, { 30193, 3 }, { 30197, 3 }, { 30198, 3 }, { 30199, 3 }
				},
			}
			
			local level = 1
			
			if pc.get_level() <= 18 then
				level = 1
			elseif pc.get_level() > 18 and pc.get_level() <= 30 then
				level = 2
			elseif pc.get_level() > 30 and pc.get_level() <= 52 then
				level = 3
			elseif pc.get_level() > 52 and pc.get_level() <= 74 then
				level = 4
			elseif pc.get_level() > 74 then
				level = 5
			end
			
			local item_table = rewardTable[level]
			local itemPos = number(1, table.getn(rewardTable[level])) 

            return rewardTable[level][itemPos][1], number(1, rewardTable[level][itemPos][2])
        end
	
		when 30129.chat."Paskalya Yumurtasý Getirdim!" with game.get_event_flag("enable_easter_event") >= 1 begin
			local eggsCount = eggs_hunt.CountItemRange(50160, 50179)
			local rewardVnum, rewardCount
			
			if eggsCount >= 20 then
				say_title("Paskalya Tavþaný:")
				say("Yeterli miktarda paskalya yumurtasý getirdin mi?")
				say("En az 20 paskalya yumurtasýna sahip olmalýsýn.")
				
				rewardVnum, rewardCount = eggs_hunt.getRewardByLevel()
				
				say("Eþya adeti "..rewardCount)
				say("Eþya:")
				
				say_item_vnum(rewardVnum)
				wait()
				
				say_title("Paskalya Tavþaný:")
				say("Eþya envanterine baþarýyla eklendi.")
				say("Paskalya yumurtalarý kaldýrýldý!")
				
				pc.give_item2(rewardVnum, rewardCount) 
				eggs_hunt.RemoveItemRange(20, 50160, 50179)
			else
				say("Yeterli miktarda yumurtan yok!")
				say("Bana daha fazla yumurta getirmelisin.")
			end
		end
		
		when 71150.use with game.get_event_flag("enable_easter_event") >= 1 begin
			if item.get_socket(0) == 1 then
				item.remove()
				eggs_hunt.giveMagicEggReward()
			else
				item.set_socket(0, item.get_socket(0) - 1)
				
				eggs_hunt.giveMagicEggReward()
			end
		end
		
		when kill with not npc.is_pc() and game.get_event_flag("enable_easter_event") >= 1 begin
			if eggs_hunt.isMetinStone(npc.get_race()) then
				if eggs_hunt.checkLevelLimit(npc.get_level(), 15) then
					local rNumber = number(1, 10000)
					if rNumber <= 1000 then
						game.drop_item_with_ownership(71150, 1)
					end
					if rNumber <= 9000 then
						local eggVnum = number(50160, 50179)
						game.drop_item_with_ownership(eggVnum, 1)
					end
				end
			end
		end
	end
end
