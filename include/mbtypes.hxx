#include <iostream>
#include <sstream>
#include <map>

#ifndef MBTYPES_HXX
#define MBTYPES_HXX

namespace Moneybeast
{
    class Account;
    class Card;
    class CaStaff;
    class CDStaff;
    class Client;
    class Person;
    struct CardNum;
    struct Money;

    class Person
    {
        public:
            Person (
                char passport_num[10],
                std::string first_name,
                std::string last_name,
                std::string patronymic,
                std::string login
            );

            void getPassportNum (char* out);
            std::string getFirstName();
            std::string getLastName();
            std::string getPatronymic();
            std::string getLogin();
            std::string getAccessKey();

        protected:
            char passport_num[10];
            std::string
                first_name_,
                last_name_,
                patronymic_;
            
            std::string
                login_,
                key_;
    };

    class Client final : public Person
    {
        public:
            Client (
                char passport_num[10],
                std::string first_name,
                std::string last_name,
                std::string patronymic,
                std::string login
            );

            void addCard (Card* card);
            void addAccount (Account* account);
            void removeCard (char *card_num);
            void removeAccount (long account_id);

            Card* getCardByNumber (CardNum& card_num);
            Account* getAccountById (Account& account_id);
        private:
            std::map<long, Account> accounts_;
            std::map<CardNum, Card> cards_; 
    };

    class CaStaff final : public Person
    {
        public:
            CaStaff (
                char passport_num[10],
                std::string first_name,
                std::string last_name,
                std::string patronymic,
                std::string login
            );

            Card* openUnnamedCard (const Client* client, Account* account);
    };

    class CDStaff final : public Person
    {
        public:
            CDStaff (
                char passport_num[10],
                std::string first_name,
                std::string last_name,
                std::string patronymic,
                std::string login
            );
            
            Account* openUnnamedCredit (
                const Client* client,
                long money_amount, long interest
            );
            Account* openUnnamedDeposit (
                const Client* client,
                long money_amount, long interest
            );
    };



    class Account final
    {
        public:
            Account (Money& money_amount, long interest);
        private:
            Money money_amount_;
            long interest_;
    };

    class Card final
    {
        public:
            Card (CardNum& card_num, Account& account);

            void getCardNum (char* out);
            long getLinkedAccountId();
        private:
            CardNum card_num;
            Account* linked_account_;
    };



    struct CardNum
    {
        char value[16];
    };

    struct Money
    {
        long big_part;
        int little_part;
    };
}

namespace mb = Moneybeast;

#endif //!MBTYPES_HXX
