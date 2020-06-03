/*****************************************************************************************************************************************************************
Name of the module	:postpaid.h
Date of creation	:21/5/2020 
Author of module	:Komal Bankar
Description of module:
This module contains declerations of the class postpaid nd includes class database,prepaid,charging and main_component as friends.

Different function supported in the module:
   char print_plan();						Function to printing all available postpaid recharge plans.
   unsigned long int calculate_tmsi(ull);	Function to calculate TMSI no of new postpaid subscriber.

Revision/Modification History:
Added	 postpaid.h 	Komal Bankar	 21/5/2020.
*****************************************************************************************************************************************************************/

#ifndef __postpaid_h__
#define __postpaid_h__
#include<iostream>
using namespace std;

#include"Database.h"
#include"prepaid.h"
//#include"main.h"
//#include"Tstel.h"
#include"charging.h"

class database;//forward declarations
class prepaid;
class postpaid;
class charging;
//declaration of postpaid class 
class postpaid
{
	private:
	static postpaid *postpaid_obj;
	ull calculate_tmsi(ull);
	char print_plan();
	public:
	static postpaid *get_postpaid_obj(void);
	void new_postpaid(ull , database *,charging *);//declaration of new_postpaid function

	void exist_postpaid(ull,database *, charging *);	//declaration of exist_postpaid function.

   friend class main_component;
   friend class database;
   friend class prepaid;

   friend class charging;

};

postpaid *postpaid::postpaid_obj;

#endif
