#pragma once

#include "BasicTypes.h"
#include "ObjectLink.h"
#include "World.h"

namespace GameEdit
{
    public ref class EditorLink
    {
    public:
        static property EditorLink^ Instance
        {
            EditorLink^ get()
            {
                return sInstance;
            }
        }

        EditorLink();
        void Init(IntPtr pRenderWindow, Windows::ResourceDictionary^ resourceDictionary);

        Text::StringBuilder^ GetImportableExtensions(String^ pRessourceType);
		void Tick();

		ObjectLink^ CreateLink(Gamedesk::Object* gamedeskObject);
		Type^ BuildDynamicClass(Gamedesk::Class* gdClass);
		Type^ GetDynamicClass(Gamedesk::Class* gdClass);

		ObjectLink^ SpawnEntity(float x, float y, float z);
		Type^ GetTypeFromName(UInt32 propertyID);



	private:
		void BuildProperty(TypeBuilder^ typeBuilder, Gamedesk::Property* prop);
		void BuildProperty(TypeBuilder^ typeBuilder, String^ propertyName, UInt32 offset, String^ category, String^ description, Type^ fieldType, String^ typeName);
		
		SortedDictionary<UInt32, Type^>^  mTypeMap;
		SortedDictionary<String^, Type^>^ mReflectedTypes;

        List<World^>^ mWorlds;

        static EditorLink^ sInstance = gcnew EditorLink();
    };  
}