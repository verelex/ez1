#pragma once
#include "quad_template.h"

class CWorkContext
{
	public:
		// ������:
		CTestItemList *m_pTIL; // �� ������ �������� � �������
		int m_BiletNumber;
		int m_mode; // ����� ������: 0 = ��������������, 1 = ������������
		int m_ErrorsCount, m_ErrorsOK;
		ULONG m_Date;
		CString m_UserCompany, m_KursName, m_DateTime, m_Time, m_UserName,
				m_UserJobPos, m_Res, m_AdminName, m_AdminJobPos;

		// ������
		void DoSomething();
};
