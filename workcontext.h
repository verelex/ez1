#pragma once
#include "quad_template.h"

class CWorkContext
{
	public:
		// данные:
		CTestItemList *m_pTIL; // на список вопросов и ответов
		int m_BiletNumber;
		int m_mode; // Режим работы: 0 = самоподготовка, 1 = тестирование
		int m_ErrorsCount, m_ErrorsOK;
		ULONG m_Date;
		CString m_UserCompany, m_KursName, m_DateTime, m_Time, m_UserName,
				m_UserJobPos, m_Res, m_AdminName, m_AdminJobPos;

		// методы
		void DoSomething();
};
