#include <cstdlib> // malloc(), free()
#include <iostream> // cout, stream
#include <cuda_runtime_api.h>
#include <stdio.h>
#include "common.h"
#include <ctime>
#include <cmath>
//using namespace std;

//#define RAND_MAX 0x7FFF

const int SIZE = 1 << 14;
const int ITERS = 5;

void DisplayResults(float *InputArray, float *SortedArray)
{
	int i;
	std::cout<<"Input array: ";
	for(i =0; i<SIZE;i++)
	{
		std::cout<<InputArray[i]<<"\t"<<std::endl;
	}
	std::cout<<"Sorted array: ";
	for(i =0; i<SIZE;i++)
	{
		std::cout<<SortedArray[i]<<"\t"<<std::endl;
	}
	std::cout<<std::endl;
}

int main()
{

	// Timing data
	float tcpu, tgpu;
	clock_t start, end;

	float sum = 0, delta = 0;
	bool status;
	float L2norm;

	float *A = new float[SIZE];
	float *Pcpu = new float[SIZE];
	float *Pgpu = new float[SIZE];
	std::cout << "Operating on data of size " << SIZE << std::endl;
	int i=0;
	for(i =0; i<SIZE;i++)
	{
		A[i] = static_cast <float>(rand())/static_cast <float>(RAND_MAX);
	}


	start = clock();
	for (int i = 0; i < ITERS; i++) 
	{
		RankSortCPU(A, Pcpu, SIZE);
	}
	end = clock();
	tcpu = (float)(end - start) * 1000 / (float)CLOCKS_PER_SEC / ITERS;

	// Display the results
	std::cout << "CPU Rank Sort took " << tcpu << " ms" << std::endl;
	/*DisplayResults(A, Pcpu);*/

	// Perform one warm-up pass and validate
	status = RankSortGPU(A, Pgpu, SIZE);
	if (!status) 
	{
		std::cout << "\n * Device error! * \n" << std::endl;
		return 1;
	}
	// And now time it
	start = clock();
	for (int i = 0; i < ITERS; i++)
	{
		RankSortGPU(A, Pgpu, SIZE);
	}
	end = clock();
	tgpu = (float)(end - start) * 1000 / (float)CLOCKS_PER_SEC / ITERS;
	
	// Display the results
	std::cout << "GPU Rank Sort took " << tgpu << " ms" << std::endl;
	//DisplayResults(A, Pgpu);

	//speedup
	std::cout << "Rank Sort speedup : " << (tcpu/tgpu) << std::endl;


	start = clock();
	for (int i = 0; i < ITERS; i++) 
	{
		OddEvenSortCPU(A, Pcpu, SIZE);
	}
	end = clock();
	tcpu = (float)(end - start) * 1000 / (float)CLOCKS_PER_SEC / ITERS;

	// Display the results
	std::cout << "CPU Odd Even Sort took " << tcpu << " ms" << std::endl;
	//DisplayResults(A, Pcpu);

	// Perform one warm-up pass and validate
	status = OddEvenSortGPU(A, Pgpu, SIZE);
	if (!status) 
	{
		std::cout << "\n * Device error! * \n" << std::endl;
		return 1;
	}
	// And now time it
	start = clock();
	for (int i = 0; i < ITERS; i++)
	{
		OddEvenSortGPU(A, Pgpu, SIZE);
	}
	end = clock();
	tgpu = (float)(end - start) * 1000 / (float)CLOCKS_PER_SEC / ITERS;
	
	// Display the results
	std::cout << "GPU Odd Even Sort took " << tgpu << " ms" << std::endl;
	//DisplayResults(A, Pgpu);

	//speedup
	std::cout << "Odd Even Sort speedup : " << (tcpu/tgpu) << std::endl;

	delete[] A;
	delete[] Pcpu;
	delete[] Pgpu;

	while(1);
	return 0;
}