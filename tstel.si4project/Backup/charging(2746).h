/**********************************************************************************************
Name of the module 	: charging.h
Date of creation 	: 21/5/2020
Author of module 	: Shubhangi Joshi
Description of module:
This module contains declerations of the class charging and includes class database, prepaid, postpaid and main component as friends

Different functions supported in the module:
	int calculate_validity(date dt1,date dt2);							Function to calculate validity i.e no of days between today's date and plan activation date
	void rchg_date(int *d,int *m,int *y);								Function for calculating next recharge date
	void generate_bill(long long int msin,validity_info st);			Function to generate and print bill for postpaid subscribers

Global datatypes accessed or modified by the module.
const int Mdays[]		this array holds the no. of days for every month

Revision/Modification History:
Added charging.h		Shubhangi Joshi		21/5/2020

************************************************************************************************/

#ifndef CHARGING_H
#define CHARGING_H
#include<iostream>
#include<ctime>
#include<fstream>

#include"prepaid.h"
#include"postpaid.h"
#include"Database.h"
#include"main.h"
#include"Tstel.h"

//Macro definations
#define YEAR 365U
#define CAL_BASE 1900U

using namespace std;
const char Mdays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
class database;//forward declarations
class prepaid;
class postpaid;
class charging;
class charging {
	
	private:
	struct date{
	int date,month,year;
	};
	static charging *cu_obj;
	charging(){}

	int calculate_validity(date dt1,date dt2);
	void rchg_date(int *d,int *m,int *y,int days);
	void generate_bill(ull msin,validity_info val_info);
	
	public:
	static charging *get_cu_obj(void);
	void rchg_cu(ull msin, struct plan_info p, string str,database *);
	void get_bal_cu(ull msin,database *);
	void gen_bill_cu(ull msin,database *);
	friend class main_component;
	friend class database;
	friend class prepaid;
	friend class postpaid;
};
//charging *charging::cu_obj;
#endif
