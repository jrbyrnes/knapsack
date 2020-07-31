#ifndef __INSTANCE_SOLVER_H__
#define __INSTANCE_SOLVER_H__

class KnapsackInstance
{
private:
	int itemCnt; //Number of items
	int cap; //The capacity
	int* weights;//An array of weights
	int* values;//An array of values
	double* valuePerWeight;

public:
	KnapsackInstance(int itemCnt_);
	~KnapsackInstance();

	void Generate();

	int GetItemCnt();
	int GetItemWeight(int itemNum);
	int GetItemValue(int itemNum);
	float GetItemValuePerWeight(int itemNum);
	int GetCapacity();
	void Print();
	void SortByPerWeight();
};

/********************************************************************/

KnapsackInstance::KnapsackInstance(int itemCnt_)
{
	itemCnt = itemCnt_;

	weights = new int[itemCnt+1];
	values = new int[itemCnt+1];
	valuePerWeight = new double[itemCnt+1];
	cap = 0;
}
/********************************************************************/
	
KnapsackInstance::~KnapsackInstance()
{
	delete [] weights;
	delete [] values;
	delete [] valuePerWeight;
}
/********************************************************************/

void KnapsackInstance::Generate()
{
    int i, wghtSum;  
        
    weights[0] = 0;
    values[0] = 0;
        
    wghtSum = 0;
    for(i=1; i<= itemCnt; i++)
    {
        weights[i] = rand()%100 + 1;
        values[i] = weights[i] + 10;
        valuePerWeight[i] = (double)values[i] / (double)weights[i];
        wghtSum += weights[i]; 
    }
    cap = wghtSum/2;
}
/********************************************************************/

int KnapsackInstance::GetItemCnt()
{
	return itemCnt;
}
/********************************************************************/

int KnapsackInstance::GetItemWeight(int itemNum)
{
	return weights[itemNum];
}
/********************************************************************/

int KnapsackInstance::GetItemValue(int itemNum)
{
	return values[itemNum];
}

/********************************************************************/

float KnapsackInstance::GetItemValuePerWeight(int itemNum)
{
	return valuePerWeight[itemNum];
}


/********************************************************************/

int KnapsackInstance::GetCapacity()
{
	return cap;
}
/********************************************************************/


//broken -- results dont make sense
void KnapsackInstance::SortByPerWeight()
{
	//fast dev sorter -- T(n) = n^2 + n -- slow
	//note for n = 50; difference is on the order of a couple thousand which is less than exponential


	struct FractionalValue
	{
		int weight;
		int value;
		float perWeightValue;
	};

	FractionalValue *sortedByFracs = new FractionalValue[itemCnt+1];

	int j;

	for (int i = 1; i <= itemCnt; i++)
	{
		FractionalValue temp;
		temp.weight = weights[i];
		temp.value = values[i];
		temp.perWeightValue = valuePerWeight[i];

		if (i == 1)
		{
			sortedByFracs[i] = temp;
		}

		else
		{
			
			for (j = i; j >= 1; j--)
			{
				if (sortedByFracs[j].perWeightValue < temp.perWeightValue)
				{
					sortedByFracs[j+1] = sortedByFracs[j];
				}

				else
				{
					break;
				}
			}

			sortedByFracs[j+1] = temp;
		}
	}

	for (int i = 1; i <= itemCnt; i++)
	{
		weights[i] = sortedByFracs[i].weight;
		values[i] = sortedByFracs[i].value;
		valuePerWeight[i] = sortedByFracs[i].perWeightValue;
	}
}
/*****************************************************************************/

void KnapsackInstance::Print()
{
	int i;

	printf("Number of items = %d, Capacity = %d\n",itemCnt, cap);
	printf("Weights: ");
	for(i=1; i<=itemCnt; i++)
	{
		printf("%d ",weights[i]);
	}
	printf("\nValues: ");
	for(i=1; i<=itemCnt; i++)
	{
		printf("%d ",values[i]);
	}
	printf("\n");
}
/*****************************************************************************/









class KnapsackSolution
{
private:
	bool* isTaken;
	bool* isDontTaken;
	int value;
	int untakenValue;
	int untakenValueThatFit;
	int wght;
	KnapsackInstance* inst;

public:
	KnapsackSolution(KnapsackInstance* inst);
	~KnapsackSolution();

	bool operator == (KnapsackSolution& otherSoln);
	void TakeItem(int itemNum);
	void UndoTakeItem(int itemNum);
	void DontTakeItem(int itemNum);
	void UndoDontTakeItem(int itemNum);
	int ComputeValue();
	int GetValue();
	int GetWeight();
	int GetUntakenValue();
	int GetUntakenValueThatFit();
	void Print(std::string str);
	void Copy(KnapsackSolution* otherSoln);
};

/********************************************************************/

KnapsackSolution::KnapsackSolution(KnapsackInstance* inst_)
{
	int i, itemCnt = inst_->GetItemCnt();

	inst = inst_;
	isTaken = new bool[itemCnt+1];
	isDontTaken = new bool[itemCnt+1];
	value = INVALID_VALUE;
	wght = 0;
	untakenValue = 0;

	for(i=1; i<=itemCnt; i++)
	{
		untakenValue += inst->GetItemValue(i);
		isTaken[i] = false;
		isDontTaken[i] = false;
	}
}
/********************************************************************/

KnapsackSolution::~KnapsackSolution()
{
	delete [] isTaken;
}
/********************************************************************/

bool KnapsackSolution::operator == (KnapsackSolution& otherSoln)
{
	return value == otherSoln.value;
}
/********************************************************************/

void KnapsackSolution::TakeItem(int itemNum)
{
	isTaken[itemNum] = true;
	isDontTaken[itemNum] = false;
	value += inst->GetItemValue(itemNum);
	wght += inst->GetItemWeight(itemNum);
	untakenValue -= inst->GetItemValue(itemNum);	
}
/********************************************************************/

void KnapsackSolution::UndoTakeItem(int itemNum)
{
	isTaken[itemNum] = false;
	value -= inst->GetItemValue(itemNum);
	wght -= inst->GetItemWeight(itemNum);
	untakenValue += inst->GetItemValue(itemNum);
}

/*****************************************************************************/
	
void KnapsackSolution::DontTakeItem(int itemNum)
{
	isTaken[itemNum] = false;
	isDontTaken[itemNum] = true;
	untakenValue -= inst->GetItemValue(itemNum);	
}

/*****************************************************************************/

void KnapsackSolution::UndoDontTakeItem(int itemNum)
{
	isDontTaken[itemNum] = false;
	untakenValue += inst->GetItemValue(itemNum);

}
/********************************************************************/

int KnapsackSolution::ComputeValue()
{
	int i, itemCnt = inst->GetItemCnt(), weight = 0;

	value = 0;
	for(i=1; i<=itemCnt; i++)
	{
		if(isTaken[i] == true)
		{
			weight += inst->GetItemWeight(i);
			if(weight > inst->GetCapacity())
			{
				value = INVALID_VALUE;
				break;
			}
			value += inst->GetItemValue(i);
		}
	}
	return value;
}
/********************************************************************/

int KnapsackSolution::GetValue()
{
	return value;
}

/********************************************************************/

int KnapsackSolution::GetUntakenValue()
{
	return untakenValue;
}

/********************************************************************/

int KnapsackSolution::GetUntakenValueThatFit()
{
	int itemCnt = inst->GetItemCnt();

	untakenValueThatFit = 0;
	
	for (int i = 1; i <= itemCnt; i++)
	{
		if (isTaken[i] == false && isDontTaken[i] == false)
		{
			if (inst->GetItemWeight(i) <= inst->GetCapacity() - wght)
			{
				untakenValueThatFit += inst->GetItemValue(i);
			}
		}
	}
	return untakenValueThatFit;
}

/********************************************************************/

int KnapsackSolution::GetWeight()
{
	return wght;
}


/********************************************************************/

void KnapsackSolution::Copy(KnapsackSolution* otherSoln)
{
	int i, itemCnt = inst->GetItemCnt();

	for(i=1; i<=itemCnt; i++)
	{
		isTaken[i] = otherSoln->isTaken[i];
	}
	value = otherSoln->value;
}
/********************************************************************/

void KnapsackSolution::Print(std::string title)
{
	int i, itemCnt = inst->GetItemCnt();

	printf("\n%s: ",title.c_str());
	for(i=1; i<=itemCnt; i++)
	{
		if(isTaken[i] == true)
			printf("%d ",i);
	}
	printf("\nValue = %d\n",value);
	
}
/*****************************************************************************/


#endif

