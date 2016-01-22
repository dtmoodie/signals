#include "signals/signal_sink.h"
#include "signals/signaler.h"
#include "signals/inter_thread.h"
#include "signals/signal_manager.h"
#include "signals/signal_sink_factory.h"

class connection;

struct test_class
{
    void func(int x, int y)
    {
        std::cout << x << ", " << y << std::endl;;
    }
};

void function(int x, int y)
{
    std::cout << x << ": " << y << std::endl;;
}


class TestSignalerImpl: public signals::signaler
{
public:
    SIG_DEF(test1, int);
    SIG_DEF(test2, int, int);
    SIG_DEF(test3, int, int, double);
    SIG_DEF(test4, int, int, double, int);
    SIG_DEF(test5, int, int, double, double, double);
    SIG_DEF(test6, int, int, int, int, int, int);
    SIG_DEF(test7, int, int, int, int, int, int, int);
    SIG_DEF(test8, int, int, int, int, int, int, int, int);
    SIG_DEF(test9, int, int, int, int, int, int, int, int, int);
    SIG_DEF(test10, int, int, int, int, int, int, int, int, int, int);
};



using namespace signals;
int main()
{
    test_class clas;
    boost::thread work_thread(std::bind([]()->void{while(!boost::this_thread::interruption_requested()){signals::thread_specific_queue::run();}}));
    auto test_ = signals::signal_manager::instance()->get_signal<void(int,int)>("test");
    signal_manager::instance()->register_thread(signal_manager::GUI);
    signal_manager::instance()->register_thread(signal_manager::processing, work_thread.get_id());
    TestSignalerImpl test2;
    test2.sig_test1(5);
    test2.sig_test2(5,6);
    test2.sig_test3(5,6,7);
    signals::SignalWithSink<signals::signal_sink,void(int,int)> test;
    auto sink = signal_sink_factory::instance()->create_log_sink(test.get_signal_type());
    test.add_log_sink(sink);
    {
        auto connection = test.connect(std::bind(&test_class::func, &clas, std::placeholders::_1, std::placeholders::_2), work_thread.get_id());
        for(int i = 0; i < 100000; ++i)
        {
            test(5, 6);
        }        
    }
    test(5, 6);
    
    work_thread.interrupt();
    work_thread.join();
    return 0;    
}