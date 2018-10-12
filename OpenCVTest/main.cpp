#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;



int main(int argc, char *argv[])
{
	Mat image;
	image = imread("/Users/dhanukaramanayake/Documents/Work/ImageProcessing/Histogram Equalizer/OpenCVTest/low-contrast.png", IMREAD_GRAYSCALE);
	
	if(! image.data)
	{
		cout << "could not open or find the image" << endl;
	}

    int h = image.rows;
    int w = image.cols;
    printf("h %d | w %d \n",h,w);
    double totalPixels = h*w;

	Mat copy = image.clone();

	int levels[256];
	memset(levels, 0, sizeof(levels));

	float pr[256];
	memset(pr, 0, sizeof(pr));

	float cpd[256];
	memset(cpd, 0, sizeof(cpd));

//    int totalPixels = 1.0*image.rows * image.cols;

	int npix[256];
	memset(npix, 0, sizeof(npix));


	for(int row = 0; row < image.rows; row++)
	{
		for(int col = 0; col < image.cols; col++)
		{
			int level = image.at<uchar>(row, col);
//            printf("row = %d | col = %d | level = %d \n",row,col,level);
			levels[level]++;
//            levels array length is 256. level is the color value. relevant color value is incremented by 1 if it matches.
//            printf("color value %d | count = %d \n", level, levels[level]);
		}
	}
   

    for(int i=0; i<=255; i++)
    {
//        printf("total pixels %f \n", totalPixels);
        pr[i] = levels[i] / double(totalPixels);
//        printf("%d | %f , %d\n",i, pr[i], levels[i]);
        if(i==0)
        {
            cpd[i] = pr[i];
//            printf("cpd array index %d | value %f\n",i,cpd[i]);
        }
        else
        {
            cpd[i] = cpd[i - 1] + pr[i];
//            printf("cpd array index %d | value %f\n",i,cpd[i]);
        }
        npix[i] = cvRound(255 * cpd[i]);
        printf("npix array index %d | value %d\n",i, npix[i]);
    }

    for(int i=0; i < image.rows; i++)
    {
        for(int j=0; j < image.cols; j++)
        {
            int pix = image.at<uchar>(i, j);
            copy.at<uchar>(i, j) = npix[pix];
        }
    }

    int max = levels[0];
    for ( int i = 0 ; i < 256; i++)
    {
        max = std::max((int)levels[i], max);
//        printf("%d %d old max : %d \n", i, levels[i], max);
    }

    int histHeight = 600;
    int histWidth = 800;

    Mat hist = Mat(histHeight, histWidth, CV_8UC1, Scalar(0));
    double multiplier = 1.0*histHeight / max;
    for (int i = 0; i < 256; i++)
    {
        levels[i] = (int)(levels[i] * multiplier);
        line(hist,
            Point(cvRound((double)histWidth/256)*i, histHeight),
            Point(cvRound((double)histWidth / 256)*i, histHeight - levels[i]),
            Scalar(255));
    }
    
    imwrite("/Users/dhanukaramanayake/Documents/Work/ImageProcessing/Histogram Equalizer/OpenCVTest/Photo.jpg", copy);
    
    cvNamedWindow("Photo", CV_WINDOW_NORMAL);
    cvNamedWindow("Original Photo", CV_WINDOW_NORMAL);
    
    imshow("Histogram", hist);
    
    imshow("Photo", copy);
    imshow("Original Photo", image);
    
    
// histogram for new image
    
    memset(levels, 0, sizeof(levels));
    for(int row = 0; row < copy.rows; row++)
    {
        for(int col = 0; col < copy.cols; col++)
        {
            int level = copy.at<uchar>(row, col);
//            printf("new row = %d | new col = %d | level = %d \n",row,col,level);
            levels[level]++;
//            levels array length is 256. level is the color value. relevant color value is incremented by 1 if it matches.
//            printf("color value %d | count = %d \n", level, levels[level]);
        }
    }

    max = levels[0];
    for ( int i = 0 ; i < 256; i++)
    {
        max = std::max((int)levels[i], max);
//                printf("%d %d new nmax : %d \n", i, levels[i], max);
    }
    
    hist = Mat(histHeight, histWidth, CV_8UC1, Scalar(0));
    multiplier = 1.0*histHeight / max;
    for (int i = 0; i < 256; i++)
    {
        levels[i] = (int)(levels[i] * multiplier);
        line(hist,
             Point(cvRound((double)histWidth/256)*i, histHeight),
             Point(cvRound((double)histWidth / 256)*i, histHeight - levels[i]),
             Scalar(255));
    }
    
    imshow("Equalized Histogram", hist);

	waitKey(0);
	image.release();
	return 0;
}
