
#pragma once
#include "Defs.h"
#include "logging.hpp"
#include "placeholder.h"
#include "signal_manager.h"
#include <boost/preprocessor.hpp>

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
	template<class C> static int Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(void))), _sig_manager); \
		obj->COMBINE(_sig_##name##_, N) = _sig_manager->get_signal<void()>(#name, obj); \
		return signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager) + 1; \
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
	template<class C> static int Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1))), _sig_manager); \
		obj->COMBINE(_sig_##name##_, N) = _sig_manager->get_signal<void(ARG1)>(#name, obj); \
        return signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager) + 1; \
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
	template<class C> static int Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2))), _sig_manager); \
        obj->COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2)>(#name, obj); \
        return signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager) + 1; \
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
	template<class C> static int Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3))), _sig_manager); \
        obj->COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3)>(#name, obj); \
        return signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager) + 1; \
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
	template<class C> static int Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4))), _sig_manager); \
        obj->COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4)>(#name, obj); \
        return signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager) + 1; \
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
	template<class C> static int Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5))), _sig_manager); \
        obj->COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5)>(#name, obj); \
        return signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager) + 1; \
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
	template<class C> static int Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6))), _sig_manager); \
        obj->COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)>(#name, obj); \
        return signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager) + 1; \
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
	template<class C> static int Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7))), _sig_manager); \
        obj->COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)>(#name, obj); \
        return signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager) + 1; \
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
	template<class C> static int Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8))), _sig_manager); \
        obj->COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8)>(#name, obj); \
        return signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager) + 1; \
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
	template<class C> static int Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9))), _sig_manager); \
        obj->COMBINE(_sig_##name##_,N) = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)>(#name, obj); \
        return signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager) + 1; \
	} \
    static void RegisterStatic(Signals::signal_registry* registry) \
    { \
        registry->add_signal(Loki::TypeInfo(typeid(THIS_CLASS)),#name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)))); \
        signal_registerer<N-1, DUMMY>::RegisterStatic(registry); \
    } \
  };

// -------------------------------------------------------------------------------------------
#define SLOT__(NAME, N, RETURN, ...)\
    virtual RETURN NAME(__VA_ARGS__); \
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
				LOG(trace) << "[" #NAME " - " << typed_signal->get_signal_type().name() << "]"; \
                _connections[signal] = typed_signal->connect(my_bind((RETURN(THIS_CLASS::*)(__VA_ARGS__))&THIS_CLASS::NAME, this, make_int_sequence<BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)>{} )); \
                return true; \
            } \
        } \
        return connect_(name, signal, Signals::_counter_<N-1>()); \
    } \
	int connect_(std::string name, manager* manager_, Signals::_counter_<N> dummy) \
	{ \
		if(name == #NAME) \
		{ \
			auto sig = manager_->get_signal<RETURN(__VA_ARGS__)>(#NAME, this, get_description()); \
			LOG(trace) << "Connecting slot with name: \"" #NAME "\" and signature <" << typeid(RETURN(__VA_ARGS__)).name() << "> to manager"; \
			_connections[sig] = sig->connect(my_bind((RETURN(THIS_CLASS::*)(__VA_ARGS__))&THIS_CLASS::NAME, this, make_int_sequence<BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)>{} )); \
			return connect_(name, manager_, Signals::_counter_<N-1>()) + 1; \
		} \
		return connect_(name, manager_, Signals::_counter_<N-1>()); \
	} \
	int disconnect_by_name(std::string name, manager* manager_, Signals::_counter_<N> dummy) \
	{ \
		if(name == #NAME) \
		{ \
			auto sig = manager_->get_signal_optional<RETURN(__VA_ARGS__)>(#NAME, this, get_description()); \
			if(sig) LOG(trace) << "[" #NAME " - " << sig->get_signal_type().name() << "]"; \
			return disconnect_by_name(name, manager_, Signals::_counter_<N-1>()) + disconnect_from_signal(sig) ? 1 : 0; \
		} \
		return disconnect_by_name(name, manager_, Signals::_counter_<N-1>()); \
	} \
	int disconnect_(manager* manager_, Signals::_counter_<N> dummy) \
	{ \
		auto sig = manager_->get_signal_optional<RETURN(__VA_ARGS__)>(#NAME, this, get_description()); \
		if(sig) LOG(trace) << "[" #NAME " - " << sig->get_signal_type().name() << "]"; \
		return disconnect_(manager_, Signals::_counter_<N-1>()) + (disconnect_from_signal(sig) ? 1 : 0); \
	} \

#define SLOT_1(RETURN, N, NAME) \
    virtual RETURN NAME(); \
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
				LOG(trace) << "[" #NAME  " - " << typed_signal->get_signal_type().name() << "]"; \
                _connections[signal] = typed_signal->connect(std::bind((RETURN(THIS_CLASS::*)())&THIS_CLASS::NAME, this)); \
                return true; \
            } \
        } \
        return connect_(name, signal, Signals::_counter_<N-1>()); \
    } \
    int connect_(std::string name, manager* manager_, Signals::_counter_<N> dummy) \
    { \
        if(name == #NAME)  \
        { \
            auto sig = manager_->get_signal<RETURN()>(#NAME, this, get_description()); \
			LOG(trace) << "Connecting slot with name: \"" #NAME "\" and signature <" << typeid(RETURN(void)).name() << "> to manager"; \
            _connections[sig] = sig->connect(std::bind((RETURN(THIS_CLASS::*)())&THIS_CLASS::NAME, this)); \
			return connect_(name, manager_, Signals::_counter_<N-1>()) + 1; \
        } \
        return connect_(name, manager_, Signals::_counter_<N-1>()); \
    } \
	int disconnect_by_name(std::string name, manager* manager_, Signals::_counter_<N> dummy) \
	{ \
		if(name == #NAME) \
		{ \
			auto sig = manager_->get_signal_optional<RETURN()>(#NAME, this, get_description()); \
			if(sig) LOG(trace) << "[" #NAME  " - " << sig->get_signal_type().name() << "]"; \
			return disconnect_by_name(name, manager_, Signals::_counter_<N-1>()) + disconnect_from_signal(sig) ? 1 : 0; \
		} \
		return disconnect_by_name(name, manager_, Signals::_counter_<N-1>()); \
	} \
	int disconnect_(manager* manager_, Signals::_counter_<N> dummy) \
	{ \
		auto sig = manager_->get_signal_optional<RETURN()>(#NAME); \
		if(sig) LOG(trace) << "[" #NAME  " - " << sig->get_signal_type().name() << "]"; \
		return disconnect_(manager_, Signals::_counter_<N-1>()) + (disconnect_from_signal(sig) ? 1 : 0); \
	} \

#define SLOT_2(RETURN, N, NAME, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_3(RETURN, N, NAME, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_4(RETURN, N, NAME, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_5(RETURN, N, NAME, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_6(RETURN, N, NAME, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_7(RETURN, N, NAME, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_8(RETURN, N, NAME, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_9(RETURN, N, NAME, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_10(RETURN, N, NAME, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_11(RETURN, N, NAME, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_12(RETURN, N, NAME, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)
#define SLOT_13(RETURN, N, NAME, ...) SLOT__(NAME, N, RETURN, __VA_ARGS__)

// -------------------------------------------------------------------------------------------
#ifdef _DEBUG
//#define LOG_RECURSION(N) BOOST_LOG_TRIVIAL(trace) << "[" << __FUNCSIG__ << "] " << N
#else
#define LOG_RECURSION(N) 
#endif
#define LOG_RECURSION(N) 

#define SIGNALS_BEGIN_1(CLASS_NAME, N_) \
typedef CLASS_NAME THIS_CLASS;      \
template<int N, typename DUMMY> struct signal_registerer \
{ \
	template<class C> static int Register(C* obj, manager* _sig_manager) \
    { \
        return signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager); \
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
	template<class C> static int Register(C* obj, manager* _sig_manager) {return 0;} \
    static void RegisterStatic(Signals::signal_registry* registry){} \
}; \
template<typename DUMMY> struct slot_registerer<N_, DUMMY> \
{ \
    static void RegisterStatic(Signals::signal_registry* registry){} \
}; \
template<int N> bool connect_(std::string name, Signals::signal_base* signal, Signals::_counter_<N> dummy) \
{ \
	LOG_RECURSION(N); \
	return connect_(name, signal, Signals::_counter_<N-1>()); \
} \
template<int N> int connect_(std::string name, Signals::signal_manager* manager, Signals::_counter_<N> dummy) \
{ \
	LOG_RECURSION(N); \
	return connect_(name, manager, Signals::_counter_<N-1>()); \
} \
template<int N> int connect_(Signals::signal_manager* manager, Signals::_counter_<N> dummy) \
{ \
	LOG_RECURSION(N); \
	return connect_(manager, Signals::_counter_<N-1>()); \
} \
template<int N> int disconnect_(Signals::signal_manager* manager_, Signals::_counter_<N> dummy) \
{ \
	LOG_RECURSION(N); \
	return disconnect_(manager_, Signals::_counter_<N-1>()); \
} \
template<int N> int disconnect_by_name(std::string name, Signals::signal_manager* manager_, Signals::_counter_<N> dummy) \
{ \
	LOG_RECURSION(N); \
	return disconnect_by_name(name, manager_, Signals::_counter_<N - 1>()); \
} \
bool connect_(std::string name, Signals::signal_base* signal, Signals::_counter_<N_> dummy) \
{ \
	LOG_RECURSION(N_); \
	return false; \
} \
int connect_(std::string name, Signals::signal_manager* manager, Signals::_counter_<N_> dummy) \
{ \
	LOG_RECURSION(N_); \
	return 0; \
} \
int connect_(Signals::signal_manager* manager, Signals::_counter_<N_> dummy) \
{ \
	LOG_RECURSION(N_); \
	return 0; \
} \
int disconnect_(Signals::signal_manager* manager_, Signals::_counter_<N_> dummy)\
{ \
	LOG_RECURSION(N_); \
	return 0; \
} \
int disconnect_by_name(std::string name, Signals::signal_manager* manager_, Signals::_counter_<N_> dummy) \
{ \
	LOG_RECURSION(N_); \
	return 0; \
} \


#define SIGNALS_BEGIN_2(CLASS_NAME, PARENT, N_) \
typedef CLASS_NAME THIS_CLASS; \
typedef PARENT PARENT_CLASS;  \
template<int N, typename DUMMY> struct signal_registerer \
{ \
	template<class C> static int Register(C* obj, manager* _sig_manager) \
    { \
        return signal_registerer<N-1, DUMMY>::Register(obj, _sig_manager); \
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
	template<class C> static int Register(C* obj, manager* _sig_manager) {return 0;} \
    static void RegisterStatic(Signals::signal_registry* registry){} \
}; \
template<typename DUMMY> struct slot_registerer<N_, DUMMY> \
{ \
    static void RegisterStatic(Signals::signal_registry* registry){} \
}; \
template<int N> bool connect_(std::string name, Signals::signal_base* signal, Signals::_counter_<N> dummy) \
{  \
	LOG_RECURSION(N); \
	return connect_(name, signal, Signals::_counter_<N-1>()); \
} \
template<int N> int connect_(std::string name, Signals::signal_manager* manager, Signals::_counter_<N> dummy)\
{ \
	LOG_RECURSION(N); \
	return connect_(name, manager, Signals::_counter_<N-1>()); \
} \
template<int N> int connect_(Signals::signal_manager* manager, Signals::_counter_<N> dummy) \
{ \
	LOG_RECURSION(N); \
	return connect_(manager, Signals::_counter_<N-1>()); \
} \
template<int N> int disconnect_by_name(std::string name, Signals::signal_manager* manager_, Signals::_counter_<N> dummy) \
{ \
	LOG_RECURSION(N); \
	return disconnect_by_name(name, manager_, Signals::_counter_<N-1>()); \
} \
template<int N> int disconnect_(Signals::signal_manager* manager_, Signals::_counter_<N> dummy) \
{ \
	LOG_RECURSION(N); \
	return disconnect_(manager_, Signals::_counter_<N-1>());\
} \
bool connect_(std::string name, Signals::signal_base* signal, Signals::_counter_<N_> dummy) \
{ \
	LOG_RECURSION(N_); \
	return false; \
} \
int connect_(std::string name, Signals::signal_manager* manager, Signals::_counter_<N_> dummy) \
{ \
	LOG_RECURSION(N_); \
	return 0; \
} \
int connect_(Signals::signal_manager* manager, Signals::_counter_<N_> dummy) \
{ \
	LOG_RECURSION(N_); \
	return 0; \
} \
int disconnect_by_name(std::string name, Signals::signal_manager* manager_, Signals::_counter_<N_> dummy) \
{ \
	LOG_RECURSION(N_); \
	return 0; \
} \
int disconnect_(Signals::signal_manager* manager_, Signals::_counter_<N_> dummy) \
{  \
	LOG_RECURSION(N_); \
	return 0;  \
}

#ifdef _MSC_VER
#define SIGNALS_BEGIN(...) BOOST_PP_CAT(BOOST_PP_OVERLOAD(SIGNALS_BEGIN_, __VA_ARGS__)(__VA_ARGS__, __COUNTER__), BOOST_PP_EMPTY())
#else
#define SIGNALS_BEGIN(...) BOOST_PP_OVERLOAD(SIGNALS_BEGIN_, __VA_ARGS__)(__VA_ARGS__, __COUNTER__)
#endif
namespace Signals
{
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
}


#define SIGNALS_END_(N) \
template<typename T> int call_parent_setup(Signals::signal_manager* manager, typename std::enable_if<Signals::has_parent<T>::value, void>::type* = nullptr) \
{ \
	LOG(trace) << typeid(T::PARENT_CLASS).name(); \
    return T::PARENT_CLASS::setup_signals(manager); \
} \
template<typename T> int call_parent_setup(Signals::signal_manager* manager, typename std::enable_if<!Signals::has_parent<T>::value, void>::type* = nullptr) \
{ \
	return 0; \
} \
template<typename T> int call_parent_connect_by_name(const std::string& name, Signals::signal_manager* manager, typename std::enable_if<Signals::has_parent<T>::value, void>::type* = nullptr) \
{ \
	LOG(trace) << typeid(T::PARENT_CLASS).name(); \
    return T::PARENT_CLASS::connect_by_name(name, manager); \
} \
template<typename T> int call_parent_connect_by_name(const std::string& name, Signals::signal_manager* manager, typename std::enable_if<!Signals::has_parent<T>::value, void>::type* = nullptr) \
{ \
	return 0; \
} \
template<typename T> bool call_parent_connect(const std::string& name, Signals::signal_base* signal, typename std::enable_if<Signals::has_parent<T>::value, void>::type* = nullptr) \
{ \
	LOG(trace) << typeid(T::PARENT_CLASS).name(); \
    return T::PARENT_CLASS::connect(name, signal); \
} \
template<typename T> bool call_parent_connect(const std::string& name, Signals::signal_base* signal, typename std::enable_if<!Signals::has_parent<T>::value, void>::type* = nullptr) \
{ \
	return false; \
} \
virtual int setup_signals(Signals::signal_manager* manager) \
{ \
    int parent_signal_count = call_parent_setup<THIS_CLASS>(manager, nullptr); \
	LOG(trace) << "Initializing signal objects and setting up automatically registered slots"; \
    _sig_manager = manager; \
    parent_signal_count += signal_registerer<N, int>::Register(this, _sig_manager); \
    return connect(manager) + parent_signal_count; \
} \
struct static_registration \
{  \
    static_registration() \
    { \
        register_<THIS_CLASS>(); \
    } \
    template<typename T> void register_(typename std::enable_if<Signals::has_parent<T>::value, void>::type* = nullptr) \
    { \
        signal_registerer<N-1, int>::RegisterStatic(Signals::signal_registry::instance()); \
        slot_registerer<N-1, int>::RegisterStatic(Signals::signal_registry::instance());\
    } \
    template<typename T> void register_(typename std::enable_if<!Signals::has_parent<T>::value, void>::type* = nullptr)\
    { \
        signal_registerer<N-1, int>::RegisterStatic(Signals::signal_registry::instance()); \
        slot_registerer<N-1, int>::RegisterStatic(Signals::signal_registry::instance());\
    } \
}; \
bool connect(std::string name, Signals::signal_base* signal) \
{  \
	if(call_parent_connect<THIS_CLASS>(name, signal, nullptr)) \
		return true; \
    return connect_(name, signal, Signals::_counter_<N-1>()); \
} \
int connect_by_name(const std::string& name, manager* manager) \
{ \
	int parent_count = call_parent_connect_by_name<THIS_CLASS>(name, manager, nullptr); \
	return connect_(name, manager, Signals::_counter_<N-1>()) + parent_count; \
} \
int connect(manager* manager) \
{ \
    return connect_(manager, Signals::_counter_<N-1>()); \
}\
int disconnect(std::string name, manager* manager_) \
{ \
	return disconnect_by_name(name, manager_, Signals::_counter_<N-1>()); \
} \
int disconnect(manager* manager_) \
{ \
	return disconnect_(manager_, Signals::_counter_<N-1>()); \
} \


#define SIGNALS_END SIGNALS_END_(__COUNTER__)

// -------------------------------------------------------------------------------------------
#define SIGNAL_IMPL(CLASS_NAME) static CLASS_NAME::static_registration g_##CLASS_NAME##_registration_instance;


// -------------------------------------------------------------------------------------------
// This macro signifies that this slot should automatically be connected to the appropriate signal from the signal_manager that is passed into setup_signals
#define REGISTER_SLOT_(NAME, N) \
int connect_(Signals::signal_manager* manager, Signals::_counter_<N> dummy) \
{ \
	LOG(trace) << "Automatically connecting slot named: \"" #NAME "\" to manager"; \
    int count = connect_(#NAME, manager, Signals::_counter_<N>()); \
    return connect_(manager, Signals::_counter_<N-1>()) + count; \
}

#define REGISTER_SLOT(NAME) REGISTER_SLOT_(NAME, __COUNTER__)

#ifdef _MSC_VER
#define SLOT_DEF(RET, ...) BOOST_PP_CAT( BOOST_PP_OVERLOAD(SLOT_, __VA_ARGS__)(RET, __COUNTER__, __VA_ARGS__), BOOST_PP_EMPTY())
#else
#define SLOT_DEF(NAME, ...) BOOST_PP_OVERLOAD(SLOT_, __VA_ARGS__)(NAME, __COUNTER__, __VA_ARGS__)
#endif

#ifdef _MSC_VER
#define SLOT_OVERLOAD(NAME, ...) BOOST_PP_CAT(BOOST_PP_OVERLOAD(SLOT_OVERLOAD_, __VA_ARGS__)(NAME, __COUNTER__, __VA_ARGS__), BOOST_PP_EMPTY())
#else
#define SLOT_OVERLOAD(NAME, ...) BOOST_PP_OVERLOAD(SLOT_OVERLOAD_, __VA_ARGS__)(NAME, __COUNTER__, __VA_ARGS__)
#endif

#define CALL_REGISTER_SLOT(name) REGISTER_SLOT(name)
#define REGISTER_SLOT_HELPER(NAME, ...) CALL_REGISTER_SLOT(NAME);

#define AUTO_SLOT(RETURN, ...) \
SLOT_DEF(RETURN, __VA_ARGS__); \
REGISTER_SLOT_HELPER(__VA_ARGS__)

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
		virtual ~signaler();
		virtual int setup_signals(manager* manager);
		virtual std::string get_description()
		{
			return "Base signaler implementation";
		}
		// This connects a signal with provided name to this object, if there are slots that can receive it
		virtual bool connect(std::string name, signal_base* signal) = 0;
		
		// This connects all slots with the given name to the provided manager
		virtual int connect_by_name(const std::string& name, signal_manager* manager) = 0;
		virtual int connect_by_name(const std::string& name); // Overload uses member signal manager
		// This disconnects all slots from the given manager
		virtual int disconnect(signal_manager* manager) = 0;
		virtual int disconnect(); // overload uses member signal manager
		// Disconnect all slots associated with this signal
		virtual bool disconnect(Signals::signal_base* signal);
		bool disconnect_from_signal(Signals::signal_base* signal);
		// Disconnects all slots associated with signals of this name
		virtual int disconnect(std::string name, signal_manager* manager) = 0;
		virtual int disconnect(std::string name); // overload uses member signal manager
		
	protected:
		manager* _sig_manager;
		std::map<Signals::signal_base*, std::shared_ptr<connection>> _connections;
	public:

	};    
}
