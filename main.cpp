#include "signals/signal_sink.h"
#include "signals/signaler.h"
#include "signals/inter_thread.h"
#include "signals/signal_manager.h"
#include "signals/signal_sink_factory.h"
#include "signals/signal_with_sink.h"
#include "signals/thread_registry.h"
class connection;

struct test_class
{
    bool func(int x, int y)
    {
        //std::cout << x << ", " << y << std::endl;
        return false;
    }
};

void function(int x, int y)
{
    //std::cout << x << ": " << y << std::endl;
	BOOST_LOG_TRIVIAL(info) << "Receiving signal " << x << ", " << y;
}


class TestSignalerImpl: public Signals::signaler
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

template<class...T> class log_sink : public Signals::signal_sink<T...>
{
public:
	static const char* sink_type()
	{
		return "log_sink";
	}
	virtual const char* get_sink_type()
	{
		return sink_type();
	}
	virtual void handle_signal(T... args)
	{

	}
	virtual void handle_return()
	{

	}
};

using namespace Signals;
int main()
{
	
	{
		BOOST_LOG_TRIVIAL(info) << "Testing sending and receiving of signals on different threads";
		// Raw signals without a manager or signal registry
		Signals::typed_signal_base<void(int, int)> test;
		boost::thread work_thread(std::bind([]()->void{while (!boost::this_thread::interruption_requested()){ Signals::thread_specific_queue::run(); }}));
		auto receiver_connection = test.connect(std::bind(&function, std::placeholders::_1, std::placeholders::_2), work_thread.get_id());
		for (int i = 0; i < 100; ++i)
		{
			BOOST_LOG_TRIVIAL(info) << "Emitting signal";
			test(i, i + 1); // Send signal on main thread, write out on worker thread
		}
		work_thread.interrupt();
		work_thread.join();
	}

	{
		BOOST_LOG_TRIVIAL(info) << "Testing signalling class with signal owned by manager";
		TestSignalerImpl test_signaler1;
		TestSignalerImpl test_signaler2;
		//auto connection = Signals::signal_manager::get_instance()->connect<void(int)>("test1", [](int i)->void{BOOST_LOG_TRIVIAL(info) << "Test sink: " << i; });
		auto connection = Signals::signal_manager::get_instance()->connect<void(int)>("test1", [](int i)->void{BOOST_LOG_TRIVIAL(info) << "Test sink: " << i; }, boost::this_thread::get_id(), "Test lambda receiver", __LINE__, __FILE__);
		//test_signaler1.sig_test10(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
		test_signaler2.sig_test1(0);
	}
	Signals::signal_manager::get_instance()->print_signal_map();
	{


	}


	// classes that uses signals and the signal manaer
    /*test_class clas;
    

    auto test_ = signal_manager::get_instance()->get_signal<bool(int,int)>("test");
	Signals::signal<void(int), default_combiner<void>, multi_sink<signal_sink<void(int)>, log_sink<int>>> test_multi_sink;
    thread_registry::get_instance()->register_thread(thread_type::GUI);
    thread_registry::get_instance()->register_thread(thread_type::processing, work_thread.get_id());
	TestSignalerImpl signaler_class;
	signaler_class.setup_signals(signal_manager::get_instance());
	signaler_class.sig_test10(1,2,3,4,5,6,7,8,9,10);
	
    auto sink = signal_sink_factory::instance()->create_log_sink(test.get_signal_type(), "null_sink");
	
    test.add_log_sink(sink);
    {
        auto connection = test.connect(std::bind(&test_class::func, &clas, std::placeholders::_1, std::placeholders::_2), work_thread.get_id());
		
        for(int i = 0; i < 100000; ++i)
        {
            auto result = test(5, 6);
            result.get_result();
        }        
    }
    test(5, 6);
    
    
	*/
    return 0;    
}
