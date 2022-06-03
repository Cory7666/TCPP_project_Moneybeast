#include <mbsystem.hxx>

namespace mbsystem = Moneybeast::System;

bool
mbsystem::try_login (std::string &&login, std::string &&password,
           Moneybeast::AccessToken &access_token)
{
    if (login != "alex" && password != "123")
        return false;
    
    access_token = AccessToken{"123"};
    return true;
}

void
mbsystem::update_personal_info (Moneybeast::Client &client)
{
    if (AccessToken{"123"} == client.getAccessToken())
    {
        client = Client{
            Passport{"1111111111"},
            "Лёха",
            "Русый",
            "alex@moneybeast.ru",
            "Дмитриевич"
        };
    }
}

void
mbsystem::update_cards_info (Moneybeast::Client &client)
{
    if (AccessToken{"123"} == client.getAccessToken())
    {
        client.addCard(new Card{CardNum{"1234123412341234"}, client.getAccountById(1)});
    }
}

void
mbsystem::update_accounts_info (Moneybeast::Client &client)
{
    if (AccessToken{"123"} == client.getAccessToken())
    {
        client.addAccount (new Account{1, 123, 2});
    }
}

void
mbsystem::get_castaff (Moneybeast::AccessToken &&access_token,
                       std::vector<Moneybeast::CaStaff> &list)
{
    if (access_token == AccessToken{"0000"})
        return;

    list.clear();
    list.insert (list.end(),
        CaStaff{
            Passport{"1234567890"},
            "Василий",
            "Обнинский",
            "vasya@moneybeast.ru"
        }
    );
}

void
mbsystem::get_cdstaff (Moneybeast::AccessToken &&access_token,
                       std::vector<Moneybeast::CDStaff> &list)
{
    if (access_token == AccessToken{"0000"})
        return;

    list.clear();
    list.insert (list.end(),
        CDStaff{
            Passport{"1234567891"},
            "Артур",
            "Торжков",
            "artur@moneybeast.ru"
        }
    );
}
