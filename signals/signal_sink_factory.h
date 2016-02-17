#pragma once
#include <functional>
#include <memory>
#include <map>
#include <set>
#include "LokiTypeInfo.h"
#include "Defs.h"

namespace Signals
{
    class SIGNAL_EXPORTS signal_sink_base;

    class SIGNAL_EXPORTS signal_sink_factory
    {
        std::map<std::string, std::map<Loki::TypeInfo, std::function<signal_sink_base*(void)> >> registry;
    public:
        static signal_sink_factory* instance();
		std::set<std::string> list_available_sinks();
		std::set<std::string> list_available_sinks(Loki::TypeInfo type);
        void register_creator(std::function<signal_sink_base*(void)> f, Loki::TypeInfo type, const std::string& sink_type);
        std::shared_ptr<signal_sink_base> create_log_sink(Loki::TypeInfo type, const std::string& sink_type);
    };
	

	template<class ...T> struct multi_sink
	{

	};
	
	template<class Sig, class Sink> struct sink_constructor
	{
		sink_constructor()
		{
			signal_sink_factory::instance()->register_creator(std::bind(&sink_constructor<Sig,Sink>::create), Loki::TypeInfo(typeid(Sig)), Sink::sink_type());
		}
		static Sink* create()
		{
			return new Sink();
		}
	};
	template<class Sig, class... Args> struct recursive_constructor
	{
	};
	template<class Sig, class C> struct recursive_constructor<Sig, C>
	{
		static void register_sink()
		{
			signal_sink_factory::instance()->register_creator(std::bind(&recursive_constructor<Sig, C>::create), Loki::TypeInfo(typeid(Sig)), C::sink_type());
		}
		static C* create()
		{
			return new C();
		}
	};
	template<class Sig, class C, class... Args> struct recursive_constructor<Sig, C, Args...>
	{
		static void register_sink()
		{
			signal_sink_factory::instance()->register_creator(std::bind(&recursive_constructor<Sig, C, Args...>::create), Loki::TypeInfo(typeid(Sig)), C::sink_type());
			recursive_constructor<Sig,Args...>::register_sink();
		}
		static C* create()
		{
			return new C();
		}
	};
	
	template<class Sig, class ... T> struct sink_constructor < Sig, multi_sink<T...>>
	{
		sink_constructor()
		{
			recursive_constructor<Sig, T...>::register_sink();
		}
	};
}
