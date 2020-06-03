#include "Tstel.h"
void barred::black_gray_subscriber(database *db_obj,prepaid *prepaid_obj,postpaid *postpaid_obj,charging *cu_obj)
{
	cout<<"Enter your choice"<<endl;
	int i=0;
	char b_g_choice;
	unsigned long long int msin=0;
	cin>>b_g_choice;
	switch(b_g_choice)
	{
		case '1':
		db_obj->display_list_db("blacklist");//display the MSIN list of blacklist subscriber
		cout<<"You cant Generate bill or get balance for blacklist subscriber"<<endl;
		break;

		case '2':
		cout<<"******************************************************************************************"<<endl;
		cout<<"***                         TSTEL NETWORK OPERATOR TOOL                              ***"<<endl;
		cout<<"******************************************************************************************"<<endl;
		cout<<endl;
		cout<<"                                   Graylist Menu                                       "<<endl;
		cout<<endl;
		cout<<"                                 1.Postpaid list                                        "<<endl;
		cout<<"                                 2.Prepaid list                                         "<<endl;
		cout<<"Enter your choice_new"<<endl;
		char choice_g;
		cin>>choice_g;
		switch(choice_g)
		{
			case '1':
        	//display list of  MSIN for graylist/postpaid subscriber
			db_obj->display_list_db("graylist/postpaid");
			cout<<"Enter your selected MSIN from above existing list"<<endl;
			cin>>msin;
                //used for passing  existing MSIN to postpaid component
			postpaid_obj->gray_postpaid_subscriber(msin,db_obj,cu_obj); 
			break;

			case '2':
			db_obj->display_list_db("graylist/prepaid");//display list of MSIN for graylist of prepaid subsriber
			cout<<"Enter your selected MSIN from above existing list"<<endl;
			cin>>msin;
                //used for passing  existing MSIN to postpaid component
			prepaid_obj->gray_prepaid_subscriber(msin,db_obj,cu_obj); 
			break;
			default :cout<<"Wrong choice is entered please enter correct choice"<<endl;
		}
			case '*':
			break;
			default :cout<<"Wrong choice is entered please enter correct choice"<<endl;
			break;
	}
}
