/**********************************************
Name of the module		: main.h
Date of creation 		: 20 May 2020 
Author of module 		: Preksha kondekar

Description of module-
This module contains all the declarations of the class main_component and incudes class database, class prepaid,class postpaid,class charging as friend.

Revision/modification History
Added  		 main.h 		Preksha kondekar		20-05-2020
***********************************************/

#ifndef __main_h__
#define __main_h__
#include<iostream>
using namespace std;

#include<iostream>


class database;//forward declarations
class prepaid;
class postpaid;
class charging;
class barred;
class main_component
{
public:
void print_menu();
void print_menu_new();
void new_subscriber(main_component *,database *,prepaid *,postpaid *,charging *);
void print_menu_existing();
void existing_subscriber(main_component *,database *,prepaid *,postpaid *,charging *);
void print_menu_black_gray();
void main_menu(main_component *,database *,prepaid *,postpaid *,charging *, barred *);
};

#endif
