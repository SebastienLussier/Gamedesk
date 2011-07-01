/**
 *  @file       EditorBase.h
 *  @brief      Declaration of the base editor class.
 *  @author     Sébastien Lussier.
 *  @date       16/12/03.
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
#ifndef     _EDITORBASE_H_
#define     _EDITORBASE_H_


#include "EditorLib.h"
#include "Patterns/Singleton.h"
#include "Command/CommandManager.h"
#include "Viewer.h"


class EditorTool;
class WorldManager;


class EDITORLIB_API EditorBase : public QMainWindow
{
    Q_OBJECT
    DECLARE_ABSTRACT_SINGLETON(EditorBase);
    CLASS_DISABLE_COPY(EditorBase);
        
public:
    //! Constructor.
    EditorBase( QWidget* parent, const char* name, Qt::WFlags f );
    
    //! Destructor.
    virtual ~EditorBase();
    
    //! Returns the tool with the given name.
    EditorTool* GetTool( const String& pToolName ) const;
    
    //! Returns all tools.
    const Map<String,EditorTool*>& GetTools() const;

    //! Set the main view.
    void SetMainView(Viewer* pMainView);

    //! Returns the main view.
    Viewer* GetMainView();

    //! Returns the world manager.
    WorldManager& GetWorldManager();
    
    //! Returns the command manager.
    CommandManager& GetCommandManager();

    virtual void Init();
    
protected:
    void InitActions();
    void InitMenu();

    void LoadTools();
    void UnloadTools();

    void InitTools();
    void KillTools();

    void LoadWorkspace();
    void SaveWorkspace();

protected slots:
    void OnTick();
    void OnNew();
    void OnOpen();
    void OnSave();
    void OnSaveAs();
    void OnCut();
    void OnCopy();
    void OnPaste();
    void OnAbout();
    void UpdateWindowMenu();

private:
    virtual void closeEvent( QCloseEvent* pEvent );

    // Tools
    Map<String,EditorTool*>	mTools;
    Viewer*                 mMainView;

    WorldManager*           mWorldManager;
    CommandManager          mCommandManager;

    // Menu
    QAction* mNewAct;
    QAction* mOpenAct;
    QAction* mSaveAct;
    QAction* mSaveAsAct;
    QAction* mExitAct;
    QAction* mCutAct;
    QAction* mCopyAct;
    QAction* mPasteAct;
    QAction* mCloseAct;
    QAction* mCloseAllAct;
    QAction* mTileAct;
    QAction* mCascadeAct;
    QAction* mNextAct;
    QAction* mPreviousAct;
    QAction* mSeparatorAct;
    QAction* mAboutAct;
    QAction* mAboutQtAct;

    // Menu
    QMenu*                  mFileMenu;
    QMenu*                  mEditMenu;
    QMenu*                  mWindowMenu;
    QMenu*                  mHelpMenu;


	// Update time.
    Double					mTime;
	Double				    mLastTime;
};


#endif  //  _EDITORBASE_H_
