#include "stdafx.h"
#include "Cache.h"
#include "../../common/tables.h"
#include "QID.h"
#include "ClientManager.h"
#include "Main.h"

extern int g_iItemCacheFlushSeconds;
extern int g_test_server;
// MYSHOP_PRICE_LIST
extern int g_iItemPriceListTableCacheFlushSeconds;
extern int g_iPlayerCacheFlushSeconds;
const int CItemPriceListTableCache::s_nMinFlushSec = 1800;
extern int g_item_count;

CItemCache::CItemCache()
{
	m_expireTime = MIN(1800, g_iItemCacheFlushSeconds);
}

CItemCache::~CItemCache()
{
}

// 이거 이상한데...
// Delete를 했으면, Cache도 해제해야 하는것 아닌가???
// 근데 Cache를 해제하는 부분이 없어.
// 못 찾은 건가?
// 이렇게 해놓으면, 계속 시간이 될 때마다 아이템을 계속 지워...
// 이미 사라진 아이템인데... 확인사살??????
// fixme
// by rtsummit
void CItemCache::Delete()
{
	if (m_data.vnum == 0)
		return;

	//char szQuery[QUERY_MAX_LEN];
	//szQuery[QUERY_MAX_LEN] = '\0';
	if (g_test_server)
		sys_log(0, "ItemCache::Delete : DELETE %u", m_data.id);

	m_data.vnum = 0;
	m_bNeedQuery = true;
	m_lastUpdateTime = time(0);
	OnFlush();

	//m_bNeedQuery = false;
	//m_lastUpdateTime = time(0) - m_expireTime; // 바로 타임아웃 되도록 하자.
}

void CItemCache::OnFlush()
{
	TPlayerItem* p = &m_data;
	if (m_data.vnum == 0)
	{
		char szQuery[QUERY_MAX_LEN];
		snprintf(szQuery, sizeof(szQuery), "DELETE FROM item%s WHERE id=%u", GetTablePostfix(), m_data.id);
		CDBManager::instance().ReturnQuery(szQuery, QID_ITEM_DESTROY, 0, nullptr);
		if (g_test_server)
			sys_log(0, "ItemCache::Flush : DELETE %u %s", m_data.id, szQuery);
	}
	else
	{
		long alSockets[ITEM_SOCKET_MAX_NUM];
		TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
		bool isSocket = false, isAttr = false;
		memset(&alSockets, 0, sizeof(long) * ITEM_SOCKET_MAX_NUM);
		memset(&aAttr, 0, sizeof(TPlayerItemAttribute) * ITEM_ATTRIBUTE_MAX_NUM);

		if (memcmp(alSockets, p->alSockets, sizeof(long) * ITEM_SOCKET_MAX_NUM))
			isSocket = true;

		if (memcmp(aAttr, p->aAttr, sizeof(TPlayerItemAttribute) * ITEM_ATTRIBUTE_MAX_NUM))
			isAttr = true;

		char szColumns[QUERY_MAX_LEN];
		char szValues[QUERY_MAX_LEN];
		char szUpdate[QUERY_MAX_LEN];

		int iLen = snprintf(szColumns, sizeof(szColumns), "id, owner_id, window, pos, count, vnum");
		int iValueLen = snprintf(szValues, sizeof(szValues), "%u, %u, %d, %d, %u, %u", p->id, p->owner, p->window, p->pos, p->count, p->vnum);
		int iUpdateLen = snprintf(szUpdate, sizeof(szUpdate), "owner_id=%u, window=%d, pos=%d, count=%u, vnum=%u", p->owner, p->window, p->pos, p->count, p->vnum);

#ifdef ENABLE_BASIC_ITEM_SYSTEM
		iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ", is_basic");
		iValueLen += snprintf(szValues + iValueLen, sizeof(szValues) - iValueLen, ", %d", p->is_basic);
		iUpdateLen += snprintf(szUpdate + iUpdateLen, sizeof(szUpdate) - iUpdateLen, ", is_basic=%d", p->is_basic);
#endif

#ifdef ENABLE_CHANGELOOK_SYSTEM
		iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ", transmutation");
		iValueLen += snprintf(szValues + iValueLen, sizeof(szValues) - iValueLen, ", %u", p->transmutation);
		iUpdateLen += snprintf(szUpdate + iUpdateLen, sizeof(szUpdate) - iUpdateLen, ", transmutation=%u", p->transmutation);
#endif

		if (isSocket)
		{
			iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ", socket0, socket1, socket2, socket3");
			iValueLen += snprintf(szValues + iValueLen, sizeof(szValues) - iValueLen,
				", %lu, %lu, %lu, %lu", p->alSockets[0], p->alSockets[1], p->alSockets[2], p->alSockets[3]);
			iUpdateLen += snprintf(szUpdate + iUpdateLen, sizeof(szUpdate) - iUpdateLen,
				", socket0=%lu, socket1=%lu, socket2=%lu, socket3=%lu", p->alSockets[0], p->alSockets[1], p->alSockets[2], p->alSockets[3]);
		}

		if (isAttr)
		{
			iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen,
				", attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7");

			iValueLen += snprintf(szValues + iValueLen, sizeof(szValues) - iValueLen,
				", %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
				p->aAttr[0].bType, p->aAttr[0].sValue,
				p->aAttr[1].bType, p->aAttr[1].sValue,
				p->aAttr[2].bType, p->aAttr[2].sValue,
				p->aAttr[3].bType, p->aAttr[3].sValue,
				p->aAttr[4].bType, p->aAttr[4].sValue,
				p->aAttr[5].bType, p->aAttr[5].sValue,
				p->aAttr[6].bType, p->aAttr[6].sValue,
				p->aAttr[7].bType, p->aAttr[7].sValue,
				p->aAttr[8].bType, p->aAttr[8].sValue,
				p->aAttr[9].bType, p->aAttr[9].sValue,
				p->aAttr[10].bType, p->aAttr[10].sValue,
				p->aAttr[11].bType, p->aAttr[11].sValue,
				p->aAttr[12].bType, p->aAttr[12].sValue,
				p->aAttr[13].bType, p->aAttr[13].sValue,
				p->aAttr[14].bType, p->aAttr[14].sValue);

			iUpdateLen += snprintf(szUpdate + iUpdateLen, sizeof(szUpdate) - iUpdateLen,
				", attrtype0=%d, attrvalue0=%d"
				", attrtype1=%d, attrvalue1=%d"
				", attrtype2=%d, attrvalue2=%d"
				", attrtype3=%d, attrvalue3=%d"
				", attrtype4=%d, attrvalue4=%d"
				", attrtype5=%d, attrvalue5=%d"
				", attrtype6=%d, attrvalue6=%d"
				", applytype0=%d, applyvalue0=%d"
				", applytype1=%d, applyvalue1=%d"
				", applytype2=%d, applyvalue2=%d"
				", applytype3=%d, applyvalue3=%d"
				", applytype4=%d, applyvalue4=%d"
				", applytype5=%d, applyvalue5=%d"
				", applytype6=%d, applyvalue6=%d"
				", applytype7=%d, applyvalue7=%d",

				p->aAttr[0].bType, p->aAttr[0].sValue,
				p->aAttr[1].bType, p->aAttr[1].sValue,
				p->aAttr[2].bType, p->aAttr[2].sValue,
				p->aAttr[3].bType, p->aAttr[3].sValue,
				p->aAttr[4].bType, p->aAttr[4].sValue,
				p->aAttr[5].bType, p->aAttr[5].sValue,
				p->aAttr[6].bType, p->aAttr[6].sValue,
				p->aAttr[7].bType, p->aAttr[7].sValue,
				p->aAttr[8].bType, p->aAttr[8].sValue,
				p->aAttr[9].bType, p->aAttr[9].sValue,
				p->aAttr[10].bType, p->aAttr[10].sValue,
				p->aAttr[11].bType, p->aAttr[11].sValue,
				p->aAttr[12].bType, p->aAttr[12].sValue,
				p->aAttr[13].bType, p->aAttr[13].sValue,
				p->aAttr[14].bType, p->aAttr[14].sValue);
		}

#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
		iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ", sealbind");
		iValueLen += snprintf(szValues + iValueLen, sizeof(szValues) - iValueLen,
			", %ld", p->sealbind);

		iUpdateLen += snprintf(szUpdate + iUpdateLen, sizeof(szUpdate) - iUpdateLen,
			", sealbind=%ld", p->sealbind);
#endif

		char szItemQuery[QUERY_MAX_LEN + QUERY_MAX_LEN];
		snprintf(szItemQuery, sizeof(szItemQuery), "REPLACE INTO item%s (%s) VALUES(%s)", GetTablePostfix(), szColumns, szValues);

		if (g_test_server)
			sys_log(0, "ItemCache::Flush :REPLACE  (%s)", szItemQuery);

		CDBManager::instance().ReturnQuery(szItemQuery, QID_ITEM_SAVE, 0, nullptr);
		++g_item_count;
	}

	m_bNeedQuery = false;
}

//
// CPlayerTableCache
//
CPlayerTableCache::CPlayerTableCache()
{
	m_expireTime = MIN(1800, g_iPlayerCacheFlushSeconds);
}

CPlayerTableCache::~CPlayerTableCache()
{
}

void CPlayerTableCache::OnFlush()
{
	if (g_test_server)
		sys_log(0, "PlayerTableCache::Flush : %s", m_data.name);

	char szQuery[QUERY_MAX_LEN];
	CreatePlayerSaveQuery(szQuery, sizeof(szQuery), &m_data);
	CDBManager::instance().ReturnQuery(szQuery, QID_PLAYER_SAVE, 0, nullptr);
}

// MYSHOP_PRICE_LIST
//
// CItemPriceListTableCache class implementation
//

CItemPriceListTableCache::CItemPriceListTableCache()
{
	m_expireTime = MIN(s_nMinFlushSec, g_iItemPriceListTableCacheFlushSeconds);
}

void CItemPriceListTableCache::UpdateList(const TItemPriceListTable * pUpdateList)
{
	//
	// 이미 캐싱된 아이템과 중복된 아이템을 찾고 중복되지 않는 이전 정보는 tmpvec 에 넣는다.
	//

	std::vector<TItemPriceInfo> tmpvec;

	for (uint idx = 0; idx < m_data.byCount; ++idx)
	{
		const TItemPriceInfo* pos = pUpdateList->aPriceInfo;
		for (; pos != pUpdateList->aPriceInfo + pUpdateList->byCount && m_data.aPriceInfo[idx].dwVnum != pos->dwVnum; ++pos)
			;

		if (pos == pUpdateList->aPriceInfo + pUpdateList->byCount)
			tmpvec.push_back(m_data.aPriceInfo[idx]);
	}

	//
	// pUpdateList 를 m_data 에 복사하고 남은 공간을 tmpvec 의 앞에서 부터 남은 만큼 복사한다.
	//

	if (pUpdateList->byCount > SHOP_PRICELIST_MAX_NUM)
	{
		sys_err("Count overflow!");
		return;
	}

	m_data.byCount = pUpdateList->byCount;

	thecore_memcpy(m_data.aPriceInfo, pUpdateList->aPriceInfo, sizeof(TItemPriceInfo) * pUpdateList->byCount);

	int nDeletedNum;	// 삭제된 가격정보의 갯수

	if (pUpdateList->byCount < SHOP_PRICELIST_MAX_NUM)
	{
		size_t sizeAddOldDataSize = SHOP_PRICELIST_MAX_NUM - pUpdateList->byCount;

		if (tmpvec.size() < sizeAddOldDataSize)
			sizeAddOldDataSize = tmpvec.size();
		if (tmpvec.size() != 0)
		{
			thecore_memcpy(m_data.aPriceInfo + pUpdateList->byCount, &tmpvec[0], sizeof(TItemPriceInfo) * sizeAddOldDataSize);
			m_data.byCount += sizeAddOldDataSize;
		}
		nDeletedNum = tmpvec.size() - sizeAddOldDataSize;
	}
	else
		nDeletedNum = tmpvec.size();

	m_bNeedQuery = true;

	sys_log(0,
		"ItemPriceListTableCache::UpdateList : OwnerID[%u] Update [%u] Items, Delete [%u] Items, Total [%u] Items",
		m_data.dwOwnerID, pUpdateList->byCount, nDeletedNum, m_data.byCount);
}

void CItemPriceListTableCache::OnFlush()
{
	char szQuery[QUERY_MAX_LEN];

	//
	// 이 캐시의 소유자에 대한 기존에 DB 에 저장된 아이템 가격정보를 모두 삭제한다.
	//

	snprintf(szQuery, sizeof(szQuery), "DELETE FROM myshop_pricelist%s WHERE owner_id = %u", GetTablePostfix(), m_data.dwOwnerID);
	CDBManager::instance().ReturnQuery(szQuery, QID_ITEMPRICE_DESTROY, 0, nullptr);

	//
	// 캐시의 내용을 모두 DB 에 쓴다.
	//

	for (int idx = 0; idx < m_data.byCount; ++idx)
	{
#ifdef ENABLE_CHEQUE_SYSTEM
		snprintf(szQuery, sizeof(szQuery),
			"REPLACE myshop_pricelist%s(owner_id, item_vnum, price, cheque) VALUES(%u, %u, %u, %u)", // @fixme204 (INSERT INTO -> REPLACE)
			GetTablePostfix(), m_data.dwOwnerID, m_data.aPriceInfo[idx].dwVnum, m_data.aPriceInfo[idx].price.dwPrice, m_data.aPriceInfo[idx].price.byChequePrice);
#else
		snprintf(szQuery, sizeof(szQuery),
			"REPLACE myshop_pricelist%s(owner_id, item_vnum, price) VALUES(%u, %u, %u)", // @fixme204 (INSERT INTO -> REPLACE)
			GetTablePostfix(), m_data.dwOwnerID, m_data.aPriceInfo[idx].dwVnum, m_data.aPriceInfo[idx].dwPrice);
#endif
		CDBManager::instance().ReturnQuery(szQuery, QID_ITEMPRICE_SAVE, 0, nullptr);
	}

	sys_log(0, "ItemPriceListTableCache::Flush : OwnerID[%u] Update [%u]Items", m_data.dwOwnerID, m_data.byCount);

	m_bNeedQuery = false;
}
// END_OF_MYSHOP_PRICE_LIST