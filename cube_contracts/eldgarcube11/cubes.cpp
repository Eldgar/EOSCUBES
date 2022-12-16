#include "../include/cubes.hpp"



// Generic eosio library, i.e. print, type, math, etc
using namespace eosio;

namespace tte {
symbol eldgarcube11::get_eosTokenSymbol(){
    const std::string_view eosString{"EOS"};
    const uint8_t eosdecimals = 4;
    const symbol eosSymbol(
			eosString,
            eosdecimals
	);

    return eosSymbol;
}


[[eosio::on_notify("eosio.token::transfer")]] 
void eldgarcube11::wegotpaid(name from, name to, eosio::asset quantity, std::string memo) {
        const eosio::name REAL_CONTRACT = "eosio.token"_n;
		const eosio::name ORIGINAL_CONTRACT = get_first_receiver();

		const std::string_view waxdaoString{"EOS"};
		const uint8_t waxdaodecimals = 4;
		const symbol waxdaosymbol(

			waxdaoString,
			waxdaodecimals

		);

		check( quantity.amount >= 100, "Quanity must be greater than 0.1 EOS" );
		check( REAL_CONTRACT == ORIGINAL_CONTRACT, "You tryna get over on us, bro?" );

		if( from == get_self() || to != get_self() )
        { return; }


		check( quantity.symbol == get_eosTokenSymbol(), "Symbol is not what we were expecting" );

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
		action(permission_level{eosio::name("eldgarcube11"), 
        "active"_n}, "eosio.token"_n,"transfer"_n,
        std::tuple{ eosio::name("eldgarcube11"), from, asset(10, waxdaosymbol), std::string("We just reacted son")}).send();

	}//end of on_notify

void eldgarcube11::addcube(
        const name                      username, 
        const std::string &             key,
        const std::vector<int32_t> &    pos,
        const std::string &             texture
 )
{
    addcube_impl(username, key, pos, texture, false);
}

//get price of block location
auto eldgarcube11::get_prices(std::vector<int32_t>  pos) {
    cubeprices find_price( get_self(), contract_account.value );
        uint64_t id = pos_conversion(pos);
		auto itr = find_price.find(id);

		if( itr != find_price.end() ){

			//return the price for this cube postion
            return itr->price;
			

		} else {
            //cube_prices new_price{x, y ,z, price, pos};
            //price.emplace(contract_account, [&](auto &g) { g = new_price; });
			check(false, "price for position doesn't exist");
		}
    check(false, "this shouldn't happen");
};


//Allow users to withdraw their balance
void eldgarcube11::withdraw(name username, asset quantity){
    require_auth(username);
    const eosio::name REAL_CONTRACT = "eosio.token"_n;
	check( quantity.amount >= 1000, "Quanity must be greater than 0.1 EOS" );
	check( quantity.symbol == get_eosTokenSymbol(), "Symbol is not what we were expecting" );

	//emplace info into table
	bal_table bals( get_self(), get_self().value );
	auto itr = bals.find( username.value );

		if( itr != bals.end() ){
			//modify this user's entry
			bals.modify( itr, same_payer, [&](auto &row) {
				row.balance -= quantity;
			});

		} else {
                check(false, "This account doesn't exist");
		}

		action(permission_level{eosio::name("eldgarcube11"), 
        "active"_n}, "eosio.token"_n,"transfer"_n,
        std::tuple{ eosio::name("eldgarcube11"), username, quantity, std::string("You have made a withdrawl from Eldgar Cubes")}).send();
};

void eldgarcube11::removecube(uint64_t id, name username)
{
    require_auth(username);
    //where does existing cubes come from?
    cubes existing_cubes(get_self(), contract_account.value);

    // Check if cube exists
    auto itr = existing_cubes.find(id);
    auto pos = itr->pos;
    auto cube_owner = itr->username;
    asset quantity = asset(get_prices(pos), get_eosTokenSymbol());
    fee(username, cube_owner, quantity);
    check(itr != existing_cubes.end(), "Unable to find an order with specified ID");

    //erase cube
    existing_cubes.erase( itr );
}


// fee to be paid for adding a block
void eldgarcube11::fee(name owner, name reciever, eosio::asset amount){
    //get balances table
    bal_table bals( get_self(), get_self().value );
		auto itr = bals.find( owner.value );
        auto itrx = bals.find(reciever.value);
		if( itr != bals.end() ){          
			//modify this user's entry
			bals.modify( itr, same_payer, [&](auto &row) {
                check(amount < row.balance, "Exceeding User Balance");
				row.balance -= amount;
			});

		} else {
			check(false, "No Balance.");
		}
        if( itrx != bals.end() ){
			//modify reciever's balance
			bals.modify( itrx, same_payer, [&](auto &row) {
				row.balance += amount;

			});

		} else {

			bals.emplace( get_self(), [&](auto &row) {

				row.user = reciever.value;
                row.username = reciever;
				row.balance = amount;
            });
		}
};




uint64_t eldgarcube11::pos_conversion(std::vector<int32_t> pos){
        int32_t x = pos[0];
        int32_t y = pos[1];
        int32_t z = pos[2];
        check((x < 300) && (y < 300) && (z < 300), "position out of limits");
        check((x > -300) && (y > -300) && (z > -300), "position out of limits");
        // create variables so position can be converted into uint64_t and used in iterator .find()
        uint64_t xval = 200000000000 + (x * 100000000);
        uint64_t yval = 20000000 + (y * 10000);
        uint64_t zval = 2000 + (z);
        uint32_t price = 10;
        uint16_t id = xval + yval + zval;

        return id;
}


uint64_t eldgarcube11::create_price(std::vector<int32_t> pos){
        cubeprices set_price(get_self(), contract_account.value );

        uint64_t id = pos_conversion(pos);
        uint32_t price = 10;
        cubeprice new_price{id, price};
        set_price.emplace(contract_account, [&](auto &row) { row = new_price; });
        return id;
}

void eldgarcube11::set_prices(std::vector<int32_t> pos)
{
    cubeprices set_price(get_self(), contract_account.value );
        uint64_t id = pos_conversion(pos);
		auto itr = set_price.find(id);
        
		if( itr != set_price.end() ){
			//modify the price for cube co-ordinates
            set_price.modify(itr, contract_account, [&](auto &row) {
            // increase current position price
            row.price = row.price * 2;
            });

		} else {
            create_price(pos);
		}
};


// uses a search of Ids on the 
bool eldgarcube11::nft_search(uint64_t lowerId, uint64_t higherId, name contract, name scope, name username)
{
    assets nfts_offers(contract, scope.value);
    //                  ^contract    ^scope
    for (uint32_t i = 0; i < (higherId - lowerId); i++)
    {
        uint64_t nft_id = lowerId + i;
        auto itr_nft = nfts_offers.find(nft_id);
        if (itr_nft != nfts_offers.end())
        {
            return true;
        }
    }
    return false;
};

uint64_t eldgarcube11::addcube_impl( 
                                const name                      username,
                                const std::string &             key,
                                const std::vector<int32_t> &    pos,
                                const std::string &             texture,
                                bool                            invoke_return_value)
   {
        check(
            (texture == "dirt") ||
            (texture == "grass") ||
            (texture == "glass") ||
            (texture == "wood") ||
            (texture == "log") ||
            (texture == "saphire") ||
            (texture == "ruby") ||
            (texture == "portal"), "Not a valid texutre");

        require_auth(username);
        if ((texture == "saphire") && !nft_search(100000000051693, 100000000051793, "simpleassets"_n, username, username))
        {
            check(false, "You do not have the required Saphire NFT");
        };
        name reciever = contract_account;
        set_prices(pos);
        asset quantity = asset(get_prices(pos), get_eosTokenSymbol());
        fee(username, reciever, quantity);
        cubes existing_cubes(get_self(), contract_account.value); 
        // proceed with cube creation
        uint64_t id = pos_conversion(pos);
	    auto itr = existing_cubes.find(id);
        check(itr == existing_cubes.end(), "A cube already exists in that position, remove cube before adding");
        cube new_cube{id, username, key, pos, texture};
        existing_cubes.emplace(contract_account, [&](auto &row) { row = new_cube; });
        return id;
   }
}