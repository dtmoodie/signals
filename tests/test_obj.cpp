#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <signals/signal.h>
#include <signals/signaler.h>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "SignalerObjectModule"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(SignalerObject)
class test_receiver: public Signals::signaler
{
public:
	SIGNALS_BEGIN(test_receiver)
		AUTO_SLOT(void, auto_slot);
		AUTO_SLOT(void, auto_slot, int);
		SLOT_DEF(void, test_slot);
		SLOT_DEF(void, test_slot, int);
		SLOT_DEF(void, test_slot, int, int);
	SIGNALS_END;

	int count0;
	int count1;
	int count2;
};

void test_receiver::auto_slot()
{
	++count0;
}
void test_receiver::auto_slot(int val)
{
	count1 += val;
}
void test_receiver::test_slot()
{
	++count0;
}
void test_receiver::test_slot(int val)
{
	count1 += val;
}
void test_receiver::test_slot(int a, int b)
{
	count2 += a + b;
}
class test_sender: public Signals::signaler
{
public:
	SIGNALS_BEGIN(test_sender);
		SIG_SEND(auto_slot); 
		SIG_SEND(auto_slot, int);
		SIG_SEND(test_slot);
		SIG_SEND(test_slot, int);
		SIG_SEND(test_slot, int, int);
	SIGNALS_END;

};
class test_derived_receiver: public test_receiver
{
public:
	SIGNALS_BEGIN(test_derived_receiver, test_receiver);
		AUTO_SLOT(void, auto_slot);
		SLOT_DEF(void, test_slot);
	SIGNALS_END;
	int derived_count0;
};
void test_derived_receiver::auto_slot()
{
	++derived_count0;
}
void test_derived_receiver::test_slot()
{
	++derived_count0;
}
class test_derived_sender: public test_sender
{
public:
};

BOOST_AUTO_TEST_CASE(ExternalSignal)
{
	test_receiver obj;
	obj.count0 = 0;
	obj.count1 = 0;
	obj.count2 = 0;
	Signals::typed_signal_base<void(void)> signal0;
	Signals::typed_signal_base<void(int)> signal1;
	Signals::typed_signal_base<void(int)> signal2;
	Signals::typed_signal_base<void(int, int)> signal3;
	
	
	BOOST_CHECK_EQUAL(obj.connect("auto_slot", &signal0), 1);
	signal0();
	BOOST_CHECK_EQUAL(obj.count0, 1);
	BOOST_CHECK_EQUAL(obj.disconnect_from_signal(&signal0), true);
	signal0();
	BOOST_CHECK_EQUAL(obj.count0, 1);
	
	BOOST_CHECK_EQUAL(obj.connect("auto_slot", &signal1), 1);
	signal1(5);
	BOOST_CHECK_EQUAL(obj.count1, 5);
	BOOST_CHECK_EQUAL(obj.disconnect_from_signal(&signal1), true);
	signal1(5);
	BOOST_CHECK_EQUAL(obj.count1, 5);

	obj.count0 = 0;
	obj.count1 = 0;
	obj.count2 = 0;

	obj.connect("test_slot", &signal2);
	signal2(1);
	BOOST_CHECK_EQUAL(obj.count1, 1);

	obj.connect("test_slot", &signal3);
	signal3(1,2);
	BOOST_CHECK_EQUAL(obj.count2, 3);
}

class simple_receiver: public Signals::signaler
{
public:
	SIGNALS_BEGIN(simple_receiver);
		AUTO_SLOT(void, test1);
		AUTO_SLOT(void, test2);
		AUTO_SLOT(void, test3);
	SIGNALS_END;
	int count1, count2, count3;
};
void simple_receiver::test1(){ ++count1;}
void simple_receiver::test2(){ ++count2;}
void simple_receiver::test3(){ ++count3;}
BOOST_AUTO_TEST_CASE(SignalManager)
{
	Signals::signal_manager manager;
	simple_receiver obj;
	obj.count1 = 0; obj.count2 = 0; obj.count3 = 0;
	BOOST_CHECK_EQUAL(obj.setup_signals(&manager), 3);
	(*manager.get_signal<void(void)>("test1"))();
	BOOST_CHECK_EQUAL(obj.count1, 1);
	(*manager.get_signal<void(void)>("test2"))();
	BOOST_CHECK_EQUAL(obj.count2, 1);
	(*manager.get_signal<void(void)>("test3"))();
	BOOST_CHECK_EQUAL(obj.count3, 1);
}

class base: public Signals::signaler
{
public:
	SIGNALS_BEGIN(base);
	SLOT_DEF(void, overloaded_slot);
	SIGNALS_END;
	int count;
};
void base::overloaded_slot()
{
	count = 1; 
}
class derived: public base
{
public:
	SIGNALS_BEGIN(derived, base);
	SLOT_DEF(void, overloaded_slot);
	SIGNALS_END;
};
void derived::overloaded_slot()
{
	count = 2; 
}
BOOST_AUTO_TEST_CASE(TestDerivedOverload)
{
	
	std::cout << "-------------------- TestDerivedOverload ----------------\n";
	base base_obj;
	base_obj.count = 0;
	Signals::signal_manager mgr1;
	base_obj.setup_signals(&mgr1);
	
	BOOST_CHECK_EQUAL(base_obj.Signals::signaler::connect_by_name("overloaded_slot"), 1);
	(*mgr1.get_signal<void(void)>("overloaded_slot"))();
	BOOST_CHECK_EQUAL(base_obj.count, 1);

	derived derived_obj;
	derived_obj.setup_signals(&mgr1);
	derived_obj.count = 0;
	BOOST_CHECK_EQUAL(derived_obj.Signals::signaler::connect_by_name("overloaded_slot"), 1);
	(*mgr1.get_signal<void(void)>("overloaded_slot"))();
	BOOST_CHECK_EQUAL(derived_obj.count, 2);
}

class auto_base: public Signals::signaler
{
public:
	SIGNALS_BEGIN(auto_base);
	AUTO_SLOT(void, overloaded_slot);
	SIGNALS_END;
	int count;
};
void auto_base::overloaded_slot()
{
	count = 1; 
}
class auto_derived: public auto_base
{
public:
	SIGNALS_BEGIN(auto_derived, auto_base);
	AUTO_SLOT(void, overloaded_slot);
	SIGNALS_END;
};
void auto_derived::overloaded_slot()
{
	count = 2; 
}

BOOST_AUTO_TEST_CASE(AutoOverloadedSlot)
{
	std::cout << "-------------------- AutoOverloadedSlot ----------------\n";
	auto_base base_obj;
	base_obj.count = 0;
	Signals::signal_manager mgr1;
	base_obj.setup_signals(&mgr1);

	BOOST_CHECK_EQUAL(base_obj.Signals::signaler::connect_by_name("overloaded_slot"), 1);
	(*mgr1.get_signal<void(void)>("overloaded_slot"))();
	BOOST_CHECK_EQUAL(base_obj.count, 1);

	auto_derived derived_obj;
	derived_obj.setup_signals(&mgr1);
	derived_obj.count = 0;
	BOOST_CHECK_EQUAL(derived_obj.Signals::signaler::connect_by_name("overloaded_slot"), 1);
	(*mgr1.get_signal<void(void)>("overloaded_slot"))();
	BOOST_CHECK_EQUAL(derived_obj.count, 2);
}

class polymorphic_slot: public Signals::signaler
{
public:
	SIGNALS_BEGIN(polymorphic_slot);
		SLOT_DEF(void, poly_slot);
		SLOT_DEF(void, poly_slot, int);
		REGISTER_SLOT(poly_slot);
	SIGNALS_END;
	int count = 0;
};
void polymorphic_slot::poly_slot()
{
	++count;
}
void polymorphic_slot::poly_slot(int val)
{
	count += val;
}
BOOST_AUTO_TEST_CASE(PolymorphicSlot)
{
	polymorphic_slot obj;
	obj.count = 0;
	Signals::signal_manager mgr;
	BOOST_CHECK_EQUAL(obj.setup_signals(&mgr), 2);
	(*mgr.get_signal<void()>("poly_slot"))();
	BOOST_CHECK_EQUAL(obj.count, 1);
	(*mgr.get_signal<void(int)>("poly_slot"))(2);
	BOOST_CHECK_EQUAL(obj.count, 3);
}


BOOST_AUTO_TEST_CASE(SignalSender)
{
	std::cout << "-------------------- SignalSender ----------------\n";
	test_receiver receiver;
	receiver.count0 = 0;
	receiver.count1 = 0;
	receiver.count2 = 0;
	test_sender sender;

	Signals::signal_manager manager;
	receiver.setup_signals(&manager);
	sender.setup_signals(&manager);
	sender.sig_auto_slot();
	BOOST_CHECK_EQUAL(receiver.count0, 1);
	sender.sig_test_slot(); // Should not call slot because it wasn't automatically linked
	BOOST_CHECK_EQUAL(receiver.count0, 1);
	sender.sig_auto_slot(1);
	BOOST_CHECK_EQUAL(receiver.count1, 1);
	receiver.connect_by_name("test_slot", &manager);
	sender.sig_test_slot();
	BOOST_CHECK_EQUAL(receiver.count0, 2);
}
BOOST_AUTO_TEST_CASE(DerivedSignalReceiver)
{
	std::cout << "-------------------- DerivedSignalReceiver ----------------\n";
	test_sender sender;
	test_derived_receiver receiver;
	receiver.count0 = 0;
	receiver.count1 = 0;
	receiver.count2 = 0;
	receiver.derived_count0 = 0;
	Signals::signal_manager manager;
	int sender_signals = sender.setup_signals(&manager);
	std::cout << "================================\n";
	int receiver_signals = receiver.setup_signals(&manager);
	std::cout << "================================\n";
	sender.sig_auto_slot();
	BOOST_CHECK_EQUAL(receiver.count0, 0); // Should not be called because derived class overloads
	BOOST_CHECK_EQUAL(receiver.derived_count0, 1); // Should be called

	sender.sig_auto_slot(1);
	BOOST_CHECK_EQUAL(receiver.count1, 1); // Should be called because no overload exists

}
BOOST_AUTO_TEST_SUITE_END()