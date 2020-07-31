#ifndef __KNAPSACK_TEMPLATE_H__
#define __KNAPSACK_TEMPLATE_H__

//#define KNAPSACK_DEBUG

#define INVALID_VALUE -1
#define TIME_LIMIT 10000		//in milliseconds
#define TIMEB struct timeb
#define FTIME ftime
#define UDT_TIME long
#define MAX_SIZE_TO_PRINT 10

int TIMEOUT = 0;
UDT_TIME gRefTime = 0;
UDT_TIME GetMilliSecondTime(TIMEB timeBuf);
void SetTime(void);
UDT_TIME GetTime(void);




#include "InstanceSolver.h"
#include "bfSolver.h"
#include "bbSolver.h"
#include "btSolver.h"

/********************************************************************/

UDT_TIME GetCurrentTime(void)
{
	UDT_TIME crntTime=0;

	TIMEB timeBuf;
	FTIME(&timeBuf);
    crntTime = GetMilliSecondTime(timeBuf);

	return crntTime;
}


void SetTime(void)
{
	gRefTime = GetCurrentTime();
}
/********************************************************************/

UDT_TIME GetTime(void)
{
	UDT_TIME crntTime = GetCurrentTime();

	return (crntTime - gRefTime);
}
/********************************************************************/

UDT_TIME GetMilliSecondTime(TIMEB timeBuf)
{
	UDT_TIME mliScndTime;

	mliScndTime = timeBuf.time;
	mliScndTime *= 1000;
	mliScndTime += timeBuf.millitm;
	return mliScndTime;
}
/*****************************************************************************/

#endif