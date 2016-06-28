#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <signals/signal.h>
#include <signals/signaler.h>
#include <signals/serialization.h>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "SignalSerializationModule"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Serialization)

BOOST_AUTO_TEST_CASE(VoidSignal)
{
    Signals::typed_signal_base<void(void)> signal;
    auto proxy = Signals::serialization::text::factory::instance()->get_proxy(signal.get_signal_type());
    BOOST_CHECK(proxy);
    bool called = false;
    auto connection = signal.connect([&called]()
    {
        called = true;
    });
    proxy->send(&signal, "");
    BOOST_CHECK(called);
    delete proxy;
}

BOOST_AUTO_TEST_CASE(IntSignal)
{
    Signals::typed_signal_base<void(int)> signal;
    auto proxy = Signals::serialization::text::factory::instance()->get_proxy(signal.get_signal_type());
    BOOST_CHECK(proxy);
    int value = 0;
    auto connection = signal.connect([&value](int value_in)
    {
        value = value_in;
    });
    proxy->send(&signal, "5");
    BOOST_CHECK_EQUAL(value,5);
    delete proxy;
}

BOOST_AUTO_TEST_CASE(Int2Signal)
{
    Signals::typed_signal_base<void(int, int)> signal;
    auto proxy = Signals::serialization::text::factory::instance()->get_proxy(signal.get_signal_type());
    BOOST_CHECK(proxy);
    int value = 0;
    auto connection = signal.connect([&value](int a, int b)
    {
        value = a + b;
    });
    proxy->send(&signal, "5 ! 6");
    BOOST_CHECK_EQUAL(value, 11);
    delete proxy;
}

BOOST_AUTO_TEST_CASE(StringSignal)
{
    Signals::typed_signal_base<void(std::string)> signal;
    auto proxy = Signals::serialization::text::factory::instance()->get_proxy(signal.get_signal_type());
    BOOST_CHECK(proxy);
    std::string string;
    auto connection = signal.connect([&string](std::string str)
    {
        string = str;
    });
    proxy->send(&signal, "test");
    BOOST_CHECK_EQUAL(string, "test");
    delete proxy;
}

BOOST_AUTO_TEST_CASE(StringComplexSingleSignal)
{
    Signals::typed_signal_base<void(std::string)> signal;
    auto proxy = Signals::serialization::text::factory::instance()->get_proxy(signal.get_signal_type());
    BOOST_CHECK(proxy);
    std::string string;
    auto connection = signal.connect([&string](std::string str)
    {
        string = str;
    });
    proxy->send(&signal, "asdflk;jlkasjdlkfjasdlf ! \" lkajsdflkjasdf \"");
    BOOST_CHECK_EQUAL(string, "asdflk;jlkasjdlkfjasdlf ! \" lkajsdflkjasdf \"");
    delete proxy;
}


BOOST_AUTO_TEST_CASE(String2Signal)
{
    Signals::typed_signal_base<void(std::string, std::string)> signal;
    auto proxy = Signals::serialization::text::factory::instance()->get_proxy(signal.get_signal_type());
    BOOST_CHECK(proxy);
    std::string string1;
    std::string string2;
    auto connection = signal.connect([&string1, &string2](std::string str1, std::string str2)
    {
        string1 = str1;
        string2 = str2;
    });
    proxy->send(&signal, "test1 ! test2");
    BOOST_CHECK_EQUAL(string1, "test1");
    BOOST_CHECK_EQUAL(string2, "test2");
    delete proxy;
}

BOOST_AUTO_TEST_CASE(QuotedString)
{
    Signals::typed_signal_base<void(std::string, int)> signal;
    auto proxy = Signals::serialization::text::factory::instance()->get_proxy(signal.get_signal_type());
    BOOST_CHECK(proxy);
    std::string string;
    int val = 0;
    auto connection = signal.connect([&string, &val](std::string str, int value)
    {
        string = str;
        val = value;
    });
    proxy->send(&signal, "\"test ! asdf\" ! 5");
    BOOST_CHECK_EQUAL(string, "test ! asdf");
    BOOST_CHECK_EQUAL(val, 5);
    delete proxy;
}



BOOST_AUTO_TEST_SUITE_END()