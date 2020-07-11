--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest basic_weapon begin
	state start begin
		when login with pc.getqf("basic_weapon") == 0 begin
			notice_all(string.format("Myte2'ye hoþgeldin %s , Keyifli oyunlar ! ",pc.get_name()))
			pc.setqf("basic_weapon",1)
		end
	end
end