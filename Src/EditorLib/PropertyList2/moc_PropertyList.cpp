/****************************************************************************
** Meta object code from reading C++ file 'PropertyList.h'
**
** Created: lun. 12. juin 21:51:10 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.1.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../PropertyList2/PropertyList.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PropertyList.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.1.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QPropertyModel[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_QPropertyModel[] = {
    "QPropertyModel\0"
};

const QMetaObject QPropertyModel::staticMetaObject = {
    { &QAbstractItemModel::staticMetaObject, qt_meta_stringdata_QPropertyModel,
      qt_meta_data_QPropertyModel, 0 }
};

const QMetaObject *QPropertyModel::metaObject() const
{
    return &staticMetaObject;
}

void *QPropertyModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPropertyModel))
	return static_cast<void*>(const_cast<QPropertyModel*>(this));
    return QAbstractItemModel::qt_metacast(_clname);
}

int QPropertyModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_QPropertyDelegate[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QPropertyDelegate[] = {
    "QPropertyDelegate\0\0editorChanged()\0"
};

const QMetaObject QPropertyDelegate::staticMetaObject = {
    { &QItemDelegate::staticMetaObject, qt_meta_stringdata_QPropertyDelegate,
      qt_meta_data_QPropertyDelegate, 0 }
};

const QMetaObject *QPropertyDelegate::metaObject() const
{
    return &staticMetaObject;
}

void *QPropertyDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPropertyDelegate))
	return static_cast<void*>(const_cast<QPropertyDelegate*>(this));
    return QItemDelegate::qt_metacast(_clname);
}

int QPropertyDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QItemDelegate::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: editorChanged(); break;
        }
        _id -= 1;
    }
    return _id;
}
