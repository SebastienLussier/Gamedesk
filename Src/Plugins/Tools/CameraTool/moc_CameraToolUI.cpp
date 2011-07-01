/****************************************************************************
** CameraToolUI meta object code from reading C++ file 'CameraToolUI.h'
**
** Created: Mon Feb 9 03:00:12 2004
**      by: The Qt MOC ($Id: //depot/qt/main/src/moc/moc.y#178 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_CameraToolUI
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "CameraToolUI.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *CameraToolUI::className() const
{
    return "CameraToolUI";
}

QMetaObject *CameraToolUI::metaObj = 0;

void CameraToolUI::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("CameraToolUI","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString CameraToolUI::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("CameraToolUI",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* CameraToolUI::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(CameraToolUI::*m1_t0)(int);
    m1_t0 v1_0 = Q_AMPERSAND CameraToolUI::OnCameraActivated;
    QMetaData *slot_tbl = QMetaObject::new_metadata(1);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(1);
    slot_tbl[0].name = "OnCameraActivated(int)";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Private;
    metaObj = QMetaObject::new_metaobject(
	"CameraToolUI", "QWidget",
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}
