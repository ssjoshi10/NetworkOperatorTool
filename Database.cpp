/*********************************************************************
Name of the module	:Database.cpp
Date of creation	:20/5/2020 
Author of module	:Uppari Kranthi Kumar
Description of module:
This module contains all the function definitions of the class database.

Different function supported in the module:
	map<ull , subscriber_info> *get_new_list_db(string );	returns the new subscriber list based on the string 
	map<ull , subscriber_info> *get_gb_list_db(string );	returns the gray subscriber list based on the string 
	void display_list_db(string );							displays list of subscribers with subscriber information based on the string passed.
	int new_to_exist_db(ull , struct plan_info , string );	deletes the subscriber from new subscriber list and adds to the existing subscriber list
	void rchg_db(ull , struct validity_info , string );		updated the validity information of a particulat subscriber
	validity_info *get_validity_db(ull, string  );			returns the validity information of a prepaid subscriber from the database 
	validity_info *get_bill_db(ull , string );				returns the validity information of a postpaid subscriber from the database 
	ull get_imsi_db(ull ,string );							returns imsi from database of a subscriber
	
	static database *get_db_obj(void);						used to create singleton object of database
	void read_file_db(ifstream &);							reads the data from the excel file 
	void write_file_db(ofstream &);							writes the updated data back to the  excel file

Revision/Modification History:
Added 		Database.cpp 		U.Kranthi Kumar			20-05-2020
Added		function header		U.Kranthi Kumar			26-05-2020
**************************************************************************/

#include"Tstel.h"

using namespace std;
database *database::db_obj;//allocating space for singleton object

//////////////////////////////////// Database ///////////////////////////////////

/*******************************************************************************/
/* Function Name:	database * database::get_db_obj(void)       				 */
/* Description:		This function is used to create the singleton object		*/
/******************************************************************************/
database * database::get_db_obj(void)
{
    if(db_obj==NULL)
   		db_obj=new database;

    return db_obj;
}
/*******************************************************************************/
/* Function Name:	ull database::get_imsi_db(ull msin, string type_of_sim)       				 */
/* Description:		This function takes msin and sim_type as input and searches in the database list based on sim_types and return imsi  	*/
/******************************************************************************/
ull database::get_imsi_db(ull msin, string type_of_sim)
{
	auto itr=get_addr(msin, type_of_sim);
	return itr->second.imsi;
}
map<ull , subscriber_info> *database::get_new_list_db(string type_of_sim)
{
	if(type_of_sim=="new/prepaid")
	{
		return &new_pre;		
	}
	else if(type_of_sim=="new/postpaid")
	{
		return &new_post;	
	}
return nullptr;
}
/*******************************************************************************/
/* Function Name:	map<ull , subscriber_info> *database::get_gb_list_db(string type_of_sim)       				 */
/* Description:		This function returns the graylist or blacklist based on type_of_sim 						*/
/******************************************************************************/
map<ull , subscriber_info> *database::get_gb_list_db(string type_of_sim)
{
	if(type_of_sim=="graylist/prepaid")
	{
		return &gray_pre;		
	}
	else if(type_of_sim=="graylist/postpaid")
	{
		return &gray_post;	
	}
	else if (type_of_sim=="blacklist")
	{
		return &black_list;
	}
return nullptr;
}
/*******************************************************************************/
/* Function Name:	void database::read_file_db(ifstream &ifp)        				 */
/* Description:		This function reads excel file using given ifstream and insert the subscriber in the respective list based on type_of_sim	*/
/******************************************************************************/
void database::read_file_db(ifstream &ifp)
{	
	char ch=0;	
	string str="",type_of_sim="";	
	struct subscriber_info subscriber;
	int i=0;
	ifp>>str;
	while(getline(ifp,str))
	{
		istringstream sstr(str);
		type_of_sim=fill_subscriber_info(subscriber,sstr);
		insert_subscriber_in_list(subscriber,type_of_sim);
	}
}
/*******************************************************************************/
/* Function Name:	void database::write_file_db(ofstream &ofp)       				 */
/* Description:		This function combines all the list into one list and insert entire subscriber info on to excel file in a sorted order	*/
/******************************************************************************/
void database::write_file_db(ofstream &ofp)
{
	map<ull , subscriber_info> mymap;
	mymap.insert(new_pre.begin(), new_pre.end());
	mymap.insert(new_post.begin(), new_post.end());
	mymap.insert(exist_pre.begin(), exist_pre.end());
	mymap.insert(exist_post.begin(), exist_post.end());
	mymap.insert(gray_pre.begin(), gray_pre.end());
	mymap.insert(gray_post.begin(), gray_post.end());
	mymap.insert(black_list.begin(), black_list.end());
	write_list_to_file(ofp,mymap);
}
/*******************************************************************************/
/* Function Name:	void database::display_list_db(string type_of_sim)        				 */
/* Description:		This function is used to display the lists based on the type_of_sim	*/
/******************************************************************************/
 void database::display_list_db(string type_of_sim)
 {
 	
 	if(type_of_sim=="new/prepaid" || type_of_sim=="new/postpaid")
 	{
 		new_display(type_of_sim);
 	}
 	else if(type_of_sim=="prepaid" || type_of_sim=="postpaid")
 	{
 		exist_display(type_of_sim);
 	}
 	else if(type_of_sim=="graylist/prepaid" || type_of_sim=="graylist/postpaid")
 	{
 		gray_display(type_of_sim);
 	}
 	else if(type_of_sim=="blacklist")
 	{
 		black_display();
 	}
 }
 /*******************************************************************************/
/* Function Name:	int database::new_to_exist_db(ull msin, struct plan_info plan, string type_of_sim)       				 */
/* Description:		This function will delete the subscriber from new subscriber list and adds the subscriber to the existing subscriber list and return 1 upon success.
					If msin is not present then prints subscriber not fount on STDOUT and returns 0	*/
/******************************************************************************/
int database::new_to_exist_db(ull msin, struct plan_info plan, string type_of_sim)
{
	struct subscriber_info subscriber={};
	string str="";
	str="new/"+type_of_sim;
  if((new_pre.find(msin)!=new_pre.end())||(new_post.find(msin)!=new_post.end()))
 	{
 		fill_plan_info(subscriber,plan);
		this->msin=msin;
		auto itr=get_addr(msin, str);
		subscriber.imsi=itr->second.imsi;
		subscriber.sim_type=type_of_sim;
		delete_subscriber_in_list(str);
		insert_subscriber_in_list(subscriber,type_of_sim);
		return 1;
	}
	 else
	{
		cout<<msin<<" Not found in New subscriber"<<endl;
		return 0;
	}
	return 0;
}
/*******************************************************************************/
/* Function Name:	void database::rchg_db(ull msin, struct validity_info validity, string type_of_sim)        				 */
/* Description:		This function fills the validity_info of the subscriber in the database based on msin and type_of_sim received	*/
/*********************/
void database::rchg_db(ull msin, struct validity_info validity, string type_of_sim)
{
	auto itr=get_addr(msin, type_of_sim );
	fill_validity_info(itr->second,validity);
}
/*******************************************************************************/
/* Function Name:	validity_info * database::get_validity_db(ull msin, string type_of_sim )        				 */
/* Description:		This function checks wheather the given msin is present in data base if present return the validity info of that subscriber	*/
/******************************************************************************/
validity_info * database::get_validity_db(ull msin, string type_of_sim )
{

	auto itr=get_addr(msin, type_of_sim );
	if((itr==new_pre.end())||(itr==new_post.end())||(itr==exist_pre.end())||(itr==exist_post.end()))
 	{
 		return nullptr;
 	}
 	else if(itr==gray_pre.end()||itr==gray_post.end()||itr==black_list.end())
 	{
 		return nullptr;
 	}
	return &itr->second.validity;
}
/*******************************************************************************/
/* Function Name:	validity_info * database::get_bill_db(ull msin, string type_of_sim )        				 */
/* Description:		This function checks wheather the given msin is present in data base if present return the validity info of that subscriber	*/
/******************************************************************************/
validity_info * database::get_bill_db(ull msin, string type_of_sim )
{
	auto itr=get_addr(msin, type_of_sim );
	if((itr==new_pre.end())||(itr==new_post.end())||(itr==exist_pre.end())||(itr==exist_post.end()))
 	{
 		return nullptr;
 	}
 	else if(itr==gray_pre.end()||itr==gray_post.end()||itr==black_list.end())
 	{
 		return nullptr;
 	}
	return &itr->second.validity;
}
/*******************************************************************************/
/* Function Name:	void database::print(struct subscriber_info &subscriber)        				 */
/* Description:		This function is used print the subscriber details on the STDOUT	*/
/******************************************************************************/
void database::print(struct subscriber_info &subscriber)
{
	cout<<"subscriber_info :"<<endl;
	cout<<"IMSI :"<<subscriber.imsi<<", ";
	cout<<"TMSI :"<<subscriber.plan.tmsi<<", ";
	cout<<"IMEI :"<<subscriber.plan.imei<<", ";
	cout<<"PLAN_TYPE :"<<subscriber.plan.plan_type<<", ";
	cout<<"LAST RECHARGE DATE :"<<subscriber.validity.date<<"-";
	cout<<subscriber.validity.month<<"-";
	cout<<subscriber.validity.yr<<", ";
	cout<<"DAYS REMAINING :"<<subscriber.plan.no_of_days<<", ";
	cout<<"BALANCE :"<<subscriber.validity.amt<<", ";
	cout<<"SIM_TYPE:"<<subscriber.sim_type<<endl;
}
//////////////////////////////////////////////////////

///////////////////////// DB_WRITE  //////////////////////////////

/*******************************************************************************/
/* Function Name:	 void database::write_list_to_file(ofstream &ofp,map<ull , subscriber_info> &mymap)        				 */
/* Description:		This function is used to write subscriber info from list on to the file	*/
/******************************************************************************/

void database::write_list_to_file(ofstream &ofp,map<ull , subscriber_info> &mymap)
{
	int i=0;
	ofp<<"S.NO"<<","<<"MSIN"<<","<<"IMSI"<<","<<"TMSI"<<","<<"IMEI"<<","<<"SIM_PLAN"<<",";
	ofp<<"DATE"<<","<<"VALIDITY"<<","<<"BALANCE"<<","<<"TYPE_OF_SIM"<<endl;
	for(auto &itr:mymap)
	{
		ofp<<++i<<",";		
		ofp<<itr.first<<",";
		put_to_file(ofp,itr.second);
	}
}
/*******************************************************************************/
/* Function Name:	void database::put_to_file(ofstream &ofp,subscriber_info &subscriber)        				 */
/* Description:		This function is used to write subscriber info from structure subscriber info on to the file	*/
/******************************************************************************/
void database::put_to_file(ofstream &ofp,subscriber_info &subscriber)
{
	ofp<<subscriber.imsi<<",";
	ofp<<subscriber.plan.tmsi<<",";
	ofp<<subscriber.plan.imei<<",";
	ofp<<subscriber.plan.plan_type<<",";
	ofp<<subscriber.validity.date<<"-"<<subscriber.validity.month<<"-"<<subscriber.validity.yr<<",";
	ofp<<subscriber.plan.no_of_days<<",";
	ofp<<subscriber.validity.amt<<",";
	ofp<<subscriber.sim_type<<endl;
}

/////////////////////////////////////////////////////////////////

/////////////////////////    DB_read        ////////////////////////////

/*******************************************************************************/
/* Function Name:	void database::insert_subscriber_in_list(struct subscriber_info subscriber, string type_of_sim)        				 */
/* Description:		This function is used to insert subscriber_info based on the type_of_sim	*/
/******************************************************************************/
void database::insert_subscriber_in_list(struct subscriber_info subscriber, string type_of_sim)
 {
 	if(type_of_sim=="new/prepaid" || type_of_sim =="new/postpaid")
	{
		new_insert(subscriber,type_of_sim);
	}
	else if(type_of_sim=="prepaid" || type_of_sim =="postpaid")
	{
		exist_insert(subscriber,type_of_sim);
	}
	else if(type_of_sim=="graylist/prepaid" || type_of_sim =="graylist/postpaid")
	{
		gray_insert(subscriber,type_of_sim);
	}
	else if(type_of_sim=="blacklist")
	{
		black_insert(subscriber);
	}
 }
/*******************************************************************************/
/* Function Name:	void database::new_insert(struct subscriber_info &subscriber,string type_of_sim )       				 */
/* Description:		This function is used to insert subscriber_info of new_prepaid or postpaid based on type_of_sim	*/
/******************************************************************************/
void database::new_insert(struct subscriber_info &subscriber,string type_of_sim )
{
	if(type_of_sim=="new/prepaid")
	{
		new_pre.insert(std::make_pair(this->msin,subscriber));
	}
	else if(type_of_sim=="new/postpaid")
	{
		new_post.insert(std::make_pair(this->msin,subscriber));
	}
}
/*******************************************************************************/
/* Function Name:	void database::exist_insert(struct subscriber_info &subscriber,string type_of_sim)        				 */
/* Description:		This function is used to insert subscriber_info of exist_prepaid or postpaid based on type_of_sim	*/
/******************************************************************************/
void database::exist_insert(struct subscriber_info &subscriber,string type_of_sim)
{
	if(type_of_sim=="prepaid")
	{
		exist_pre.insert(std::make_pair(this->msin,subscriber));
	}
	else if(type_of_sim=="postpaid")
	{
		exist_post.insert(std::make_pair(this->msin,subscriber));
	}
}
/*******************************************************************************/
/* Function Name:	void database::gray_insert(struct subscriber_info &subscriber,string type_of_sim)        				 */
/* Description:		This function is used to insert subscriber_info of gray_prepaid or postpaid based on type_of_simt	*/
/******************************************************************************/
void database::gray_insert(struct subscriber_info &subscriber,string type_of_sim)
{
	
	if(type_of_sim=="graylist/prepaid")
	{
		gray_pre.insert(std::make_pair(this->msin,subscriber));
	}
	else if(type_of_sim=="graylist/postpaid")
	{
		gray_post.insert(std::make_pair(this->msin,subscriber));
	}
}
/*******************************************************************************/
/* Function Name:	void database::black_insert(struct subscriber_info &subscriber)        				 */
/* Description:		This function is used to insert subscriber_info of blacklist	*/
/******************************************************************************/
void database::black_insert(struct subscriber_info &subscriber)
{
		black_list.insert(std::make_pair(this->msin,subscriber));
	
}
/////////////////////////////////////////////////////////

/////////////////////////////// DB_display ///////////////////////////
/*******************************************************************************/
/* Function Name:	void database::new_display(string type_of_sim )        				 */
/* Description:		This function is used to displays list of new subscribers(prepaid/postpaid) with subscriber information based on the string passed	*/
/******************************************************************************/
void database::new_display(string type_of_sim )
{
	if(type_of_sim=="new/prepaid")
	{
		for(auto& itr:new_pre)
		{
			cout<<"Ph.No :";
			cout<<itr.first<<" ";
			cout<<endl;
		}
	}
	else if(type_of_sim=="new/postpaid")
	{
		for(auto& itr:new_post)
		{
			cout<<"Ph.No :";
			cout<<itr.first<<" ";
			cout<<endl;
		}
	}
}
/*******************************************************************************/
/* Function Name:	void database::black_display()        				 */
/* Description:		This function is used to displays list of blacklist subscribers with subscriber information	*/
/******************************************************************************/
void database::black_display()
{
	for(auto& itr:black_list)
		{
			cout<<"Ph.No :";
			cout<<itr.first<<" ";
			cout<<endl;
		}

}
/*******************************************************************************/
/* Function Name:	void database::gray_display(string type_of_sim)        				 */
/* Description:		This function is used to displays list of gray subscribers(prepaid/postpaid) with subscriber information based on the string passed	*/
/******************************************************************************/
void database::gray_display(string type_of_sim)
{
	
	if(type_of_sim=="graylist/prepaid")
	{
		for(auto& itr:gray_pre)
		{
			cout<<"Ph.No :";
			cout<<itr.first<<" ";
			cout<<endl;
		}
	}
	else if(type_of_sim=="graylist/postpaid")
	{
		for(auto& itr:gray_post)
		{
			cout<<"Ph.No :";
			cout<<itr.first<<" ";
			cout<<endl;
		}
	}
}
/*******************************************************************************/
/* Function Name:	void database::exist_display(string type_of_sim)        				 */
/* Description:		This function is used to displays list of exist subscribers(prepaid/postpaid) with subscriber information based on the string passed	*/
/******************************************************************************/
void database::exist_display(string type_of_sim)
{
	if(type_of_sim=="prepaid")
	{
		for(auto& itr:exist_pre)
		{
			cout<<"Ph.No :";
			cout<<itr.first<<" ";
			cout<<endl;
		}
	}
	else if(type_of_sim=="postpaid")
	{
		for(auto& itr:exist_post)
		{
			cout<<"Ph.No :";
			cout<<itr.first<<" ";
			cout<<endl;
		}
	}
}

/////////////////////////////////////////////////////////////////// 

//////////////////// database delete ///////////////////////////////

/*******************************************************************************/
/* Function Name:	void database::delete_subscriber_in_list(string type_of_sim)       				 */
/* Description:		This function is used to delete subscriber from the list based on string passed	*/
/******************************************************************************/
void database::delete_subscriber_in_list(string type_of_sim)
 {
 	if(type_of_sim=="new/prepaid" || type_of_sim =="new/postpaid")
	{
		 new_delete(type_of_sim);
	}
	else if(type_of_sim=="prepaid" || type_of_sim =="postpaid")
	{
		 exist_delete(type_of_sim);
	}
	else if(type_of_sim=="graylist/prepaid" || type_of_sim =="graylist/postpaid")
	{
		 gray_delete(type_of_sim);
	}
	else if(type_of_sim=="blacklist")
	{
		 black_delete();
	}

 }
/*******************************************************************************/
/* Function Name:	void database::new_delete(string type_of_sim)        				 */
/* Description:		This function is used to delete new (prepaid/postpaid) subscriber from the list based on string passed	*/
/******************************************************************************/
void database::new_delete(string type_of_sim)
{

	if(type_of_sim=="new/prepaid")
	{
		 new_pre.erase(msin);
	}
	else if(type_of_sim=="new/postpaid")
	{
		 new_post.erase(msin);
	}
}
/*******************************************************************************/
/* Function Name:	void database::exist_delete(string type_of_sim)        				 */
/* Description:		This function is used to delete exist (prepaid/postpaid) subscriber from the list based on string passed	*/
/******************************************************************************/
void database::exist_delete(string type_of_sim)
{
	if(type_of_sim=="prepaid")
	{
		 exist_pre.erase(msin);
	}
	else if(type_of_sim=="postpaid")
	{
		 exist_post.erase(msin);
	}
}
/*******************************************************************************/
/* Function Name:	void database::gray_delete(string type_of_sim)       				 */
/* Description:		This function is used to delete gray (prepaid/postpaid) subscriber from the list based on string passed	*/
/******************************************************************************/
void database::gray_delete(string type_of_sim)
{
	if(type_of_sim=="graylist/prepaid")
	{
		 gray_pre.erase(msin);
	}
	else if(type_of_sim=="graylist/postpaid")
	{
		 gray_post.erase(msin);
	}
}
/*******************************************************************************/
/* Function Name:	void database::black_delete()        				 */
/* Description:		This function is used to delete blacklist subscriber from the list	*/
/******************************************************************************/
void database::black_delete()
{
		 black_list.erase(msin);
	
}
/////////////////////////////////////////////////////

///////////////////filling structures///////////////
/*******************************************************************************/
/* Function Name:	string database::fill_subscriber_info(subscriber_info &subscriber,std::istringstream &sstr)        				 */
/* Description:		This function is used to fill subscriber_info structure from the info received from file	*/
/******************************************************************************/
string database::fill_subscriber_info(subscriber_info &subscriber,std::istringstream &sstr)
{
	string type_of_sim="";
	string str="";
	int i=0;
	char ch=0;//used to remove comma's from istringstream sstr

	sstr>>i>>ch;
	sstr>>this->msin>>ch;
	sstr>>subscriber.imsi>>ch;
	sstr>>subscriber.plan.tmsi>>ch;
	sstr>>subscriber.plan.imei>>ch;	
	sstr>>subscriber.plan.plan_type>>ch;	
	sstr>>subscriber.validity.date>>ch;
	sstr>>subscriber.validity.month>>ch;
	sstr>>subscriber.validity.yr>>ch;
	sstr>>subscriber.plan.no_of_days>>ch;
	sstr>>subscriber.plan.amt>>ch;
	sstr>>type_of_sim;
	subscriber.sim_type=type_of_sim;
	////////////common data between plan_info & validity_info structures/////////////////
	subscriber.validity.amt=subscriber.plan.amt;
	subscriber.validity.tmsi=subscriber.plan.tmsi;
	subscriber.validity.imei=subscriber.plan.imei;
	subscriber.validity.no_of_days=subscriber.plan.no_of_days;
	return type_of_sim;
}
/*******************************************************************************/
/* Function Name:	void database::fill_plan_info(subscriber_info &subscriber,plan_info &plan)        				 */
/* Description:		This function uses  deep copy to fill destination structure with the source plan_info structure 	*/
/******************************************************************************/
void database::fill_plan_info(subscriber_info &subscriber,plan_info &plan)
{
	subscriber.plan.plan_type=plan.plan_type;
	subscriber.plan.amt=plan.amt;
	subscriber.plan.tmsi=plan.tmsi;
	subscriber.plan.imei=plan.imei;
	subscriber.plan.no_of_days=plan.no_of_days;
	////////////common data between plan_info & validity_info structures/////////////////
	subscriber.validity.amt=subscriber.plan.amt;
	subscriber.validity.tmsi=subscriber.plan.tmsi;
	subscriber.validity.imei=subscriber.plan.imei;
	subscriber.validity.no_of_days=subscriber.plan.no_of_days;
	
}
/*******************************************************************************/
/* Function Name:	void database::fill_validity_info(subscriber_info &subscriber, validity_info &validity)        				 */
/* Description:		This function uses  deep copy to fill destination structure with the source validity_info structure	*/
/******************************************************************************/
void database::fill_validity_info(subscriber_info &subscriber, validity_info &validity)
{
	// subscriber.validity.imei=validity.imei;
	// subscriber.validity.amt=validity.amt;
	subscriber.validity.date=validity.date;
	subscriber.validity.month=validity.month;
	subscriber.validity.yr=validity.yr;
	// subscriber.validity.no_of_days=validity.no_of_days;
}

///////////////////////////////////////////////////
/*******************************************************************************/
/* Function Name:	map<ull , subscriber_info>::iterator database::get_addr(ull msin, string type_of_sim)        				 */
/* Description:		This function finds the address of msin in the database lists and returns the address	*/
/******************************************************************************/
map<ull , subscriber_info>::iterator database::get_addr(ull msin, string type_of_sim)
{
	map<ull , subscriber_info>::iterator itr;

	if(type_of_sim=="new/prepaid" || type_of_sim =="new/postpaid")
	{
		if(type_of_sim=="new/prepaid")
		{
			itr=new_pre.find(msin);
		}
		else if(type_of_sim=="new/postpaid")
		{
			itr=new_post.find(msin);
		}
	}
	else if(type_of_sim=="prepaid" || type_of_sim =="postpaid")
	{		
		if(type_of_sim=="prepaid")
		{
			itr=exist_pre.find(msin);
		}
		else if(type_of_sim=="postpaid")
		{
			itr=exist_post.find(msin);
		}
	}
	else if(type_of_sim=="graylist/prepaid" || type_of_sim =="graylist/postpaid")
	{
		if(type_of_sim=="graylist/prepaid")
		{
			itr=gray_pre.find(msin);
		}
		else if(type_of_sim=="graylist/postpaid")
		{
			itr=gray_post.find(msin);
		}
	}
	else if(type_of_sim=="blacklist")
	{
		itr=black_list.find(msin);
	}	
return itr;
}
