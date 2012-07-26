#include <stdio.h>
#include "delegate.h"
// #include "mutex.h"

struct DummyLock
{
    void lock() {}
    void unlock() {}
};

void print_hello()
{
    printf("print_hello\n");
}

void print_world()
{
    printf("print_world\n");
}

class Object
{
public:
    Object() { printf("Object::Object\n"); }
    ~Object() { printf("Object::~Object\n"); }
    void print() const
    {
        printf("in Object::print\n");
    }
    void test() const
    {
        wan::delegate<void()> d;
        d.add(this, &Object::print);
        d();
    }
};

class Functor
{
public:
    void operator()() const
    {
        printf("in Functor::operator() const\n");
    }
};

int main()
{
    {
        wan::delegate<void(), DummyLock> d;
        d.add(&print_hello);
        d.add(&print_world);
        if (d.equals(&d))
            printf("the same\n");
        else
            printf("not the same\n");
        printf("\n");
        wan::delegate<void(), DummyLock>* pd = d.clone();
        if (d.equals(pd))
            printf("the same\n");
        else
            printf("not the same\n");
        delete pd;
        printf("\n");
        d();
        wan::delegate<void(), DummyLock> d1(d);
        printf("%u\n", sizeof(d));
        d.add(d);
        d();
        printf("\n");
        d.remove(d1);
        d();
    }

    /*
    {
        wan::delegate<void(), Mutex> d;
        d.add(&print_hello);
        printf("%u\n", sizeof(d));
    }
    */

    {
        wan::delegate<void()> d;
        d.add(new Object(), &Object::print, true);
        d();
        wan::delegate<void()> d1(d);
        wan::delegate<void()>* d2 = d.clone();
        d1();
        (*d2)();
        delete d2;
    }

    {
        Object obj;
        obj.test();
    }

    {
        wan::delegate<void()> d;
        d.add(new Functor(), true);
        d();
        d = d;
        d();
    }

    {
        wan::local::MemberCallback<Functor, void()> md(new Functor(),
                &Functor::operator(), true);
        md.invoke();
        md = md;
        md.invoke();
    }
}
