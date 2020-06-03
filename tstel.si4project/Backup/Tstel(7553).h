/*********************************************************************
  Name of the module	:Tstel.h
  Date of creation	:23/5/2020 
  Author of module	:Uppari Kranthi Kumar
  Description of module:
  This module contains the Global Datatypes and includes all other header files that are required for Not Operator Tool

  Global Datatypes Used:

  struct plan_info{};		this structure holds the plan details of subscriber like Amount, No.of days valid till,TMSI , IMEI and Type of plan selected.
  struct validity_info{} 	this structure holds the validity details like IMEI, previos recharge date, No. of days valid till and Amount.
  struct subscriber_ifo{}	this structure holds the entire subscriber info  like IMSI , along with plan details and validity details

  typedef unsigned long long int ull; using "unsigned long long int" as "ull"

  Revision/Modification History:
  Added 		Tstel.h 		U.Kranthi Kumar			23-05-2020
 **************************************************************************/

#ifndef __Tstel_h__
#define __Tstel_h__
#include<iostream>

typedef unsigned long long int ull;

struct validity_info{
	ull imei;
	int amt;
	int date,month,yr;
	int no_of_days;	
};

struct plan_info {
	int amt;
	char plan_type;
	int no_of_days;
	ull tmsi, imei;
};

struct subscriber_info{
	std::string sim_type;
	ull imsi;
	struct plan_info plan;
	struct validity_info validity;
};

#include"main.h"
#include"Database.h"
#include"prepaid.h"
#include"postpaid.h"
#include"charging.h"

//#include "main.cpp"
//#include "Database.cpp"
//#include "prepaid.cpp"
//#include "postpaid.cpp"
//#include "charging.cpp"
#endif
