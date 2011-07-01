#ifndef     _PROPERTY_HELPER_H_
#define     _PROPERTY_HELPER_H_


#include "Containers/DList.h"


#define DECLARE_QPROPERTY_HELPER(Helper, Type)      \
            public:                                 \
                virtual UInt32 GetType() const      \
                {                                   \
                    return Type;                    \
                }                                   \
                static Helper& Instance()           \
                {                                   \
                    return mSelf;                   \
                }                                   \
            private:                                \
                static Helper mSelf

#define IMPLEMENT_QPROPERTY_HELPER(Helper)          \
            Helper Helper::mSelf


class QPropertyDelegate;

/**
 *
 */
class EDITORLIB_API QPropertyHelper
{
    DECLARE_DLIST(QPropertyHelper);

public:
    virtual UInt32 GetType() const = 0;

    virtual QWidget* CreateEditor(QPropertyDelegate* pDelegate, QWidget* pParent, const QStyleOptionViewItem& pOption, const Property* pProperty, UInt32 pComponentIdx) const = 0;
    virtual void SetEditorData(QWidget* pEditor, const Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const = 0;
    virtual void SetModelData(QWidget* pEditor, Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const = 0;

    virtual Bool Paint(QPainter* pPainter, const QStyleOptionViewItem& pOption, const Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const
    {
        return false;
    }

protected:
    // Disable creation from the outside...
    QPropertyHelper();
    virtual ~QPropertyHelper();
};


/**
 *
 */
class EDITORLIB_API QPropertyHelpersManager
{
    friend class QPropertyHelper;

public:
    static QPropertyHelper* GetHelper( UInt32 pPropertyType );
        
private:
    static void AddHelper( QPropertyHelper& pHelper );
    static void RemoveHelper( QPropertyHelper& pHelper );
};


#endif  //  _PROPERTY_HELPER_H_
