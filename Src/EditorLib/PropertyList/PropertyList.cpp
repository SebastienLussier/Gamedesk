#include "EditorLib.h"
#include "PropertyList.h"
#include "PropertyHelper.h"

#include "World/Entity.h"


QPropertyModel::QPropertyModel(QObject* parent)
    : QAbstractItemModel(parent)
    , mEdited(NULL)
{
}

QPropertyModel::~QPropertyModel()
{
}

void QPropertyModel::SetEdited(Object* pEdited)
{
    mEdited = pEdited;
}

Object* QPropertyModel::GetEdited()
{
    return mEdited;
}

const Object* QPropertyModel::GetEdited() const
{
    return mEdited;
}

int QPropertyModel::rowCount(const QModelIndex& parentIndex) const
{
    if( !parentIndex.isValid() )
    {
        return mEdited ? mEdited->GetClass()->GetPropertyCount() : 0;
    }
    else
    {
        if( parentIndex.internalPointer() >= (void*)2048 )
        {
            Property* prop = static_cast<Property*>(parentIndex.internalPointer());
            return prop->GetComponentCount();
        }
        else
        {
            return 0;   
        }
    }
}

int QPropertyModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 2;
}

Qt::ItemFlags QPropertyModel::flags(const QModelIndex& index ) const
{
    if( !index.isValid() )
        return Qt::ItemIsEnabled;

    if( index.column() == PROPERTY_COLUMN )
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant QPropertyModel::data(const QModelIndex& index, int role) const
{
    if( !index.isValid() )
        return QVariant();

    if( role != Qt::DisplayRole && role != Qt::ToolTipRole && role != Qt::StatusTipRole )
        return QVariant();

    Property* prop = GetProperty(index);

    if( role == Qt::ToolTipRole || role == Qt::StatusTipRole )
        return QVariant(prop->GetDescription());

    if( index.column() == PROPERTY_COLUMN )
    {
        if( IsComponent(index) )
            return QVariant(prop->GetComponentName(index.row()));
        else
            return QVariant(prop->GetName());
    }
    else
    {
        String str;

        if( IsComponent(index) )
            prop->GetComponentValueString(mEdited, GetComponentIndex(index), str);  
        else
            prop->GetValueString(mEdited, str);

        return QVariant(str.c_str());
    }
}

bool QPropertyModel::setData( const QModelIndex& pIndex, const QVariant& pValue, int role )
{    
    if( !pIndex.isValid() )
        return false;

    if( role != Qt::DisplayRole )
        return false;

    dataChanged(pIndex, pIndex);

    Property* prop = QPropertyModel::GetProperty(pIndex);

    if( IsComponent(pIndex) )
    {
        dataChanged(pIndex.parent(), pIndex);
    }
    else if( prop->GetComponentCount() )
    {
        dataChanged(createIndex(0, pIndex.column(), pIndex.row()), 
                    createIndex(prop->GetComponentCount()-1, pIndex.column(), pIndex.row()));
    }
    
    return false;
}

QVariant QPropertyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( orientation != Qt::Horizontal )
        return QVariant();

    return section == PROPERTY_COLUMN ? QVariant("Property") : QVariant("Value");
}

QModelIndex QPropertyModel::index(int row, int column, const QModelIndex& parent) const
{
    if( !parent.isValid() )
    {
        if( !mEdited )
            return QModelIndex();

        Class::PropertyIterator itProp(mEdited->GetClass());
        for( int i = 0; i != row; i++ )
            ++itProp;

        return createIndex( row, column, *itProp );
    }
    else
    {
        return createIndex( row, column, parent.row() );
    }
}

QModelIndex QPropertyModel::parent(const QModelIndex& idx) const
{
    if( !idx.isValid() )
        return QModelIndex();

    if( !IsComponent(idx) )
        return QModelIndex();
    else
    {
        return index( (size_t)idx.internalPointer(), 0, QModelIndex() );
    }
}

Property* QPropertyModel::GetProperty(const QModelIndex& index)
{
    if( index.internalPointer() >= (void*)2048 )
        return static_cast<Property*>(index.internalPointer());
    else
        return static_cast<Property*>(index.parent().internalPointer());
}

Bool QPropertyModel::IsComponent(const QModelIndex& index)
{
    return index.internalPointer() < (void*)2048;
}

UInt32 QPropertyModel::GetComponentIndex(const QModelIndex& index)
{
    return IsComponent(index) ? (UInt32)index.row() : Property::InvalidComponent;
}



QPropertyDelegate::QPropertyDelegate(QObject* pParent)
    : QItemDelegate(pParent)
    , mActiveEditor(0)
{
}

QWidget* QPropertyDelegate::createEditor(QWidget* pParent, const QStyleOptionViewItem& pOption, const QModelIndex& pIndex) const
{
    Property* baseProp = QPropertyModel::GetProperty(pIndex);

    QPropertyDelegate* self = const_cast<QPropertyDelegate*>(this);
    QPropertyHelper* propHelper = QPropertyHelpersManager::GetHelper(baseProp->GetID());
    if( propHelper )
        self->mActiveEditor = propHelper->CreateEditor(self, pParent, pOption, baseProp, QPropertyModel::GetComponentIndex(pIndex));
    
    if( self->mActiveEditor )
        self->mActiveEditor->installEventFilter(self);
    
    return self->mActiveEditor;
}

void QPropertyDelegate::setEditorData(QWidget* pEditor, const QModelIndex& pIndex) const
{
    GD_ASSERT(pEditor == mActiveEditor);

    const QPropertyModel* model = reinterpret_cast<const QPropertyModel*>(pIndex.model());
    Property* baseProp = QPropertyModel::GetProperty(pIndex);
        
    QPropertyHelper* propHelper = QPropertyHelpersManager::GetHelper(baseProp->GetID());
    if( propHelper && model )
        propHelper->SetEditorData(pEditor, model->GetEdited(), baseProp, QPropertyModel::GetComponentIndex(pIndex));
}

void QPropertyDelegate::setModelData(QWidget* pEditor, QAbstractItemModel* pModel, const QModelIndex& pIndex) const
{
    GD_ASSERT(pEditor == mActiveEditor);

    if( !pIndex.isValid() )
        return;

    QPropertyModel* model = reinterpret_cast<QPropertyModel*>(pModel);
    Property* baseProp = QPropertyModel::GetProperty(pIndex);
    
    QPropertyHelper* propHelper = QPropertyHelpersManager::GetHelper(baseProp->GetID());
    if( propHelper && model )
        propHelper->SetModelData(pEditor, model->GetEdited(), baseProp, QPropertyModel::GetComponentIndex(pIndex));

    model->setData(pIndex, QVariant(), Qt::DisplayRole);
}

void QPropertyDelegate::updateEditorGeometry(QWidget* pEditor, const QStyleOptionViewItem& pOption, const QModelIndex& pIndex) const
{
    GD_ASSERT(pEditor == mActiveEditor);

    if( !pIndex.isValid() )
        return;

    pEditor->setGeometry(pOption.rect);
}

void QPropertyDelegate::paint(QPainter* pPainter, const QStyleOptionViewItem& pOption, const QModelIndex& pIndex) const
{
    Q_ASSERT(pIndex.isValid());

    // We don't need any specialized display for the "Property" column
    if( pIndex.column() == QPropertyModel::PROPERTY_COLUMN )
    {
        QItemDelegate::paint(pPainter, pOption, pIndex);
        return;
    }

    const QPropertyModel* model = reinterpret_cast<const QPropertyModel*>(pIndex.model());
    Property* baseProp = QPropertyModel::GetProperty(pIndex);
    QPropertyHelper* propHelper = QPropertyHelpersManager::GetHelper(baseProp->GetID());

    /*
    if( !QPropertyModel::IsComponent(pIndex) )
        printf("Painting %s\n", baseProp->GetName());
    else
        printf("Painting %s.%s\n", baseProp->GetName(), baseProp->GetComponentName(QPropertyModel::GetComponentIndex(pIndex)));
    */

    if( propHelper && model )
    {
        if( propHelper->Paint(pPainter, pOption, model->GetEdited(), baseProp, QPropertyModel::GetComponentIndex(pIndex)) )
            return;
    }
  
    QItemDelegate::paint(pPainter, pOption, pIndex);
}

QSize QPropertyDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_ASSERT(index.isValid());
    const QAbstractItemModel *model = index.model();
    Q_ASSERT(model);

    QVariant value = model->data(index, Qt::FontRole);
    QFont fnt = value.isValid() ? qvariant_cast<QFont>(value) : option.font;
    QString text = model->data(index, Qt::DisplayRole).toString();
    QRect pixmapRect;
    if (model->data(index, Qt::DecorationRole).isValid())
        pixmapRect = QRect(0, 0, option.decorationSize.width(),
        option.decorationSize.height());

    QFontMetrics fontMetrics(fnt);
    QRect textRect(0, 0, fontMetrics.width(text), fontMetrics.lineSpacing() + 3);
    QRect checkRect = check(option, textRect, model->data(index, Qt::CheckStateRole));
    doLayout(option, &checkRect, &pixmapRect, &textRect, true);

    return (pixmapRect|textRect|checkRect).size();
}

bool QPropertyDelegate::eventFilter(QObject *object, QEvent *event)
{
    QWidget *editor = ::qobject_cast<QWidget*>(object);

    if(!editor)
        return false;

    GD_ASSERT(editor == mActiveEditor);

    if(event->type() == QEvent::KeyPress) 
    {
        switch(static_cast<QKeyEvent *>(event)->key()) 
        {
        case Qt::Key_Tab:
            emit commitData(editor);
            emit closeEditor(editor, QAbstractItemDelegate::EditNextItem);
            mActiveEditor = 0;
            return true;
        case Qt::Key_Backtab:
            emit commitData(editor);
            emit closeEditor(editor, QAbstractItemDelegate::EditPreviousItem);
            mActiveEditor = 0;
            return true;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            emit commitData(editor);
            emit closeEditor(editor, QAbstractItemDelegate::SubmitModelCache);
            mActiveEditor = 0;
            return true;
        case Qt::Key_Escape:
            // don't commit data
            emit closeEditor(editor, QAbstractItemDelegate::RevertModelCache);
            mActiveEditor = 0;
            return true;
        default:
            break;
        }
    } 
    else if(event->type() == QEvent::FocusOut && !editor->isActiveWindow()) 
    {
#ifndef QT_NO_DRAGANDDROP
        // The window may loose focus during an drag operation.
        // i.e when dragging involves the task bar on Windows.
        //if(QDragManager::self() && QDragManager::self()->object != 0)
        //    return false;
#endif
        if( QApplication::activeModalWidget() && QApplication::activeModalWidget()->parent() == editor )
            return true;

        emit commitData(editor);
        emit closeEditor(editor, NoHint);
        mActiveEditor = 0;
        return true;
    }

    return false;
}

void QPropertyDelegate::editorChanged()
{
    GD_ASSERT(mActiveEditor);
    emit commitData(mActiveEditor);
}
