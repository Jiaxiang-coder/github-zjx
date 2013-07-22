
// PublishAide_1View.cpp : implementation of the CPublishAide_1View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PublishAide_1View.h"
#endif
#include <afxdisp.h>
#include <mshtml.h>
#include <afxinet.h>
//#include <mshtml.idl>
#include <atlbase.h>
#include <oleacc.h>
#include <string.h>
CComModule _Module;
#include <atlcom.h>
#include <wininet.h>

#include "PublishAide_1Doc.h"
#include "PublishAide_1View.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

BOOL CALLBACK EnumChildProc(HWND hwnd,LPARAM lParam)
{
	TCHAR	buf[100];

	::GetClassName( hwnd, (LPTSTR)&buf, 100 );
	if ( _tcscmp( buf, _T("Internet Explorer_Server") ) == 0 )
	{
		*(HWND*)lParam = hwnd;
		return FALSE;
	}
	else
		return TRUE;
};

IMPLEMENT_DYNCREATE(CPublishAide_1View, CHtmlView)

BEGIN_MESSAGE_MAP(CPublishAide_1View, CHtmlView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CPublishAide_1View construction/destruction

CPublishAide_1View::CPublishAide_1View()
{
	// TODO: add construction code here

}

CPublishAide_1View::~CPublishAide_1View()
{
}

BOOL CPublishAide_1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CHtmlView::PreCreateWindow(cs);
}

void CPublishAide_1View::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	Navigate2(_T("https://login.taobao.com/member/login.jhtml"),NULL,NULL);
}


// CPublishAide_1View printing


void CPublishAide_1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}


void CPublishAide_1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPublishAide_1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPublishAide_1View diagnostics

#ifdef _DEBUG
void CPublishAide_1View::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CPublishAide_1View::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CPublishAide_1Doc* CPublishAide_1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPublishAide_1Doc)));
	return (CPublishAide_1Doc*)m_pDocument;
}
#endif //_DEBUG
  

// CPublishAide_1View message handlers



void CPublishAide_1View::OnDocumentComplete(LPCTSTR lpszURL)
{
	// TODO: Add your specialized code here and/or call the base class
	LPDISPATCH lpDisp= GetHtmlDocument();
	CComPtr<IHTMLDocument2> cpDoc2;
	CComPtr<IHTMLDocument3> cpDoc3;
	CComPtr<IHTMLLocation> cpLoc;
	HRESULT hres;
	CComBSTR b1,b2,s3,s4,s5;
	//++m_nCount;
	hres= lpDisp->QueryInterface(IID_IHTMLDocument2,(void**)&cpDoc2);
	if(SUCCEEDED(hres)&&cpDoc2)
	{
		hres= cpDoc2->get_location(&cpLoc);
		if(SUCCEEDED(hres)&&cpLoc)
		{
			hres=cpLoc->get_href(&b1.m_str);		//obtain current href
			if(SUCCEEDED(hres)&&b1.m_str)
			{
			/*	b1.Append(L"\r\n");

				CFile f1(L"ffhello.txt",CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate);
				f1.SeekToEnd();
				f1.Write(b1.m_str,b1.Length()*2);
				f1.Close();*/

				b2.Attach(wcsstr(b1.m_str,L"https://login.taobao.com/member/login.jhtml"));
				if(b2.m_str)					//current href has substring https://login.taobao.com/member/login.jhtml invoke LoginFunction()
				{
					LoginFunction();			//自动登录
					b2.Detach();				//释放字符串
					goto L1;
				}
				b2.Attach(wcsstr(b1.m_str,L"http://i.taobao.com/my_taobao.htm"));
				if(b2.m_str)
				{
					//Navigate2(L"http://mai.taobao.com/seller_admin.htm");	
					s3.Attach(L"http://mai.taobao.com/seller_admin.htm");  //href 内的子字符串
					s4.Attach(L"site-nav-bd");		//最近的id
					GotoAnchor(&s4.m_str,&s3.m_str);
					s3.Detach();
					s4.Detach();
					b2.Detach();
					goto L1;
				}
				b2.Attach(wcsstr(b1.m_str,L"http://mai.taobao.com/seller_admin.htm"));
				if(b2.m_str)
				{
					s3.Attach(L"http://upload.taobao.com/auction/sell.jhtml");	//发布商品
					s4.Attach(L"J_SelectMenu");
					GotoAnchor(&s4.m_str,&s3.m_str);
					s3.Detach();
					s4.Detach();/**/
					goto L1;
				}
				b2.Attach(wcsstr(b1.m_str,L"http://upload.taobao.com/auction/sell.jhtml"));
				if(b2.m_str)
				{
					
						AuctionCategorySelect();	//自动选择
						goto L1;
				}
				//b2.Attach(wcsstr(b1.m_str,L"http://upload.taobao.com/auction/publish/publish.htm"));
				//if(b2.m_str)
				//{
			
			}

		}
	}
	
L1:	CHtmlView::OnDocumentComplete(lpszURL);
}

int CPublishAide_1View::GotoAnchor(BSTR* sNearlyID,BSTR* sLinkURL)
{
	LPDISPATCH lpDisp=GetHtmlDocument();
	CComPtr<IHTMLDocument3> cpDoc3;
	CComPtr<IHTMLElement> cpElement;
	CComPtr<IHTMLAnchorElement> cpAncEle;
	CComPtr<IHTMLElementCollection> cpElemCol;
	HRESULT hres;
	CComBSTR bs1(*sNearlyID);
	CComBSTR bs2;
	hres=lpDisp->QueryInterface(IID_IHTMLDocument3,(void**)&cpDoc3);

	if(SUCCEEDED(hres)&&cpDoc3)
	{
		hres = cpDoc3->getElementById(*sNearlyID,&cpElement);	//用ID取得离目标 href 最近的元素
			if(SUCCEEDED(hres)&&cpElement)
			{
				cpElement->get_all((IDispatch**)&cpElemCol);	//Retrieves the collection of all the objects in the HTML that are within the scope of this object. 
				long len;
				cpElemCol->get_length(&len);	// length of elememts collection
				for(long i=0;i<=len;++i)				//ergodic
				{
					hres=cpElemCol->item(CComVariant(i),CComVariant(i),(IDispatch**)&cpElement);
					if(SUCCEEDED(hres)&&cpElement)
					{
						hres=cpElement->QueryInterface(IID_IHTMLAnchorElement,(void**)&cpAncEle);	//if this element is a IHTMLAnchorElement
						if(SUCCEEDED(hres)&&cpAncEle)
						{
							hres=cpAncEle->get_href(&bs1.m_str);	//get href of the element
							if(!bs1.m_str)continue;
							if(SUCCEEDED(hres))
							{
								bs2.Attach(wcsstr(bs1.m_str,*sLinkURL));	//if target href if a substring of href of current element 
								if(bs2.m_str==0)
									continue;
								bs1.Detach();
								bs1.Attach(L"_self");			// 加入target值，在当前窗口打开网页
								cpAncEle->put_target(bs1.m_str);
								bs1.Detach();
								hres=cpAncEle->QueryInterface(IID_IHTMLElement,(void**)&cpElement);		//obtain pointer of IHTMLElement interface 
								if(SUCCEEDED(hres)&&cpElement)
								cpElement->click();
								/*strref=bs1.m_str;
								Navigate2(strref,NULL,NULL);*/
								break;
							}
						}
					}
				}
			}
	}
	return 0;
}

BOOL CPublishAide_1View::LoginFunction()
{
	LPDISPATCH phDoc= GetHtmlDocument();
	CComPtr<IHTMLDocument3> copDoc3;
	phDoc->QueryInterface(IID_IHTMLDocument3,(void**)&copDoc3);
	CComPtr<IHTMLInputElement> userName,passWord,button,checkbox;
	CComPtr<IHTMLElement> htmptr2;
	CComPtr<IHTMLAnchorElement> cpAE;

	HRESULT hres;
	CComBSTR obnm,obval;
	VARIANT_BOOL p1;
	obnm.Attach(L"J_SafeLoginCheck");		
	hres=copDoc3->getElementById(obnm,&htmptr2);
	if(!SUCCEEDED(hres)|!htmptr2)return 0;
	hres=htmptr2->QueryInterface(IID_IHTMLInputElement,(void**)&checkbox);
	checkbox->get_checked(&p1);
	if(p1!=0)
	htmptr2->click();
	obnm.Detach();

	obnm.Attach(L"TPL_username_1");
	obval.Attach(L"18711968756");
	copDoc3->getElementById(obnm,&htmptr2);
	htmptr2->QueryInterface(IID_IHTMLInputElement,(void**)&userName);
	userName->put_value(obval);
	obnm.Detach();
	obval.Detach();

	obnm.Attach(L"TPL_password_1");
	obval.Attach(L"zjxxs.8616");
	copDoc3->getElementById(obnm,&htmptr2);
	htmptr2->QueryInterface(IID_IHTMLInputElement,(void**)&passWord);
	passWord->put_value(obval);
	obnm.Detach();
	obval.Detach();

	obnm.Attach(L"J_SubmitStatic");
	copDoc3->getElementById(obnm,&htmptr2);
	htmptr2->click();
	obnm.Detach();
	obval.Detach();
	return 0;
}

//int CPublishAide_1View::AuctionCategorySelect(BSTR* bsRoot,BSTR* bsCategory, BSTR* bsBrand, BSTR* bsModelNumber) const
int CPublishAide_1View::AuctionCategorySelect()		//发布商品时的自动选择			//未完成
{
	LPDISPATCH lpDisp=GetHtmlDocument();
	CComPtr<IHTMLElementCollection> cpElemCol;
	CComPtr<IHTMLDOMNode> cpDomNode,cpDomChild1,cpDomChild2;
	IDispatch* pACDisp;
	CComPtr<IHTMLElement> cpElem1;
	IHTMLElement** pElem1;
	IDispatch* pItemDisp;
	CComPtr<IHTMLLIElement> cpLiElem;
	CComPtr<IHTMLAttributeCollection> pAttrColl;
	CComPtr<IHTMLOListElement> cpOlEl;
	CComPtr<IHTMLUniqueName> cpUnName;
	CComPtr<IHTMLDocument3> cpDoc3;
	CComPtr<IHTMLDocument2> cpDoc2;
	CComPtr<IHTMLDOMTextNode> cpDomTextNode;
	CComPtr<IHTMLFiltersCollection> cpFC;
	CComPtr<IElementSelector> cpElemSel;
	IHTMLDOMAttribute* pItem;
	VARIANT vACIndex;
	BSTR bstrName;
	LONG lNodeType;
	VARIANT vNodeValue;
	VARIANT vValue;
	VARIANT_BOOL vbSpecified;
	HRESULT hres;
	LONG lACLength,lACValue;
	CComBSTR bs1(L"cc-tree-item346");		//手机类的 li 元素 id
	CComBSTR bs2(L"cc-cbox-item2107");		// 联想手机 li 元素 id
	CComBSTR bs3(L"cc-cbox-item1833");		// w100 li 元素 id
	CComBSTR bsTemp;
	CComBSTR bs4;

	vNodeValue.vt=VT_BSTR;
	CComBSTR bstr0("J_CatePubBtn");			//按钮
	//CComBSTR bstr1("J_CatePubBtn");
	CComVariant vr1;
	CComVariant vr2;
	vr1.vt=VT_BSTR;
	vr1.bstrVal=L"cc-tree-item346";		
	vr2.vt=VT_DISPATCH;
	vr2.pdispVal=pACDisp;
	hres= lpDisp->QueryInterface(IID_IHTMLDocument3,(void**)&cpDoc3);
	if(SUCCEEDED(hres)&&cpDoc3)
	{
		hres=cpDoc3->getElementById(bs1.m_str,&cpElem1);
		if(SUCCEEDED(hres)&&cpElem1)
		{
			cpElem1->click();
			/*cpElem1->click();
			hres=cpDoc3->getElementById(bs2.m_str,&cpElem1);
			if(SUCCEEDED(hres)&&cpElem1)
			{
				cpElem1->click();
				hres=cpDoc3->getElementById(bs3.m_str,&cpElem1);
				if(SUCCEEDED(hres)&&cpElem1)
				{
					cpElem1->click();
					hres= cpDoc3->getElementById(bstr0.m_str,&cpElem1);
					if(SUCCEEDED(hres)&&cpElem1)
					{
						cpElem1->click();
					}
				}
			}*/
		}
	}
	bs1.Detach();
	bs2.Detach();
	bs3.Detach();
	bstr0.Detach();
	return 0;
}

