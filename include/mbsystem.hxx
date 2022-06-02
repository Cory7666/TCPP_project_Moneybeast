/**
 * @file mbsystem.hxx
 * @author Alex Alexeev <activity.alex69985@gmail.com>
 * @brief Basic bank system.
 * @version 0.1
 * @date 2022-06-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <string>
#include <vector>

#include <mbtypes.hxx>

#ifndef MBSYSTEM_HXX
#define MBSYSTEM_HXX

namespace Moneybeast
{
    namespace System
    {
        /**
         * @brief Try to login by sending login and password to the trusted
         * server.
         * 
         * @param login User's login.
         * @param password User's password.
         * @param access_token Access token that would be received from trusted
         * server.
         * @return true if success else false
         */
        bool
        try_login (std::string &&login, std::string &&password,
        Moneybeast::AccessToken &access_token);

        /**
         * @brief Update information about client with token.
         * 
         * @param client
         */
        void
        update_personal_info (Moneybeast::Client &client);

        /**
         * @brief Update information about client's cards.
         * 
         * @param client 
         */
        void
        update_cards_info (Moneybeast::Client &client);
        
        /**
         * @brief Update information about client's accounts.
         * 
         * @param client 
         */
        void
        update_accounts_info (Moneybeast::Client &client);


        
        /**
         * @brief Get list of active Staff of Cards Department.
         * 
         * @param access_token 
         * @param list 
         */
        void
        get_castaff (Moneybeast::AccessToken &&access_token,
                     std::vector<Moneybeast::CaStaff> &list);
        
        /**
         * @brief Get list of active Staff of Credits/Deposits Department.
         * 
         * @param access_token 
         * @param list 
         */
        void
        get_cdstaff (Moneybeast::AccessToken &&access_token,
                     std::vector<Moneybeast::CDStaff> &list);
    }

    namespace system = System;
}

#endif //! MBSYSTEM_HXX
