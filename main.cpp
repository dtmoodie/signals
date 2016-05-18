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
        SLOT_DEF(test_slot, void, int);
        REGISTER_SLOT(test_slot);
        AUTO_SLOT(test_slot, void, int, int);
	SIGNALS_END;
	int test0_counter = 0;
	int test1_counter = 0;
	int test2_counter = 0;
	int test3_counter = 0;
	int test4_counter = 0;
	int test5_counter = 0;
	int test6_counter = 0;
};

class Derived: public TestSignalerImpl
{
public:
	SIGNALS_BEGIN(Derived, TestSignalerImpl);
		SIG_SEND(test0);
        AUTO_SLOT(test_slot, void, int, int, int);
	SIGNALS_END;

    void test_slot(int);
	int dtest3_counter = 0;
	int dtest1_counter = 0;
};
void TestSignalerImpl::test_slot(int a)
{
    LOG(info) << __FUNCTION__;
	test1_counter = test1_counter + a;
}
void TestSignalerImpl::test_slot(int a, int b)
{
    LOG(info) << __FUNCTION__;
    test2_counter += a+b;
}


void Derived::test_slot(int a)
{
	LOG(info) << __FUNCTION__;
	dtest1_counter = dtest1_counter  + a;
}
void Derived::test_slot(int a, int b, int c)
{
    dtest3_counter += a + b + c;
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

		Signals::signal_manager mgr;

		{
			TestSignalerImpl test_signaler1;

			test_signaler1.setup_signals(&mgr);
			(*mgr.get_signal<void(int)>("test_slot"))(1);
			{
				Derived derived;
                TestSignalerImpl* ptr = &derived;
				//derived.setup_signals(&mgr);
                ptr->setup_signals(&mgr);
				(*mgr.get_signal<void(int)>("test_slot"))(2);
                (*mgr.get_signal<void(int,int)>("test_slot"))(1,2);
                (*mgr.get_signal<void(int,int, int)>("test_slot"))(1,2, 3);
			}
			(*mgr.get_signal<void(int)>("test_slot"))(3);
		}
        // By connecting the signal through the signal manager with corresponding description, line, and file information.  A signal map can be generated so that we know what is sending and receiving a signal
        // The test signaler class automatically registers to the signal manager as a sender of a particular signal.  Currently senders and receivers are not deregistered with disconnection of signals.
		auto connection = Signals::signal_manager::get_instance()->connect<void(int)>("test1", [](int i)->void{LOG(info) << "Test sink: " << i; }, get_this_thread(), "Test lambda receiver", __LINE__, __FILE__);
        
		
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
