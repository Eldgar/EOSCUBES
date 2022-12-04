#include "../include/cubes.hpp"



// Generic eosio library, i.e. print, type, math, etc
using namespace eosio;

namespace tte {
[[eosio::on_notify("eosio.token::transfer")]] 
void eldgarcubes5::wegotpaid(name from, name to, eosio::asset quantity, std::string memo) {
        const eosio::name REAL_CONTRACT = "eosio.token"_n;

		const eosio::name ORIGINAL_CONTRACT = get_first_receiver();

		const std::string_view waxString{"EOS"};

		const uint8_t waxdecimals = 4;

		const symbol waxsymbol(
			waxString,
			waxdecimals
		);

		const std::string_view waxdaoString{"EOS"};

		const uint8_t waxdaodecimals = 4;

		const symbol waxdaosymbol(

			waxdaoString,

			waxdaodecimals

		);

		check( quantity.amount >= 10, "Quanity must be greater than 0.1 EOS" );

		check( REAL_CONTRACT == ORIGINAL_CONTRACT, "You tryna get over on us, bro?" );

		if( from == get_self() || to != get_self() ){ return; }

		check( quantity.symbol == waxsymbol, "Symbol is not what we were expecting" );

		//emplace info into table

		bal_table bals( get_self(), get_self().value );

		auto itr = bals.find( from.value );

		if( itr != bals.end() ){

			//modify this user's entry

			bals.modify( itr, same_payer, [&](auto &row) {

				row.balance += quantity;

			});

		} else {

			bals.emplace( get_self(), [&](auto &row) {

				row.user = from.value;
                row.username = from;
				row.balance = quantity;

			});

		}
																			//name of account/contract with tokens
		action(permission_level{eosio::name("eldgarcubes5"), 
        "active"_n}, "eosio.token"_n,"transfer"_n,
        std::tuple{ eosio::name("eldgarcubes5"), from, asset(10, waxdaosymbol), std::string("We just reacted son")}).send();

	}//end of on_notify

void eldgarcubes5::addcube(
        const name                      username, 
        const std::string &             key,
        const std::vector<int32_t> &    pos,
        const std::string &             texture
 )
{
    addcube_impl(username, key, pos, texture, false);
}


void eldgarcubes5::removecube(uint16_t id, name username)
{
    require_auth(username);
    //where does existing cubes come from?
    cubes existing_cubes(get_self(), contract_account.value);

    // Check if cube exists
    auto itr = existing_cubes.find(id);
    check(itr != existing_cubes.end(), "Unable to find an order with specified ID");

    // proceed with order modification
   //existing_cubes.modify(itr, contract_account, [&](auto &g) {
    //     g.id = id;
    //     g.key=key;
    //     g.pos=pos;
    //     g.texture=texture;
   // });
    existing_cubes.erase( itr );
}


// fee to be paid for adding a block
void eldgarcubes5::fee(name owner, asset amount){

    const std::string_view waxString{"EOS"};

		const uint8_t waxdecimals = 4;

		const symbol waxsymbol(
			waxString,
			waxdecimals
		);

    bal_table bals( get_self(), get_self().value );

		auto itr = bals.find( owner.value );

		if( itr != bals.end() ){

			//modify this user's entry
			bals.modify( itr, same_payer, [&](auto &row) {
                check(amount <= row.balance, "Exceeding User Balance");
				row.balance -= amount;

			});

		} else {

			check(false, "No Balance.");
		}
}


//get price of block location default 0.01 EOS
auto eldgarcubes5::get_prices(std::vector<int32_t>  pos) {
    cubeprices find_price( get_self(), contract_account.value );
        int32_t x = pos[0];
        int32_t y = pos[1];
        int32_t z = pos[2];
		auto itr = find_price.find( x && y && z);

		if( itr != find_price.end() ){

			//return the price for this cube postion
            return itr->price;
			

		} else {
            //cube_prices new_price{x, y ,z, price, pos};
            //price.emplace(contract_account, [&](auto &g) { g = new_price; });
			check(false, "price for position doesn't exist");
		}
    
};


uint16_t eldgarcubes5::create_price(std::vector<int32_t> pos){
        cubeprices set_price(get_self(), contract_account.value );
    
        int32_t x = pos[0];
        int32_t y = pos[1];
        int32_t z = pos[2];
        uint32_t price = 10;
        uint16_t id=set_price.available_primary_key();
      
        cubeprice new_price{id, x, y ,z, price, pos};
        set_price.emplace(contract_account, [&](auto &row) { row = new_price; });
        return id;
}

void eldgarcubes5::set_prices(std::vector<int32_t> pos)
{
    cubeprices set_price(get_self(), contract_account.value );
    
        int32_t x = pos[0];
        int32_t y = pos[1];
        int32_t z = pos[2];
		auto itr = set_price.find( x && y && z);
		if( itr != set_price.end() ){

			//modify this user's entry
            set_price.modify(itr, contract_account, [&](auto &row) {
            // increase current position price
            row.price = row.price * 2;
            });

		} else {
            create_price(pos);
		}
};


uint16_t eldgarcubes5::addcube_impl( 
                                const name                      username,
                                const std::string &             key,
                                const std::vector<int32_t> &    pos,
                                const std::string &             texture,
                                bool                            invoke_return_value)
   {
      require_auth(username);
      const std::string_view waxString{"EOS"};

		const uint8_t waxdecimals = 4;

		const symbol waxsymbol(
			waxString,
			waxdecimals

		);
      set_prices(pos);
      asset quantity = asset(get_prices(pos), waxsymbol);
      fee(username, quantity);
      cubes existing_cubes(get_self(), contract_account.value); 
      // proceed with cube creation
      uint16_t id=existing_cubes.available_primary_key();
      cube new_cube{id, username, key, pos, texture};
      existing_cubes.emplace(contract_account, [&](auto &row) { row = new_cube; });
      return id;
   }
}