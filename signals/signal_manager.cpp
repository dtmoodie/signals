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

signal_base* signal_manager::get_signal_optional(const std::string& name, const Loki::TypeInfo& type)
{
    auto itr = _signals.find(type);
    if(itr != _signals.end())
    {
        auto itr2 = itr->second.find(name);
        if(itr2 != itr->second.end())
        {
            return itr2->second.get();
        }
    }
    return nullptr;
}

signal_base* signal_manager::get_signal_optional(const std::string& name, const std::string& type)
{
    for(auto& signature : _signals)
    {
        std::string type_name = signature.first.name();
        if(type == signature.first.name())
        {
            auto itr2 = signature.second.find(name);
            if(itr2 != signature.second.end())
            {
                return itr2->second.get();
            }
        }
    }
    return nullptr;
}

void signal_manager::register_sender(Loki::TypeInfo signal_signature, std::string signal_name, signaler* sender_ptr)
{
	register_sender(signal_signature, signal_name, Loki::TypeInfo(typeid(*sender_ptr)), sender_ptr, sender_ptr->get_description(), sender_ptr->get_signal_description(signal_name));
}
void signal_manager::register_sender(Loki::TypeInfo signal_signature, std::string signal_name, Loki::TypeInfo sender_type, void* sender_ptr, std::string desc, std::string tooltip)
{
	sender s;
    s.description = desc;
    s.type = sender_type;
    s.ptr = sender_ptr;
    s.signature = signal_signature;
    s.signal_name = signal_name;
    s.tooltip = tooltip;
    _registered_senders[signal_signature][signal_name].push_back(s);
}
void signal_manager::register_sender(Loki::TypeInfo signal_signature, std::string signal_name, std::string desc, std::string file_name, int line_number, std::string tooltip)
{
    sender s;
    s.description = desc;
    s.file = file_name;
    s.line = line_number;
    s.signature = signal_signature;
    s.signal_name = signal_name;
    s.tooltip = tooltip;
    _registered_senders[signal_signature][signal_name].push_back(s);
}
void signal_manager::remove_sender(void* sender_ptr, std::string name)
{

}
void signal_manager::remove_sender(std::string file_name, int line_number, std::string name)
{

}

void signal_manager::register_receiver(Loki::TypeInfo signal_signature, const std::string& signal_name, Loki::TypeInfo receiver_type, void* receiver_ptr, const std::string& desc, const std::string& tooltip)
{
	receiver r;
    r.description = desc;
    r.tooltip = tooltip;
    r.ptr = receiver_ptr;
    r.type = receiver_type;
    r.signature = signal_signature;
    r.signal_name = signal_name;
    _registered_receivers[signal_signature][signal_name].push_back(r);
}

void signal_manager::register_receiver(Loki::TypeInfo signal_signature, const std::string& signal_name, int line_number, const std::string& file_name, const std::string& desc, const std::string& tooltip)
{
	receiver r;
    r.description = desc;
    r.tooltip = tooltip;
    r.line = line_number;
    r.file = file_name;
    r.signature = signal_signature;
    r.signal_name = signal_name;
    _registered_receivers[signal_signature][signal_name].push_back(r);
}
void signal_manager::register_receiver(Loki::TypeInfo signal_signature, const std::string& signal_name, signaler* receiver)
{
    register_receiver(signal_signature, signal_name, Loki::TypeInfo(typeid(*receiver)), receiver, receiver->get_description(), receiver->get_slot_description(signal_name));
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
std::vector<std::string> signal_manager::get_signal_names()
{
    std::set<std::string> names;
    for(auto& sig : _signals)
    {
        for(auto& sig1 : sig.second)
        {
            names.insert(sig1.first);
        }
    }
    std::vector<std::string> output;
    output.insert(output.end(), names.begin(), names.end());
    return output;
}
void signal_manager::print_signal_map()
{
	/*for (auto& type : _signals)
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

	}*/
}
std::vector<receiver> signal_manager::get_receivers(Loki::TypeInfo type, std::string name)
{
    return _registered_receivers[type][name];
}

std::vector<receiver> signal_manager::get_receivers(Loki::TypeInfo type)
{
    std::vector<receiver> output;
    auto itr = _registered_receivers.find(type);
    if(itr != _registered_receivers.end())
    {
        for(auto& r : itr->second)
        {
            output.insert(output.end(), r.second.begin(), r.second.end());
        }
    }
    return output;
}

std::vector<receiver> signal_manager::get_receivers(std::string name)
{
    std::vector<receiver> output;
    for(auto& sig_itr : _registered_receivers)
    {
        auto name_itr = sig_itr.second.find(name);
        if(name_itr != sig_itr.second.end())
        {
            output.insert(output.end(), name_itr->second.begin(), name_itr->second.end());
        }
    }
    return output;
}
std::vector<receiver> signal_manager::get_receivers()
{
    std::vector<receiver> output;
    for(auto& sig : _registered_receivers)
    {
        for(auto& name: sig.second)
        {
            output.insert(output.end(), name.second.begin(), name.second.end());
        }
    }
    return output;
}
std::vector<sender> signal_manager::get_senders(Loki::TypeInfo type, std::string name)
{
    return _registered_senders[type][name];
}

std::vector<sender> signal_manager::get_senders(Loki::TypeInfo type)
{
    std::vector<sender> output;
    auto itr = _registered_senders.find(type);
    if(itr != _registered_senders.end())
    {
        for(auto& r : itr->second)
        {
            output.insert(output.end(), r.second.begin(), r.second.end());
        }
    }
    return output;
}

std::vector<sender> signal_manager::get_senders(std::string name)
{
    std::vector<sender> output;
    for(auto& sig_itr : _registered_senders)
    {
        auto name_itr = sig_itr.second.find(name);
        if(name_itr != sig_itr.second.end())
        {
            output.insert(output.end(), name_itr->second.begin(), name_itr->second.end());
        }
    }
    return output;
}
std::vector<sender> signal_manager::get_senders()
{
        std::vector<sender> output;
    for(auto& sig : _registered_senders)
    {
        for(auto& name: sig.second)
        {
            output.insert(output.end(), name.second.begin(), name.second.end());
        }
    }
    return output;
}
void Signals::register_sender(signaler* sender, const std::string& signal_name, Loki::TypeInfo signal_signature, signal_manager* mgr)
{
    mgr->register_sender(signal_signature, signal_name, sender);
}
void Signals::register_receiver(signaler* receiver, const std::string& signal_name, Loki::TypeInfo signal_signature, signal_manager* mgr)
{
    mgr->register_receiver(signal_signature, signal_name, receiver);
}
