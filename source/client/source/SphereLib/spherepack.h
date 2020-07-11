#pragma once
#include <assert.h>

#include "vector.h"
#include "pool.h"
#include "sphere.h"
#include "frustum.h"

#define DEMO 0

enum SpherePackFlag
{

	SPF_SUPERSPHERE		= (1 << 0),
	SPF_ROOT_TREE   = (1 << 1),
	SPF_LEAF_TREE   = (1 << 2),
	SPF_ROOTNODE    = (1 << 3),
	SPF_RECOMPUTE   = (1 << 4),
	SPF_INTEGRATE   = (1 << 5),
	SPF_HIDDEN      = (1 << 6),
	SPF_PARTIAL     = (1 << 7),
	SPF_INSIDE      = (1 << 8),
};

class SpherePackFactory;
class SpherePack;

class SpherePackCallback
{
	public:
		SpherePackCallback() {}
		virtual ~SpherePackCallback() {}

		virtual void VisibilityCallback (const Frustum& f,
										 SpherePack* sphere,
										 ViewState state)
		{};

		virtual void RayTraceCallback (const Vector3d& p1,
									   const Vector3d& dir,
									   float distance,
									   const Vector3d& sect,
									   SpherePack* sphere)
		{};

		virtual void RangeTestCallback (const Vector3d& searchpos,
										float distance,
										SpherePack* sphere,
										ViewState state)
		{};

		virtual void PointTest2dCallback (const Vector3d& searchpos,
										  SpherePack* sphere,
										  ViewState state)
		{};
};

class SpherePack : public Sphere
{
	public:
		SpherePack (void)
		{
			mUserData         = 0;
			mFactory          = 0;
			mNext             = 0;
			mPrevious         = 0;
			mParent           = 0;
			mNextSibling      = 0;
			mPrevSibling      = 0;
			mChildren         = 0;
			mChildCount       = 0;
			mFifo1            = 0;
			mFifo2            = 0;
			SetRadius (0);
			mCenter.Set (0, 0, 0);
			IS_SPHERE	= false;
		};
		virtual ~SpherePack()
		{
		}

		void Init (SpherePackFactory* factory,
				   const Vector3d& pos,
				   float radius,
				   void* userdata, bool isSphere)
		{
			IS_SPHERE			= isSphere;
			mUserData         = userdata;
			mParent           = 0;
			mNextSibling      = 0;
			mPrevSibling      = 0;
			mFlags            = 0;
			mFifo1            = 0;
			mFifo2            = 0;
			mFactory          = factory;
			mCenter           = pos;
			SetRadius (radius);
		};

		void SetSpherePackFlag (SpherePackFlag flag)
		{
			mFlags |= flag;
		};
		void ClearSpherePackFlag (SpherePackFlag flag)
		{
			mFlags &= ~flag;
		};
		bool HasSpherePackFlag (SpherePackFlag flag) const
		{
			if (mFlags & flag)
			{
				return true;
			}
			return false;
		};


		void SetParent (SpherePack* pack)
		{
			mParent = pack;
		};
		SpherePack* GetParent (void) const
		{
			return mParent;
		};

		inline void NewPos (const Vector3d& pos);
		inline void NewPosRadius (const Vector3d& pos, float radius);

		void Unlink (void)
		{
			if (mFifo1)
			{
				*mFifo1 = 0;
				mFifo1 = 0;
			}

			if (mFifo2)
			{
				*mFifo2 = 0;
				mFifo2 = 0;
			}

			if (mParent)
			{
				mParent->LostChild (this);
			}

			assert (!mChildren);

			mParent = 0;
		}


		void AddChild (SpherePack* pack)
		{

			SpherePack* my_child = mChildren;
			mChildren = pack;

			pack->SetNextSibling (my_child);
			pack->SetPrevSibling (0);
			pack->SetParent (this);

			if (my_child)
			{
				my_child->SetPrevSibling (pack);
			}

			mChildCount++;

			#if defined(_DEBUG)
			float dist = DistanceSquared (pack);
			float radius = sqrtf (dist) + pack->GetRadius();

			assert (radius <= GetRadius() + 0.0001f);
			#endif
		}

		void SetNextSibling (SpherePack* child)
		{
			mNextSibling = child;
		}
		void SetPrevSibling (SpherePack* child)
		{
			mPrevSibling = child;
		}

		SpherePack* _GetNextSibling (void) const
		{
			return mNextSibling;
		}
		SpherePack* _GetPrevSibling (void) const
		{
			return mPrevSibling;
		}
		SpherePack* GetChildren (void)    const
		{
			return mChildren;
		}

		SpherePack* GetNext (void)     const
		{
			return mNext;
		};
		SpherePack* GetPrevious (void) const
		{
			return mPrevious;
		};

		void SetNext (SpherePack* pack)
		{
			mNext = pack;
		};
		void SetPrevious (SpherePack* pack)
		{
			mPrevious = pack;
		};

		void* GetUserData (void) const
		{
			return mUserData;
		};
		void   SetUserData (void* data, bool isSphere)
		{
			mUserData = data;
			IS_SPHERE = isSphere;
		};

		float DistanceSquared (const SpherePack* pack) const
		{
			return mCenter.DistanceSq (pack->mCenter);
		};

		void LostChild (SpherePack* pack);

		const Vector3d& GetPos (void) const
		{
			return mCenter;
		};

		void Render (unsigned int color);

		bool Recompute (float gravy);

		int GetChildCount (void) const
		{
			return mChildCount;
		};

		#if DEMO
		void SetColor (unsigned int color)
		{
			mColor = color;
		};
		unsigned int GetColor (void) const
		{
			return mColor;
		};
		#endif

		void SetFifo1 (SpherePack** fifo)
		{
			mFifo1 = fifo;
		};

		void SetFifo2 (SpherePack** fifo)
		{
			mFifo2 = fifo;
		};

		void ComputeBindingDistance (SpherePack* parent)
		{
			mBindingDistance = parent->GetRadius() - GetRadius();
			if (mBindingDistance <= 0)
			{
				mBindingDistance = 0;
			}
			else
			{
				mBindingDistance *= mBindingDistance;
			}
		}

		void Visibility (const Frustum& f,
						 SpherePackCallback* callback,
						 ViewState state);

		void RayTrace (const Vector3d& p1,
					   const Vector3d& dir,
					   float distance,
					   SpherePackCallback* callback);


		void RangeTest (const Vector3d& p,
						float distance,
						SpherePackCallback* callback,
						ViewState state);

		void PointTest2d (const Vector3d& p,
						  SpherePackCallback* callback,
						  ViewState state);

		void Reset (void);

	private:
		SpherePack*       mNext;
		SpherePack*       mPrevious;

		SpherePack*       mParent;
		SpherePack*       mChildren;

		SpherePack*       mNextSibling;
		SpherePack*       mPrevSibling;

		SpherePack**      mFifo1;
		SpherePack**      mFifo2;

		long               mFlags;
		long               mChildCount;

		float             mBindingDistance;

		void*             mUserData;

		SpherePackFactory* mFactory;
		#if DEMO
		unsigned long      mColor;
		#endif

	public:
		bool				IS_SPHERE;
};

class SpherePackFifo
{
	public:
		SpherePackFifo (int fifosize)
		{
			mCount = 0;
			mSP = 0;
			mBottom = 0;
			mFifoSize = fifosize;
			mFifo = new SpherePack *[mFifoSize];
		};

		virtual ~SpherePackFifo (void)
		{
			delete [] mFifo;
		};

		SpherePack** Push (SpherePack* sphere)
		{
			mCount++;
			SpherePack** ret = &mFifo[mSP];
			mFifo[mSP] = sphere;
			mSP++;
			if (mSP == mFifoSize)
			{
				mSP = 0;
			}
			return ret;
		};

		SpherePack* Pop (void)
		{
			while (mSP != mBottom)
			{
				mCount--;
				SpherePack* ret = mFifo[mBottom];
				mBottom++;
				if (mBottom == mFifoSize)
				{
					mBottom = 0;
				}
				if (ret)
				{
					return ret;
				}
			}
			return 0;
		}

		bool Flush (SpherePack* pack)
		{
			if (mSP == mBottom)
			{
				return false;
			}
			int i = mBottom;
			while (i != mSP)
			{
				if (mFifo[i] == pack)
				{
					mFifo[i] = 0;
					return true;
				}
				i++;
				if (i == mFifoSize)
				{
					i = 0;
				}
			}
			return false;
		};

		int GetCount (void) const
		{
			return mCount;
		};

	private:
		int         mCount;
		int         mSP;
		int         mBottom;
		int         mFifoSize;
		SpherePack** mFifo;
};


class SpherePackFactory : public SpherePackCallback
{
	public:

		SpherePackFactory (int maxspheres,
						   float rootsize,
						   float leafsize,
						   float gravy);


		virtual ~SpherePackFactory (void);

		void Process (void);

		SpherePack* AddSphere_ (const Vector3d& pos,
								float radius,
								void* userdata,
								bool isSphere,
								int flags = SPF_LEAF_TREE);

		void AddIntegrate (SpherePack* pack);
		void AddRecompute (SpherePack* recompute);

		void Integrate (SpherePack* pack, SpherePack* supersphere, float node_size);

		void Render (void);

		void Remove (SpherePack* pack);
		#if DEMO
		unsigned int GetColor (void);
		#endif

		void FrustumTest (const Frustum& f, SpherePackCallback* callback);

		void RayTrace (const Vector3d& p1,
					   const Vector3d& p2,
					   SpherePackCallback* callback);

		void RangeTest (const Vector3d& center, float radius, SpherePackCallback* callback);
		void PointTest2d (const Vector3d& center, SpherePackCallback* callback);

		virtual void RayTraceCallback (const Vector3d& p1,
									   const Vector3d& dir,
									   float distance,
									   const Vector3d& sect,
									   SpherePack* sphere);

		virtual void RangeTestCallback (const Vector3d& p, float distance, SpherePack* sphere, ViewState state);
		virtual void PointTest2dCallback (const Vector3d& p, SpherePack* sphere, ViewState state);

		virtual void VisibilityCallback (const Frustum& f, SpherePack* sphere, ViewState state);


		void Reset (void);

	private:


		SpherePack* mRoot;
		SpherePack* mLeaf;
		SpherePackCallback* mCallback;

		Pool< SpherePack > mSpheres;

		SpherePackFifo* mIntegrate;
		SpherePackFifo* mRecompute;

		#if DEMO
#define MAXCOLORS 12
		int mColorCount;
		unsigned int mColors[MAXCOLORS];
		#endif

		float mMaxRootSize;
		float mMaxLeafSize;
		float mSuperSphereGravy;
};

void SpherePack::NewPos (const Vector3d& pos)
{
	mCenter = pos;

	if (mParent && !HasSpherePackFlag (SPF_INTEGRATE))
	{
		float dist = DistanceSquared (mParent);

		if (dist >= mBindingDistance)
		{
			mFactory->AddRecompute (mParent);
			Unlink();
			mFactory->AddIntegrate (this);
		}
	}

}

void SpherePack::NewPosRadius (const Vector3d& pos, float radius)
{
	mCenter = pos;

	if (mParent && !HasSpherePackFlag (SPF_INTEGRATE))
	{
		if (radius != GetRadius())
		{
			SetRadius (radius);
			ComputeBindingDistance (mParent);
		}

		mFactory->AddRecompute (mParent);

		float dist = DistanceSquared (mParent);

		if (dist >= mBindingDistance)
		{
			Unlink();
			mFactory->AddIntegrate (this);
		}
	}
}