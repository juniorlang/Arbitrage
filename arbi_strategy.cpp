#include "arbi_strategy.h"
#include "../tables_ext/schemeinfo_ext.h"
#include "../tables_ext/ft_stkinfo_ext.h"
#include "../public_arbi/arbi_generalfunc.h"

// ���ػ���
void CStrategyBase::LoadOpportunitys()
{
	tagOpportunity stOpportunity;

	// ������ֵ1
	stOpportunity.m_dbThreshold = m_objTaskParam.m_dbOpenvalue1;
	stOpportunity.m_dbStkQty = m_objTaskParam.m_dbOpenqty1;
	m_vectOpportunitys.push_back(stOpportunity);

	// ������ֵ2
	stOpportunity.m_dbThreshold = m_objTaskParam.m_dbOpenvalue2;
	stOpportunity.m_dbStkQty = m_objTaskParam.m_dbOpenqty2;
	m_vectOpportunitys.push_back(stOpportunity);

	// ������ֵ3
	stOpportunity.m_dbThreshold = m_objTaskParam.m_dbOpenvalue3;
	stOpportunity.m_dbStkQty = m_objTaskParam.m_dbOpenqty3;
	m_vectOpportunitys.push_back(stOpportunity);

	// ������ֵ4
	stOpportunity.m_dbThreshold = m_objTaskParam.m_dbOpenvalue4;
	stOpportunity.m_dbStkQty = m_objTaskParam.m_dbOpenqty4;
	m_vectOpportunitys.push_back(stOpportunity);
}

// �Ƿ��п�����������(�������)
bool CStrategyBase::ExistOpportunity(kdt_stkqty& dbQty, kdt_price& dbCurrValue)
{
	// ȡ��ǰʱ���������ֵ
	if (!GetCurrOpportunityValue(dbCurrValue))
		return false;

	// �������л�����ֵ
	LoadOpportunitys();

	// �������趨˳������ж�
	for (int i = 0; i < m_objTaskParam.m_nOpptavlcount; i++)
	{
		if (Compare(dbCurrValue, m_vectOpportunitys[i].m_dbThreshold))
		{
			dbQty = m_vectOpportunitys[i].m_dbStkQty;
			return true;
		}
	}

	return false;
}

// ���ᴦ��ʽ�����ѷ�ʽ-�������沢����
bool CStrategyBase::CalPubProfitInfo(const kdt_stkqty dbQty)
{
	return true;
}

// �Ƿ���Ҫֹ��
bool CStrategyBase::NeedStopLoss(const kdt_price dbRealDiff, kdt_price& dbOpenCostDiff)
{
	return false;
}

// ��֤����۸�
bool CStrategyBase::IsValidQuotPrice(kdt_price dbPrice, const TQuotInfo& stQuot)
{
	return (FBG(stQuot.dbMaxDownValue, 0.0) &&
			FBG(stQuot.dbMaxRiseValue, 0.0) &&
			!FLS(dbPrice, stQuot.dbMaxDownValue) &&
			!FBG(dbPrice, stQuot.dbMaxRiseValue));
}

// �����½�������Ϣ
void CStrategyBase::SetNewSchemeInfo()
{
	CKpmsSno	objKpmsSno(this);

	m_objSchemeInfo.m_nArbitype			= m_objTask.m_nArbitype; 
	m_objSchemeInfo.m_nSchemeid			= objKpmsSno.GetGeneralSno(SNO_TYPE_ALGORSCHEMEINFO);
	m_objSchemeInfo.m_nProjectid		= m_objTask.m_nProjectid;
	m_objSchemeInfo.m_nCombid			= m_objTask.m_nCombid;
	m_objSchemeInfo.m_cHedgeflag		= m_objTaskParam.m_cHedgeflag;
	m_objSchemeInfo.m_cArbidirection	= m_objTaskParam.m_cArbidirection;	
	m_objSchemeInfo.m_nTaskid			= m_objTask.m_nTaskid; 
	m_objSchemeInfo.m_nStatus			= SCHEMESTATUS_STARTING;
	m_objSchemeInfo.m_nCreateuserid 	= m_objTask.m_nUserid;
	m_objSchemeInfo.m_nCreatedate		= m_objSysConfig.m_nBusidate;
	m_objSchemeInfo.m_nCreatetime		= m_objSysConfig.GetPhyTime();
	m_objSchemeInfo.m_cLockflag			= SCHEME_UNLOCK;
	m_objSchemeInfo.m_nLockdate			= 0;
	m_objSchemeInfo.m_nLocktime			= 0;
	m_objSchemeInfo.m_nLocktimeout		= 0;
	m_objSchemeInfo.m_cManualflag		= SCHEME_MANUAL_NO;
	m_objSchemeInfo.m_cSrcmarket		= m_objTaskParam.m_cSrcmarket;
	m_objSchemeInfo.m_cDestmarket		= m_objTaskParam.m_cDestmarket;
	KCPD_STRCPY_S(m_objSchemeInfo.m_szSrccode, sizeof(m_objSchemeInfo.m_szSrccode), m_objTaskParam.m_szSrccode);
	KCPD_STRCPY_S(m_objSchemeInfo.m_szDestcode, sizeof(m_objSchemeInfo.m_szDestcode), m_objTaskParam.m_szDestcode);
	KCPD_STRCPY_S(m_objSchemeInfo.m_szSrcid, sizeof(m_objSchemeInfo.m_szSrcid), m_objTaskParam.m_szSrcid);	
	KCPD_STRCPY_S(m_objSchemeInfo.m_szDestid, sizeof(m_objSchemeInfo.m_szSrcid), m_objTaskParam.m_szDestid);
	KCPD_STRCPY_S(m_objSchemeInfo.m_szSrcopenpricestrg, sizeof(m_objSchemeInfo.m_szSrcopenpricestrg), m_objTaskParam.m_szSrcsalepricestrg);
	KCPD_STRCPY_S(m_objSchemeInfo.m_szSrcclosepricestrg, sizeof(m_objSchemeInfo.m_szSrcclosepricestrg), m_objTaskParam.m_szSrcbuypricestrg);
	KCPD_STRCPY_S(m_objSchemeInfo.m_szDestopenpricestrg, sizeof(m_objSchemeInfo.m_szDestopenpricestrg), m_objTaskParam.m_szDestbuypricestrg);
	KCPD_STRCPY_S(m_objSchemeInfo.m_szDestclosepricestrg, sizeof(m_objSchemeInfo.m_szDestclosepricestrg), m_objTaskParam.m_szDestsalepricestrg);
	KCPD_STRCPY_S(m_objSchemeInfo.m_szDescribe, sizeof(m_objSchemeInfo.m_szDescribe), "�Զ�������񴴽�");
}

// ����������������Ϣ
void CStrategyBase::AddSchemeBaseInfo(const kdt_stkqty dbQty)
{
	SetNewSchemeInfo();
	m_objSchemeInfo.m_dbQty		= dbQty;
	m_objSchemeInfo.m_dbSrcqty	= dbQty;
	m_objSchemeInfo.m_dbDestqty	= dbQty;
	m_objSchemeInfo.Insert();
}


// ��������������Ϣ
void CStrategyBase::AddSchemeRelateInfo()
{
	kpms_stream		strm(m_pBusinessBase);
	CSchemeinfoExt	objSchemeRelateInfoExt(m_pBusinessBase);

	objSchemeRelateInfoExt.m_nSchemeid		= m_objSchemeInfo.m_nSchemeid;
	objSchemeRelateInfoExt.m_nProjectid		= m_objSchemeInfo.m_nProjectid;
	objSchemeRelateInfoExt.m_nCombid		= m_objSchemeInfo.m_nCombid;
	objSchemeRelateInfoExt.m_nCreateoperid	= m_objSchemeInfo.m_nCreateuserid;
	objSchemeRelateInfoExt.m_nLastdate		= m_objSchemeInfo.m_nCreatedate;

	if (CArbiGeneralFunc::GetFundIdByProject(strm, m_objSchemeInfo.m_nProjectid, objSchemeRelateInfoExt.m_nFundid) != 0)
		BusRaiseException(836320055, 1, "��ѯ�ʲ���Ԫ[%d]��Ӧ�Ĳ�Ʒ��Ϣʧ��", m_objSchemeInfo.m_nProjectid);

	objSchemeRelateInfoExt.Insert();
}

// ���ӷ�������
void CStrategyBase::AddSchemeParam()
{
	CKpmsSno    objKpmsSno(m_pBusinessBase);

	m_objTaskParam.m_nParamid = objKpmsSno.GetGeneralSno(SNO_CS_PARAM);
	m_objTaskParam.m_nSchemeid = m_objSchemeInfo.m_nSchemeid;

	m_objTaskParam.Insert();
}


// ��������
void CStrategyBase::CreateScheme(const kdt_stkqty dbQty)
{
	// ����������������Ϣ
	AddSchemeBaseInfo(dbQty);

	// ��������������Ϣ
	AddSchemeRelateInfo();

	// ���ӷ�������
	AddSchemeParam();
}