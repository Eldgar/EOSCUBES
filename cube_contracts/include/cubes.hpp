#pragma once
#include <eosiolib/contracts/eosio/action.hpp>
#include <eosiolib/contracts/eosio/eosio.hpp>
#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/symbol.hpp>
#include <string>
#include <eosio/crypto.hpp>
#include <eosio/transaction.hpp>
#include <string_view>

// Generic eosio library, i.e. print, type, math, etc
using namespace eosio;

namespace tte {

// IMPORTANT: Must be the same as the --filter-name parameter's value of rodeos
static constexpr auto contract_account = "eldgarcubes5"_n;

class[[eosio::contract("eldgarcubes5")]] eldgarcubes5 : public contract
{
public:
    using contract::contract;
    eldgarcubes5(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds) {}

    

    // cubes with texture and x,y,z co-ordinates
    struct [[eosio::table]] cube
    {
        uint16_t                id = 0;
        name                    username;
        std::string             key;
        std::vector<int32_t>    pos;
        std::string             texture;

        auto primary_key() const { return id; }
        //EOSLIB_SERIALIZE( cube, (id)(username)(key)(pos)(texture))
    };
    typedef eosio::multi_index<"cubes"_n, cube>     cubes;


    // cubes price for specified position value
    struct [[eosio::table]] cubeprice
    {
        uint16_t               id = 0;
        int32_t                x;
        int32_t                y;
        int32_t                z;
        uint32_t                price;
        std::vector<int32_t>    pos;
        
        uint16_t primary_key() const { return id; }
        //EOSLIB_SERIALIZE( cube, (id)(username)(key)(pos)(texture))
    };
    typedef eosio::multi_index<"cubeprices"_n, cubeprice>     cubeprices;

    struct [[eosio::table]] balances 
    {
		uint64_t            user;
        name                username;
		eosio::asset        balance;
		uint64_t primary_key() const { return user; }

	};

	using bal_table = eosio::multi_index<"balances"_n, balances
	>;

    [[eosio::on_notify("eosio.token::transfer")]] 
	void wegotpaid(name from, name to, eosio::asset quantity, std::string memo);

    void fee(name owner, eosio::asset quantity);

    uint16_t create_price(std::vector<int32_t>  pos);

    void set_prices(std::vector<int32_t> pos);

    auto get_prices(std::vector<int32_t> pos);

    [[eosio::action]]
    void addcube(const name username, const std::string &key, const std::vector<int32_t> &pos, const std::string &texture);

    [[eosio::action]]
    void removecube(uint16_t id, name username);

protected:
    
    
    uint16_t addcube_impl(
                        const name                    username, 
                        const std::string &           key,
                        const std::vector<int32_t> &  pos,
                        const std::string &           texture,
                        bool                          invoke_return_value);
};

//EOSIO_DISPATCH(eldgarcubes4,(wegotpaid)(addcube)(removecube))

} // namespace tte
