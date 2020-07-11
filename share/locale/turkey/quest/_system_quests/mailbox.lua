--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest mailbox_open begin
	state start begin
		when 30308.chat."Posta Kutusu" begin
			game.open_mailbox()
			setskin(NOWINDOW)
		end

		when 72340.use begin
			game.open_mailbox()
		end
	end
end