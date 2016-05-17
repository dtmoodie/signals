#include "signals/signal_sink.h"
#include "signals/signaler.h"
#include "signals/inter_thread.h"
#include "signals/signal_manager.h"
#include "signals/signal_sink_factory.h"
#include "signals/signal_with_sink.h"
#include "signals/thread_registry.h"
#include "signals/boost_thread.h"

#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
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
	LOG(info) << "Receiving signal " << x << ", " << y;
}


class TestSignalerImpl: public Signals::signaler
{
public:
	SIGNALS_BEGIN(TestSignalerImpl);
		SIG_SEND(test0);
	    SIG_SEND(test1, int);
        SIG_SEND(test1, double);
	    SIG_SEND(test2, int, int);
	    SIG_SEND(test3, int, int, double);
	    SIG_SEND(test4, int, int, double, int);
	    SIG_SEND(test5, int, int, double, double, double);
	    SIG_SEND(test6, int, int, int, int, int, int);
	    SIG_SEND(test7, int, int, int, int, int, int, int);
	    SIG_SEND(test8, int, int, int, int, int, int, int, int);
	    SIG_SEND(test9, int, int, int, int, int, int, int, int, int);
        SIG_SEND(test9, int, int, int, int, int, int, int, int, double);
        SIG_SEND(test9, int, int, int, int, int, int, int, double, double);
        SLOT_DEF(test_slot, void);
        REGISTER_SLOT(test_slot);
        SLOT_DEF(test_slot, void, int);
        REGISTER_SLOT(test_slot);
        SLOT_DEF(test_slot, void, int, int);
        REGISTER_SLOT(test_slot);
        SLOT_DEF(test_slot, void, int, int, int);
        REGISTER_SLOT(test_slot);
        SLOT_DEF(test_slot, void, int, int, int, int);
        REGISTER_SLOT(test_slot);
        SLOT_DEF(test_slot, void, int, int, int, int, int);
        REGISTER_SLOT(test_slot);
        SLOT_DEF(test_slot, void, int, int, int, int, int, int);
        REGISTER_SLOT(test_slot);
	SIGNALS_END();
};
void TestSignalerImpl::test_slot(int, int, int, int, int, int)
{
    LOG(info) << __FUNCTION__;
}
void TestSignalerImpl::test_slot(int, int, int, int, int)
{
    LOG(info) << __FUNCTION__;
}
void TestSignalerImpl::test_slot(int, int, int, int)
{
    LOG(info) << __FUNCTION__;
}
void TestSignalerImpl::test_slot(int, int, int)
{
    LOG(info) << __FUNCTION__;
}
void TestSignalerImpl::test_slot(int, int)
{
    LOG(info) << __FUNCTION__;
}
void TestSignalerImpl::test_slot(int)
{
    LOG(info) << __FUNCTION__;
}
void TestSignalerImpl::test_slot()
{
    LOG(info) << __FUNCTION__;
}

SIGNAL_IMPL(TestSignalerImpl);


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
    std::cout <<  Signals::signal_registry::instance()->print_signal_map() << std::endl;
	{
		LOG(info) << "Testing sending and receiving of signals on different threads";
		// Raw signals without a manager or signal registry, similar usage to boost signals
		Signals::typed_signal_base<void(int, int)> test;
        // The work thread runs a simple message receiver loop
		boost::thread work_thread(std::bind([]()->void{while (!boost::this_thread::interruption_requested()){ Signals::thread_specific_queue::run(); }}));
        
        // Everytime the test signal sends a signal, receive it with the static function "function" on the work thread
        
		auto receiver_connection = test.connect(std::bind(&function, std::placeholders::_1, std::placeholders::_2), get_thread_id(work_thread.get_id()));
		for (int i = 0; i < 5; ++i)
		{
			LOG(info) << "Emitting signal";
			test(i, i + 1); // Send signal on main thread, write out on worker thread
		}

        // Create a serialization proxy for test's signature and send serialized data to the signal
        auto proxy = Signals::serialization::text::factory::instance()->get_proxy(&test);
        if(proxy)
        {
			LOG(info) << "Serialized sending of signal";
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
			LOG(info) << "Emitting signal, to be auto serialized";
            std::cout << "Auto serialized value: ";  test(i, i+1);
            std::cout << "\n";
        }
        delete proxy;
	}

	{
		LOG(info) << "Testing signalling class with signal owned by manager";
		TestSignalerImpl test_signaler1;
		TestSignalerImpl test_signaler2;
        typed_signal_base<void(int, int, int, int, int, int)> auto_connected_signal;
        test_signaler1.connect_signal("test_slot", &auto_connected_signal);
        auto_connected_signal(5,4,3,2,1,0);
		Signals::signal_manager mgr;
        test_signaler1.setup_signals(&mgr);
        (*mgr.get_signal<void()>("test_slot"))();
        (*mgr.get_signal<void(int)>("test_slot"))(0);
        (*mgr.get_signal<void(int,int)>("test_slot"))(5,4);
        (*mgr.get_signal<void(int, int, int)>("test_slot"))(0,1,2);
        (*mgr.get_signal<void(int, int, int, int)>("test_slot"))(0,1,2,3);
        (*mgr.get_signal<void(int, int, int, int, int)>("test_slot"))(0,1,2,3,4);
        (*mgr.get_signal<void(int, int, int, int, int, int)>("test_slot"))(0,1,2,3,4,5);

        // By connecting the signal through the signal manager with corresponding description, line, and file information.  A signal map can be generated so that we know what is sending and receiving a signal
        // The test signaler class automatically registers to the signal manager as a sender of a particular signal.  Currently senders and receivers are not deregistered with disconnection of signals.
		auto connection = Signals::signal_manager::get_instance()->connect<void(int)>("test1", [](int i)->void{LOG(info) << "Test sink: " << i; }, get_this_thread(), "Test lambda receiver", __LINE__, __FILE__);
        
		//test_signaler1.sig_test10(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
		int val = 0;
		test_signaler2.sig_test1(val);
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
