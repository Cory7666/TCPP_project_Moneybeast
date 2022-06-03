#include <mbsystem.hxx>

namespace mbsystem = Moneybeast::System;

bool
mbsystem::try_login (const std::string &login, const std::string &password,
           Moneybeast::AccessToken &access_token)
{
    if (login == "dmitry" && password == "123")
    {
        access_token = AccessToken{"123"};
        return true;
    }
    else
        return false;
}

bool
mbsystem::update_personal_info (Moneybeast::Client &client)
{
    if (client.getAccessToken().isSet())
    {
        client.setFirstName ("Дмитрий");
        client.setLastName  ("Иванов");
        client.setPatronymic ("Дмитриевич");
        client.setEmail     ("dmitry@example.com");
        client.setPassportNum (Passport{"1111987634"});

        return true;
    }

    return false;
}

bool
mbsystem::update_cards_info (Moneybeast::Client &client)
{
    if (client.getAccessToken().isSet())
    {
        client.addCard(new Card{CardNum{"1234123412341234"}, client.getAccountById(1)});
        return true;
    }

    return false;
}

bool
mbsystem::update_accounts_info (Moneybeast::Client &client)
{
    if (client.getAccessToken().isSet())
    {
        client.addAccount (new Account{1, 123, 2});
        client.addAccount (new Account{2, 555, 6});
        return true;
    }

    return false;
}

bool
mbsystem::get_castaff (const Moneybeast::AccessToken &access_token,
                       std::vector<Moneybeast::CaStaff> &list)
{
    if (!access_token.isSet())
        return false;

    list.clear();
    list.insert (list.end(),
        CaStaff{
            Passport{"1234567890"},
            "Василий",
            "Обнинский",
            "vasya@moneybeast.ru"
        }
    );

    return true;
}

bool
mbsystem::get_cdstaff (const Moneybeast::AccessToken &access_token,
                       std::vector<Moneybeast::CDStaff> &list)
{
    if (!access_token.isSet())
        return false;

    list.clear();
    list.insert (list.end(),
        CDStaff{
            Passport{"1234567891"},
            "Артур",
            "Торжков",
            "artur@moneybeast.ru"
        }
    );

    return true;
}
