/*****************************************************************************************************************************************************************
Name of the module  :prepaid.cpp
Date of creation    :21/5/2020 
Author of module    :Komal Bankar
Description of module:
This module contains all the function definations of class prepaid.
Different function supported in the module:
   static prepaid*get_prepaid_obj(void);    Function to create singleton object of prepaid class.  
   void new_prepaid(ull);                   Function to get all details of new prepaid subscriber.
   void exist_prepaid(ull);                 Function for existing prepaid subscriber.

Revision/Modification History:
Added       prepaid.cpp      Komal Bankar      21/5/2020.

*****************************************************************************************************************************************************************/
#include"prepaid.h"
#include<cstdlib>
#include<ctime>

prepaid *prepaid::get_prepaid_obj(void)
{
   // singleton *instance1;
    if(prepaid_obj==NULL)
   		prepaid_obj=new prepaid;
    return prepaid_obj;
}

//function to printing all the available plans for prepaid subscriber//

char prepaid::print_plan()
{
	char ch='0';
	cout<<"Plans Available for Prepaid subscriber\n\n"<<endl;
	cout<<"Plan A(Rs 48):All local calls 2.5p/sec,STD calls 5p/sec;\n"<<"Messages-Local Re 1/SMS,STD Re 1.5/SMS,\n"<<"28 Days Validity.\n\n"<<endl;
	cout<<"Plan B(Rs 249):Unlimited Local/STD calls; \n"<<"100 SMS per day free,\n"<<"28 Days Validity.\n\n"<<endl;
	cout<<"Plan C(Rs 399):Unlimited Local/STD calls;\n"<<"100 SMS per day free,\n"<<"56 Days Validity.\n\n"<<endl;
	cout<<"Plan D(Rs 559):Unlimited Local/STD calls;\n"<<"100 SMS per day free,\n"<<"365 Days Validity.\n\n"<<endl;
    cout<<"******Enter Plan(A/B/C/D) which U want select******\n";
    cin>>ch;
	return ch;//return selected plan char

}
//Function to calculate the tmsi.//
ull prepaid:: calculate_tmsi(ull imsi)
{
    ull num;
    num=imsi/10000000000;
    num=num*100000;
    //Use current time as seed for random genarator
    srand(time(0));
    num+=rand()%90000+10000;//random numbers on every program run
    return num;//return calculated tmsi number.
}
//defination of new_prepaid function.
void prepaid::new_prepaid(ull msin, database *db_obj,charging *cu_obj)
{   
    ull imsi=0;
    int i=1;
    struct plan_info st;//variable of structure plan_info
    st.tmsi=0;
    st.imei=0;
    st.plan_type='0';
    st.no_of_days=0;
    st.amt=0;
    imsi=db_obj->get_imsi_db(msin,"prepaid");//call to get_imsi function.
    st.tmsi=calculate_tmsi(imsi);//call to calculate_tmsi function and store returned tmsi no into structure plan_info
        
    st.plan_type=print_plan();//call to print_plan function and store the returned char of selected plan into structure plan_info 

	//initialize the amt and no_of_days data member of structure plan_info 	according to selected plan
	switch(st.plan_type)
	{
		case 'A':st.amt=48;
		         st.no_of_days=28;
		         break;
		case 'B':st.amt=249;
		         st.no_of_days=28;
		         break;
		case 'C':st.amt=399;
		         st.no_of_days=56;
		         break;
		case 'D':st.amt=559;
		         st.no_of_days=365;
		         break;
        default : 
                    cout<<"/**********Wrong Option please select again***********/"<<endl<<endl;
                    i=0;
    }
    //Reads the imei no from user and store into structure plan_info
    if(i)//checks it option selected is wrong
    {
	cout<<"Enter your IMEI number\n";
    cin>>st.imei;    
          
    if(db_obj->new_to_exist_db(msin,st,"prepaid"))//call this function to send details of subscriber to database.
        {
            cu_obj-> rchg_cu(msin,st,"prepaid",db_obj);//call this function to informs the plan details of subscriber to charging unit for bill generation.
        }
    }
    else
        new_prepaid(msin,db_obj,cu_obj);

}
//defination of existing prepaid function.
void prepaid::exist_prepaid(ull msin,database *db_obj,charging *cu_obj)
{
        cu_obj->get_bal_cu(msin,db_obj);//call this function for bal calculation of subscriber.
}	

