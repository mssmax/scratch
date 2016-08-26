#pragma once
#include "stdafx.h"
#include "resource.h"
#include "mtaconfig.h"
#include "UploadThread.h"
#include "mfg.h"
#include "mocks.h"
#include "CUtilities_mock.h"

//====== Matchers ======

MATCHER_P(VariantBSTREq, wcs, "") {
	return ((arg.vt & VT_TYPEMASK) == VT_BSTR) && (wcscmp(arg.bstrVal, wcs)==0);
}

MATCHER_P(BSTREq, wcs, "") {
	return (wcs==NULL || arg==NULL) ? wcs==arg : (wcscmp(arg, wcs)==0);
}

MATCHER_P(BSTRHasSubstr, wcs, "") {
	return (wcs==NULL || arg==NULL) ? false : (wcsstr(arg, wcs)!=NULL);
}


//====== Actions ======

ACTION_P(CopyWCStringToArg0AndReturnTRUE, str) {
	wcscpy(arg0, str);
	return TRUE;
}

ACTION_P(CopyStringToArg0AndReturnTRUE, str) {
	strcpy(arg0, str);
	return TRUE;
}

ACTION_P(SetLastErrorAndReturnFALSE, err) {
	::SetLastError(err);
	return FALSE;
}

ACTION_P(ReturnFalseIfArg0Is, s)
{
	return (strcmp(s, arg0)!=0);
}

ACTION_P2(IncreaseMessageIndexBy, pointerToCUploadThreadMiddleLevTestableInst, value) {
	pointerToCUploadThreadMiddleLevTestableInst->IncreaseMessageIndexBy(value);
}


//====== Fakes ======

//class CUploadThreadXMLAPITestable : public CUploadThread
//{
//public:
//	testing::NiceMock<IXMLDOMDocument2_Mock> m_xmlDoc;
//	testing::NiceMock<IXMLDOMElement_Mock> m_xmlDocElement;
//
//	CUploadThreadXMLAPITestable(IMtaConfig *pMtaConfig, IUtilities *pUtil, IStatusInterface* pSi, IEmailMessageEx *pMsg)
//		: CUploadThread(pMtaConfig, pUtil, pSi, pMsg)
//	{
//		m_sXMLAPIFolder="\\xmlapi";
//		ON_CALL(m_xmlDoc, get_documentElement(_)).WillByDefault(testing::DoAll(testing::SetArgPointee<0>(&m_xmlDocElement),testing::Return(S_OK)));
//	}
//	virtual ~CUploadThreadXMLAPITestable() { }
//
//	HRESULT CreateXMLDOMDocument(MSXML2::IXMLDOMDocument **ppDoc)
//	{
//		*ppDoc=&m_xmlDoc;
//		return S_OK;
//	}
//};

class CUploadThreadMiddleLevTestable : public CUploadThread
{
	int m_ffmIndex;
public:
	CUploadThreadMiddleLevTestable(IMtaConfig *pMtaConfig, IUtilities *pUtil, IStatusInterface* pSi, IEmailMessageEx *pMsg)
		: CUploadThread(pMtaConfig, pUtil, pSi, pMsg), m_ffmIndex(0)
	{
		sfinddir_emailqueue="\\fm\\in\\emailqueue\\";

		ON_CALL(*this, ProcessFaxFrmtMessage(_)).WillByDefault(Invoke(this, &CUploadThreadMiddleLevTestable::ProcessFaxFrmtMessageFake));
	}

	virtual ~CUploadThreadMiddleLevTestable() {
	}
	//MOCK_METHOD5(ConstructEmail, strw_c(LPCWSTR lpszalias, LPCWSTR lpszaddress, LPCWSTR lpszdomain/* = NULL*/, BOOL bfilter/* = TRUE*/, str_c *saddress/*=NULL*/));
	MOCK_METHOD1(ProcessFaxFrmtMessage, void(IGenericFaxFrmtMessage *pfm));
	MOCK_METHOD1(ProcessXMLAPI, void(IGenericFaxFrmtMessage *pfm));

	//fill queue with fake items so that the following email queue processing can be tested
	void ProcessFaxFrmtMessageFake(IGenericFaxFrmtMessage *pfm) {
		mailqueueItem_s mqi;
		str_c sPath;
		sPath.format("\\fm\\in\\%d.sfm", ++m_ffmIndex);
		AddMailQueueItem(sPath/*pfm->GetFilePath()*/, mqi);
	}

	void IncreaseMessageIndexBy(int value) {
		m_messageIndex+=value;
	}
};

//====== Fixtures ======

template<class T>
class CUploadThread_fxt_base: public mfgfixture
{
public:
	testing::NiceMock<T> m_ut;
	testing::NiceMock<CMtaConfigMock> m_config;
	testing::NiceMock<CEmailMessageExMock> m_msg;
	testing::NiceMock<SmtpSenderMock> m_smtp;
	testing::NiceMock<CStatusInterfaceMock> m_si;
	testing::NiceMock<CFaxFrmtMessage_Mock> m_ffm;
	testing::NiceMock<CUtilities_Mock> m_util;

	CUploadThread_fxt_base() : m_ut(&m_config, &m_util, &m_si, &m_msg) {
		SetSmtpSenderMock(&m_smtp);
		SetFaxFrmtMsgMock(&m_ffm);
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}
};

class CUploadThread_fxt : public CUploadThread_fxt_base<CUploadThread> {};
//class CUploadThreadXMLAPITest : public CUploadThread_fxt_base<CUploadThreadXMLAPITestable> {};
class CUploadThreadMiddleLevTest : public CUploadThread_fxt_base<CUploadThreadMiddleLevTestable>
{
public:
	testing::NiceMock<dirfilelist_c_mock> m_dfl;

	CUploadThreadMiddleLevTest()
	{
		SetDirFileListMock(&m_dfl);
	}

	void ExpectDirFileListCallsSequence(int count)
	{
		testing::InSequence s;
		EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));
		for (int i=1; i<=count; i++)
		{
			str_c sPath;
			sPath.format("\\fm\\in\\%d.sfm", i);
			EXPECT_CALL(m_dfl, getcurrentfilepath(_, _)).WillOnce(CopyStringToArg0AndReturnTRUE((LPCSTR)sPath));
			EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(i<count ? TRUE : FALSE));
		}
	}
};
