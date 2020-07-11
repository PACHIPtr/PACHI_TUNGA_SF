#pragma once

template <class Type> class Pool
{
	public:
		Pool (void)
		{
			mHead = 0;
			mFree = 0;
			mData = 0;
			mCurrent = 0;
			mFreeCount = 0;
			mUsedCount = 0;
		};

		~Pool (void)
		{
			if (mData)
			{
				delete [] mData;
			}
		};


		void Release (void)
		{
			if (mData)
			{
				delete [] mData;
			}

			mHead = 0;
			mFree = 0;

			mData = 0;
			mCurrent = 0;
			mFreeCount = 0;
			mUsedCount = 0;
		};

		void Set (int maxitems)
		{
			if (mData)
			{
				delete [] mData;
			}
			mMaxItems = maxitems;
			mData = new Type[mMaxItems];
			mFree = mData;
			mHead = 0;
			int loopValue = (mMaxItems - 1);
			for (int i = 0; i < loopValue; i++)
			{
				mData[i].SetNext (&mData[i + 1]);
				if (i == 0)
				{
					mData[i].SetPrevious (0);
				}
				else
				{
					mData[i].SetPrevious (&mData[i - 1]);
				}
			}

			mData[loopValue].SetNext (0);
			mData[loopValue].SetPrevious (&mData[loopValue - 1]);
			mCurrent = 0;
			mFreeCount = maxitems;
			mUsedCount = 0;
		};


		Type* GetNext (bool& looped)
		{

			looped = false;

			if (!mHead)
			{
				return 0;
			}
			Type* ret;

			if (!mCurrent)
			{
				ret = mHead;
				looped = true;
			}
			else
			{
				ret = mCurrent;
			}

			if (ret)
			{
				mCurrent = ret->GetNext();
			}


			return ret;
		};

		bool IsEmpty (void) const
		{
			if (!mHead)
			{
				return true;
			}
			return false;
		};

		int Begin (void)
		{
			mCurrent = mHead;
			return mUsedCount;
		};

		int GetUsedCount (void) const
		{
			return mUsedCount;
		};
		int GetFreeCount (void) const
		{
			return mFreeCount;
		};

		Type* GetNext (void)
		{
			if (!mHead)
			{
				return 0;
			}

			Type* ret;

			if (!mCurrent)
			{
				ret = mHead;
			}
			else
			{
				ret = mCurrent;
			}

			if (ret)
			{
				mCurrent = ret->GetNext();
			}


			return ret;
		};

		void Release (Type* t)
		{

			if (t == mCurrent)
			{
				mCurrent = t->GetNext();
			}

			Type* prev = t->GetPrevious();

			if (prev)
			{
				Type* next = t->GetNext();
				prev->SetNext (next);
				if (next)
				{
					next->SetPrevious (prev);
				}
			}
			else
			{
				Type* next = t->GetNext();
				mHead = next;
				if (mHead)
				{
					mHead->SetPrevious (0);
				}
			}

			Type* temp = mFree;
			mFree = t;
			t->SetPrevious (0);
			t->SetNext (temp);

			mUsedCount--;
			mFreeCount++;
		};

		Type* GetFreeNoLink (void)
		{
			if (!mFree)
			{
				return 0;
			}
			Type* ret = mFree;
			mFree = ret->GetNext();
			mUsedCount++;
			mFreeCount--;
			ret->SetNext (0);
			ret->SetPrevious (0);
			return ret;
		};

		Type* GetFreeLink (void)
		{
			if (!mFree)
			{
				return 0;
			}
			Type* ret = mFree;
			mFree = ret->GetNext();
			Type* temp = mHead;
			mHead = ret;
			if (temp)
			{
				temp->SetPrevious (ret);
			}
			mHead->SetNext (temp);
			mHead->SetPrevious (0);
			mUsedCount++;
			mFreeCount--;
			return ret;
		};

		void AddAfter (Type* e, Type* item)
		{
			if (e)
			{
				Type* eprev = e->GetPrevious();
				Type* enext = e->GetNext();
				e->SetNext (item);
				item->SetNext (enext);
				item->SetPrevious (e);
				if (enext)
				{
					enext->SetPrevious (item);
				}
			}
			else
			{
				mHead = item;
				item->SetPrevious (0);
				item->SetNext (0);
			}

		}

		void AddBefore (Type* e, Type* item)
		{
			Type* eprev = e->GetPrevious();
			Type* enext = e->GetNext();

			if (!eprev)
			{
				mHead = item;
			}
			else
			{
				eprev->SetNext (item);
			}

			item->SetPrevious (eprev);
			item->SetNext (e);

			e->SetPrevious (item);

		}


	private:
		int   mMaxItems;
		Type* mCurrent;
		Type* mData;
		Type* mHead;
		Type* mFree;
		int   mUsedCount;
		int   mFreeCount;
};