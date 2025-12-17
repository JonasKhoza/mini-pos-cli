#include "domain/Product.h"
#include "helpers/GlobalHelpers.h"

#include <nlohmann/json.hpp>

#include <string>



using json = nlohmann::json;

//converts an instance of Product to json object
inline void to_json(json& j, const Product& p)
{

    std::string created_at {GlobalHelpers::to_string_timestamp(p.created_at)};
    std::string updated_at {GlobalHelpers::to_string_timestamp(p.updated_at)};
    j = json
    {
        {"isbn", p.isbn},
        {"title", p.title},
        {"author", p.author},
        {"quantity", p.quantity},
        {"price", p.price},
        {"created_at", created_at},
        {"updated_at",updated_at}
    };
}

//converts json to an instance of Product
inline void from_json(const json& j, Product& p)
{
    j.at("isbn").get_to(p.isbn);
    j.at("title").get_to(p.title);
    j.at("author").get_to(p.author);
    j.at("quantity").get_to(p.quantity);
    j.at("price").get_to(p.price);

    std::string created_at_str;
    std::string updated_at_str;
    j.at("created_at").get_to(created_at_str);
    j.at("updated_at").get_to(updated_at_str);
    p.created_at = GlobalHelpers::from_string_timestamp(created_at_str);
    p.updated_at = GlobalHelpers::from_string_timestamp(updated_at_str);
}