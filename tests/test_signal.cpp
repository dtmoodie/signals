
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <signals/signal.h>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "RawSignalModule"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(RawSignal)

int value = 0;
static void incrementor()
{
	++value;
}
BOOST_AUTO_TEST_CASE(StaticFunction)
{
	Signals::typed_signal_base<void(void)> signal;
	{
		auto connection = signal.connect(std::bind(&incrementor));
		signal();
		BOOST_CHECK_EQUAL(value, 1);
	}
	signal();
	BOOST_CHECK_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(LambdaFunction)
{
	int val = 0;
	Signals::typed_signal_base<void(void)> signal;
	{
		auto connection = signal.connect([&val](){++val;});
		signal();
		BOOST_CHECK_EQUAL(val, 1);
	}
	signal();
	BOOST_CHECK_EQUAL(val, 1);
}

BOOST_AUTO_TEST_CASE(MemberFunction)
{
	class test_member
	{
	public:
		void foo()
		{
			++count;
		}
		int count;
	};

	Signals::typed_signal_base<void(void)> signal;
	test_member test_obj;
	test_obj.count = 0;
	{
		auto connection = signal.connect(std::bind(&test_member::foo, &test_obj));
		signal();
		BOOST_CHECK_EQUAL(test_obj.count, 1);
	}
	signal();
	BOOST_CHECK_EQUAL(test_obj.count, 1);
}

BOOST_AUTO_TEST_SUITE_END()