/**
 * @file mbtypes.hxx
 * @author Alex Alexeev <activity.alex69985@gmail.com>
 * @brief
 * @version 0.3
 * @date 2022-05-21
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cstdlib>
#include <cstring>

#ifndef MBTYPES_HXX
#define MBTYPES_HXX

namespace Moneybeast
{
    using Money = long double;
    using Interest = float;
    using AccountId = unsigned long;

    class Account;
    class Card;
    class CaStaff;
    class CDStaff;
    class Client;
    class Person;
    struct AccessToken;
    struct CardNum;
    struct Passport;
    



    /**
     * @brief Card number struct.
     *
     */
    struct CardNum
    {
        char value[16];

        CardNum() { }
        CardNum(const char v[16]);
        CardNum (const std::string &v)
            : CardNum (v.c_str())
        { }

        bool operator==(const CardNum &obj) const;
        bool operator<(const CardNum &obj) const;

        explicit operator std::string () const
        {
            return std::string{value, 0, 16};
        }
    };

    /**
     * @brief Passport number class.
     *
     */
    struct Passport
    {
        char num[10];

        Passport()
            : Passport ("0000000000")
        { }
        Passport(const char n[10]);
        Passport (const std::string &n)
            : Passport (n.c_str())
        { }

        explicit operator std::string () const
        {
            return std::string{num, 0, 10};
        }
    };

    /**
     * @brief Access Token.
     * 
     */
    struct AccessToken
    {
        char value[3];

        AccessToken();
        AccessToken (const char v[3]);
        AccessToken (const std::string &v)
            : AccessToken (v.c_str())
        { }

        constexpr bool
        operator== (const AccessToken &obj) const
        {
            if (value[0] == obj.value[0] && value[1] == obj.value[1]
             && value[2] == obj.value[2])
                return true;
            else
                return false;
        }

        explicit operator std::string () const
        {
            return std::string{value, 0, 3};
        }

        bool isSet() const;
    };



    /**
     * @brief Basic class for people.
     *
     */
    class Person
    {
    public:
        Person() {}
        Person (const Person &obj)
        {
            this->pass_ = obj.pass_;
            this->first_name_ = obj.first_name_;
            this->last_name_ = obj.last_name_;
            this->patronymic_ = obj.patronymic_;
            this->email_ = obj.email_;
        }

        /**
         * @brief Construct a new Person object.
         *
         * @param pass Passport.
         * @param first_name First name.
         * @param last_name Last name.
         * @param email Email.
         * @param patronymic Patronymic (optional).
         */
        Person(
            const Passport &pass,
            const std::string first_name,
            const std::string last_name,
            const std::string email,
            const std::string patronymic = "")
            : pass_(pass), first_name_(first_name), last_name_(last_name),
              email_(email), patronymic_(patronymic)
        {
        }

        /**
         * @brief Get the Passport Num object.
         *
         * @return Passport
         */
        inline Passport
        getPassportNum() const
        { return pass_; }

        /**
         * @brief Set the Passport Num.
         * 
         * @param pass 
         */
        inline void
        setPassportNum (const Passport &pass)
        { pass_ = pass; }

        /**
         * @brief Get the First Name object.
         *
         * @return std::string
         */
        inline std::string
        getFirstName() const
        { return first_name_; }

        /**
         * @brief Set the First Name.
         * 
         * @param first_name 
         */
        inline void
        setFirstName (const std::string &first_name)
        { first_name_ = first_name; }

        /**
         * @brief Get the Last Name.
         *
         * @return std::string
         */
        inline std::string getLastName() const
        { return last_name_; }

        /**
         * @brief Set the Last Name.
         * 
         * @param first_name 
         */
        inline void
        setLastName (const std::string &last_name)
        { last_name_ = last_name; }

        /**
         * @brief Get the Patronymic.
         *
         * @return std::string
         */
        inline std::string
        getPatronymic() const
        { return patronymic_; }

        /**
         * @brief Set the Patronymic.
         * 
         * @param patronymic New patronymic.
         */
        inline void
        setPatronymic (const std::string &patronymic)
        { patronymic_ = patronymic; }

        /**
         * @brief Get the Email.
         *
         * @return std::string
         */
        inline std::string
        getEmail() const
        { return email_; }

        /**
         * @brief Set the Email.
         * 
         * @param email New email.
         */
        inline void
        setEmail (const std::string &email)
        { email_ = email; }

        inline Person&
        operator= (const Person &obj)
        {
            this->pass_ = obj.pass_;
            this->first_name_ = obj.first_name_;
            this->last_name_ = obj.last_name_;
            this->patronymic_ = obj.patronymic_;
            this->email_ = obj.email_;

            return *this;
        }

    protected:
        Passport pass_;
        std::string
            first_name_,
            last_name_,
            patronymic_;

        std::string email_;
    };

    /**
     * @brief Bank client.
     *
     */
    class Client : public Person
    {
    public:
        Client() {}
        Client (const Client &obj)
        {
            this->pass_ = obj.pass_;
            this->first_name_ = obj.first_name_;
            this->last_name_ = obj.last_name_;
            this->patronymic_ = obj.patronymic_;
            this->email_ = obj.email_;
            this->accounts_ = obj.accounts_;
            this->cards_ = obj.cards_;
            this->access_token_ = obj.access_token_;
            this->inner_wallet_ = obj.inner_wallet_;
        }

        /**
         * @brief Construct a new Client object.
         *
         * @param pass Passport.
         * @param first_name First name.
         * @param last_name Last name.
         * @param email Email.
         * @param patronymic Patronymic (optional).
         */
        Client(
            const Passport &pass,
            const std::string first_name,
            const std::string last_name,
            const std::string email,
            const std::string patronymic = "",
            const AccessToken &token = AccessToken{"000"})
            : Person(pass, first_name, last_name, email, patronymic),
              access_token_(token), inner_wallet_ (0.0)
        {
        }

        /**
         * @brief Add card to the client.
         *
         * @param card Card pointer.
         */
        void addCard(Card *card);
        /**
         * @brief Add account to the
         *
         * @param uniqueId Indentificator for account.
         * @param account Account.
         */
        void addAccount(Account *account);
        /**
         * @brief Remove card.
         *
         * @param card_num Card number.
         */
        void removeCard(const CardNum &card_num);
        /**
         * @brief Remove account.
         *
         * @param account_id Account unique indentificator.
         */
        void removeAccount(const AccountId& account_id);

        /**
         * @brief Get the Card By Number object.
         *
         * @param card_num Card number.
         * @return Card* Pointer to card if found.
         */
        Card* getCardByNumber(const CardNum &card_num) const;
        /**
         * @brief Get the Account By Id object.
         *
         * @param account_id Account unique indentificator.
         * @return Account* Pointer to the account if found.
         */
        Account* getAccountById(const AccountId &account_id) const;
        inline Money
        getWalletState() const
        { return inner_wallet_; }
        inline void
        setWallet(const Money &money_amount)
        { inner_wallet_ = money_amount; }

        /**
         * @brief Get the Access Token object.
         * 
         * @return AccessToken 
         */
        inline AccessToken
        getAccessToken() const
        { return access_token_; }

        /**
         * @brief Set the Access Token.
         * 
         * @param token New token value.
         */
        inline void
        setAccessToken(const AccessToken& token)
        { access_token_ = token; }

        /**
         * @brief Get the Login.
         * 
         * @return std::string 
         */
        inline std::string
        getLogin() const
        { return login_; }

        /**
         * @brief Set the Login.
         * 
         * @param token New login value.
         */
        inline void
        setLogin(const std::string &login)
        { login_ = login; }

        inline std::map<AccountId, Account*>&
        getAccountsMap()
        { return accounts_; }

        inline std::map<CardNum, Card*>&
        getCardsMap()
        { return cards_; }

        inline Client&
        operator= (const Client &obj)
        {
            this->pass_ = obj.pass_;
            this->first_name_ = obj.first_name_;
            this->last_name_ = obj.last_name_;
            this->patronymic_ = obj.patronymic_;
            this->email_ = obj.email_;
            this->accounts_ = obj.accounts_;
            this->cards_ = obj.cards_;
            this->access_token_ = obj.access_token_;
            this->inner_wallet_ = obj.inner_wallet_;

            return *this;
        }

    protected:
        std::map<AccountId, Account*> accounts_;
        std::map<CardNum, Card*> cards_;

        AccessToken access_token_;
        Money inner_wallet_;
        std::string login_;
    };

    /**
     * @brief A particular employee of the card issuance department.
     *
     */
    class CaStaff final : public Client
    {
    public:
        CaStaff() {}
        CaStaff (const CaStaff &obj)
        {
            this->pass_ = obj.pass_;
            this->first_name_ = obj.first_name_;
            this->last_name_ = obj.last_name_;
            this->patronymic_ = obj.patronymic_;
            this->email_ = obj.email_;
            this->accounts_ = obj.accounts_;
            this->cards_ = obj.cards_;
            this->access_token_ = obj.access_token_;
            this->inner_wallet_ = obj.inner_wallet_;
        }

        /**
         * @brief Construct a new Ca Staff object.
         *
         * @param pass Passport.
         * @param first_name First name.
         * @param last_name Last name.
         * @param email Email.
         * @param patronymic Patronymic (optional).
         */
        CaStaff(
            const Passport &pass,
            const std::string &first_name,
            const std::string &last_name,
            const std::string &email,
            const std::string &patronymic = "",
            const AccessToken &token = AccessToken{"000"})
            : Client (pass, first_name, last_name, email, patronymic, token)
        { }

        /**
         * @brief Open Unnamed card for client.
         *
         * @param client Client.
         * @param account Account which to link the card.
         * @return Card* Card pointer.
         */
        Card *openUnnamedCard(const Client *client, Account *account);

        inline CaStaff&
        operator= (const CaStaff &obj)
        {
            this->pass_ = obj.pass_;
            this->first_name_ = obj.first_name_;
            this->last_name_ = obj.last_name_;
            this->patronymic_ = obj.patronymic_;
            this->email_ = obj.email_;
            this->accounts_ = obj.accounts_;
            this->cards_ = obj.cards_;
            this->access_token_ = obj.access_token_;
            this->inner_wallet_ = obj.inner_wallet_;

            return *this;
        }
    };

    /**
     * @brief A particular employee of the credit/deposit issuance department.
     *
     */
    class CDStaff final : public Client
    {
    public:
        CDStaff() {}
        CDStaff (const CDStaff &obj)
        {
            this->pass_ = obj.pass_;
            this->first_name_ = obj.first_name_;
            this->last_name_ = obj.last_name_;
            this->patronymic_ = obj.patronymic_;
            this->email_ = obj.email_;
            this->accounts_ = obj.accounts_;
            this->cards_ = obj.cards_;
            this->access_token_ = obj.access_token_;
            this->inner_wallet_ = obj.inner_wallet_;
        }

        /**
         * @brief Construct a new CDStaff object.
         *
         * @param pass Passport.
         * @param first_name First name.
         * @param last_name Last name.
         * @param email Email.
         * @param patronymic Patronymic (optional).
         */
        CDStaff(
            const Passport &pass,
            const std::string first_name,
            const std::string last_name,
            const std::string email,
            const std::string patronymic = "",
            const AccessToken &token = AccessToken{"000"})
            : Client (pass, first_name, last_name, email, patronymic, token)
        { }

        /**
         * @brief Open Unnamed credit for client.
         *
         * @param client Client pointer.
         * @param money_amount Money amount.
         * @param interest Interest.
         * @return Account*
         */
        Account *openUnnamedCredit(
            const Client *client,
            const Money& money_amount, const Interest& interest);
        /**
         * @brief Open Unnamed deposit for client.
         *
         * @param client Client pointer.
         * @param money_amount Money amount.
         * @param interest Interest.
         * @return Account*
         */
        Account *openUnnamedDeposit(
            const Client *client,
            const Money& money_amount, const Interest& interest);
        
        inline CDStaff&
        operator= (const CDStaff &obj)
        {
            this->pass_ = obj.pass_;
            this->first_name_ = obj.first_name_;
            this->last_name_ = obj.last_name_;
            this->patronymic_ = obj.patronymic_;
            this->email_ = obj.email_;
            this->accounts_ = obj.accounts_;
            this->cards_ = obj.cards_;
            this->access_token_ = obj.access_token_;
            this->inner_wallet_ = obj.inner_wallet_;

            return *this;
        }
    };

    /**
     * @brief Client's account.
     *
     */
    class Account final
    {
    public:
        /**
         * @brief Construct a new Account object.
         *
         * @param money_amount Money amount.
         * @param interest Interest.
         */
        Account(const AccountId &id, const Money &money_amount, const Interest &interest)
            : id_(id), money_amount_(money_amount), interest_(interest)
        { }

        /**
         * @brief Get the Id.
         * 
         * @return AccountId 
         */
        inline AccountId
        getId () const
        {
            return id_;
        }
        /**
         * @brief Get Money Amount.
         *
         * @return Money
         */
        inline Money
        getMoneyAmount() const
        {
            return money_amount_;
        }
        /**
         * @brief Get Interest.
         *
         * @return Interest
         */
        inline Interest
        getInterest() const
        {
            return interest_;
        }
        /**
         * @brief Get the Owner pointer.
         *
         * @return Client*
         */
        inline Client *
        getOwner() const
        {
            return owner_;
        }
        /**
         * @brief Deposit money into the account.
         *
         * @param money_amount Money amount.
         */
        void depositMoney(const Money &money_amount);
        /**
         * @brief Transfer money from account to client's inner wallet.
         *
         * @param money_amount Money amount.
         */
        void takeMoney(const Money &money_amount);

    private:
        AccountId id_;
        Client *owner_;
        Money money_amount_;
        Interest interest_;
    };

    /**
     * @brief Card.
     *
     */
    class Card final
    {
    public:
        /**
         * @brief Construct a new Card object.
         *
         * @param card_num Card number.
         * @param account Account.
         */
        Card(const CardNum &card_num, Account *account)
            : card_num_(card_num), linked_account_(account)
        { }

        /**
         * @brief Get the Card Num object.
         *
         * @return CardNum
         */
        inline CardNum
        getCardNum() const
        {
            return card_num_;
        }
        /**
         * @brief Get the Linked Account Id object.
         *
         * @return Account*
         */
        inline Account *
        getLinkedAccount() const
        {
            return linked_account_;
        }

    private:
        CardNum card_num_;
        Account *linked_account_;
    };
}

namespace mb = Moneybeast;


#endif //! MBTYPES_HXX
