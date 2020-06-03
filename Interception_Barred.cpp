/********************************************************************
Name of the module		: Interception_Barred.cpp
Date of creation 		: 20 May 2020 
Author of module 		: Preksha kondekar

Description of module-
This module contains all the definations of all the functions of  class barred

Different function supported in the module:

void print_menu_black_gray();  Display black/gray list

void gray_list_menu();   Display gray list menu 

void black_gray_subscriber(database *,prepaid *,postpaid *,charging *);      This function has objects of database, charging class as function parameter and return type is void. This function perform operation related to the gray/black list subscriber.gray/black list subcriber menu displays on stdout user choose one out of them .If selected option is blacklist it will display blacklist MSIN list on stdout.If selected option is graylist it will again ask for user choice graylist(prepaid/postpaid). display graylist MSIN (prepaid/postpaid)list on stdout .user choose one MSIN from list  and  pass that MSIN to the prepaid and postpaid components

Revision/modification History
Added   	Interception_Barred.cpp		Preksha kondekar		20-05-2020

*********************************************************************/




#include "Tstel.h"

//Display black/gray list
void barred::print_menu_black_gray()
{
	cout<<"******************************************************************************************"<<endl;
	cout<<"***                         TSTEL NETWORK OPERATOR TOOL                                ***"<<endl;
	cout<<"******************************************************************************************"<<endl;
	cout<<endl;
	cout<<"                          Interception/Barred Subscriber Menu                   "<<endl;
	cout<<endl;
	cout<<"                             1. Blacklist(Barred)                          "<<endl;
	cout<<"                             2. Greylist(Interception)                     "<<endl;
	cout<<"                             *. Back to Main Menu                          "<<endl;
}

//Display gray list menu as follow
void barred::gray_list_menu()
{
cout<<"******************************************************************************************"<<endl;
cout<<"***                         TSTEL NETWORK OPERATOR TOOL                              ***"<<endl;
cout<<"******************************************************************************************"<<endl;
cout<<endl;
cout<<"                                   Graylist Menu                                       "<<endl;
cout<<endl;
cout<<"                                 1.Postpaid list                                        "<<endl;
cout<<"                                 2.Prepaid list                                         "<<endl;
cout<<"                                 *.Back to main menu                                         "<<endl;
}

/*
*********************void black_gray_subscriber(database *,charging *); ****************************
This function has objects of database, charging class as function parameter and return type is void
This function perform operation related to the gray/black list subscriber.gray/black list subcriber menu displays on stdout user choose one out of them .If selected option is blacklist it will display blacklist MSIN list on stdout.If selected option is graylist it will again ask for user choice graylist(prepaid/postpaid). display graylist MSIN (prepaid/postpaid)list on stdout .user choose one MSIN from list  and  pass that MSIN to the prepaid and postpaid components
*****************************************************************************************************/
//void barred::black_gray_subscriber(database *db_obj,charging *cu_obj)
void barred::black_gray_subscriber(database *db_obj,prepaid *prepaid_obj,postpaid *postpaid_obj,charging *cu_obj,barred *barred_obj)
{
	while(1)
	{
		int i=0,flag1=0;
		char b_g_choice=0;
		unsigned long long int msin=0;
		barred_obj->print_menu_black_gray();//Display Interception/barred menu
		cout<<"Enter your choice"<<endl;
		cin>>b_g_choice;
		switch(b_g_choice)
		{
			case '1':
				db_obj->display_list_db("blacklist");//display the MSIN list of blacklist subscriber
				cout<<"You cant Generate bill or get balance for blacklist subscriber"<<endl;
				break;

			case '2':
				while(1)
				{
					gray_list_menu();
					cout<<"Enter your choice_new"<<endl;
					char choice_g=0;
					int flag=0;
					cin>>choice_g;
					switch(choice_g)
					{
						case '1':
			        //display list of  MSIN for graylist/postpaid subscriber
						db_obj->display_list_db("graylist/postpaid");
						cout<<"Enter your selected MSIN from obove existing list"<<endl;
						cin>>msin;

				        postpaid_obj->gray_postpaid_subscriber(msin,db_obj,cu_obj);//pass the existing MSIN to the postpaid components 
				        break;
				        case '2':
						db_obj->display_list_db("graylist/prepaid");//display list of MSIN for graylist of prepaid subsriber
						cout<<"Enter your selected MSIN from obove existing list"<<endl;
						cin>>msin;
						prepaid_obj->gray_prepaid_subscriber(msin,db_obj,cu_obj);//pass MSIN of existing subscriber to the prepaid component
						break;
						case '*':
						flag=1;
						break;
					}
					if(flag==1)
					break;
				}

			case '*':
			flag1=1;
			break;
			default :cout<<"Wrong choice is entered please enter correct choice"<<endl;
			break;
		}
		if(flag1==1)
		break;
	}
}