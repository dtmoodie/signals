#pragma once
#include "Defs.h"
#include "LokiTypeInfo.h"
#include "placeholder.h"
#include "connection.h"
#include "logging.hpp"
#include "combiner.h"

#include <sstream>
#include <functional>
#include <map>
#include <memory>
#include <iostream>
template<int ...>
struct seq { };

template<int N, int ...S>
struct gens : gens<N-1, N-1, S...> { };

template<int ...S>
struct gens<0, S...> {
  typedef seq<S...> type;
};

 
namespace Signals
{
    class signal_base;
    template<class Signature,
    template<class> class Combiner = default_combiner
    > class typed_signal_base { };
	template<class T, template<class> class C, template<class...> class S> class typed_signal;

	namespace serialization
	{
        template<int ...S, class R, class ... T, template<class> class C>
          void call_signal(seq<S...>, std::tuple<T...>& params, Signals::typed_signal_base<R(T...), C>* sig) 
          {
              (*sig)(std::get<S>(params)...);
          }

		namespace text
		{
			class SIGNAL_EXPORTS serialization_proxy_base
			{
            protected:
                std::ostream* _dest;
			public:
                serialization_proxy_base();
                virtual ~serialization_proxy_base();
                virtual void set_output_iostream(std::ostream* stream);
				virtual void install(signal_base* signal);
				virtual void send(signal_base* signal, std::string str);
			};

			class SIGNAL_EXPORTS factory
			{
				std::map<Loki::TypeInfo, std::function<serialization_proxy_base*()>> registry;
			public:
				static factory* instance();
				serialization_proxy_base* get_proxy(signal_base* signal);
				serialization_proxy_base* get_proxy(Loki::TypeInfo type);
				void register_proxy(std::function<serialization_proxy_base*()> function, Loki::TypeInfo type);
			};



			template<class... T> class SIGNAL_EXPORTS Serializer
			{
			public:
				static void serialize(std::ostream* dest, T... args)
				{

				}
			};

#ifdef SUPPORTS_EXPRESSION_SFINAE
            // ********************* serialize SFINAE *****************************************
            //http://stackoverflow.com/questions/257288/is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence
            template<typename T> auto serialize_impl(std::ostream& ss, T& val, unsigned int)->decltype(ss << val, void)
            {
            }
            template<typename T> auto serialize_impl(std::ostream& ss, T& val, int)->decltype(ss << val, void)
            {
                ss << " ! " << val;
            }
              
            // ********************* deserialize SFINAE *****************************************
            template<typename T> void deserialize_impl(std::istream& ss, T& val, unsigned int)
            {
                
            }
            template<typename T> auto deserialize_impl(std::istream& ss, T& val, int) ->decltype(ss >> val, void)
            {
                std::string tmp;
                std::getline(ss, tmp, '!');
                std::stringstream tmpss;
                tmpss << tmp;
                tmpss >> val;
            }
#else
            template<typename T> void serialize_impl(std::stringstream& ss, T& val, int,
                typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>::type* = 0)
            {
                ss << " ! " << val;
            }
            template<typename T> void serialize_impl(std::stringstream& ss, T& val, unsigned int,
                typename std::enable_if<!(std::is_integral<T>::value || std::is_floating_point<T>::value)>::type* = 0)
            {
                LOG(debug) << "Non specialized serialize called for " << typeid(T).name();
            }
            template<typename T> void deserialize_impl(std::stringstream& ss, T& val, int,
                typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>::type* = 0)
            {
                std::string tmp;
                std::getline(ss, tmp, '!');
                std::stringstream tmpss;
                tmpss << tmp;
                tmpss >> val;
            }
            template<typename T> void deserialize_impl(std::stringstream& ss, T& val, unsigned int,
                typename std::enable_if<!(std::is_integral<T>::value || std::is_floating_point<T>::value)>::type* = 0)
            {
                LOG(debug) << "Non specialized deserialize called for " << typeid(T).name();
            }
#endif
            template<int N, class... T> class tuple_serializer
            {
            public:
                static void deserialize(std::stringstream& ss, std::tuple<T...>& args)
                {
                    tuple_serializer<N-1, T...>::deserialize(ss, args);
                    deserialize_impl(ss, std::get<N>(args), 0);        
                }
                static void serialize(std::stringstream& ss, std::tuple<T...>& args)
                {
                    serialize_impl(ss, std::get<N>(args), 0);    
                }
            };

            template<class... T> class tuple_serializer<0, T...>
            {
            public:
                static void deserialize(std::stringstream& ss, std::tuple<T...>& args)
                {
                    deserialize_impl(ss, std::get<0>(args), 0);
                }
                static void serialize(std::stringstream& ss, std::tuple<T...>& args)
                {
                    serialize_impl(ss, std::get<0>(args), 0);
                }
            };

			template<class T1, class ... Ts> class SIGNAL_EXPORTS Serializer<T1, Ts...> : public Serializer<Ts...>
			{
			public:
				static void serialize(std::ostream* dest, T1 arg1, Ts... args)
				{
                    if(dest)
                        *dest << arg1 << " ";
                    else
					    std::cout << arg1 << " ";
					Serializer<Ts...>::serialize(dest, args...);
				}
				static void serialize_ss(std::stringstream& ss, T1 arg1, Ts... args)
				{
					ss << arg1 << " ! ";
					Serializer<Ts...>::serialize_ss(ss, args...);
				}
				static void deserialize(std::stringstream& ss, T1& arg1, Ts&... args)
				{
					std::string tmp;
					std::getline(ss, tmp, '!');
					std::stringstream tmpss;
					tmpss << tmp;
					tmpss >> arg1;
					Serializer<Ts...>::deserialize(ss, args...);
                    
                }
			};

			template<typename Sig> class SIGNAL_EXPORTS serialization_proxy: public serialization_proxy_base
			{
			};
			template<class R, class... T> class SIGNAL_EXPORTS serialization_proxy<R(T...)>: public serialization_proxy_base
			{
                std::shared_ptr<connection> _connection;
			public:
				void install(signal_base* signal)
				{
                    auto typed_sig = dynamic_cast<typed_signal_base<R(T...), default_combiner>*>(signal);
                    if(typed_sig)
                    {
                        _connection = typed_sig->connect_log_sink(my_bind(&serialization_proxy<void(T...)>::receive, this,  make_int_sequence<sizeof...(T)>{}));
                    }
				}
				void send(signal_base* signal, std::string str)
				{
                    auto typed_sig = dynamic_cast<typed_signal_base<R(T...), default_combiner>*>(signal);
					if (typed_sig)
					{
						std::stringstream ss;
						ss << str;
                        std::tuple<T...> args;
						tuple_serializer<sizeof...(T) - 1, T...>::deserialize(ss, args);
                        call_signal(typename gens<sizeof...(T)>::type(), args, typed_sig);
					}
				}
                void receive(T... args)
                {
                    Serializer<T...>::serialize(_dest, args...);
                    if(_dest)
                        *_dest << "\n";
                    else
                        std::cout << "\n";
                }
			};
            template<class R> class SIGNAL_EXPORTS serialization_proxy<R()>: public serialization_proxy_base
            {
                std::shared_ptr<connection> _connection;
            public:
				void install(signal_base* signal)
				{
                    auto typed_sig = dynamic_cast<typed_signal_base<R(), default_combiner>*>(signal);
                    if(typed_sig)
                    {
                        _connection = typed_sig->connect_log_sink(std::bind(&serialization_proxy<R()>::receive, this));
                    }
				}
				void send(signal_base* signal, std::string str)
				{
                    auto typed_sig = dynamic_cast<typed_signal_base<R(), default_combiner>*>(signal);
					if (typed_sig)
					{
						(*typed_sig)();
					}
				}
                void receive()
                {
                    
                }
            };

			template<typename Sig> class constructor
			{
			};
			template<typename R, typename...T> class constructor<R(T...)>
			{
			public:
				constructor()
				{
					factory::instance()->register_proxy(std::bind(&constructor<R(T...)>::construct), Loki::TypeInfo(typeid(R(T...))));
				}
				static serialization_proxy_base* construct()
				{
					return new serialization_proxy<R(T...)>();
				}
			};
		} // namespace text
	} // namespace serialization
} // namespace Signals
