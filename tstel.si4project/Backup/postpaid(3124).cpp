/*****************************************************************************************************************************************************************
Name of the module  :postpaid.cpp
Date of creation    :21/5/2020 
Author of module    :Komal Bankar

Description of module:
This module contains all the function definations of class postpaid.

Different function supported in the module:
   static postpaid*get_postpaid_obj(void);  Function to create singleton object of postpaid class.  
   void new_postpaid(ull);                  Function to get all details of new postpaid subscriber.
   void exist_postpaid(ull);                Function for existing postpaid subscriber.

Revision/Modification History:
Added        postpaid.cpp        Komal Bankar        21/5/2020.
*****************************************************************************************************************************************************************/

#include"postpaid.h"
#include<cstdlib>
#include<ctime>


 postpaid *postpaid::get_postpaid_obj(void){
 // singleton *instance1;
    if(postpaid_obj==NULL)
   		postpaid_obj=new postpaid;
    
    return postpaid_obj;
}

//Function to printing all the available plans for postpaid subscriber//
char postpaid:: print_plan()
{
	char ch;
	cout<<"Plans Available for Postpaid subscriber\n\n"<<endl;
	
	cout<<"Plan A:Unlimited Local/STD calls; \n"<<"10 SMS per day free;Local Re 1/SMS,STD Re 2/SMS;\n"<<"Rs 499 rental.\n\n"<<endl;
	cout<<"Plan B:Unlimited Local/STD calls;\n"<<"Free Unlimited SMS;\n"<<"Rs 749 rental.\n\n"<<endl;
	
    cout<<"******Enter Plan(A/B) which U want to select******\n";
    cin>>ch; 
    return ch;//return the selected plan char//
}
//Function to calculate the tmsi.//
ull postpaid:: calculate_tmsi(ull imsi)
{
    ull num;
    num=imsi/10000000000;
    num=num*100000;
    //Use current time as seed for random genarator
    srand(time(0));
    num+=rand()%90000+10000;//random numbers on every program run
    return num;//return the calculated tmsi number.
}
//defination of new_postpaid function.
void postpaid::new_postpaid(ull msin,database *db_obj,charging *cu_obj)
{
    ull imsi;
    int i=1;
    struct plan_info st;//variable of structure plan_info 
    st.tmsi=0;
    st.imei=0;
    st.plan_type='0';
    st.no_of_days=0;
    st.amt=0;
	imsi=db_obj->get_imsi_db(msin,"postpaid");//call to get_imsi function.
	st.tmsi=calculate_tmsi(imsi);//call to calculate_tmsi function and store returned tmsi no into structure plan_info
  
        
	st.plan_type=print_plan();//call to print_plan function and store the returned char of selected plan into structure plan_info 
	
	
   //initialize the amt and no_of_days data member of structure plan_info according to selected plan
    switch(st.plan_type)
	{
		case 'A':st.amt=499;
		         st.no_of_days=30;
		         break;
		case 'B':st.amt=749;
		         st.no_of_days=30;
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
    if(db_obj->new_to_exist_db(msin,st,"postpaid"))//call this function to send details of subscriber to database.
        {
            cu_obj-> rchg_cu(msin,st,"postpaid",db_obj);//call this function to informs the plan details of subscriber to charging unit for bill generation.
        }
    }
    else
        new_postpaid(msin,db_obj,cu_obj);

}
void postpaid::exist_postpaid(ull msin,database *db_obj,charging *cu_obj)
{
        cu_obj->gen_bill_cu(msin,db_obj);//call this function for bill generation of subscriber.
}	

