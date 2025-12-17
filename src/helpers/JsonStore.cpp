#include "helpers/JsonStore.h"
#include "errors/Error.h"
#include "domain/Product.h"
#include "helpers/json_serializers.h"

#include <unordered_map>
#include <string>
#include <fstream>
#include <memory>
#include <utility>


 JsonStore::JsonStore(const FilePath_T& data_dir)
 :m_data_dir{data_dir}
 {
    ensure_parent_exists();
    this->m_products_path = m_data_dir / "products.json";
    this->m_sales_dir = m_data_dir / "sales.json";
 }

void JsonStore::ensure_parent_exists()
{
    std::error_code ec;
 
    if(!std::filesystem::exists(m_data_dir))
        std::filesystem::create_directory(m_data_dir, ec);

    if (ec)
        throw Error("Failed to create directory: " + ec.message());
    
}

// Load entire products.json as a map<isbn,Product>
// If file missing -> return empty map
std::unordered_map<std::string, Product> JsonStore::load_products()
{

    std::unordered_map<std::string, Product> existing_products;

    if (!std::filesystem::exists(m_products_path)) return existing_products;

    std::ifstream ifs {m_products_path};

    if (!ifs) throw Error("Failed to open products.json");

    json j;

    //read all json data(single json object) from the stream into j
    ifs >> j;

    if (!j.is_object()) throw Error("products.json is not an onject.");

    for (auto it{j.begin()}; it != j.end(); ++it)
    {
       
        Product product{};
        //write the json into product
        from_json(it.value(), product);

        std::string isbn  {product.isbn};
        existing_products.emplace(isbn, std::move(product)); //need to confirm the order of creation because move sematics might be triggered first

    }

    return existing_products;

}

void JsonStore::save_products(const std::unordered_map<std::string, Product>& products)
{

    json json_products = json::object();

    for (const auto& [isbn, p] : products)
    {
        json j;
        to_json(j, p); //takes both by reference, creayes j as an out-parameter
        json_products[isbn] = j;
    }

    std::string data {json_products.dump(4)}; // pretty-print for readability (or dump() for compact)
    //atomic write
    atomic_write(m_products_path, data);
}


//Needs to be re-written using low-level code(platform specific) durability
void JsonStore::atomic_write(const FilePath_T& dest, const std::string& data)
{

    std::filesystem::path temp {dest};
    temp += ".tmp";

    std::ofstream outs{temp, std::ios::binary | std::ios::trunc};
    if (!outs)
            throw Error("Failed to open temp file for automic write.");

    outs << data;
    outs.flush(); //does not guarantee that data reached physical disk, needs low-level platform code for that

    if (!outs) throw Error("Failed to write to temp file.");

    //explicitly close the temp file after flushing, so rename does not try to work on an open filename an open file
    outs.close();

    if (!outs) throw Error("Failed to close temp file after write.");

    std::error_code ec;
    // rename => on POSIX this is atomic and replaces dest
    std::filesystem::rename(temp, dest, ec);

    if (ec)
    {
        #ifdef _WIN32
        // On Windows, try replace via remove + rename (or call ReplaceFile WinAPI for true atomic replace)
            std::filesystem::remove(dest, ec);

            if (ec) 
            {
                throw Error("Failed to remove dest before rename: " + ec.message());
            }
           
            std::filesystem::rename(temp, dest, ec);
        if (ec) throw Error("Atomic rename failed after removing dest: " + ec.message());
        #else
                throw Error("Atomic rename failed: " + ec.message());
        #endif
    }
   
}