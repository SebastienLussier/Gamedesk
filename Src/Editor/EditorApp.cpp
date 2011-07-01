/**
 *  @file       EditorApp.cpp
 *  @brief      Gamedesk editor application.
 *  @author     Sébastien Lussier.
 *  @date       06/01/04.
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
#include "Editor.h"
#include "EditorApp.h"

#include "GDEditor.h"


IMPLEMENT_CLASS(EditorApp);


class QtWindow : public Window
{
    DECLARE_CLASS(QtWindow,Window);

public:
    QtWindow()
    {
    }

    QtWindow( QWidget* pWidget )
        : mWidget(pWidget)
    {
    }

    virtual ~QtWindow()
    {
    }

    virtual void Init( const String& pTitle, Int32 pWidth, Int32 pHeight, Bool pVisible, Bool pFullscreen )
    {
        mWidget->setWindowTitle(pTitle.c_str());
        mWidget->resize(pWidth,pHeight);
        
        if( pVisible && !mWidget->isVisible() )
            mWidget->show();

        if( !pVisible && mWidget->isVisible() )
            mWidget->hide();

        if( pFullscreen && !mWidget->isFullScreen() )
            mWidget->showFullScreen();

        if( !pFullscreen && mWidget->isFullScreen() )
            mWidget->showNormal();
    }

    virtual void Kill()
    {
        mWidget->close();
    }

    void Tick()
    {
        mWidget->update();
    }

    virtual Handle GetHandle() const
    {
        return (Handle)mWidget->winId();
    }

    virtual void SetPosition( const Vector2i& pPos )
    {
        mWidget->move(pPos.x, pPos.y);
    }

    virtual void SetSize( const Vector2i& pSize )
    {
        mWidget->resize(pSize.x, pSize.y);
    }

    Vector2i GetClientSize() const
    {
        Vector2i size;
        QSize qSize = mWidget->size();

        size.x = qSize.width();
        size.y = qSize.height();

        return size;
    }

    Vector2i GetClientPos() const
    {
        Vector2i   pos;

        pos.x = mWidget->x(); 
        pos.y = mWidget->y();

        return pos;
    }


    virtual void SetFocus()
    {
        mWidget->setFocus(Qt::OtherFocusReason);
    }

    virtual void ShowMinimized()
    {
        mWidget->showMinimized();
    }

    virtual void ShowMaximized()
    {
        mWidget->showMaximized();
    }

    virtual void ShowNormal()
    {
        mWidget->showNormal();
    }

    virtual void ShowFullscreen()
    {
        mWidget->showFullScreen();
    }

    virtual Bool SupportFullScreen()
    {
        return true;
    }

private:
    QWidget*    mWidget;
};

IMPLEMENT_CLASS(QtWindow);



EditorApp::EditorApp() : 
    mQtApp(NULL)
{
}

void EditorApp::Init()
{
    Super::Init();

    mEditor->Init();
}

void EditorApp::Kill()
{
    delete mEditor;
    delete mQtApp; 

    Super::Kill();
}

void EditorApp::SetupWindows()
{
    mQtApp = new QApplication( mArgCount, mArgValues );
    mEditor = new GDEditor();

    mEditor->showMaximized();
    mEditor->show();

    mMainWindow   = new QtWindow(mEditor);
    mRenderWindow = mEditor->GetMainView()->GetRenderWindow();
}

void EditorApp::SetupSubsystems( ConfigFile& pConfigFile )
{
    Super::SetupSubsystems( pConfigFile );
}

void EditorApp::Run()
{
    Int32 result = EXIT_FAILURE;
    result = mQtApp->exec();
}

Handle EditorApp::GetWinHandle() const
{
    return mEditor->winId();
}
