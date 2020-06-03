/*********************************************************************
Name of the module	:Database.cpp
Date of creation	:20/5/2020 
Author of module	:Uppari Kranthi Kumar
Description of module:
This module contains all the function definitions of the class database.

Different function supported in the module:
	map<ull , subscriber_info> *get_new_list_db(string );	returns the new subscriber list based on the string 
	map<ull , subscriber_info> *get_gb_list_db(string );	returns the gray subscriber list based on the string 
	void display_list(string );								displays list of subscribers based on the string passed.
	int new_to_exist_db(ull , struct plan_info , string );	deletes the subscriber from new subscriber list and adds to the existing subscriber list
	void rchg_db(ull , struct validity_info , string );		updated the validity information of a particulat subscriber
	validity_info *get_validity_db(ull, string  );			returns the validity information of a prepaid subscriber from the database 
	validity_info *get_bill_db(ull , string );				returns the validity information of a postpaid subscriber from the database 
	ull get_imsi_db(ull ,string );							returns imsi from database of a subscriber
	
	static database *get_db_obj(void);						use to create singleton object of database
	void read_file(ifstream &);								reads the data from the excel file 
	void write_file(ofstream &);							writes the updated data back to the  excel file

Revision/Modification History:
Added 		Database.cpp 		U.Kranthi Kumar			20-05-2020
**************************************************************************/


#include"Database.h"
using namespace std;

//////////////////////////////////// Database ///////////////////////////////////

database * database::get_db_obj(void)
{
    if(db_obj==NULL)
   		db_obj=new database;

    return db_obj;
}
ull database::get_imsi_db(ull , string type_of_sim)
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
void database::read_file(ifstream &ifp)
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
void database::write_file(ofstream &ofp)
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
 void database::display_list(string type_of_sim)
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
int database::new_to_exist_db(ull msin, struct plan_info plan, string type_of_sim)
{
	struct subscriber_info subscriber={};
	string str="";
	str="new/"+type_of_sim;
  if((new_pre.find(msin)!=new_pre.end())||(new_post.find(msin)!=new_post.end()))
 	{
 		fill_plan_info(subscriber.plan,plan);
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
void database::rchg_db(ull msin, struct validity_info validity, string type_of_sim)
{
	auto itr=get_addr(msin, type_of_sim );
	fill_validity_info(itr->second.validity,validity);
}

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
void database::black_insert(struct subscriber_info &subscriber)
{
		black_list.insert(std::make_pair(this->msin,subscriber));
	
}
/////////////////////////////////////////////////////////

/////////////////////////////// DB_display ///////////////////////////
void database::new_display(string type_of_sim )
{
	if(type_of_sim=="new/prepaid")
	{
		for(auto& itr:new_pre)
		{
			cout<<"Ph.No :";
			cout<<itr.first<<" ";
			print(itr.second);
		}
	}
	else if(type_of_sim=="new/postpaid")
	{
		for(auto& itr:new_post)
		{
			cout<<"Ph.No :";
			cout<<itr.first<<" ";
			print(itr.second);
		}
	}
}
void database::black_display()
{
	for(auto& itr:black_list)
		{
			cout<<"Ph.No :";
			cout<<itr.first<<" ";
			print(itr.second);
		}

}

void database::gray_display(string type_of_sim)
{
	
	if(type_of_sim=="graylist/prepaid")
	{
		for(auto& itr:gray_pre)
		{
			cout<<"Ph.No :";
			cout<<itr.first<<" ";
			print(itr.second);
		}
	}
	else if(type_of_sim=="graylist/postpaid")
	{
		for(auto& itr:gray_post)
		{
			cout<<"Ph.No :";
			cout<<itr.first<<" ";
			print(itr.second);
		}
	}
}

void database::exist_display(string type_of_sim)
{
	if(type_of_sim=="prepaid")
	{
		for(auto& itr:exist_pre)
		{
			cout<<"Ph.No :";
			cout<<itr.first<<" ";
			print(itr.second);
		}
	}
	else if(type_of_sim=="postpaid")
	{
		for(auto& itr:exist_post)
		{
			cout<<"Ph.No :";
			cout<<itr.first<<" ";
			print(itr.second);
		}
	}
}

/////////////////////////////////////////////////////////////////// 

//////////////////// database delete ///////////////////////////////
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
void database::black_delete()
{
		 black_list.erase(msin);
	
}
/////////////////////////////////////////////////////

///////////////////filling structures///////////////
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
	subscriber.validity.no_of_days=subscriber.plan.no_of_days;
	sstr>>subscriber.validity.amt>>ch;
	sstr>>type_of_sim;
	subscriber.sim_type=type_of_sim;
	return type_of_sim;
}
void database::fill_plan_info(plan_info &dest,plan_info &src)
{
	dest.amt=src.amt;
	dest.plan_type=src.plan_type;
	dest.no_of_days=src.no_of_days;
	dest.tmsi=src.tmsi;
	dest.imei=src.imei;
}
void database::fill_validity_info(validity_info &dest, validity_info &src)
{
	dest.imei=src.imei;
	dest.amt=src.amt;
	dest.date=src.date;
	dest.month=src.month;
	dest.yr=src.yr;
	dest.no_of_days=src.no_of_days;
}

///////////////////////////////////////////////////
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
		if(type_of_sim=="prepaid")
		{
			itr=gray_pre.find(msin);
		}
		else if(type_of_sim=="postpaid")
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
