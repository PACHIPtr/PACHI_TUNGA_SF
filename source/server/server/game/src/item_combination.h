#ifndef __INC_ITEM_COMBINATION_H__
#define __INC_ITEM_COMBINATION_H__

#define COMB_MAX_NUM		10
#define COMB_MAX_DISTANCE	1000

void Comb_open_costume(LPCHARACTER ch);
void Comb_open_skillbook(LPCHARACTER ch);
void Comb_add_item(LPCHARACTER ch, int combination_index, int inven_index);
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
void Comb_add_item2(LPCHARACTER ch, int combination_index, int inven_index, int inven_type);
#else
void Comb_add_item2(LPCHARACTER ch, int combination_index, int inven_index);
#endif
void Comb_del_item(LPCHARACTER ch, int combination_index);
bool Comb_make_cotume(LPCHARACTER ch);
bool Comb_make_skillbook(LPCHARACTER ch);
void Comb_close(LPCHARACTER ch);
void Comb_clean_item(LPCHARACTER ch);
void Comb_init();

#endif
