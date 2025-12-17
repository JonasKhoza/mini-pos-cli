#include "app.h"
#include "helpers/JsonStore.h"
#include "domain/Inventory.h"
#include "domain/TransactionManager.h"

#include "cli/CLI11.hpp"

#include <iostream>
#include <filesystem>

int main(int argc, char* argv[])
{
    try
    {

        std::filesystem::path exe_path {std::filesystem::canonical(std::filesystem::path{argv[0]})};
        std::filesystem::path main_dir {exe_path.parent_path().parent_path().parent_path() / "data"};
        
        JsonStore store {main_dir};
        auto products_map {store.load_products()};

        Inventory inv;
        inv.load_from_map(products_map);

        TransactionManager tm{inv, store};

        App app{inv, tm, store};
        app.setupCommands(); //register commands (no parsing here)
        return app.run(argc, argv); //parse & execute. returns an exit code
       
    }
    catch(const std::exception& e)
    {
        std::cerr << "Fatal: " << e.what() << '\n';
        return 1;
    }
    
}

