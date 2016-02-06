#include "combiner.h"
using namespace Signals;

void default_combiner<void>::operator()()
{

}
void default_combiner<void>::operator()(std::future<void> val)
{

}
void default_combiner<void>::get_result()
{

}

default_combiner<bool>::default_combiner()
{
}

void default_combiner<bool>::operator()(bool val)
{

}
void default_combiner<bool>::operator()(std::future<bool> val)
{
    results.push_back(val.share());
    //results.push_back(val);
}
bool default_combiner<bool>::get_result()
{
    bool ret = true;
    for(int i = 0; i < results.size(); ++i)
    {
        ret = ret && results[i].get();
    }
    return ret;
}
