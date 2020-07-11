--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest dragon_soul_refine begin
	state start begin
		when 20001.chat."Ejderha Taþý Arýndýrma " with ds.is_qualified() != 0 begin
			say_title(mob_name(20001))
			say ("Ejderha Taþý mý yükseltmek istiyorsun?[ENTER]Tam yerindesin. Ama þunu bilmelisin yükseltmeler[ENTER]baþarýsýzda olabilir. Yükseltme yaparken, farklý renkte[ENTER]fasülyelere ihtiyaç duyacaksýn.")
			ds.open_refine_window()
		end
	end
end
