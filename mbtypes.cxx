#include "mbtypes.hxx"

using namespace Moneybeast;





void Client::addCard (Card *card)
{
    cards_.insert(std::pair<CardNum, Card*>{card->getCardNum(), card});
}

void Client::addAccount(Account *account)
{
    accounts_.insert(std::pair<AccountId, Account*>{account->getId(), account});
}

void Client::removeCard(const CardNum &card_num)
{
    cards_.erase(card_num);
}

void Client::removeAccount(const AccountId& account_id)
{
    accounts_.erase(account_id);
}

Card* Client::getCardByNumber(CardNum &card_num) const
{
    auto t = cards_.find (card_num);
    if (t != cards_.end())
        return t->second;
    else
        return nullptr;
}

Account* Client::getAccountById(AccountId &account_id) const
{
    auto t = accounts_.find (account_id);
    if (t != accounts_.end())
        return t->second;
    else
        return nullptr;
}





Card* CaStaff::openUnnamedCard(const Client *client, Account *account)
{
    CardNum card_num{};
    for (int i = 0; i < 16; i++)
        card_num.value[i] = (std::rand() % 10) + '0';

    return new Card{card_num, account};
}





Account* CDStaff::openUnnamedCredit(
            const Client *client,
            const Money& money_amount, const Interest& interest)
{
    return new Account{money_amount, -interest};
}

Account* CDStaff::openUnnamedDeposit(
            const Client *client,
            const Money& money_amount, const Interest& interest)
{
    return new Account{money_amount, interest};
}





void Account::depositMoney(const Money &money_amount)
{
    if (money_amount > owner_->getWalletState())
        return;
    
    owner_->setWallet(owner_->getWalletState() - money_amount);
}

void Account::takeMoney(const Money &money_amount)
{
    if (money_amount > money_amount_)
        return;
    
    owner_->setWallet(owner_->getWalletState() + money_amount);
    money_amount_ -= money_amount;
}





CardNum::CardNum (const char v[16])
{
    std::memcpy (value, v, 16);
}
bool CardNum::operator== (const CardNum &obj) const
{
    for (int i = 0; i < 16; i++)
        if (value[i] != obj.value[i])
            return false;
    return true;
}
bool CardNum::operator<(const CardNum &obj) const
{
    for (int i = 0; i < 16; i++)
        if (value[i] >= obj.value[i])
            return false;
    return true;
}





Passport::Passport (const char n[10])
{
    std::memcpy (num, n, 10);
}
