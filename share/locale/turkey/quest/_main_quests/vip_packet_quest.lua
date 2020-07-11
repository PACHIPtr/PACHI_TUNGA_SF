--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest vip_packet begin
	state start begin
		when login or levelup with pc.level > 15 begin
			set_state(order_vip)
		end
	end
	state order_vip begin
		when login or enter begin
			send_letter("&Paketler")
		end
		when button or info begin
			say_title("Paketler: ")
			say("")
			say("Karakterini güçlendirecek paketler nesne markette.")
			say("Karakterinin geliþimini saðlayacak ekstra güçlendirmeler")
			say("120. seviyeye ulaþmana yardýmcý olacak.")
			set_state(_COMPLETE)
		end
	end
	state _COMPLETE begin
	end
end