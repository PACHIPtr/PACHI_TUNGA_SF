#ifndef __INC_METIN_II_ABUSE_H__
#define __INC_METIN_II_ABUSE_H__

class AbuseController
{
	public:
		AbuseController(LPCHARACTER ch);
		void InitiatedFight(const PIXEL_POSITION& mobPos, bool aggressive);
		void SuspiciousAttackCooldown();
		void DeadWalk();
		void DeadPickup();
		void MarkDead() { m_lastDeathTime = get_dword_time(); }
		bool JustDied() const { return m_lastDeathTime + 1500 > get_dword_time(); }
		void VerifyCoordinates(LPSECTREE tree, long x, long y, long z);
		bool CanPickupItem();
		void SignalMovement(DWORD lastStopTime);
		void CheckSpeedhack(long x, long y);
		void ResetMovementSpeedhackChecker(bool full = false);
		void Analyze();
		bool IsSuspectedFishbot() const { return m_isSuspectedFishbot; }
		bool IsHideFishingBubble() const;
		void SuspectFishbotBy(LPCHARACTER ch);
		void EndSuspectFishbot();
		void NotifyHiddenBubble();
		void NotifyFishedWithBubbleHide(bool valid, long ms);		
		void ReceiveMoveAttackPacket() { m_lastMoveAttackPacketTime = get_dword_time(); };
		void ReceiveAttackPacket();
		void AutoPickupSuspect();
	private:
		LPCHARACTER m_ch;
		DWORD m_lastUpdatePulse;
		DWORD m_lastAttackAnalysis;
		DWORD m_lastSuspiciousAttackTime;
		DWORD m_nextAttackCooldown;
		DWORD m_nextDetectStart;
		struct PullInfo 
		{
			PIXEL_POSITION mobPos;
			PIXEL_POSITION playerPos;
			bool isAggressive;
		};
		std::vector<PullInfo> m_vecPullInfo;
		void AnalyzeAttacks();
		DWORD m_lastDeathTime;
		DWORD m_nextDeadWalkLog;
		DWORD m_nextDeadPickupLog;
		long m_whStartX;
		long m_whStartY;
		long m_whStartZ;
		int m_whHackCount;
		DWORD m_whLastDetection;
		DWORD m_whIgnoreUntil;
		int m_whContinuousInvalid;
		DWORD m_whNextDecrease;
		long m_mshStartX;
		long m_mshStartY;
		long m_mshStartZ;
		std::vector<double> m_mshRates;
		DWORD m_mshStartDetect;
		DWORD m_mshLastDetect;
		DWORD m_mshIgnoreUntil;
		DWORD m_mshFirstDetect;
		bool m_isSuspectedFishbot;
		int m_fishesLeftToBubbleHide;
		int m_bubbleHideTimesLeft;
		int m_bubbleHideTimes;
		bool m_waitDmgSuspect;
		DWORD m_lastMoveAttackPacketTime;
		DWORD m_ignoreWaitDmgUntil;
		std::vector<DWORD> m_moveAttackTimes;
		int m_pickupBotSuspectCount;
};

template <typename T>
const T getMedian(std::vector<T>& values, size_t vectorSize)
{
	T median = 0;

	if (vectorSize > 0)
	{
		std::sort(values.begin(), values.end());

		if (vectorSize % 2 == 0)
			median = (values[vectorSize / 2 - 1] + values[vectorSize / 2]) / 2;
		else
			median = values[(size_t)std::floor(vectorSize / 2)];
	}

	return median;
}

template <typename T>
const T getAverage(std::vector<T>& values, size_t vectorSize)
{
	T average = 0;

	if (vectorSize > 0)
	{
		for (const auto value : values)
			average += value;

		average /= vectorSize;
	}

	return average;
}

typedef std::shared_ptr<AbuseController> spAbuseController;

#endif // !__INC_METIN_II_ABUSE_H__