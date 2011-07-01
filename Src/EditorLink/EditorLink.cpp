// This is the main DLL file.

#include "stdafx.h"
#include "EditorLink.h"
#include "GlobalResources.h"

#pragma unmanaged
#include "Application/Application.h"
#include "Application/Window.h"
#include "Application/Win32/Win32Window.h"

#include "Config/ConfigFile.h"
#include "Module/ModuleManager.h"
#include "Object/ObjectIterator.h"
#include "FileManager/FileManager.h"
#include "SystemInfo/SystemInfo.h"

#include "Debug/PerformanceMonitor.h"

#include "World/World.h"

#include "Thread/Thread.h"
#include "Graphic/Font/Font.h"
#include "Graphic/Font/FontHdl.h"
#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"
#include "Graphic/RenderTarget/RenderWindow.h"

#include "World/Camera.h"


HWND gHWnd;
Gamedesk::World* gWorld = 0;

class GDEditApp : public Gamedesk::Application, public Gamedesk::Window::Listener
{
    DECLARE_CLASS(GDEditApp, Gamedesk::Application);

public:
    GDEditApp() 
        : Gamedesk::Application("GDEdit.cfg")
		, mWorld(NULL)
        , mTime(0)
        , mLastTime(0)
        , mRun(true)
    {
    }

    ~GDEditApp()
    {
        GD_DELETE(mWorld);
        GD_DELETE(mRenderWindow);
    }

    void SetupWindows()
    {
        Gamedesk::Win32Window* newWindow = GD_NEW(Gamedesk::Win32Window, this, "Launch::Gamedesk");
        newWindow->Init( gHWnd );
        mMainWindow = newWindow;
		mMainWindow->AddListener(this);

        Gamedesk::RenderWindow::WindowInfo windowInfo;
        windowInfo.mWindowInternal = (Gamedesk::Handle)mMainWindow;
        windowInfo.mWindowHandle   = (Gamedesk::Handle)mMainWindow->GetHandle();
        windowInfo.mFullscreen     = mMainWindow->IsFullScreen();
        windowInfo.mSize           = mMainWindow->GetClientSize();
        
        Gamedesk::RenderTarget::Format targetFormat;
        targetFormat.mAlpha        = true;
        targetFormat.mDepth        = true;
        targetFormat.mDoubleBuffer = true;
        targetFormat.mStencil      = true;
        targetFormat.mStereo       = false;

        Gamedesk::Class* renderWindowClass = Gamedesk::ModuleManager::Instance()->LoadClass( "Win32OGLRenderWindow.Win32OGLRenderWindow" );
        mRenderWindow = Gamedesk::Cast<Gamedesk::RenderWindow>( renderWindowClass->AllocateNew() );
        mRenderWindow->Create( windowInfo, targetFormat );
        mLastTime = Gamedesk::SystemInfo::Instance()->GetSeconds();
    }

    void Init()
    {
        Super::Init();

        mWorld = GD_NEW(Gamedesk::World, this, "Launch::GameEdit");
        mWorld->Init(0);
		gWorld = mWorld;

        mFont.GetFont( "Data/Fonts/tahoma.ttf", 14 );

        mFPS = 0;
        mLastTime = mTime = Gamedesk::SystemInfo::Instance()->GetSeconds();
	    mFrameCount = 0;
    }

    virtual void Kill()
    {
	    Super::Kill();
    }
    
    virtual void SetupSubsystems( Gamedesk::ConfigFile& pConfigFile )
    {
        Super::SetupSubsystems( pConfigFile );
    }

    virtual void Run()
    {
    }

    void Tick()
    {
        Gamedesk::Double newTime = Gamedesk::SystemInfo::Instance()->GetSeconds();
            
        mDelta = newTime - mTime;
        mTime = newTime;
        mFrameCount++;
    		
        if( mTime - mLastTime > 0.5 )
        {
	        mFPS        = mFrameCount / (mTime-mLastTime);
	        mLastTime   = mTime;
	        mFrameCount = 0;
        }

        {
			Profile("Tick");

			// Update the world.
			{
				Profile("World Update");
				mWorld->Update(mDelta);
			}

			// Render the world.
			{
				Profile("World Render");
				mWorld->Render();
				Gamedesk::RenderWindow* renderWindow = Gamedesk::Cast<Gamedesk::RenderWindow>( Gamedesk::GraphicSubsystem::Instance()->GetRenderer()->GetRenderTarget() );
				mFont->DrawString( 6, 20, "FPS: %.2f", mFPS );
				renderWindow->SwapBuffers();
			}
        }
    }

    void OnResizeWindow( const Gamedesk::Window& pWindow )
    {
        mRenderWindow->Resize( pWindow.GetClientSize().x, pWindow.GetClientSize().y );
    }

protected:
    Gamedesk::World*      mWorld;

    // Update time.
    Gamedesk::Double      mDelta;
    Gamedesk::Double		mTime;
	Gamedesk::Double		mLastTime;
    Gamedesk::FontHdl		mFont;

    // FPS
    Gamedesk::Float       mFPS;
 	Gamedesk::Int32       mFrameCount;	

    Gamedesk::Bool        mRun;
};


IMPLEMENT_CLASS(GDEditApp);

GDEditApp*    gGameEditInstance;


#pragma managed

namespace GameEdit
{
	

Type^ EditorLink::GetTypeFromName(UInt32 propertyID)
{
	return mTypeMap[propertyID];
}

void EditorLink::BuildProperty(TypeBuilder^ typeBuilder, Gamedesk::Property* prop)
{
	String^ propertyName = gcnew String(prop->GetName());
	UInt32	offset = prop->GetOffset();
	String^ category = "";
	String^ description = gcnew String(prop->GetDescription());
	Type^	fieldType = GetTypeFromName(prop->GetID());
	String^ typeName = gcnew String(prop->GetTypeName());
	
    BuildProperty(typeBuilder, propertyName, offset, category, description, fieldType, typeName);
}

void EditorLink::BuildProperty(TypeBuilder^ typeBuilder, String^ propertyName, UInt32	offset, String^ category, String^ description, Type^ fieldType, String^ typeName)
{
	String^ getPropertyMethodName = "GetProperty_";
	getPropertyMethodName += typeName;

    // Create the corresponding public property
    PropertyBuilder^ propertyBuilder = typeBuilder->DefineProperty( propertyName, Reflection::PropertyAttributes::HasDefault, fieldType, nullptr);

    // Define the required set of property attributes
    Reflection::MethodAttributes propertyAttributes = Reflection::MethodAttributes::Public | Reflection::MethodAttributes::SpecialName | Reflection::MethodAttributes::HideBySig;

	Reflection::MethodInfo^ getPropertyMethod = ObjectLink::typeid->GetMethod(getPropertyMethodName);
	Reflection::MethodInfo^ setPropertyMethod = ObjectLink::typeid->GetMethod("SetProperty", gcnew array<Type^>{ Int32::typeid, fieldType });

    // Build the getter:
	//	get 
	//  {
    //		return ObjectLink::GetProperty_XXXX(16);
    //  }
	//
	// Code to generate:
	//	IL_0000:  ldarg.0
	//	IL_0001:  ldc.i4.s   16
	//	IL_0003:  call       ObjectLink::GetProperty_XXXX(Int32)
	//	IL_0008:  ret
    MethodBuilder^ getter = typeBuilder->DefineMethod( "get_" + propertyName, propertyAttributes, fieldType, Type::EmptyTypes);
    ILGenerator^ getterIlGen = getter->GetILGenerator();
    getterIlGen->Emit(OpCodes::Ldarg_0);
    if(offset < 128)
		getterIlGen->Emit(OpCodes::Ldc_I4_S, (unsigned char)offset);
	else
		getterIlGen->Emit(OpCodes::Ldc_I4, (int)offset);
	getterIlGen->Emit(OpCodes::Call, getPropertyMethod);
    getterIlGen->Emit(OpCodes::Ret);

    // Build the setter:
	//  set 
    //  {
    //      ObjectLink::SetProperty(16, value);
    //  }
	//
	// Code to generate:
	//	IL_0000:  ldarg.0
	//	IL_0001:  ldc.i4.s   16
	//	IL_0003:  ldarg.1
	//	IL_0004:  call       ObjectLink::SetProperty(Int32, value)
	//	IL_0009:  ret
    MethodBuilder^ setter = typeBuilder->DefineMethod( "set_" + propertyName, propertyAttributes, nullptr, gcnew array<Type^> { fieldType });
    ILGenerator^ setterIlGen = setter->GetILGenerator();
    setterIlGen->Emit(OpCodes::Ldarg_0);
    if(offset < 128)
		setterIlGen->Emit(OpCodes::Ldc_I4_S, (unsigned char)offset);
	else
		setterIlGen->Emit(OpCodes::Ldc_I4, (int)offset);
	setterIlGen->Emit(OpCodes::Ldarg_1);
	setterIlGen->Emit(OpCodes::Call, setPropertyMethod);
	setterIlGen->Emit(OpCodes::Ret);

	// Bind the getter and setter
    propertyBuilder->SetGetMethod(getter);
    propertyBuilder->SetSetMethod(setter);

    // Set the Category and Description attributes
    propertyBuilder->SetCustomAttribute(gcnew CustomAttributeBuilder(CategoryAttribute::typeid->GetConstructor(gcnew array<Type^> { String::typeid }), gcnew array<Object^> { category })); 
    propertyBuilder->SetCustomAttribute(gcnew CustomAttributeBuilder(DescriptionAttribute::typeid->GetConstructor(gcnew array<Type^> { String::typeid }), gcnew array<Object^> { description })); 
}

Type^ EditorLink::BuildDynamicClass(Gamedesk::Class* gdClass)
{
    // Define the dynamic assembly, module and type
    Reflection::AssemblyName^ assemblyName = gcnew Reflection::AssemblyName("DynamicAssembly");
    AssemblyBuilder^ assemblyBuilder = Threading::Thread::GetDomain()->DefineDynamicAssembly(assemblyName, AssemblyBuilderAccess::Run);
    ModuleBuilder^ moduleBuilder = assemblyBuilder->DefineDynamicModule("DynamicModule");
    TypeBuilder^ typeBuilder= moduleBuilder->DefineType(gcnew String(gdClass->GetName().c_str()), Reflection::TypeAttributes::Public);
	typeBuilder->SetParent(ObjectLink::typeid);

    // Create dynamic properties corresponding to the C++ class
    Gamedesk::Class::PropertyIterator itProp(gdClass);
	for( ; itProp.IsValid(); ++itProp )
	{
		Gamedesk::Property* prop = *itProp;
        BuildProperty(typeBuilder, prop);
    }

    // Create and instantiate the dynamic type
    Type^ type = typeBuilder->CreateType();
    return type;
}

Type^ EditorLink::GetDynamicClass(Gamedesk::Class* gdClass)
{
	Type^ reflectedType = nullptr;
	String^ className = gcnew String(gdClass->GetName().c_str());

	if(!mReflectedTypes->TryGetValue(className, reflectedType))
	{
		reflectedType = BuildDynamicClass(gdClass);
		mReflectedTypes[className] = reflectedType;
	}

	return reflectedType;
}
           
EditorLink::EditorLink()
{
    mReflectedTypes = gcnew Collections::Generic::SortedDictionary<String^, Type^>;
	mTypeMap = gcnew Collections::Generic::SortedDictionary<UInt32, Type^>;
	mTypeMap[Gamedesk::PropertyBool::ID()] = Boolean::typeid;
	mTypeMap[Gamedesk::PropertyChar::ID()] = Char::typeid;
	mTypeMap[Gamedesk::PropertyInt16::ID()] = Int16::typeid;
	mTypeMap[Gamedesk::PropertyInt32::ID()] = Int32::typeid;
	mTypeMap[Gamedesk::PropertyInt64::ID()] = Int64::typeid;
	mTypeMap[Gamedesk::PropertyByte::ID()] = Byte::typeid;
	mTypeMap[Gamedesk::PropertyUInt16::ID()] = UInt16::typeid;
	mTypeMap[Gamedesk::PropertyUInt32::ID()] = UInt32::typeid;
	mTypeMap[Gamedesk::PropertyUInt64::ID()] = UInt64::typeid;
	mTypeMap[Gamedesk::PropertyFloat::ID()] = Single::typeid;
	mTypeMap[Gamedesk::PropertyDouble::ID()] = Double::typeid;
	mTypeMap[Gamedesk::PropertyVector3f::ID()] = GameEdit::Vector3f::typeid;
	mTypeMap[Gamedesk::PropertyQuaternionf::ID()] = GameEdit::Quaternionf::typeid;
	mTypeMap[Gamedesk::PropertyColor3f::ID()] = GameEdit::Color3f::typeid;
	mTypeMap[Gamedesk::PropertyColor4f::ID()] = GameEdit::Color4f::typeid;
	mTypeMap[Gamedesk::PropertyString::ID()] = String::typeid;
	mTypeMap[Gamedesk::PropertyEnum::ID()] = GameEdit::Enum::typeid;
}

void EditorLink::Init(IntPtr renderWindow, Windows::ResourceDictionary^ resourceDictionary)
{
    GlobalResources::Resources = resourceDictionary;

    gHWnd = (HWND)renderWindow.ToPointer();
    gGameEditInstance = GD_NEW(GDEditApp, 0, "GameEdit::GameEditInstance");
    gGameEditInstance->Init();
}

class Patate : public Gamedesk::Object
{
	DECLARE_CLASS(Patate, Gamedesk::Object);

public:
	Patate() {}

properties:
	Gamedesk::Float		mFloat;
	Gamedesk::Vector3f	mVector3f;
};

IMPLEMENT_CLASS(Patate);

void Patate::StaticRegisterProperties( Gamedesk::Class* pClass )
{
    if( pClass != Patate::StaticClass() )
        return;

    // mPosition
    static Gamedesk::PropertyFloat mFloatProperty("Float", "Test Float", (Gamedesk::UInt32)&((Patate*)(0))->mFloat);
    pClass->AddProperty(&mFloatProperty);

	static Gamedesk::PropertyVector3f mVector3fProperty("Vector3f", "Test Vector3f", (Gamedesk::UInt32)&((Patate*)(0))->mVector3f);
    pClass->AddProperty(&mVector3fProperty);
}

ObjectLink^ EditorLink::CreateLink(Gamedesk::Object* gamedeskObject)
{
	Type^ newType = GetDynamicClass(gamedeskObject->GetClass());
	ObjectLink^ newObj = safe_cast<ObjectLink^>(Activator::CreateInstance(newType));
	newObj->mGDObject = gamedeskObject;
	return newObj;
}

ObjectLink^ EditorLink::SpawnEntity(float x, float y, float z)
{
//	Patate* sPatate = new Patate();
	if(gWorld)
	{
		Gamedesk::Object* spawnedObject = gWorld->SpawnEntity(Gamedesk::Class::GetClassByName("TestRenderTexture"), Gamedesk::Vector3f(x,y,z));
		return CreateLink(spawnedObject);
	}

	return nullptr;
}

Text::StringBuilder^ EditorLink::GetImportableExtensions(String^ pRessourceType)
{
    Text::StringBuilder^ sb = gcnew Text::StringBuilder;
    sb->Append("This is first line\n");

    return sb;
}

void EditorLink::Tick()
{
    gGameEditInstance->Tick();
}

}