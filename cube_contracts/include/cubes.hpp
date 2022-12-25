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
static constexpr auto contract_account = "eldgarcube12"_n;

class[[eosio::contract("eldgarcube12")]] eldgarcube12 : public contract
{
public:
    using contract::contract;
    eldgarcube12(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds) {}

    

    // cubes with texture and x,y,z co-ordinates
    struct [[eosio::table]] cube
    {
        uint64_t               id = 0;
        name                    username;
        std::string             key;
        std::vector<int32_t>    pos;
        std::string             texture;

        auto primary_key() const { return id; }
        
    };
    typedef eosio::multi_index<"cubes"_n, cube>     cubes;


    // cubes price for specified position value
    struct [[eosio::table]] cubeprice
    {
        uint64_t               id = 0;
        uint32_t                price;
        
        uint16_t primary_key() const { return id; }
        
    };
    typedef eosio::multi_index<"cubeprices"_n, cubeprice>     cubeprices;

    struct [[eosio::table]] balances 
    {
		uint64_t            user;
        name                username;
		eosio::asset        balance;
		uint64_t primary_key() const { return user; }

	};

	using bal_table = eosio::multi_index<"balances"_n, balances>;

    struct [[eosio::table]] assets {
		uint64_t asset_id;
		name collection_name;
		name schema_name;
		int32_t template_id;
		name ram_payer;
		std::vector<asset> backed_tokens;
		std::vector<uint8_t> immutable_serialized_data;
		std::vector<uint8_t> mutable_serialized_data;
		uint64_t primary_key() const { return asset_id; }
	};

	using atomic_assets = eosio::multi_index<"assets"_n, assets>;
    //eosio::indexed_by<"templateid"_n, eosio::const_mem_fun<assets, uint64_t, &assets::second_key>>>;
    [[eosio::on_notify("eosio.token::transfer")]] 
	void wegotpaid(name from, name to, eosio::asset quantity, std::string memo);

    void fee(name owner, name reciever, eosio::asset quantity);

    symbol get_tokenSymbol();

    uint64_t create_price(std::vector<int32_t>  pos);

    void set_prices(std::vector<int32_t> pos);

    bool nft_search(int32_t template_id,  name contract, name scope);

    uint64_t pos_conversion(std::vector<int32_t> pos);

    auto get_prices(std::vector<int32_t> pos);

    [[eosio::action]]
    void withdraw(name username, eosio::asset quantity);

    [[eosio::action]]
    void addcube(const name username, const std::string &key, const std::vector<int32_t> &pos, const std::string &texture);

    [[eosio::action]]
    void removecube(uint64_t id, name username);

protected:
    
    
    uint64_t addcube_impl(
                        const name                    username, 
                        const std::string &           key,
                        const std::vector<int32_t> &  pos,
                        const std::string &           texture,
                        bool                          invoke_return_value);
};

//EOSIO_DISPATCH(eldgarcube11,(wegotpaid)(addcube)(removecube))

} // namespace tte
