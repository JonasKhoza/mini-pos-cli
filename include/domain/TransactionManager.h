#pragma once

#include "Inventory.h"
#include "helpers/JsonStore.h"
#include "SaleLine.h"

#include <string>

class TransactionManager
{
public:
   // TransactionManager() = default; 
    TransactionManager(Inventory& inv, JsonStore& store);

    //returns pair(success, message or sale_id if success)
   // std::pair<bool, std::string> perform_sale(const std::vector<SaleLine>& items);

    //std::pair<bool, std::string> refund_sale(const std::string& sale_id, double refund_amount);

     // void reconcile_pending_sales();
 Inventory& m_inv;
    JsonStore& m_store;
};

