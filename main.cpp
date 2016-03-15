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
		// Raw signals without a manager or signal registry, similar usage to boost signals
		Signals::typed_signal_base<void(int, int)> test;
        // The work thread runs a simple message receiver loop
		boost::thread work_thread(std::bind([]()->void{while (!boost::this_thread::interruption_requested()){ Signals::thread_specific_queue::run(); }}));
        
        // Everytime the test signal sends a signal, receive it with the static function "function" on the work thread
		auto receiver_connection = test.connect(std::bind(&function, std::placeholders::_1, std::placeholders::_2), work_thread.get_id());
		for (int i = 0; i < 5; ++i)
		{
			BOOST_LOG_TRIVIAL(info) << "Emitting signal";
			test(i, i + 1); // Send signal on main thread, write out on worker thread
		}

        // Create a serialization proxy for test's signature and send serialized data to the signal
        auto proxy = Signals::serialization::text::factory::instance()->get_proxy(&test);
        if(proxy)
        {
            BOOST_LOG_TRIVIAL(info) << "Serialized sending of signal";
            // Test sending a signal from a text string
            proxy->send(&test, "5 ! 6");
            proxy->install(&test);
        }else
        {
            LOG(error) << "Null proxy returned for signal of type " << test.get_signal_type().name();
        }
        // Shutdown the workthread
		work_thread.interrupt();
		work_thread.join();

        // test serializzing to a stringstream
        std::stringstream ss;
        proxy->set_output_iostream(&ss);
        test(10, 11);
        std::string serialized = ss.str();
        proxy->set_output_iostream(nullptr);
        // Test serializing a signal to std::cout
        for(int i = 0; i < 5; ++i)
        {
            BOOST_LOG_TRIVIAL(info) << "Emitting signal, to be auto serialized";
            std::cout << "Auto serialized value: "; test(i, i+1);
        }
        delete proxy;
	}

	{
		BOOST_LOG_TRIVIAL(info) << "Testing signalling class with signal owned by manager";
		TestSignalerImpl test_signaler1;
		TestSignalerImpl test_signaler2;
		
        // By connecting the signal through the signal manager with corresponding description, line, and file information.  A signal map can be generated so that we know what is sending and receiving a signal
        // The test signaler class automatically registers to the signal manager as a sender of a particular signal.  Currently senders and receivers are not deregistered with disconnection of signals.
		auto connection = Signals::signal_manager::get_instance()->connect<void(int)>("test1", [](int i)->void{BOOST_LOG_TRIVIAL(info) << "Test sink: " << i; }, boost::this_thread::get_id(), "Test lambda receiver", __LINE__, __FILE__);
		//test_signaler1.sig_test10(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
		test_signaler2.sig_test1(0);
	}
	Signals::signal_manager::get_instance()->print_signal_map();
	{
        Signals::typed_signal_base<void(void)> test;
        auto proxy = Signals::serialization::text::factory::instance()->get_proxy(&test);
        if(proxy)
        {
            proxy->send(&test, "");

            delete proxy;
        }
	}
    return 0;    
}
