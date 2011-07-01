/**
 *  @file       Application.cpp
 *  @brief      Main application class.
 *  @author     Sébastien Lussier.
 *  @date       20/01/02.
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
#include "Engine.h"
#include "Application.h"

#include "Debug/PerformanceMonitor.h"
#include "Module/ModuleManager.h"
#include "FileManager/FileManager.h"

#include "Graphic/RenderTarget/RenderWindow.h"
#include "Subsystem/Subsystem.h"
#include "Resource/ResourceManager.h"


namespace Gamedesk {
	
	
IMPLEMENT_ABSTRACT_CLASS(Application);
IMPLEMENT_ABSTRACT_SINGLETON(Application);

Application::Application(const Char* subsystemConfig)
    : mArgCount(0)
    , mSubsystemConfig(subsystemConfig)
    , mMainWindow(NULL)
    , mRenderWindow(NULL)
{
    mInstance = this;
}

Application::Application()
    : mArgCount(0)
    , mSubsystemConfig( GD_PLATFORM_NAME "Subsystems.cfg" )
    , mMainWindow(NULL)
    , mRenderWindow(NULL)
{
    mInstance = this;
}

Application::~Application()
{
    Vector<Subsystem*>::iterator itSubsystem;

    // Delete every loaded subsystem.
    for( itSubsystem = mSubsystemList.begin(); itSubsystem != mSubsystemList.end(); ++itSubsystem )
        GD_DELETE(*itSubsystem);

    mSubsystemList.clear();
    mInstance = NULL;
}

void Application::Launch( Int32 pArgCount, Char** pArgValues )
{
    mArgCount  = pArgCount;
    mArgValues = pArgValues;

    try
    {
        Init();
        Run();
        Kill();
    }
    catch( Exception& e )
    {
        Core::MsgBox( "Program Error", e.ToString().c_str() );
    }
    catch( std::exception e )
    {
        Core::MsgBox( "Program Error", e.what() );
    }
    catch( ... )
    {
        Core::MsgBox( "Program Error", "Unknown exception encountered ! The application will now quit." );
    }
}

void Application::Init()
{
    Super::Init();

    Profile("Application Init");

    SetupWindows();

    GD_ASSERT_M( mMainWindow != NULL, "[Application::Init] SetupWindows() did not initialize mMainWindow correctly!" );
    GD_ASSERT_M( mRenderWindow != NULL, "[Application::Init] SetupWindows() did not initialize mRenderWindow correctly!" );

    InitSingletons();
    InitSubsystems();
}

void Application::Kill()
{
    KillSubsystems();
    KillSingletons();

    Super::Kill();
}

void Application::InitSingletons()
{
    Profile("ResourceManager Init");
    ResourceManager::Instance()->Init();
}

void Application::InitSubsystems()
{
    // If the subsystem config file can be found, load it.
    if( !FileManager::FileExist(mSubsystemConfig.GetFileName()) )
        return;

    mSubsystemConfig.Load();

    // Configure the subsystems if we need to.
    if( mSubsystemConfig.Get( "General", "ShowSubsystemDlg", false ) )
        SetupSubsystems( mSubsystemConfig );

    // Load every subsystem.
    for( ConfigFile::Iterator itConfig( mSubsystemConfig ); itConfig.IsValid(); ++itConfig )
    {
        try
        {
            ConfigSection& section = *itConfig;
            if( section.GetName() != "General" )
            {
                String subsystemClassName = section.Get( "Current", String("None") );
                if( subsystemClassName != "None" )
                {
                    String folder = section.Get( "PluginDir", String("") );
                    if( !folder.empty() )
                    {
                        ModuleManager::Instance()->LoadModulesInFolder( folder.c_str() );
                    }

                    LoadSubsystem( Class::GetClassByName(subsystemClassName.c_str()) );
                }
            }

        }
        catch( Exception& )
        {
        }
    }
}

void Application::KillSingletons()
{
    ResourceManager::Instance()->Kill();
}

void Application::KillSubsystems()
{
    // Kill each subsystem.
    Vector<Subsystem*>::iterator itSubsystem;
    for( itSubsystem = mSubsystemList.begin(); itSubsystem != mSubsystemList.end(); ++itSubsystem )
        (*itSubsystem)->Kill();
}

Subsystem* Application::LoadSubsystem( Class* pSubsystemClass )
{
    if( !pSubsystemClass )
        throw NullPointerException( "pSubsystemClass", Here );

    Profile(pSubsystemClass->GetName());

    if( !pSubsystemClass->IsDerivedFrom( Subsystem::StaticClass() ) )
        throw InvalidClassException( pSubsystemClass, Subsystem::StaticClass(), Here );

    // If the subsystem is already loaded, return it.
    for( Vector<Subsystem*>::iterator itSubsystems = mSubsystemList.begin();
         itSubsystems != mSubsystemList.end(); ++itSubsystems )
    {
        if( (*itSubsystems)->GetClass() == pSubsystemClass )
            return (*itSubsystems);
    }

    // Not loaded yet so load it.
    Subsystem* subsystem = Cast<Subsystem>( pSubsystemClass->AllocateNew() );
    subsystem->Init();
    mSubsystemList.push_back( subsystem );

    return subsystem;
}

void Application::SetupSubsystems( ConfigFile& pConfigFile )
{
    pConfigFile.Get( "General",          "ShowSubsystemDlg", true );
    pConfigFile.Get( "GraphicSubsystem", "PluginDir",        String("Plugins/Graphic/") );
    pConfigFile.Get( "GraphicSubsystem", "Current",          String("None") );
    pConfigFile.Get( "SoundSubsystem",   "PluginDir",        String("Plugins/Sound/") );
    pConfigFile.Get( "SoundSubsystem",   "Current",          String("None") );
    pConfigFile.Get( "InputSubsystem",   "PluginDir",        String("Plugins/Input/") );
    pConfigFile.Get( "InputSubsystem",   "Current",          String("None") );
}

Window* Application::GetWindow()
{
    return mMainWindow;
}

RenderWindow* Application::GetRenderWindow()
{
    return mRenderWindow;
}


} // namespace Gamedesk
