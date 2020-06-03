/**********************************************************************************************
Name of the module 	: charging.cpp
Date of creation 	: 21/5/2020
Author of module	: Shubhangi Joshi
Description of module:
This module contains all the function definitions of the class charging

Different functions supported in the module:

	void rchg_cu(long long int msin, struct plan_info p, string str);	Function to store the new subscriber info to database
	void get_bal_cu(long long int msin);								Function for printing balance for prepaid subscribers
	void gen_bill_cu(long long int msin);								Function to generate postpaid bill into file
	static charging *get_cu_obj(void);									Function to create singleton object of charging class

Revision/Modification History:
Added 		charging.cpp		Shubhangi Joshi		21/5/2020
Added		function header		Shubhangi Joshi		26/5/2020
************************************************************************************************/

#include"Tstel.h"

charging *charging::cu_obj;


/*****************************************************************************************************
Function Name: void get_cu_obj(void)

Description: This function is used to create the object for the class charging.
This is a static function which makes sure that only one object for the class charging is created.
This function returns the created object.
*****************************************************************************************************/
charging * charging::get_cu_obj(void)///static function
{
	if(cu_obj == NULL)
	{
		cu_obj = new charging;
	}
	return cu_obj;
}

/*****************************************************************************************************
Function Name: void rchg_cu(ull msin, struct plan_info plan_details, string str,database *db_obj)

Description: The prepaid and postpaid components calls this function when the user requests for new 
prepaid or postpaid connection. This function has the arguments msin, struct plan (which includes imei
number, plan amount and validity), string which identifies whether the msin is prepaid or postpaid and
database class object. This all the information is copied in the struct validity_info along with todays
date and will make a call to rchg_db() function using database class object to store these details to
database.

*****************************************************************************************************/
void charging::rchg_cu(ull msin, struct plan_info plan_details, string str,database *db_obj)
{
	struct validity_info val_info = {0,0,0,0,0,0};
	time_t now = time(0);
	tm *ptr = localtime(&now);							
	val_info.imei = plan_details.imei;
	val_info.amt = plan_details.amt;
	val_info.no_of_days = plan_details.no_of_days;
	val_info.date = ptr->tm_mday;
	val_info.month = 1U + ptr->tm_mon;
	val_info.yr = CAL_BASE + ptr->tm_year;
	db_obj->rchg_db(msin,val_info,str);					//calling databse function
}

/*****************************************************************************************************
Function Name: int calculate_validity(date last_date, date curr_date)

Description: This function is used to calculate no. of days between two dates i.e between today's date
and plan activation date. Two structures dt1 & dt2 holding plan actiation date and today's date resp
are passed as arguments to this function. This function calculates the difference between two dates and
returns the integer difference value. From this function we get that for how many days the plan has
been active.
*****************************************************************************************************/

int charging::calculate_validity(date last_date, date curr_date) 
{
	long int n1 = (last_date.year)*YEAR + last_date.date;  
	for (int i = 0U; i < last_date.month - 1U; i++) 
		n1 += Mdays[i]; 

	long int n2 = curr_date.year*YEAR + curr_date.date; 
	for (int i = 0U; i < curr_date.month - 1U; i++) 
		n2 += Mdays[i]; 

	return (n2 - n1); 
}

/*****************************************************************************************************
Function Name: void rchg_date(int *date, int *month, int *year, int days)

Description: This function is used to calculate next recharge date. Today's date in the form of date,
month and year values and no of days after which recharge is to be done. According to perticular date
month and year conditions next recharge date is calculated.
*****************************************************************************************************/

void charging::rchg_date(int *date, int *month, int *year, int days)
{
	*date = *date + days;
	while(*date > Mdays[*month - 1U])
	{
		*date = *date - Mdays[*month - 1U];
		(*month)++;
		
		if(*month > 12U)
		{
			*month = 1U;
			(*year)++;
		}
	}
}

/*****************************************************************************************************
Function Name: void get_bal_cu(ull msin, database *db_obj, string type_of_sim)

Description: A call to this function has been made by the prepaid component when the existing prepaid
subscriber wants to check the balance and validity. Prepaid component will pass the msin and database
class object as arguments. Current date is stored in date type curr_date variable. A call to 
get_validity_db function is made to get the plan activation date. Calculate_validity function is called
to get no of days between two dates. Next recharge date is calculated using rchg_date function.
Then all the details for the requested subscriber are printed on stdout.
*****************************************************************************************************/

void charging::get_bal_cu(ull msin, database *db_obj, string type_of_sim)
{
	struct date lst_rchg_dt = {0,0,0}, curr_dt = {0,0,0}, nxt_rchg_dt = {0,0,0};
	time_t now=time(0);
	tm *ptr = localtime(&now);								
	curr_dt.date = ptr->tm_mday;
	curr_dt.month = 1U + ptr->tm_mon;
	curr_dt.year = CAL_BASE + ptr->tm_year;

	//Getting data from database//
	struct validity_info *sub_info = db_obj->get_validity_db(msin, type_of_sim);

	if(sub_info == NULL)
	{
		cout<<"MSIN is Invalid \nCan't display balance"<<endl;
	}
	else
	{
		lst_rchg_dt.date = sub_info->date;
		lst_rchg_dt.month = sub_info->month;
		lst_rchg_dt.year = sub_info->yr;

		//getting validity//
		int diff = calculate_validity(lst_rchg_dt,curr_dt);
		
		//getting next recharge date//
		nxt_rchg_dt.date = curr_dt.date;
		nxt_rchg_dt.month = curr_dt.month;
		nxt_rchg_dt.year = curr_dt.year;
		rchg_date(&nxt_rchg_dt.date, &nxt_rchg_dt.month, &nxt_rchg_dt.year, (sub_info->no_of_days)-diff);
				
		//Printig the details on stdout//
		cout<<"MSIN: "<<msin<<endl;
		cout<<"IMEI: "<<sub_info->imei<<endl;
		cout<<"TMSI: "<<sub_info->tmsi<<endl;
		cout<<"Available balance is: "<<sub_info->amt;
		cout<<"\nvalidity is: "<<sub_info->no_of_days-diff<<" days"<<endl;
		cout<<"Next recharge date is: "<<nxt_rchg_dt.date<<"/"<<nxt_rchg_dt.month<<"/"<<nxt_rchg_dt.year<<endl;
	}
}

/*****************************************************************************************************
Function Name: void generate_bill(ull msin,validity_info val_info, string type_of_sim)

Description: This function creates a file called bill.csv and generates a postpaid bill into it. This
bill will have the details as: msin, imei no, bill amount for perticular msin and plan activation date.
*****************************************************************************************************/

int charging :: generate_bill(ull msin, validity_info val_info, string type_of_sim)
{
	struct date from_date = {0,0,0}, to_date = {0,0,0};

	//Calculating from billing date
	from_date.date = 1U;
	if(val_info.month == 1U)
	{
		from_date.month = 12U;
		from_date.year = val_info.yr - 1U;
	}
	else
	{
		from_date.month = val_info.month - 1U;
		from_date.year = val_info.yr;
	}

	//Calculating to billing date	
	if(Mdays[from_date.month - 1U] == 31U)
	{
		to_date.date = 31U;
	}
	else if(Mdays[from_date.month - 1U] == 30U)
	{
		to_date.date = 30U;
	}
	else
	{
		to_date.date = 28U;
	}
	to_date.month = from_date.month;
	to_date.year = from_date.year;

	//Opening file
	ofstream myfile;
	stringstream sstr;
	sstr<<msin;
	string filename=sstr.str();
	filename = filename + "_bill.csv";
	myfile.open(filename);		//creating file//
	if(!myfile)
	{
		cout<<"\nError in Opening file";
		return 0;
	}
	//Storing the data into file//
	myfile<<type_of_sim;
	myfile << " Bill\n";
	myfile << "msin,"<<msin << "\n";
	myfile << "IMEI No.,"<<val_info.imei <<"\n";
	myfile << "TMSI No,"<<val_info.tmsi <<"\n";
	myfile << "Amount,"<<val_info.amt <<" Rs\n";
	myfile << "Date,"<<from_date.date<<"/"<<from_date.month<<"/"<<from_date.year<<" to ";
	myfile << to_date.date<<"/"<<to_date.month<<"/"<<to_date.year;
	myfile.close();
	return 1;
}

/*****************************************************************************************************
Function Name: void gen_bill_cu(ull msin, database *db_obj, string type_of_sim)

Description: A call to this function has been made by the postpaid component when the existing postpaid
subscriber wants to get the bill. Postpaid component will pass the msin and database class object as
arguments. A call to get_bill_db function is made to get the plan activation date. All the data from
databse is copied into struct validity_info. A call is made to the function generate bill to generate
the bill for requested postpaid subscriber. Also the bill amount for the requested subscriber is printed
on stdout.
*****************************************************************************************************/

void charging::gen_bill_cu(ull msin, database *db_obj, string type_of_sim)
{
	//Getting data from database//
	struct validity_info *sub_info = db_obj-> get_bill_db(msin, type_of_sim);
	if(sub_info == NULL)
	{
		cout<<"MSIN is Invalid \nCan't generate bill"<<endl;
	}
	else
	{	
		struct validity_info val_info = {0,0,0,0,0,0};

		//Storing the data//
		val_info.amt = sub_info->amt;
		val_info.date = sub_info->date;
		val_info.imei = sub_info->imei;
		val_info.month = sub_info->month;
		val_info.no_of_days = sub_info->no_of_days;
		val_info.yr = sub_info->yr;
		val_info.tmsi=sub_info->tmsi;

		//Printing the data on stdout//
		cout<<"MSIN: "<<msin<<endl;
		cout<<"Total bill amount for last month is: "<<sub_info->amt<<endl;

		//Generating bill//
		if(generate_bill(msin,val_info,type_of_sim))
		{
			cout<<"\nBill generated\n";
		}
		else
		{
			cout<<"\nBill not generated\n";
		}
	}
}
