/**
 *  @file       EditorBase.cpp
 *  @brief      Definition of the base editor class.
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
#include "EditorLib.h"
#include "EditorBase.h"
#include "EditorTool.h"
#include "Viewer.h"
#include "WorldManager.h"
#include "Config/ConfigFile.h"
#include "Module/ModuleManager.h"
#include "Object/ObjectIterator.h"
#include "FileManager/FileManager.h"
#include "Input/InputSubsystem.h"
#include "System/SystemInfo.h"
#include "Sound/SoundSubsystem.h"
#include "World/Camera.h"
#include "Debug/PerformanceMonitor.h"
#include "PropertyList/PropertyList.h"


IMPLEMENT_ABSTRACT_SINGLETON(EditorBase);


EditorBase::EditorBase( QWidget* parent, const char* name, Qt::WFlags f ) :
    QMainWindow( parent, f ),
    mMainView(0),
    mWorldManager(0),    
    mTime(0),
    mLastTime(0)
{
    mInstance = this;

    setObjectName(name);
    setWindowTitle("Gamedesk Editor");
    statusBar()->showMessage(tr("Ready"));
    
    InitActions();
    InitMenu();

    QBrush bgBrush( QColor( 0, 0, 0 ) );

    mWorldManager = GD_NEW(WorldManager, this, "EditorLib::EditorBase");

    LoadTools();
}

EditorBase::~EditorBase()
{
    mInstance = NULL;
    
    Core::DebugOut( "############################################\n" );
    Core::DebugOut( "Profiling results (~%d Mhz)\n", SystemInfo::Instance()->GetCpuFrequency() / 1000 );

	for (tree<PerformanceStats>::iterator itr = PerformanceMonitor::mStats.begin(); itr != PerformanceMonitor::mStats.end(); ++itr)
	{
		for( Int32 i = 0; i < PerformanceMonitor::mStats.depth(itr) * 4; i++ )
            Core::DebugOut( " " );

		tree<PerformanceStats>::iterator Parent = PerformanceMonitor::mStats.parent(itr);
		if (Parent != 0)
        {
            UInt64 pTimeUS = itr->mSamples > 0 ? itr->mTotalTime / itr->mSamples : 0;
            Core::DebugOut( "%s(%d) : %d us\n", itr->mName.c_str(), itr->mSamples, pTimeUS );
        }
        else
            Core::DebugOut( "%s\n", itr->mName.c_str() );
    }
    Core::DebugOut( "############################################\n" );

    GD_DELETE(mWorldManager);
    mWorldManager = 0;
}

EditorTool* EditorBase::GetTool( const String& pToolName ) const
{
    Map<String,EditorTool*>::const_iterator itTool = mTools.find(pToolName);
    if( itTool != mTools.end() )
        return itTool->second;

    return NULL;
}

const Map<String,EditorTool*>& EditorBase::GetTools() const
{
    return mTools;
}

void EditorBase::SetMainView(Viewer* pMainView)
{
    mMainView = pMainView;
    setCentralWidget(mMainView);
}

Viewer* EditorBase::GetMainView()
{
    GD_ASSERT(mMainView);
    return mMainView;
}

WorldManager& EditorBase::GetWorldManager()
{
    GD_ASSERT(mWorldManager);
    return *mWorldManager;
}

CommandManager& EditorBase::GetCommandManager()
{
    return mCommandManager;
}

void EditorBase::Init()
{
    mWorldManager->Init();

    InitTools();

    LoadWorkspace();

    mLastTime = SystemInfo::Instance()->GetSeconds();

    QTimer* refreshTimer = new QTimer( this );
    connect( refreshTimer, SIGNAL(timeout()), SLOT(OnTick()) );
    refreshTimer->start( 1000 / 30 );    
}

void EditorBase::InitActions()
{
    mNewAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    mNewAct->setShortcut(tr("Ctrl+N"));
    mNewAct->setStatusTip(tr("Create a new file"));
    connect(mNewAct, SIGNAL(triggered()), this, SLOT(OnNew()));

    mOpenAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    mOpenAct->setShortcut(tr("Ctrl+O"));
    mOpenAct->setStatusTip(tr("Open an existing file"));
    connect(mOpenAct, SIGNAL(triggered()), this, SLOT(OnOpen()));

    mSaveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    mSaveAct->setShortcut(tr("Ctrl+S"));
    mSaveAct->setStatusTip(tr("Save the document to disk"));
    connect(mSaveAct, SIGNAL(triggered()), this, SLOT(OnSave()));

    mSaveAsAct = new QAction(tr("Save &As..."), this);
    mSaveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(mSaveAsAct, SIGNAL(triggered()), this, SLOT(OnSaveAs()));

    mExitAct = new QAction(tr("E&xit"), this);
    mExitAct->setShortcut(tr("Ctrl+Q"));
    mExitAct->setStatusTip(tr("Exit the application"));
    connect(mExitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    mCutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    mCutAct->setShortcut(tr("Ctrl+X"));
    mCutAct->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
    connect(mCutAct, SIGNAL(triggered()), this, SLOT(OnCut()));

    mCopyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    mCopyAct->setShortcut(tr("Ctrl+C"));
    mCopyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
    connect(mCopyAct, SIGNAL(triggered()), this, SLOT(OnCopy()));

    mPasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    mPasteAct->setShortcut(tr("Ctrl+V"));
    mPasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
    connect(mPasteAct, SIGNAL(triggered()), this, SLOT(OnPaste()));

    mSeparatorAct = new QAction(this);
    mSeparatorAct->setSeparator(true);

    mAboutAct = new QAction(tr("&About"), this);
    mAboutAct->setStatusTip(tr("Show the application's About box"));
    connect(mAboutAct, SIGNAL(triggered()), this, SLOT(OnAbout()));

    mAboutQtAct = new QAction(tr("About &Qt"), this);
    mAboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(mAboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void EditorBase::InitMenu()
{
    mFileMenu = menuBar()->addMenu(tr("&File"));
    mFileMenu->addAction(mNewAct);
    mFileMenu->addAction(mOpenAct);
    mFileMenu->addAction(mSaveAct);
    mFileMenu->addAction(mSaveAsAct);
    mFileMenu->addSeparator();
    mFileMenu->addAction(mExitAct);

    mEditMenu = menuBar()->addMenu(tr("&Edit"));
    mEditMenu->addAction(mCutAct);
    mEditMenu->addAction(mCopyAct);
    mEditMenu->addAction(mPasteAct);

    mWindowMenu = menuBar()->addMenu(tr("&Window"));
    connect(mWindowMenu, SIGNAL(aboutToShow()), this, SLOT(UpdateWindowMenu()));

    menuBar()->addSeparator();

    mHelpMenu = menuBar()->addMenu(tr("&Help"));
    mHelpMenu->addAction(mAboutAct);
    mHelpMenu->addAction(mAboutQtAct);
}

void EditorBase::LoadTools()
{
    ModuleManager::Instance()->LoadModulesInFolder( "Plugins/Tools/" );

    // Create the tools and init their UI;
    Class::Iterator itClassesNew( EditorTool::StaticClass() );
    for( ; itClassesNew.IsValid(); ++itClassesNew )
    {
        EditorTool* newTool = Cast<EditorTool>((*itClassesNew)->AllocateNew());

        try
        {
            newTool->SetEditor(this);

            QWidget* toolWidget = newTool->CreateUI();
            if( toolWidget != mMainView )
            {
                QDockWidget* contentsWindow = GD_NEW(QDockWidget, this, 0)(toolWidget->objectName(), this);
                contentsWindow->setWidget(toolWidget);
                addDockWidget(Qt::LeftDockWidgetArea, contentsWindow);
            }

            newTool->Hide();
            mTools[(*itClassesNew)->GetName()] = newTool;
        }
        catch( Exception& e )
        {
            qWarning( e.GetMessage().c_str() );
            GD_DELETE(newTool);
        }
    }
}

void EditorBase::InitTools()
{
    // Init all tools.
    Map<String,EditorTool*>::iterator itTool;
    for(itTool = mTools.begin(); itTool != mTools.end(); itTool++)
    {
        try
        {
            itTool->second->Init();
        }
        catch( Exception& e )
        {
            qWarning( e.GetMessage().c_str() );
        }    
    }
}

void EditorBase::KillTools()
{
    // Kill the tools
    Map<String,EditorTool*>::iterator   itTool;
    for(itTool = mTools.begin(); itTool != mTools.end(); ++itTool)
    {
        EditorTool* tool = itTool->second;
        tool->Kill();
    }
}

void EditorBase::UnloadTools()
{
    // Delete the tools.
    Map<String,EditorTool*>::iterator   itTool;
    for(itTool = mTools.begin(); itTool != mTools.end(); ++itTool)
    {
        EditorTool* tool = itTool->second;
        tool->DestroyUI();
        GD_DELETE(tool);
    }
}

void EditorBase::LoadWorkspace()
{
    ConfigFile      config( "Workspace.cfg" );

    // If the subsystem config file can be found, load it.
    if( FileManager::FileExist("Config/Workspace.cfg") )
        config.Load();

    Int32   toolsCount = config.Get( "Workspace", "ToolsCount", (Int32)0 );
    String  toolClassName;

    for( Int32 i = 0; i < toolsCount; i++ )
    {
        ConfigSection& pSection = config["Tool_"+ToString(i)];
        toolClassName = pSection["ClassName"];

        EditorTool* tool = GetTool(toolClassName);
        if( tool )
            tool->LoadSettings( pSection );
    }
}

void EditorBase::SaveWorkspace()
{
    ConfigFile      config( "Workspace.cfg" );

    Map<String,EditorTool*>::iterator   itTool;
    config["Workspace"]["ToolsCount"] = (Int32)mTools.size();

    Int32   i = 0;

    for( itTool = mTools.begin(); itTool != mTools.end(); ++itTool, i++ )
    {
        EditorTool* tool = itTool->second;

        ConfigSection& pSection = config["Tool_"+ToString(i)];
        pSection["ClassName"] = (String)tool->GetClass()->GetName();
        tool->SaveSettings( pSection );
    }

    try
    {
        config.Save();
    }
    catch( Exception& )
    {
        Core::MsgBox( "Gamedesk - Saving Workspace", "Unable to save workspace information! (file \"Workspace.cfg\" is read-only?)" );
    }
}

void EditorBase::OnTick()
{
    mTime = SystemInfo::Instance()->GetSeconds();
    Double elapsedTime = mTime - mLastTime;

    elapsedTime *= SoundSubsystem::Instance()->GetSpeedRatio();

    {
    Profile("Tick");

	// Update the input system.
    {
    Profile("Input");
    InputSubsystem::Instance()->Update();
    }

    // Update the physic subsystem.
    //{
    //Profile("Physic");
    //PhysicSubsystem::Instance()->Update(elapsedTime);
    //}

	// Update the world manager.
    {
    Profile("World");
	mWorldManager->Update(elapsedTime);
    }

    // Update the sound subsystem.
    {
    Profile("Sound");
    Camera* camera = mWorldManager->GetCurrentCamera();
    SoundSubsystem::Instance()->Update( elapsedTime, 
                                        camera->GetPosition(),
                                        camera->GetView(),
                                        camera->GetUp() );
    }
    }

    mLastTime = mTime;
}

void EditorBase::closeEvent( QCloseEvent* pEvent )
{
    SaveWorkspace();

    KillTools();
    UnloadTools();

    QMainWindow::closeEvent( pEvent );
}

void EditorBase::OnNew()
{
}
void EditorBase::OnOpen()
{
}
void EditorBase::OnSave()
{
}
void EditorBase::OnSaveAs()
{
}
void EditorBase::OnCut()
{
}
void EditorBase::OnCopy()
{
}
void EditorBase::OnPaste()
{
}
void EditorBase::OnAbout()
{
    QMessageBox::about(this, tr("About Gamedesk"),
        tr("This is a kick ass editor!! (Yeah right!)"));
}

void EditorBase::UpdateWindowMenu()
{
}
