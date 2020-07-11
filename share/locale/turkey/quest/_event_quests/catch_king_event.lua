--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest catch_king_event begin
	state start begin
		when 20506.chat." Kralý Yakala " begin
			local s = select("Toplam Puan", " En Ýyi Skorlar", "Kapat")
			if s == 1 then
				local totalScore = game.mini_game_catch_king_get_score(1)
				local countTotal = table.getn(totalScore)

				if countTotal == 0 then
					say_title("Toplam Puan")
					say("")
					say(" Þu anda kayýtlý puan yok.")
					say("")
					return
				else
					say_title("Toplam Puan")
					say("")
					for i = 1, countTotal do
						say(string.format(" % d yerleþtirin:% s,% d", i, totalScore[i][1], totalScore[i][3]))
					end
					say("")
				end
			elseif s == 2 then
				local maxScore = game.mini_game_catch_king_get_score(0)
				local countMaxScore = table.getn(maxScore)
				
				if countMaxScore == 0 then
					say_title(" En Ýyi Skorlar")
					say("")
					say(gameforge.Catch_King._8)
					say("")
					return
				else
					say_title(" En Ýyi Skorlar")
					say("")
					for i = 1, countMaxScore do	
						say(string.format(" % d yerleþtirin:% s,% d", i, maxScore[i][1], maxScore[i][3]))
					end
					say("")
				end
			elseif s == 3 then
				return
			end
		end
		
        when 79603.pick or 79603.use with pc.count_item(79603) >= 25 begin
            pc.remove_item(79603, 25)
            pc.give_item2(79604)
        end
	end
end
