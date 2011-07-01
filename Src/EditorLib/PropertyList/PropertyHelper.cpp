#include "EditorLib.h"
#include "PropertyHelper.h"
#include "PropertyList.h"


static DList::Root<QPropertyHelper>& GetQPropertyHelpersList()
{
    static DList::Root<QPropertyHelper> sDList;
    return sDList;
}

QPropertyHelper* QPropertyHelpersManager::GetHelper( UInt32 pPropertyType )
{
    DList::Root<QPropertyHelper>::Iterator itList(GetQPropertyHelpersList());

    for( ; itList.IsValid(); ++itList )
    {
        if( (*itList)->GetType() == pPropertyType )
            return (*itList);
    }

    return NULL;
}

void QPropertyHelpersManager::AddHelper( QPropertyHelper& pHelper )
{
    GetQPropertyHelpersList().AddTail(pHelper);
}

void QPropertyHelpersManager::RemoveHelper( QPropertyHelper& pHelper )
{
    GetQPropertyHelpersList().RemoveElement(pHelper);
}


QPropertyHelper::QPropertyHelper()
{
    // Automatic registration
    QPropertyHelpersManager::AddHelper(*this);
}

QPropertyHelper::~QPropertyHelper()
{
    // Automatic un-registration
    QPropertyHelpersManager::RemoveHelper(*this);
}
