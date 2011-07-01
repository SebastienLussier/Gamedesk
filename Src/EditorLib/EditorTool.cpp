/**
 *  @file       EditorTool.cpp
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
#include "EditorLib.h"
#include "EditorTool.h"
#include "EditorBase.h"
#include "Config/Configfile.h"


IMPLEMENT_ABSTRACT_CLASS(EditorTool);

EditorTool::EditorTool() :
    mVisible(true),
    mPosX(100),
    mPosY(100),
    mWidth(200),
    mHeight(200),
    mWindow(NULL),
    mEditor(NULL)
{
}

EditorTool::~EditorTool()
{    
}

void EditorTool::LoadSettings( ConfigSection& pSection )
{
    // Get our information from the config file.
    mVisible =  (Bool)pSection.Get( "Visible",  true );
    mPosX    = (Int32)pSection.Get( "PosX",     (Int32)10 );
    mPosY    = (Int32)pSection.Get( "PosY",     (Int32)10 );
    mWidth   = (Int32)pSection.Get( "Width",    (Int32)50 );
    mHeight  = (Int32)pSection.Get( "Height",   (Int32)50 );

    // Set the window information.
    mWindow->resize( mWidth, mHeight );

    if( mWindow->parentWidget() )
        mWindow->parentWidget()->move( mPosX, mPosY );

    // Show or hide the window depending of the setting.
    if( mVisible )
        Show();
    else
        Hide();
}

void EditorTool::SaveSettings( ConfigSection& pSection ) const
{
    // If window exist, get the latest information from it.
    if( mWindow )
    {
        //pSection["Visible"] =  (Bool)mWindow->isVisible();
        pSection["PosX"]    = (Int32)mWindow->parentWidget()->pos().x();
        pSection["PosY"]    = (Int32)mWindow->parentWidget()->pos().y();
        pSection["Width"]   = (Int32)mWindow->width();
        pSection["Height"]  = (Int32)mWindow->height();
    }
    else
    {
        // Save the information in the config section.
        //pSection["Visible"] = mVisible;
        pSection["PosX"]    = mPosX;
        pSection["PosY"]    = mPosY;
        pSection["Width"]   = mWidth;
        pSection["Height"]  = mHeight;
    }
}

Bool EditorTool::IsVisible() const
{
    if( mWindow )
        return mWindow->isVisible();
    else
        return false;
}

void EditorTool::Show()
{
    // Make sure the window as been created.
    GD_ASSERT(mWindow);

    mWindow->show();
}

void EditorTool::Hide()
{
    if( mWindow )
    {
        mWindow->hide();
    }
}

EditorBase* EditorTool::GetEditor()
{
    return mEditor;
}

void EditorTool::SetEditor(EditorBase* pEditor)
{
    mEditor = pEditor;
}

void EditorTool::SetWindow( QWidget* pWindow )
{
    mWindow = pWindow;
}

Bool EditorTool::IsPointOnTool(Int32 pX, Int32 pY) const
{
	if(mWindow)
	{
		QPoint widgetPoint = mWindow->mapFromGlobal(QPoint(pX, pY));
		
		return widgetPoint.x() >= 0 && widgetPoint.x() < mWindow->width() &&
			   widgetPoint.y() >= 0 && widgetPoint.y() < mWindow->height();
	}

	return false;
}
