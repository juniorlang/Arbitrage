/******************************************************************************
*                                                                              
*                            投资交易系统(KPMS) V2.0                           
*                                                                              
*           Copyright (C) 2009-2010, SHENZHEN KINGDOM Co., Ltd.                
*                                                                              
*                             All Rights Reserved.                             
*                                                                              
* ===========================================================================  
*
* 文 件 名：arbi_strategy.h
* 模块名称：[套利策略接口类]
* 功能说明：[处理套利任务中各种策略的机会判断、委托撤补等..]
* 作    者：
* 创建日期：2013-06-04 09:13:02
* 版 本 号：2.0
* 修改历史：
*
* 修改日期       姓名            内容
* ---------------------------------------------------------------------------  
* 2013-06-04    liangj          初始版本
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
	kdt_price	m_dbThreshold;	// 阈值
	kdt_stkqty	m_dbStkQty;		// 数量
};

// --[套利策略基类]
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

	// 检查套利任务
	virtual void CheckBeforeRun() {};

	// 启动任务前检查
	virtual void CheckBeforeStart() {};

	// 新建、修改、删除任务前检查
	virtual void CheckBeforeManage(char cAction) {};

	// 是否有套利机会(多个参数)
	virtual bool ExistOpportunity(kdt_stkqty& dbQty, kdt_price& dbCurrValue);

	// 机会处理方式：提醒方式-计算收益并发布
	virtual bool CalPubProfitInfo(const kdt_stkqty dbQty);

	// 是否需要止损
	virtual bool NeedStopLoss(const kdt_price dbRealDiff, kdt_price& dbOpenCostDiff);

	// 交易
	virtual bool Trade() = 0;

	// 取当前机会行情计算值
	virtual bool GetCurrOpportunityValue(double& dbCurrValue) = 0;

	// 创建方案
	void CreateScheme(const kdt_stkqty dbQty);

protected:

	// 比较阈值判断是否有套利机会
	virtual bool Compare(kdt_price dbCurrValue, kdt_price dbThresholdParam) = 0;

	// 加载机会
	virtual void LoadOpportunitys();

	// 验证行情价格
	bool IsValidQuotPrice(kdt_price dbPrice, const TQuotInfo& stQuot);

	// 设置新建方案信息
	void SetNewSchemeInfo();

	// 新增方案基本表信息
	virtual void AddSchemeBaseInfo(const kdt_stkqty dbQty);

	// 新增方案关联信息
	void AddSchemeRelateInfo();

	// 增加方案参数
	void AddSchemeParam();

public:
	CKpmsBusiness*			m_pBusinessBase;
	CA_taskparamExt			m_objTaskParam;
	CA_taskExt				m_objTask;
	CA_schemeinfoExt		m_objSchemeInfo;

	CSysconfigExt			m_objSysConfig;
	vector<tagOpportunity>	m_vectOpportunitys;

	kdt_char2				m_szSrcBsflag;		// 近期买卖类别
	kdt_char2				m_szDestBsflag;		// 远期买卖类别
	CArbiQuotMap			m_objQuotMap;
	CArbiParamValue			m_objUserParam;		// 用户通用参数
};

#endif