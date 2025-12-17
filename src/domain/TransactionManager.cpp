#include "domain/TransactionManager.h"

TransactionManager::TransactionManager(Inventory& inv, JsonStore& store) 
: m_inv{inv}
, m_store{store}
{

};