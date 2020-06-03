/*********************************************************************
Name of the module	:Database.h
Date of creation	:20/5/2020 
Author of module	:Uppari Kranthi Kumar
Description of module:
This module contains all the declarations of the class database and incudes class main_component, prepaid, postpaid & charging as friends.

Different function supported in the module:

	void print(struct subscriber_info &);		used to print subscriber details on the STDOUT.
	map<ull , subscriber_info>::iterator get_addr(ull , string );
//////////////////////display////////////////////
	void new_display(string );					this function displays the new subscriber details both prepaid and postpaid based on string passed
	void exist_display(string );				this function displays the exist subscriber details both prepaid and postpaid based on string passed
	void gray_display(string );					this function displays the graylist subscriber details both prepaid and postpaid based on string passed
	void black_display();						this function displays the blacklist subscriber details.
/////////////////////write//////////////////////
	void write_list_to_file(ofstream &,map<ull , subscriber_info> &);	inserts the subscriber info to respective list in database class
	void put_to_file(ofstream &,subscriber_info &);						writes the subscriber info to the excel file
///////////////////delete//////////////////
	void delete_subscriber_in_list(string );	helps in deleting the subscriber from the list in database
	void new_delete(string );					helps in deleting the subscriber from the new subscriber list in database
	void exist_delete(string );					helps in deleting the subscriber from the existing subscriber list in database
	void gray_delete(string );					helps in deleting the subscriber from the gray subscriber list in database
	void black_delete();						helps in deleting the subscriber from the blacklist in database
/////////////////read///////////////////////
	void insert_subscriber_in_list(struct subscriber_info , string );	helps in inserting the subscriber to the list in database
	void new_insert(struct subscriber_info &,string );					helps in inserting the subscriber to the new subscriber list in database
	void exist_insert(struct subscriber_info &,string );				helps in inserting the subscriber to the existing subscriber list in database
	void gray_insert(struct subscriber_info &,string );					helps in inserting the subscriber to the gray subscriber list in database
	void black_insert(struct subscriber_info & );						helps in inserting the subscriber to the blacklist in database
///////////////fill structures////////////
	string fill_subscriber_info(subscriber_info &,std::istringstream &);fills the subscriber_info structure before inserting in database
	void fill_validity_info(validity_info &,validity_info &);			fills the destination validity_info structure from source structure
	void fill_plan_info(plan_info &,plan_info &);						fills the destination plan_info structure from source structure


Revision/Modification History:
Added 		Database.h 		U.Kranthi Kumar			20-05-2020
**************************************************************************/





#ifndef __Database_h__
#define __Database_h__

#include<iostream>
#include<sstream>
#include<fstream>
#include<forward_list>
#include<map>

//#include"main.h"
#include"prepaid.h"
#include"postpaid.h"
#include"charging.h"
//#include"Tstel.h"

using namespace std;

class database
{
private:
	static database *db_obj;
	database(){}
	ull msin;
	map<ull , subscriber_info> new_pre,new_post;
	map<ull , subscriber_info> black_list,gray_pre,gray_post;
	map<ull , subscriber_info> exist_pre,exist_post;
	
	void print(struct subscriber_info &);
	map<ull , subscriber_info>::iterator get_addr(ull , string );
//////////////////////display////////////////////
	void new_display(string );
	void exist_display(string );
	void gray_display(string );
	void black_display();
/////////////////////write//////////////////////
	void write_list_to_file(ofstream &,map<ull , subscriber_info> &);
	void put_to_file(ofstream &,subscriber_info &);
///////////////////delete//////////////////
	void delete_subscriber_in_list(string );
	void new_delete(string );
	void exist_delete(string );
	void gray_delete(string );
	void black_delete();
/////////////////read///////////////////////
	void insert_subscriber_in_list(struct subscriber_info , string );
	void new_insert(struct subscriber_info &,string );
	void exist_insert(struct subscriber_info &,string );
	void gray_insert(struct subscriber_info &,string );
	void black_insert(struct subscriber_info & );
///////////////filling structures //////////
	string fill_subscriber_info(subscriber_info &,std::istringstream &);
	void fill_validity_info(validity_info &,validity_info &);
	void fill_plan_info(plan_info &,plan_info &);
////////////////////////////////////////////
public:
	map<ull , subscriber_info> *get_new_list_db(string );
	map<ull , subscriber_info> *get_gb_list_db(string );
	void display_list(string );
	int new_to_exist_db(ull , struct plan_info , string );
	void rchg_db(ull , struct validity_info , string );
	validity_info *get_validity_db(ull, string  );
	validity_info *get_bill_db(ull , string );
	ull get_imsi_db(ull ,string );	
	void read_file(ifstream &);
	void write_file(ofstream &);
	static database *get_db_obj(void);
////////////////////////////////////////////
friend class main_component;
friend class prepaid;
friend class postpaid;
friend class charging;

};

database *database::db_obj;

#endif
