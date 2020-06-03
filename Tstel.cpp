/*********************************************************************
Name of the module	:Tstel.cpp
Date of creation	:23/5/2020 
Author of module	:Uppari Kranthi Kumar

Description of module:
This module contains the main() function required to run Not Operator Tool,creates the objects for all components  ,
reads the data from the "Tstel.csv" file and Writes the Data on to the Excel file.

Revision/Modification History:
Added 		Tstel.cpp 		U.Kranthi Kumar			23-05-2020
**************************************************************************/
#include"Tstel.h"
using namespace std;
int main(int argc, char *argv[], char * envp[])
{
	database *db_obj=database::get_db_obj();
	prepaid *prepaid_obj=prepaid::get_prepaid_obj();
	postpaid *postpaid_obj=postpaid::get_postpaid_obj();
	charging *cu_obj=charging::get_cu_obj();
	main_component *main_obj=new main_component;
	barred *barred_obj=new barred;
	ifstream ifp;
	ofstream ofp;
	string str;
////////////////reading from excel file////////////////
	ifp.open("Tstel_DB.csv");
	if(!ifp)
	{
		cout<<"Error in opening file";
		return 0;
	}
	db_obj->read_file_db(ifp);
	ifp.close();

main_obj->main_menu(main_obj,db_obj,prepaid_obj,postpaid_obj,cu_obj,barred_obj);
	
////////////////writing to excel file//////////////
ofp.open("Temp.csv");
	if(!ofp)
	{
		cout<<"Error in Writting to file "<<endl;
		return 0;
	}
	db_obj->write_file_db(ofp);	
	cout<<"Excel sheet generated as Temp.csv"<<endl;
ofp.close();

}
