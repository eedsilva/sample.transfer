#include "../include/sample.transfer.hpp"

using namespace sample;

transfer::transfer(name s, name code, datastream<const char *> ds) : contract(s, code, ds){ }
transfer::~transfer() { }

void transfer::go(uint16_t r[]) {

}

void transfer::transfer_handler(name from, name to, asset quantity) {
	sample::transfer::users_table ut(_self, _self.value);
	print("\nDeposit Complete");
}

extern "C" {
    void apply(uint64_t self, uint64_t code, uint64_t action) {

        size_t size = action_data_size();
        constexpr size_t max_stack_buffer_size = 512;
        void* buffer = nullptr;
        if( size > 0 ) {
            buffer = max_stack_buffer_size < size ? malloc(size) : alloca(size);
            read_action_data(buffer, size);
        }
        datastream<const char*> ds((char*)buffer, size);
        
        if (code == name("eosio.token").value && action == name("transfer").value) {
            sample::transfer t(name(self), name(code), ds);
            auto args = unpack_action_data<sample::transfer::transfer_args>();
            t.transfer_handler(args.from, args.to, args.quantity);
        }
    } //end apply
}; //end dispatcher
