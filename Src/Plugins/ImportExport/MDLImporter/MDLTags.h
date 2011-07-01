/**
 *  @file       MDLTags.h
 *  @brief      Defines strings and integer constants associated with each section and parameter found in MDL files.
 *  @author     Sébastien Lussier.
 *  @date       05/02/04.
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
#ifndef     _MDL_TAGS_H_
#define     _MDL_TAGS_H_


namespace NWN
{

static const String  FILEDEPENDANCY        = "filedependancy";
static const UInt32  FILEDEPENDANCY_ID     = 0;

static const String  NEWMODEL              = "newmodel";
static const UInt32  NEWMODEL_ID           = 1;

static const String  DONEMODEL             = "donemodel";
static const UInt32  DONEMODEL_ID          = 2;

static const String  SETSUPERMODEL         = "setsupermodel";
static const UInt32  SETSUPERMODEL_ID      = 3;

static const String  CLASSIFICATION        = "classification";
static const UInt32  CLASSIFICATION_ID     = 4;

static const String  SETANIMATIONSCALE     = "setanimationscale";
static const UInt32  SETANIMATIONSCALE_ID  = 5;

static const String  BEGINMODELGEOM        = "beginmodelgeom";
static const UInt32  BEGINMODELGEOM_ID     = 6;

static const String  ENDMODELGEOM          = "endmodelgeom";
static const UInt32  ENDMODELGEOM_ID       = 7;

static const String  NEWANIM               = "newanim";
static const UInt32  NEWANIM_ID            = 8;

static const String  DONEANIM              = "doneanim";
static const UInt32  DONEANIM_ID           = 9;

static const String  NODE                  = "node";
static const UInt32  NODE_ID               = 10;

static const String  ENDNODE               = "endnode";
static const UInt32  ENDNODE_ID            = 11;

static const String  BITMAP                = "bitmap";
static const UInt32  BITMAP_ID             = 12;

static const String  VERTS                 = "verts";
static const UInt32  VERTS_ID              = 13;

static const String  FACES                 = "faces";
static const UInt32  FACES_ID              = 14;

static const String  TVERTS                = "tverts";
static const UInt32  TVERTS_ID             = 15;

static const String  PARENT                = "parent";
static const UInt32  PARENT_ID             = 16;

static const String  POSITION              = "position";
static const UInt32  POSITION_ID           = 17;

static const String  ORIENTATION           = "orientation";
static const UInt32  ORIENTATION_ID        = 18;

static const String  WIRECOLOR             = "wirecolor";
static const UInt32  WIRECOLOR_ID          = 19;

static const String  AMBIENT               = "ambient";
static const UInt32  AMBIENT_ID            = 20;

static const String  DIFFUSE               = "diffuse";
static const UInt32  DIFFUSE_ID            = 21;

static const String  SPECULAR              = "specular";
static const UInt32  SPECULAR_ID           = 22;

static const String  SHININESS             = "shininess";
static const UInt32  SHININESS_ID          = 23;

static const String  LENGTH                = "length";
static const UInt32  LENGTH_ID             = 24;

static const String  TRANSTIME             = "transtime";
static const UInt32  TRANSTIME_ID          = 25;

static const String  ANIMROOT              = "animroot";
static const UInt32  ANIMROOT_ID           = 26;

static const String  POSITIONKEY           = "positionkey";
static const UInt32  POSITIONKEY_ID        = 27;

static const String  ORIENTATIONKEY        = "orientationkey";
static const UInt32  ORIENTATIONKEY_ID     = 28;

static const String  ENDLIST               = "endlist";
static const UInt32  ENDLIST_ID            = 29;

static const String  EVENT                 = "event";
static const UInt32  EVENT_ID              = 30;

static const String  COLORS                = "colors";
static const UInt32  COLORS_ID             = 31;

static const String  SCALE                 = "scale";
static const UInt32  SCALE_ID              = 32;

static const String  SELFILLUMCOLOR        = "selfillumcolor";
static const UInt32  SELFILLUMCOLOR_ID     = 33;

static const String  ALPHA                 = "alpha";
static const UInt32  ALPHA_ID              = 34;

static const String  TRANSPARENCY_HINT     = "transparencyhint";
static const UInt32  TRANSPARENCY_HINT_ID  = 341;

static const String  CONSTRAINTS           = "constraints";
static const UInt32  CONSTRAINTS_ID        = 35;

static const String  COLORSTART            = "colorStart";
static const UInt32  COLORSTART_ID         = 37;

static const String  COLOREND              = "colorEnd";
static const UInt32  COLOREND_ID           = 38;

static const String  ALPHASTART            = "alphaStart";
static const UInt32  ALPHASTART_ID         = 39;

static const String  ALPHAEND              = "alphaEnd";
static const UInt32  ALPHAEND_ID           = 40;

static const String  SIZESTART             = "sizeStart";
static const UInt32  SIZESTART_ID          = 41;

static const String  SIZEEND               = "sizeEnd";
static const UInt32  SIZEEND_ID            = 42;

static const String  SIZESTART_Y           = "sizeStart_y";
static const UInt32  SIZESTART_Y_ID        = 43;

static const String  SIZESEND_Y            = "sizeEnd_y";
static const UInt32  SIZESEND_Y_ID         = 44;

static const String  FRAMESTART            = "frameStart";
static const UInt32  FRAMESTART_ID         = 45;

static const String  FRAMEEND              = "frameEnd";
static const UInt32  FRAMEEND_ID           = 46;

static const String  BIRTHRATE             = "birthrate";
static const UInt32  BIRTHRATE_ID          = 47;

static const String  LIFEEXP               = "lifeExp";
static const UInt32  LIFEEXP_ID            = 48;

static const String  MASS                  = "mass";
static const UInt32  MASS_ID               = 49;

static const String  SPREAD                = "spread";
static const UInt32  SPREAD_ID             = 50;

static const String  VELOCITY              = "velocity";
static const UInt32  VELOCITY_ID           = 51;

static const String  RANDVEL               = "randvel";
static const UInt32  RANDVEL_ID            = 52;

static const String  FPS                   = "fps";
static const UInt32  FPS_ID                = 53;

static const String  BLEND                 = "Blend";
static const UInt32  BLEND_ID              = 54;

static const String  TEXTURE               = "texture";
static const UInt32  TEXTURE_ID            = 55;

static const String  XGRID                 = "xgrid";
static const UInt32  XGRID_ID              = 56;

static const String  YGRID                 = "ygrid";
static const UInt32  YGRID_ID              = 57;

static const String  PARTICLEROT           = "particleRot";
static const UInt32  PARTICLEROT_ID        = 58;

static const String  XSIZE                 = "xsize";
static const UInt32  XSIZE_ID              = 59;

static const String  YSIZE                 = "ysize";
static const UInt32  YSIZE_ID              = 60;

static const String  RENDER                = "render";
static const UInt32  RENDER_ID             = 61;

static const String  P2P_TYPE              = "p2p_type";
static const UInt32  P2P_TYPE_ID           = 62;

static const String  DRAG                  = "drag";
static const UInt32  DRAG_ID               = 63;

static const String  GRAV                  = "grav";
static const UInt32  GRAV_ID               = 64;

static const String  INHERIT               = "inherit";
static const UInt32  INHERIT_ID            = 65;

static const String  INHERIT_LOCAL         = "inherit_local";
static const UInt32  INHERIT_LOCAL_ID      = 66;

static const String  INHERIT_PART          = "inherit_part";
static const UInt32  INHERIT_PART_ID       = 67;

static const String  UPDATE                = "update";
static const UInt32  UPDATE_ID             = 68;

static const String  BIRTHRATEKEY          = "birthratekey";
static const UInt32  BIRTHRATEKEY_ID       = 69;

static const String  TILEFADE              = "tilefade";
static const UInt32  TILEFADE_ID           = 70;

static const String  TRIMESH               = "trimesh";
static const UInt32  TRIMESH_ID            = 1000;

static const String  DANGLY_MESH           = "danglymesh";
static const UInt32  DANGLY_MESH_ID        = 1001;

static const String  AABB                  = "aabb";
static const UInt32  AABB_ID               = 1002;

static const String  DUMMY                 = "dummy";
static const UInt32  DUMMY_ID              = 1003;

static const String  PATCH                 = "patch";
static const UInt32  PATCH_ID              = 1004;

static const String  EMITTER               = "emitter";
static const UInt32  EMITTER_ID            = 1005;

static const String  REFERENCE             = "reference";
static const UInt32  REFERENCE_ID          = 1006;

static const String  LIGHT                 = "light";
static const UInt32  LIGHT_ID              = 1007;

static const String  SKIN                  = "skin";
static const UInt32  SKIN_ID               = 1008;


static const UInt32  UNKNOWN_ID            = 0xFFFFFFFF;


};


#endif  //  _MDL_TAGS_H_
