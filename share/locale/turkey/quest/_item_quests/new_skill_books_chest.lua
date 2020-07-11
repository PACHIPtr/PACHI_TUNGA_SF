--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest new_skill_books_chest begin
	state start begin
		when 50296.use begin
			pc.give_item2(50515+number(1,9))
			pc.give_item2(50515+number(1,9))
			pc.give_item2(50515+number(1,9))
			pc.give_item2(50515+number(1,9))
			pc.give_item2(50515+number(1,9))
			pc.remove_item("50296",1)
		end
		
		when 50297.use begin
			pc.give_item2(50559+number(1,9))
			pc.give_item2(50559+number(1,9))
			pc.give_item2(50559+number(1,9))
			pc.give_item2(50559+number(1,9))
			pc.give_item2(50559+number(1,9))
			pc.remove_item("50297",1)
		end
		
		when 50298.use begin
			pc.give_item2(50323)
			pc.give_item2(50323)
			pc.give_item2(50323)
			pc.give_item2(50323)
			pc.give_item2(50323)
			pc.remove_item("50298",1)
		end
		
		when 50299.use begin
			pc.give_item2(50324)
			pc.give_item2(50324)
			pc.give_item2(50324)
			pc.give_item2(50324)
			pc.give_item2(50324)
			pc.remove_item("50299",1)
		end
	end
end