/**********************************************
Name of the module		: Interception_Barred.h
Date of creation 		: 20 May 2020 
Author of module 		: Preksha kondekar

Description of module-
This module contains all the declarations of the class barred 

Revision/modification History
Added  		 Interception_Barred.h 		Preksha kondekar		20-05-2020
***********************************************/


#ifndef __Interception_barred_h__
#define __Interception_barred_h__

class database;//forward declarations
class prepaid;
class postpaid;
class charging;
class barred
{
public:

	void print_menu_black_gray();
	void gray_list_menu();
	void black_gray_subscriber(database *,prepaid *,postpaid *,charging *,barred *);
};

#endif
