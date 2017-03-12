#include <cstdlib> // malloc(), free()
#include <iostream>
#include <stdio.h>
#include "common.h"
using namespace std;

void RankSortCPU( float* InputArray, float* SortedArray, int size)
{
	int *rank = new int[size];
	for(int i=0;i<size;i++)
	{
		rank[i]=0; //initialize rank array
	}
	//find rank of each element in the array
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size;j++)
		{
			if(InputArray[i]>=InputArray[j])
			{
				rank[i]++;
			}
		}
	}
	//sorted array
	for(int i=0;i<size;i++)
	{
		SortedArray[(rank[i]-1)] = InputArray[i];
	}
}

void OddEvenSortCPU( float* InputArray, float* SortedArray, int size)
{
	float temp;
	for(int i=0;i<size;i++)
	{
		SortedArray[i] = InputArray[i];
	}
	for(int j=0;j<size;j++)
	{
		//odd phase
		for(int i=1;i<size-1;i+=2)
		{
			if(SortedArray[i]>SortedArray[i+1])
			{
				temp = SortedArray[i];
				SortedArray[i] = SortedArray[i+1];
				SortedArray[i+1] = temp;
			}
		}
		//even phase
		for(int i=0;i<size-1;i+=2)
		{
			if(SortedArray[i]>SortedArray[i+1])
			{
				temp = SortedArray[i];
				SortedArray[i] = SortedArray[i+1];
				SortedArray[i+1] = temp;
			}
		}
	}
}