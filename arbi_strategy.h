/******************************************************************************
*                                                                              
*                            Ͷ�ʽ���ϵͳ(KPMS) V2.0                           
*                                                                              
*           Copyright (C) 2009-2010, SHENZHEN KINGDOM Co., Ltd.                
*                                                                              
*                             All Rights Reserved.                             
*                                                                              
* ===========================================================================  
*
* �� �� ����arbi_strategy.h
* ģ�����ƣ�[�������Խӿ���]
* ����˵����[�������������и��ֲ��ԵĻ����жϡ�ί�г�����..]
* ��    �ߣ�
* �������ڣ�2013-06-04 09:13:02
* �� �� �ţ�2.0
* �޸���ʷ��
*
* �޸�����       ����            ����
* ---------------------------------------------------------------------------  
* 2013-06-04    liangj          ��ʼ�汾
******************************************************************************/


#ifndef __ARBI_STRATEGY_H__
#define __ARBI_STRATEGY_H__

#include "../comm/kpms_head.h"
#include "../tables_ext/a_task_ext.h"
#include "../tables_ext/a_taskparam_ext.h"
#include "../tables_ext/a_schemeinfo_ext.h"
#include "../public_arbi/arbi_quot.h"
#include "../public_arbi/arbi_invest_trade.h"


struct tagOpportunity
{
	kdt_price	m_dbThreshold;	// ��ֵ
	kdt_stkqty	m_dbStkQty;		// ����
};

// --[�������Ի���]
class CStrategyBase : public CKpmsBase
{
public:
	CStrategyBase(CKpmsBusiness* pBusinessBase,
					CA_taskExt& objTaskExt,
					CA_taskparamExt& objTaskParamExt,
					CA_schemeinfoExt& objSchemeInfoExt,
					CArbiParamValue& objUserParam,
					CSysconfigExt& objSysconfigExt) : CKpmsBase(pBusinessBase),
		m_pBusinessBase(pBusinessBase),
		m_objTask(objTaskExt),
		m_objTaskParam(objTaskParamExt),
		m_objSchemeInfo(objSchemeInfoExt),
		m_objQuotMap(pBusinessBase),
		m_objUserParam(objUserParam),
		m_objSysConfig(objSysconfigExt)
	{
		m_szSrcBsflag[0] = 0;
		m_szDestBsflag[0] = 0;
	}
	
public:

	// �����������
	virtual void CheckBeforeRun() {};

	// ��������ǰ���
	virtual void CheckBeforeStart() {};

	// �½����޸ġ�ɾ������ǰ���
	virtual void CheckBeforeManage(char cAction) {};

	// �Ƿ�����������(�������)
	virtual bool ExistOpportunity(kdt_stkqty& dbQty, kdt_price& dbCurrValue);

	// ���ᴦ��ʽ�����ѷ�ʽ-�������沢����
	virtual bool CalPubProfitInfo(const kdt_stkqty dbQty);

	// �Ƿ���Ҫֹ��
	virtual bool NeedStopLoss(const kdt_price dbRealDiff, kdt_price& dbOpenCostDiff);

	// ����
	virtual bool Trade() = 0;

	// ȡ��ǰ�����������ֵ
	virtual bool GetCurrOpportunityValue(double& dbCurrValue) = 0;

	// ��������
	void CreateScheme(const kdt_stkqty dbQty);

protected:

	// �Ƚ���ֵ�ж��Ƿ�����������
	virtual bool Compare(kdt_price dbCurrValue, kdt_price dbThresholdParam) = 0;

	// ���ػ���
	virtual void LoadOpportunitys();

	// ��֤����۸�
	bool IsValidQuotPrice(kdt_price dbPrice, const TQuotInfo& stQuot);

	// �����½�������Ϣ
	void SetNewSchemeInfo();

	// ����������������Ϣ
	virtual void AddSchemeBaseInfo(const kdt_stkqty dbQty);

	// ��������������Ϣ
	void AddSchemeRelateInfo();

	// ���ӷ�������
	void AddSchemeParam();

public:
	CKpmsBusiness*			m_pBusinessBase;
	CA_taskparamExt			m_objTaskParam;
	CA_taskExt				m_objTask;
	CA_schemeinfoExt		m_objSchemeInfo;

	CSysconfigExt			m_objSysConfig;
	vector<tagOpportunity>	m_vectOpportunitys;

	kdt_char2				m_szSrcBsflag;		// �����������
	kdt_char2				m_szDestBsflag;		// Զ���������
	CArbiQuotMap			m_objQuotMap;
	CArbiParamValue			m_objUserParam;		// �û�ͨ�ò���
};

#endif