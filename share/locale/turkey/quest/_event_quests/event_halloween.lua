--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest event_halloween begin
	state start begin
		function pumpkin_count()
			return 410000 + 4100*game.get_event_flag("halloween_round")
		end
		
		function set_collected_pumpkin(a,b)
			mysql.query("update log.halloween_event set collected_pumpkin=collected_pumpkin + "..a.." where pid="..b.."")
		end
		
		function set_round_reward()
			mysql.query("update log.halloween_event set round_reward=round_reward + 1 where collected_pumpkin>=100")
		end	
		
		function set_rank_reward()
			mysql.query("update log.halloween_event set rank_reward=rank_reward + 1 ORDER BY collected_pumpkin DESC LIMIT 5")
		end
		
		function delete_round_reward(a)
			mysql.query("update log.halloween_event set round_reward=0 where pid="..a.."")
		end
		
		function delete_rank_reward(a)
			mysql.query("update log.halloween_event set rank_reward=0 where pid="..a.."")
		end
		
		function delete_collected_pumpkin()
			mysql.query("update log.halloween_event set collected_pumpkin=0 where pid>0")
		end
		
		function get_collected_pumpkin(a)
			local x = mysql.query("select collected_pumpkin from log.halloween_event where pid="..a.." ORDER BY collected_pumpkin DESC LIMIT 1")
			if x == nil then
				return 0
			end
			local y = tonumber(x[1][1])
			return y
		end
		
		function get_round_reward(a)
			local x = mysql.query("select round_reward from log.halloween_event where pid="..a.." ORDER BY collected_pumpkin DESC LIMIT 1")
			if x == nil then
				return 0
			end
			local y = tonumber(x[1][1])
			return y
		end
		
		function get_rank_reward(a)
			local x = mysql.query("select rank_reward from log.halloween_event where pid="..a.." ORDER BY collected_pumpkin DESC LIMIT 1")
			if x == nil then
				return 0
			end
			local y = tonumber(x[1][1])
			return y
		end

		when login or enter with game.get_event_flag("ex_halloween_drop") > 0 begin
		local mapIndex = pc.get_map_index()
			if mapIndex == 1 or mapIndex == 21 or mapIndex == 41 then
				if find_npc_by_vnum(20416) == 0 then  
					if mapIndex == 1 then
						mob.spawn(20416, 608, 617, 1, 1, 1)
					elseif mapIndex == 21 then
						mob.spawn(20416, 596, 610, 1, 1, 1)
					elseif mapIndex == 41 then
						mob.spawn(20416, 357, 743, 1, 1, 1)
					end
				end
			end
			if pc.getqf("noticed") == 0 then
				send_letter("Ekþi mi tatlý mý?")
			end
			if pc.getqf("inserted") == 0 then
				mysql.query("insert into log.halloween_event values('"..pc.get_player_id().."', '"..pc.get_name().."', '0', '0', '0')")
				pc.setqf("inserted",1)
			end
		end
		
		when button or info begin
			say_title("Ekþi mi tatlý mý?")
			say("")
			say("Cadýlar Bayramý Etkinliði baþladý.")
			say("Sam pumpkin nerede? Herhalde yine sadece kendi")
			say("gördüðü hayaletlerin önüne saklanýyordur.")
			say("Harika ödüller kazanabilirsin!")
			say("")
			pc.setqf("noticed",1)
			clear_letter()
		end
		
		when 20416.chat."Ekþi mi tatlý mý?" begin
			say("K... Kim o? Sakýn bir adým daha yaklaþma!")
			say("Çok þükür! Ben de sanýmþtým ki...")
			say("Dur anlatayým:")
			say("Hayaletlerden çok korkarým! Cadýlar Bayramý ")
			say("gecelerinde dirilip etrafta dolaþýrlar!")
			say("Ha...hayaletleri kendimden uzak tutmak için")
			say("mutlaka bir kaç bal kabaðý fenerine ihtiyacým var!")
			say("Bana bunlardan getirir misin?")
			say("Ne kadar çok, o kadar iyi! Bana toplam "..event_halloween.pumpkin_count().." bal")
			say("kabaðý feneri getir. Ama lütfen acele et!")
			say("(Topladýðýn fenerleri Sam Pumpkin üzerine çek)")
			say("")
			if pc.getqf("inserted") == 0 then
				mysql.query("insert into log.halloween_event values('"..pc.get_player_id().."', '"..pc.get_name().."', '0', '0', '0')")
				pc.setqf("inserted",1)
			end
		end
		
		when 20416.chat."Toplananlarý kontrol et" begin
			say("En az "..event_halloween.pumpkin_count().." bal kabaðý fenerine ihtiyacým var.")
			say("Þimdiye kadar "..game.get_event_flag("collected_pumpkin").." adet toplanmýþ.")
			say("Hayaletleri sadece fener ýþýðý uzak tutar.")
			say("Beni karanlýkta býrakmayýn!")
			say("")
			say("Çizelge yeri")
			say(pc.get_halloween_rank())
			say("")
			say("Toplanan: "..event_halloween.get_collected_pumpkin(pc.get_player_id()))
			say("")
		end
		
		when 20416.chat."Ödülleri görüntüle" begin
			say("Toplama ödülü (en az 100 fener):")
			say_item_vnum(71191)
			say("")
			say("Çizelge konumu ödülü:")
			say_item_vnum(71190)
			say("")
		end
		
		when 20416.take with item.get_vnum() == 30336 and game.get_event_flag("ex_halloween_drop") > 0 begin
			local count = item.get_count()
			local remain = game.get_event_flag("collected_pumpkin") + count - event_halloween.pumpkin_count()
			local cur_round = game.get_event_flag("halloween_round") + 1
			if pc.getqf("inserted") == 0 then
				mysql.query("insert into log.halloween_event values('"..pc.get_player_id().."', '"..pc.get_name().."', '0', '0', '0')")
				pc.setqf("inserted",1)
			end
			syschat("Verilen bal kabaðý fenerleri: "..count)
			item.remove()
			if game.get_event_flag("collected_pumpkin") + count >= event_halloween.pumpkin_count() then
				if cur_round == game.get_event_flag("halloween_last_round") then
					notice_all("Yeterince bal kabaðý feneri toplandý.")
					notice_all("Bir ödül hak edip etmediðini Sam Pumpkin'e sor.")
					game.set_event_flag("ex_halloween_drop",0)
					game.set_event_flag("ex_halloween_drop_2",0)
					game.set_event_flag("halloween_reward",get_time()+12*60*60)
					game.set_event_flag("halloween_round",0)
				end
				game.set_event_flag("collected_pumpkin",remain)
				event_halloween.set_rank_reward()
				event_halloween.set_round_reward()
				event_halloween.delete_collected_pumpkin()
				event_halloween.set_collected_pumpkin(remain,pc.get_player_id())
				syschat("Toplam bal kabaðý feneri sayýsý: "..event_halloween.get_collected_pumpkin(pc.get_player_id()))
				return
			end
			event_halloween.set_collected_pumpkin(count,pc.get_player_id())
			game.set_event_flag("collected_pumpkin",game.get_event_flag("collected_pumpkin") + count)
			syschat("Toplam bal kabaðý feneri sayýsý: "..event_halloween.get_collected_pumpkin(pc.get_player_id()))
		end
		
		when 20416.chat."Ödülünü teslim al" begin
			if pc.getqf("inserted") == 0 then
				mysql.query("insert into log.halloween_event values('"..pc.get_player_id().."', '"..pc.get_name().."', '0', '0', '0')")
				pc.setqf("inserted",1)
			end
			
			if event_halloween.get_round_reward(pc.get_player_id()) == 0 and event_halloween.get_rank_reward(pc.get_player_id()) == 0 then
				say("Hey! Ne? Beni sürekli korkutmayý KES artýk! Zaten")
				say("yeterince korkuyorum. Beni koruyacak olan")
				say("fenerler nerede? Bunlardan bana yeteri kadar")
				say("getirirsen seni cömertçe ödüllendiririm.")
				return
			end
			
			if event_halloween.get_round_reward(pc.get_player_id()) > 0 then
				say("Vay canýna! "..event_halloween.get_round_reward(pc.get_player_id()).." adet þekerleme poþeti kazandýn!")
				say("")
				say_item_vnum(71191)
			say("Ödülünü þimdi almak ister misin?")
				say("")
				if select("Evet","Hayýr") == 1 then
					pc.give_item2("71191",event_halloween.get_round_reward(pc.get_player_id()))
					event_halloween.delete_round_reward(pc.get_player_id())
					if game.get_event_flag("halloween_reward") == 0 then
						return
					end
				else
					if game.get_event_flag("halloween_reward") == 0 then
						return
					end
				end
			end
			
			if event_halloween.get_rank_reward(pc.get_player_id()) > 0 then
				if game.get_event_flag("halloween_reward") - get_time() < 0 then
					say("Hey! Ne? Beni sürekli korkutmayý KES artýk! Zaten")
					say("yeterince korkuyorum. Beni koruyacak olan")
					say("fenerler nerede? Bunlardan bana yeteri kadar")
					say("getirirsen seni cömertçe ödüllendiririm.")
					return
				end
				say("Oooo! Çizelge konumun için "..event_halloween.get_rank_reward(pc.get_player_id()).." bal kabaðý kutusu")
				say("kazandýn!")
				say("")
				say_item_vnum(71190)
				say("Ödülünü þimdi almak ister misin?")
				say("")
				if select("Evet","Hayýr") == 1 then
					pc.give_item2("71190",event_halloween.get_rank_reward(pc.get_player_id()))
					event_halloween.delete_rank_reward(pc.get_player_id())
				end
			end
		end
		
		when login or enter with game.get_event_flag("ex_halloween_drop") == 0 begin
			if pc.getqf("inserted") == 1 then
				pc.setqf("inserted",0)
			end
			if pc.getqf("noticed") == 1 then
				pc.setqf("noticed",0)
			end
		end
	end
end	