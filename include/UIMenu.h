#ifndef __UIMENU_H__
#define __UIMENU_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "observer_impl_base.hpp"

namespace DuiLib {

#define WM_MENU			(WM_USER + 0xFF)

/////////////////////////////////////////////////////////////////////////////////////
//
struct ContextMenuParam
{
	// 1: remove all
	// 2: remove the sub menu
	WPARAM wParam;
	HWND hWnd;
};

enum MenuAlignment
{
	eMenuAlignment_Left = 1 << 1,
	eMenuAlignment_Top = 1 << 2,
	eMenuAlignment_Right = 1 << 3,
	eMenuAlignment_Bottom = 1 << 4,
};

typedef class ObserverImpl<BOOL, ContextMenuParam> ContextMenuObserver;
typedef class ReceiverImpl<BOOL, ContextMenuParam> ContextMenuReceiver;

extern ContextMenuObserver s_context_menu_observer;

// MenuUI
extern const TCHAR* const kMenuUIClassName;// = _T("MenuUI");
extern const TCHAR* const kMenuUIInterfaceName;// = _T("Menu");

class CListUI;
class CMenuUI : public CListUI
{
public:
	CMenuUI();
	~CMenuUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	virtual void DoEvent(TEventUI& event);

	virtual bool Add(CControlUI* pControl);
	virtual bool AddAt(CControlUI* pControl, int iIndex);

	virtual int GetItemIndex(CControlUI* pControl) const;
	virtual bool SetItemIndex(CControlUI* pControl, int iIndex);
	virtual bool Remove(CControlUI* pControl);

	SIZE EstimateSize(SIZE szAvailable);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
};

/////////////////////////////////////////////////////////////////////////////////////
//
// CMenuBuilderCallback
class CMenuBuilderCallback : public IDialogBuilderCallback
{
public:
	virtual CControlUI* CreateControl(LPCTSTR pstrClass, CPaintManagerUI* pManager);
};

/////////////////////////////////////////////////////////////////////////////////////
//
// CMenuWnd
extern const TCHAR* const kMenuElementUIClassName;// = _T("MenuElementUI");
extern const TCHAR* const kMenuElementUIInterfaceName;// = _T("MenuElement);

class CMenuElementUI;
class CMenuWnd : public CWindowWnd, public ContextMenuReceiver
{
public:
	CMenuWnd(HWND hParent = NULL);
	void Init(CMenuElementUI* pOwner, INotifyUI* pNotifyUI, STRINGorID xml, LPCTSTR pSkinType, POINT point);
	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage(HWND hWnd);

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL Receive(ContextMenuParam param);

protected:
	HWND m_hParent;
	POINT m_BasedPoint;
	STRINGorID m_xml;
	CStdString m_sType;
	CPaintManagerUI m_pm;
	CMenuElementUI* m_pOwner;
	CMenuUI* m_pLayout;
public:
	static INotifyUI* m_pNotifyUI;
};

/////////////////////////////////////////////////////////////////////////////////////
//
// MenuElementUI
class CListContainerElementUI;
class CMenuElementUI : public CListContainerElementUI
{
	friend CMenuWnd;
public:
	CMenuElementUI();
	~CMenuElementUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

    void DoPaint(void* ctx, const RECT& rcPaint);

	void DrawItemText(void* ctx, const RECT& rcItem);

	SIZE EstimateSize(SIZE szAvailable);

	bool Activate();

	void DoEvent(TEventUI& event);

	void CreateMenuWnd();

protected:
	CMenuWnd* m_pWindow;
};

} // namespace DuiLib

#endif // __UIMENU_H__