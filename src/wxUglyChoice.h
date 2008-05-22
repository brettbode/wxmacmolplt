#ifndef _WX_UGLY_CHOICE_H_
#define _WX_UGLY_CHOICE_H_

#include "wx/wx.h"
#include <vector>

using namespace std;

/**
 * A custom widget for use in place of wxChoice.  Unlike wxChoice, wxUglyChoice
 * allows individual items to be disabled.
 */
class wxUglyChoice : public wxPanel {
  private:
    int            selection;
    vector< int >  item;
    int            btnID;

    wxTextCtrl         *m_txt;
    wxBitmapButton     *m_btn;
    wxMenu             m_menu;


    DECLARE_DYNAMIC_CLASS(wxUglyChoice)

  public:
    /**
     * Default Constructor.
     */
    wxUglyChoice() {}

    /**
     * Constructor.
     */
    wxUglyChoice(wxWindow* parent,
                 wxWindowID id = -1,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 int n = 0,
                 const wxString choices[] = NULL,
                 long style = 0);

    /**
		* Constructor.
     */
    wxUglyChoice(wxWindow* parent,
                 wxWindowID id = -1,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 const wxArrayString & choices=wxArrayString(),
                 long style = 0);
	
    /**
     * Destructor.  TODO
     */
    ~wxUglyChoice();

    /**
     * Adds an item to the end of the list.
     *
     * @param item The string to add.
     * @return The index of the newly added item or -1 on failure.
     */
    int Append(const wxString &item);

    /**
     * Adds an item at the given position.
     *
     * @param item The string to add.
     * @param pos The position at which to insert the new item.
     * @return The index of the newly added item or -1 on failure.
     */
    int Insert(const wxString &item, int pos);

    /**
     * Deletes an item from the list.
     *
     * @param n The index of the item to delete.
     */
    void Delete(int n);

    /**
     * Removes all items from the list.
     */
    void Clear();

    /**
     * Tests if there are items added to the list.
     *
     * @return True if there are no items in the list, false otherwise.
     */
    bool IsEmpty() const;

    /**
     * @return The number of items in the list.
     */
    int GetCount() const;

    /**
     * Sets the tooltip for the control.
     *
     * @param tip The text of the tool tip.
     */
    void SetToolTip(const wxString& tip);
    
    /**
     * Enables or disables an item.
     *
     * @param n The item to enable or disable.
     * @param enabled The new state of the item.  True enables the item while
     *                false disables it.
     */
    void SetEnabled(int n, bool enabled);

    /**
     * @return True if the item is enabled, false otherwise.
     */
    bool GetEnabled(int n) const;

    /**
     * Changes the currently selected item.
     *
     * @param n The index of the item to select.
     */
    bool SetSelection(int n);

    /**
     * Included to keep DialogBlocks happy.
     */
    bool SetStringSelection(const wxString &string);

    /**
     * @return The index of the currently selected item.
     */
    int GetSelection() const;

    /**
     * Retrieves the string associated with the item at the given index.
     *
     * @param n The index of the item.
     * @return The string associated with the item.
     */
    wxString GetString(int n) const;

    /**
     * Retrieves the string associated with the currently selected item.
     *
     * @return The string associated with the item.
     */
    wxString GetStringSelection() const;

    
    /* EVENT HANDLERS */
    void onButtonClick(wxCommandEvent &event);
    void onMenuSelect(wxCommandEvent &event);

    //DECLARE_EVENT_TABLE();
};

#endif /* _WX_UGLY_CHOICE_H_ */

