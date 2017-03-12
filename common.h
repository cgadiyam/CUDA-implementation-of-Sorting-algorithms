#ifndef __COMMON_H__
#define __COMMON_H__

void RankSortCPU( float* InputArray, float* SortedArray, int size);

void OddEvenSortCPU( float* InputArray, float* SortedArray, int size);

bool RankSortGPU( float* InputArray, float* SortedArray, int size);

bool OddEvenSortGPU( float* InputArray, float* SortedArray, int size);

#endif