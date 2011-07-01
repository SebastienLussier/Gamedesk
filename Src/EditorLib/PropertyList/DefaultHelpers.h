#ifndef     _DEFAULT_HELPERS_H_
#define     _DEFAULT_HELPERS_H_


#include "PropertyList.h"
#include "PropertyHelper.h"


template <class P, class T, typename W, int NSubdiv=255>
class QPropertyNumberHelper : public QPropertyHelper
{
public:
    virtual QWidget* CreateEditor(QPropertyDelegate* pDelegate, QWidget* pParent, const QStyleOptionViewItem& /*pOption*/, const Property* pProperty, UInt32 pComponentIdx) const
    {
        GD_ASSERT(pComponentIdx == Property::InvalidComponent);
        const P* prop = reinterpret_cast<const P*>(pProperty);
        
        if( prop->UseSlider() )
        {
            QSlider* editor = new QSlider(pParent);
            int min = prop->GetMinimum() * T(NSubdiv);
            int max = prop->GetMaximum() * T(NSubdiv);
            editor->setRange(min, max);
            editor->setPageStep(NSubdiv);
            editor->setTickInterval(1);
            editor->setSingleStep(1);
            editor->setOrientation(Qt::Horizontal);

            editor->connect(editor, SIGNAL(sliderMoved(int)), pDelegate, SLOT(editorChanged()));

            return editor;
        }
        else
        {
            W* editor = new W(pParent);
            editor->setMinimum((T)prop->GetMinimum());
            editor->setMaximum((T)prop->GetMaximum());

            if( strcmp(editor->metaObject()->className(), "QDoubleSpinBox") == 0 )
            {
                QDoubleSpinBox* spinbox = (QDoubleSpinBox*)editor;
                spinbox->setDecimals(4);
            }

            return editor;
        }
    }

public:

    virtual void SetEditorData(QWidget* pEditor, const Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const
    {
        GD_ASSERT(pComponentIdx == Property::InvalidComponent);
        const P* prop   = reinterpret_cast<const P*>(pProperty);

        if( pEditor->inherits("QSlider") )
        {
            QSlider* editor = static_cast<QSlider*>(pEditor);
            editor->setValue(prop->GetValue(pEditedObject) * T(NSubdiv));
        }
        else
        {
            W* editor = static_cast<W*>(pEditor);
            editor->setValue(prop->GetValue(pEditedObject));
        }
    }

    virtual void SetModelData(QWidget* pEditor, Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const
    {
        GD_ASSERT(pComponentIdx == Property::InvalidComponent);
        const P* prop = reinterpret_cast<const P*>(pProperty);

        if( pEditor->inherits("QSlider") )
        {
            QSlider* editor = static_cast<QSlider*>(pEditor);
            T value = (T)editor->value() / T(NSubdiv);
            prop->SetValue(pEditedObject, value);
        }
        else
        {
            W* editor = static_cast<W*>(pEditor);
            editor->interpretText();
            T value = (T)editor->value();
            prop->SetValue(pEditedObject, value);
        }
    }
};


class EDITORLIB_API QPropertyCharHelper : public QPropertyNumberHelper<PropertyChar, Char, QSpinBox>
{
    DECLARE_QPROPERTY_HELPER(QPropertyCharHelper, PropertyChar::ID());
};

class EDITORLIB_API QPropertyByteHelper : public QPropertyNumberHelper<PropertyByte, Byte, QSpinBox>
{
    DECLARE_QPROPERTY_HELPER(QPropertyByteHelper, PropertyByte::ID());
};

class EDITORLIB_API QPropertyInt16Helper : public QPropertyNumberHelper<PropertyInt16, Int16, QSpinBox>
{
    DECLARE_QPROPERTY_HELPER(QPropertyInt16Helper, PropertyInt16::ID());
};

class EDITORLIB_API QPropertyUInt16Helper : public QPropertyNumberHelper<PropertyUInt16, UInt16, QSpinBox>
{
    DECLARE_QPROPERTY_HELPER(QPropertyUInt16Helper, PropertyUInt16::ID());
};

class EDITORLIB_API QPropertyInt32Helper : public QPropertyNumberHelper<PropertyInt32, Int32, QSpinBox>
{
    DECLARE_QPROPERTY_HELPER(QPropertyInt32Helper, PropertyInt32::ID());
};

class EDITORLIB_API QPropertyUInt32Helper : public QPropertyNumberHelper<PropertyUInt32, UInt32, QSpinBox>
{
    DECLARE_QPROPERTY_HELPER(QPropertyUInt32Helper, PropertyUInt32::ID());
};

class EDITORLIB_API QPropertyInt64Helper : public QPropertyNumberHelper<PropertyInt64, Int64, QSpinBox>
{
    DECLARE_QPROPERTY_HELPER(QPropertyInt64Helper, PropertyInt64::ID());
};

class EDITORLIB_API QPropertyUInt64Helper : public QPropertyNumberHelper<PropertyUInt64, UInt64, QSpinBox>
{
    DECLARE_QPROPERTY_HELPER(QPropertyUInt64Helper, PropertyUInt64::ID());
};

class EDITORLIB_API QPropertyFloatHelper : public QPropertyNumberHelper<PropertyFloat, Float, QDoubleSpinBox>
{
    DECLARE_QPROPERTY_HELPER(QPropertyFloatHelper, PropertyFloat::ID());
};

class EDITORLIB_API QPropertyDoubleHelper : public QPropertyNumberHelper<PropertyDouble, Double, QDoubleSpinBox>
{
    DECLARE_QPROPERTY_HELPER(QPropertyDoubleHelper, PropertyDouble::ID());
};


template <typename TProp, typename TComponent, typename TComponentHelper>
class TPropertyComponentHelper : public QPropertyHelper
{
public:
    virtual QWidget* CreateEditor(QPropertyDelegate* pDelegate, QWidget* pParent, const QStyleOptionViewItem& pOption, const Property* pProperty, UInt32 pComponentIdx) const
    {
        if( pComponentIdx != Property::InvalidComponent )
        {
            TComponent tmpProperty("","", pProperty->GetOffset() + sizeof(TProp::ComponentType)*pComponentIdx);
            return TComponentHelper::Instance().CreateEditor(pDelegate, pParent, pOption, &tmpProperty, Property::InvalidComponent);
        }

        return 0;
    }

    virtual void SetEditorData(QWidget* pEditor, const Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const
    {
        if( pComponentIdx != Property::InvalidComponent )
        {
            TComponent tmpProperty("","", pProperty->GetOffset() + sizeof(TProp::ComponentType)*pComponentIdx);
            TComponentHelper::Instance().SetEditorData(pEditor, pEditedObject, &tmpProperty, Property::InvalidComponent);
        }
    }

    virtual void SetModelData(QWidget* pEditor, Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const
    {
        if( pComponentIdx != Property::InvalidComponent )
        {
            TComponent tmpProperty("","", pProperty->GetOffset() + sizeof(TProp::ComponentType)*pComponentIdx);
            TComponentHelper::Instance().SetModelData(pEditor, pEditedObject, &tmpProperty, Property::InvalidComponent);
        }
    }
};

class EDITORLIB_API QPropertyVector3fHelper : public TPropertyComponentHelper<PropertyVector3f, PropertyFloat, QPropertyFloatHelper>
{
    DECLARE_QPROPERTY_HELPER(QPropertyVector3fHelper, PropertyVector3f::ID());
};

class EDITORLIB_API QPropertyQuaternionfHelper : public TPropertyComponentHelper<PropertyQuaternionf, PropertyFloat, QPropertyFloatHelper>
{
    DECLARE_QPROPERTY_HELPER(QPropertyQuaternionfHelper, PropertyQuaternionf::ID());
};


class QColorWidget : public QWidget
{
public:
    //! Constructor.
    QColorWidget(QPropertyDelegate* pDelegate, QWidget* pParent) 
        : QWidget(pParent)
        , mDelegate(pDelegate)
    {
    }

    //! Destructor.
    virtual ~QColorWidget() {}

    const QColor& value() const
    {
        return mColor;
    }

    Color3f valueAsColor3() const
    {
        return ToColor3(mColor);
    }

    Color4f valueAsColor4() const
    {
        return ToColor4(mColor);
    }

    void setValue(const QColor& pColor)
    {
        mColor = pColor;
    }

    void setValue(const Color3f& pColor)
    {
        mColor = FromColor3(pColor);
    }

    void setValue(const Color4f& pColor)
    {
        mColor = FromColor4(pColor);
    }

    static Color3f ToColor3( QColor pColor )
    {
        Color3f color;
        color.R = pColor.redF();
        color.G = pColor.greenF();
        color.B = pColor.blueF();
        return color;
    }

    static Color4f ToColor4( QColor pColor )
    {
        Color4f color;
        color.R = pColor.redF();
        color.G = pColor.greenF();
        color.B = pColor.blueF();
        color.A = pColor.alphaF();
        return color;
    }    

    static QColor FromColor3( const Color3f& pColor )
    {
        QColor color;        
        color.setRgbF(pColor.R, pColor.G, pColor.B);
        return color;
    }

    static QColor FromColor4( const Color4f& pColor )
    {
        QColor color;        
        color.setRgbF(pColor.R, pColor.G, pColor.B, pColor.A);
        return color;
    }

    static void Paint(QPainter* pPainter, QRect rect, Color3f pColor)
    {
        Paint(pPainter, rect, FromColor3(pColor));
    }

    static void Paint(QPainter* pPainter, QRect rect, Color4f pColor)
    {
        Paint(pPainter, rect, FromColor4(pColor));
    }

    static void Paint(QPainter* pPainter, QRect rect, QColor pColor)
    {
        pPainter->fillRect( rect, QBrush(QColor(0,0,0)) );

        QRect colorRect = rect;
        colorRect.adjust(2, 2, -3, -2);

        pPainter->fillRect( colorRect, QBrush(pColor) );
    }

protected:
    void focusInEvent(QFocusEvent* pEvent)
    {
        static bool reentrant = false;

        if( reentrant )
            return;

        reentrant = true;
        
        QColor col = QColorDialog::getColor( mColor, this );
        if( col.isValid() )
            mColor = col;

        reentrant = false;

        pEvent->setAccepted(true);
        mDelegate->editorChanged();
    }

    void paintEvent(QPaintEvent* pEvent)
    {
        QPainter painter(this);
        Paint(&painter, rect(), mColor);

        pEvent->setAccepted(true);
    }

public:
    QColor mColor;
    QPropertyDelegate* mDelegate;    
};

class EDITORLIB_API QPropertyColor3fHelper : public TPropertyComponentHelper<PropertyColor3f, PropertyFloat, QPropertyFloatHelper>
{
    typedef TPropertyComponentHelper<PropertyColor3f, PropertyFloat, QPropertyFloatHelper> Super;
    DECLARE_QPROPERTY_HELPER(QPropertyColor3fHelper, PropertyColor3f::ID());

public:
    virtual QWidget* CreateEditor(QPropertyDelegate* pDelegate, QWidget* pParent, const QStyleOptionViewItem& pOption, const Property* pProperty, UInt32 pComponentIdx) const
    {
        if( pComponentIdx == Property::InvalidComponent )
        {
            return new QColorWidget(pDelegate, pParent);
        }
        else
        {
            PropertyFloat tmpProperty("","", pProperty->GetOffset() + sizeof(PropertyColor3f::ComponentType)*pComponentIdx);
            tmpProperty.SetMinimum(0);
            tmpProperty.SetMaximum(1);
            tmpProperty.UseSlider(true);
            return QPropertyFloatHelper::Instance().CreateEditor(pDelegate, pParent, pOption, &tmpProperty, Property::InvalidComponent);
        }
    }

    virtual void SetEditorData(QWidget* pEditor, const Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const
    {
        if( pComponentIdx != Property::InvalidComponent )
        {
            Super::SetEditorData(pEditor, pEditedObject, pProperty, pComponentIdx);
        }
        else
        {
            const PropertyColor3f* prop = reinterpret_cast<const PropertyColor3f*>(pProperty);
            QColorWidget* editor = static_cast<QColorWidget*>(pEditor);
            editor->setValue(prop->GetValue(pEditedObject));
        }
    }

    virtual void SetModelData(QWidget* pEditor, Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const
    {
        if( pComponentIdx != Property::InvalidComponent )
        {
            Super::SetModelData(pEditor, pEditedObject, pProperty, pComponentIdx);
        }
        else
        {
            const PropertyColor3f* prop = reinterpret_cast<const PropertyColor3f*>(pProperty);
            QColorWidget* editor = static_cast<QColorWidget*>(pEditor);
            prop->SetValue(pEditedObject, editor->valueAsColor3());
        }
    }

    virtual Bool Paint(QPainter* pPainter, const QStyleOptionViewItem& pOption, const Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const
    {
        if( pComponentIdx != Property::InvalidComponent )
            return false;

        const PropertyColor3f* prop = reinterpret_cast<const PropertyColor3f*>(pProperty);
        QColorWidget::Paint(pPainter, pOption.rect, prop->GetValue(pEditedObject));

        return true;
    }
};


class EDITORLIB_API QPropertyColor4fHelper : public TPropertyComponentHelper<PropertyColor4f, PropertyFloat, QPropertyFloatHelper>
{
    typedef TPropertyComponentHelper<PropertyColor4f, PropertyFloat, QPropertyFloatHelper> Super;
    DECLARE_QPROPERTY_HELPER(QPropertyColor4fHelper, PropertyColor4f::ID());

public:
    virtual QWidget* CreateEditor(QPropertyDelegate* pDelegate, QWidget* pParent, const QStyleOptionViewItem& pOption, const Property* pProperty, UInt32 pComponentIdx) const
    {
        if( pComponentIdx == Property::InvalidComponent )
        {
            return new QColorWidget(pDelegate, pParent);
        }
        else
        {
            PropertyFloat tmpProperty("","", pProperty->GetOffset() + sizeof(PropertyColor4f::ComponentType)*pComponentIdx);
            tmpProperty.SetMinimum(0);
            tmpProperty.SetMaximum(1);
            tmpProperty.UseSlider(true);
            return QPropertyFloatHelper::Instance().CreateEditor(pDelegate, pParent, pOption, &tmpProperty, Property::InvalidComponent);
        }
    }

    virtual void SetEditorData(QWidget* pEditor, const Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const
    {
        if( pComponentIdx != Property::InvalidComponent )
        {
            Super::SetEditorData(pEditor, pEditedObject, pProperty, pComponentIdx);
        }
        else
        {
            const PropertyColor4f* prop = reinterpret_cast<const PropertyColor4f*>(pProperty);
            QColorWidget* editor = static_cast<QColorWidget*>(pEditor);
            editor->setValue(prop->GetValue(pEditedObject));
        }
    }

    virtual void SetModelData(QWidget* pEditor, Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const
    {
        if( pComponentIdx != Property::InvalidComponent )
        {
            Super::SetModelData(pEditor, pEditedObject, pProperty, pComponentIdx);
        }
        else
        {
            const PropertyColor4f* prop = reinterpret_cast<const PropertyColor4f*>(pProperty);
            QColorWidget* editor  = static_cast<QColorWidget*>(pEditor);
            prop->SetValue(pEditedObject, editor->valueAsColor4());
        }
    }

    virtual Bool Paint(QPainter* pPainter, const QStyleOptionViewItem& pOption, const Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const
    {
        if( pComponentIdx != Property::InvalidComponent )
            return false;

        const PropertyColor4f* prop = reinterpret_cast<const PropertyColor4f*>(pProperty);
        QColorWidget::Paint(pPainter, pOption.rect, prop->GetValue(pEditedObject));

        return true;
    }
};


class EDITORLIB_API QPropertyBoolHelper : public QPropertyHelper
{
    DECLARE_QPROPERTY_HELPER(QPropertyBoolHelper, PropertyBool::ID());

public:
    virtual QWidget* CreateEditor(QPropertyDelegate* pDelegate, QWidget* pParent, const QStyleOptionViewItem& pOption, const Property* pProperty, UInt32 pComponentIdx) const
    {
        GD_ASSERT(pComponentIdx == Property::InvalidComponent);

        const PropertyBool* prop = reinterpret_cast<const PropertyBool*>(pProperty);
        QComboBox* editor = new QComboBox(pParent);

        editor->setEditable(false);
        editor->addItem("False", QVariant(0));
        editor->addItem("True", QVariant(1));
        
        return editor;
    }

    virtual void SetEditorData(QWidget* pEditor, const Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const
    {
        GD_ASSERT(pComponentIdx == Property::InvalidComponent);

        const PropertyBool* prop   = reinterpret_cast<const PropertyBool*>(pProperty);
        QComboBox* editor = static_cast<QComboBox*>(pEditor);

        editor->setCurrentIndex(prop->GetValue(pEditedObject) ? 1 : 0);
    }

    virtual void SetModelData(QWidget* pEditor, Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const
    {
        GD_ASSERT(pComponentIdx == Property::InvalidComponent);

        const PropertyBool* prop    = reinterpret_cast<const PropertyBool*>(pProperty);
        QComboBox* editor  = static_cast<QComboBox*>(pEditor);

        Bool value = editor->currentIndex() == 1;
        prop->SetValue(pEditedObject, value);
    }
};


class EDITORLIB_API QPropertyEnumHelper : public QPropertyHelper
{
    DECLARE_QPROPERTY_HELPER(QPropertyEnumHelper, PropertyEnum::ID());

public:
    virtual QWidget* CreateEditor(QPropertyDelegate* pDelegate, QWidget* pParent, const QStyleOptionViewItem& pOption, const Property* pProperty, UInt32 pComponentIdx) const
    {
        GD_ASSERT(pComponentIdx == Property::InvalidComponent);

        const PropertyEnum* prop = reinterpret_cast<const PropertyEnum*>(pProperty);
        QComboBox* editor = new QComboBox(pParent);

        editor->setEditable(false);

        for( UInt32 i = 0; i < prop->GetCount(); i++ )
            editor->addItem(prop->GetStringFromIndex(i), prop->GetValueFromIndex(i));

        return editor;
    }

    virtual void SetEditorData(QWidget* pEditor, const Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const
    {
        GD_ASSERT(pComponentIdx == Property::InvalidComponent);

        const PropertyEnum* prop   = reinterpret_cast<const PropertyEnum*>(pProperty);
        QComboBox* editor = static_cast<QComboBox*>(pEditor);

        editor->setCurrentIndex(prop->GetIndexFromValue(prop->GetValue(pEditedObject)));
    }

    virtual void SetModelData(QWidget* pEditor, Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const
    {
        GD_ASSERT(pComponentIdx == Property::InvalidComponent);

        const PropertyEnum* prop    = reinterpret_cast<const PropertyEnum*>(pProperty);
        QComboBox* editor  = static_cast<QComboBox*>(pEditor);

        prop->SetValue(pEditedObject, editor->itemData(editor->currentIndex()).toUInt());
    }
};


class EDITORLIB_API QPropertyStringHelper : public QPropertyHelper
{
    DECLARE_QPROPERTY_HELPER(QPropertyStringHelper, PropertyString::ID());

public:
    virtual QWidget* CreateEditor(QPropertyDelegate* pDelegate, QWidget* pParent, const QStyleOptionViewItem& pOption, const Property* pProperty, UInt32 pComponentIdx) const
    {
        GD_ASSERT(pComponentIdx == Property::InvalidComponent);

        const PropertyString* prop = reinterpret_cast<const PropertyString*>(pProperty);
        QLineEdit* editor = new QLineEdit(pParent);

        if( prop->GetMaxLength() != Number<UInt32>::Max )
            editor->setMaxLength(prop->GetMaxLength());

        return editor;
    }

    virtual void SetEditorData(QWidget* pEditor, const Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const
    {
        GD_ASSERT(pComponentIdx == Property::InvalidComponent);

        const PropertyString* prop   = reinterpret_cast<const PropertyString*>(pProperty);
        QLineEdit* editor = static_cast<QLineEdit*>(pEditor);

        editor->setText(prop->GetValue(pEditedObject).c_str());
    }

    virtual void SetModelData(QWidget* pEditor, Object* pEditedObject, const Property* pProperty, UInt32 pComponentIdx) const
    {
        GD_ASSERT(pComponentIdx == Property::InvalidComponent);

        const PropertyString* prop    = reinterpret_cast<const PropertyString*>(pProperty);
        QLineEdit* editor  = static_cast<QLineEdit*>(pEditor);

        prop->SetValue(pEditedObject, String(editor->text().toStdString()));
    }
};


#endif  //  _DEFAULT_HELPERS_H_
