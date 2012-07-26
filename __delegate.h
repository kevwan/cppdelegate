/**
 * @author Kevin Wan <wanjunfeng@gmail.com>
 * @date   06/30/2005
 * Copyright (C) Kevin Wan
 */
   
namespace local
{
template <typename ReturnType TEMPLATE_LIST>
class ICallback<ReturnType(TYPE_LIST)>
{
    typedef ICallback<ReturnType(TYPE_LIST)> SelfType;

public:
    virtual ~ICallback() {}

    virtual ReturnType invoke(TYPE_LIST) const = 0;
    virtual bool equals(const SelfType* pDelegate) const = 0;
    virtual SelfType* clone() const = 0;
};

template <typename ReturnType TEMPLATE_LIST>
class NativeCallback<ReturnType(TYPE_LIST)>
    : public ICallback<ReturnType(TYPE_LIST)>
{
    typedef ICallback<ReturnType(TYPE_LIST)> SuperType;
    typedef NativeCallback<ReturnType(TYPE_LIST)> SelfType;
    typedef ReturnType (*FunctionPtr)(TYPE_LIST);

public:
    explicit NativeCallback(FunctionPtr ptr)
        : m_handler(ptr)
    {
    }

    NativeCallback(const SelfType& rhs)
        : ICallback<ReturnType(TYPE_LIST)>(rhs)
        , m_handler(rhs.m_handler)
    {
    }

    ReturnType invoke(TYPE_PARAM_LIST) const
    {
        return (*m_handler)(PARAM_LIST);
    }

    bool equals(const SuperType* pDelegate) const
    {
        const SelfType* pRhs = dynamic_cast<const SelfType*>(pDelegate);
        if (pRhs == 0) return false;
        if (m_handler == pRhs->m_handler) return true;
        return false;
    }

    SelfType* clone() const
    {
        return new SelfType(*this);
    }

private:
    FunctionPtr m_handler;
};

template <typename ObjectType, typename ReturnType TEMPLATE_LIST>
class MemberCallback<ObjectType, ReturnType(TYPE_LIST)>
    : public ICallback<ReturnType(TYPE_LIST)>
{
    typedef ICallback<ReturnType(TYPE_LIST)> SuperType;
    typedef MemberCallback<ObjectType, ReturnType(TYPE_LIST)> SelfType;
    typedef ReturnType (ObjectType::*FunctionPtr)(TYPE_LIST);
    typedef ReturnType (ObjectType::*ConstFunctionPtr)(TYPE_LIST) const;

    enum { CONST_POINTER, NEED_DELETE, DONT_DELETE };
    struct ObjectManager
    {
        bool equals(const ObjectManager& rhs)
        {
            return object.pObject == rhs.object.pObject;
        }

        union { ObjectType* pObject; const ObjectType* pConstObject; } object;
        int                 property;
        int                 refCount;
    };

public:
    MemberCallback(ObjectType* t, FunctionPtr ptr, bool needDelete = false)
        : m_isConstMemFunc(false)
    {
        m_pObjectManager = new ObjectManager();
        m_pObjectManager->object.pObject = t;
        m_pObjectManager->property = needDelete ? NEED_DELETE : DONT_DELETE;
        m_pObjectManager->refCount = 0;
        m_handler.ptr = ptr;
        incrementRefCount();
    }

    MemberCallback(ObjectType* t, ConstFunctionPtr ptr, bool needDelete = false)
        : m_isConstMemFunc(true)
    {
        m_pObjectManager = new ObjectManager();
        m_pObjectManager->object.pObject = t;
        m_pObjectManager->property = needDelete ? NEED_DELETE : DONT_DELETE;
        m_pObjectManager->refCount = 0;
        m_handler.constPtr = ptr;
        incrementRefCount();
    }

    MemberCallback(const ObjectType* t, ConstFunctionPtr ptr)
        : m_isConstMemFunc(true)
    {
        m_pObjectManager = new ObjectManager();
        m_pObjectManager->object.pConstObject = t;
        m_pObjectManager->property = CONST_POINTER;
        m_pObjectManager->refCount = 0;
        m_handler.constPtr = ptr;
        incrementRefCount();
    }

    MemberCallback(const SelfType& rhs)
        : ICallback<ReturnType(TYPE_LIST)>(rhs)
        , m_pObjectManager(rhs.m_pObjectManager)
        , m_handler(rhs.m_handler)
        , m_isConstMemFunc(rhs.m_isConstMemFunc)
    {
        incrementRefCount();
    }

    virtual ~MemberCallback()
    {
        decrementRefCount();
    }

    MemberCallback& operator=(const SelfType& rhs)
    {
        if (this == &rhs)
            return *this;
        decrementRefCount();
        m_pObjectManager = rhs.m_pObjectManager;
        m_handler = rhs.m_handler;
        m_isConstMemFunc = rhs.m_isConstMemFunc;
        incrementRefCount();
        return *this;
    }

    ReturnType invoke(TYPE_PARAM_LIST) const
    {
        if (m_isConstMemFunc)
        {
            if (m_pObjectManager->property == CONST_POINTER)
                return (m_pObjectManager->object.pConstObject->*(m_handler.constPtr))(PARAM_LIST);
            return (m_pObjectManager->object.pObject->*(m_handler.constPtr))(PARAM_LIST);
        }
        return (m_pObjectManager->object.pObject->*(m_handler.ptr))(PARAM_LIST);
    }

    bool equals(const SuperType* pDelegate) const
    {
        const SelfType* pRhs = dynamic_cast<const SelfType*>(pDelegate);
        if (pRhs == 0) return false;
        if (m_pObjectManager->equals(*pRhs->m_pObjectManager)
                && m_isConstMemFunc == pRhs->m_isConstMemFunc
                && m_handler.ptr == pRhs->m_handler.ptr)
            return true;
        return false;
    }

    SelfType* clone() const
    {
        return new SelfType(*this);
    }

private:
    void incrementRefCount()
    {
        ++m_pObjectManager->refCount;
    }

    void decrementRefCount()
    {
        if (--m_pObjectManager->refCount == 0)
        {
            if (m_pObjectManager->property == NEED_DELETE)
                delete m_pObjectManager->object.pObject;

            delete m_pObjectManager;
        }
    }

private:
    ObjectManager*  m_pObjectManager;
    union { FunctionPtr ptr; ConstFunctionPtr constPtr; } m_handler;
    bool            m_isConstMemFunc;
};
} /* namespace local */

template <typename ReturnType TEMPLATE_LIST, typename LockType>
class delegate<ReturnType(TYPE_LIST), LockType>
    : public local::ICallback<ReturnType(TYPE_LIST)>
    , public ThreadingModel<LockType>
{
    typedef local::ICallback<ReturnType(TYPE_LIST)> SuperType;
    typedef delegate<ReturnType(TYPE_LIST), LockType> SelfType;
    typedef ThreadingModel<LockType> ThreadingModelType;
    struct CallbackHolder
    {
        SuperType*      instance;
        CallbackHolder* prev;
        ReturnType call(TYPE_PARAM_LIST)
        {
            if (prev != 0) prev->call(PARAM_LIST);
            return instance->invoke(PARAM_LIST);
        }
    };

public:
    delegate() : m_pHolder(0) {}

    explicit delegate(ReturnType (*ptr)(TYPE_LIST))
        : m_pHolder(0)
    {
        this->add(ptr);
    }

    template <typename ObjectType>
    delegate(const ObjectType* t, ReturnType(ObjectType::*ptr)(TYPE_LIST) const)
        : m_pHolder(0)
    {
        this->add(t, ptr);
    }

    template <typename ObjectType>
    delegate(ObjectType* t, ReturnType(ObjectType::*ptr)(TYPE_LIST),
            bool needDelete = false)
        : m_pHolder(0)
    {
        this->add(t, ptr, needDelete);
    }

    template <typename ObjectType>
    delegate(ObjectType* t, ReturnType(ObjectType::*ptr)(TYPE_LIST) const,
            bool needDelete = false)
        : m_pHolder(0)
    {
        this->add(t, ptr, needDelete);
    }

    template <typename FunctorType>
    explicit delegate(const FunctorType* pFunctor)
        : m_pHolder(0)
    {
        this->add(pFunctor);
    }

    template <typename FunctorType>
    explicit delegate(FunctorType* pFunctor, bool needDelete = false)
        : m_pHolder(0)
    {
        this->add(pFunctor, needDelete);
    }

    delegate(const SelfType& rhs)
        : local::ICallback<ReturnType(TYPE_LIST)>(rhs)
        , ThreadingModelType()
    {
        copyFrom(rhs);
    }

    SelfType& operator=(const SelfType& rhs)
    {
        if (this == &rhs)
            return *this;
        this->release();
        copyFrom(rhs);
        return *this;
    }

    ~delegate()
    {
        release();
    }

    void release()
    {
        typename ThreadingModelType::Lock guard(*this);
        CallbackHolder* ptr = m_pHolder;
        while (ptr != 0)
        {
            CallbackHolder* prev = ptr->prev;
            delete ptr->instance;
            delete ptr;
            ptr = prev;
        }
        m_pHolder = 0;
    }

    ReturnType operator()(TYPE_PARAM_LIST)
    {
        return this->invoke(PARAM_LIST);
    }

    ReturnType invoke(TYPE_PARAM_LIST) const
    {
        typename ThreadingModelType::Lock guard(*this);
        if (m_pHolder == 0) return ReturnType();
        return m_pHolder->call(PARAM_LIST);
    }

    bool equals(const SuperType* pDelegate) const
    {
        const SelfType* pRhs = dynamic_cast<const SelfType*>(pDelegate);
        if (pRhs == 0) return false;

        SelfType* temp = 0;
        const SelfType* pClone;
        cloneForComparison(pRhs, pClone, temp, typename ThreadingModelType::ThreadTag());

        typename ThreadingModelType::Lock guard(*this);
        CallbackHolder* ptr1 = m_pHolder;
        CallbackHolder* ptr2 = pClone->m_pHolder;
        while (ptr1 != 0 && ptr2 != 0)
        {
            if (!ptr1->instance->equals(ptr2->instance))
                break;
            ptr1 = ptr1->prev;
            ptr2 = ptr2->prev;
        }
        delete temp;
        return (ptr1 == 0 && ptr2 == 0);
    }

    SelfType* clone() const
    {
        SelfType* pClone = new SelfType();
        typename ThreadingModelType::Lock guard(*this);
        CallbackHolder* ptr = m_pHolder;
        CallbackHolder* pReverse = 0;
        while (ptr != 0)
        {
            CallbackHolder* pHolder = new CallbackHolder();
            pHolder->instance = ptr->instance->clone();
            pHolder->prev = pReverse;
            pReverse = pHolder;
            ptr = ptr->prev;
        }

        CallbackHolder* prev = 0;
        while (pReverse != 0)
        {
            CallbackHolder* next = pReverse->prev;
            pReverse->prev = prev;
            prev = pReverse;
            pReverse = next;
        }
        pClone->m_pHolder = prev;

        return pClone;
    }

    void add(ReturnType (*ptr)(TYPE_LIST))
    {
        SuperType* pNew = new local::NativeCallback<ReturnType(TYPE_LIST)>(ptr);
        this->addImpl(pNew);
    }

    template <typename ObjectType>
    void add(const ObjectType* t, ReturnType(ObjectType::*ptr)(TYPE_LIST) const)
    {
        SuperType* pNew = new local::MemberCallback<ObjectType, ReturnType(TYPE_LIST)>(t, ptr);
        this->addImpl(pNew);
    }

    template <typename ObjectType>
    void add(ObjectType* t, ReturnType(ObjectType::*ptr)(TYPE_LIST), bool needDelete = false)
    {
        SuperType* pNew = new local::MemberCallback<ObjectType, ReturnType(TYPE_LIST)>
                (t, ptr, needDelete);
        this->addImpl(pNew);
    }

    template <typename ObjectType>
    void add(ObjectType* t, ReturnType(ObjectType::*ptr)(TYPE_LIST) const,
            bool needDelete = false)
    {
        SuperType* pNew = new local::MemberCallback<ObjectType, ReturnType(TYPE_LIST)>
            (t, ptr, needDelete);
        this->addImpl(pNew);
    }

    template <typename FunctorType>
    void add(FunctorType* pFunctor, bool needDelete = false)
    {
        this->add(pFunctor, &FunctorType::operator(), needDelete);
    }

    template <typename FunctorType>
    void add(const FunctorType* pFunctor)
    {
        this->add(pFunctor, &FunctorType::operator());
    }

    void add(const SelfType& rhs)
    {
        SelfType* pClone = rhs.clone();
        this->addImpl(pClone);
    }

    void remove(ReturnType (*ptr)(TYPE_LIST))
    {
        SuperType* pNew = new local::NativeCallback<ReturnType(TYPE_LIST)>(ptr);
        this->removeImpl(pNew);
    }

    template <typename ObjectType>
    void remove(const ObjectType* t, ReturnType(ObjectType::*ptr)(TYPE_LIST) const)
    {
        SuperType* pNew = new local::MemberCallback<ObjectType, ReturnType(TYPE_LIST)>(t, ptr);
        this->removeImpl(pNew);
    }

    template <typename ObjectType>
    void remove(ObjectType* t, ReturnType(ObjectType::*ptr)(TYPE_LIST), bool needDelete = false)
    {
        SuperType* pNew = new local::MemberCallback<ObjectType, ReturnType(TYPE_LIST)>
            (t, ptr, needDelete);
        this->removeImpl(pNew);
    }

    template <typename ObjectType>
    void remove(ObjectType* t, ReturnType(ObjectType::*ptr)(TYPE_LIST) const,
            bool needDelete = false)
    {
        SuperType* pNew = new local::MemberCallback<ObjectType, ReturnType(TYPE_LIST)>
            (t, ptr, needDelete);
        this->removeImpl(pNew);
    }

    template <typename FunctorType>
    void remove(FunctorType* pFunctor, bool needDelete = false)
    {
        this->remove(pFunctor, &FunctorType::operator(), needDelete);
    }

    template <typename FunctorType>
    void remove(const FunctorType* pFunctor)
    {
        this->remove(pFunctor, &FunctorType::operator());
    }

    void remove(const SelfType& rhs)
    {
        this->remove(rhs, typename ThreadingModelType::ThreadTag());
    }

private:
    void cloneForComparison(const SelfType* pRhs, const SelfType*& pClone,
            SelfType*& ptrForDelete, SingleThreadTag) const
    {
        pClone = pRhs;
        ptrForDelete = 0;
    }

    void cloneForComparison(const SelfType* pRhs, const SelfType*& pClone,
            SelfType*& ptrForDelete, MultiThreadTag) const
    {
            ptrForDelete = pRhs->clone();
            pClone = ptrForDelete;
    }

    void copyFrom(const SelfType& rhs)
    {
        SelfType* pClone = rhs.clone();
        m_pHolder = pClone->m_pHolder;
        pClone->m_pHolder = 0;
        delete pClone;
    }

    void remove(const SelfType& rhs, SingleThreadTag)
    {
        this->removeImpl(&rhs);
    }

    void remove(const SelfType& rhs, MultiThreadTag)
    {
        this->removeImpl(rhs.clone());
    }

    void addImpl(SuperType* pRhs)
    {
        typename ThreadingModelType::Lock guard(*this);
        CallbackHolder* pH = new CallbackHolder();
        pH->instance = pRhs;
        pH->prev = m_pHolder;
        m_pHolder = pH;
    }

    void removeImpl(const SuperType* pRhs)
    {
        typename ThreadingModelType::Lock guard(*this);
        CallbackHolder* ptr = m_pHolder;
        CallbackHolder* prev = 0;
        while (ptr != 0)
        {
            if (ptr->instance->equals(pRhs))
            {
                if (prev == 0) m_pHolder = ptr->prev;
                else prev->prev = ptr->prev;
                delete ptr->instance;
                delete ptr;
                break;
            }
            prev = ptr;
            ptr = ptr->prev;
        }
    }

    void removeImpl(SuperType* pRhs)
    {
        const SuperType* pConst = pRhs;
        this->removeImpl(pConst);
        delete pRhs;
    }

private:
    CallbackHolder* m_pHolder;
};
