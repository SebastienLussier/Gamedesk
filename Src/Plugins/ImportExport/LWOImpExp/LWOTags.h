/**
 *  @file       LWOTags.h
 *  @brief      Lightwave file tags.
 *  @author     Sébastien Lussier.
 *  @date       07/01/04.
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
#ifndef     _LWOTAGS_H_
#define     _LWOTAGS_H_


#define ID4(a,b,c,d)	((unsigned long) (d)<<24 | (unsigned long) (c)<<16 | (unsigned long) (b)<<8 | (unsigned long) (a))


// Universal IFF identifiers
const UInt32 ID_FORM		= ID4('F','O','R','M');
const UInt32 ID_LWO2		= ID4('L','W','O','2');

// PRIMARY CHUNK ID
const UInt32 ID_LAYR		= ID4('L','A','Y','R');
const UInt32 ID_PNTS		= ID4('P','N','T','S');
const UInt32 ID_VMAP		= ID4('V','M','A','P');
const UInt32 ID_VMAD		= ID4('V','M','A','D');
const UInt32 ID_POLS		= ID4('P','O','L','S');
const UInt32 ID_TAGS		= ID4('T','A','G','S');
const UInt32 ID_PTAG		= ID4('P','T','A','G');
const UInt32 ID_ENVL		= ID4('E','N','V','L');
const UInt32 ID_CLIP		= ID4('C','L','I','P');
const UInt32 ID_SURF		= ID4('S','U','R','F');
const UInt32 ID_BBOX		= ID4('B','B','O','X');
const UInt32 ID_DESC		= ID4('D','E','S','C');
const UInt32 ID_TEXT		= ID4('T','E','X','T');
const UInt32 ID_ICON		= ID4('I','C','O','N');

// POLS TYPE
const UInt32 ID_FACE		= ID4('F','A','C','E');
const UInt32 ID_CRVS		= ID4('C','U','R','V');
const UInt32 ID_PCHS		= ID4('P','T','C','H');
const UInt32 ID_MBAL		= ID4('M','B','A','L');
const UInt32 ID_BONE		= ID4('B','O','N','E');

// PTAG TYPE
//const UInt32 ID_SURF		= ID4('S','U','R','F');
const UInt32 ID_BNID		= ID4('B','N','I','D');
const UInt32 ID_SMGP		= ID4('S','M','G','P');
const UInt32 ID_PART		= ID4('P','A','R','T');

// IMAGE SUB-CHUNK ID
const UInt32 ID_STIL		= ID4('S','T','I','L');
const UInt32 ID_ISEQ		= ID4('I','S','E','Q');
const UInt32 ID_ANIM		= ID4('A','N','I','M');
const UInt32 ID_XREF		= ID4('X','R','E','F');
const UInt32 ID_STCC		= ID4('S','T','C','C');
const UInt32 ID_TIME        = ID4('T','I','M','E');
const UInt32 ID_CONT		= ID4('C','O','N','T');
const UInt32 ID_BRIT		= ID4('B','R','I','T');
const UInt32 ID_SATR		= ID4('S','A','T','R');
const UInt32 ID_HUE		    = ID4('H','U','E',' ');
const UInt32 ID_GAMM		= ID4('G','A','M','M');
const UInt32 ID_NEGA		= ID4('N','E','G','A');
const UInt32 ID_CROP		= ID4('C','R','O','P');
const UInt32 ID_ALPH		= ID4('A','L','P','H');
const UInt32 ID_COMP		= ID4('C','O','M','P');
const UInt32 ID_IFLT		= ID4('I','F','L','T');
const UInt32 ID_PFLT		= ID4('P','F','L','T');

// ENVELOPE SUB-CHUNK
const UInt32 ID_PRE		    = ID4('P','R','E',' ');
const UInt32 ID_POST		= ID4('P','O','S','T');
const UInt32 ID_KEY		    = ID4('K','E','Y',' ');
const UInt32 ID_SPAN		= ID4('S','P','A','N');
const UInt32 ID_CHAN		= ID4('C','H','A','N');

// VERTEX MAP
const UInt32 ID_PICK	    = ID4('P','I','C','K');
const UInt32 ID_WGHT	    = ID4('W','G','H','T');
const UInt32 ID_MNVW	    = ID4('M','N','V','W');
const UInt32 ID_TXUV	    = ID4('T','X','U','V');
const UInt32 ID_RGB 	    = ID4('R','G','B',' ');
const UInt32 ID_RGBA	    = ID4('R','G','B','A');
const UInt32 ID_MORF	    = ID4('M','O','R','F');
const UInt32 ID_SPOT	    = ID4('S','P','O','T');

// SURFACE SUB-CHUNK ID
const UInt32 ID_COLR		= ID4('C','O','L','R');
const UInt32 ID_DIFF		= ID4('D','I','F','F');
const UInt32 ID_LUMI		= ID4('L','U','M','I');
const UInt32 ID_SPEC		= ID4('S','P','E','C');
const UInt32 ID_REFL		= ID4('R','E','F','L');
const UInt32 ID_TRAN		= ID4('T','R','A','N');
const UInt32 ID_TRNL		= ID4('T','R','N','L');
const UInt32 ID_GLOS		= ID4('G','L','O','S');
const UInt32 ID_SHRP		= ID4('S','H','R','P');
const UInt32 ID_BUMP		= ID4('B','U','M','P');
const UInt32 ID_SIDE		= ID4('S','I','D','E');
const UInt32 ID_SMAN		= ID4('S','M','A','N');
const UInt32 ID_RFOP		= ID4('R','F','O','P');
const UInt32 ID_RIMG		= ID4('R','I','M','G');
const UInt32 ID_RSAN		= ID4('R','S','A','N');
const UInt32 ID_RBLR        = ID4('R','B','L','R');
const UInt32 ID_RIND		= ID4('R','I','N','D');
const UInt32 ID_CLRH		= ID4('C','L','R','H');
const UInt32 ID_TROP		= ID4('T','R','O','P');
const UInt32 ID_TIMG		= ID4('T','I','M','G');
const UInt32 ID_TBLR		= ID4('T','B','L','R');
const UInt32 ID_CLRF		= ID4('C','L','R','F');
const UInt32 ID_ADTR		= ID4('A','D','T','R');
const UInt32 ID_GLOW		= ID4('G','L','O','W');
const UInt32 ID_LINE		= ID4('L','I','N','E');
//const UInt32 ID_ALPH		= ID4('A','L','P','H');
const UInt32 ID_AVAL		= ID4('A','V','A','L');
const UInt32 ID_VCOL        = ID4('V','C','O','L');
const UInt32 ID_BLOK		= ID4('B','L','O','K');

// TEXTURE LAYER
//const UInt32 ID_CHAN		= ID4('C','H','A','N');
const UInt32 ID_TYPE		= ID4('T','Y','P','E');
const UInt32 ID_NAME		= ID4('N','A','M','E');
const UInt32 ID_ENAB		= ID4('E','N','A','B');
const UInt32 ID_OPAC		= ID4('O','P','A','C');
const UInt32 ID_FLAG		= ID4('F','L','A','G');
const UInt32 ID_PROJ		= ID4('P','R','O','J');
const UInt32 ID_STCK		= ID4('S','T','C','K');
const UInt32 ID_TAMP		= ID4('T','A','M','P');

// TEXTURE MAPPING
const UInt32 ID_TMAP		= ID4('T','M','A','P');
const UInt32 ID_AXIS		= ID4('A','X','I','S');
const UInt32 ID_CNTR		= ID4('C','N','T','R');
const UInt32 ID_SIZE		= ID4('S','I','Z','E');
const UInt32 ID_ROTA		= ID4('R','O','T','A');
const UInt32 ID_OREF		= ID4('O','R','E','F');
const UInt32 ID_FALL		= ID4('F','A','L','L');
const UInt32 ID_CSYS		= ID4('C','S','Y','S');

// IMAGE MAP
const UInt32 ID_IMAP		= ID4('I','M','A','P');
const UInt32 ID_IMAG		= ID4('I','M','A','G');
const UInt32 ID_WRAP		= ID4('W','R','A','P');
const UInt32 ID_WRPW		= ID4('W','R','P','W');
const UInt32 ID_WRPH		= ID4('W','R','P','H');
//const UInt32 ID_VMAP		= ID4('V','M','A','P');
const UInt32 ID_AAST		= ID4('A','A','S','T');
const UInt32 ID_PIXB		= ID4('P','I','X','B');

// PROCUDUAL TEXTURE
const UInt32 ID_PROC		= ID4('P','R','O','C');
//const UInt32 ID_COLR		= ID4('C','O','L','R');
const UInt32 ID_VALU		= ID4('V','A','L','U');
const UInt32 ID_FUNC		= ID4('F','U','N','C');
const UInt32 ID_FTPS		= ID4('F','T','P','S');
const UInt32 ID_ITPS		= ID4('I','T','P','S');
const UInt32 ID_ETPS		= ID4('E','T','P','S');

// GRADIENT
const UInt32 ID_GRAD		= ID4('G','R','A','D');
const UInt32 ID_GRST		= ID4('G','R','S','T');
const UInt32 ID_GREN		= ID4('G','R','E','N');

// SHADER PLUGIN
const UInt32 ID_SHDR		= ID4('S','H','D','R');
const UInt32 ID_DATA		= ID4('D','A','T','A');


#endif  //  _LWOTAGS_H_
