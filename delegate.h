#ifndef WAN_DELEGATE_H
#define WAN_DELEGATE_H
   
/**
 * @author Kevin Wan <wanjunfeng@gmail.com>
 * @date   06/30/2005
 * Copyright (C) Kevin Wan
 */
#include "threadingmodel.h"
   
namespace wan
{
namespace local
{
template <typename> class ICallback;
template <typename> class NativeCallback;
template <typename, typename> class MemberCallback;
} // namespace local

template <typename, typename LockType = void> class delegate;

#define TEMPLATE_LIST_0
#define TEMPLATE_LIST_1 TEMPLATE_LIST_0, typename T0
#define TEMPLATE_LIST_2 TEMPLATE_LIST_1, typename T1
#define TEMPLATE_LIST_3 TEMPLATE_LIST_2, typename T2
#define TEMPLATE_LIST_4 TEMPLATE_LIST_3, typename T3
#define TEMPLATE_LIST_5 TEMPLATE_LIST_4, typename T4
#define TEMPLATE_LIST_6 TEMPLATE_LIST_5, typename T5

#define TYPE_LIST_0
#define TYPE_LIST_1 T0
#define TYPE_LIST_2 TYPE_LIST_1, T1
#define TYPE_LIST_3 TYPE_LIST_2, T2
#define TYPE_LIST_4 TYPE_LIST_3, T3
#define TYPE_LIST_5 TYPE_LIST_4, T4
#define TYPE_LIST_6 TYPE_LIST_5, T5

#define TYPE_PARAM_LIST_0
#define TYPE_PARAM_LIST_1 T0 t0
#define TYPE_PARAM_LIST_2 TYPE_PARAM_LIST_1, T1 t1
#define TYPE_PARAM_LIST_3 TYPE_PARAM_LIST_2, T2 t2
#define TYPE_PARAM_LIST_4 TYPE_PARAM_LIST_3, T3 t3
#define TYPE_PARAM_LIST_5 TYPE_PARAM_LIST_4, T4 t4
#define TYPE_PARAM_LIST_6 TYPE_PARAM_LIST_5, T5 t5

#define PARAM_LIST_0
#define PARAM_LIST_1 t0
#define PARAM_LIST_2 PARAM_LIST_1, t1
#define PARAM_LIST_3 PARAM_LIST_2, t2
#define PARAM_LIST_4 PARAM_LIST_3, t3
#define PARAM_LIST_5 PARAM_LIST_4, t4
#define PARAM_LIST_6 PARAM_LIST_5, t5

#define TEMPLATE_LIST 	TEMPLATE_LIST_0
#define TYPE_LIST     	TYPE_LIST_0
#define TYPE_PARAM_LIST TYPE_PARAM_LIST_0
#define PARAM_LIST		PARAM_LIST_0
#include "__delegate.h"
#undef TEMPLATE_LIST
#undef TYPE_LIST
#undef TYPE_PARAM_LIST
#undef PARAM_LIST

#define TEMPLATE_LIST 	TEMPLATE_LIST_1
#define TYPE_LIST     	TYPE_LIST_1
#define TYPE_PARAM_LIST TYPE_PARAM_LIST_1
#define PARAM_LIST		PARAM_LIST_1
#include "__delegate.h"
#undef TEMPLATE_LIST
#undef TYPE_LIST
#undef TYPE_PARAM_LIST
#undef PARAM_LIST

#define TEMPLATE_LIST 	TEMPLATE_LIST_2
#define TYPE_LIST     	TYPE_LIST_2
#define TYPE_PARAM_LIST TYPE_PARAM_LIST_2
#define PARAM_LIST		PARAM_LIST_2
#include "__delegate.h"
#undef TEMPLATE_LIST
#undef TYPE_LIST
#undef TYPE_PARAM_LIST
#undef PARAM_LIST

#define TEMPLATE_LIST 	TEMPLATE_LIST_3
#define TYPE_LIST     	TYPE_LIST_3
#define TYPE_PARAM_LIST TYPE_PARAM_LIST_3
#define PARAM_LIST		PARAM_LIST_3
#include "__delegate.h"
#undef TEMPLATE_LIST
#undef TYPE_LIST
#undef TYPE_PARAM_LIST
#undef PARAM_LIST

#define TEMPLATE_LIST 	TEMPLATE_LIST_4
#define TYPE_LIST     	TYPE_LIST_4
#define TYPE_PARAM_LIST TYPE_PARAM_LIST_4
#define PARAM_LIST		PARAM_LIST_4
#include "__delegate.h"
#undef TEMPLATE_LIST
#undef TYPE_LIST
#undef TYPE_PARAM_LIST
#undef PARAM_LIST

#define TEMPLATE_LIST 	TEMPLATE_LIST_5
#define TYPE_LIST     	TYPE_LIST_5
#define TYPE_PARAM_LIST TYPE_PARAM_LIST_5
#define PARAM_LIST		PARAM_LIST_5
#include "__delegate.h"
#undef TEMPLATE_LIST
#undef TYPE_LIST
#undef TYPE_PARAM_LIST
#undef PARAM_LIST

#define TEMPLATE_LIST 	TEMPLATE_LIST_6
#define TYPE_LIST     	TYPE_LIST_6
#define TYPE_PARAM_LIST TYPE_PARAM_LIST_6
#define PARAM_LIST		PARAM_LIST_6
#include "__delegate.h"
#undef TEMPLATE_LIST
#undef TYPE_LIST
#undef TYPE_PARAM_LIST
#undef PARAM_LIST

// remove the macros
#undef TEMPLATE_LIST_0
#undef TEMPLATE_LIST_1
#undef TEMPLATE_LIST_2
#undef TEMPLATE_LIST_3
#undef TEMPLATE_LIST_4
#undef TEMPLATE_LIST_5
#undef TEMPLATE_LIST_6

#undef TYPE_LIST_0
#undef TYPE_LIST_1
#undef TYPE_LIST_2
#undef TYPE_LIST_3
#undef TYPE_LIST_4
#undef TYPE_LIST_5
#undef TYPE_LIST_6

#undef TYPE_PARAM_LIST_0
#undef TYPE_PARAM_LIST_1
#undef TYPE_PARAM_LIST_2
#undef TYPE_PARAM_LIST_3
#undef TYPE_PARAM_LIST_4
#undef TYPE_PARAM_LIST_5
#undef TYPE_PARAM_LIST_6

#undef PARAM_LIST_0
#undef PARAM_LIST_1
#undef PARAM_LIST_2
#undef PARAM_LIST_3
#undef PARAM_LIST_4
#undef PARAM_LIST_5
#undef PARAM_LIST_6
} // namespace

#endif // WAN_DELEGATE_H
