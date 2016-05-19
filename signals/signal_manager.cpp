#include "signal_manager.h"
#include "logging.hpp"
#include "signaler.h"
using namespace Signals;




signal_registry::signal_registry()
{

}

signal_registry* signal_registry::instance()
{
    static signal_registry* g_inst = nullptr;
	if(g_inst == nullptr)
		g_inst = new signal_registry();
    return g_inst;
}

void signal_registry::add_signal(Loki::TypeInfo sender, std::string name, Loki::TypeInfo signature)
{
    _static_signal_registry[sender][name].push_back(signature);
}

void signal_registry::add_slot(Loki::TypeInfo receiver, std::string name, Loki::TypeInfo signature)
{
    _static_slot_registry[receiver][name].push_back(signature);
}

void signal_registry::add_python_registration_function(Loki::TypeInfo sender, std::function<void(void)> f)
{
    _static_python_registration_functions[sender] = f;
}
std::string signal_registry::print_signal_map()
{
    std::stringstream ss;
    std::vector<Loki::TypeInfo> _printedSenders;
    for(auto& senders : _static_signal_registry)
    {
        ss << "--------------------------\n";
        ss << senders.first.name() << "\n";
        if(senders.second.size())
            ss << "  ---- SIGNALS ----\n";
        for(auto& sig : senders.second)
        {
            ss << "    " << sig.first << "\n";
            for(auto& s : sig.second)
            {
                ss << "    - " << s.name() << "\n";
            }
        }
        auto itr = _static_slot_registry.find(senders.first);
        if(itr != _static_slot_registry.end())
        {
            ss << "  ---- SLOTS ----\n";
            for(auto & slot : itr->second)
            {
                ss << "    " << slot.first << "\n";
                for(auto& overload : slot.second)
                {
                    ss << "      - " << overload.name() << "\n";
                }
            }
        }
        _printedSenders.push_back(senders.first);
    }
    // Now go through all receiver objects and print the ones that don't have signals
    for(auto& receiver : _static_slot_registry)
    {
        if(std::count(_printedSenders.begin(), _printedSenders.end(), receiver.first) == 0)
        {
            ss << "--------------------------\n";
            ss << receiver.first.name() << "\n";
            ss << "  ---- SLOTS ----\n";
            for(auto& slot : receiver.second)
            {
                ss << "    " << slot.first;
                for(auto& overload : slot.second)
                {
                    ss << "     - " << overload.name() << "\n";
                }            
            }
        }
    }
    return ss.str();
}



signal_manager::signal_manager()
{
}

static signal_manager* g_instance = nullptr;


signal_manager* signal_manager::get_instance()
{
    if (g_instance == nullptr)
    {
        g_instance = new signal_manager();
    }
    return g_instance;
}
void signal_manager::set_instance(signal_manager* inst)
{
    g_instance = inst;
}


std::shared_ptr<signal_base>& signal_manager::get_signal(const std::string& name, Loki::TypeInfo type)
{
    return _signals[type][name];
}
void signal_manager::register_sender(Loki::TypeInfo signal_signature, std::string signal_name, Loki::TypeInfo sender_type, signaler* sender_ptr)
{
	_registered_sender_objects[signal_signature][signal_name].push_back(std::make_tuple(sender_type, sender_ptr, sender_ptr->get_description()));
}
void signal_manager::register_sender(Loki::TypeInfo signal_signature, std::string signal_name, Loki::TypeInfo sender_type, void* sender_ptr, std::string desc)
{
	auto& vec = _registered_sender_objects[signal_signature][signal_name];
	if(std::count_if(vec.begin(), vec.end(), [sender_ptr](const std::tuple<Loki::TypeInfo, void*, std::string>& data){return std::get<1>(data) == sender_ptr; }) == 0)
		vec.push_back(std::make_tuple(sender_type, sender_ptr, desc));
}
void signal_manager::register_sender(Loki::TypeInfo signal_signature, std::string signal_name, std::string desc, std::string file_name, int line_number)
{
	_registered_senders[signal_signature][signal_name].push_back(std::make_tuple(line_number, file_name, desc));
}
void signal_manager::remove_sender(void* sender_ptr)
{

}
void signal_manager::remove_sender(std::string file_name, int line_number)
{

}

void signal_manager::register_receiver(Loki::TypeInfo signal_signature, std::string signal_name, Loki::TypeInfo receiver_type, void* receiver_ptr, std::string desc)
{
	_registered_receiver_objects[signal_signature][signal_name].push_back(std::make_tuple(receiver_type, receiver_ptr, desc));
}
void signal_manager::register_receiver(Loki::TypeInfo signal_signature, std::string signal_name, int line_number, std::string file_name, std::string desc)
{
	_registered_receivers[signal_signature][signal_name].push_back(std::make_tuple(line_number, file_name, desc));
}

std::vector<signal_base*> signal_manager::get_signals(std::string name)
{
	std::vector<signal_base*> output;
	for (auto& itr : _signals)
	{
		auto itr2 = itr.second.find(name);
		if (itr2 != itr.second.end())
		{
			output.push_back(itr2->second.get());
		}
	}
	return output;
}
std::vector<signal_base*> signal_manager::get_signals(Loki::TypeInfo type)
{
	std::vector<signal_base*> output;
	for (auto& itr : _signals[type])
	{
		output.push_back(itr.second.get());
	}
	return output;
}
std::vector<signal_base*> signal_manager::get_signals(Loki::TypeInfo type, std::string name)
{
	std::vector<signal_base*> output;
	for (auto& itr : _signals)
	{
		for (auto& itr2 : itr.second)
		{
			output.push_back(itr2.second.get());
		}
	}
	return output;
}
std::vector<signal_base*> signal_manager::get_signals()
{
	std::vector<signal_base*> output;

	return output;
}

void signal_manager::print_signal_map()
{
	for (auto& type : _signals)
	{
		for (auto& sig : type.second)
		{
			std::cout << "[" << sig.first << "] <" << type.first.name() << ">\n";
			std::cout << " Senders:\n";
			//auto cleaned_type = _signal_signature_map.find(type.first);
			
				//std::cout << "   " << cleaned_type->second.name();
			auto signature = sig.second->get_signal_type();
			auto itr = _registered_sender_objects[signature][sig.first];
			for (auto sender : itr)
			{
				std::cout << "    " << std::get<0>(sender).name() << " [" << std::get<1>(sender) << "] " << std::get<2>(sender) << "\n";
			}
			auto itr2 = _registered_senders[signature][sig.first];
			for (auto sender : itr2)
			{
				std::cout << "    " << std::get<1>(sender) << " [" << std::get<0>(sender) << "] " << std::get<2>(sender) << "\n";
			}
			std::cout << " Receivers:\n";
			auto itr3 = _registered_receivers[signature][sig.first];
			for (auto receiver : itr3)
			{
				std::cout << "    " << std::get<1>(receiver) << " [" << std::get<0>(receiver) << "] " << std::get<2>(receiver) << "\n";
			}
			auto itr4 = _registered_receiver_objects[signature][sig.first];
			for (auto receiver : itr4)
			{
				std::cout << "    " << std::get<0>(receiver).name() << " [" << std::get<1>(receiver) << "] " << std::get<2>(receiver) << "\n";
			}
			
			
		}

	}
}
/*
void signal_registery::register_sender(const std::string& signal_name, Loki::TypeInfo type, void* sender, Loki::TypeInfo signal_signature, signal_manager* mgr, const std::string& desc)
{
	BOOST_LOG(debug) << "Registering signal emitter " << type.name() << " of signal " << signal_name << " with signature " << signal_signature.name();
	mgr->register_sender(signal_signature, signal_name, type, sender);
}
void signal_registery::register_receiver(const std::string& signal_name, Loki::TypeInfo type, void* receiver, Loki::TypeInfo signal_signature, signal_manager* mgr, const std::string& desc)
{
	BOOST_LOG(debug) << "Registering signal receiver " << type.name() << " of signal " << signal_name << " with signature " << signal_signature.name();
	mgr->register_receiver(signal_signature, signal_name, type, receiver);
}
*/