#ifndef __Interception_barred_h__
#define __Interception_barred_h__

class database;//forward declarations
class prepaid;
class postpaid;
class charging;
class barred
{
public:
	void black_gray_subscriber(database *,prepaid *,postpaid *,charging *);
};

#endif
