#pragma once
#include <future>
#include "Defs.h"
#include <vector>
#include <memory>
namespace Signals
{
    template<class R> class SIGNAL_EXPORTS default_combiner
    {
	public:
        void operator()(R val)
        {
            
        }
        void operator()(std::future<R>& val)
        {
        
        }
        R get_result()
        {
            return R();
        }
    };
    template<> class SIGNAL_EXPORTS default_combiner<void>
    {
	public:
        void operator()();
        void operator()(std::future<void> val);
        void get_result();
    private:
        std::vector<std::shared_future<void>> results;
    };
    template<> class SIGNAL_EXPORTS default_combiner<bool>
    {
	public:
        default_combiner();
        void operator()(bool val);
        void operator()(std::future<bool> val);
        bool get_result();
    private:
        std::vector<std::shared_future<bool>> results;
    };
    
}
