/**
 *  @file       ModelBrowserTool.h
 *  @brief      Tool used to display a list of models and load them..
 *  @author     Sébastien Lussier.
 *  @date       10/02/04.
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
#ifndef     _MODELBROWSERTOOL_H_
#define     _MODELBROWSERTOOL_H_


#include "EditorTool.h"
#include "QRenderWindow.h"

#include "Graphic/MeshHdl.h"
#include "Graphic/FontHdl.h"

#include "World/Camera.h"


class ConfigSection;
class ModelBrowserUI;
class Model3D;


/**
 *  
 *  @author Sébastien Lussier.
 *  @date   10/02/04
 */
class ModelBrowserTool : public EditorTool, public RenderListener
{
    DECLARE_CLASS(ModelBrowserTool,EditorTool);

public:
    ModelBrowserTool();
    virtual ~ModelBrowserTool();

    void Init();
    
    virtual void LoadSettings( ConfigSection& pSection );
    virtual void SaveSettings( ConfigSection& pSection ) const;

    void SetViewedModel( const String& pModelFilename );

    void Render();

protected:
    //! Create the ui window for the tool.
    virtual void CreateUI();
    //! Destroy the ui window for the tool.
    virtual void DestroyUI();

private:
	//! Set the camera position to focus on the viewed model.
	void SetCameraFocus();

    ModelBrowserUI*  mModelBrowserWindow;
    Camera           mCamera;
    Vector3f	     mObjectCenter;
    Model3D*         mModel;
};


#endif  //  _MODELBROWSERTOOL_H_
