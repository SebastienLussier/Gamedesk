/**
 *  @file       EditorLib.h
 *  @brief      Editor classes used by the Editor itself and it's plugins.
 *  @author     Sébastien Lussier.
 *  @date       12/12/03.
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
#ifndef     _EDITORLIB_H_
#define     _EDITORLIB_H_


#ifndef EDITORLIB_API
#define EDITORLIB_API DLL_IMPORT
#endif


// Movie recording
#include <windows.h>
#include <vfw.h>

// Qt Includes...
#pragma warning( push )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4512 )
#include <QAbstractItemModel>
#include <qaction.h>
#include <qapplication.h>
#include <qbuffer.h>
#include <qcheckbox.h> 
#include <qcombobox.h>
#include <qcolordialog.h>
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <qfiledialog.h>
#include <qitemdelegate.h>
#include <QKeyEvent>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistwidget.h>
#include <qmainwindow.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <QModelIndex>
#include <qslider.h> 
#include <qpainter.h>
#include <qpushbutton.h>
#include <qstatusbar.h>
#include <qstyle.h> 
#include <qstring.h>
#include <qstringlist.h>
#include <qtimer.h>
#include <qtreeview.h>
#include <QVariant>
#include <qwidget.h>
#include <qworkspace.h>
#pragma warning( pop )

#include "Engine.h"


#endif  //  _EDITORLIB_H_
