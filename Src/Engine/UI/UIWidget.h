#ifndef     _UI_WIDGET_H_
#define     _UI_WIDGET_H_


#include "UIElement.h"
#include "Patterns/Delegate.h"


namespace Gamedesk {
	

class UIWidget;


template <typename EventArgs>
class UIEvent
{
public:
    typedef Delegate2<UIWidget*, EventArgs> Handler;

public:
    UIEvent& operator += ( Handler& pHandler )
    {
        mDelegates.push_back( pHandler );
    }

    void operator () ( UIWidget* sender, const EventArgs& args )
    {
        for( typename List<Handler>::iterator it = mDelegates.begin(); it != mDelegates.end(); ++it )
            (*it)( sender, args );
    }

private:
    List<Handler>   mDelegates;
};


class UIEventArgs
{
};

class UIMouseEventArgs : public UIEventArgs
{
public:
    UIMouseEventArgs( const UIPoint& pMousePosition )
        : mMousePosition(pMousePosition)
    {
    }

public:
    UIPoint mMousePosition;
};

class UIMouseMoveEventArgs : public UIMouseEventArgs
{
public:
    UIMouseMoveEventArgs( const UIPoint& pMousePosition )
        : UIMouseEventArgs(pMousePosition)
    {
    }
};

typedef UIEvent<UIMouseMoveEventArgs> UIMouseMoveEvent;


class UIMousePressEventArgs : public UIMouseEventArgs
{
public:
    UIMousePressEventArgs( const UI::MouseButton& pMouseButton, const UIPoint& pMousePosition )
        : UIMouseEventArgs(pMousePosition)
        , mMouseButton(pMouseButton)
    {
    }

public:
    UI::MouseButton     mMouseButton;
};

class UIMouseReleaseEventArgs : public UIMouseEventArgs
{
public:
    UIMouseReleaseEventArgs( const UI::MouseButton& pMouseButton, const UIPoint& pMousePosition )
        : UIMouseEventArgs(pMousePosition)
        , mMouseButton(pMouseButton)
    {
    }

public:
    UI::MouseButton     mMouseButton;
};

class UIMouseEnterEventArgs : public UIMouseMoveEventArgs
{
public:
    UIMouseEnterEventArgs( const UIPoint& pMousePosition )
        : UIMouseMoveEventArgs(pMousePosition)
    {
    }
};

class UIMouseLeaveEventArgs : public UIMouseMoveEventArgs   
{
public:
    UIMouseLeaveEventArgs( const UIPoint& pMousePosition )
        : UIMouseMoveEventArgs(pMousePosition)
    {
    }
};


class UIWidget : public UIElement
{
    DECLARE_UI_CLASS(UIWidget, UIElement);

public:
    class ConstChildIterator
    {
        friend class UIWidget;
        
    public:
        ConstChildIterator& operator ++ ()
        {
            while( it != mParent->mChilds.end() && !((*it)->IsA(UIWidget::ClassID())) )
                ++it;

            return *this;
        }

        ConstChildIterator& operator -- ()
        {
            while( it != mParent->mChilds.begin() && !((*it)->IsA(UIWidget::ClassID())) )
                --it;

            return *this;
        }

        const UIWidget* operator * () const
        {
            return (UIWidget*)*it;
        }

        operator Bool() const
        {
            return it != mParent->mChilds.end();
        }

    protected:
        ConstChildIterator( List<UIElement*>::const_iterator pIt, const UIWidget* pParent )
            : it(pIt)
            , mParent(pParent)
        {
            if( !(*it)->IsA(UIWidget::ClassID()) )
                operator++();
        }    

    private:
        List<UIElement*>::const_iterator it;
        const UIWidget* mParent;
    };

    class ChildIterator
    {
        friend class UIWidget;

    public:
        ChildIterator& operator ++ ()
        {
            if( it != mParent->mChilds.end() )
                ++it;

            while( it != mParent->mChilds.end() && !((*it)->IsA(UIWidget::ClassID())) )
                ++it;

            return *this;
        }

        ChildIterator& operator -- ()
        {
            if( it != mParent->mChilds.begin() )
                --it;

            while( it != mParent->mChilds.begin() && !((*it)->IsA(UIWidget::ClassID())) )
                --it;

            return *this;
        }

        UIWidget* operator * ()
        {
            return (UIWidget*)*it;
        }

        operator Bool() const
        {
            return it != mParent->mChilds.end();
        }

    protected:
        ChildIterator( List<UIElement*>::iterator pIt, UIWidget* pParent )
            : it(pIt)
            , mParent(pParent)
        {
            if( it != mParent->mChilds.end() && !(*it)->IsA(UIWidget::ClassID()) )
                operator++();
        }    
        
    private:
        List<UIElement*>::iterator it;
        UIWidget* mParent;
    };

    UIWidget();
    UIWidget( UIElement* pParent );

    UIWidget* GetParentWidget();
    const UIWidget* GetParentWidget() const;

    Bool IsMouseOver() const;

    ChildIterator BeginChilds()            {   return ChildIterator( mChilds.begin(), this );    }
    ConstChildIterator BeginChilds() const {   return ConstChildIterator( mChilds.begin(), this );    }

    ChildIterator EndChilds()              {   return ChildIterator( mChilds.end(), this );      }
    ConstChildIterator EndChilds() const   {   return ConstChildIterator( mChilds.end(), this );      }

    virtual Bool EventMouseEnter( const UIMouseEnterEventArgs& pEventArgs );
    virtual Bool EventMouseLeave( const UIMouseLeaveEventArgs& pEventArgs );
    virtual Bool EventMousePress( const UIMousePressEventArgs& pEventArgs );
    virtual Bool EventMouseRelease( const UIMouseReleaseEventArgs& pEventArgs );

protected:
    virtual void Paint();    

private:
    Bool mEnabled;
    Bool mVisible;
};


} // namespace Gamedesk


#endif  //  _UI_WIDGET_H_
