/**
 * @file mbtypes.hxx
 * @author Alex Alexeev <activity.alex69985@gmail.com>
 * @brief 
 * @version 0.1
 * @date 2022-05-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <string>
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





    /**
     * @brief Basic class for people.
     * 
     */
    class Person
    {
    public:
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
            const Passport& pass,
            const std::string first_name,
            const std::string last_name,
            const std::string email,
            const std::string patronymic = ""
        );

        /**
         * @brief Get the Passport Num object.
         * 
         * @return Passport 
         */
        inline Passport
        getPassportNum() const
        { return pass_; }
        /**
         * @brief Get the First Name object.
         * 
         * @return std::string 
         */
        inline std::string
        getFirstName() const
        { return first_name_; }
        /**
         * @brief Get the Last Name object.
         * 
         * @return std::string 
         */
        inline std::string getLastName() const
        { return last_name_; }
        /**
         * @brief Get the Patronymic object.
         * 
         * @return std::string 
         */
        inline std::string
        getPatronymic() const
        { return patronymic_; }
        /**
         * @brief Get the Email object.
         * 
         * @return std::string 
         */
        inline std::string
        getEmail() const
        { return email_; }
        /**
         * @brief Get the Access Key object.
         * 
         * @return std::string 
         */
        inline std::string
        getAccessKey() const
        { return key_; }

    protected:
        const Passport pass_;
        const std::string
            first_name_,
            last_name_,
            patronymic_;

        const std::string
            email_,
            key_;
    };





    /**
     * @brief Bank client.
     * 
     */
    class Client final : public Person
    {
    public:
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
            const Passport& pass,
            const std::string first_name,
            const std::string last_name,
            const std::string email,
            const std::string patronymic = ""
        );

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
        void addAccount(long uniqueId, Account *account);
        /**
         * @brief Remove card.
         * 
         * @param card_num Card number.
         */
        void removeCard(const CardNum& card_num);
        /**
         * @brief Remove account.
         * 
         * @param account_id Account unique indentificator.
         */
        void removeAccount(long account_id);

        /**
         * @brief Get the Card By Number object.
         * 
         * @param card_num Card number.
         * @return Card* Pointer to card if found.
         */
        Card *getCardByNumber(CardNum &card_num) const;
        /**
         * @brief Get the Account By Id object.
         * 
         * @param account_id Account unique indentificator.
         * @return Account* Pointer to the account if found.
         */
        Account *getAccountById(Account &account_id) const;

    private:
        std::map<long, Account> accounts_;
        std::map<CardNum, Card> cards_;
    };





    /**
     * @brief A particular employee of the card issuance department.
     * 
     */
    class CaStaff final : public Person
    {
    public:
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
            const Passport& pass,
            const std::string first_name,
            const std::string last_name,
            const std::string email,
            const std::string patronymic = ""
        );

        /**
         * @brief Open Unnamed card for client.
         * 
         * @param client Client.
         * @param account Account which to link the card.
         * @return Card* Card pointer.
         */
        Card *openUnnamedCard(const Client *client, Account *account);
    };





    /**
     * @brief A particular employee of the credit/deposit issuance department.
     * 
     */
    class CDStaff final : public Person
    {
    public:
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
            const Passport& pass,
            const std::string first_name,
            const std::string last_name,
            const std::string email,
            const std::string patronymic = ""
        );

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
            long money_amount, long interest);
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
            long money_amount, long interest);
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
        Account(const Money &money_amount, long interest);

        /**
         * @brief Get the Money Amount object.
         * 
         * @return Money 
         */
        Money getMoneyAmount() const;
        /**
         * @brief Get the Interest object.
         * 
         * @return long 
         */
        long getInterest() const;
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
        Client* owner_;
        Money money_amount_;
        long interest_;
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
        Card(CardNum &card_num, Account* account);

        /**
         * @brief Get the Card Num object.
         * 
         * @return CardNum 
         */
        CardNum getCardNum() const;
        /**
         * @brief Get the Linked Account Id object.
         * 
         * @return long 
         */
        long getLinkedAccountId() const;

    private:
        CardNum card_num;
        Account *linked_account_;
    };





    /**
     * @brief Card number struct.
     * 
     */
    struct CardNum
    {
        char value[16];

        bool operator==(const CardNum &obj);
        bool operator<=(const CardNum &obj);
        bool operator>=(const CardNum &obj);
        bool operator<(const CardNum &obj);
        bool operator>(const CardNum &obj);
    };

    /**
     * @brief Money struct.
     * 
     */
    struct Money
    {
        long big_part;
        int little_part;

        bool operator==(const Money &obj);
        bool operator<=(const Money &obj);
        bool operator>=(const Money &obj);
        bool operator<(const Money &obj);
        bool operator>(const Money &obj);
    };

    /**
     * @brief Passport number class.
     * 
     */
    struct Passport
    {
        char num[10];
    };
}

namespace mb = Moneybeast;

#endif //! MBTYPES_HXX
