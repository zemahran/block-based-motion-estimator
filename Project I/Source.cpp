#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
#include "sstream"
#include "conio.h"

using namespace cv;
using namespace std;

int main()
{	
	//Image frame
	Mat Z = imread("frame1_25192.jpg", 0);

	//8x8 blocks vector
	vector <Mat> smallBlocks;
	Rect rect = Rect(0, 0, 8, 8);
	for (int i = 0; i < Z.rows; i += 8)
	{
		for (int j = 0; j < Z.cols; j += 8)
		{
			rect = Rect(j, i, 8, 8);
			smallBlocks.push_back(Mat(Z, rect));
		}
	}

	/*a vector of small 8x8 2D MATs starting from the top left
	block: smallBlocks[0] then indexing row by row, ending with the bottom
	right 8x8 block: smallBlocks[(image.cols/8)*(image.rows/8)-1]..
	this vector contains (image.cols/8)*(image.rows/8) indices,
	thus in this case (16*16 image): the vector contains 4 indices starting
	from smallBlocks[0] and ending with smallBlocks[3]*/

	//Reference frame
	Mat RR = imread("frame2_25193.jpg", 0);

	//Padding the reference frame
	Mat R = Mat(RR.rows, RR.cols, CV_8UC1);
	copyMakeBorder(RR, R, 1, 1, 1, 1, BORDER_CONSTANT, 0);

	//Output variables
	Mat Res = Mat(RR.rows, RR.cols, CV_8UC1);
	Mat Residual = Mat(RR.rows, RR.cols, CV_8UC1);
	copyMakeBorder(Res, Residual, 1, 1, 1, 1, BORDER_CONSTANT, 0);
	string vectors[1584]; //44*36

	//Variables needed for the spiral search
	Mat tmpMat = Mat(8, 8, CV_8UC1);
	double SAEs[45];
	double tmpInt = 0;
	double mean = 0;
	double min = 0;
	int minindex = 0;
	string s = "";
	double vi, vj = 0;

	//Spiral Search
	for (int i = 1, b = 0; i < R.rows-2; i += 8) {
		for (int j = 1; j < R.cols-2; j += 8) {

			min = 0;
			minindex = 0;
			s = "";
			ostringstream ossi;
			ostringstream ossj;
			vi, vj = 0;
			
			//Comparison 1
			for (int x = i, k = 0; x, k < 8; x++, k++) {
				for (int y = j, w = 0; y, w < 8; y++, w++) {
					tmpInt += ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y)) * ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y));
				}
			}
			mean = tmpInt / 64;
			tmpInt = 0;
			SAEs[0] = mean;
			SAEs[1] = i;
			SAEs[2] = j;
			SAEs[3] = 0;
			SAEs[4] = 0;

			//Comparison 2
			for (int x = i-1, k = 0; x, k < 8; x++, k++) {
				for (int y = j, w = 0; y, w < 8; y++, w++) {
					tmpInt += ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y)) * ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y));
				}
			}
			mean = tmpInt / 64;
			tmpInt = 0;
			SAEs[5] = mean;
			SAEs[6] = i-1;
			SAEs[7] = j;
			SAEs[8] = -1;
			SAEs[9] = 0;

			//Comparison 3
			for (int x = i-1, k = 0; x, k < 8; x++, k++) {
				for (int y = j+1, w = 0; y, w < 8; y++, w++) {
					tmpInt += ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y)) * ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y));
				}
			}
			mean = tmpInt / 64;
			tmpInt = 0;
			SAEs[10] = mean;
			SAEs[11] = i-1;
			SAEs[12] = j+1;
			SAEs[13] = -1;
			SAEs[14] = -1;

			//Comparison 4
			for (int x = i, k = 0; x, k < 8; x++, k++) {
				for (int y = j+1, w = 0; y, w < 8; y++, w++) {
					tmpInt += ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y)) * ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y));
				}
			}
			mean = tmpInt / 64;
			tmpInt = 0;
			SAEs[15] = mean;
			SAEs[16] = i;
			SAEs[17] = j+1;
			SAEs[18] = 0;
			SAEs[19] = -1;

			//Comparison 5
			for (int x = i+1, k = 0; x, k < 8; x++, k++) {
				for (int y = j+1, w = 0; y, w < 8; y++, w++) {
					tmpInt += ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y)) * ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y));
				}
			}
			mean = tmpInt / 64;
			tmpInt = 0;
			SAEs[20] = mean;
			SAEs[21] = i+1;
			SAEs[22] = j+1;
			SAEs[23] = 1;
			SAEs[24] = -1;

			//Comparison 6
			for (int x = i+1, k = 0; x, k < 8; x++, k++) {
				for (int y = j, w = 0; y, w < 8; y++, w++) {
					tmpInt += ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y)) * ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y));
				}
			}
			mean = tmpInt / 64;
			tmpInt = 0;
			SAEs[25] = mean;
			SAEs[26] = i+1;
			SAEs[27] = j;
			SAEs[28] = 1;
			SAEs[29] = 0;

			//Comparison 7
			for (int x = i+1, k = 0; x, k < 8; x++, k++) {
				for (int y = j-1, w = 0; y, w < 8; y++, w++) {
					tmpInt += ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y)) * ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y));
				}
			}
			mean = tmpInt / 64;
			tmpInt = 0;
			SAEs[30] = mean;
			SAEs[31] = i+1;
			SAEs[32] = j-1;
			SAEs[33] = 1;
			SAEs[34] = 1;

			//Comparison 8
			for (int x = i, k = 0; x, k < 8; x++, k++) {
				for (int y = j-1, w = 0; y, w < 8; y++, w++) {
					tmpInt += ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y)) * ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y));
				}
			}
			mean = tmpInt / 64;
			tmpInt = 0;
			SAEs[35] = mean;
			SAEs[36] = i;
			SAEs[37] = j-1;
			SAEs[38] = 0;
			SAEs[39] = 1;

			//Comparison 9
			for (int x = i-1, k = 0; x, k < 8; x++, k++) {
				for (int y = j-1, w = 0; y, w < 8; y++, w++) {
					tmpInt += ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y)) * ((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y));
				}
			}
			mean = tmpInt / 64;
			tmpInt = 0;
			SAEs[40] = mean;
			SAEs[41] = i-1;
			SAEs[42] = j-1;
			SAEs[43] = -1;
			SAEs[44] = 1;

			min = SAEs[0];
			for (int k = 5; k < 45; k+=5) {
				if (min > SAEs[k]) {
					min = SAEs[k];
					minindex = k;
				}
			}

			for (int x = SAEs[minindex + 1], k = 0; x, k < 8; x++, k++) {
				for (int y = SAEs[minindex + 2], w = 0; y, w < 8; y++, w++) {
					tmpMat.at<uchar>(k, w) = abs((int)smallBlocks[b].at<uchar>(k, w) - (int)R.at<uchar>(x, y));
				}
			}

			for (int x = i, k = 0; x,k < 8; x++, k++) {
				for (int y = j, w = 0; y,w < 8; y++, w++) {
					Residual.at<uchar>(x, y) = (int)tmpMat.at<uchar>(k, w);
				}
			}

			vi = SAEs[minindex + 3];
			vj = SAEs[minindex + 4];
			ossi << vi;
			ossj << vj;
			s += ossi.str() + ", " + ossj.str();
			vectors[b] = s;
			cout << "motion vector for the block with i = " << i << " and j = " << j << " is: " << vectors[b] << endl;
			b++;
		}
	}

	cout << "loop ended#" << endl << endl;

	namedWindow("Residual Frame", CV_WINDOW_AUTOSIZE);
	imshow("Residual Frame", Residual);
	waitKey(0);

	waitKey(0);
	_getch();
	return 0;
}
