/********************************************************************
Name of the module		: main.cpp
Date of creation 		: 20 May 2020 
Author of module 		: Preksha kondekar

Description of module-
This module contains all the definations of all the functions of  class main_component

Different function supported in the module:
void print_menu();      		This function display main menu
void print_menu_new();  		This function display New subsriber menu
void print_menu_existing(); 	This function display Existing subsriber menu
void print_menu_black_gray();   This function display Gray/black subsriber menu
void black_gray_subscriber(database *,charging *);   					This function perform operation related to the gray/black list subscriber.According to user choice it will displays the Gray/black subscriber list and pass  MSIN to the prepaid and postpaid components based on user choice
void new_subscriber(database *,prepaid *,postpaid *,charging *);  		This function perform operation related to the new subscriber menu displays the new subscriber MSIN list for prepaid and postpaid and pass that MSIN to the prepaid and postpaid components based on user choice
void existing_subscriber(database *,prepaid *,postpaid *,charging *);  	This function perform operation related to the existing subscriber menu displays the existing subscriber MSIN list for prepaid and postpaid and pass that MSIN to the prepaid and postpaid components based on user choice
void main_menu(main_component *,database *,prepaid *,postpaid *,charging *);  This is the main function which calls all other classes and passes reqiured objects.

Revision/modification History
Added   	main.cpp		Preksha kondekar		20-05-2020

*********************************************************************/


#include "Tstel.h"

#include<iostream>

using namespace std;
void main_component::main_menu(main_component *main_obj,database *db_obj,prepaid *prepaid_obj,postpaid *postpaid_obj,charging *cu_obj,barred *barred_obj)
{

	char choice;
	while(1)
	{
		main_obj->print_menu();//Display main menu
		cout<<"Enter your choice"<<endl;
		cin>>choice;
		switch(choice)
		{
			case '1':
			main_obj->new_subscriber(main_obj,db_obj,prepaid_obj,postpaid_obj,cu_obj);//perform the operation related to the new subsriber
			break;


			case '2':
			main_obj->existing_subscriber(main_obj,db_obj,prepaid_obj,postpaid_obj,cu_obj);//perform the operation related to the existing subsriber
			break;

			case '3':
			barred_obj->black_gray_subscriber(db_obj,prepaid_obj,postpaid_obj,cu_obj,barred_obj);//perform the operation related to the gray black list subsriber
			break;

			case '*':
			return ;
            default :cout<<"Wrong choice is entered please enter correct choice"<<endl;
            break;
		}
	}
////////////////////////////////////////////////////////////
}

//Display main menu
void main_component::print_menu()
{
	cout<<"******************************************************************************************"<<endl;
	cout<<"***                        TSTEL NETWORK OPERATOR TOOL                              ***"<<endl;
	cout<<"******************************************************************************************"<<endl;
	cout<<endl;
	cout<<"                                 Main Menu                                    "<<endl;
	cout<<endl;
	cout<<"                             1. New Subscriber                                  "<<endl;
	cout<<"                             2. Existing Subsriber                              "<<endl;
	cout<<"                             3. Interception/barred list                        "<<endl;
	cout<<"                             *. Exit                                            "<<endl;
}


//Display new subcsriber menu
void main_component::print_menu_new()
{
	cout<<"******************************************************************************************"<<endl;
	cout<<"***                        TSTEL NETWORK OPERATOR TOOL                              ***"<<endl;
	cout<<"******************************************************************************************"<<endl;
	cout<<endl;
	cout<<"                             New Subscriber Menu                                    "<<endl;
	cout<<endl;
	cout<<"                             1. Postpaid                                   "<<endl;
	cout<<"                             2. Prepaid                                    "<<endl;
	cout<<"                             *. Back to Main Menu                          "<<endl;
}

//Display Existing menu
void main_component::print_menu_existing()
{
	cout<<"******************************************************************************************"<<endl;
	cout<<"***                        TSTEL NETWORK OPERATOR TOOL                                 ***"<<endl;
	cout<<"******************************************************************************************"<<endl;
	cout<<endl;
	cout<<"                           Existing Subscriber Menu                                  "<<endl;
	cout<<endl;
	cout<<"                             1. Postpaid                                   "<<endl;
	cout<<"                             2. Prepaid                                    "<<endl;
	cout<<"                             *. Back to Main Menu                          "<<endl;
}

// perform operation related to existing subscriber
void main_component::existing_subscriber(main_component *main_obj,database *db_obj,prepaid *prepaid_obj,postpaid *postpaid_obj,charging *cu_obj)
{
	while(1)
	{
		unsigned long long int msin=0;
		char choice_existing;
		int i=0,flag=0;
		main_obj->print_menu_existing();//Display existing subcriber menu/////////////changed
		cout<<"Enter your choice"<<endl;
		cin>>choice_existing;
		switch(choice_existing)
		{
			case '1':

			 db_obj->display_list_db("postpaid");//Display postpaid list of existing subscriber 
			cout<<"Enter your selected MSIN from obove existing list"<<endl;
			cin>>msin;
	                //used for passing  existing MSIN to postpaid component
			postpaid_obj->exist_postpaid_subscriber(msin,db_obj,cu_obj);
			break;

			case '2':

			db_obj->display_list_db("prepaid");//Display prepaid list of existing subscriber
			cout<<"Enter your selected MSIN from obove existing list"<<endl;
			cin>>msin;
	                //used for passing existing MSIN to prepaid component
			prepaid_obj->exist_prepaid_subscriber(msin,db_obj,cu_obj);
			break;

			case '*':
			flag=1;
			break;
	               
		        default:cout<<"Wrong choice is entered please enter correct choice"<<endl;
	                break;
		}
		if(flag==1)
		break;
	}
}

// perform operation on new subscriber
void main_component ::new_subscriber(main_component *main_obj,database *db_obj, prepaid *prepaid_obj,postpaid *postpaid_obj,charging *cu_obj)
{
	while(1)
	{
		unsigned long long int msin=0;
		char choice_new;
		int i=0,flag=0;
		main_obj->print_menu_new();//Display new subcriber menu//////////////////////changed
		cout<<"Enter your choice"<<endl;
		cin>>choice_new;
		switch(choice_new)
		{
			case '1':
			db_obj->display_list_db("new/postpaid");//Display postpaid MSIN list of new 
			cout<<"Enter your selected MSIN from obove list"<<endl;
			cin>>msin;


	                //used for passing MSIN of new subscriber to postpaid component
			postpaid_obj->new_postpaid_subscriber(msin,db_obj,cu_obj);
			break;

			case '2':
			db_obj->display_list_db("new/prepaid");
			cout<<"Enter your selected MSIN from obove list"<<endl;
			cin>>msin;

	                //used for passing MSIN of new subscriber to prepaid component
			prepaid_obj->new_prepaid_subscriber(msin,db_obj,cu_obj);
			break;
			case '*':
			flag=1;
			break;
	                
	        default :cout<<"Wrong choice is entered please enter correct choice"<<endl;
	        break;
		}
		if(flag==1)
		break;
	}

}
