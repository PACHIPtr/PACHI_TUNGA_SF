#pragma once

#include <cstdint>
#include <new>
#include <memory>
#include <mutex>
#include <unordered_map>

//#define DYNAMIC_POOL_STRICT
#define USE_NEW_POOLS
#define USE_TC_MALLOC

#if defined(USE_NEW_POOLS)
#if !defined(USE_TC_MALLOC)
template<typename T, std::size_t N = 32>
class CStaticPool
{
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const pointer const_pointer;
	typedef value_type& reference;
	typedef const reference const_reference;

	CStaticPool(const CStaticPool&) = delete;
	CStaticPool& operator=(const CStaticPool&) = delete;
	CStaticPool() : m_freelist(nullptr), m_allocation_count(0), m_name("CStaticPool<T, N>") { Create(N); }
	CStaticPool(CStaticPool&& o) : m_pool(std::move(o.m_pool)), m_freelist(o.m_freelist), m_allocation_count(o.m_allocation_count), m_name(std::move(o.m_name)), m_elements(0)
	{
		o.m_freelist = nullptr;
		o.m_allocation_count = 0;
		o.m_elements = 0;
	}
	virtual ~CStaticPool() { assert(m_allocation_count == 0); }

	CStaticPool& operator=(CStaticPool&& o)
	{
		std::unique_lock<std::recursive_mutex> lk(m_mutex);
		if (&o != this)
		{
			m_pool = std::move(o.m_pool);
			m_freelist = o.m_freelist;
			m_allocation_count = o.m_allocation_count;
			m_name = std::move(o.m_name);
			m_elements = o.m_elements;
			o.m_freelist = nullptr;
			o.m_allocation_count = 0;
			o.m_elements = 0;
		}
		return *this;
	}

	pointer Alloc()
	{
		std::unique_lock<std::recursive_mutex> lk(m_mutex);
		pointer obj = nullptr;
		if(m_freelist != nullptr)
		{
			obj = reinterpret_cast<pointer>(m_freelist);
			m_freelist = reinterpret_cast<std::uint8_t**>(*m_freelist);
			++m_allocation_count;
		}
		return obj;
	}
	void Free(pointer obj)
	{
		std::unique_lock<std::recursive_mutex> lk(m_mutex);
		if(reinterpret_cast<std::uintptr_t>(obj) >= reinterpret_cast<std::uintptr_t>(m_pool.get()) && reinterpret_cast<std::uintptr_t>(obj) < reinterpret_cast<std::uintptr_t>(m_pool.get()) + sizeof(value_type) * m_elements)
		{
			*reinterpret_cast<std::uint8_t**>(obj) = reinterpret_cast<std::uint8_t*>(m_freelist);
			m_freelist = reinterpret_cast<std::uint8_t**>(obj);
			--m_allocation_count;
		}
	}
	template<typename... Args> pointer Construct(Args&&... args)
	{
		std::unique_lock<std::recursive_mutex> lk(m_mutex);
		pointer obj = Alloc();
		
		// [Noff]: taken from cppreference no hate pls
		// using placement new to construct the object

		if (obj)
			::new(obj) T(std::forward<Args>(args)...);
		
		return obj;
	}
	void Destruct(pointer obj)
	{
		std::unique_lock<std::recursive_mutex> lk(m_mutex);
		if (reinterpret_cast<std::uintptr_t>(obj) >= reinterpret_cast<std::uintptr_t>(m_pool.get()) && reinterpret_cast<std::uintptr_t>(obj) < reinterpret_cast<std::uintptr_t>(m_pool.get()) + sizeof(value_type) * m_elements)
		{
			obj->~value_type();
			*reinterpret_cast<std::uint8_t**>(obj) = reinterpret_cast<std::uint8_t*>(m_freelist);
			m_freelist = reinterpret_cast<std::uint8_t**>(obj);
			--m_allocation_count;
		}
	}

	bool Empty() const { return m_allocation_count == 0; }
	std::size_t GetCapacity() const { return m_elements; }

	void Clear()
	{
		static_assert(sizeof(T) >= sizeof(std::uintptr_t), "Element size must be greater than uintptr_t");
		std::unique_lock<std::recursive_mutex> lk(m_mutex);
		if (!m_pool)
		{
			Create(N);
			return;
		}
		memset(m_pool.get(), 0, sizeof(value_type) * m_elements);
		std::uint8_t* end = m_pool.get() + sizeof(value_type) * (m_elements - 1);
		std::uint8_t* ptr = nullptr;
		for (std::size_t n = 0; n < m_elements; ++n)
		{
			*reinterpret_cast<std::uint8_t**>(end) = ptr;
			ptr = end;
			if(n != 0)
				end -= sizeof(value_type);
		}
		m_freelist = reinterpret_cast<std::uint8_t**>(end);
	}
	void FreeAll() { Clear(); }
	void Destroy()
	{
		std::unique_lock<std::recursive_mutex> lk(m_mutex);
		m_pool.reset();
		m_freelist = nullptr;
		m_elements = 0;
		m_allocation_count = 0;
	}

	void Create(std::size_t n)
	{
		std::unique_lock<std::recursive_mutex> lk(m_mutex);
		m_pool.reset(new std::uint8_t[sizeof(value_type) * n]);
		m_elements = n;
		Clear();
	}

	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() const { return m_name; }

private:
	mutable std::recursive_mutex m_mutex;
	std::unique_ptr<std::uint8_t> m_pool;
	std::uint8_t** m_freelist;
	std::size_t m_allocation_count;
	std::string m_name;
	std::size_t m_elements;
};

template<typename T, std::size_t N = 32>
class CDynamicPool
{
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const pointer const_pointer;
	typedef value_type& reference;
	typedef const reference const_reference;

	CDynamicPool(const CDynamicPool& o) = delete;
	CDynamicPool& operator=(const CDynamicPool&o) = delete;

	CDynamicPool() : m_name("CDynamicPool<T, N>"), m_cleanup_counter(0) {}
	CDynamicPool(CDynamicPool&& o) : m_pools(std::move(o.m_pools)), m_name(std::move(o.m_name)), m_cleanup_counter(o.m_cleanup_counter) { o.m_cleanup_counter = 0; }
	virtual ~CDynamicPool() {}

	CDynamicPool& operator=(CDynamicPool&& o)
	{
		std::unique_lock<std::recursive_mutex> lk(m_mutex);
		if(&o != this)
		{
			m_pools = std::move(o.m_pools);
			m_name = std::move(o.m_name);
			m_cleanup_counter = o.m_cleanup_counter;
			o.m_cleanup_counter = 0;
		}
		return *this;
	}

	pointer Alloc()
	{
		std::unique_lock<std::recursive_mutex> lk(m_mutex);
		pointer obj = nullptr;
		for(auto& pool : m_pools)
		{
			obj = pool.Alloc();
			if (obj)
				break;
		}
		if(!obj)
		{
			m_pools.emplace_back();
			obj = m_pools.back().Alloc();
		}
		if(++m_cleanup_counter > N * 2)
		{
			m_cleanup_counter = 0;
			Cleanup();
		}
		return obj;
	}
	template<typename... Args> pointer Construct(Args&&... args)
	{
		std::unique_lock<std::recursive_mutex> lk(m_mutex);
		pointer obj = nullptr;
		for (auto& pool : m_pools)
		{
			obj = pool.Construct(std::forward<Args>(args)...);
			if (obj)
				break;
		}
		if (!obj)
		{
			m_pools.emplace_back();
			obj = m_pools.back().Construct(std::forward<Args>(args)...);
		}
		if (++m_cleanup_counter > N * 2)
		{
			m_cleanup_counter = 0;
			Cleanup();
		}
		return obj;
	}
	void Free(pointer obj)
	{
		std::unique_lock<std::recursive_mutex> lk(m_mutex);
		for (auto& pool : m_pools)
			pool.Free(obj);
		if (++m_cleanup_counter > N * 2)
		{
			m_cleanup_counter = 0;
			Cleanup();
		}
	}
	void Destruct(pointer obj)
	{
		std::unique_lock<std::recursive_mutex> lk(m_mutex);
		for (auto& pool : m_pools)
			pool.Free(obj);
		if (++m_cleanup_counter > N * 2)
		{
			m_cleanup_counter = 0;
			Cleanup();
		}
	}

	void Clear()
	{
		std::unique_lock<std::recursive_mutex> lk(m_mutex);
		m_pools.clear();
		m_cleanup_counter = 0;
	}
	void FreeAll() { Clear(); }
	void Destroy() { Clear(); }

	std::size_t GetCapacity() const { return N * m_pools.size(); }

	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() const { return m_name; }

	void Create(std::size_t n)
	{
		std::unique_lock<std::recursive_mutex> lk(m_mutex);
		if (n % N != 0)
			n = n / N + 1;
		else
			n = n / N;
		m_pools.clear();
		m_pools.resize(n);
	}

	void Cleanup()
	{
		std::unique_lock<std::recursive_mutex> lk(m_mutex);
		for(auto iter = m_pools.begin(); iter != m_pools.end(); ++iter)
		{
			if ((*iter).Empty())
				m_pools.erase(iter--);
		}
	}

private:
	mutable std::recursive_mutex m_mutex;
	std::vector< CStaticPool<value_type, N> > m_pools;
	std::string m_name;
	std::size_t m_cleanup_counter;
};
template<typename T, std::size_t N = 32>
using CDynamicPoolEx = CDynamicPool<T, N>;
#else
template<typename T>
class CDynamicPool
{
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const pointer const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;

	CDynamicPool(const CDynamicPool& o) = delete;
	CDynamicPool& operator=(const CDynamicPool&o) = delete;

	CDynamicPool() : m_name("CDynamicPool<T>") {}
	CDynamicPool(CDynamicPool&& o) : m_name(std::move(o.m_name)), m_allocated(std::move(o.m_allocated)) {}
	virtual ~CDynamicPool()
	{
		assert(m_allocated.empty());
		Clear();
	}

	CDynamicPool& operator=(CDynamicPool&& o)
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		if (&o != this)
		{
			m_name = std::move(o.m_name);
			m_allocated = std::move(o.m_allocated);
		}
		return *this;
	}

	pointer Alloc()
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		pointer obj = nullptr;
		if (m_free.empty())
			obj = reinterpret_cast<pointer>(malloc(sizeof(value_type)));
		else
		{
			obj = m_free.front();
			m_free.pop_front();
		}
		if(obj)
			m_allocated.emplace(obj, false);
		return obj;
	}
	template<typename... Args> pointer Construct(Args&&... args)
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		pointer obj = nullptr;
		if(m_free.empty())
			obj = reinterpret_cast<pointer>(malloc(sizeof(value_type)));
		else
		{
			obj = m_free.front();
			m_free.pop_front();
		}
		if (obj)
		{
			::new (obj) value_type(std::forward<Args>(args)...);
			m_allocated.emplace(obj, true);
		}
		return obj;
	}
	void Free(pointer obj)
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		auto iter = m_allocated.find(obj);
		if (iter != m_allocated.end())
		{
			//free(iter->first);
			m_free.emplace_back(obj);
			m_allocated.erase(iter);
		}
	}
	void Destruct(pointer obj)
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		auto iter = m_allocated.find(obj);
		if (iter != m_allocated.end())
		{
			obj->~value_type();
			//free(iter->first);
			m_free.emplace_back(obj);
			m_allocated.erase(iter);
		}
	}

	void Destroy(pointer obj)
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		auto iter = m_allocated.find(obj);
		if (iter != m_allocated.end())
		{
			if(iter->second)
				obj->~value_type();
			free(iter->first);
			m_allocated.erase(iter);
		}
	}

	void Clear()
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		for(auto iter : m_allocated)
		{
			if (iter.second)
				iter.first->~value_type();
			free(iter.first);
		}
		for (auto iter : m_free)
			free(iter);
		m_allocated.clear();
		m_free.clear();
	}
	void FreeAll() { Clear(); }
	void Destroy() { Clear(); }

	// ReSharper disable once CppMemberFunctionMayBeStatic
	std::size_t GetCapacity() const { return 0x7fffffff; }

	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() const { return m_name; }

	void Create(std::size_t n)
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		for (auto iter : m_allocated)
		{
			if (iter.second)
				iter.first->~value_type();
			//free(iter.first);
			m_free.emplace_back(iter.first);
		}
		m_allocated.clear();
		m_allocated.reserve(n);
	}

private:
	mutable std::mutex m_mutex;
	std::string m_name;
	std::unordered_map<pointer, bool> m_allocated;
	std::list<pointer> m_free;
};
template<typename T>
using CDynamicPoolEx = CDynamicPool<T>;
#endif
#else
template<typename T>
class CDynamicPool
{
	public:
		CDynamicPool()
		{
			//Tracen(typeid(T).name());
			m_uInitCapacity = 0;
			m_uUsedCapacity = 0;
		}
		virtual ~CDynamicPool()
		{
			assert (m_kVct_pkData.empty());
//#ifdef _DEBUG
//			char szText[256];
//			sprintf(szText, "--------------------------------------------------------------------- %s Pool Capacity %d\n", typeid(T).name(), m_uUsedCapacity);
//			OutputDebugString(szText);
//			printf(szText);
//#endif
		}

		void SetName (const char* c_szName)
		{
		}

		void Clear()
		{
			Destroy();
		}

		void Destroy()
		{
			/*
			#ifdef _DEBUG
						if (!m_kVct_pkData.empty())
						{
							char szText[256];
							sprintf(szText, "--------------------------------------------------------------------- %s Pool Destroy\n", typeid(T).name());
							OutputDebugString(szText);
							printf(szText);
						}
			#endif
			*/
			std::for_each (m_kVct_pkData.begin(), m_kVct_pkData.end(), Delete);
			m_kVct_pkData.clear();
			m_kVct_pkFree.clear();
		}

		void Create (UINT uCapacity)
		{
			m_uInitCapacity = uCapacity;
			m_kVct_pkData.reserve (uCapacity);
			m_kVct_pkFree.reserve (uCapacity);
		}
		T* Alloc()
		{
			if (m_kVct_pkFree.empty())
			{
				T* pkNewData = new T;
				m_kVct_pkData.push_back (pkNewData);
				++m_uUsedCapacity;
				return pkNewData;
			}

			T* pkFreeData = m_kVct_pkFree.back();
			m_kVct_pkFree.pop_back();
			return pkFreeData;
		}
		void Free (T* pkData)
		{
			#ifdef DYNAMIC_POOL_STRICT
			assert (__IsValidData (pkData));
			assert (!__IsFreeData (pkData));
			#endif
			m_kVct_pkFree.push_back (pkData);
		}
		void FreeAll()
		{
			m_kVct_pkFree = m_kVct_pkData;
		}

		DWORD GetCapacity()
		{
			return m_kVct_pkData.size();
		}

	protected:
		bool __IsValidData (T* pkData)
		{
			if (m_kVct_pkData.end() == std::find (m_kVct_pkData.begin(), m_kVct_pkData.end(), pkData))
			{
				return false;
			}
			return true;
		}
		bool __IsFreeData (T* pkData)
		{
			if (m_kVct_pkFree.end() == std::find (m_kVct_pkFree.begin(), m_kVct_pkFree.end(), pkData))
			{
				return false;
			}

			return true;
		}

		static void Delete (T* pkData)
		{
			delete pkData;
		}

	protected:
		std::vector<T*> m_kVct_pkData;
		std::vector<T*> m_kVct_pkFree;

		UINT m_uInitCapacity;
		UINT m_uUsedCapacity;
};


template<typename T>
class CDynamicPoolEx
{
	public:
		CDynamicPoolEx()
		{
			m_uInitCapacity = 0;
			m_uUsedCapacity = 0;
		}
		virtual ~CDynamicPoolEx()
		{
			assert (m_kVct_pkFree.size() == m_kVct_pkData.size());
			Destroy();

			#ifdef _DEBUG
			char szText[256];
			sprintf (szText, "--------------------------------------------------------------------- %s Pool Capacity %d\n", typeid (T).name(), m_uUsedCapacity);
			OutputDebugString (szText);
			printf (szText);
			#endif
		}

		void Clear()
		{
			Destroy();
		}

		void Destroy()
		{
			#ifdef _DEBUG
			if (!m_kVct_pkData.empty())
			{
				char szText[256];
				sprintf (szText, "--------------------------------------------------------------------- %s Pool Destroy\n", typeid (T).name());
				OutputDebugString (szText);
				printf (szText);
			}
			#endif
			std::for_each (m_kVct_pkData.begin(), m_kVct_pkData.end(), Delete);
			m_kVct_pkData.clear();
			m_kVct_pkFree.clear();
		}

		void Create (UINT uCapacity)
		{
			m_uInitCapacity = uCapacity;
			m_kVct_pkData.reserve (uCapacity);
			m_kVct_pkFree.reserve (uCapacity);
		}
		T* Alloc()
		{
			if (m_kVct_pkFree.empty())
			{
				T* pkNewData = New();
				m_kVct_pkData.push_back (pkNewData);
				++m_uUsedCapacity;
				return pkNewData;
			}

			T* pkFreeData = m_kVct_pkFree.back();
			m_kVct_pkFree.pop_back();
			return pkFreeData;
		}
		void Free (T* pkData)
		{
			#ifdef DYNAMIC_POOL_STRICT
			assert (__IsValidData (pkData));
			assert (!__IsFreeData (pkData));
			#endif
			m_kVct_pkFree.push_back (pkData);
		}
		void FreeAll()
		{
			m_kVct_pkFree = m_kVct_pkData;
		}

		DWORD GetCapacity()
		{
			return m_kVct_pkData.size();
		}

	protected:
		bool __IsValidData (T* pkData)
		{
			if (m_kVct_pkData.end() == std::find (m_kVct_pkData.begin(), m_kVct_pkData.end(), pkData))
			{
				return false;
			}
			return true;
		}
		bool __IsFreeData (T* pkData)
		{
			if (m_kVct_pkFree.end() == std::find (m_kVct_pkFree.begin(), m_kVct_pkFree.end(), pkData))
			{
				return false;
			}

			return true;
		}

		static T* New()
		{
			return (T*)::operator new (sizeof (T));
		}
		static void Delete (T* pkData)
		{
			::operator delete (pkData);
		}

	protected:
		std::vector<T*> m_kVct_pkData;
		std::vector<T*> m_kVct_pkFree;

		UINT m_uInitCapacity;
		UINT m_uUsedCapacity;

};
#endif
template <class T>
class CPooledObject
{
	public:
		CPooledObject()
		{
		}
		virtual ~CPooledObject()
		{
		}

		template<typename... Args> static T* New(Args&&... args)
		{
#if defined(USE_NEW_POOLS)
			return ms_kPool.Construct(std::forward<Args>(args)...);
#else
			return ms_kPool.Alloc();
#endif
		}
		void* operator new(size_t)
		{
#if defined(USE_NEW_POOLS)
			return ms_kPool.Construct();
#else
			return ms_kPool.Alloc();
#endif
		}

		void operator delete (void* pT)
		{
#if defined(USE_NEW_POOLS)
			ms_kPool.Destruct(reinterpret_cast<T*>(pT));
#else
			ms_kPool.Free ((T*)pT);
#endif
		}


		static void DestroySystem()
		{
			ms_kPool.Destroy();
		}

		static void DeleteAll()
		{
			ms_kPool.FreeAll();
		}

	protected:
		static CDynamicPoolEx<T> ms_kPool;
};

template <class T> CDynamicPoolEx<T> CPooledObject<T>::ms_kPool;

/*

template <class T>
class CDynamicSizePool
{
#define GETPREVP(p) *(T**)((char*)p+sizeof(T))
#define GETNEXTP(p) *(T**)((char*)p+sizeof(T)+sizeof(T*))
    public:
        CDynamicSizePool()
        {
            Initialize();
        }

        virtual ~CDynamicSizePool()
        {
            Clear();
        }

        void Initialize()
        {
            m_nodes = NULL;
            m_nodeCount = 0;

            m_pFreeList = NULL;
            m_pUsedList = NULL;
        }

        void SetName(const char* c_szName)
        {
            m_stName = c_szName;
        }

        T* Alloc()
        {
            void* pnewNode;

            if (m_pFreeList)
            {
                pnewNode = m_pFreeList;
                m_pFreeList = GETNEXTP(m_pFreeList);
            }
            else
            {
                pnewNode = AllocNode();
            }

            if (!pnewNode)
                return NULL;

            if (!m_pUsedList)
            {
                m_pUsedList = pnewNode;
                GETPREVP(m_pUsedList) = NULL;
                GETNEXTP(m_pUsedList) = NULL;
            }
            else
            {
                GETPREVP(m_pUsedList) = (T*) pnewNode;
                GETNEXTP(pnewNode) = (T*) m_pUsedList;
                GETPREVP(pnewNode) = NULL;
                m_pUsedList = pnewNode;
            }
            //Tracef("%s Pool Alloc %p\n", m_stName.c_str(), pnewNode);
            return (T*) pnewNode;
        }

        void Free(T * pdata)
        {
            void* pfreeNode = (void*) pdata;

            if (pfreeNode == m_pUsedList)
            {
                if (NULL != (m_pUsedList = GETNEXTP(m_pUsedList)))
                    GETPREVP(m_pUsedList) = NULL;
            }
            else
            {
                if (GETNEXTP(pfreeNode))
                    GETPREVP(GETNEXTP(pfreeNode)) = GETPREVP(pfreeNode);

                if (GETPREVP(pfreeNode))
                    GETNEXTP(GETPREVP(pfreeNode)) = GETNEXTP(pfreeNode);
            }

            GETPREVP(pfreeNode) = NULL;
            GETNEXTP(pfreeNode) = (T*)m_pFreeList;
            m_pFreeList = pfreeNode;
            //Tracef("%s Pool Free\n", m_stName.c_str());
        }

        void FreeAll()
        {
            void * pcurNode;
            void * pnextNode;

            pcurNode = m_pUsedList;

            while (pcurNode)
            {
                pnextNode = GETNEXTP(pcurNode);
                Free(pcurNode);
                pcurNode = pnextNode;
            }
        }

        void Clear()
        {
            void* pcurNode;
            void* pnextNode;
            int count = 0;

            pcurNode = m_pFreeList;
            while (pcurNode)
            {
                pnextNode = GETNEXTP(pcurNode);
				((T*)pcurNode)->~T();
                ::operator delete(pcurNode);
                pcurNode = pnextNode;
                ++count;
            }
            m_pFreeList = NULL;

            pcurNode = m_pUsedList;
            while (pcurNode)
            {
                pnextNode = GETNEXTP(pcurNode);
				((T*)pcurNode)->~T();
                ::operator delete(pcurNode);
                pcurNode = pnextNode;
                ++count;
            }
            m_pUsedList = NULL;
            //Tracef("%s Pool Clear %d\n", m_stName.c_str(), count);
        }

    protected:
        void* AllocNode()
        {
            return ::operator new(sizeof(T)+sizeof(T*)*2);
        }

    protected:
        void *      m_nodes;
        void *      m_pFreeList;
        void *      m_pUsedList;

        int         m_nodeCount;
        std::string m_stName;
#undef GETNEXTP
#undef GETPREVP

};

template <class T>
class CPooledObject
{
    public:
		CPooledObject()
		{
		}
		virtual ~CPooledObject()
		{
		}

        void * operator new(unsigned int mem_size)
        {
            return ms_DynamicSizePool.Alloc();
        }

        void operator delete(void* pT)
        {
            ms_DynamicSizePool.Free((T*)pT);
        }

        static void SetPoolName(const char* szPoolName)
        {
            ms_DynamicSizePool.SetName(szPoolName);
        }

        static void ClearPool()
        {
            ms_DynamicSizePool.Clear();
        }

        static void FreePool()
        {
            ms_DynamicSizePool.FreeAll();
        }

    protected:
        static CDynamicSizePool<T> ms_DynamicSizePool;
};

template <class T> CDynamicSizePool<T> CPooledObject<T>::ms_DynamicSizePool;
*/

/*
template<typename T>
class CPoolNode : public T
{
	public:
		CPoolNode()
		{
			m_pNext = NULL;
			m_pPrev = NULL;
		}

		virtual ~CPoolNode()
		{
		}

	public:
		CPoolNode<T> * m_pNext;
		CPoolNode<T> * m_pPrev;
};

template<typename T>
class CDynamicPool
{
	public:
		typedef CPoolNode<T> TNode;

	public:
		CDynamicPool()
		{
			Initialize();
		}

		virtual ~CDynamicPool()
		{
			assert(m_pFreeList==NULL && "CDynamicPool::~CDynamicPool() - NOT Clear");
			assert(m_pUsedList==NULL && "CDynamicPool::~CDynamicPool() - NOT Clear");
			Clear();
		}

		void Initialize()
		{
			m_nodes = NULL;
			m_nodeCount = 0;

			m_pFreeList = NULL;
			m_pUsedList = NULL;
		}

		void SetName(const char* c_szName)
		{
			m_stName = c_szName;
		}

		DWORD GetCapacity()
		{
			return m_nodeCount;
		}

		T* Alloc()
		{
			TNode* pnewNode;

			if (m_pFreeList)
			{
				pnewNode = m_pFreeList;
				m_pFreeList = m_pFreeList->m_pNext;
			}
			else
			{
				pnewNode = AllocNode();
			}

			if (!pnewNode)
				return NULL;

			if (!m_pUsedList)
			{
				m_pUsedList = pnewNode;
				m_pUsedList->m_pPrev = m_pUsedList->m_pNext = NULL;
			}
			else
			{
				m_pUsedList->m_pPrev = pnewNode;
				pnewNode->m_pNext = m_pUsedList;
				pnewNode->m_pPrev = NULL;
				m_pUsedList = pnewNode;
			}
			//Tracef("%s Pool Alloc %p\n", m_stName.c_str(), pnewNode);
			return (T*) pnewNode;
		}

		bool IsUsedData(T* pdata)
		{
			TNode* pchkNode=(TNode*)pdata;
			TNode* pcurNode = m_pUsedList;
			while (pcurNode)
			{
				if (pcurNode==pdata)
					return true;

				pcurNode = pcurNode->m_pNext;
			}
			return false;
		}

		bool IsFreeData(T* pdata)
		{
			TNode* pchkNode=(TNode*)pdata;
			TNode* pcurNode = m_pFreeList;
			while (pcurNode)
			{
				if (pcurNode==pdata)
					return true;

				pcurNode = pcurNode->m_pNext;
			}
			return false;
		}

		void Free(T * pdata)
		{
			assert(IsUsedData(pdata));
			assert(!IsFreeData(pdata));

			TNode* pfreeNode = (TNode*) pdata;

			if (pfreeNode == m_pUsedList)
			{
				m_pUsedList = m_pUsedList->m_pNext;
				if (NULL != m_pUsedList)
					m_pUsedList->m_pPrev = NULL;
			}
			else
			{
				if (pfreeNode->m_pNext)
					pfreeNode->m_pNext->m_pPrev = pfreeNode->m_pPrev;

				if (pfreeNode->m_pPrev)
					pfreeNode->m_pPrev->m_pNext = pfreeNode->m_pNext;
			}

			pfreeNode->m_pPrev = NULL;
			pfreeNode->m_pNext = m_pFreeList;
			m_pFreeList = pfreeNode;
			//Tracef("%s Pool Free\n", m_stName.c_str());
		}

		void FreeAll()
		{
			TNode * pcurNode;
			TNode * pnextNode;

			pcurNode = m_pUsedList;

			while (pcurNode)
			{
				pnextNode = pcurNode->m_pNext;
				Free(pcurNode);
				pcurNode = pnextNode;
			}

			assert(NULL==m_pUsedList);
		}

		void Clear()
		{
			TNode* pcurNode;
			TNode* pnextNode;

			DWORD count = 0;

			pcurNode = m_pFreeList;
			while (pcurNode)
			{
				pnextNode = pcurNode->m_pNext;
				delete pcurNode;
				pcurNode = pnextNode;
				++count;
			}
			m_pFreeList = NULL;

			pcurNode = m_pUsedList;
			while (pcurNode)
			{
				pnextNode = pcurNode->m_pNext;
				delete pcurNode;
				pcurNode = pnextNode;
				++count;
			}

			m_pUsedList = NULL;

			assert(count==m_nodeCount && "CDynamicPool::Clear()");

			m_nodeCount=0;
		}

	protected:
		TNode* AllocNode()
		{
			++m_nodeCount;
			return new TNode;
		}

	protected:
		TNode *		m_nodes;
		TNode *		m_pFreeList;
		TNode *		m_pUsedList;

		DWORD		m_nodeCount;
		std::string	m_stName;
};
*/