#pragma once
#include "Defs.h"
#include "LokiTypeInfo.h"

#include <functional>
#include <map>

namespace Signals
{
	class signal_base;
	template<class T, template<class> class C> class typed_signal_base;
	template<class T, template<class> class C, template<class...> class S> class typed_signal;

	namespace serialization
	{
		namespace text
		{
			class SIGNAL_EXPORTS serialization_proxy_base
			{
			public:
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
				static void serialize(T... args)
				{
				}
			};

			template<class T1, class ... Ts> class SIGNAL_EXPORTS Serializer<T1, Ts...> : public Serializer<Ts...>
			{
			public:
				static void serialize(T1 arg1, Ts... args)
				{
					std::cout << arg1 << " ";
					Serializer<Ts...>::serialize(args...);
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
			public:
				void install(signal_base* signal)
				{

				}
				void send(signal_base* signal, std::string str)
				{
					typed_signal_base<Sig>* typed_sig = dynamic_cast<typed_signal_base<Sig>*>(signal);
					if (typed_sig)
					{
						std::stringstream ss;
						ss << str;
						T... args;
						Serializer<Sig>::deserialize(ss, args...);
					}
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
