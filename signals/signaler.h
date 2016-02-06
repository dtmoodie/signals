#pragma once
#include "Defs.h"
#include "signal_manager.h"
#include <boost/preprocessor.hpp>

#define SIG_DEF_1(name, ARG1) \
inline void sig_##name(ARG1 arg1) \
{ \
    static auto registerer = register_receiver(this, #name, Loki::TypeInfo(typeid(void(ARG1)))); \
    emit(#name, arg1); \
}

#define SIG_DEF_2(name, ARG1, ARG2) \
inline void sig_##name(ARG1 arg1, ARG2 arg2) \
{ \
    static auto registerer = register_receiver(this, #name, Loki::TypeInfo(typeid(void(ARG1,ARG2)))); \
    emit(#name, arg1, arg2); \
}

#define SIG_DEF_3(name, ARG1, ARG2, ARG3) \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3) \
{ \
    static auto registerer = register_receiver(this, #name, Loki::TypeInfo(typeid(void(ARG1,ARG2,ARG3)))); \
    emit(#name, arg1, arg2, arg3); \
}

#define SIG_DEF_4(name, ARG1, ARG2, ARG3, ARG4) \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4) \
{ \
    static auto registerer = register_receiver(this, #name, Loki::TypeInfo(typeid(void(ARG1,ARG2,ARG3,ARG4)))); \
    emit(#name, arg1, arg2, arg3, arg4); \
}

#define SIG_DEF_5(name, ARG1, ARG2, ARG3, ARG4, ARG5) \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5) \
{ \
    static auto registerer = register_receiver(this, #name, Loki::TypeInfo(typeid(void(ARG1,ARG2,ARG3,ARG4,ARG5)))); \
    emit(#name, arg1, arg2, arg3, arg4, arg5); \
}

#define SIG_DEF_6(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6) \
{ \
    static auto registerer = register_receiver(this, #name, Loki::TypeInfo(typeid(void(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6)))); \
    emit(#name, arg1, arg2, arg3, arg4, arg5, arg6); \
}

#define SIG_DEF_7(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7) \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6, ARG7 arg7) \
{ \
    static auto registerer = register_receiver(this, #name, Loki::TypeInfo(typeid(void(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7)))); \
    emit(#name, arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
}

#define SIG_DEF_8(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8) \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6, ARG7 arg7, ARG8 arg8) \
{ \
    static auto registerer = register_receiver(this, #name, Loki::TypeInfo(typeid(void(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8))));\
    emit(#name, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
}

#define SIG_DEF_9(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9) \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6, ARG7 arg7, ARG8 arg8, ARG9 arg9) \
{ \
    static auto registerer = register_receiver(this, #name, Loki::TypeInfo(typeid(void(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9)))); \
    emit(#name, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); \
}

#define SIG_DEF_10(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10) \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6, ARG7 arg7, ARG8 arg8, ARG9 arg9, ARG10 arg10)\
{ \
    static auto registerer = register_receiver(this, #name, Loki::TypeInfo(typeid(void(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10)))); \
    emit(#name, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10); \
}

// name / signature
#ifdef _MSC_VER
#define SIG_DEF(name, ...) BOOST_PP_CAT( BOOST_PP_OVERLOAD(SIG_DEF_, __VA_ARGS__ )(name, __VA_ARGS__), BOOST_PP_EMPTY() )
#else
#define SIG_DEF(name, ...) BOOST_PP_CAT( BOOST_PP_OVERLOAD(SIG_DEF_, __VA_ARGS__ )(name, __VA_ARGS__), BOOST_PP_EMPTY() )
#endif
namespace Signals
{
    class SIGNAL_EXPORTS signaler
    {
    protected:
        template<class... T> void emit(const std::string& signal_name, T&... args)
        {
        }
    public:

    };
    
}
