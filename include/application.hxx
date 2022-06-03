#include <iostream>
#include <string>
#include <filesystem>
#include <iterator>
#include <vector>
#include <map>

#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "mbtypes.hxx"
#include "mbsystem.hxx"

#ifndef APPLICATION_HXX
#define APPLICATION_HXX

namespace bpo = boost::program_options;
namespace bpt = boost::property_tree;
namespace stdfs = std::filesystem;

class Application final
{
public:
    enum RETURN_CODES {
        SUCCESS,
        INCORRECT_OPTIONS,
        NOT_ENOUGHS_OPTIONS,
        REGISTRATION_FAILED,
        REGISTRATION_REQUIRED,
        UPDATE_FAILED,
        FETCH_ERROR
    };

    Application () = delete;
    Application (int argc, char **argv,
        const std::string config_dir = "/.config/Moneybeast");
    Application (const Application&) = delete;
    Application (Application&&) = delete;
    ~Application();

    int execute();

private:
    /**
     * @brief Start user registration procedure.
     * 
     * @return int 
     */
    int registerUser();
    int whoAmI ();
    int printClientInfo();

    bpo::variables_map vm_map_;
    bpo::options_description description_;

    mb::Client client_;

    const std::string config_dir_;

    // non-important variables
    mb::Money tmp_money;
    mb::Interest tmp_interest;
    mb::AccountId tmp_account_id;
    std::string tmp_bpo_input;
};

#endif //! APPLICATION_HXX
