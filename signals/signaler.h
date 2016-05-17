
#pragma once
#include "Defs.h"
#include "placeholder.h"
#include "signal_manager.h"
#include <boost/preprocessor.hpp>
#include <boost/log/trivial.hpp>
#define COMBINE1(X,Y) X##Y  // helper macro
#define COMBINE(X,Y) COMBINE1(X,Y)


#define SIGNALS_BEGIN_(CLASS_NAME, N_) \
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
bool connect_(std::string name, Signals::signal_base* signal, Signals::_counter_<N_> dummy){ return false; }

#define SIGNALS_BEGIN(CLASS_NAME) SIGNALS_BEGIN_(CLASS_NAME, __COUNTER__)

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




#define SIGNALS_END_(N) \
virtual void setup_signals(manager* manager) { _sig_manager = manager; signal_registerer<N, int>::Register(this, _sig_manager);} \
struct static_registration{ static_registration() { signal_registerer<N-1, int>::RegisterStatic(Signals::signal_registry::instance()); slot_registerer<N-1, int>::RegisterStatic(Signals::signal_registry::instance());}}; \
    virtual void connect_signal(std::string name, Signals::signal_base* signal){ connect_(name, signal, Signals::_counter_<N-1>()); }

#define SIGNALS_END SIGNALS_END_(__COUNTER__)

#define SLOT_(NAME, N, RETURN, ...) \
    RETURN NAME##(__VA_ARGS__); \
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
                _connections.push_back(typed_signal->connect(my_bind(&THIS_CLASS::##NAME, this, make_int_sequence<BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)>{} ))); \
                return true; \
            } \
        } \
        return connect_(name, signal, Signals::_counter_<N-1>()); \
    }

#define SIGNAL_IMPL(CLASS_NAME) static CLASS_NAME::static_registration g_##CLASS_NAME##_registration_instance;


#ifdef _MSC_VER
#define SLOT_DEF(NAME, RETURN, ...) SLOT_(NAME, __COUNTER__, RETURN, __VA_ARGS__)
#else
#define SLOT(NAME, RETURN, ...) BOOST_PP_OVERLOAD(SLOT_, __VA_ARGS__ )(NAME, RETURN, __VA_ARGS__, __COUNTER__)
#endif

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
    protected:
		manager* _sig_manager;
        std::vector<std::shared_ptr<connection>> _connections;
    public:

    };
    
}
