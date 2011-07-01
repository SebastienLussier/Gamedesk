/**
*  @file       UIElement.cpp
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
#include "Engine.h"
#include "UIElement.h"


namespace Gamedesk {
	
	
//////////////////////////////////////////////////////////////////////////
// UIElement
UIElement::UIElement()
    : mParent(NULL)
    , mFlags(0)
{
}

UIElement::UIElement( UIElement* pParent )
    : mFlags(0)
{
    if( pParent )
        pParent->AddChild(this);
}

UIElement::~UIElement()
{
    for( List<UIElement*>::iterator itChild = mChilds.begin(); itChild != mChilds.end(); ++itChild )
        GD_DELETE(*itChild);
}

void UIElement::Draw()
{
    Paint();

    // Draw childs
    for( List<UIElement*>::iterator itChild = mChilds.begin(); itChild != mChilds.end(); ++itChild )
        (*itChild)->Draw();        
}

void UIElement::UpdateLayout()
{
    // Invalidate position/size
    for( List<UIElement*>::iterator itChild = mChilds.begin(); itChild != mChilds.end(); ++itChild )
        (*itChild)->ClearFlags( Flag_ValidPos );        

    UInt32 numResolved;
    do
    {
        numResolved = 0;

        for( List<UIElement*>::iterator itChild = mChilds.begin(); itChild != mChilds.end(); ++itChild )
        {
            UIElement* child = (*itChild);

            // Left
            if( !child->HasFlags(Flag_ValidLeft) && child->ResolveAttach(UI::Left) )
                numResolved++;

            // Bottom
            if( !child->HasFlags(Flag_ValidBottom) && child->ResolveAttach(UI::Bottom) )
                numResolved++;

            // Right
            if( !child->HasFlags(Flag_ValidRight) && child->ResolveAttach(UI::Right) )
                numResolved++;

            // Top
            if( !child->HasFlags(Flag_ValidTop) && child->ResolveAttach(UI::Top) )
                numResolved++;
        }
    } while( numResolved != 0 );

    // Update each child layout recursively
    for( List<UIElement*>::iterator itChild = mChilds.begin(); itChild != mChilds.end(); ++itChild )
    {
        // Only update if resolve was successful.
        if( (*itChild)->HasFlags(Flag_ValidPos) )
            (*itChild)->UpdateLayout();
    }
}

Bool UIElement::IsResizable( UI::Side pSide ) const
{
    return mConstraints[pSide].mAttach != this && mConstraints[OppositeSide(pSide)].mAttach != this;
}

Bool UIElement::ResolveAttach( UI::Side pSide )
{
    UIScalar numResizableElements = 0;
    UIScalar attachPos = 0;
    UIScalar usedSpace = 0;
    UIScalar usedSpaceOpp = 0;

    // Try to resolve our attach recursively. counting the number of resizable item on our way.
    if( !ResolveAttach( pSide, attachPos, numResizableElements, usedSpace, false ) )
        return false;

    // Resolve successful and no resizable elements were found
     if( numResizableElements == 0 )
        return true;
 
    UIScalar oppositeAttachPos = 0;

    // We found resizable items... so we'll have to iterate on the opposite side 
    // to look for a valid attach, and we'll count the number of resizable item there.
    if( !ResolveAttach( OppositeSide(pSide), oppositeAttachPos, numResizableElements, usedSpaceOpp, false ) )
        return false;

    if( !IsResizable(pSide) )
        usedSpace -= mConstraints[pSide].mAttach == this ? mConstraints[pSide].mSpacing : mConstraints[OppositeSide(pSide)].mSpacing;     // This element Was counted twiced.
    else
        numResizableElements--;                 // This element Was counted twiced.

    UIScalar availableSpace = oppositeAttachPos - attachPos - usedSpace + usedSpaceOpp;

    // Now do a second pass which will do the "real" resolve of all elements,
    // including resizable ones. No need to check return value as we know that
    // at this point resolve can't fail.
    ResolveAttach( pSide, attachPos, numResizableElements, availableSpace, true );
    availableSpace = -availableSpace;
    mConstraints[OppositeSide(pSide)].mAttach->ResolveAttach( OppositeSide(pSide), attachPos, numResizableElements, availableSpace, true );

    return true;
}

Bool UIElement::ResolveAttach( UI::Side pSide, UIScalar& pAttachPos, UIScalar& pNumResizableElements, UIScalar& pSpace, Bool pSecondPass )
{
    // If we have found an attach, stop recursion...
    if( HasFlags(SideToFlag(pSide)) )
    {
        pAttachPos = mRect[pSide];
        return true;
    }

    // If linked to itself... can’t go further!
    // TODO: try using the opposite attach, validate infinite recursion
    if( mConstraints[pSide].mAttach == this )
        return false;

    if( IsResizable(pSide) && !pSecondPass )
        pNumResizableElements++;

    if( !mConstraints[pSide].mAttach->ResolveAttach( pNumResizableElements > 0 ? pSide : mConstraints[pSide].mAttachSide, pAttachPos, pNumResizableElements, pSpace, pSecondPass ) )
        return false;

    if( !pSecondPass && pNumResizableElements != 0 )
    {
        pSpace += mConstraints[pSide].mSpacing;
        if( !IsResizable(pSide) )
            pSpace += mConstraints[pSide].mAttach == this ? mConstraints[pSide].mSpacing : mConstraints[OppositeSide(pSide)].mSpacing;
    }

    // On the way back, assign position directly.
    // If we have found resizable elements, we'll have to do that in a second pass.
    if( pNumResizableElements == 0 || pSecondPass )
    {
        pAttachPos += mConstraints[pSide].mSpacing;
        mRect[pSide] = pAttachPos;

        if( !IsResizable(pSide) )
            pAttachPos += mConstraints[pSide].mAttach == this ? mConstraints[pSide].mSpacing : mConstraints[OppositeSide(pSide)].mSpacing;
        else
            pAttachPos += pSpace / pNumResizableElements;

        mRect[OppositeSide(pSide)] = pAttachPos;

        SetFlags(SideToFlag(pSide) | SideToFlag(OppositeSide(pSide)));
    }

    return true;
}

void UIElement::AddChild( UIElement* pWidget )
{
    pWidget->mParent = this;
    mChilds.push_back(pWidget);
}

void UIElement::RemoveChild( UIElement* pWidget )
{
    mChilds.remove(pWidget);
}

UIElement* UIElement::GetParent()
{
    return mParent;
}

const UIElement* UIElement::GetParent() const
{
    return mParent;
}

void UIElement::SetWidth( UIScalar pWidth )
{
    if( !mConstraints[UI::Right].IsValid() )
    {
        mConstraints[UI::Right].mAttach     = this;
        mConstraints[UI::Right].mAttachSide = UI::Left;
        mConstraints[UI::Right].mSpacing    = pWidth;
    }
    else if( !mConstraints[UI::Left].IsValid() )
    {
        mConstraints[UI::Left].mAttach     = this;
        mConstraints[UI::Left].mAttachSide = UI::Right;
        mConstraints[UI::Left].mSpacing    = -pWidth;
    }   

    // Element is attached from both side, 
    // so Width is already specified indirectly.
}

void UIElement::SetHeight( UIScalar pHeight )
{
    if( !mConstraints[UI::Top].IsValid() )
    {
        mConstraints[UI::Top].mAttach     = this;
        mConstraints[UI::Top].mAttachSide = UI::Bottom;
        mConstraints[UI::Top].mSpacing    = pHeight;
    }
    else if( !mConstraints[UI::Bottom].IsValid() )
    {
        mConstraints[UI::Bottom].mAttach     = this;
        mConstraints[UI::Bottom].mAttachSide = UI::Top;
        mConstraints[UI::Bottom].mSpacing    = -pHeight;
    }   

    // Element is attached from both side, 
    // so Width is already specified indirectly.
}

void UIElement::SetSize( const UIPoint& pSize )
{
    SetWidth(pSize.x);
    SetHeight(pSize.y);
}

void UIElement::Attach( UI::Side pSide, UIElement* pAttach, UI::Side pAttachSide, UIScalar pSpacing )
{
    // If side was already linked to the opposite side (to specify a fixed width/height)
    // Try to relink to the opposite side.
    UI::Side oppSide = OppositeSide(pSide);

    if( mConstraints[pSide].mAttach == this && !mConstraints[oppSide].IsValid() )
    {   
        mConstraints[oppSide].mAttach     = this;
        mConstraints[oppSide].mAttachSide = pSide;
        mConstraints[oppSide].mSpacing    = -mConstraints[pSide].mSpacing;
    }

    mConstraints[pSide] = UIConstraint( pAttach, pAttachSide, pSpacing );
}

void UIElement::AttachParent( UI::Side pSide, UI::Side pAttachSide, UIScalar pSpacing )
{
    Attach( pSide, mParent, pAttachSide, pSpacing );
}


//////////////////////////////////////////////////////////////////////////
// UIElement::UIConstraint
UIElement::UIConstraint::UIConstraint()
: mAttach(NULL)
{
}

UIElement::UIConstraint::UIConstraint( UIElement* pAttach, UI::Side pAttachSide, UIScalar pSpacing )
: mAttach(pAttach)
, mAttachSide(pAttachSide)
, mSpacing(pSpacing)
{
    GD_ASSERT_M(mAttach != NULL, "[UIConstraint::UIConstraint] Invalid attach specified" );
}

UIElement::UIConstraint::UIConstraint( const UIElement::UIConstraint& pOther )
{
    (*this) = pOther;
}

const UIElement::UIConstraint& UIElement::UIConstraint::operator = ( const UIElement::UIConstraint& pOther )
{
    mAttach = pOther.mAttach;
    mAttachSide = pOther.mAttachSide;
    mSpacing = pOther.mSpacing;

    return *this;
}

Bool UIElement::UIConstraint::IsValid() const
{
    return mAttach != NULL;
}


} // namespace Gamedesk
