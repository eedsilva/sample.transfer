#include "../include/sample.transfer.hpp"

using namespace sample;

transfer::transfer(name s, name code, datastream<const char *> ds) : contract(s, code, ds){ }
transfer::~transfer() { }

void transfer::go(name user, asset money) {
    require_auth(user);
    
    users_table users(_self, _self.value);
    auto ut = users.find(user.value);
    
    eosio_assert(ut != users.end(), "user doesn't exist");
    eosio_assert(ut->amount_tlos.amount != 0, "you have 0 TLOS in your account.");
    eosio_assert(ut->amount_tlos >= money , "you don't have enough money to play.");

    users.modify(ut, same_payer, [&](auto &u) {
        u.amount_tlos -= money;
    });

    print("\nMoney updated.");
}

void transfer::transfer_handler(name from, name to, asset quantity) {
	require_auth(from);
    
    eosio_assert(to == get_self(), "to cannot be self.");
    eosio_assert(quantity.symbol == symbol("TLOS", 4), "you should send TLOS only.");
	
    users_table users(_self, _self.value);
	auto user = users.find(from.value);
    
    if(user == users.end()) {
        users.emplace(get_self(), [&](auto &u) {
            u.user_name = from;
            u.amount_tlos = quantity;
        });
    } else {
        users.modify(user, same_payer, [&](auto &u) {
            u.amount_tlos += quantity;
        });
    }

    print("\nDeposit Complete");
}

extern "C" {
    void apply(uint64_t self, uint64_t code, uint64_t action) {
        if(code == self && action == "go"_n.value) {
            execute_action(name(self), name(code), &sample::transfer::go);
        } else if (code == name("eosio.token").value && action == name("transfer").value) {
            size_t size = action_data_size();
            constexpr size_t max_stack_buffer_size = 512;
            void* buffer = nullptr;
            
            if( size > 0 ) {
                buffer = max_stack_buffer_size < size ? malloc(size) : alloca(size);
                read_action_data(buffer, size);
            }
            
            datastream<const char*> ds((char*)buffer, size);
            
            sample::transfer t(name(self), name(code), ds);
            auto args = unpack_action_data<sample::transfer::transfer_args>();
            t.transfer_handler(args.from, args.to, args.quantity);
        }
    } //end apply
}; //end dispatcher
