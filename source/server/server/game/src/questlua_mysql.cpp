#include "stdafx.h"
#include <sstream>
#include "char.h"
#include "questmanager.h"
#include "questlua.h"
#include "questevent.h"
#include "config.h"
#include "db.h"

namespace quest
{
	// mysql_update_query(string) - 'returns 0 for failed and 1 for OK
	ALUA(_mysql_update_query)
	{
		SQLMsg* msg = DBManager::instance().DirectQuery(lua_tostring(L, 1));
		if (msg) {
			lua_pushnumber(L, 1);
		}
		else {
			sys_err("MySQL Query failed!");
			lua_pushnil(L);
		}
		M2_DELETE(msg);
		return 1;
	}

	// mysql_query(string) - 'returns a lua mysql table'
	ALUA(_mysql_query)
	{
		string query = lua_tostring(L, 1);
		SQLMsg* run = DBManager::instance().DirectQuery(query.c_str());
		MYSQL_RES* res = run->Get()->pSQLResult;

		//Error in the query
		if (!run) {
			sys_err("MySQL query failed!");
			lua_pushnil(L);
			return 0;
		}

		if (!res) {
			lua_newtable(L);
			return 1;
		}

		lua_newtable(L);

		//Fetch field list
		unsigned long rowcount = 1;

		set<MYSQL_FIELD*> fieldList;
		while (MYSQL_FIELD * f = mysql_fetch_field(res)) {
			fieldList.insert(f);
		}

		MYSQL_ROW row;
		while ((row = mysql_fetch_row(res)))
		{
			lua_newtable(L);

			int i = 0;
			set<MYSQL_FIELD*>::iterator iter;
			for (iter = fieldList.begin(); iter != fieldList.end(); ++iter)
			{
				MYSQL_FIELD* f = *iter;
				lua_pushstring(L, f->name);
				lua_pushstring(L, row[i]);
				lua_settable(L, -3); //stack: [arg] table table2
				i++;
			}

			lua_pushnumber(L, rowcount);
			lua_insert(L, -2); //stack: [arg] table rowcount table2
			lua_settable(L, -3);
			rowcount++;
		}

		M2_DELETE(run);
		return 1;
	}

	// myqsl_real_escape_string(string) - 'returns an escaped string'
	ALUA(_mysql_real_escape_string)
	{
		char* cescapedstring = new char[strlen(lua_tostring(L, 1)) * 3 + 1];
		DBManager::instance().EscapeString(cescapedstring, strlen(lua_tostring(L, 1)) * 2 + 1, lua_tostring(L, 1), strlen(lua_tostring(L, 1)));
		lua_pushstring(L, cescapedstring);
		return 1;
	}

	ALUA(_get_table_postfix)
	{
		lua_pushstring(L, get_table_postfix());
		return 1;
	}

#ifdef _MSC_VER
#define INFINITY (DBL_MAX+DBL_MAX)
#define NAN (INFINITY-INFINITY)
#endif
	ALUA(_mysql_direct_query)
	{
		// char szQuery[1024];

		if (!lua_isstring(L, 1))
			return 0;
		// strncpy(szQuery, lua_tostring(L, 1), sizeof(szQuery));

		int i = 0, m = 1;
		MYSQL_ROW row;
		MYSQL_FIELD* field;
		MYSQL_RES* result;
		// SQLMsg * pMsg = DBManager::instance().DirectQuery(szQuery);
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(lua_tostring(L, 1)));
		if (pMsg.get())
		{
			// ret1 (number of affected rows)
			lua_pushnumber(L, pMsg->Get()->uiAffectedRows);
			//-1 if error such as duplicate occurs (-2147483648 via lua)
			//   if wrong syntax error occurs (4294967295 via lua)
			// ret2 (table of affected rows)
			lua_newtable(L);
			if ((result = pMsg->Get()->pSQLResult) &&
				!(pMsg->Get()->uiAffectedRows == 0 || pMsg->Get()->uiAffectedRows == (uint32_t)-1))
			{
				// LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
				// ch->ChatPacket(CHAT_TYPE_INFO, "<%s> Retrieved %u fields\n", __FUNCTION__, mysql_num_fields(result));
				// ch->ChatPacket(CHAT_TYPE_INFO, "<%s> Retrieved %u rows\n", __FUNCTION__, mysql_num_rows(result));
				// ch->ChatPacket(CHAT_TYPE_INFO, "<%s> Affected %u rows\n", __FUNCTION__, pMsg->Get()->uiAffectedRows);
				// ch->ChatPacket(CHAT_TYPE_INFO, "<%s> Num %u rows\n", __FUNCTION__, pMsg->Get()->uiNumRows);

				while ((row = mysql_fetch_row(result)))
				{
					lua_pushnumber(L, m);
					lua_newtable(L);
					while ((field = mysql_fetch_field(result)))
					{
						lua_pushstring(L, field->name);
						if (!(field->flags & NOT_NULL_FLAG) && (row[i] == nullptr))
						{
							// lua_pushstring(L, "nullptr");
							lua_pushnil(L);
						}
						else if (IS_NUM(field->type))
						{
							double val = NAN;
							lua_pushnumber(L, (sscanf(row[i], "%lf", &val) == 1) ? val : NAN);
						}
						else if (field->type == MYSQL_TYPE_BLOB)
						{
							lua_newtable(L);
							for (DWORD iBlob = 0; iBlob < field->max_length; iBlob++)
							{
								lua_pushnumber(L, row[i][iBlob]);
								lua_rawseti(L, -2, iBlob + 1);
							}
						}
						else
							lua_pushstring(L, row[i]);
						// LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
						// ch->ChatPacket(CHAT_TYPE_INFO, "<%s> Retrieved %d flag %s for %s\n", __FUNCTION__, field->type, field->name, row[i]?row[i]:"nullptr");
						lua_rawset(L, -3);
						i++;
					}
					mysql_field_seek(result, 0);
					i = 0;

					lua_rawset(L, -3);
					m++;
				}
			}
		}
		else { lua_pushnumber(L, 0); lua_newtable(L); }

		// delete pMsg;
		return 2;
	}

	void RegisterMysqlFunctionTable()
	{
		luaL_reg mysql_functions[] =
		{
			{"get_table_postfix", _get_table_postfix},
			{"direct_query", _mysql_direct_query},
			{"query", _mysql_query},
			{"real_escape_string", _mysql_real_escape_string},
			{"update", _mysql_update_query},
			{nullptr, nullptr}
		};

		CQuestManager::instance().AddLuaFunctionTable("mysql", mysql_functions);
	}
}