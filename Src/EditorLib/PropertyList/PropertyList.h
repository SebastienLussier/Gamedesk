#ifndef _PROPERTY_LIST_H_
#define _PROPERTY_LIST_H_


#include "../EditorLib.h"


class Entity;


class EDITORLIB_API QPropertyModel : public QAbstractItemModel
{
    Q_OBJECT
    friend class QPropertyDelegate;

public:
    enum
    {
        PROPERTY_COLUMN = 0,
        VALUE_COLUMN    = 1,
        NB_COLUMNS      = 2
    };

    QPropertyModel(QObject *parent = 0);
    ~QPropertyModel();

    void SetEdited(Object* pEdited);
    Object* GetEdited();
    const Object* GetEdited() const;

    // Reimplemented from QAbstractItemModel
    Qt::ItemFlags   flags(const QModelIndex &index) const;
    bool            setData( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    QVariant        data(const QModelIndex &index, int role) const;
    QVariant        headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    // Since we have a hierarchical model
    QModelIndex     index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex     parent(const QModelIndex &index) const;

    int             rowCount(const QModelIndex &parent = QModelIndex()) const;
    int             columnCount(const QModelIndex &parent = QModelIndex()) const;

    static Property* GetProperty(const QModelIndex& index);
    static Bool      IsComponent(const QModelIndex& index);
    static UInt32    GetComponentIndex(const QModelIndex& index);

private:
    Object* mEdited;
};


class EDITORLIB_API QPropertyDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    QPropertyDelegate(QObject *parent = 0);

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    bool eventFilter(QObject *object, QEvent *event);

public slots:
    void editorChanged();

private:
    QWidget* mActiveEditor;
};


class EDITORLIB_API QPropertyList : public QTreeView
{
public:
    QPropertyList()
    {
        setModel(&mModel);
        setItemDelegate(&mDelegate);

        setWindowTitle("Property Editor");
        setRootIsDecorated(false);
        setAlternatingRowColors(true);
        setEditTriggers(QAbstractItemView::CurrentChanged|QAbstractItemView::SelectedClicked);
    }

    virtual ~QPropertyList() 
    {
    }

    void SetEditedObject( Object* pEditedObject )
    {
        mModel.SetEdited(pEditedObject);
    }

private:
    QPropertyModel     mModel;
    QPropertyDelegate  mDelegate;
};


#endif
