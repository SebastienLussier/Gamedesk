/**
*  @file       UIElement.h
*  @brief	   Base of all UI elements.
*  @author     Sébastien Lussier.
*  @date       11/07/05.
*/
/*
*  Copyright (C) 2005 Gamedesk
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
#ifndef     _UI_ELEMENT_H_
#define     _UI_ELEMENT_H_


#include "UIBase.h"


namespace Gamedesk {


class UIElement
{
    DECLARE_BASE_UI_CLASS(UIElement);

public:
    UIElement();
    UIElement( UIElement* pParent );
    virtual ~UIElement();

    virtual void Draw();
    virtual void UpdateLayout();

    void AddChild( UIElement* pWidget );
    void RemoveChild( UIElement* pWidget );

    UIElement* GetParent();
    const UIElement* GetParent() const;

    UIScalar GetWidth() const     { return mRect.mP2.x - mRect.mP1.x; }
    UIScalar GetHeight() const    { return mRect.mP2.y - mRect.mP1.y; }
    UIPoint  GetSize() const      { return mRect.mP2 - mRect.mP1; }
    UIPoint  GetPosition() const  { return mRect.mP1; }
    const UIRect& GetRect() const { return mRect; } 

    Bool IsResizable( UI::Side pSide ) const;

    void SetWidth( UIScalar pWidth );
    void SetHeight( UIScalar pHeight );
    void SetSize( const UIPoint& pSize );

    void Attach( UI::Side pSide, UIElement* pAttach, UI::Side pAttachSide, UIScalar pSpacing );
    void AttachParent( UI::Side pSide, UI::Side pAttachSide, UIScalar pSpacing );

    Bool HasFlags( UInt32 pFlags )      { return (mFlags & pFlags) == pFlags; }
    void SetFlags( UInt32 pFlags )      { mFlags |= pFlags; }
    void ClearFlags( UInt32 pFlags )    { mFlags &= ~pFlags; }

protected:
    UIElement( const UIElement& pParent );                      // Disable copy!
    const UIElement& operator = ( const UIElement& pElement );  // Disable copy!

    virtual void Paint() = 0;
    
    Bool ResolveAttach( UI::Side pSide );
    Bool ResolveAttach( UI::Side pSide, UIScalar& pAttachPos, UIScalar& pNumResizableElements, UIScalar& pSpace, Bool pSecondPass );

    static UI::Side OppositeSide( UI::Side pSide )
    {
        return UI::Side( pSide < UI::Right ? pSide + UI::Right : pSide - UI::Right );
    }

    static UInt32 SideToFlag( UI::Side pSide )
    {
        static const UInt32 FLAGS[] = { 1, 2, 4, 8 };
        return FLAGS[pSide];
    }   

protected:
    enum Flags
    {
        Flag_None               = 0x00000000,
        Flag_ValidLeft          = 0x00000001,
        Flag_ValidBottom        = 0x00000002,
        Flag_ValidRight         = 0x00000004,
        Flag_ValidTop           = 0x00000008,
        Flag_ValidPos           = Flag_ValidLeft | Flag_ValidRight | Flag_ValidTop | Flag_ValidBottom,
        Flag_MouseOver          = 0x00000010
    };

    class UIConstraint
    {
    public:
        UIConstraint();
        UIConstraint( UIElement* pAttach, UI::Side pAttachSide, UIScalar pSpacing );
        UIConstraint( const UIConstraint& pOther );
        const UIConstraint& operator = ( const UIConstraint& pOther );

        Bool     IsValid() const;
        Bool     CanBeResolved() const;
        UIScalar Resolve();

        UIElement*  mAttach;
        UI::Side        mAttachSide;
        UIScalar    mSpacing;
    };

protected:
    UIElement*              mParent;    //!< Parent.
    List<UIElement*>        mChilds;    //!< List of children.

    //! Constraints are used to update position and size automatically.
    UIConstraint            mConstraints[UI::Side_NUM]; 
    UIRect                  mRect;      //!< Computed using constraints, can't be set directly.
    UInt32                  mFlags;
};


} // namespace Gamedesk


#endif  //  _UI_ELEMENT_H_
