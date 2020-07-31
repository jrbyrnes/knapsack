
#include <cstdio>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include <string.h>
#include <string> 
#include <iostream>
#include "knapsack_template.h" 





int main(int argc, char* argv[])
{
	UDT_TIME time, BFTime;
    float speedup;
	int itemCnt;
	KnapsackInstance* inst; //a Knapsack instance object
	KnapsackBFSolver BFSolver; //brute-force solver
	KnapsackBTSolver BTSolver; //backtracking solver
	KnapsackBBSolver BBSolver1(UB1); //branch-and-bound solver with UB1
	KnapsackBBSolver BBSolver2(UB2); //branch-and-bound solver with UB2
	KnapsackBBSolver BBSolver3(UB3); //branch-and-bound solver with UB3
	KnapsackSolution *DPSoln, *BFSoln, *BTSoln, *BBSoln1, *BBSoln2, *BBSoln3;

    if(argc < 2) {
        printf("Invalid Number of command-line arguments\n");
	    exit(1);
    }
    
    if (argc == 3)
    {
    	std::string secondArg = argv[2];
    	if (secondArg.compare("-withTimeOut") != 0)
    	{
    		std::cout << "Unrecognized argument: " + secondArg << std::endl;
    		exit(1);
    	}

    	else
    	{
    		TIMEOUT = 1;
    		printf("Using time limit of: %d\n", TIME_LIMIT); 
    	}
    }

    itemCnt = atoi(argv[1]);



    if(itemCnt < 1) {
        printf("Invalid number of items\n");
        exit(1);
    }

	inst = new KnapsackInstance(itemCnt);
	BFSoln = new KnapsackSolution(inst);
	BTSoln = new KnapsackSolution(inst);
	BBSoln1 = new KnapsackSolution(inst);
	BBSoln2 = new KnapsackSolution(inst);
	BBSoln3 = new KnapsackSolution(inst);
	
	inst->Generate();
	inst->Print();

	/*
	SetTime();
	BFSolver.Solve(inst,BFSoln);
	BFTime = time = GetTime();
	if (time == TIME_LIMIT)
	{
		printf("\n\nBrute Force Timed out\n");
	}

	else
	{
		printf("\n\n");
	}
	printf("Solved using brute-force enumeration (BF) in %ld ms. Optimal value = %d",time, BFSoln->GetValue());
	if(itemCnt <= MAX_SIZE_TO_PRINT)
		BFSoln->Print("Brute-Force Solution");


	SetTime();
	BTSolver.Solve(inst,BTSoln);
	time = GetTime();
	if (time == TIME_LIMIT)
	{
		printf("\n\nBacktracking Timed out\n");
	}
	else
	{
		printf("\n\n");
	}
	printf("Solved using backtracking (BT) in %ld ms. Optimal value = %d",time, BTSoln->GetValue());
	if(itemCnt <= MAX_SIZE_TO_PRINT)
		BTSoln->Print("Backtracking Solution");
	if(*BFSoln == *BTSoln)
		printf("\nSUCCESS: BF and BT solutions match");
	else
		printf("\nERROR: BF and BT solutions mismatch");
    speedup = time==0? 0 : 100.0*(BFTime-time)/(float)BFTime;
	printf("\nSpeedup of BT relative to BF is %.2f%c",speedup,'%');

	SetTime();
	BBSolver1.Solve(inst,BBSoln1);
	time = GetTime();
	if (time == TIME_LIMIT)
	{
		printf("\n\nUB1 Timed out\n");
	}
	else
	{
		printf("\n\n");
	}
	printf("Solved using branch-and-bound (BB) with UB1 in %ld ms. Optimal value = %d",time, BBSoln1->GetValue());
	if(itemCnt <= MAX_SIZE_TO_PRINT)
		BBSoln1->Print("BB-UB1 Solution");
	if(*BFSoln == *BBSoln1)
		printf("\nSUCCESS: BF and BB-UB1 solutions match");
	else
		printf("\nERROR: BF and BB-UB1 solutions mismatch");
    speedup = time==0? 0 : 100.0*(BFTime-time)/(float)BFTime;
	printf("\nSpeedup of BB-UB1 relative to BF is %.2f%c",speedup,'%');

	SetTime();
	BBSolver2.Solve(inst,BBSoln2);
	time = GetTime();
	if (time == TIME_LIMIT)
	{
		printf("\n\nUB2 Timed out\n");
	}
	else
	{
		printf("\n\n");
	}
	printf("Solved using branch-and-bound (BB) with UB2 in %ld ms. Optimal value = %d",time, BBSoln2->GetValue());
	if(itemCnt <= MAX_SIZE_TO_PRINT)
		BBSoln2->Print("BB-UB2 Solution");
	if(*BFSoln == *BBSoln2)
		printf("\nSUCCESS: BF and BB-UB2 solutions match");
	else
		printf("\nERROR: BF and BB-UB2 solutions mismatch");
    speedup = time==0? 0 : 100.0*(BFTime-time)/(float)BFTime;
	printf("\nSpeedup of BB-UB2 relative to BF is %.2f%c",speedup,'%');
	*/
	
	SetTime();
	BBSolver3.Solve(inst,BBSoln3);
	time = GetTime();
	if (time == TIME_LIMIT)
	{
		printf("\n\nUB3 Timed out\n");
	}
	else
	{
		printf("\n\n");
	}
	
	printf("Solved using branch-and-bound (BB) with UB3 in %ld ms. Optimal value = %d",time, BBSoln3->GetValue());
	if(itemCnt <= MAX_SIZE_TO_PRINT)
		BBSoln3->Print("BB-UB3 Solution");
	/*
	if(*BFSoln == *BBSoln3)
		printf("\nSUCCESS: BF and BB-UB3 solutions match");
	else
		printf("\nERROR: BF and BB-UB3 solutions mismatch");
    speedup = time==0? 0 : 100.0*(BFTime-time)/(float)BFTime;
	printf("\nSpeedup of BB-UB3 relative to BF is %.2f%c",speedup,'%');
	*/
	delete inst;
	delete DPSoln;
	delete BFSoln;
	delete BTSoln;
	delete BBSoln1;
	delete BBSoln2;
	delete BBSoln3;

	printf("\n\nProgram Completed Successfully\n");

	return 0;
}
