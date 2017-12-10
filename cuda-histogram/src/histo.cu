
#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <time.h>

#define TIMER_CREATE(t)               \
  cudaEvent_t t##_start, t##_end;     \
  cudaEventCreate(&t##_start);        \
  cudaEventCreate(&t##_end);               
 
 
#define TIMER_START(t)                \
  cudaEventRecord(t##_start);         \
  cudaEventSynchronize(t##_start);    \
 
 
#define TIMER_END(t)                             \
  cudaEventRecord(t##_end);                      \
  cudaEventSynchronize(t##_end);                 \
  cudaEventElapsedTime(&t, t##_start, t##_end);  \
  cudaEventDestroy(t##_start);                   \
  cudaEventDestroy(t##_end);     
  
#define TILE_SIZE 16
#define CUDA_TIMING

unsigned char *input_gpu;
unsigned int *output_gpu;

/*******************************************************/
/*                 Cuda Error Function                 */
/*******************************************************/
inline cudaError_t checkCuda(cudaError_t result) {
    #if defined(DEBUG) || defined(_DEBUG)
    if (result != cudaSuccess) {
        fprintf(stderr, "CUDA Runtime Error: %s\n", cudaGetErrorString(result));
        exit(-1);
    }
    #endif
    return result;
}
                
// GPU kernel and functions
__global__ void kernel(unsigned char *input, 
                       unsigned int *output,
                       unsigned int numberOfPixels){
//    int x = blockIdx.x*TILE_SIZE+threadIdx.x;
//    int y = blockIdx.y*TILE_SIZE+threadIdx.y;
    int pixel = blockIdx.x * blockDim.x + threadIdx.x;
    if (pixel < numberOfPixels) {
        atomicAdd(&(output[input[pixel]]), 1);
    }
}


void calculateHistogram(unsigned char *inputImageData,
                          unsigned int height,
                          unsigned int width,
                          unsigned int* outputHistogram) {
    unsigned int numberOfPixels = height * width;
    int gridXSize = 1 + (( width - 1) / TILE_SIZE);
    int gridYSize = 1 + ((height - 1) / TILE_SIZE);

    int XSize = gridXSize*TILE_SIZE;
    int YSize = gridYSize*TILE_SIZE;

    // Both are the same size (CPU/GPU).
    int size = XSize*YSize;

    // Allocate arrays in GPU memory
    checkCuda(cudaMalloc((void**)&input_gpu    , size*sizeof(unsigned char)));
    checkCuda(cudaMalloc((void**)&output_gpu  , size*sizeof(unsigned int)));
    checkCuda(cudaMemset(output_gpu , 0 , size*sizeof(unsigned int)));
    checkCuda(cudaMemcpy(input_gpu, inputImageData, height*width*sizeof(char),cudaMemcpyHostToDevice));
    checkCuda(cudaDeviceSynchronize());
    
    // Execute algorithm
    dim3 dimGrid(gridXSize, gridYSize);
    dim3 dimBlock(TILE_SIZE, TILE_SIZE);
    #if defined(CUDA_TIMING)
        float Ktime;
        TIMER_CREATE(Ktime);
        TIMER_START(Ktime);
    #endif

    // Kernel Call
    kernel<<<dimGrid, dimBlock>>>(input_gpu, output_gpu, numberOfPixels);
    checkCuda(cudaDeviceSynchronize());
    #if defined(CUDA_TIMING)
        TIMER_END(Ktime);
        printf("Kernel Execution Time: %f ms\n", Ktime);
    #endif
        
    // Retrieve results from the GPU
    checkCuda(cudaMemcpy(outputHistogram, 
                        output_gpu, 
                        256 * sizeof(unsigned int), 
                        cudaMemcpyDeviceToHost));
                        
    // Free resources and end the program
    checkCuda(cudaFree(output_gpu));
    checkCuda(cudaFree(input_gpu));

}
