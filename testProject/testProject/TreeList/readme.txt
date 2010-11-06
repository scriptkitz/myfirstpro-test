The Treelist control is a combination Tree/List control derived from CTreeCtrl.

Here's the description of the classes that are used:

    * CTLFrame - derived from CWnd, this class is the frame class for the treelist control. It is used? to include the header control, the tree itself, and the horizontal scroll bar.
    *

      CNewHeaderCtrl - derived from CHeaderCtrl. Used as the header in the TreeList.
    *

      CNewTreeListCtrl - derived from CTreeCtrl, used as the main tree in the TreeList.
    *

      CTLItem - represents each item inside the tree.
    *

      SSortType - structure that's used to indicate whether the sort of the tree is in an ascending order or descending, and which column is being sorted.

How to insert TreeList into your project?

   1. Insert the following files into your project:
          * TLFrame.cpp, TLFrame.h
          * NewTreeListCtrl.cpp, NewTreeListCtrl.h
          * NewHeaderCtrl.cpp, NewHeaderCtrl.h
   2. Include the file "TLFrame.h" in the app file (where the InitInstance function is) and insert the following line in the InitInstance? function:

      ....
      CTLFrame::RegisterClass();
      ....

   3. Layout a user-defined control inside the dialog into which the control is supposed to be intserted. In the class field type: "LANTIVTREELISTCTRL"
   4. Include "TLFrame.h" in the dialog's header file, and add a member variable: CTLFrame m_wndMyTreeList;
   5. In your OnInitDialog() or OnCreate() functions subclass the control:

      ....
      m_wndMyTreeList.SubclassDlgItem(IDC_TREE_LIST, this);

      // IDC_TREE_LIST is the ID of the user-defined control you 

      // inserted into the dialog

      ....

That's it !
Using the TreeListCtrl

The use of the control is simple. It's a tree, so treat it as one (HTREEITEM etc.). In addition, there are functions like InsertColumn, SetItemText, SetItemColor, SetItemBold, GetItemText.
Implementation

Here's my approach to implementing the TreeList. First of all, I had to create a frame window, that would include the 3 objects: header, tree, horz scroll bar. That's what CTLFrame is for. In addition, this CWnd derived class helps during the scroll: the header is clipped after it's repositioned, so there's a feeling of scrolling.

The class CNewHeaderCtrl was created only to put the 3d triangles in it. I decided to include sorting since it's needed in 9/10 cases, and it's a shame every programmer needs to insert it by himself. Finally, CNewTreeListCtrl is the more complicated part. Every item inside the tree, has a DWORD data associated with it, that stores a pointer to a CTLItem class. CTLItem stores the information about each item- its columns' strings, whether the item is bold, the item's color, and the item data that the user wishes to associate with the item. All the functions that deal with the items, like InsertItem, DeleteItem, SetItemText etc. were overridden in order to use the CTLItem technique.

I had some difficulties with the scrolling part, since there are many different conditions for the scroll bars: vertical scroll is shown and then the horz becomes shorter, it's hidden so the horz should become larger again; what happens if the users changed some column's size... and problems like that. I believe I handle each of these cases so I think there won't be any problems with this part.
License

This article has no explicit license attached to it but may contain usage terms in the article text or the download files themselves. If in doubt please contact the author via the discussion board below.

A list of licenses authors might use can be found here