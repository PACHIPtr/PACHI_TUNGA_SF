#include "stdafx.h"
#include "constants.h"
#include "utils.h"
#include "config.h"
#include "char.h"
#include "char_manager.h"
#include "db.h"
#include "packet.h"
#include "desc_client.h"
#include "item.h"
#include "item_manager.h"
#include "locale_service.h"

void CHARACTER::OpenMailBox()
{
	if (IsOpenSafebox() || GetShop() || IsCubeOpen() || IsDead() || GetExchange() || GetMyShop() || IsCombOpen() || isSashOpened(true) || isSashOpened(false) || isChangeLookOpened() || isAuraOpened(true) || isAuraOpened(false))
		return;

	SetOpenMailBox(true);
	LoadingMails();
	//StartCheckTimeMailBox();
	LoadMails();
	SendMailBoxPackets(MAILBOX_SUB_HEADER_OPEN);
}

void CHARACTER::LOAD_CONFIG_MAILS()
{
	FILE* file;
	char buf[256], token_string[256], value_string[256], file_name[256];

	snprintf(file_name, sizeof(file_name), "%s/mailbox_config.txt", LocaleService_GetBasePath().c_str());
	file = fopen(file_name, "r");
	while (fgets(buf, sizeof(buf), file))
	{
		parse_token(buf, token_string, value_string);

		TOKEN("MAILBOX_TASA_SEND_YANG_PORCIENTO")
		{
			str_to_number(tasa_porciento, value_string);
		}

		TOKEN("MAILBOX_YANG_SEND_MAIL")
		{
			str_to_number(gold_envio, value_string);
		}

		TOKEN("MAILBOX_COUNT_MAILS")
		{
			str_to_number(limite_mensajes, value_string);
		}

		TOKEN("MAILBOX_SEND_MAIL_TO_ME")
		{
			str_to_number(send_mail_to_me, value_string);
		}
	}

	fclose(file);
}

void CHARACTER::LoadingMails()
{
	char szQuery[1024];

#ifdef USE_WON_SYSTEM_MAILBOX
	snprintf(szQuery, sizeof(szQuery),
		"SELECT id_mail, name_send, titulo_mail, description_mail, item_vnum, item_count, gold, won, check_acept, id_item, UNIX_TIMESTAMP(time_mail), check_view  FROM %smailbox WHERE id_receive_mail = %d", get_table_postfix(), GetPlayerID());
#else
	snprintf(szQuery, sizeof(szQuery),
		"SELECT id_mail, name_send, titulo_mail, description_mail, item_vnum, item_count, gold, check_acept, id_item, UNIX_TIMESTAMP(time_mail), check_view  FROM %smailbox WHERE id_receive_mail = %d", get_table_postfix(), GetPlayerID());
#endif
	std::unique_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery(szQuery));
	if (pMsg->Get()->uiNumRows > 0) {
		DWORD id_mail;
		int item_vnum;
		int item_count;
		int gold;
#ifdef USE_WON_SYSTEM_MAILBOX
		int won;
#endif
		int check_acept;
		int id_item;
		DWORD time;
		int check;

		info_mailbox_values.clear();

		for (int i = 0; i < mysql_num_rows(pMsg->Get()->pSQLResult); ++i)
		{
			MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
			int col = 0;

			str_to_number(id_mail, row[col++]);
			char* name_send = row[col++];
			char* titulo_mail = row[col++];
			char* description_mail = row[col++];
			str_to_number(item_vnum, row[col++]);
			str_to_number(item_count, row[col++]);
			str_to_number(gold, row[col++]);
#ifdef USE_WON_SYSTEM_MAILBOX
			str_to_number(won, row[col++]);
#endif
			str_to_number(check_acept, row[col++]);
			str_to_number(id_item, row[col++]);
			str_to_number(time, row[col++]);
			str_to_number(check, row[col++]);

			// Save Info Mails
			save_mail.id_mail = id_mail;
			save_mail.id_receive_mail = GetPlayerID();
			memcpy(save_mail.nombre_pj, name_send, sizeof(save_mail.nombre_pj));
			memcpy(save_mail.asunto, titulo_mail, sizeof(save_mail.asunto));
			memcpy(save_mail.descrip, description_mail, sizeof(save_mail.descrip));
			save_mail.item_vnum = item_vnum;
			save_mail.item_count = item_count;
			save_mail.gold = gold;
#ifdef USE_WON_SYSTEM_MAILBOX
			save_mail.won = won;
#endif
			save_mail.check_acept = check_acept;
			save_mail.id_item = id_item;
			save_mail.time = time;
			save_mail.check = check;
			memcpy(&copy_mail, &save_mail, sizeof(save_mail));
			info_mailbox_values.push_back(copy_mail);
			// Save Info Mails
		}
	}
}

void CHARACTER::LoadMails()
{
	if (info_mailbox_values.size() > 0) {
		for (int i = 0; i < info_mailbox_values.size(); ++i) {
#ifdef USE_WON_SYSTEM_MAILBOX
			SendMailBoxPackets(MAILBOX_SUB_HEADER_RECEIVE_INFO,
				info_mailbox_values[i].id_mail,
				info_mailbox_values[i].nombre_pj,
				info_mailbox_values[i].asunto,
				info_mailbox_values[i].descrip,
				info_mailbox_values[i].item_vnum,
				info_mailbox_values[i].item_count,
				info_mailbox_values[i].gold,
				info_mailbox_values[i].won,
				info_mailbox_values[i].check_acept,
				info_mailbox_values[i].id_item,
				info_mailbox_values[i].time,
				info_mailbox_values[i].check
			);
#else
			SendMailBoxPackets(MAILBOX_SUB_HEADER_RECEIVE_INFO,
				info_mailbox_values[i].id_mail,
				info_mailbox_values[i].nombre_pj,
				info_mailbox_values[i].asunto,
				info_mailbox_values[i].descrip,
				info_mailbox_values[i].item_vnum,
				info_mailbox_values[i].item_count,
				info_mailbox_values[i].gold,
				info_mailbox_values[i].check_acept,
				info_mailbox_values[i].id_item,
				info_mailbox_values[i].time,
				info_mailbox_values[i].check
			);
#endif
		}
	}
}

void CHARACTER::CheckViewMail(int id_mail)
{
	for (int i = 0; i < info_mailbox_values.size(); ++i) {
		if (info_mailbox_values[i].id_mail == id_mail && info_mailbox_values[i].check == 1) {
			DBManager::instance().DirectQuery("UPDATE %smailbox SET check_view = 0 WHERE id_receive_mail = %u and id_mail = %d", get_table_postfix(), info_mailbox_values[i].id_receive_mail, info_mailbox_values[i].id_mail);
			info_mailbox_values[i].check = 0;
			SendMInfoMap();
			SendMailBoxPackets(MAILBOX_SUB_HEADER_CHECK_VIEW_SUCCESSFUL, info_mailbox_values[i].id_mail);
		}
	}
}

void CHARACTER::SendMInfoMap() {
	int c_a = 0, m_a = 0, m_s = 0;

	if (info_mailbox_values.size() > 0) {
		for (int i = 0; i < info_mailbox_values.size(); ++i) {
			if (info_mailbox_values[i].check == 1) {
				++c_a;
			}
			if (info_mailbox_values[i].check == 1 && info_mailbox_values[i].check_acept == 1) {
				++m_a;
			}
			if (info_mailbox_values[i].check == 1 && info_mailbox_values[i].check_acept == 0) {
				++m_s;
			}
		}
		ChatPacket(CHAT_TYPE_COMMAND, "SERVER_MAILBOX_MAP %d %d %d", c_a, m_a, m_s);
	}
	else {
		ChatPacket(CHAT_TYPE_COMMAND, "SERVER_MAILBOX_MAP %d %d %d", 0, 0, 0);
	}
}

#ifdef USE_WON_SYSTEM_MAILBOX
void CHARACTER::SendMailBox(const char* name, const char* titulo, const char* descrip, int item_vnum, int gold, int won)
#else
void CHARACTER::SendMailBox(const char* name, const char* titulo, const char* descrip, int item_vnum, int gold)
#endif
{
	char szQuery[QUERY_MAX_LEN];

	char szEscapedName[1024];
	DBManager::instance().EscapeString(szEscapedName, sizeof(szEscapedName), name, strlen(name));

	if (FindPlayerName(szEscapedName) == false) { return; }

	char szName[CHARACTER_NAME_MAX_LEN + 1];
	str_lower(name, szName, sizeof(szName));

	if (send_mail_to_me == 1) { //Comprueba si se esta enviando un mensaje a usted mismo
		if (strncasecmp(szName, GetName(), CHARACTER_NAME_MAX_LEN) == 0)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[MailBox]No puede enviarse mensajes a usted mismo."));
			return;
		}
	}

	if (GetGold() < gold_envio + gold) {
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[MailBox]No posee yang para enviar el mensaje"));
		return;
	}

#ifdef USE_WON_SYSTEM_MAILBOX
	if (GetCheque() < won) {
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("No tienes suficiente Won para enviar el mensaje."));
		return;
	}
#endif

	if (GetCountMails(GetIdPlayerReceive(name)) >= limite_mensajes) //Limita el envio a un pj si posee mas de 20 mensajes en el buzon
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[MailBox]No puede enviar el mensaje a %s porque tiene lleno el buzon"), name);
		return;
	}

	//Comprueba si se envia un item o no
	DWORD item_vnum_send = 0;
	DWORD item_id_send = 0;

	LPITEM item = GetItem(TItemPos(INVENTORY, item_vnum));
	if (nullptr != item) {
		item_vnum_send = item->GetVnum();
		item_id_send = item->GetID();
	}
	//Comprueba si es un item o no y coloca el valot de check
	int check_acept = 0;

	if (item_vnum_send != 0 || gold != 0) {
		check_acept = 1;
	}
	else {
		check_acept = 0;
	}

	char descripcion[100 * 2 + 1];
	char asunto[25 * 2 + 1];
	DBManager::instance().EscapeString(descripcion, sizeof(descripcion), descrip, strlen(descrip));
	DBManager::instance().EscapeString(asunto, sizeof(asunto), titulo, strlen(titulo));

	//ChatPacket(CHAT_TYPE_INFO, "%s %s %s",asunto,descripcion,titulo);

	//86400*30 //30 dias
	DWORD time = get_global_time() + (60 * 2); //3 minutos

#ifdef USE_WON_SYSTEM_MAILBOX
	DBManager::instance().DirectQuery("INSERT INTO player.mailbox(id_receive_mail, name_send, titulo_mail, description_mail, item_vnum, item_count, id_item, gold, won, check_acept, time_mail) VALUES(%d,'%s','%s','%s',%d,%d,%d,%d,%d,%d,FROM_UNIXTIME(%d))",
		GetIdPlayerReceive(szEscapedName),
		GetName(),
		asunto,
		descripcion,
		item_vnum_send,
		0,
		item_id_send,
		gold,
		won,
		check_acept,
		time);
#else
	DBManager::instance().DirectQuery("INSERT INTO player.mailbox(id_receive_mail, name_send, titulo_mail, description_mail, item_vnum, item_count, id_item, gold, check_acept, time_mail) VALUES(%d,'%s','%s','%s',%d,%d,%d,%d,%d,FROM_UNIXTIME(%d))",
		GetIdPlayerReceive(szEscapedName),
		GetName(),
		asunto,
		descripcion,
		item_vnum_send,
		0,
		item_id_send,
		gold,
		check_acept,
		time);
#endif
	//DBManager::instance().AsyncQuery(szQuery);

	if (item_vnum_send != 0) { InsertItemMailBox(item); }

	if (gold > 0) {
		PointChange(POINT_GOLD, -gold_envio);
		PointChange(POINT_GOLD, -gold);
	}

#ifdef USE_WON_SYSTEM_MAILBOX
	if (won > 0) {
		PointChange(POINT_CHEQUE, -won);
	}
#endif

	ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[MailBox] Mensaje enviado a %s."), name);
	SendMailBoxPackets(MAILBOX_SUB_HEADER_SEND_MAIL_SUCCESSFUL);
}

void CHARACTER::CheckNameMailBox(const char* name)
{
	if (FindPlayerName(name) == true)
	{
		SendMailBoxPackets(MAILBOX_SUB_HEADER_CHECK_NAME_SUCCESSFUL);
	}
}

void CHARACTER::AcceptMail(int id_mail)
{
	if (FindIDMail(id_mail) == false) { return; }
	if (get_check_acept_mailbox(id_mail) == 0) { return; }

	bool item_check = false;

	if (get_item_vnum_mailbox(id_mail) != 0) //Si el mensaje tiene un item lo recibe
	{
		LPITEM item = ITEM_MANAGER::instance().CreateItem(get_item_vnum_mailbox(id_mail));

		int iEmptyPos = item->IsDragonSoul() ? GetEmptyDragonSoulInventory(item) : GetEmptyInventory(item->GetSize());
		if (iEmptyPos < 0)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[MailBox]Usted no cuenta con espacio suficiente en su inventario."));
			return;
		}

		item_check = true;
	}

	int gold_check = get_gold_mailbox(id_mail);

#ifdef USE_WON_SYSTEM_MAILBOX
	int won_check = get_won_mailbox(id_mail);
#endif

#ifdef USE_WON_SYSTEM_MAILBOX
	if (gold_check > 0 || won_check > 0) {
		const int64_t TotalMoney = static_cast<int64_t>(GetGold()) + static_cast<int64_t>(gold_check);
		const int64_t TotalCheque = static_cast<int64_t>(GetCheque()) + static_cast<int64_t>(won_check);

		if (TotalMoney >= GOLD_MAX) {
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("No se ha podido aceptar el Yang. El limite de Yang en el inventario es de: %d."), GOLD_MAX);
			return;
		}

		if (TotalCheque >= CHEQUE_MAX) {
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("No se ha podido aceptar el won. El limite de won en el inventario es de: %d."), CHEQUE_MAX);
			return;
		}

		if (gold_check > 0) {
			int tasa_descuento = gold_check - (tasa_porciento * gold_check / 100);
			PointChange(POINT_GOLD, tasa_descuento);
		}

		if (won_check > 0) {
			PointChange(POINT_CHEQUE, won_check);
		}
	}
#else
	if (gold_check > 0) {
		const int64_t TotalMoney = static_cast<int64_t>(GetGold()) + static_cast<int64_t>(gold_check);

		if (TotalMoney >= GOLD_MAX) {
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("No se ha podido aceptar el Yang. El limite de Yang en el inventario es de: %d."), GOLD_MAX);
			return;
		}

		if (gold_check > 0) {
			int tasa_descuento = gold_check - (tasa_porciento * gold_check / 100);
			PointChange(POINT_GOLD, tasa_descuento);
		}
	}

#endif

	if (item_check == true) { ReceiveItem(get_id_item_mailbox(id_mail)); }
	DBManager::instance().DirectQuery("UPDATE %smailbox SET check_acept = 0 WHERE id_receive_mail = %u and id_mail = %d", get_table_postfix(), GetPlayerID(), id_mail);

	for (int i = 0; i < info_mailbox_values.size(); ++i) {
		if (info_mailbox_values[i].check_acept == 1 && info_mailbox_values[i].id_receive_mail == GetPlayerID() && info_mailbox_values[i].id_mail == id_mail) {
			info_mailbox_values[i].check_acept = 0;
		}
	}

	SendMailBoxPackets(MAILBOX_SUB_HEADER_CHECK_ACCEPT_SUCCESSFUL, id_mail);
	SendMailBoxPackets(MAILBOX_SUB_HEADER_LOADING_ACCEPT);
}

void CHARACTER::AcceptAllMails()
{
	if (FindIDReceiveMail() == false) { return; }

	LPITEM item;
	int iEmptyPos;

	for (int i = 0; i < info_mailbox_values.size(); ++i) {
		if (info_mailbox_values[i].check_acept == 1 && info_mailbox_values[i].id_receive_mail == GetPlayerID()) {
			bool item_check = false;

			if (get_item_vnum_mailbox(info_mailbox_values[i].id_mail) != 0) //Si el mensaje tiene un item lo recibe
			{
				item = ITEM_MANAGER::instance().CreateItem(get_item_vnum_mailbox(info_mailbox_values[i].id_mail));

				iEmptyPos = item->IsDragonSoul() ? GetEmptyDragonSoulInventory(item) : GetEmptyInventory(item->GetSize());
				if (iEmptyPos < 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[MailBox]Usted no cuenta con espacio suficiente en su inventario."));
					SendMailBoxPackets(MAILBOX_SUB_HEADER_LOADING_ACCEPT);
					return;
				}
				item_check = true;
			}

			int gold_check = get_gold_mailbox(info_mailbox_values[i].id_mail);

#ifdef USE_WON_SYSTEM_MAILBOX
			int won_check = get_won_mailbox(info_mailbox_values[i].id_mail);
			if (gold_check > 0 || won_check > 0) {
				const int64_t TotalMoney = static_cast<int64_t>(GetGold()) + static_cast<int64_t>(gold_check);
				const int64_t TotalCheque = static_cast<int64_t>(GetCheque()) + static_cast<int64_t>(won_check);

				if (TotalMoney >= GOLD_MAX) {
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("No se ha podido aceptar el Yang. El limite de Yang en el inventario es de: %d."), GOLD_MAX);
					SendMailBoxPackets(MAILBOX_SUB_HEADER_LOADING_ACCEPT);
					return;
				}

				if (TotalCheque >= CHEQUE_MAX) {
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("No se ha podido aceptar el won. El limite de won en el inventario es de: %d."), CHEQUE_MAX);
					SendMailBoxPackets(MAILBOX_SUB_HEADER_LOADING_ACCEPT);
					return;
				}

				if (gold_check > 0) {
					int tasa_descuento = gold_check - (tasa_porciento * gold_check / 100);
					PointChange(POINT_GOLD, tasa_descuento);
				}

				if (won_check > 0) {
					PointChange(POINT_CHEQUE, won_check);
				}
			}
#else
			if (gold_check > 0) {
				const int64_t TotalMoney = static_cast<int64_t>(GetGold()) + static_cast<int64_t>(gold_check);

				if (TotalMoney >= GOLD_MAX) {
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("No se ha podido aceptar el Yang. El limite de Yang en el inventario es de: %d."), GOLD_MAX);
					SendMailBoxPackets(MAILBOX_SUB_HEADER_LOADING_ACCEPT);
					return;
				}

				if (gold_check > 0) {
					int tasa_descuento = gold_check - (tasa_porciento * gold_check / 100);
					PointChange(POINT_GOLD, tasa_descuento);
				}
			}

#endif

			if (item_check == true) { ReceiveItem(info_mailbox_values[i].id_item); }
			DBManager::instance().DirectQuery("UPDATE %smailbox SET check_acept = 0 WHERE id_receive_mail = %u and id_mail = %d", get_table_postfix(), GetPlayerID(), info_mailbox_values[i].id_mail);
			info_mailbox_values[i].check_acept = 0;
			SendMailBoxPackets(MAILBOX_SUB_HEADER_CHECK_ACCEPT_SUCCESSFUL, info_mailbox_values[i].id_mail);
		}
	}
	SendMailBoxPackets(MAILBOX_SUB_HEADER_LOADING_ACCEPT);
}

void CHARACTER::ReceiveItem(int id_item)
{
	char szQuery1[1024];

	snprintf(szQuery1, sizeof(szQuery1),
		"SELECT vnum, count,"
#ifdef USE_TRANSMUTATION_SYSTEM_MAILBOX
		" transmutation,"
#endif
		" socket0, socket1, socket2, socket3, attrtype0, attrvalue0, "
		"attrtype1, attrvalue1, "
		"attrtype2, attrvalue2, "
		"attrtype3, attrvalue3, "
		"attrtype4, attrvalue4, "
		"attrtype5, attrvalue5, "
		"attrtype6, attrvalue6 "
#ifdef USE_ACCE_SYSTEM_MAILBOX
		",applytype0, applyvalue0, "
		"applytype1, applyvalue1, "
		"applytype2, applyvalue2, "
		"applytype3, applyvalue3, "
		"applytype4, applyvalue4, "
		"applytype5, applyvalue5, "
		"applytype6, applyvalue6, "
		"applytype7, applyvalue7 "
#endif
		"FROM player.mailbox_items WHERE id_item = %u", id_item);
	std::unique_ptr<SQLMsg> pMsg1(DBManager::Instance().DirectQuery(szQuery1));

	TPlayerItem item;

	for (int i2 = 0; i2 < mysql_num_rows(pMsg1->Get()->pSQLResult); ++i2)
	{
		MYSQL_ROW row1 = mysql_fetch_row(pMsg1->Get()->pSQLResult);
		int w = 0;

		str_to_number(item.vnum, row1[w++]);
		str_to_number(item.count, row1[w++]);

#ifdef USE_TRANSMUTATION_SYSTEM_MAILBOX
		str_to_number(item.transmutation, row1[w++]);
#endif
		for (BYTE j = 0; j < ITEM_SOCKET_MAX_NUM; j++) {
			str_to_number(item.alSockets[j], row1[w++]);
		}

		for (BYTE n = 0; n < ITEM_ATTRIBUTE_MAX_NUM; n++)
		{
			str_to_number(item.aAttr[n].bType, row1[w++]);
			str_to_number(item.aAttr[n].sValue, row1[w++]);
		}
	}

	LPITEM pItem = ITEM_MANAGER::Instance().CreateItem(item.vnum, item.count);
	if (!pItem) return;

	pItem->SetAttributes(item.aAttr);
	pItem->SetSockets(item.alSockets);

#ifdef USE_TRANSMUTATION_SYSTEM_MAILBOX
	pItem->SetTransmutation(item.transmutation);
#endif

	int iEmptyPos = pItem->IsDragonSoul() ? GetEmptyDragonSoulInventory(pItem) : GetEmptyInventory(pItem->GetSize());

	pItem->AddToCharacter(this, TItemPos(pItem->IsDragonSoul() ? DRAGON_SOUL_INVENTORY : INVENTORY, iEmptyPos));
	DBManager::instance().DirectQuery("UPDATE %smailbox SET id_item = 0, item_vnum = 0, item_count = 0 WHERE id_item = %u", get_table_postfix(), id_item);

	for (int i = 0; i < info_mailbox_values.size(); ++i) {
		if (info_mailbox_values[i].id_item == id_item) {
			info_mailbox_values[i].item_vnum = 0;
			info_mailbox_values[i].id_item = 0;
			info_mailbox_values[i].item_count = 0;
		}
	}

	DBManager::Instance().DirectQuery("DELETE FROM %smailbox_items WHERE id_item = %u", get_table_postfix(), id_item);
}

void CHARACTER::DeleteMail(int id_mail)
{
	if (FindIDMail(id_mail) == false) { return; }
	if (get_check_acept_mailbox(id_mail) == 1) { return; }

	DBManager::instance().Query("DELETE FROM %smailbox WHERE id_mail=%d and id_receive_mail = %u", get_table_postfix(), id_mail, GetPlayerID());

	for (int i = 0; i < info_mailbox_values.size(); ++i) {
		if (info_mailbox_values[i].id_receive_mail == GetPlayerID() && info_mailbox_values[i].id_mail == id_mail) {
			info_mailbox_values.erase(info_mailbox_values.begin() + i);
		}
	}

	SendMailBoxPackets(MAILBOX_SUB_HEADER_CHECK_DELETE_SUCCESSFUL, id_mail);
	SendMailBoxPackets(MAILBOX_SUB_HEADER_LOADING_DELETE);
}

void CHARACTER::DeleteAllMails()
{
	if (FindIDReceiveMail() == false) { return; }

	for (int i = 0; i < info_mailbox_values.size(); ++i) {
		if (info_mailbox_values[i].check_acept == 0 && info_mailbox_values[i].id_receive_mail == GetPlayerID()) {
			DBManager::instance().Query("DELETE FROM %smailbox WHERE id_mail=%d", get_table_postfix(), info_mailbox_values[i].id_mail);
			SendMailBoxPackets(MAILBOX_SUB_HEADER_CHECK_DELETE_SUCCESSFUL, info_mailbox_values[i].id_mail);
		}
	}

	ClearLIST(1);
	SendMailBoxPackets(MAILBOX_SUB_HEADER_LOADING_DELETE);
}

void CHARACTER::ClearLIST(int func)
{
	//Las variables que no cumplen los requisitos se guardan en nueva lista.
	//y no se agregan las que no se cumplen , eliminando de la lista.

	for (int i = 0; i < info_mailbox_values.size(); ++i) {
		//FuncDeleteallMails
		if (func == 1) {
			if (info_mailbox_values[i].check_acept != 0 && info_mailbox_values[i].id_receive_mail == GetPlayerID()) {
				save_mail.id_mail = info_mailbox_values[i].id_mail;
				save_mail.id_receive_mail = info_mailbox_values[i].id_receive_mail;
				memcpy(save_mail.nombre_pj, info_mailbox_values[i].nombre_pj, sizeof(save_mail.nombre_pj));
				memcpy(save_mail.asunto, info_mailbox_values[i].asunto, sizeof(save_mail.asunto));
				memcpy(save_mail.descrip, info_mailbox_values[i].descrip, sizeof(save_mail.descrip));
				save_mail.item_vnum = info_mailbox_values[i].item_vnum;
				save_mail.item_count = info_mailbox_values[i].item_count;
				save_mail.gold = info_mailbox_values[i].gold;
#ifdef USE_WON_SYSTEM_MAILBOX
				save_mail.won = info_mailbox_values[i].won;
#endif
				save_mail.check_acept = info_mailbox_values[i].check_acept;
				save_mail.id_item = info_mailbox_values[i].id_item;
				save_mail.time = info_mailbox_values[i].time;
				save_mail.check = info_mailbox_values[i].check;
				memcpy(&copy_mail, &save_mail, sizeof(save_mail));

				info_mailbox_values_extra.push_back(copy_mail);
			}
		}
		//FuncDeleteMailsTime
		else {
			if ((info_mailbox_values[i].time - get_global_time()) > 0 && info_mailbox_values[i].id_receive_mail == GetPlayerID()) {
				save_mail.id_mail = info_mailbox_values[i].id_mail;
				save_mail.id_receive_mail = info_mailbox_values[i].id_receive_mail;
				memcpy(save_mail.nombre_pj, info_mailbox_values[i].nombre_pj, sizeof(save_mail.nombre_pj));
				memcpy(save_mail.asunto, info_mailbox_values[i].asunto, sizeof(save_mail.asunto));
				memcpy(save_mail.descrip, info_mailbox_values[i].descrip, sizeof(save_mail.descrip));
				save_mail.item_vnum = info_mailbox_values[i].item_vnum;
				save_mail.item_count = info_mailbox_values[i].item_count;
				save_mail.gold = info_mailbox_values[i].gold;
#ifdef USE_WON_SYSTEM_MAILBOX
				save_mail.won = info_mailbox_values[i].won;
#endif
				save_mail.check_acept = info_mailbox_values[i].check_acept;
				save_mail.id_item = info_mailbox_values[i].id_item;
				save_mail.time = info_mailbox_values[i].time;
				save_mail.check = info_mailbox_values[i].check;
				memcpy(&copy_mail, &save_mail, sizeof(save_mail));

				info_mailbox_values_extra.push_back(copy_mail);
			}
		}
	}

	info_mailbox_values.clear();

	if (info_mailbox_values_extra.size() > 0)
	{
		for (int i = 0; i < info_mailbox_values_extra.size(); ++i) {
			save_mail.id_mail = info_mailbox_values_extra[i].id_mail;
			save_mail.id_receive_mail = info_mailbox_values_extra[i].id_receive_mail;
			memcpy(save_mail.nombre_pj, info_mailbox_values_extra[i].nombre_pj, sizeof(save_mail.nombre_pj));
			memcpy(save_mail.asunto, info_mailbox_values_extra[i].asunto, sizeof(save_mail.asunto));
			memcpy(save_mail.descrip, info_mailbox_values_extra[i].descrip, sizeof(save_mail.descrip));
			save_mail.item_vnum = info_mailbox_values_extra[i].item_vnum;
			save_mail.item_count = info_mailbox_values_extra[i].item_count;
			save_mail.gold = info_mailbox_values_extra[i].gold;
#ifdef USE_WON_SYSTEM_MAILBOX
			save_mail.won = info_mailbox_values_extra[i].won;
#endif
			save_mail.check_acept = info_mailbox_values_extra[i].check_acept;
			save_mail.id_item = info_mailbox_values_extra[i].id_item;
			save_mail.time = info_mailbox_values_extra[i].time;
			save_mail.check = info_mailbox_values_extra[i].check;
			memcpy(&copy_mail, &save_mail, sizeof(save_mail));

			info_mailbox_values.push_back(copy_mail);
		}
	}

	info_mailbox_values_extra.clear();
}
#ifdef USE_WON_SYSTEM_MAILBOX
void CHARACTER::SendMailBoxPackets(BYTE subheader, int id_mail, char nombre_pj[12], char asunto[25], char descrip[100], int item_vnum, int item_count, int gold, int won, int check_acept, int id_item, DWORD time, int check_view)
#else
void CHARACTER::SendMailBoxPackets(BYTE subheader, int id_mail, char nombre_pj[12], char asunto[25], char descrip[100], int item_vnum, int item_count, int gold, int check_acept, int id_item, DWORD time, int check_view)
#endif
{
	TPacketGCMailBoxReceive pack;
	pack.bSubType = subheader;

	if (subheader == MAILBOX_SUB_HEADER_RECEIVE_INFO)
	{
		pack.id_mail = id_mail;
		memcpy(pack.nombre_pj, nombre_pj, sizeof(pack.nombre_pj));
		memcpy(pack.asunto, asunto, sizeof(pack.asunto));
		memcpy(pack.descrip, descrip, sizeof(pack.descrip));

		pack.item_vnum = 0;
		pack.item_count = 0;
		pack.gold = gold;
#ifdef USE_WON_SYSTEM_MAILBOX
		pack.won = won;
#endif
		pack.check_acept = check_acept;
		pack.time = time;
		pack.check_view = check_view;

		TPlayerItem item;
		int item_vnum_w = 0;
		int item_count_w = 0;

#ifdef USE_TRANSMUTATION_SYSTEM_MAILBOX
		int item_transmutation_w = 0;
#endif
		if (id_item != 0)
		{
			char szQuery1[1024];
			snprintf(szQuery1, sizeof(szQuery1),
				"SELECT vnum, count,"
#ifdef USE_TRANSMUTATION_SYSTEM_MAILBOX
				" transmutation,"
#endif
				" socket0, socket1, socket2, socket3, attrtype0, attrvalue0, "
				"attrtype1, attrvalue1, "
				"attrtype2, attrvalue2, "
				"attrtype3, attrvalue3, "
				"attrtype4, attrvalue4, "
				"attrtype5, attrvalue5, "
				"attrtype6, attrvalue6 "
#ifdef USE_ACCE_SYSTEM_MAILBOX
				",applytype0, applyvalue0, "
				"applytype1, applyvalue1, "
				"applytype2, applyvalue2, "
				"applytype3, applyvalue3, "
				"applytype4, applyvalue4, "
				"applytype5, applyvalue5, "
				"applytype6, applyvalue6, "
				"applytype7, applyvalue7 "
#endif
				"FROM player.mailbox_items WHERE id_item = %u", id_item);
			std::unique_ptr<SQLMsg> pMsg1(DBManager::Instance().DirectQuery(szQuery1));
			for (int i2 = 0; i2 < mysql_num_rows(pMsg1->Get()->pSQLResult); ++i2)
			{
				MYSQL_ROW row1 = mysql_fetch_row(pMsg1->Get()->pSQLResult);
				int w = 0;

				str_to_number(item_vnum_w, row1[w++]);
				str_to_number(item_count_w, row1[w++]);
#ifdef USE_TRANSMUTATION_SYSTEM_MAILBOX
				str_to_number(item_transmutation_w, row1[w++]);
#endif

				pack.item_vnum = item_vnum_w;
				pack.item_count = item_count_w;

#ifdef USE_TRANSMUTATION_SYSTEM_MAILBOX
				pack.transmutation = item_transmutation_w;
#endif
				for (BYTE j = 0; j < ITEM_SOCKET_MAX_NUM; j++) {
					str_to_number(item.alSockets[j], row1[w++]);
					thecore_memcpy(pack.alSockets, item.alSockets, sizeof(pack.alSockets));
				}

				for (BYTE n = 0; n < ITEM_ATTRIBUTE_MAX_NUM; n++)
				{
					str_to_number(item.aAttr[n].bType, row1[w++]);
					str_to_number(item.aAttr[n].sValue, row1[w++]);

					thecore_memcpy(pack.aAttr, item.aAttr, sizeof(pack.aAttr));
				}
			}
		}
	}

	if (subheader == MAILBOX_SUB_HEADER_CHECK_ACCEPT_SUCCESSFUL || subheader == MAILBOX_SUB_HEADER_CHECK_DELETE_SUCCESSFUL || subheader == MAILBOX_SUB_HEADER_CHECK_VIEW_SUCCESSFUL)
	{
		pack.id_mail = id_mail;
	}

	LPDESC d = GetDesc();

	if (nullptr == d)
	{
		sys_err("User CHARACTER::SendMailBoxPackets (%s)'s DESC is nullptr POINT.", GetName());
		return;
	}

	d->Packet(&pack, sizeof(pack));
}

void CHARACTER::InsertItemMailBox(LPITEM item)
{
	if (nullptr == item) {
		ChatPacket(CHAT_TYPE_INFO, "[MailBox]Error 000");
		return;
	}

	char szColumns[QUERY_MAX_LEN], szValues[QUERY_MAX_LEN];
#ifdef USE_TRANSMUTATION_SYSTEM_MAILBOX
	int iLen = snprintf(szColumns, sizeof(szColumns), "id_item, id_receive_mail, vnum, count , transmutation");
	int iUpdateLen = snprintf(szValues, sizeof(szValues), "%d, %u, %d, %d, %d", item->GetID(), GetPlayerID(), item->GetVnum(), item->GetCount(), item->GetTransmutation());
#else
	int iLen = snprintf(szColumns, sizeof(szColumns), "id_item, id_receive_mail, vnum, count");
	int iUpdateLen = snprintf(szValues, sizeof(szValues), "%d, %u, %d, %d", item->GetID(), GetPlayerID(), item->GetVnum(), item->GetCount());
#endif

	// Sockets
	iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ", socket0, socket1, socket2");
	iUpdateLen += snprintf(szValues + iUpdateLen, sizeof(szValues) - iUpdateLen, ", %ld, %ld, %ld", item->GetSocket(0), item->GetSocket(1), item->GetSocket(2));

	// Attributes
#ifdef USE_ACCE_SYSTEM_MAILBOX
	iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ", attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7");
	iUpdateLen += snprintf(szValues + iUpdateLen, sizeof(szValues) - iUpdateLen, ", %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		item->GetAttributeType(0), item->GetAttributeValue(0),
		item->GetAttributeType(1), item->GetAttributeValue(1),
		item->GetAttributeType(2), item->GetAttributeValue(2),
		item->GetAttributeType(3), item->GetAttributeValue(3),
		item->GetAttributeType(4), item->GetAttributeValue(4),
		item->GetAttributeType(5), item->GetAttributeValue(5),
		item->GetAttributeType(6), item->GetAttributeValue(6),
		item->GetAttributeType(7), item->GetAttributeValue(7),
		item->GetAttributeType(8), item->GetAttributeValue(8),
		item->GetAttributeType(9), item->GetAttributeValue(9),
		item->GetAttributeType(10), item->GetAttributeValue(10),
		item->GetAttributeType(11), item->GetAttributeValue(11),
		item->GetAttributeType(12), item->GetAttributeValue(12),
		item->GetAttributeType(13), item->GetAttributeValue(13),
		item->GetAttributeType(14), item->GetAttributeValue(14));
#else
	iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ", attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6");
	iUpdateLen += snprintf(szValues + iUpdateLen, sizeof(szValues) - iUpdateLen, ", %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		item->GetAttributeType(0), item->GetAttributeValue(0),
		item->GetAttributeType(1), item->GetAttributeValue(1),
		item->GetAttributeType(2), item->GetAttributeValue(2),
		item->GetAttributeType(3), item->GetAttributeValue(3),
		item->GetAttributeType(4), item->GetAttributeValue(4),
		item->GetAttributeType(5), item->GetAttributeValue(5),
		item->GetAttributeType(6), item->GetAttributeValue(6));
#endif

	char QueryInsert[QUERY_MAX_LEN];
	snprintf(QueryInsert, sizeof(QueryInsert), "INSERT INTO player.mailbox_items(%s) VALUES(%s)", szColumns, szValues);
	DBManager::Instance().DirectQuery(QueryInsert);
	ITEM_MANAGER::instance().RemoveItem(item);
}

bool CHARACTER::FindPlayerName(const char* name)
{
	char szQuery3[1024];
	snprintf(szQuery3, sizeof(szQuery3),
		"SELECT * FROM %splayer WHERE name = '%s'", get_table_postfix(), name);
	std::unique_ptr<SQLMsg> pMsg3(DBManager::Instance().DirectQuery(szQuery3));
	if (pMsg3->Get()->uiNumRows == 0) {
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[MailBox] No existe ningun jugador con ese nombre"));
		return false;
	}
	return true;
}

int CHARACTER::GetIdPlayerReceive(const char* name)
{
	char szQuery4[1024];
	DWORD id_player_receive;
	snprintf(szQuery4, sizeof(szQuery4),
		"SELECT id FROM %splayer WHERE name = '%s'", get_table_postfix(), name);
	std::unique_ptr<SQLMsg> pMsg4(DBManager::Instance().DirectQuery(szQuery4));
	MYSQL_ROW  row = mysql_fetch_row(pMsg4->Get()->pSQLResult);
	str_to_number(id_player_receive, row[0]);
	return id_player_receive;
}

int CHARACTER::GetCountMails(int id_player_receive)
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT COUNT(*) FROM %smailbox WHERE id_receive_mail = %u", get_table_postfix(), id_player_receive));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;
	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	int bCount = 0;
	str_to_number(bCount, row[0]);
	return bCount;
}

bool CHARACTER::FindIDMail(int id_mail)
{
	for (int i = 0; i < info_mailbox_values.size(); ++i) {
		if (info_mailbox_values[i].id_mail == id_mail && info_mailbox_values[i].id_receive_mail == GetPlayerID()) {
			return true;
		}
	}
	return false;
}

bool CHARACTER::FindIDReceiveMail()
{
	if (info_mailbox_values.size() <= 0) {
		return false;
	}
	return true;
}

int CHARACTER::get_check_acept_mailbox(int id_mail)
{
	for (int i = 0; i < info_mailbox_values.size(); ++i) {
		if (info_mailbox_values[i].id_mail == id_mail && info_mailbox_values[i].id_receive_mail == GetPlayerID()) {
			return info_mailbox_values[i].check_acept;
		}
	}
	return 0;
}

int CHARACTER::get_item_vnum_mailbox(int id_mail)
{
	for (int i = 0; i < info_mailbox_values.size(); ++i) {
		if (info_mailbox_values[i].id_mail == id_mail && info_mailbox_values[i].id_receive_mail == GetPlayerID()) {
			return info_mailbox_values[i].item_vnum;
		}
	}
	return 0;
}

int CHARACTER::get_id_item_mailbox(int id_mail)
{
	for (int i = 0; i < info_mailbox_values.size(); ++i) {
		if (info_mailbox_values[i].id_mail == id_mail && info_mailbox_values[i].id_receive_mail == GetPlayerID()) {
			return info_mailbox_values[i].id_item;
		}
	}
	return 0;
}

int CHARACTER::get_gold_mailbox(int id_mail)
{
	for (int i = 0; i < info_mailbox_values.size(); ++i) {
		if (info_mailbox_values[i].id_mail == id_mail && info_mailbox_values[i].id_receive_mail == GetPlayerID()) {
			return info_mailbox_values[i].gold;
		}
	}
	return 0;
}

#ifdef USE_WON_SYSTEM_MAILBOX
int CHARACTER::get_won_mailbox(int id_mail)
{
	for (int i = 0; i < info_mailbox_values.size(); ++i) {
		if (info_mailbox_values[i].id_mail == id_mail && info_mailbox_values[i].id_receive_mail == GetPlayerID()) {
			return info_mailbox_values[i].won;
		}
	}
	return 0;
}
#endif

bool CHARACTER::get_mailbox_time_extra() {
	for (int i = 0; i < info_mailbox_values.size(); ++i) {
		DWORD time = info_mailbox_values[i].time - get_global_time();
		if (time > 0) {
			return false;
		}
	}
	return true;
}

EVENTFUNC(check_time_mailbox_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);
	if (info == nullptr)
	{
		sys_err("check_time_mailbox_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (nullptr == ch || ch->IsNPC())
		return 0;

	bool check_delete = false;

	if (ch->get_mailbox_size() > 0) {
		for (int i = 0; i < ch->get_mailbox_size(); ++i) {
			if ((ch->get_mailbox_time(i) - get_global_time()) < 0) {
				DBManager::instance().Query("DELETE FROM %smailbox WHERE id_mail=%d", get_table_postfix(), ch->get_mailbox_id_mail(i));
				ch->SendMailBoxPackets(MAILBOX_SUB_HEADER_CHECK_DELETE_SUCCESSFUL, ch->get_mailbox_id_mail(i));
				ch->ChatPacket(CHAT_TYPE_INFO, "email_delete %d", ch->get_mailbox_id_mail(i));
				check_delete = true;
			}
		}

		if (check_delete == true) {
			ch->ChatPacket(CHAT_TYPE_INFO, "email_delete2 ");
			ch->ClearLIST(0);
			ch->SendMailBoxPackets(MAILBOX_SUB_HEADER_LOADING_DELETE);
		}
	}

	ch->ChatPacket(CHAT_TYPE_INFO, "%d", ch->get_mailbox_size());

	return PASSES_PER_SEC(5);
}

void CHARACTER::StartCheckTimeMailBox()
{
	if (MailBoxUpdateTime)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;

	MailBoxUpdateTime = event_create(check_time_mailbox_event, info, PASSES_PER_SEC(60));	// 1ºÐ
}