
#pragma once
#include "Defs.h"
#include "signal_manager.h"
#include <boost/preprocessor.hpp>
#include <boost/log/trivial.hpp>

#define SIG_DEF_1(name) \
	Signals::typed_signal_base<void(void)>* _sig_##name = nullptr; \
inline void sig_##name() \
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
    if(_sig_##name == nullptr)\
											{ \
		_sig_##name = _sig_manager->get_signal<void(void)>(#name); \
		Signals::register_sender(this, #name, Loki::TypeInfo(typeid(void(void))), _sig_manager); \
											} \
    (*_sig_##name)(); \
}

#define SIG_DEF_2(name, ARG1) \
	Signals::typed_signal_base<void(ARG1)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1 arg1) \
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
    if(_sig_##name == nullptr)\
	{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1)>(#name); \
		Signals::register_sender(this, #name, Loki::TypeInfo(typeid(void(ARG1))), _sig_manager); \
	} \
    (*_sig_##name)(arg1); \
}

#define SIG_DEF_3(name, ARG1, ARG2) \
	Signals::typed_signal_base<void(ARG1, ARG2)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1 arg1, ARG2 arg2) \
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
    if(_sig_##name == nullptr)\
									{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2)>(#name); \
		Signals::register_sender(this, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2))), _sig_manager); \
									} \
    (*_sig_##name)(arg1, arg2); \
}

#define SIG_DEF_4(name, ARG1, ARG2, ARG3) \
	Signals::typed_signal_base<void(ARG1, ARG2, ARG3)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3) \
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
    if(_sig_##name == nullptr)\
								{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3)>(#name); \
		Signals::register_sender(this, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3))), _sig_manager); \
								} \
    (*_sig_##name)(arg1, arg2, arg3); \
}

#define SIG_DEF_5(name, ARG1, ARG2, ARG3, ARG4) \
	Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4) \
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
    if(_sig_##name == nullptr)\
							{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4)>(#name); \
		Signals::register_sender(this, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4))), _sig_manager); \
							} \
    (*_sig_##name)(arg1, arg2, arg3, arg4); \
}

#define SIG_DEF_6(name, ARG1, ARG2, ARG3, ARG4, ARG5) \
	Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4, ARG5)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5) \
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
    if(_sig_##name == nullptr)\
						{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5)>(#name); \
		Signals::register_sender(this, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5))), _sig_manager); \
						} \
    (*_sig_##name)(arg1, arg2, arg3, arg4, arg5); \
}

#define SIG_DEF_7(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
	Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6) \
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
    if(_sig_##name == nullptr)\
					{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)>(#name); \
		Signals::register_sender(this, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6))), _sig_manager); \
					} \
    (*_sig_##name)(arg1, arg2, arg3, arg4, arg5, arg6); \
}

#define SIG_DEF_8(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7) \
	Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6, ARG7 arg7) \
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
    if(_sig_##name == nullptr)\
				{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)>(#name); \
		Signals::register_sender(this, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7))), _sig_manager); \
				} \
    (*_sig_##name)(arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
}

#define SIG_DEF_9(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8) \
	Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6, ARG7 arg7, ARG8 arg8) \
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
    if(_sig_##name == nullptr)\
			{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8)>(#name); \
		Signals::register_sender(this, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8))), _sig_manager); \
			} \
    (*_sig_##name)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
}

#define SIG_DEF_10(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9) \
	Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6, ARG7 arg7, ARG8 arg8, ARG9 arg9) \
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
    if(_sig_##name == nullptr)\
		{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)>(#name); \
		Signals::register_sender(this, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9))), _sig_manager); \
		} \
    (*_sig_##name)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); \
}

#define SIG_DEF_11(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10) \
	Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4, ARG5 arg5, ARG6 arg6, ARG7 arg7, ARG8 arg8, ARG9 arg9, ARG10 arg10) \
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
    if(_sig_##name == nullptr)\
	{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10)>(#name); \
		Signals::register_sender(this, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10))), _sig_manager); \
	} \
    (*_sig_##name)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10); \
}
#define SIGNALS_BEGIN \
template<int N> struct signal_registerer \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) {} \
}; \
template<> struct signal_registerer<__COUNTER__> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) {} \
};

#define SIGNAL_1(name, N) Signals::typed_signal_base<void(void)>* _sig_##name = nullptr; \
inline void sig_##name()\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(_sig_##name == nullptr)\
	{ \
		_sig_##name = _sig_manager->get_signal<void(void)>(#name); \
	} \
	(*_sig_##name)(); \
} \
template<> struct signal_registerer<N> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(void))), _sig_manager); \
		_sig_manager->get_signal<void()>(#name, obj); \
		signal_registerer<N-1>::Register(obj, _sig_manager); \
	} \
};

#define SIGNAL_2(name, ARG1, N) Signals::typed_signal_base<void(ARG1)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1& arg1)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(_sig_##name == nullptr)\
	{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1)>(#name, this); \
	} \
	(*_sig_##name)(arg1); \
} \
template<> struct signal_registerer<N> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1))), _sig_manager); \
		_sig_manager->get_signal<void(ARG1)>(#name, obj); \
		signal_registerer<N-1>::Register(obj, _sig_manager); \
	} \
};

#define SIGNAL_3(name, ARG1, ARG2,  N) \
Signals::typed_signal_base<void(ARG1, ARG2)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1& arg1, ARG2& arg2)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(_sig_##name == nullptr)\
	{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2)>(#name, this); \
	} \
	(*_sig_##name)(arg1, arg2); \
} \
template<> struct signal_registerer<N> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2))), _sig_manager); \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2)>(#name, obj); \
		signal_registerer<N-1>::Register(obj, _sig_manager); \
	} \
};

#define SIGNAL_4(name, ARG1, ARG2, ARG3, N) \
Signals::typed_signal_base<void(ARG1, ARG2, ARG3)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1& arg1, ARG2& arg2, ARG3& arg3)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(_sig_##name == nullptr)\
	{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3)>(#name, this); \
	} \
	(*_sig_##name)(arg1, arg2, arg3); \
} \
template<> struct signal_registerer<N> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3))), _sig_manager); \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3)>(#name, obj); \
		signal_registerer<N-1>::Register(obj, _sig_manager); \
	} \
};

#define SIGNAL_5(name, ARG1, ARG2, ARG3, ARG4, N) \
Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1& arg1, ARG2& arg2, ARG3& arg3, ARG4& arg4)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(_sig_##name == nullptr)\
	{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4)>(#name, this); \
	} \
	(*_sig_##name)(arg1, arg2, arg3, arg4); \
} \
template<> struct signal_registerer<N> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4))), _sig_manager); \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4)>(#name, obj); \
		signal_registerer<N-1>::Register(obj, _sig_manager); \
	} \
};

#define SIGNAL_6(name, ARG1, ARG2, ARG3, ARG4, ARG5, N) \
Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4, ARG5)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1& arg1, ARG2& arg2, ARG3& arg3, ARG4& arg4, ARG5& arg5)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(_sig_##name == nullptr)\
	{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5)>(#name, this); \
	} \
	(*_sig_##name)(arg1, arg2, arg3, arg4, arg5); \
} \
template<> struct signal_registerer<N> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5))), _sig_manager); \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5)>(#name, obj); \
		signal_registerer<N-1>::Register(obj, _sig_manager); \
	} \
};

#define SIGNAL_7(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, N) \
Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1& arg1, ARG2& arg2, ARG3& arg3, ARG4& arg4, ARG5& arg5, ARG6& arg6)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(_sig_##name == nullptr)\
	{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)>(#name, this); \
	} \
	(*_sig_##name)(arg1, arg2, arg3, arg4, arg5, arg6); \
} \
template<> struct signal_registerer<N> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6))), _sig_manager); \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)>(#name, obj); \
		signal_registerer<N-1>::Register(obj, _sig_manager); \
	} \
};

#define SIGNAL_8(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, N) \
Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1& arg1, ARG2& arg2, ARG3& arg3, ARG4& arg4, ARG5& arg5, ARG6& arg6, ARG7& arg7)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(_sig_##name == nullptr)\
	{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)>(#name, this); \
	} \
	(*_sig_##name)(arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
} \
template<> struct signal_registerer<N> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7))), _sig_manager); \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)>(#name, obj); \
		signal_registerer<N-1>::Register(obj, _sig_manager); \
	} \
};

#define SIGNAL_9(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, N) \
Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1& arg1, ARG2& arg2, ARG3& arg3, ARG4& arg4, ARG5& arg5, ARG6& arg6, ARG7& arg7, ARG8& arg8)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(_sig_##name == nullptr)\
	{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8)>(#name, this); \
	} \
	(*_sig_##name)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
} \
template<> struct signal_registerer<N> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8))), _sig_manager); \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8)>(#name, obj); \
		signal_registerer<N-1>::Register(obj, _sig_manager); \
	} \
};

#define SIGNAL_10(name, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, N) \
Signals::typed_signal_base<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)>* _sig_##name = nullptr; \
inline void sig_##name(ARG1& arg1, ARG2& arg2, ARG3& arg3, ARG4& arg4, ARG5& arg5, ARG6& arg6, ARG7& arg7, ARG8& arg8, ARG9& arg9)\
{\
	if(!_sig_manager) _sig_manager = manager::get_instance(); \
	if(_sig_##name == nullptr)\
	{ \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)>(#name, this); \
	} \
	(*_sig_##name)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); \
} \
template<> struct signal_registerer<N> \
{ \
	template<class C> static void Register(C* obj, manager* _sig_manager) \
	{ \
		Signals::register_sender(obj, #name, Loki::TypeInfo(typeid(void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9))), _sig_manager); \
		_sig_##name = _sig_manager->get_signal<void(ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)>(#name, obj); \
		signal_registerer<N-1>::Register(obj, _sig_manager); \
	} \
};

#ifdef _MSC_VER
#define SIG_SEND(...) BOOST_PP_CAT( BOOST_PP_OVERLOAD(SIGNAL_, __VA_ARGS__ )(__VA_ARGS__, __COUNTER__), BOOST_PP_EMPTY() )
#else
#define SIG_SEND(...) BOOST_PP_CAT( BOOST_PP_OVERLOAD(SIGNAL_, __VA_ARGS__ )(__VA_ARGS__, __COUNTER__), BOOST_PP_EMPTY() )
#endif

#define SIGNALS_END_(N) virtual void setup_signals(manager* manager) { _sig_manager = manager; signal_registerer<N>::Register(this, _sig_manager);}
#define SIGNALS_END SIGNALS_END_(__COUNTER__)


// name / signature
#ifdef _MSC_VER
#define SIG_DEF(...) BOOST_PP_CAT( BOOST_PP_OVERLOAD(SIG_DEF_, __VA_ARGS__ )(__VA_ARGS__), BOOST_PP_EMPTY() )
#else
#define SIG_DEF(...) BOOST_PP_CAT( BOOST_PP_OVERLOAD(SIG_DEF_, __VA_ARGS__ )(__VA_ARGS__), BOOST_PP_EMPTY() )
#endif
namespace Signals
{
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
    public:

    };
    
}
