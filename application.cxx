#include "application.hxx"

Application::Application (int argc, char **argv,
    const std::string config_dir)
    : description_("Использование программы"), config_dir_ (std::string{getenv("HOME")} + config_dir)
{
    try {
        description_.add_options()
            ("help,h", "Получить помощь.")
            ("register", "Пройти процесс входа в аккаунт.")
            ("whoami", "Вывести информацию о себе.")
            ("status", "Вывести расширенную информацию о себе.")
            ("update", "Обновить информацию.\n")

            ("accountmod,a", "Изменить состояние счёта.")
            ("cardmod,c", "Изменить карту.")
            ("walletmod,w", "Изменить состояние внутреннего кошелька.\n")

            ("deposit,D", "Перевод на кошелёк/счёт.")
            ("take,T", "Вывод с кошелька/счёта.")
            ("open,O", "Открыть карту/счёт.")
            ("delete", "Удалить счёт/карту.\n")

            ("accounttype", bpo::value<std::string>(&tmp_bpo_input), "Тип открываемого счёта [deposit/credit].")
            ("accountid", bpo::value<mb::AccountId>(&tmp_account_id), "ID счёта.")
            ("cardid", bpo::value<std::string>(&tmp_bpo_input), "Немер карты.")
            ("interest", bpo::value<mb::Interest>(&tmp_interest), "Процент.")
            ("sum", bpo::value<mb::Money>(&tmp_money), "Размер суммы.")
        ;
        bpo::store (bpo::parse_command_line (argc, argv, description_), vm_map_);
        bpo::notify (vm_map_);
    }
    catch (const bpo::error &e)
    {
        std::cout << e.what() << std::endl
                  << description_ << std::endl;
        exit(INCORRECT_OPTIONS);
    }


    if (!stdfs::is_directory (config_dir_))
    {
        stdfs::create_directories (config_dir_);
    }

    /* Load saved user configuration */
    {
        bpt::ptree pt;
        try
        {
            bpt::json_parser::read_json (config_dir_ + "/config.json", pt);
        }
        catch (const bpt::json_parser_error)
        { }

        /* Load Client Info */
        client_ = mb::Client {
            mb::Passport{pt.get("passport", "0000000000")},
            pt.get ("first_name", "Noname"),
            pt.get ("last_name", "Noname"),
            pt.get ("email", "n/a"),
            pt.get ("patronymic", ""),
            mb::AccessToken{pt.get("access_token", "000")}
        };
        client_.setWallet (pt.get<mb::Money>("wallet", 0.0));
        client_.setLogin (pt.get ("login", ""));
    }

    /* Load client's accounts */
    {
        bpt::ptree pt;
        try
        {
            bpt::json_parser::read_json(config_dir_ + "/accounts.json", pt);
            for (auto k : pt.get_child("accounts"))
            {
                mb::Account *a = new mb::Account{
                    k.second.get<mb::AccountId>("id"),
                    k.second.get<mb::Money>("money"),
                    k.second.get<mb::Interest>("interest")
                };
                client_.addAccount(a);
            }
        }
        catch(const bpt::json_parser_error& e)
        {
            std::cerr << e.what() << std::endl;
        }
        catch(const boost::wrapexcept<boost::property_tree::ptree_bad_path>& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    /* Load client's cards */
    {
        bpt::ptree pt;
        try
        {
            bpt::json_parser::read_json(config_dir_ + "/cards.json", pt);
            for (auto k : pt.get_child("cards"))
            {
                mb::Card *c = new mb::Card{
                    mb::CardNum{k.second.get<std::string>("num")},
                    client_.getAccountById(k.second.get<mb::AccountId>("accountid")),
                };
                client_.addCard(c);
            }
        }
        catch(const bpt::json_parser_error& e)
        {
            std::cerr << e.what() << std::endl;
        }
        catch(const boost::wrapexcept<boost::property_tree::ptree_bad_path>& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

Application::~Application ()
{
    /* Save user information */
    {
        bpt::ptree pt;
        pt.put ("passport",     std::string{client_.getPassportNum()});
        pt.put ("first_name",   client_.getFirstName());
        pt.put ("last_name",    client_.getLastName());
        pt.put ("email",        client_.getEmail());
        pt.put ("patronymic",   client_.getPatronymic());
        pt.put ("access_token", std::string{client_.getAccessToken()});
        pt.put ("login",        client_.getLogin());
        pt.put ("wallet",       client_.getWalletState());

        try
        {
            bpt::json_parser::write_json (config_dir_ + "/config.json", pt);
        }
        catch (const bpt::json_parser_error &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    /* Save accounts */
    {
        bpt::ptree pt_main, pt, tmp_pt;
        try
        {
            for (auto k : client_.getAccountsMap())
            {
                tmp_pt.put ("id", k.first);
                tmp_pt.put ("money", k.second->getMoneyAmount());
                tmp_pt.put ("interest", k.second->getInterest());
                pt.push_back (std::make_pair ("", tmp_pt));
            }

            pt_main.add_child ("accounts", pt);
            bpt::json_parser::write_json(config_dir_ + "/accounts.json", pt_main);
        }
        catch(const bpt::json_parser_error& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    /* Save cards */
    {
        bpt::ptree pt_main, pt, tmp_pt;
        try
        {
            for (auto k : client_.getCardsMap())
            {
                tmp_pt.put ("num", std::string{k.first});
                tmp_pt.put ("accountid", k.second->getLinkedAccount()->getId());
                pt.push_back (std::make_pair ("", tmp_pt));
            }

            pt_main.add_child ("cards", pt);
            bpt::json_parser::write_json(config_dir_ + "/cards.json", pt_main);
        }
        catch(const bpt::json_parser_error& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

int Application::execute()
{
    /* Print help, if user need it */
    if (vm_map_.count("help") > 0)
    {
        std::cerr << description_ << std::endl;
        return SUCCESS;
    }

    /* Register user */
    if (vm_map_.count ("register"))
        return registerUser();

    /*  */
    if (!client_.getAccessToken().isSet())
    {
        std::cerr << "Ошибка: пользователь должен пройти регистрацию.\n"
                     "Для этого запустите программу с опцией \"--register\"."
                  << std::endl;
        return REGISTRATION_REQUIRED;
    }

    if (vm_map_.count ("update") > 0)
    {
        return (mb::system::update_personal_info (client_)
             && mb::system::update_accounts_info (client_)
             && mb::system::update_cards_info    (client_)
             ) ? SUCCESS : UPDATE_FAILED;
    }

    /* Print who is the user */
    if (vm_map_.count ("whoami") > 0)
    {
        return whoAmI();
    }

    /* Print extended user information */
    if (vm_map_.count ("status") > 0)
    {
        return printClientInfo();
    }
    
    /* Accounts mod */
    if (vm_map_.count ("accountmod") || vm_map_.count ("a"))
    {
        if (vm_map_.count ("delete"))
        {
            if (vm_map_.count ("accountid"))
            {
                client_.removeAccount (tmp_account_id);
                return SUCCESS;
            }
            else
            {
                std::cerr << "Опция \"--delete\" не указана." << std::endl;
                return NOT_ENOUGHS_OPTIONS;
            }
        }

        else if (vm_map_.count ("open") || vm_map_.count ("O"))
        {
            if ((vm_map_.count ("accounttype")
                    && (tmp_bpo_input == "deposit" || tmp_bpo_input == "credit")
                ) && vm_map_.count ("interest") && vm_map_.count ("sum"))
            {
                std::vector<mb::CDStaff> v;
                if (!mb::system::get_cdstaff (client_.getAccessToken(), v) || v.size() < 1)
                    return FETCH_ERROR;
                
                mb::Account *a;
                if (tmp_bpo_input == "deposit")
                    a = v[0].openUnnamedDeposit(&client_, tmp_money, tmp_interest);
                else
                    a = v[0].openUnnamedCredit (&client_, tmp_money, tmp_interest);
                
                if (a != nullptr)
                    client_.addAccount (a);
                return SUCCESS;
            }
            else
            {
                std::cerr << "Опция \"--[accounttype|interest|sum]\" не указана." << std::endl;
                return NOT_ENOUGHS_OPTIONS;
            }
        }

        else if (vm_map_.count ("take") || vm_map_.count ("T"))
        {
            if (vm_map_.count ("accountid"))
            {
                if (vm_map_.count ("sum"))
                {
                    client_.getAccountById (tmp_account_id)->takeMoney (tmp_money);
                    return SUCCESS;
                }
                else
                {
                    std::cerr << "Опция \"--sum\" не указана." << std::endl;
                    return NOT_ENOUGHS_OPTIONS;
                }
            }
            else
            {
                std::cerr << "Опция \"--accountid\" не указана." << std::endl;
                return NOT_ENOUGHS_OPTIONS;
            }
        }

        else if (vm_map_.count ("deposit") || vm_map_.count ("D"))
        {
            if (vm_map_.count ("accountid"))
            {
                if (vm_map_.count ("sum"))
                {
                    client_.getAccountById (tmp_account_id)->depositMoney (tmp_money);
                    return SUCCESS;
                }
                else
                {
                    std::cerr << "Опция \"--sum\" не указана." << std::endl;
                    return NOT_ENOUGHS_OPTIONS;
                }
            }
            else
            {
                std::cerr << "Опция \"--accountid\" не указана." << std::endl;
                return NOT_ENOUGHS_OPTIONS;
            }
        }

        else
        {
            std::cerr << "Опция \"--[take|deposit|open|delete]\" не указана." << std::endl;
            return NOT_ENOUGHS_OPTIONS;
        }
    }

    /* Cards mod */
    if (vm_map_.count ("cardmod") || vm_map_.count ("c"))
    {
        if (vm_map_.count ("delete"))
        {
            if (vm_map_.count ("cardid"))
            {
                client_.removeCard (mb::CardNum{tmp_bpo_input});
                return SUCCESS;
            }
            else
            {
                std::cerr << "Опция \"--cardid\" не указана." << std::endl;
                return NOT_ENOUGHS_OPTIONS;
            }
        }
        else if (vm_map_.count ("open") || vm_map_.count ("O"))
        {
            if (vm_map_.count ("accountid"))
            {
                std::vector<mb::CaStaff> v;
                if (!mb::system::get_castaff (client_.getAccessToken(), v) || v.size() < 1)
                    return FETCH_ERROR;
                
                mb::Card *c;
                if (client_.getAccountById(tmp_account_id) != nullptr)
                {
                    c = v[0].openUnnamedCard (&client_, client_.getAccountById(tmp_account_id));
                    if (c != nullptr)
                    {
                        client_.addCard(c);
                    }
                    return SUCCESS;
                }
                else
                {
                    std::cout << "ОШИБКА: такого счёта не существует." << std::endl;
                    return FETCH_ERROR;
                }
            }
            else
            {
                std::cerr << "Опция \"--accountid\" не указана." << std::endl;
                return NOT_ENOUGHS_OPTIONS;
            }
        }
        else
        {
            std::cerr << "Опция \"--[delete|open]\" не указана." << std::endl;
            return NOT_ENOUGHS_OPTIONS;
        }
    }

    /* Wallet mod */
    if (vm_map_.count ("walletmod") || vm_map_.count ("w"))
    {
        if (vm_map_.count ("take") || vm_map_.count ("T"))
        {
            if (vm_map_.count ("sum"))
            {
                if (client_.getWalletState() < tmp_money)
                {
                    std::cout << "На внутреннем кошельке недостаточно денег. Установка значения внутреннего кошелька на 0." << std::endl;
                    client_.setWallet(0.0);
                }
                else
                {
                    client_.setWallet (client_.getWalletState() - tmp_money);
                }
                return SUCCESS;
            }
            else
            {
                std::cerr << "Опция \"--sum\" не указана." << std::endl;
                return NOT_ENOUGHS_OPTIONS;
            }
        }

        else if (vm_map_.count ("deposit") || vm_map_.count ("D"))
        {
            if (vm_map_.count ("sum"))
            {
                client_.setWallet (client_.getWalletState() + tmp_money);
                return SUCCESS;
            }
            else
            {
                std::cerr << "Опция \"--sum\" не указана." << std::endl;
                return NOT_ENOUGHS_OPTIONS;
            }
        }
        else
        {
            std::cerr << "Опция \"--[take|deposit]\" не указана." << std::endl;
            return NOT_ENOUGHS_OPTIONS;
        }
    }

    return SUCCESS;
}

int Application::registerUser()
{
    std::string login, passwd;
    mb::AccessToken token;
        
    std::cout << "Введите логин:\t"; std::cin >> login;
    std::cout << "Введите пароль:\t"; std::cin >> passwd;

    if ( mb::system::try_login (login, passwd, token) )
    {
        client_.setLogin (login);
        client_.setAccessToken (token);

        std::cout << "Успешная регистрация! Загрузка данных..." << std::endl;
        std::cout << " *  Персональная информация... "
                  << ((mb::system::update_personal_info(client_)) ? "ГОТОВО" : "ОШИБКА")
                  << std::endl;
        std::cout << " *  Список счетов... "
                  << ((mb::system::update_accounts_info(client_)) ? "ГОТОВО" : "ОШИБКА")
                  << std::endl;
        std::cout << " *  Список карт... "
                  << ((mb::system::update_cards_info(client_)) ? "ГОТОВО" : "ОШИБКА")
                  << std::endl;
        return SUCCESS;
    }
    else
    {
        std::cerr << "ОШИБКА: неверный логин или пароль." << std::endl;
        return REGISTRATION_FAILED;
    }
}

int Application::whoAmI ()
{
    std::cout << client_.getFirstName() << " "
              << client_.getLastName()  << " "
              << client_.getPatronymic()
              << " <" << client_.getEmail() << ">" << std::endl;
    return SUCCESS;
}

int Application::printClientInfo()
{
    using std::cout;
    using std::endl;

    cout << "Информация о клиенте:" << endl
         << " *  Фамилия: " << client_.getLastName() << endl
         << " *  Имя: " << client_.getFirstName() << endl
         << " *  Отчество: " << client_.getPatronymic() << endl
         << " *  Паспорт: " << std::string{client_.getPassportNum()} << endl
         << " *  E-mail: " << client_.getEmail() << endl
         << " *  Логин: " << client_.getLogin() << endl
         << " *  Токен: " << std::string{client_.getAccessToken()} << endl
         << " *  Состояние внутреннего кошелька: " << client_.getWalletState() << endl;
    
    cout << "Список счетов:" << endl
         << "| ID счёта | Процент, % | Состояние счёта |" << endl;
    for (auto k : client_.getAccountsMap())
    {
        cout << " *  " << k.first << "\t" << k.second->getInterest() << "\t" << k.second->getMoneyAmount() << endl;
    }

    cout << "Список карт:" << endl
         << "| Номер карты | ID счёта |" << endl;
    for (auto k : client_.getCardsMap())
    {
        cout << " *  " << std::string{k.first} << "\t" << k.second->getLinkedAccount()->getId() << endl;
    }

    return SUCCESS;
}
