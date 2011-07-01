#ifndef     _DLIST_H_
#define     _DLIST_H_


#define DECLARE_DLIST(CLASSNAME)            \
    public:                                 \
        DList::Node<CLASSNAME>  __mDList;


namespace Gamedesk {


class DList
{
public:
    template <typename T>
    class Node
    {
    public:
        Node()
        { 
            Reset(); 
        }
        
        ~Node()                     
        { 
            GD_ASSERT(!IsLinked()); 
        }

        void Reset(void)            
        { 
            mPrev = mNext = 0;
        }
        
        Bool IsLinked() const   
        {
            return mPrev || mNext;
        }

        T* mNext;
        T* mPrev;
    };

    template <typename T>
    static void Reset(T& node)          
    { 
        node.__mDList.Reset();
    }

    template <typename T>
    static Bool IsLinked(const T& node) 
    { 
        return node.__mDList.IsLinked();
    }
   
    template <typename T>
    class Root
    {
    public:
        Root() 
            : mHead(0)
            , mTail(0)     
        {
        }

        void AddHead(T& pNewNode)    
        {
            GD_ASSERT(!IsLinked(pNewNode));

            if(mHead) 
                InsertBefore(*mHead, pNewNode);
            else 
                mHead = mTail = &pNewNode;
        }

        void AddTail(T& pNewNode)    
        {                                               
            GD_ASSERT(!IsLinked(pNewNode));

            if(mTail) 
                InsertAfter(*mTail, pNewNode);
            else 
                mHead = mTail = &pNewNode;
        }

        void InsertBefore(T& pCurrentNode, T& pNewNode)
        {                                                
            GD_ASSERT(!IsLinked(pNewNode));

            pNewNode.__mDList.mNext     = &pCurrentNode;
            pNewNode.__mDList.mPrev     = pCurrentNode.__mDList.mPrev;
            pCurrentNode.__mDList.mPrev = &pNewNode;

            if(pNewNode.__mDList.mPrev)
                pNewNode.__mDList.mPrev->__mDList.mNext = &pNewNode;
            else 
                mHead = &pNewNode;
        }

        void InsertAfter(T& pCurrentNode, T& pNewNode)
        {
            GD_ASSERT(!IsLinked(pNewNode));

            pNewNode.__mDList.mPrev     = &pCurrentNode;
            pNewNode.__mDList.mNext     = pCurrentNode.__mDList.mNext;
            pCurrentNode.__mDList.mNext = &pNewNode;

            if(pNewNode.__mDList.mNext)
                pNewNode.__mDList.mNext->__mDList.mPrev = &pNewNode;
            else
                mTail = &pNewNode;
        }

        T* RemoveHead()         
        {                                                 
            T* Result = mHead;

            if(mHead) 
            {   
                mHead = mHead->__mDList.mNext;

                if(mHead) 
                    mHead->__mDList.mPrev = NULL;
                else
                    mTail = NULL;   

                Result->__mDList.Reset();
            }       

            return Result;
        }

        T* RemoveTail()         
        { 
            T* Result = mTail;

            if(mTail) 
            {
                mTail = mTail->__mDList.mPrev;

                if(mTail) 
                    mTail->__mDList.mNext = NULL;
                else 
                    mHead = NULL;

                Result->__mDList.Reset();
            }

            return Result;
        }

        void RemoveElement(T& pOldNode) 
        {         
            if(pOldNode.__mDList.mPrev) 
                pOldNode.__mDList.mPrev->__mDList.mNext = pOldNode.__mDList.mNext;
            else 
                mHead = pOldNode.__mDList.mNext;

            if(pOldNode.__mDList.mNext) 
                pOldNode.__mDList.mNext->__mDList.mPrev = pOldNode.__mDList.mPrev;
            else 
                mTail = pOldNode.__mDList.mPrev;

            pOldNode.__mDList.Reset();
        }                       


        Bool IsEmpty() const            
        { 
            return mHead == NULL;
        }

        Bool FindNode(const T& pToFind)    
        {
            T* Index = mHead;

            while((Index) && (Index != &pToFind)) 
                Index = Index->__mDList.mNext;

            return Index != NULL;
        }

        T* GetHead() const      
        { 
            return mHead; 
        }

        T* GetTail() const      
        { 
            return mTail; 
        }       

    private:
        T*  mHead;            
        T*  mTail;            

    public:
        class Iterator
        {
        public:	
            Iterator(Root<T>& pHead) 
                : mHead(&pHead)      
            { 
                Begin(); 
            }

            void Initialize(Root<T>& pHead)                
            { 
                mHead = &pHead; 
                Begin(); 
            }

            void Begin()                                                
            { 
                mIndex = mHead->GetHead();  
            }


            void End()                                                  
            { 
                mIndex = mHead->GetTail();  
            }

            Bool IsValid() const                                        
            { 
                return mIndex != NULL; 
            }

            T* operator*() const                                
            { 
                return mIndex; 
            }

            Iterator& operator++ ()         
            {
                GD_ASSERT(mIndex);
                mIndex = mIndex->__mDList.mNext;
                return *this;
            } 

            Iterator& operator-- ()         
            {
                GD_ASSERT(mIndex); 
                mIndex = mIndex->__mDList.mPrev;
                return *this;
            } 

            Iterator& operator-= (UInt32 pCount)  
            {
                GD_ASSERT(mIndex); 

                if(pCount > 0)
                {
                    while(pCount && mIndex) 
                    { 
                        mIndex = mIndex->__mDList.mPrev; 
                        pCount--;
                    }
                }
                else
                {
                    while(pCount && mIndex) 
                    {
                        mIndex = mIndex->__mDList.mPrev; 
                        pCount++;
                    }                                                
                }
                return *this;
            } 

            Iterator&  operator+= (UInt32 pCount)  
            {
                GD_ASSERT(mIndex); 

                if(pCount > 0)
                {
                    while(pCount && mIndex) 
                    {
                        mIndex = mIndex->__mDList.mNext; 
                        pCount--;
                    }
                }
                else
                {
                    while(pCount && mIndex) 
                    { 
                        mIndex = mIndex->__mDList.mNext; 
                        pCount++;
                    }
                }
                return *this;
            } 

            Bool operator== (const Iterator& pOther) const
            {
                return mIndex == pOther.mIndex;
            }

            Bool operator!= (const Iterator& pOther) const
            {
                return !(*this == pOther);
            }

            Bool FindForward(const T& pToFind)
            {
                while(!IsEmpty() && (mIndex != &pToFind))
                {
                    ++(*this);
                }
                return(!IsEmpty());
            }

            Bool FindReverse(const T& pToFind)
            {
                while(!IsEmpty() && (mIndex != &pToFind))
                {
                    --(*this);
                }
                return(!IsEmpty());
            }

            T* RemoveCurrent()
            {
                GD_ASSERT(mIndex);

                T* Tmp = mIndex;
                ++(*this);
                mHead->RemoveElement(*Tmp);

                return(Tmp);
            }

        private:
            Root<T>*      mHead;
            T*            mIndex;
        };
    };
};


} // namespace Gamedesk


#endif  //  _DLIST_H_
