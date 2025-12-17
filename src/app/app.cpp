#include "app.h"
#include "errors/Error.h"
#include "domain/Product.h"
#include "helpers/GlobalHelpers.h"

#include <iostream>
#include <memory> //for std::unique_ptr
#include <string>



void App::setupCommands()
{
     
     //Add-product
     auto add = m_app.add_subcommand("add-product", "Add a product to inventory.");
     auto opts = std::make_shared<AddProductOptions>(); //keep alive inside lambda

     add->add_option("--isbn", opts->isbn, "isbn")->required();
     add->add_option("--title", opts->title, "title");
     add->add_option("--author", (*opts).author, "author");
     add->add_option("--price", opts->price, "Price in decimal (e.g., 19.99)")->required();
     add->add_option("--quantity", opts->quantity, "quantity");
     add->add_option("--overwrite", opts->overwrite, "Overwrite if isbn exists"); 

     // register callback. CLI11 will call this after parsing when this subcommand matches.
     add->callback([this, opts]()
     {
               try
               {
                    Product product{opts->isbn, opts->title, opts->author, opts->quantity, opts->price};

                    //attempts creating a product if not existing, or overwriting if exists
                    bool isProductAdded = m_inv.add_product(product, opts->overwrite);

                    if(!isProductAdded && !opts->overwrite)
                    {
                         std::cerr<< "Product already exists. Use --overwrite to replace.\n";
                         std::exit(4);
                    }

                    //If overwrite: invoke an update path (add_product)
                    if(!isProductAdded && opts->overwrite) 
                    {
                         std::cerr<< "An unknown error occured, product failed to update.\n";
                         std::exit(4);
                    }
                    
                    //persist products 
                    auto products_map {m_inv.dump_to_map()};
                    m_store.save_products(products_map);

                    std::cout<<"Product added/updated: "<<opts->isbn<<"\n";
               }
               catch(const std::exception& e)
               {
                    std::cerr << "Error adding product: " << e.what() << '\n';
                    std::exit(2);
               }
               
     });


     //List all existing products
     auto list_products = m_app.add_subcommand("list-products", "Lists existing created products.");
     list_products->callback([this](){
          try
          {
                auto products = m_inv.list_all();

               // Column widths
               const int col_isbn = 15;
               const int col_title = 95;
               const int col_author = 25;
               const int col_qty = 10;
               const int col_price = 10;
               const int col_created = 25;
               const int col_updated = 25;

               std::cout << "All available products\n\n";

               std::cout 
                    << std::left
                    << std::setw(col_isbn)   << "ISBN"
                    << std::setw(col_title)  << "TITLE"
                    << std::setw(col_author) << "AUTHOR"
                    << std::setw(col_qty)    << "QTY"
                    << std::setw(col_price)  << "PRICE"
                    << std::setw(col_created)<< "CREATED_AT"
                    << std::setw(col_updated)<< "UPDATED_AT"
                    << "\n";

               std::cout << std::string(200, '-') << "\n";

               for (const auto& product : products)
               {
                    std::cout
                         << std::left
                         << std::setw(col_isbn)   << product.isbn
                         << std::setw(col_title)  << product.title
                         << std::setw(col_author) << product.author
                         << std::setw(col_qty)    << product.quantity
                         << std::setw(col_price)  << product.price
                         << std::setw(col_created)<< GlobalHelpers::to_string_timestamp(product.created_at)
                         << std::setw(col_updated)<< GlobalHelpers::to_string_timestamp(product.updated_at)
                         << "\n";
               }
               }
          catch(const std::exception& e)
          {
               std::cerr << "Error listing products: " << e.what() << '\n';
               std::exit(2);
          }
          
     });

     //   //Sell-product
     // auto sell = m_app.add_subcommand("sell", "Sell an item.");
     // auto sellOpts = std::make_shared<SellOptions>(); //keep alive inside lambda

     // add->callback([this, sellOpts](){
     //      try
     //      {
     //           SaleLine line{sellOpts->isbn, sellOpts->quanitity, sellOpts->price};
     //           auto [ok, result] = m_tm.perform_sale({line});

     //           if(!ok) 
     //           {
     //                std::cerr << "Sale failed: "<< result << "\n";
     //                std::exit(2);
     //           }

     //           std::cout << "Sale completed, id=" << result << "\n";
     //      }
     //      catch(const std::exception& e)
     //      {
     //           std::cerr << "Error: " << e.what() << '\n';
     //           std::exit(1);
     //      }
          
     // });


     // auto import_products_csv = m_app.add_subcommand("import-csv", "Create products in bulk using a comma delimited csv file.");
     // auto file_path_opts = std::make_shared<FilePath>();
     // import_products_csv->add_option("--file", file_path_opts->file_path, "Takes a full path to a file.");

     // import_products_csv->callback([this](){
          
     // });
}

int App::run(int argc, char** argv)
{
     //Optional: call m_app.require_subcommand(1) to force at least one subcommand
     try
     {
          //CLI::App allows to call ensure_utf8 on argv if needed before parse.
          argv = m_app.ensure_utf8(argv); //this can throw - but we catch below
          CLI11_PARSE(m_app, argc, argv); //will run the matched subcommand callback

          return 0;
     }
     catch(const CLI::Error& e)
     {
          //CLI11 throws nicely typed errors (prints help, etc...)
          std::cerr<< e.what() <<"\n";
          return e.get_exit_code();
     }
     catch(const std::exception& e)
     {
          //domain or IO exception
          std::cerr << "Unhandled error: " << e.what() << '\n';
          return 1;
     }
     
}

