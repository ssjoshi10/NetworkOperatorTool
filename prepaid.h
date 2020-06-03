/*****************************************************************************************************************************************************************
i.Name of the module:prepaid.h
ii.Date of creation:21/5/2020 
iii.Author of module:Komal Bankar
iv.Description of module:
This module contains declerations of the class prepaid and includes class database,postpaid,charging and main_component as friends.
vi.Different function supported in the module:
   char print_plan();//Function to printing all available prepaid  recharge plans.
   unsigned long int calculate_tmsi(ull);// Function to calculate TMSI no of new prepaid subscriber.
vi.Revision/Modification History:Added prepaid.h Komal Bankar 21/5/2020.

*****************************************************************************************************************************************************************/
#ifndef __prepaid_h__
#define __prepaid_h__

#include<iostream>
using namespace std;
class database;//forward declarations
class prepaid;
class postpaid;
class charging;
//declaration of prepaid class 
class prepaid
{  
    private:
    static prepaid *prepaid_obj;
	ull calculate_tmsi(ull);
	char print_plan();
	public:
	static prepaid *get_prepaid_obj(void);
	void new_prepaid_subscriber(ull,database *,charging *);//declaration of n_prepaid function.
	void exist_prepaid_subscriber(ull,database *, charging *);//declaration of exist_prepaid function.	
	void gray_prepaid_subscriber(ull,database *, charging *);
friend class main_component;
friend class database;
friend class postpaid;
friend class charging;
};

#endif
