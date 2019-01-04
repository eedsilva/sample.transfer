#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>

using namespace eosio;
using namespace std;

namespace sample {
    class[[eosio::contract("sample.transfer")]] transfer : public eosio::contract {

    public:
        transfer(name s, name code, datastream<const char *> ds);
        ~transfer();

        [[eosio::action]]
        void go(name user, asset money);

        struct transfer_args {
          name from;
          name to;
          asset quantity;
          string memo;
        };
        
        struct[[ eosio::table("user"), eosio::contract("sample.transfer") ]] user {
            name user_name;
            asset amount_tlos;

            uint64_t primary_key() const { return user_name.value; }
            EOSLIB_SERIALIZE(user, (user_name)(amount_tlos))
        };
        typedef eosio::multi_index<"users"_n, user> users_table;
    
    // private:
       void transfer_handler(name from, name to, asset quantity);
    
    };
}