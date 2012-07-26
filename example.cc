#include <stdio.h>
#include "delegate.h"

const char* bar = "##########################################";

void native_func(int value)
{
    printf("%s\n", bar);
    printf("native function, value = %d\n", value);
}

class Object
{
public:
    static void static_member_func(int value)
    {
        printf("%s\n", bar);
        printf("static member function, value = %d\n", value);
    }
    void non_const_member_func(int value)
    {
        printf("%s\n", bar);
        printf("non-const member function, value = %d\n", value);
    }
    void const_member_func(int value) const
    {
        printf("%s\n", bar);
        printf("const member function, value = %d\n", value);
    }
};

class Functor
{
public:
    void operator()(int value)
    {
        printf("%s\n", bar);
        printf("non-const functor, value = %d\n", value);
    }
};

class ConstFunctor
{
public:
    void operator()(int value) const
    {
        printf("%s\n", bar);
        printf("const functor, value = %d\n", value);
    }
};

int main()
{
    typedef wan::delegate<void(int)> MyDelegate;

    Object obj;
    Functor functor;
    ConstFunctor constFunctor;
    MyDelegate dele;
    dele.add(&native_func);
    dele.add(&Object::static_member_func);
    dele.add(&obj, &Object::non_const_member_func);
    dele.add(&obj, &Object::const_member_func);
    dele.add(&functor);
    dele.add(&constFunctor);
    dele(111);

    printf("%s\n", bar);
    printf("\n\nafter remove operations\n\n");

    dele.remove(&native_func);
    dele.remove(&obj, &Object::non_const_member_func);
    dele(222);

    printf("%s\n", bar);
    printf("\n\nadd delegate object to delegate object\n\n");

    MyDelegate temp;
    temp.add(&native_func);
    temp.add(&obj, &Object::non_const_member_func);
    dele.add(&temp);
    dele(333);

    printf("%s\n", bar);
}
