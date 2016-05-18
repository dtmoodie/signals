
#pragma once
#include "Defs.h"
#include "placeholder.h"
#include "signal_manager.h"
#include <boost/preprocessor.hpp>
#include <boost/log/trivial.hpp>
#define COMBINE1(X,Y) X##Y  // helper macro
#define COMBINE(X,Y) COMBINE1(X,Y)




#define SIGNAL_1(name, N) \
Signals::typed_signal_base<void(void)>* COMBINE(_sig_##name##_,N) = nullptr; \
inline void sig_##name()\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(COMBINE(_sig_##name##_,N) == nullptr)\
	{ \
		COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(void)>(#name); \
	} \
	(*COMBINE(_sig_##name##_,N))(); \
} \
template<typename DUMMY> struct signal_registerer<N, DUMMY> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(void))), _sig_manager); \
		_sig_manager->get_signal<void()>(#name, obj); \
		signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager); \
	} \
    static void RegisterStatic(Signals::signal_registry* registry) \
    { \
        registry->add_signal(Loki::TypeInfo(typeid(THIS_CLASS)),#name, Loki::TypeInfo(typeid(void(void)))); \
        signal_registerer<N-1, DUMMY>::RegisterStatic(registry); \
    } \
};

#define SIGNAL_2(name, ARG1, N) \
Signals::typed_signal_base<void(ARG1)>* COMBINE(_sig_##name##_,N) = nullptr; \
inline void sig_##name(ARG1 arg1)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(COMBINE(_sig_##name##_,N) == nullptr)\
	{ \
		COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1)>(#name, this); \
	} \
	(*COMBINE(_sig_##name##_,N))(arg1); \
} \
template<typename DUMMY> struct signal_registerer<N, DUMMY> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1))), _sig_manager); \
		_sig_manager->get_signal<void(ARG1)>(#name, obj); \
        signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager); \
	} \
    static void RegisterStatic(Signals::signal_registry* registry) \
    { \
        registry->add_signal(Loki::TypeInfo(typeid(THIS_CLASS)),#name, Loki::TypeInfo(typeid(void(ARG1)))); \
        signal_registerer<N-1, DUMMY>::RegisterStatic(registry); \
    } \
};

#define SIGNAL_3(name, ARG1, ARG2,  N) \
Signals::typed_signal_base<void(ARG1, ARG2)>* COMBINE(_sig_##name##_,N) = nullptr; \
inline void sig_##name(ARG1 arg1, ARG2 arg2)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(COMBINE(_sig_##name##_,N) == nullptr)\
	{ \
        COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2)>(#name, this); \
	} \
	(*COMBINE(_sig_##name##_,N))(arg1, arg2); \
} \
template<typename DUMMY> struct signal_registerer<N, DUMMY> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2))), _sig_manager); \
        obj->COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2)>(#name, obj); \
        signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager); \
	} \
    static void RegisterStatic(Signals::signal_registry* registry) \
    { \
        registry->add_signal(Loki::TypeInfo(typeid(THIS_CLASS)),#name, Loki::TypeInfo(typeid(void(ARG1, ARG2)))); \
        signal_registerer<N-1, DUMMY>::RegisterStatic(registry); \
    } \
};

#define SIGNAL_4(name, ARG1, ARG2, ARG3, N) \
Signals::typed_signal_base<void(ARG1, ARG2, ARG3)>* COMBINE(_sig_##name##_,N) = nullptr; \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(COMBINE(_sig_##name##_,N) == nullptr)\
	{ \
        COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3)>(#name, this); \
	} \
	(*COMBINE(_sig_##name##_,N))(arg1, arg2, arg3); \
} \
template<typename DUMMY> struct signal_registerer<N, DUMMY> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3))), _sig_manager); \
        obj->COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3)>(#name, obj); \
        signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager); \
	} \
    static void RegisterStatic(Signals::signal_registry* registry) \
    { \
        registry->add_signal(Loki::TypeInfo(typeid(THIS_CLASS)), #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3)))); \
        signal_registerer<N-1, DUMMY>::RegisterStatic(registry); \
    } \
};

#define SIGNAL_5(name, ARG1, ARG2, ARG3, ARG4, N) \
Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4)>* COMBINE(_sig_##name##_,N) = nullptr; \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(COMBINE(_sig_##name##_,N) == nullptr)\
	{ \
        COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4)>(#name, this); \
	} \
	(*COMBINE(_sig_##name##_,N))(arg1, arg2, arg3, arg4); \
} \
template<typename DUMMY> struct signal_registerer<N, DUMMY> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4))), _sig_manager); \
        obj->COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4)>(#name, obj); \
        signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager); \
	} \
    static void RegisterStatic(Signals::signal_registry* registry) \
    { \
        registry->add_signal(Loki::TypeInfo(typeid(THIS_CLASS)), #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4)))); \
        signal_registerer<N-1, DUMMY>::RegisterStatic(registry); \
    } \
};

#define SIGNAL_6(name, ARG1, ARG2, ARG3, ARG4, ARG5, N) \
Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4, ARG5)>* COMBINE(_sig_##name##_,N) = nullptr; \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(COMBINE(_sig_##name##_,N) == nullptr)\
	{ \
		COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5)>(#name, this); \
	} \
	(*COMBINE(_sig_##name##_,N))(arg1, arg2, arg3, arg4, arg5); \
} \
template<typename DUMMY> struct signal_registerer<N, DUMMY> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5))), _sig_manager); \
        obj->COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5)>(#name, obj); \
        signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager); \
	} \
    static void RegisterStatic(Signals::signal_registry* registry) \
    { \
        registry->add_signal(Loki::TypeInfo(typeid(THIS_CLASS)), #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5)))); \
        signal_registerer<N-1, DUMMY>::RegisterStatic(registry); \
    } \
};

#define SIGNAL_7(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, N) \
Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)>* COMBINE(_sig_##name##_,N) = nullptr; \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(COMBINE(_sig_##name##_,N) == nullptr)\
	{ \
        COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)>(#name, this); \
	} \
	(*COMBINE(_sig_##name##_,N))(arg1, arg2, arg3, arg4, arg5, arg6); \
} \
template<typename DUMMY> struct signal_registerer<N, DUMMY> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6))), _sig_manager); \
        obj->COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)>(#name, obj); \
        signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager); \
	} \
    static void RegisterStatic(Signals::signal_registry* registry) \
    { \
        registry->add_signal(Loki::TypeInfo(typeid(THIS_CLASS)), #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)))); \
        signal_registerer<N-1, DUMMY>::RegisterStatic(registry); \
    } \
};

#define SIGNAL_8(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, N) \
Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)>* COMBINE(_sig_##name##_,N) = nullptr; \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6, ARG7 arg7)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(COMBINE(_sig_##name##_,N) == nullptr)\
	{ \
        COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)>(#name, this); \
	} \
	(*COMBINE(_sig_##name##_,N))(arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
} \
template<typename DUMMY> struct signal_registerer<N, DUMMY> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7))), _sig_manager); \
        obj->COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)>(#name, obj); \
        signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager); \
	} \
    static void RegisterStatic(Signals::signal_registry* registry) \
    { \
        registry->add_signal(Loki::TypeInfo(typeid(THIS_CLASS)), #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)))); \
        signal_registerer<N-1, DUMMY>::RegisterStatic(registry); \
    } \
};

#define SIGNAL_9(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, N) \
Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8)>* COMBINE(_sig_##name##_,N) = nullptr; \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6, ARG7 arg7, ARG8 arg8)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(COMBINE(_sig_##name##_,N) == nullptr)\
	{ \
        COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8)>(#name, this); \
	} \
	(*COMBINE(_sig_##name##_,N))(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
} \
template<typename DUMMY> struct signal_registerer<N, DUMMY> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8))), _sig_manager); \
        obj->COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8)>(#name, obj); \
        signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager); \
	} \
    static void RegisterStatic(Signals::signal_registry* registry) \
    { \
        registry->add_signal(Loki::TypeInfo(typeid(THIS_CLASS)), #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8)))); \
        signal_registerer<N-1, DUMMY>::RegisterStatic(registry); \
    } \
};

#define SIGNAL_10(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, N) \
Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)>* COMBINE(_sig_##name##_,N) = nullptr; \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6, ARG7 arg7, ARG8 arg8, ARG9& arg9)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(COMBINE(_sig_##name##_,N) == nullptr)\
	{ \
        COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)>(#name, this); \
	} \
	(*COMBINE(_sig_##name##_,N))(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); \
} \
template<typename DUMMY> struct signal_registerer<N, DUMMY> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9))), _sig_manager); \
        obj->COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)>(#name, obj); \
        signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager); \
	} \
    static void RegisterStatic(Signals::signal_registry* registry) \
    { \
        registry->add_signal(Loki::TypeInfo(typeid(THIS_CLASS)),#name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)))); \
        signal_registerer<N-1, DUMMY>::RegisterStatic(registry); \
    } \
  };

// -------------------------------------------------------------------------------------------
#define SLOT__(NAME, N, RETURN, ...)\
    virtual RETURN NAME##(__VA_ARGS__); \
    template<typename DUMMY> struct slot_registerer<N, DUMMY> \
    { \
        static void RegisterStatic(Signals::signal_registry* registry) \
        { \
            registry->add_slot(Loki::TypeInfo(typeid(THIS_CLASS)),#NAME, Loki::TypeInfo(typeid(RETURN(__VA_ARGS__)))); \
            slot_registerer<N-1, DUMMY>::RegisterStatic(registry); \
        } \
    }; \
    bool connect_(std::string name, Signals::signal_base* signal, Signals::_counter_<N> dummy) \
    {   \
        if(name == #NAME) \
        { \
            auto typed_signal = dynamic_cast<Signals::typed_signal_base<RETURN(__VA_ARGS__)>*>(signal); \
            if(typed_signal) \
            { \
                _connections[signal] = typed_signal->connect(my_bind((RETURN(THIS_CLASS::*)(__VA_ARGS__))&THIS_CLASS::NAME, this, make_int_sequence<BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)>{} )); \
                return true; \
            } \
        } \
        return connect_(name, signal, Signals::_counter_<N-1>()); \
    } \
    bool connect_(std::string name, manager* manager, Signals::_counter_<N> dummy) \
    { \
        if(name == #NAME)  \
        { \
            auto sig = manager->get_signal<RETURN(__VA_ARGS__)>(#NAME, this, get_description()); \
            _connections[sig] = sig->connect(my_bind((RETURN(THIS_CLASS::*)(__VA_ARGS__))&THIS_CLASS::NAME, this, make_int_sequence<BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)>{})); \
            return true; \
        } \
        return false; \
    }

#define SLOT_1(NAME, N, RETURN) \
    virtual RETURN NAME##(); \
    template<typename DUMMY> struct slot_registerer<N, DUMMY> \
    { \
        static void RegisterStatic(Signals::signal_registry* registry) \
        { \
            registry->add_slot(Loki::TypeInfo(typeid(THIS_CLASS)),#NAME, Loki::TypeInfo(typeid(RETURN()))); \
            slot_registerer<N-1, DUMMY>::RegisterStatic(registry); \
        } \
    }; \
    bool connect_(std::string name, Signals::signal_base* signal, Signals::_counter_<N> dummy) \
    {   \
        if(name == #NAME) \
        { \
            auto typed_signal = dynamic_cast<Signals::typed_signal_base<RETURN()>*>(signal); \
            if(typed_signal) \
            { \
                _connections[signal] = typed_signal->connect(std::bind((RETURN(THIS_CLASS::*)())&THIS_CLASS::NAME, this)); \
                return true; \
            } \
        } \
        return connect_(name, signal, Signals::_counter_<N-1>()); \
    } \
    bool connect_(std::string name, manager* manager, Signals::_counter_<N> dummy) \
    { \
        if(name == #NAME)  \
        { \
            auto sig = manager->get_signal<RETURN()>(#NAME, this, get_description()); \
            _connections[sig] = sig->connect(std::bind((RETURN(THIS_CLASS::*)())&THIS_CLASS::NAME, this)); \
            return true; \
        } \
        return false; \
    }
#define SLOT_2(NAME, N, RETURN, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_3(NAME, N, RETURN, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_4(NAME, N, RETURN, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_5(NAME, N, RETURN, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_6(NAME, N, RETURN, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_7(NAME, N, RETURN, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_8(NAME, N, RETURN, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_9(NAME, N, RETURN, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_10(NAME, N, RETURN, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_11(NAME, N, RETURN, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_12(NAME, N, RETURN, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_13(NAME, N, RETURN, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)

// -------------------------------------------------------------------------------------------
#define SIGNALS_BEGIN_1(CLASS_NAME, N_) \
typedef CLASS_NAME THIS_CLASS;      \
template<int N, typename DUMMY> struct signal_registerer \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
    { \
        signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager); \
    } \
    static void RegisterStatic(Signals::signal_registry* registry) \
    { \
        signal_registerer<N-1, DUMMY>::RegisterStatic(registry); \
    } \
}; \
template<int N, typename DUMMY> struct slot_registerer \
{ \
    static void RegisterStatic(Signals::signal_registry* registry) \
    { \
        slot_registerer<N-1, DUMMY>::RegisterStatic(registry); \
    } \
}; \
template<typename DUMMY> struct signal_registerer<N_, DUMMY> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) {} \
    static void RegisterStatic(Signals::signal_registry* registry){} \
}; \
template<typename DUMMY> struct slot_registerer<N_, DUMMY> \
{ \
    static void RegisterStatic(Signals::signal_registry* registry){} \
}; \
template<int N> bool connect_(std::string name, Signals::signal_base* signal, Signals::_counter_<N> dummy){ return connect_(name, signal, Signals::_counter_<N-1>()); } \
template<int N> bool connect_(std::string name, Signals::signal_manager* manager, Signals::_counter_<N> dummy){ return connect_(name, manager, Signals::_counter_<N-1>()); } \
template<int N> bool register_slot_to_manager(Signals::signal_manager* manager, Signals::_counter_<N> dummy){ return register_slot_to_manager(manager, Signals::_counter_<N-1>()); } \
bool connect_(std::string name, Signals::signal_base* signal, Signals::_counter_<N_> dummy){ return false; } \
bool connect_(std::string name, Signals::signal_manager* manager, Signals::_counter_<N_> dummy){ return false; } \
bool register_slot_to_manager(Signals::signal_manager* manager, Signals::_counter_<N_> dummy){ return false; }

#define SIGNALS_BEGIN_2(CLASS_NAME, PARENT, N_) \
typedef CLASS_NAME THIS_CLASS; \
typedef PARENT PARENT_CLASS;  \
template<int N, typename DUMMY> struct signal_registerer \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
    { \
        signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager); \
    } \
    static void RegisterStatic(Signals::signal_registry* registry) \
    { \
        signal_registerer<N-1, DUMMY>::RegisterStatic(registry); \
    } \
}; \
template<int N, typename DUMMY> struct slot_registerer \
{ \
    static void RegisterStatic(Signals::signal_registry* registry) \
    { \
        slot_registerer<N-1, DUMMY>::RegisterStatic(registry); \
    } \
}; \
template<typename DUMMY> struct signal_registerer<N_, DUMMY> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) {} \
    static void RegisterStatic(Signals::signal_registry* registry){} \
}; \
template<typename DUMMY> struct slot_registerer<N_, DUMMY> \
{ \
    static void RegisterStatic(Signals::signal_registry* registry){} \
}; \
template<int N> bool connect_(std::string name, Signals::signal_base* signal, Signals::_counter_<N> dummy){ return connect_(name, signal, Signals::_counter_<N-1>()); } \
template<int N> bool connect_(std::string name, Signals::signal_manager* manager, Signals::_counter_<N> dummy){ return connect_(name, manager, Signals::_counter_<N-1>()); } \
template<int N> bool register_slot_to_manager(Signals::signal_manager* manager, Signals::_counter_<N> dummy){ return register_slot_to_manager(manager, Signals::_counter_<N-1>()); } \
bool connect_(std::string name, Signals::signal_base* signal, Signals::_counter_<N_> dummy){ return false; } \
bool connect_(std::string name, Signals::signal_manager* manager, Signals::_counter_<N_> dummy){ return false; } \
bool register_slot_to_manager(Signals::signal_manager* manager, Signals::_counter_<N_> dummy){ return false; }


#ifdef _MSC_VER
#define SIGNALS_BEGIN(...) BOOST_PP_CAT(BOOST_PP_OVERLOAD(SIGNALS_BEGIN_, __VA_ARGS__)(__VA_ARGS__, __COUNTER__), BOOST_PP_EMPTY())
#else

#endif

template<class T>
struct Void {
  typedef void type;
};

template<class T, class U = void>
struct has_parent {
    enum { value = 0 };
};

template<class T>
struct has_parent<T, typename Void<typename T::PARENT_CLASS>::type > {
    enum { value = 1 };
};


#define SIGNALS_END_(N) \
template<typename T> void call_parent(Signals::signal_manager* manager, typename std::enable_if<has_parent<T>::value, void>::type* = nullptr) \
{ \
    T::PARENT_CLASS::setup_signals(manager); \
} \
template<typename T> void call_parent(Signals::signal_manager* manager, typename std::enable_if<!has_parent<T>::value, void>::type* = nullptr){ } \
virtual void setup_signals(Signals::signal_manager* manager) \
{ \
    call_parent<THIS_CLASS>(manager, nullptr); \
    _sig_manager = manager; \
    /*signal_registerer<N, int>::Register(this, _sig_manager);*/ \
    connect_signals(manager); \
} \
struct static_registration \
{  \
    static_registration() \
    { \
        register_<THIS_CLASS>(); \
    } \
    template<typename T> void register_(typename std::enable_if<has_parent<T>::value, void>::type* = nullptr) \
    { \
        /*T::PARENT_CLASS::signal_registerer<N-1, int>::RegisterStatic(Signals::signal_registry::instance());*/ \
        /*T::PARENT_CLASS::slot_registerer<N-1, int>::RegisterStatic(Signals::signal_registry::instance()); */\
        signal_registerer<N-1, int>::RegisterStatic(Signals::signal_registry::instance()); \
        slot_registerer<N-1, int>::RegisterStatic(Signals::signal_registry::instance());\
    } \
    template<typename T> void register_(typename std::enable_if<!has_parent<T>::value, void>::type* = nullptr)\
    { \
        signal_registerer<N-1, int>::RegisterStatic(Signals::signal_registry::instance()); \
        slot_registerer<N-1, int>::RegisterStatic(Signals::signal_registry::instance());\
    } \
}; \
void connect_signal(std::string name, Signals::signal_base* signal) \
{  \
    connect_(name, signal, Signals::_counter_<N-1>()); \
} \
void connect_signals(manager* manager) \
{ \
    register_slot_to_manager(manager, Signals::_counter_<N-1>()); \
}


#define SIGNALS_END SIGNALS_END_(__COUNTER__)

// -------------------------------------------------------------------------------------------
#define SIGNAL_IMPL(CLASS_NAME) static CLASS_NAME::static_registration g_##CLASS_NAME##_registration_instance;


// -------------------------------------------------------------------------------------------
// This macro signifies that this slot should automatically be connected to the appropriate signal from the signal_manager that is passed into setup_signals
#define REGISTER_SLOT_(NAME, N) \
bool register_slot_to_manager(Signals::signal_manager* manager, Signals::_counter_<N> dummy) \
{ \
    connect_(#NAME, manager, Signals::_counter_<N-1>()); \
    return register_slot_to_manager(manager, Signals::_counter_<N-1>()); \
}

#define REGISTER_SLOT(NAME) REGISTER_SLOT_(NAME, __COUNTER__)

#ifdef _MSC_VER
#define SLOT_DEF(NAME, ...) BOOST_PP_CAT( BOOST_PP_OVERLOAD(SLOT_, __VA_ARGS__)(NAME, __COUNTER__, __VA_ARGS__), BOOST_PP_EMPTY())
#else
#define SLOT_DEF(NAME, ...) BOOST_PP_OVERLOAD(SLOT_, __VA_ARGS__))(NAME, __COUNTER__, __VA_ARGS__)
#endif

#ifdef _MSC_VER
#define SLOT_OVERLOAD(NAME, ...) BOOST_PP_CAT(BOOST_PP_OVERLOAD(SLOT_OVERLOAD_, __VA_ARGS__)(NAME, __COUNTER__, __VA_ARGS__), BOOST_PP_EMPTY())
#else

#endif


#define AUTO_SLOT(NAME, ...) SLOT_DEF(NAME, __VA_ARGS__); REGISTER_SLOT(NAME)

#ifdef _MSC_VER
#define SIG_SEND(...) BOOST_PP_CAT( BOOST_PP_OVERLOAD(SIGNAL_, __VA_ARGS__ )(__VA_ARGS__, __COUNTER__), BOOST_PP_EMPTY() )
#else
#define SIG_SEND(...) BOOST_PP_OVERLOAD(SIGNAL_, __VA_ARGS__ )(__VA_ARGS__, __COUNTER__)
#endif

namespace Signals
{
    template<int N> class _counter_{};
    class SIGNAL_EXPORTS signaler
    {
	public:
		typedef signal_manager manager;
		signaler();
		~signaler();
		virtual void setup_signals(manager* manager);
		virtual std::string get_description()
		{
			return "Base signaler implementation";
		}
        // Disconnect all slots associated with this signal
        virtual void disconnect(Signals::signal_base* signal);
        // Disconnects all slots associated with signals of this name
        virtual void disconnect(std::string name);
        // Disconnects all slots associated with signals of this name with given signature
        template<typename Sig> void disconnect(std::string name)
        {
        
        }
    protected:
		manager* _sig_manager;
        std::map<Signals::signal_base*, std::shared_ptr<connection>> _connections;
    public:

    };    
}
