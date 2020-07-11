--[[
	Myte2 Server Files
	PACHI | Tunga
	https://forum.turkmmo.com/uye/2127751-pachi/
--]]
quest questscroll5 begin
        state start begin
                when 50317.use begin
                        local s=0
                        local t=select("Aa", "Ab", "Ba", "Bb", "Bc", "Ca", "Cb", "Cc", "Cd", "Da", "Db", "Dc")
                        if 1==t then
                                s=select(
                                questscroll5.get_menu(1),
                                questscroll5.get_menu(2),
                                questscroll5.get_menu(3),
                                questscroll5.get_menu(4),
                                questscroll5.get_menu(5),
                                questscroll5.get_menu(6)
                                )+1-1
                        elseif 2==t then
                                s=select(
                                questscroll5.get_menu(7),
                                questscroll5.get_menu(8),
                                questscroll5.get_menu(9),
                                questscroll5.get_menu(10),
                                questscroll5.get_menu(11)
                                )+7-1
                        elseif 3==t then
                                s=select(
                                questscroll5.get_menu(12),
                                questscroll5.get_menu(13),
                                questscroll5.get_menu(14),
                                questscroll5.get_menu(15)
                                )+12-1
                        elseif 4==t then
                                s=select(
                                questscroll5.get_menu(16),
                                questscroll5.get_menu(17),
                                questscroll5.get_menu(18),
                                questscroll5.get_menu(19),
                                questscroll5.get_menu(20),
                                questscroll5.get_menu(21)
                                )+16-1
                        elseif 5==t then
                                s=select(
                                questscroll5.get_menu(22),
                                questscroll5.get_menu(23),
                                questscroll5.get_menu(24),
                                questscroll5.get_menu(25)
                                )+22-1
                        elseif 6==t then
                                s=select(
                                questscroll5.get_menu(26),
                                questscroll5.get_menu(27),
                                questscroll5.get_menu(28),
                                questscroll5.get_menu(29),
                                questscroll5.get_menu(30),
                                questscroll5.get_menu(31)
                                )+26-1
                        elseif 7==t then
                                s=select(
                                questscroll5.get_menu(32),
                                questscroll5.get_menu(33),
                                questscroll5.get_menu(34),
                                questscroll5.get_menu(35)
                                )+32-1
                        elseif 8==t then
                                s=select(
                                questscroll5.get_menu(36),
                                questscroll5.get_menu(37),
                                questscroll5.get_menu(38),
                                questscroll5.get_menu(39),
                                questscroll5.get_menu(40),
                                questscroll5.get_menu(41)
                                )+36-1
                        elseif 9==t then
                                s=select(
                                questscroll5.get_menu(42),
                                questscroll5.get_menu(43),
                                questscroll5.get_menu(44),
                                questscroll5.get_menu(45),
                                questscroll5.get_menu(46),
                                questscroll5.get_menu(47),
                                questscroll5.get_menu(48),
                                questscroll5.get_menu(49)
                                )+42-1
                        elseif 10==t then
                                s=select(
                                questscroll5.get_menu(50),
                                questscroll5.get_menu(51),
                                questscroll5.get_menu(52),
                                questscroll5.get_menu(53)
                                )+50-1
                        elseif 11==t then
                                s=select(
                                questscroll5.get_menu(54),
                                questscroll5.get_menu(55),
                                questscroll5.get_menu(56),
                                questscroll5.get_menu(57)
                                )+54-1
                        elseif 12==t then
                                s=select(
                                questscroll5.get_menu(58),
                                questscroll5.get_menu(59)
                                )+58-1
                        end
                        questscroll5.set_mission(s)
						if item.get_count() > 1 then
							item.set_count(item.get_count()-1)
						else
							item.remove()
						end
                end
                when 50318.use begin
                        local s=0
                        if is_test_server() then
                                test_chat("questscroll5.use.50317")
                                s=select(
                                questscroll5.get_menu(1),
                                questscroll5.get_menu(2),
                                questscroll5.get_menu(3),
                                questscroll5.get_menu(4),
                                questscroll5.get_menu(5),
                                questscroll5.get_menu(6),
                                questscroll5.get_menu(7),
                                questscroll5.get_menu(8),
                                questscroll5.get_menu(9),
                                questscroll5.get_menu(10),
                                questscroll5.get_menu(11)
                                )+1-1
                        else
                                s=number(1, 11)
                        end
                        questscroll5.set_mission(s)
						if item.get_count() > 1 then
							item.set_count(item.get_count()-1)
						else
							item.remove()
						end
                end
                when 50319.use begin
                        local s=0
                        if is_test_server() then
                                test_chat("questscroll5.use.50319")
                                s=select(
                                questscroll5.get_menu(12),
                                questscroll5.get_menu(13),
                                questscroll5.get_menu(14),
                                questscroll5.get_menu(15),
                                questscroll5.get_menu(16),
                                questscroll5.get_menu(17),
                                questscroll5.get_menu(18),
                                questscroll5.get_menu(19),
                                questscroll5.get_menu(20),
                                questscroll5.get_menu(21),
                                questscroll5.get_menu(22),
                                questscroll5.get_menu(23),
                                questscroll5.get_menu(24),
                                questscroll5.get_menu(25)
                                )+12-1
                        else
                                s=number(12, 25)
                        end
                        questscroll5.set_mission(s)
						if item.get_count() > 1 then
							item.set_count(item.get_count()-1)
						else
							item.remove()
						end
                end
                when 50320.use begin
                        local s=0
                        if is_test_server() then
                                test_chat("questscroll5.use.50320")
                                s=select(
                                questscroll5.get_menu(26),
                                questscroll5.get_menu(27),
                                questscroll5.get_menu(28),
                                questscroll5.get_menu(29),
                                questscroll5.get_menu(30),
                                questscroll5.get_menu(31),
                                questscroll5.get_menu(32),
                                questscroll5.get_menu(33),
                                questscroll5.get_menu(34),
                                questscroll5.get_menu(35),
                                questscroll5.get_menu(36),
                                questscroll5.get_menu(37),
                                questscroll5.get_menu(38),
                                questscroll5.get_menu(39),
                                questscroll5.get_menu(40),
                                questscroll5.get_menu(41),
                                questscroll5.get_menu(42),
                                questscroll5.get_menu(43),
                                questscroll5.get_menu(44),
                                questscroll5.get_menu(45),
                                questscroll5.get_menu(46),
                                questscroll5.get_menu(47),
                                questscroll5.get_menu(48),
                                questscroll5.get_menu(49)
                                )+26-1
                        else
                                s=number(26, 49)
                        end
                        questscroll5.set_mission(s)
						if item.get_count() > 1 then
							item.set_count(item.get_count()-1)
						else
							item.remove()
						end
                end
                when 50321.use begin
                        local s=0
                        if is_test_server() then
                                test_chat("questscroll5.use.50321")
                                s=select(
                                questscroll5.get_menu(50),
                                questscroll5.get_menu(51),
                                questscroll5.get_menu(52),
                                questscroll5.get_menu(53),
                                questscroll5.get_menu(54),
                                questscroll5.get_menu(55),
                                questscroll5.get_menu(56),
                                questscroll5.get_menu(57),
                                questscroll5.get_menu(58),
                                questscroll5.get_menu(59)
                                )+50-1
                        else
                                s=number(50, 59)
                        end
                        questscroll5.set_mission(s)
						if item.get_count() > 1 then
							item.set_count(item.get_count()-1)
						else
							item.remove()
						end
                end
                function get_menu(mission_index)
                        return locale.questscroll5.mission[mission_index][3]
                end
                function set_mission(mission_index)
                        pc.setqf("mission_index", mission_index)

                        local mission_type=questscroll5.get_mission_type()
                        if mission_type==QUEST_SCROLL_TYPE_KILL_MOB then
                                test_chat("KILL_MOB1")
                                set_state("kill1")
                        elseif mission_type==QUEST_SCROLL_TYPE_KILL_MOB2 then
                                test_chat("KILL_MOB2")
                                set_state("kill2")
                        end
                end
                function get_mission_type()
                        return questscroll5.get_mission()[1]
                end
                function notify_already_running()
                        say_title("Zaten bir görev almýþsýn.")
                        say("")
                        ----"12345678901234567890123456789012345678901234567890"|
                        say("Zaten bir adet görev almýþsýn. ")
                        say("Yeni bir görev alamazsýn.")
                        say("")
                end
                function show_mission_menu()
                        local s=select("Tamam", "Ýptal ")
                        if s==2 then
                                say_title(questscroll5.get_title().." Ýptali")
                                say("")
                                say("Ýptal etmek istediðine emin misin ?")
                                say("")
                                local a=select("Evet", "Hayýr")
                                if a==1 then
                                        say(questscroll5.get_title())
                                        say("")
                                        say("Görev iptal edildi.")
                                        say("")
                                        restart_quest()
                                end
                        end
                end
        end
        state kill1 begin
                when questscroll1.use or
                        50317.use or
                        50318.use or
                        50319.use or
                        50320.use or
                        50321.use or
                        50322.use or
                        50323.use or
                        50324.use or
                        50325.use begin
                        questscroll5.notify_already_running()
                end
                when letter begin
                        send_letter(questscroll5.get_title())
                end
                when button or info begin
                        say_title(questscroll5.get_title())
                        say(questscroll5.get_desc())
                        say("")
                        say_reward(questscroll5.get_summ())
                        say_reward(string.format(
                                "(Kalan %s : %d)",
                                questscroll5.get_mob1_name(), questscroll5.get_mob1_rest_kill_count() ))
                        say("")
                        questscroll5.show_mission_menu()
                end
                when enter begin
                        pc.setqf("mob1_kill_count", 0)
                end
                when leave begin
                        pc.setqf("mob1_kill_count", 0)
                end
                when letter begin
                        q.set_counter(questscroll5.get_mob1_name(), questscroll5.get_mob1_rest_kill_count())
                end
                when kill begin
                        test_chat("questscroll5.kill_race:"..npc.get_race())
                        test_chat("questscroll5.dest_race:"..questscroll5.get_mob1_vnum())
                        if npc.get_race()==questscroll5.get_mob1_vnum() then
                                local mob1_kill_count=questscroll5.get_mob1_cur_kill_count()+1
                                questscroll5.set_mob1_cur_kill_count(mob1_kill_count)
                                q.set_counter(questscroll5.get_mob1_name(), questscroll5.get_mob1_rest_kill_count())
                                if mob1_kill_count>=questscroll5.get_mob1_max_kill_count() then
                                        set_state(reward)
                                end
                        end
                end
        end
        state kill2 begin
                when questscroll1.use or
                        50317.use or
                        50318.use or
                        50319.use or
                        50320.use or
                        50321.use or
                        50322.use or
                        50323.use or
                        50324.use or
                        50325.use begin

                        questscroll5.notify_already_running()
                end
                when letter begin
                        send_letter(questscroll5.get_title())
                end
                when button or info begin
                        say_title(questscroll5.get_title())
                        say(questscroll5.get_desc())
                        say("")
                        say_reward(questscroll5.get_summ())
                        say_reward(string.format("(Kalan %s : %d, %s : %d)",
                                questscroll5.get_mob1_name(), questscroll5.get_mob1_rest_kill_count(),
                                questscroll5.get_mob2_name(), questscroll5.get_mob2_rest_kill_count() ))
                        say("")
                        questscroll5.show_mission_menu()
                end
                when enter begin
                        pc.setqf("mob1_kill_count", 0)
                        pc.setqf("mob2_kill_count", 0)
                end
                when leave begin
                        pc.setqf("mob1_kill_count", 0)
                        pc.setqf("mob2_kill_count", 0)
                end
                when letter begin
                        q.set_counter(questscroll5.get_mob1_name(), questscroll5.get_mob1_rest_kill_count())
                end
                when kill begin
                        local npc_race=npc.get_race()
                        local mob1_kill_count=questscroll5.get_mob1_cur_kill_count()
                        local mob2_kill_count=questscroll5.get_mob2_cur_kill_count()

                        test_chat("questscroll5.kill_race:"..npc_race)
                        test_chat("questscroll5.dest_race1: ["..questscroll5.get_mob1_vnum().."]")
                        test_chat("questscroll5.dest_race2: ["..questscroll5.get_mob2_vnum().."]")
                        if npc_race==questscroll5.get_mob1_vnum() then
                                mob1_kill_count=mob1_kill_count+1
                                questscroll5.set_mob1_cur_kill_count(mob1_kill_count)
                                q.set_counter(questscroll5.get_mob1_name(), questscroll5.get_mob1_rest_kill_count())
                        elseif npc_race==questscroll5.get_mob2_vnum() then
                                mob2_kill_count=mob2_kill_count+1
                                questscroll5.set_mob2_cur_kill_count(mob2_kill_count)
                                q.set_counter(questscroll5.get_mob2_name(), questscroll5.get_mob2_rest_kill_count())
                        end

                        if        mob1_kill_count>=questscroll5.get_mob1_max_kill_count() and
                                mob2_kill_count>=questscroll5.get_mob2_max_kill_count() then
                                set_state(reward)
                        end
                end
        end
        state reward begin
                when letter begin
                        send_letter(questscroll5.get_complete_title())
                end
                when button begin
                        restart_quest()

                        say_title(questscroll5.get_complete_title())
                        say("Görevi baþarýyla bitirdin.")
                        say("Ödülün:")
                        say("")

                        local reward_type=questscroll5.get_reward_type()
                        if number(0, 100)<=90 then
                                local s=number(1, 3)
                                if 1==s then
                                        questscroll5.reward_exp(reward_type)
                                        questscroll5.reward_money(reward_type)
                                elseif 2==s then
                                        questscroll5.reward_money(reward_type)
                                        questscroll5.reward_item(reward_type)
                                elseif 3==s then
                                        questscroll5.reward_exp(reward_type)
                                        questscroll5.reward_item(reward_type)
                                end
                        else
                                questscroll5.reward_exp(reward_type)
                                questscroll5.reward_money(reward_type)
                                questscroll5.reward_item(reward_type)
                        end

                        say("")
                end
                function get_mission()
                        return locale.questscroll5.mission[pc.getqf("mission_index")]
                end
                function get_title()
                        return questscroll5.get_mission()[3]
                end
                function get_complete_title()
                        return string.format("%s Tamamlandý!", questscroll5.get_title())
                end
                function get_desc()
                        return "[WINDOW_SIZE width;360|height;280]"..questscroll5.get_mission()[4]
                end
                function get_summ()
                        return questscroll5.get_mission()[5]
                end
                function get_mob1_name()
                        return mob_name(questscroll5.get_mob1_vnum())
                        -- return questscroll5.get_mission()[6]
                end
                function get_mob1_max_kill_count()
                        return questscroll5.get_mission()[7]
                end
                function get_mob1_vnum()
                        return tonumber(questscroll5.get_mission()[6])
                        -- return mob_vnum(questscroll5.get_mob1_name())
                end
                function get_mob1_rest_kill_count()
                        local ret=questscroll5.get_mob1_max_kill_count()-questscroll5.get_mob1_cur_kill_count()
                        if ret<0 then return 0 end
                        return ret
                end
                function set_mob1_cur_kill_count(value)
                        pc.setqf("mob1_kill_count", value)
                end
                function get_mob1_cur_kill_count()
                        return pc.getqf("mob1_kill_count")
                end

                function get_mob2_name()
                        return mob_name(questscroll5.get_mob2_vnum())
                        -- return questscroll5.get_mission()[8]
                end
                function get_mob2_max_kill_count()
                        return questscroll5.get_mission()[9]
                end
                function get_mob2_vnum()
                        return tonumber(questscroll5.get_mission()[8])
                        -- return mob_vnum(questscroll5.get_mob2_name())
                end
                function get_mob2_rest_kill_count()
                        local ret=questscroll5.get_mob2_max_kill_count()-questscroll5.get_mob2_cur_kill_count()
                        if ret<0 then return 0 end
                        return ret
                end
                function set_mob2_cur_kill_count(value)
                        pc.setqf("mob2_kill_count", value)
                end
                function get_mob2_cur_kill_count()
                        return pc.getqf("mob2_kill_count")
                end

                function reward_table_get_prob_sum(item_array)
                        local sum=0
                        for i=1, table.getn(item_array) do
                                sum=sum+item_array[i][2]
                        end
                        return sum
                end
                function reward_table_get_value(item_array)
                        local prob_sum=questscroll5.reward_table_get_prob_sum(item_array)
                        local prob_pos=number(0, prob_sum)

                        test_chat("select_reward_item.prob_pos="..prob_pos)
                        for i=1, table.getn(item_array) do
                                prob_cur=item_array[i][2]
                                if prob_pos<=prob_cur then
                                        return item_array[i][1]
                                end
                                prob_pos=prob_pos-prob_cur
                        end

                        chat("select_reward_item.overflow")
                        return 0
                end
                function get_reward_type()
                        return questscroll5.get_mission()[2]
                end
                function get_reward_money(reward_type)
                        return questscroll5.reward_table_get_value(locale.questscroll5.reward_money[reward_type])
                end
                function get_reward_exp(reward_type)
                        return questscroll5.reward_table_get_value(locale.questscroll5.reward_exp[reward_type])
                end
                function get_reward_item(reward_type)
                        return questscroll5.reward_table_get_value(locale.questscroll5.reward_item[reward_type])
                end
                function reward_money(reward_type)
                        local reward_money=questscroll5.get_reward_money(reward_type)
                        if reward_money>1000 and reward_money<=2000000 then
                                pc.change_money(reward_money)
                                say_reward(string.format("Yang: %d ", reward_money))
                        else
                                chat("questscroll5.reward_MONEY.error type="..reward_type)
                        end
                end
                function reward_exp(reward_type)
                        local reward_exp=questscroll5.get_reward_exp(reward_type)
                        if reward_exp>1000 and reward_exp<=3000000 then
                                pc.give_exp2(reward_exp)
                                say_reward(string.format("Tecrübe puaný: %d", reward_exp))
                        else
                                chat("questscroll5.reward_EXP.error type="..reward_type)
                        end
                end
                function reward_item(reward_type)
                        local reward_item=questscroll5.get_reward_item(reward_type)
                        pc.give_item2(reward_item)
                        say_reward(string.format("Eþya: %s", item_name(reward_item)))
                end
        end
end