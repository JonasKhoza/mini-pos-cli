#ifndef APP_H
#define APP_H

#include "domain/Product.h"
#include "domain/Inventory.h"
#include "domain/TransactionManager.h"
#include "helpers/JsonStore.h"

#include <iostream>
#include <string>
#include <memory>
#include "cli/CLI11.hpp"


class App
{
public:
    App() = default;

    // cheap constructor - only store refs, no IO or parsing
    App(Inventory& inv, TransactionManager& tm, JsonStore& store)
      : 
        m_app{R"(book-cli - A small-but-real system that models products, inentory, 
                    sales transactions and imports files via a CLI.)"
            }
        , m_inv{inv}
        ,m_tm{tm}
        ,m_store(store) 
        {}

    //register all subcommands and callbacks (no parsing)
    void setupCommands();
     // parse argv and run. returns 0 on success, non-zero on error.
    int run(int argc, char** argv);

private:
    CLI::App m_app;
    Inventory m_inv;
    TransactionManager& m_tm;
    JsonStore& m_store;
    
    // small helper structs whose lifetime must live until parse+callback runs
    struct AddProductOptions
    {
        std::string isbn, title, author;
        int quantity;
        float price;
        bool overwrite{false};
    };

    struct SellOptions 
    {
        std::string isbn{};
        int quanitity{};
        float price{};
    };
    struct FilePath 
    {
        std::string file_path{};
    };
   

};


#endif