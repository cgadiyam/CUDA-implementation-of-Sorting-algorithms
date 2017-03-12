#include <cuda.h>
#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>
#include <iostream>

__global__ void RankSortKernel(float* DataIn, float* DataOut, int* rank, int size)
{
	// Retrieve our coordinates in the block
	int tx = (blockIdx.x * 512) + threadIdx.x;
	rank[tx] = 0;
	if(tx < size)
	{
		for(int i=0;i<size;i++)
		{
			if(DataIn[tx]>=DataIn[i])
			{
				rank[tx]++;
			}
		}
		DataOut[(rank[tx]-1)] = DataIn[tx];
	}
}

__global__ void OddEvenSortKernel(float* Array, int size, bool Odd_Phase)
{
	int tx = (blockIdx.x * 512) + threadIdx.x;
	float temp;
	int index = 2*tx;
	if(tx < (size/2))
	{
		if(Odd_Phase == false)
		{
			if((index+1) < size)
			{
				if(Array[index]>Array[index+1])
				{
					temp = Array[index];
					Array[index] = Array[index+1];
					Array[index+1] = temp;
				}
			}
		}
		else
		{
			if((index+2) < size)
			{
				if(Array[index+1]>Array[index+2])
				{
					temp = Array[index+1];
					Array[index+1] = Array[index+2];
					Array[index+2] = temp;
				}
			}
		}
	}
}

bool RankSortGPU( float* InputArray, float* SortedArray, int size)
{
	int blocksize, gridsize;
	// Error return value
	cudaError_t status;
	// Number of bytes
	int bytes = size * sizeof(float);
	// Pointers to the device arrays
	float *DataIn, *DataOut;
	int *rank;
	int bytes1 = size * sizeof(float);
	// Allocate memory on the device
	cudaMalloc((void**) &DataIn, bytes);
	cudaMalloc((void**) &DataOut, bytes);
	cudaMalloc((void**) &rank, bytes1);
	// Copy the host input data to the device
	cudaMemcpy(DataIn, InputArray, bytes, cudaMemcpyHostToDevice);
	// Specify the size of the grid and the size of the block
	dim3 dimBlock(512, 1); 
	dim3 dimGrid((int)ceil((float)size/512), 1);
	// Launch the kernel on a size-by-size block of threads
	RankSortKernel<<<dimGrid, dimBlock>>>(DataIn, DataOut, rank, size);
	// Wait for completion
	cudaThreadSynchronize();
	// Check for errors
	status = cudaGetLastError();
	if (status != cudaSuccess) 
	{
		std::cout << "Kernel failed 1: " << cudaGetErrorString(status) <<
		std::endl;
		cudaFree(DataIn);
		cudaFree(DataOut);
		return false;
	}
	// Retrieve the result matrix
	cudaMemcpy(SortedArray, DataOut, bytes, cudaMemcpyDeviceToHost);
	// Free device memory
	cudaFree(DataIn);
	cudaFree(DataOut);
	// Success
	return true;
}

bool OddEvenSortGPU( float* InputArray, float* SortedArray, int size)
{
	int blocksize, gridsize;
	// Error return value
	cudaError_t status;
	// Number of bytes
	int bytes = size * sizeof(float);
	// Pointers to the device arrays
	float *Array;
	bool Odd_Phase;
	// Allocate memory on the device
	cudaMalloc((void**) &Array, bytes);
	// Copy the host input data to the device
	cudaMemcpy(Array, InputArray, bytes, cudaMemcpyHostToDevice);
	int new_size = size/2;
	// Specify the size of the grid and the size of the block
	dim3 dimBlock(512, 1); 
	dim3 dimGrid((int)ceil((float)new_size/512), 1);
	for(int i=0;i<size;i++)
	{
		//even phase
		Odd_Phase = false;
		// Launch the kernel on a size-by-size block of threads
		OddEvenSortKernel<<<dimGrid, dimBlock>>>(Array, size, Odd_Phase);
		// Wait for completion
		cudaThreadSynchronize();
		// Check for errors
		status = cudaGetLastError();
		if (status != cudaSuccess) 
		{
			std::cout << "Kernel failed 2: " << cudaGetErrorString(status) <<
			std::endl;
			cudaFree(Array);
			return false;
		}
		//odd phase
		Odd_Phase = true;
		// Launch the kernel on a size-by-size block of threads
		OddEvenSortKernel<<<dimGrid, dimBlock>>>(Array, size, Odd_Phase);
		// Wait for completion
		cudaThreadSynchronize();
		// Check for errors
		status = cudaGetLastError();
		if (status != cudaSuccess) 
		{
			std::cout << "Kernel failed 3: " << cudaGetErrorString(status) <<
			std::endl;
			cudaFree(Array);
			return false;
		}
	}
	// Retrieve the result matrix
	cudaMemcpy(SortedArray, Array, bytes, cudaMemcpyDeviceToHost);
	// Free device memory
	cudaFree(Array);
	// Success
	return true;
}

