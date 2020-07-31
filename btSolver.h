#include "InstanceSolver.h"

// Backtracking solver

class KnapsackBTSolver: public KnapsackBFSolver
{
protected:
	KnapsackInstance* inst;
	KnapsackSolution* crntSoln;
	KnapsackSolution* bestSoln;

	virtual void FindSolns(int itemNum);
	virtual void CheckCrntSoln();


public:
	KnapsackBTSolver();
	~KnapsackBTSolver();
	void Solve(KnapsackInstance* inst, KnapsackSolution* soln);
};

/*****************************************************************************/

KnapsackBTSolver::KnapsackBTSolver()
{
	crntSoln = NULL;
}
/********************************************************************/

KnapsackBTSolver::~KnapsackBTSolver()
{
	if(crntSoln != NULL)
		delete crntSoln;
}
/********************************************************************/

void KnapsackBTSolver::Solve(KnapsackInstance* inst_, KnapsackSolution* soln_)
{
	inst = inst_;	
	bestSoln = soln_;
	crntSoln = new KnapsackSolution(inst);
	FindSolns(1);
}

/*****************************************************************************/

void KnapsackBTSolver::FindSolns(int itemNum)
{

	if (TIMEOUT == 1)
	{
		if (GetTime() >= TIME_LIMIT)
		{
			return;
		}
	}

	int itemCnt = inst->GetItemCnt();

	if (crntSoln->GetWeight() > inst->GetCapacity())
	{
		return;
	}

	if(itemNum == itemCnt + 1)
	{
		CheckCrntSoln();
		return;
	}
	crntSoln->DontTakeItem(itemNum);
	FindSolns(itemNum+1);
	crntSoln->TakeItem(itemNum);
	FindSolns(itemNum+1);
	crntSoln->UndoTakeItem(itemNum);
}

/*****************************************************************************/

void KnapsackBTSolver::CheckCrntSoln()
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