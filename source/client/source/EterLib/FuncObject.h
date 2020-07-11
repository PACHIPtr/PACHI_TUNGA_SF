#pragma once

template<typename T>
class CFuncObject
{
	public:
		CFuncObject()
		{
			Clear();
		}

		virtual ~CFuncObject()
		{
		}

		void Clear()
		{
			m_pSelfObject = nullptr;
			m_pFuncObject = nullptr;
		}

		void Set(T* pSelfObject, void (T::*pFuncObject)())
		{
			m_pSelfObject = pSelfObject;
			m_pFuncObject = pFuncObject;
		}

		bool IsEmpty()
		{
			if (m_pSelfObject != nullptr)
				return false;

			if (m_pFuncObject != nullptr)
				return false;

			return true;
		}

		void Run()
		{
			if (m_pSelfObject)
				if (m_pFuncObject)
					(m_pSelfObject->*m_pFuncObject)();
		}

	protected:
		T *	m_pSelfObject;
		void (T::*m_pFuncObject) ();
};
