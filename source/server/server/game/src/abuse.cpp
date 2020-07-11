#include "stdafx.h"

#include "config.h"
#include "utils.h"
#include "char.h"
#include "desc.h"
#include "sectree.h"
#include "questmanager.h"
#include "log.h"

#include "abuse.h"
#include <algorithm>
#include <string>

AbuseController::AbuseController(LPCHARACTER ch)
{
	m_ch = ch;
	m_lastUpdatePulse = 0;
	m_lastAttackAnalysis = 0;
	m_lastSuspiciousAttackTime = 0;
	m_nextAttackCooldown = 0;
	m_nextDetectStart = 0;
	m_vecPullInfo.clear();
	m_lastDeathTime = 0;
	m_nextDeadWalkLog = 0;
	m_nextDeadPickupLog = 0;
	m_whIgnoreUntil = 0;
	m_whStartX = 0;
	m_whStartY = 0;
	m_whStartZ = 0;
	m_whHackCount = 0;
	m_whLastDetection = 0;
	m_whContinuousInvalid = 0;
	m_whNextDecrease = 0;
	m_mshStartDetect = 0;
	m_mshLastDetect = 0;
	m_mshFirstDetect = 0;
	m_mshStartX = 0;
	m_mshStartY = 0;
	m_mshIgnoreUntil = 0;
	m_mshRates.clear();
	m_isSuspectedFishbot = false;
	m_fishesLeftToBubbleHide = 0;
	m_bubbleHideTimes = 0;
	m_bubbleHideTimesLeft = 0;
	m_waitDmgSuspect = false;
	m_lastMoveAttackPacketTime = 0;
	m_ignoreWaitDmgUntil = 0;
}

void AbuseController::SuspiciousAttackCooldown()
{
	m_lastSuspiciousAttackTime = 0;
	m_vecPullInfo.clear();
}

void AbuseController::InitiatedFight(const PIXEL_POSITION& mobPos, bool aggressive)
{
	DWORD now = get_dword_time();

	if (m_nextDetectStart > now)
		return;

	m_lastSuspiciousAttackTime = now;
	
	const PIXEL_POSITION& playerPos = m_ch->GetXYZ();

	PullInfo pinfo;
	pinfo.mobPos = mobPos;
	pinfo.playerPos = playerPos;
	pinfo.isAggressive = aggressive;
	m_vecPullInfo.push_back(pinfo);
}

bool analyzeStaticMov(const std::vector<int>& distances, std::string& log)
{
	if (distances.size() < 5)
		return false;

	int average = 0;
	for (const int& d : distances)
		average += d;

	average /= distances.size();

	if (average > 1000) {
		log += " static" + std::to_string(distances.size())
			+ " avgstatic" + std::to_string(average);

		return true;
	}

	return false;
};

void AbuseController::AnalyzeAttacks()
{
	m_lastAttackAnalysis = thecore_pulse();

	if (m_vecPullInfo.size() < 5) { 
		if (!m_vecPullInfo.empty() && m_lastSuspiciousAttackTime + 3 * 1000 < get_dword_time())
			SuspiciousAttackCooldown();

		return;
	}

	std::vector<PIXEL_POSITION> monsterClusters;
	int aggresiveMobs = 0;

	for (size_t i = 0, size = m_vecPullInfo.size(); i < size; ++i)
	{
		const PIXEL_POSITION& mobPos = m_vecPullInfo[i].mobPos;
		if (m_vecPullInfo[i].isAggressive)
			++aggresiveMobs;

		bool fit = false;
		for (const auto c : monsterClusters)
		{
			if (DISTANCE_APPROX(c.x - mobPos.x, c.y - mobPos.y) < 200) {
				fit = true;
				break;
			}
		}

		if (!fit) {
			PIXEL_POSITION cluster;
			cluster.x = mobPos.x;
			cluster.y = mobPos.y;
			cluster.z = mobPos.z;
			monsterClusters.push_back(cluster);
		}
	}

	std::vector<int> distances;
	for (size_t i = 0, size = monsterClusters.size(); i < size; ++i)
	{
		for (size_t j = i + 1; j < size; ++j)
		{
			int dist = DISTANCE_APPROX(monsterClusters[j].x - monsterClusters[i].x, monsterClusters[j].y - monsterClusters[i].y);
			distances.push_back(dist);
		}
	}

	size_t distancesCount = distances.size();

	int median = getMedian(distances, distancesCount);
	int average = getAverage(distances, distancesCount);

	bool isHack = false;
	int aggresiveRatio = aggresiveMobs / m_vecPullInfo.size();
	std::string log = "MOB_PULL";

	if (median > 4000 + aggresiveRatio * 1000 ||
		(average > 4500 + aggresiveRatio * 1000 && median > 2000 + aggresiveRatio * 1000))
	{
		isHack = true;
	}
	else
	{
		PIXEL_POSITION basePosition = m_vecPullInfo[0].playerPos;

		std::vector<int> distances;
		for (size_t i = 1, size = m_vecPullInfo.size(); i < size && !isHack; ++i)
		{
			const PIXEL_POSITION& playerPos = m_vecPullInfo[i].playerPos;
			const int threshold = m_vecPullInfo[i].isAggressive ? 750 : 300;

			if (DISTANCE_APPROX(playerPos.x - basePosition.x, playerPos.y - basePosition.y) > threshold)
			{
				isHack = analyzeStaticMov(distances, log);

				distances.clear();
				basePosition = playerPos;
				continue;
			}

			const PIXEL_POSITION& mobPos = m_vecPullInfo[i].mobPos;
			distances.push_back(DISTANCE_APPROX(mobPos.x - basePosition.x, mobPos.y - basePosition.y));
		}

		if (!isHack)
			isHack = analyzeStaticMov(distances, log);
	}

	if (isHack && monsterClusters.size() < 3)
		isHack = false;

	if (isHack)
	{
		log += " avg" + std::to_string(average)
			+ " med" + std::to_string(median)
			+ " dc" + std::to_string(distancesCount)
			+ " mcc" + std::to_string(monsterClusters.size())
			+ " m" + std::to_string(m_ch->GetMapIndex());

		LogManager::instance().HackLog(log.c_str(), m_ch);

		m_vecPullInfo.clear();

		if (test_server) {
			sys_log(0, "MOB_PULL!: %s", log.c_str());
			m_ch->ChatPacket(CHAT_TYPE_BIG_NOTICE, "Mob pull detected: %s", log.c_str());
		}
	}

	SuspiciousAttackCooldown();
}

void AbuseController::DeadWalk()
{
	if (JustDied())
		return;

	DWORD now = get_dword_time();
	if (m_nextDeadWalkLog > now)
		return;

	std::string info = "DEAD_WALK t" + std::to_string(now - m_lastDeathTime);
	LogManager::instance().HackLog(info.c_str(), m_ch);
	m_ch->GetDesc()->DelayedDisconnect(10);

	m_nextDeadWalkLog = now + 3 * 1000;
}

void AbuseController::DeadPickup()
{
	if (JustDied())
		return;

	DWORD now = get_dword_time();
	if (m_nextDeadPickupLog > now)
		return;

	std::string info = "DEAD_PICKUP #%ld t" + std::to_string(now - m_lastDeathTime);

	m_ch->GetDesc()->DelayedDisconnect(10);
	LogManager::instance().HackLog(info.c_str(), m_ch);

	m_nextDeadPickupLog = now + 3 * 1000;
}

void AbuseController::VerifyCoordinates(LPSECTREE tree, long x, long y, long z)
{
	DWORD now = get_dword_time();
	if (m_whIgnoreUntil > now)
		return;

	bool canMove = !tree->IsAttr(x, y, ATTR_BLOCK | ATTR_OBJECT);
	if (canMove)
	{
		m_whContinuousInvalid = 0;

		if (m_whHackCount > 0 && m_whNextDecrease < now)
		{
			m_whHackCount -= 1 + std::min(10000, (int)(now - m_whNextDecrease)) / 2000;
			if (m_whHackCount < 0)
				m_whHackCount = 0;

			m_whNextDecrease = now + 2 * 2000;
			if (test_server)
				sys_log(0, "Decrease hack count for %s, new value: %d", m_ch->GetName(), m_whHackCount);
		}

		return;
	}

	for (int dx = -100; dx < 100; ++dx)
	{
		for (int dy = -100; dy < 100; ++dy)
		{
			if (!tree->IsAttr(x + dx, y + dy, ATTR_BLOCK | ATTR_OBJECT))
			{
				m_whContinuousInvalid = 0;
				return;
			}
		}
	}

	if (test_server)
		sys_log(0, "Coordinates %ld x %ld for %s don't check out", x, y, m_ch->GetName());

	if (m_whLastDetection + 15 * 1000 < now || !m_whStartX || !m_whStartY)
	{
		m_whLastDetection = now;
		m_whNextDecrease = now + 60 * 1000;
		m_whContinuousInvalid = 0;
		m_whStartX = x;
		m_whStartY = y;
		m_whStartZ = z;
		return;
	}

	++m_whHackCount;
	++m_whContinuousInvalid;
	m_whLastDetection = now;
	m_whNextDecrease = now + 15 * 1000;

	if (m_whHackCount >= 5)
	{
		bool punish = false;
		int hackDist = DISTANCE_APPROX(m_whStartX - x, m_whStartY - y);

		if (m_whHackCount > 50) {
			punish = true;
		}
		else if (abs(m_whHackCount - m_whContinuousInvalid) < m_whHackCount / 5)
		{
			int heightDif = abs(m_whStartZ - z);

			if (test_server) {
				sys_log(0, "Hacking values: dist %d, zdif: %d, count: %d, continuous: %d", hackDist, heightDif, m_whHackCount, m_whContinuousInvalid);
			}

			if (hackDist > 1500 ||
				heightDif > 100)
			{
				punish = true;
			}
		}

		if (punish)
		{
			if (test_server) {
				m_ch->ChatPacket(CHAT_TYPE_BIG_NOTICE, "Wall hacker! [%d, %d]", m_whHackCount, m_whContinuousInvalid);
				m_ch->ChatPacket(CHAT_TYPE_INFO, "d%d", hackDist);
			}

			std::string info = "WALL_HACK c" + std::to_string(m_whHackCount);
			info += " i" + std::to_string(m_whContinuousInvalid);
			info += " d" + std::to_string(hackDist);
			info += " sx" + std::to_string(m_whStartX);
			info += " sy" + std::to_string(m_whStartY);
			info += " cx" + std::to_string(x);
			info += " cy" + std::to_string(y);
			info += " m" + std::to_string(m_ch->GetMapIndex());

			LogManager::instance().HackLog(info.c_str(), m_ch);

			if (hackDist > 2000)
			{
				LPDESC d = m_ch->GetDesc();
				if (d) {
					m_ch->ChatPacket(CHAT_TYPE_NOTICE, "Beachte die Schranken dieser Welt.");
					d->DelayedDisconnect(number(10, 15));
				}
				else {
					sys_err("Error while trying to disconnect [%s] due to wallhacking", m_ch->GetName());
				}
			}

			m_whHackCount = 0;
			m_whIgnoreUntil = test_server ? now : now + 10 * 1000;
		}
	}
}

void AbuseController::ResetMovementSpeedhackChecker(bool full /* = false */)
{
	m_mshStartX = 0;
	m_mshStartY = 0;
	m_mshStartDetect = 0;

	if (full)
	{
		m_mshFirstDetect = 0;
		m_mshRates.clear();
	}
}

bool AbuseController::CanPickupItem()
{
	if (get_dword_time() - m_mshLastDetect > 3000)
		return true;

	size_t count = m_mshRates.size();
	if (count < 3)
		return true;

	return getMedian(m_mshRates, count) > 0.5;
}

void AbuseController::SignalMovement(DWORD lastStopTime)
{
	if (lastStopTime < get_dword_time() - 1000)
	{
		ResetMovementSpeedhackChecker(false);
		return;
	}
}

void AbuseController::CheckSpeedhack(long x, long y)
{
	DWORD now = get_dword_time();

	if (m_mshIgnoreUntil > now || m_ch->IsWalking())
		return;

	if (m_mshFirstDetect > 0 && m_mshFirstDetect + 60 * 1000 < now)
	{
		ResetMovementSpeedhackChecker(true);
	}

	if (!m_mshStartDetect || !m_mshStartX || !m_mshStartY)
	{
		m_mshStartX = x;
		m_mshStartY = y;
		m_mshStartDetect = now;
		return;
	}

	int dx = abs(m_mshStartX - x);
	int dy = abs(m_mshStartY - y);

	float distWalked = DISTANCE_SQRT(dx, dy);
	if (distWalked < 600)
		return;

	int expectedDuration = CalculateDuration(m_ch->GetLimitPoint(POINT_MOV_SPEED),
		(int)((distWalked / m_ch->GetMoveMotionSpeed()) * 1000.0f));

	int duration = now - m_mshStartDetect;
	double rate = (double)duration / expectedDuration;

	if (rate < 0.75)
	{
		m_mshLastDetect = now;

		if (test_server)
			m_ch->ChatPacket(CHAT_TYPE_GUILD, "Speedhack! Rate: %f", rate);

		if (!m_mshFirstDetect)
			m_mshFirstDetect = now;

		m_mshRates.push_back(rate);
		size_t count = m_mshRates.size();

		if (count >= 10)
		{
			const double median = getMedian(m_mshRates, count);
			const double average = getAverage(m_mshRates, count);

			std::string info = "MOV_SPEED_HACK r" + std::to_string(median) + " (avgr" + std::to_string(average) + ")"
				+ " t" + std::to_string((now - m_mshFirstDetect) / 1000)
				+ " (" + std::to_string(m_ch->GetX()) + ", " + std::to_string(m_ch->GetY()) + ")";

			LogManager::instance().HackLog(info.c_str(), m_ch);

			if (test_server)
				m_ch->ChatPacket(CHAT_TYPE_INFO, "Speedhack detected");

			if (!test_server)
				m_mshIgnoreUntil = now + 30 * 1000;


			ResetMovementSpeedhackChecker(true);
		}
	}
}

void AbuseController::Analyze()
{
	DWORD now = thecore_pulse();
	if (m_lastUpdatePulse + PASSES_PER_SEC(1) > now)
		return;

	if (m_lastAttackAnalysis + PASSES_PER_SEC(3) < now)
		AnalyzeAttacks();

	m_lastUpdatePulse = now;
}

void AbuseController::SuspectFishbotBy(LPCHARACTER suspiciousCH)
{
	if (!suspiciousCH || !suspiciousCH->IsGM())
		return;

	sys_log(0, "Abuse: %s suspects that %s might be a fishbot.", suspiciousCH->GetName(), m_ch->GetName());

	m_isSuspectedFishbot = true;
	m_fishesLeftToBubbleHide = number(1, 4);
	m_bubbleHideTimesLeft = 2;
}

void AbuseController::EndSuspectFishbot()
{
	sys_log(0, "Abuse: %s is no longer being tracked for fishbot.", m_ch->GetName());
	m_isSuspectedFishbot = false;
}

bool AbuseController::IsHideFishingBubble() const
{
	return m_fishesLeftToBubbleHide == 0;
}

void AbuseController::NotifyHiddenBubble()
{
	++m_bubbleHideTimes;
	--m_bubbleHideTimesLeft;
}

void AbuseController::NotifyFishedWithBubbleHide(bool valid, long ms)
{
	if (valid)
	{
		LogManager::instance().HackLog("POSSIBLE_FISHBOT [req] " + std::to_string(ms) + "ms", m_ch);

		if (m_bubbleHideTimes < 7)
			m_bubbleHideTimes += 2;
	}

	if (m_bubbleHideTimesLeft <= 0) {
		EndSuspectFishbot();
		return;
	}

	m_fishesLeftToBubbleHide = number(1, 7);
}

void AbuseController::ReceiveAttackPacket()
{
	DWORD now = get_dword_time();
	DWORD dif = now - m_lastMoveAttackPacketTime;

	if (m_ignoreWaitDmgUntil > now)
		return;

	if (!m_waitDmgSuspect && dif < 5000)
		return;

	if (!m_waitDmgSuspect)
	{
		m_ch->SetSuspectWaitDmg();
		m_waitDmgSuspect = true;
	}

	m_moveAttackTimes.push_back(dif);
	size_t count = m_moveAttackTimes.size();

	if (count >= 25) {
		const double median = getMedian(m_moveAttackTimes, count);

		if (median <= 1200)
			m_ignoreWaitDmgUntil = now + number(300, 600) * 1000;
		else if (median <= 2100)
			m_ignoreWaitDmgUntil = now + number(120, 300) * 1000;
		else if (median <= 3000)
			m_ignoreWaitDmgUntil = now + number(30, 60) * 1000;
		else {
			std::string desc = "WAITDMG " + std::to_string(median);
			desc += " (" + std::to_string(m_moveAttackTimes[0]) + ", " + std::to_string(m_moveAttackTimes[count - 1]) + ")";

			LogManager::instance().HackLog(desc, m_ch);

			m_ch->GetDesc()->DelayedDisconnect(5);

			m_ignoreWaitDmgUntil = now + 300 * 1000;
		}

		m_moveAttackTimes.clear();
	}
}

void AbuseController::AutoPickupSuspect()
{
	++m_pickupBotSuspectCount;
	if (m_pickupBotSuspectCount >= 10)
	{
		LogManager::instance().HackLog("POSSIBLE_PICKUP_BOT", m_ch);
		m_pickupBotSuspectCount = 0;
	}
}