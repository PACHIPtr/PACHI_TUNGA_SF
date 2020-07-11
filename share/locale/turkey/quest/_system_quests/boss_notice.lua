--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest boss_notice begin
	state start begin
		when 1192.kill begin
			notice_all(string.format("CH%s: %s, Güçlü Buz Cadýsý'ný öldürdü!", pc.get_channel_id(), pc.get_name()))
			notice_all(string.format("Bilgi: Güçlü Buz Cadýsý 1 saatte bir Sürgün Maðarasý 1. Kat'ta çýkar."))
		end
		when 2206.kill begin
			notice_all(string.format("CH%s: %s, Alev Kralý'ný öldürdü!", pc.get_channel_id(), pc.get_name()))
			notice_all(string.format("Bilgi: Alev Kral 15 dakikada bir Doyyumhwan'da çýkar."))
		end
		when 2306.kill begin
			notice_all(string.format("CH%s: %s, Dev Hayalet Aðac'ý öldürdü!", pc.get_channel_id(), pc.get_name()))
			notice_all(string.format("Bilgi: Dev Hayalet Aðaç 1 saatte bir Kýzýl Orman'da çýkar."))
		end
	end
end