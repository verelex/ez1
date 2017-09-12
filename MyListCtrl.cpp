
#include "stdafx.h"
#include "MyListCtrl.h"
#include "Resource.h"
#include "webbrowser2.h"

BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBeginDrag)
	ON_WM_MOUSEMOVE( )
	ON_WM_LBUTTONUP( )
	ON_WM_RBUTTONDOWN( )
	ON_WM_LBUTTONDBLCLK( )
END_MESSAGE_MAP()
//--------------------------------------------------------------------------------
CMyListCtrl::CMyListCtrl(CWebBrowser2 *pwb) : CListCtrl()
{
	m_mode = 0;
	// Set up initial variables for drag-drop
	m_bDragging = false;
	m_nDragIndex = -1;
	m_nDropIndex = -1;
	m_pDragImage = NULL;
	// Set up ie web browser pointer. todo: make const
	m_ieBrowser = pwb;
}
//--------------------------------------------------------------------------------
CString CMyListCtrl::GetPKID(void)
{
	return m_PKID;
}
//--------------------------------------------------------------------------------
void CMyListCtrl::SetPKID(CString s)
{
	m_PKID = s;
}
//--------------------------------------------------------------------------------
void CMyListCtrl::OnRButtonDown(UINT nFlags,CPoint point)
{
	LVHITTESTINFO hti;
	memset(&hti, 0, sizeof(hti));
	hti.pt.x = point.x;
	hti.pt.y = point.y;
	SubItemHitTest(&hti);

	// onlySelected=true if the item clicked is selected, and no others are
	/*bool onlySelected = false;//(GetItemState(hti.iItem, LVIS_SELECTED) != 0);
	int NumItems = GetItemCount();
	for (int item=0; item<NumItems; item++)
	{
		if (item!=hti.iItem && GetItemState(item, LVIS_SELECTED)!=0)
		{
			onlySelected = false;
		}
	}
	bool ctrlKeyDown = ((nFlags&MK_CONTROL) != 0);
	bool shiftKeyDown = ((nFlags&MK_SHIFT) != 0);

	if (onlySelected && !ctrlKeyDown && !shiftKeyDown)
	{*/
		// deselect all items
		//SetItemState(-1, 0, LVIS_SELECTED);

		// set clipboard text to what that field's value is
		int sizeBytes = MAX_PATH * sizeof(TCHAR);
		hData = GlobalAlloc(GHND,sizeBytes);
		if(hData)
		{
			LPVOID pStr = GlobalLock(hData);
			if(pStr)
			{
				//memset(pStr,0,sizeBytes);
				GetItemText(hti.iItem,hti.iSubItem,(LPTSTR)pStr,sizeBytes-1);
				GlobalUnlock(hData);
				//
				if(!OpenClipboard())
				{
					MessageBox(L"Cannot open the Clipboard");
					return;
				}
				// Remove the current Clipboard contents  
				if(!EmptyClipboard())
				{
					MessageBox(L"Cannot empty the Clipboard");
					return;  
				}
				// Get the currently selected data, hData handle to 
				// global memory of data
				if(SetClipboardData(CF_UNICODETEXT,hData) == NULL)  
				{
					//MessageBox(L"Unable to set Clipboard data");
					ReportError();
				}  
				CloseClipboard();
				//MessageBox((LPCTSTR)pStr);
			}
			//GlobalFree(hData); // <--- ни в коем случае не освобождать!!!
		}
	//}
}
//--------------------------------------------------------------------------------
void CMyListCtrl::OnLButtonDblClk( UINT nFlags,CPoint point )
{
	LVHITTESTINFO hti;
	memset(&hti, 0, sizeof(hti));
	hti.pt.x = point.x;
	hti.pt.y = point.y;
	SubItemHitTest(&hti);

	// onlySelected=true if the item clicked is selected, and no others are
	bool onlySelected = (GetItemState(hti.iItem, LVIS_SELECTED) != 0);
	int NumItems = GetItemCount();
	for (int item=0; item<NumItems; item++)
	{
		if (item!=hti.iItem && GetItemState(item, LVIS_SELECTED)!=0)
		{
			onlySelected = false;
		}
	}
	bool ctrlKeyDown = ((nFlags&MK_CONTROL) != 0);
	bool shiftKeyDown = ((nFlags&MK_SHIFT) != 0);

	if (onlySelected && !ctrlKeyDown && !shiftKeyDown)
	{
		// deselect all items
		SetItemState(-1, 0, LVIS_SELECTED);

		/*if (m_mode == 1)
		{
			PTCHAR str1 = new TCHAR[MAX_PATH];
			GetItemText(hti.iItem,hti.iSubItem,str1,MAX_PATH-1);
			m_iter = m_plpss->begin();
			for (UINT i=0; i < m_plpss->size(); i++)
			{
				if( !wcscmp(m_iter->second.c_str(),str1) )
				{
					lstrcpy(dest,m_iter->first.c_str());
				}
				m_iter ++;
			}
			//MessageBox(str);
			if (str1) delete str1;
		}*/
		if (m_mode == 0 && hti.iSubItem != 0) // первый столбец не трогаем
		{
			// get the bounding box for the field to be edited
			CRect bounds;
			memset(&bounds, 0, sizeof(bounds));
			GetSubItemRect(hti.iItem, hti.iSubItem, LVIR_LABEL, bounds);

			// create the edit box
			edit = new CMyEdit;
			// DestroyWindow() called from CMyEdit::OnKillFocus(CWnd* w)
			// delete?
			bounds.right = bounds.left+200;
			bounds.bottom = bounds.top+200;
			edit->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,bounds,this,0);
			
			// set edit box text to what that field's value is
			PTCHAR str = new TCHAR[MAX_PATH];
			GetItemText(hti.iItem,hti.iSubItem,str,MAX_PATH-1);
			edit->SetWindowTextW(str);
			edit->SetMLCitems(this,hti.iItem,hti.iSubItem);
			if (str) delete str;

			edit->ShowWindow(SW_SHOWNORMAL);
			edit->UpdateWindow();
			edit->SetFocus();
		}
		if (hti.iSubItem == 0) // первый столбец
		{
			if(m_ieBrowser)
			{
				SetPKID(GetItemText(hti.iItem,hti.iSubItem)); // primary key id
				NavigateToDllResource();
			}
		}
	}
	else
	{
		//CListCtrl::OnLButtonDown(nFlags,point); // было это хз зачем
		CListCtrl::OnLButtonDblClk(nFlags,point);
	}
}
//--------------------------------------------------------------------------------
void CMyListCtrl::ExportDataToBuffer(char* buf)
{
	int len = GetItemCount(); // сколько кортежей (линий) в ListView
	for(int i=0; i<len; i++)
	{
		for(int j=1; j<4; j++) // 4 столбца, но 1 не трогаем т.к. там номер
		{
			CString strW = GetItemText(i,j);
			int size = strW.GetLength();
			char* strA = new char[size+1];
			if(strA)
			{
				memset(strA,0,size+1);
				// перевод из unicode
				WideCharToMultiByte(CP_ACP,0,(WCHAR*)strW.GetBuffer(),size,
									strA,size,0,0);
				strncat(buf,strA,size);
				strncat(buf,"|",1); // разделитель подстрок
				delete strA;
			}
		}
		//strncat(buf,"`",1); // разделитель строк // upd: нет его
	}
}
//--------------------------------------------------------------------------------
int CMyListCtrl::GetDataSize()
{
	int size = 0;
	int len = GetItemCount(); // количество строк (кортежей)
	for(int i=0; i<len; i++)
	{
		for(int j=1; j<4; j++) // 4 столбца, но 1 не трогаем т.к. там номер
		{
			CString str1 = GetItemText(i,j);
			size += str1.GetLength();
		}
	}
	return size;
}
//--------------------------------------------------------------------------------
void CMyListCtrl::SetHasChangesFlag()
{
	*m_FlagPointer = TRUE;
}
//--------------------------------------------------------------------------------
void CMyListCtrl::ReportError()
{ 
    LPVOID lpMsgBuf;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );
 
    MessageBox((LPCTSTR)lpMsgBuf); 

    LocalFree(lpMsgBuf);
}
//--------------------------------------------------------------------------------
void CMyListCtrl::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// This routine sets the parameters for a Drag and Drop operation.
	// It sets some variables to track the Drag/Drop as well
	// as creating the drag image to be shown during the drag.

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// Save the index of the item being dragged in m_nDragIndex
	// This will be used later for retrieving the info dragged
	m_nDragIndex = pNMListView->iItem;

	// Create a drag image
	POINT pt;
	int nOffset = -10; // offset in pixels for drag image (positive is up and to the left; neg is down and to the right)
	//if(GetSelectedCount() > 1) // more than one item is selected
	pt.x = nOffset;
	pt.y = nOffset;

	m_pDragImage = CreateDragImage(m_nDragIndex, &pt);
	//ASSERT(m_pDragImage); // make sure it was created
	/*if(m_pDragImage)
	{
		// We will call delete later (in LButtonUp) to clean this up

		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP1);
		m_pDragImage->Replace(0, &bitmap, &bitmap);

		// Change the cursor to the drag image
		// (still must perform DragMove() in OnMouseMove() to show it moving)
		m_pDragImage->BeginDrag(0, CPoint(nOffset, nOffset - 4));
		m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction);
	
		// Set dragging flag and others
		m_bDragging = TRUE;	// we are in a drag and drop operation
		m_nDropIndex = -1;	// we don't have a drop index yet
		//m_pDragList = &m_listL; // make note of which list we are dragging from
		//m_pDropWnd = &m_listL;	// at present the drag list is the drop list
	}*/
	// Capture all mouse messages
	SetCapture();

	*pResult = 0;
}
//--------------------------------------------------------------------------------
void CMyListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// While the mouse is moving, this routine is called.
	// This routine will redraw the drag image at the present
	// mouse location to display the dragging.
	// Also, while over a CListCtrl, this routine will highlight
	// the item we are hovering over.

	// If we are in a drag/drop procedure (m_bDragging is true)
	if(m_bDragging)
	{
		// Move the drag image
		CPoint pt(point); // get our current mouse coordinates
		ClientToScreen(&pt); // convert to screen coordinates
		m_pDragImage->DragMove(pt); // move the drag image to those coordinates
		// Unlock window updates (this allows the dragging image to be shown smoothly)
		m_pDragImage->DragShowNolock(false);

		// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint(pt);
		//ASSERT(pDropWnd); // make sure we have a window
		if(pDropWnd)
		{
			// Save current window pointer as the CListCtrl we are dropping onto
			//m_pDropWnd = pDropWnd;

			// Convert from screen coordinates to drop target client coordinates
			pDropWnd->ScreenToClient(&pt);

			// If we are hovering over a CListCtrl we need to adjust the highlights
			if(pDropWnd->IsKindOf(RUNTIME_CLASS(CListCtrl)))
			{			
				// Note that we can drop here
				SetCursor(LoadCursor(NULL, IDC_ARROW));
				UINT uFlags;
				CListCtrl* pList = (CListCtrl*)pDropWnd;

				// Turn off hilight for previous drop target
				pList->SetItemState(m_nDropIndex, 0, LVIS_DROPHILITED);
				// Redraw previous item
				pList->RedrawItems(m_nDropIndex, m_nDropIndex);

				// Get the item that is below cursor
				m_nDropIndex = ((CListCtrl*)pDropWnd)->HitTest(pt, &uFlags);
				// Highlight it
				pList->SetItemState(m_nDropIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);
				// Redraw item
				pList->RedrawItems(m_nDropIndex, m_nDropIndex);
				pList->UpdateWindow();
			}
			else
			{
				// If we are not hovering over a CListCtrl, change the cursor
				// to note that we cannot drop here
				SetCursor(LoadCursor(NULL, IDC_NO));
			}
			// Lock window updates
			m_pDragImage->DragShowNolock(true);
		}
	}
		
	CListCtrl::OnMouseMove(nFlags, point);
}
//--------------------------------------------------------------------------------
void CMyListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// This routine is the end of the drag/drop operation.
	// When the button is released, we are to drop the item.
	// There are a few things we need to do to clean up and
	// finalize the drop:
	//	1) Release the mouse capture
	//	2) Set m_bDragging to false to signify we are not dragging
	//	3) Actually drop the item (we call a separate function to do that)

	// If we are in a drag and drop operation (otherwise we don't do anything)
	if(m_bDragging)
	{
		// Release mouse capture, so that other controls can get control/messages
		ReleaseCapture();

		// Note that we are NOT in a drag operation
		m_bDragging = FALSE;

		// End dragging image
		m_pDragImage->DragLeave(GetDesktopWindow());
		m_pDragImage->EndDrag();
		if(m_pDragImage) delete m_pDragImage; // must delete it because it was created 
											  // at the beginning of the drag

		CPoint pt (point); // Get current mouse coordinates
		ClientToScreen(&pt); // Convert to screen coordinates
		// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint(pt);
		//ASSERT(pDropWnd); //make sure we have a window pointer
		if(pDropWnd)
		{
			// If window is CListCtrl, we perform the drop
			if (pDropWnd->IsKindOf(RUNTIME_CLASS(CListCtrl)))
			{
				//m_pDropList = (CListCtrl*)pDropWnd; // Set pointer to the list we are dropping on
				DropItemOnList(/*m_pDragList, m_pDropList*/); // Call routine to perform the actual drop
			}
		}
	}

	CListCtrl::OnLButtonUp(nFlags, point);
}
//--------------------------------------------------------------------------------
void CMyListCtrl::DropItemOnList()
{
	// This routine performs the actual drop of the item dragged.
	// It simply grabs the info from the Drag list (pDragList)
	// and puts that info into the list dropped on (pDropList).

	//Set up the LV_ITEM for retrieving item from pDragList and adding the new item to the pDropList
	PTCHAR szLabel = new TCHAR[256];
	if(szLabel)
	{
		// Unhilight the drop target
		SetItemState(m_nDropIndex, 0, LVIS_DROPHILITED);

		LVITEM lvi;

		memset(&lvi,0,sizeof(LVITEM)); // clear memory space for LV_ITEM
		lvi.iItem = m_nDragIndex;
		lvi.mask = LVIF_TEXT;
		lvi.pszText = szLabel;
		lvi.cchTextMax = 255;

		if(GetSelectedCount() == 1)
		{
			// Get item that was dragged
			GetItem(&lvi);
			CString s1 = GetItemText(lvi.iItem, 1);
			CString s2 = GetItemText(lvi.iItem, 2);
			CString s3 = GetItemText(lvi.iItem, 3);

			// Delete the original item (for Move operation)
			// This is optional. If you want to implement a Copy operation, don't delete.
			// This works very well though for re-arranging items within a CListCtrl.
			// It is written at present such that when dragging from one list to the other
			// the item is copied, but if dragging within one list, the item is moved.
			//if(pDragList == pDropList)
			//{
			DeleteItem(m_nDragIndex);
			if(m_nDragIndex < m_nDropIndex) m_nDropIndex--; // decrement drop index to account for item
															// being deleted above it
			//}

			// Insert item into pDropList
			// if m_nDropIndex == -1, iItem = GetItemCount() (inserts at end of list), else iItem = m_nDropIndex
			lvi.iItem = (m_nDropIndex == -1) ? GetItemCount() : m_nDropIndex;
			InsertItem(&lvi);
			SetItemText(lvi.iItem, 1, (LPCTSTR)s1);
			SetItemText(lvi.iItem, 2, (LPCTSTR)s2);
			SetItemText(lvi.iItem, 3, (LPCTSTR)s3);

			// Select the new item we just inserted
			SetItemState(lvi.iItem, LVIS_SELECTED, LVIS_SELECTED);
		}
		delete szLabel;
	}
}
//--------------------------------------------------------------------------------
PTCHAR CMyListCtrl::strrpos(PTCHAR src, TCHAR cmp)
{
	int len = lstrlen(src);
	for(int i=len; i>0; i--)
	{
		if(src[i] == cmp) return src+i;
	}
	return 0;
}
//-----------------------------------------------------
void CMyListCtrl::GetPath(PTCHAR src_in, PTCHAR dest_out)
{
	PTCHAR tmp = strrpos(src_in,'\\');
	if(tmp)
	{
		int len1 = lstrlen(src_in);
		int len2 = lstrlen(tmp);
		lstrcpyn(dest_out, src_in, len1 - len2 + 2); // +2 for including trailing slash + '\0'
	}
	else
		MessageBox(_T("GetPath() : символ '\\' не найден"));
}
//--------------------------------------------------------------------------------
void CMyListCtrl::NavigateToDllResource()
{
	PTCHAR tmp1 = new TCHAR[MAX_PATH*2];
	PTCHAR tmp2 = new TCHAR[MAX_PATH*2];
	if(tmp1 && tmp2)
	{
		GetModuleFileName(0,tmp1,MAX_PATH);
		GetPath(tmp1, tmp2);
		lstrcat(tmp2,_T("vxres1.dll/103"));
		lstrcpy(tmp1,_T("res://"));
		lstrcat(tmp1,tmp2);
		//MessageBox(tmp1);
		m_ieBrowser->Navigate(tmp1, NULL, NULL, NULL,NULL);
		delete tmp1;
		delete tmp2;
	}
}
//  [8/20/2010 user]------------------------------------------------------
