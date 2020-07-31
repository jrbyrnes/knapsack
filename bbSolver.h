#include "InstanceSolver.h"

//n = [140-147], 162, 165, 169, 173

enum UPPER_BOUND { UB1, UB2, UB3 };

// Branch-and-Bound solver
class KnapsackBBSolver: public KnapsackBFSolver
{
protected:
	enum UPPER_BOUND ub;
	KnapsackInstance* inst;
	KnapsackSolution* crntSoln;
	KnapsackSolution* bestSoln;
	int cap;


	
	int slack, slackRem;
	int fractionalSolution;
	
	virtual void FindSolns(int itemNum);
	virtual void CheckCrntSoln();

public:
	KnapsackBBSolver(enum UPPER_BOUND ub_);
	~KnapsackBBSolver();
	void Solve(KnapsackInstance* inst, KnapsackSolution* soln);
	int Grab(int GrabWeight);
};

/*****************************************************************************/

KnapsackBBSolver::KnapsackBBSolver(enum UPPER_BOUND ub_)
{
	ub = ub_;
	crntSoln = NULL;

}
/********************************************************************/

KnapsackBBSolver::~KnapsackBBSolver()
{
	if(crntSoln != NULL)
		delete crntSoln;
}

/********************************************************************/

void KnapsackBBSolver::Solve(KnapsackInstance* inst_, KnapsackSolution* soln_)
{
	inst = inst_;	
	bestSoln = soln_;
	crntSoln = new KnapsackSolution(inst);
	cap = inst->GetCapacity();


	if (ub == UB3) //|| ub == UB1)
	{
		inst->SortByPerWeight();
		slack = 1;
		slackRem = 0;
		fractionalSolution = Grab(cap);	
	
	
		//initial solution -- top X items sorted by value per weight subject to cap constraint 
		for (int i = 1; i < slack; i++)
		{
			bestSoln->TakeItem(i);
		}
	}

	FindSolns(1);
}


/*****************************************************************************/

void KnapsackBBSolver::FindSolns(int itemNum)
{
	//check timeout
	if (TIMEOUT == 1)
	{
		if (GetTime() >= TIME_LIMIT)
		{
			return;
		}
	}

	static int itemCnt = inst->GetItemCnt(); //only need to be initialized once

	//backtrack
	if (crntSoln->GetWeight() > cap)
	{
		return;
	}

	//check if sol is best
	if(itemNum == itemCnt + 1)
	{
		CheckCrntSoln();
		return;
	}

	//check heuristic
	switch(ub)
	{
		case UB1:
			if (crntSoln->GetValue() + crntSoln->GetUntakenValue() <= bestSoln->GetValue())
			{
				return;
			}
			break;

		case UB2: 
			if (crntSoln->GetValue() + crntSoln->GetUntakenValueThatFit() <= bestSoln->GetValue())
			{
				return;
			}
			break;

		
		case UB3: 
			if (fractionalSolution <= bestSoln->GetValue())
			{
				return;
			}
			break;
		
		default:
			break;

	}







	crntSoln->TakeItem(itemNum);
	FindSolns(itemNum+1);
	crntSoln->UndoTakeItem(itemNum);


	//need these locals for each stack in recursion
	int removedWeight = 0;
	int prevFractionalSolution;
	int prevSlack, prevSlackRem;

	//DontTakeItem for UB3
	
	if (ub == UB3)
	{


		prevFractionalSolution = fractionalSolution;
		prevSlack = slack;
		prevSlackRem = slackRem;

		if (itemNum <= slack)
		{
			if (itemNum == slack)
			{
				fractionalSolution -= slackRem * inst->GetItemValuePerWeight(itemNum);
				removedWeight = slackRem;
				slackRem = 0;
				slack += 1;
			}

			else
			{
				fractionalSolution -= inst->GetItemValue(itemNum);
				removedWeight = inst->GetItemWeight(itemNum);
			}

			fractionalSolution += Grab(removedWeight);
		}	
	}
	
	crntSoln->DontTakeItem(itemNum);
	FindSolns(itemNum+1);

	//UndoDontTakeItem UB3
	if (ub == UB3)
	{
		//roll values back to previous
		fractionalSolution = prevFractionalSolution;
		slack = prevSlack;
		slackRem = prevSlackRem;
	}

	crntSoln->UndoDontTakeItem(itemNum);

}

/*****************************************************************************/

/*
 * Grab implements the incremental modification of fractional knapsack
 *
*/


int KnapsackBBSolver::Grab(int grabWeight)
{
	int runningSum = 0;
	int slackDiff = inst->GetItemWeight(slack) - (slackRem);
	int itemCount = inst->GetItemCnt();

	if (slackDiff > grabWeight)
	{	
		runningSum += grabWeight * inst->GetItemValuePerWeight(slack);
		slackRem += grabWeight;
		grabWeight = 0;
		
	}

	else
	{
		runningSum += slackDiff * inst->GetItemValuePerWeight(slack);
		slackRem = 0;
		++slack;
		grabWeight -= slackDiff;

		while (grabWeight > 0 && slack <= itemCount)
		{
			if (inst->GetItemWeight(slack) > grabWeight)
			{
				break;
			}

			else
			{
				runningSum += inst->GetItemValue(slack);
				grabWeight -= inst->GetItemWeight(slack);
				slack++;
			}
		}

		if (grabWeight != 0 && slack <= itemCount)
		{
			runningSum += grabWeight * inst->GetItemValuePerWeight(slack) + 1; //+1 to account for potential double round down
			slackRem = grabWeight;
			grabWeight = 0;
			
		}
	}

	// correct for loop break condition
	if (slack > inst->GetItemCnt()+ 1)
	{
		slack = inst->GetItemCnt()+ 1;
	}

	return runningSum;
}

/*****************************************************************************/


void KnapsackBBSolver::CheckCrntSoln()
{
	int crntVal = crntSoln->ComputeValue();

#ifdef KNAPSACK_DEBUG
	printf("\nChecking solution ");
	crntSoln->Print(" ");
#endif

	if(crntVal == INVALID_VALUE)
		return;

	if(bestSoln->GetValue() == INVALID_VALUE) //The first solution is initially the best solution
		bestSoln->Copy(crntSoln);
	else
	{
		if(crntVal > bestSoln->GetValue())
			bestSoln->Copy(crntSoln);
	}
}

/*****************************************************************************/




