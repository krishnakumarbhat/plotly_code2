#pragma once
/**
 * Defines various customers
 * @note Maximum of 16 customers(0 to 15) are allowed as per current implementation of DC_GET_VARIANT macro
 */
enum DCCustomer {
   DC_CUSTOMER_BMW = 0,
   DC_CUSTOMER_FORD,
   DC_CUSTOMER_CHANGAN,
   DC_CUSTOMER_RNA,
   DC_CUSTOMER_SCANIA,
   DC_CUSTOMER_NISSAN,
   DC_CUSTOMER_HONDA,
   DC_CUSTOMER_HKMC,
   DC_CUSTOMER_TML,
   DC_CUSTOMER_STLA,
   DC_CUSTOMER_MTNL,
   DC_CUSTOMER_TRATON,
   DC_CUSTOMER_CEER,
   DC_CUSTOMER_GPO,
   DC_CUSTOMER_MAX
};
