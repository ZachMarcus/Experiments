#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>

using namespace cv;
using namespace std;

extern void calculateHistogram(unsigned char *inputImageData, 
                          unsigned int height, 
                          unsigned int width,
		  	  unsigned int * histogram);

double CLOCK() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC,  &t);
    return (t.tv_sec * 1000)+(t.tv_nsec*1e-6);
}

int main( int argc, const char** argv ) { 
        // arg 1: Input image 
        // arg 2: histogram value minimum (optional)
        // arg 3: histogram value maximum (optional)
        int minimumHisto = 0;
        int maximumHisto = 255;

        if (argc > 2) {
            minimumHisto = atoi(argv[2]);
        }
        if (argc > 3) {
            maximumHisto = atoi(argv[3]);
        }

        
        double start_gpu, finish_gpu;
        
        // Read input image from argument in black and white
        Mat input_image = imread(argv[1], IMREAD_GRAYSCALE);

        if (input_image.empty()){
            cout << "Image cannot be loaded..!!" << endl;
            return -1;
        }
        
        unsigned int height = input_image.rows;
        unsigned int  width = input_image.cols;
        
        ///////////////////////
        // START GPU Processing
        ///////////////////////
        
        start_gpu = CLOCK();

        unsigned int * histogram = (unsigned int *)malloc(255*sizeof(unsigned int));
	//std::vector<int> 
	calculateHistogram((unsigned char *) input_image.data, height, width, histogram);

        finish_gpu = CLOCK();
        std::cout << "GPU execution time: " << finish_gpu - start_gpu << " ms" << std::endl;

        for (int i = minimumHisto; i < maximumHisto; i++) {
            std::cout << "Value: " << i << ":\t";
	    for (int stars = 0; stars < histogram[i] / 5000; stars++) {
	        std::cout << "*";
            }
            std::cout << std::endl;
        }

        return 0;
}
