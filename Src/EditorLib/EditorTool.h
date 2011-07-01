/**
 *  @file       EditorTool.h
 *  @brief      Basic class for all tools of the editor.
 *  @author     Sébastien Lussier.
 *  @date       12/12/03.
 */
/*
 *  Copyright (C) 2004 Gamedesk
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  Gamedesk
 *  http://gamedesk.type-cast.com
 *
 */
#ifndef     _EDITORTOOL_H_
#define     _EDITORTOOL_H_


class ConfigSection;
class EditorBase;


class EDITORLIB_API EditorTool : public Object
{
    DECLARE_ABSTRACT_CLASS(EditorTool,Object);

public:
    EditorTool();
    virtual ~EditorTool();
    
    virtual void LoadSettings( ConfigSection& pSection );
    virtual void SaveSettings( ConfigSection& pSection ) const;

    Bool         IsVisible() const;
    virtual void Show();
    virtual void Hide();

    //! Create the ui window for the tool.
    virtual QWidget* CreateUI() = 0;

    //! Destroy the ui window for the tool.
    virtual void DestroyUI() = 0;

    /**
     * Set the editor for this tool.
     * @param   pEditor The editor associated with the tool.
     */
    void SetEditor(EditorBase* pEditor);

    //! Returns the editor associated with this tool.
    EditorBase* GetEditor();

protected:
    //! Set the ui window for the tool.
    void SetWindow( QWidget* pWindow );

	//! Returns whether the point is inside the tool's window.
	Bool IsPointOnTool(Int32 pX, Int32 pY) const;

private:
    Bool        mVisible;
    Int32       mPosX;
    Int32       mPosY;
    Int32       mWidth;
    Int32       mHeight;
    QWidget*    mWindow;
    EditorBase* mEditor;
};


#endif  //  _EDITORTOOL_H_
