#ifndef     _UI_BASE_H_
#define     _UI_BASE_H_


#include "Maths/Vector2.h"
#include "Graphic/Color4.h"


namespace Gamedesk {


typedef Float               UIReal;
typedef Int32               UIScalar;
typedef Vector2<UIScalar>   UIPoint;
typedef Color4f             UIColor;


class UILine
{
public:
    UILine()
    {
    }

    UILine( UIPoint p1, UIPoint p2 )
        : mP1(p1)
        , mP2(p2)
    {
    }

    UILine( UIScalar pX1, UIScalar pY1, UIScalar pX2, UIScalar pY2 )
        : mP1(pX1, pY1)
        , mP2(pX2, pY2)
    {
    }

    UIPoint     mP1;
    UIPoint     mP2;
};


class UIRect
{
public:
    UIRect()
    {
    }

    UIRect( UIPoint pP1, UIPoint pP2 )
        : mP1(pP1)
        , mP2(pP2)
    {
    }

    UIRect( UIScalar pX1, UIScalar pY1, UIScalar pX2, UIScalar pY2 )
        : mP1(pX1, pY1)
        , mP2(pX2, pY2)
    {
    }

	UIScalar& operator [] ( UIScalar pIndex )
	{
		return ((UIScalar*)this)[pIndex];
	}

	const UIScalar& operator [] ( UIScalar pIndex ) const
	{
		return ((UIScalar*)this)[pIndex];
	}

    Bool Contains( const UIPoint& pPoint ) const
    {
        return pPoint.x >= mP1.x && pPoint.y >= mP1.y && pPoint.x <= mP2.x && pPoint.y <= mP2.y;
    }

    UIRect Resize( UIScalar pX1, UIScalar pY1, UIScalar pX2, UIScalar pY2 ) const
    {
        return UIRect( mP1.x + pX1, mP1.y + pY1, mP2.x + pX2, mP2.y + pY2 );
    }
    
    UIScalar& Left()                { return mP1.x; }
    const UIScalar& Left() const    { return mP1.x; }

    UIScalar& Bottom()              { return mP1.y; }
    const UIScalar& Bottom() const  { return mP1.y; }

    UIScalar& Right()               { return mP2.x; }
    const UIScalar& Right() const   { return mP2.x; }

    UIScalar& Top()                 { return mP2.y; }
    const UIScalar& Top() const     { return mP2.y; }

    UIPoint BottomLeft() const      { return UIPoint(mP1.x, mP1.y); }
    UIPoint BottomRight() const     { return UIPoint(mP2.x, mP1.y); }
    UIPoint TopLeft() const         { return UIPoint(mP1.x, mP2.y); }
    UIPoint TopRight() const        { return UIPoint(mP2.x, mP2.y); }

public:
	UIPoint     mP1;
	UIPoint     mP2;
};


// Forward declarations
class UIElement;

namespace UI
{

enum Side
{
    Left,
    Bottom,
    Right,
    Top,        
    Side_NUM
};

enum Direction
{
    Horizontal      = 0x00000001,
    Vertical        = 0x00000002
};

enum MouseButton
{
    MouseButton_Left    = 0x00000001,     //!< Left mouse button.
    MouseButton_Right   = 0x00000002,     //!< Right mouse button.
    MouseButton_Middle  = 0x00000004      //!< Middle mouse button.
};

}

enum UIAlignment
{
    AlignLeft       = 0x00000001,
    AlignRight      = 0x00000002,
    AlignHCenter    = 0x00000004,
    AlignTop        = 0x00000008,
    AlignBottom     = 0x00000010,
    AlignVCenter    = 0x00000020,
    AlignCenter     = AlignHCenter | AlignVCenter
};


//! Used to generate unique id for UI classes (see DECLARE_UI_CLASS)
class ENGINE_API UITypeManager
{
public:
    static UInt32 CreateClassID()
    {
        static UInt32 mNextId = 1;
        return mNextId++;
    }
};


#define DECLARE_UI_CLASS_ID()                                   \
    public:                                                     \
    virtual UInt32 GetClassID() const                           \
    {                                                           \
        return ClassID();                                       \
    }                                                           \
    static UInt32 ClassID()                                     \
    {                                                           \
        static const UInt32 mID = UITypeManager::CreateClassID();\
        return mID;                                             \
    }

#define DECLARE_BASE_UI_CLASS( thisClass )                      \
    DECLARE_UI_CLASS_ID()                                       \
    virtual Bool IsA( UInt32 pClassID ) const                   \
    {                                                           \
        if( ClassID() == pClassID )                             \
            return true;                                        \
        else                                                    \
            return false;                                       \
    }

//! Use this macro to create a new ui class with simple run time type identification.
#define DECLARE_UI_CLASS( thisClass, superClass )               \
    DECLARE_UI_CLASS_ID()                                       \
    public:                                                     \
    typedef superClass Super;                                   \
    virtual Bool IsA( UInt32 pClassID ) const                   \
    {                                                           \
        if( ClassID() == pClassID )                             \
            return true;                                        \
        else                                                    \
            return Super::IsA( pClassID );                      \
    }


} // namespace Gamedesk


#endif  //  _UI_BASE_H_
