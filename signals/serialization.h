#pragma once
#include "Defs.h"
namespace signals
{
    template<class... T> class EXPORT_SIGNALS Serializer
    {
    public:
        static void serialize(T... args)
        {
        }
    };

    template<class T1, class ... Ts> class EXPORT_SIGNALS Serializer<T1, Ts...> : public Serializer<Ts...>
    {
    public:
        static void serialize(T1 arg1, Ts... args)
        {
            std::cout << arg1 << " ";
            Serializer<Ts...>::serialize(args...);
        }
        static void serialize_ss(std::stringstream& ss, T1 arg1, Ts... args)
        {
            ss << arg1 << ", ";
            Serializer<Ts...>::serialize_ss(ss, args...);
        }
        static void deserialize(std::stringstream& ss, T1& arg1, Ts&... args)
        {
            ss >> arg1;
            Serializer<Ts...>::deserialize(ss, args...);
        }
    };
} // namespace signals