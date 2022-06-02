#include <mbsystem.hxx>

namespace mbsystem = Moneybeast::System;

bool
mbsystem::try_login (std::string &&login, std::string &&password,
           Moneybeast::AccessToken &access_token)
{
    return true;
}

void
mbsystem::update_personal_info (Moneybeast::Client &client)
{

}

void
mbsystem::update_cards_info (Moneybeast::Client &client)
{

}

void
mbsystem::update_accounts_info (Moneybeast::Client &client)
{

}

void
mbsystem::get_castaff (Moneybeast::AccessToken &&access_token,
                       std::vector<Moneybeast::CaStaff> &list)
{

}

void
mbsystem::get_cdstaff (Moneybeast::AccessToken &&access_token,
                       std::vector<Moneybeast::CDStaff> &list)
{

}
