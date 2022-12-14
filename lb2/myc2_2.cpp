#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <omp.h>

using namespace std;
using namespace cv;


int main()
{

	Mat img = imread("F:\\123.jpg"); 

	if (img.empty())
	{
		std::cout << "Could not read the image: " << std::endl;
		return 1;
	}

	Mat img1(img.rows, img.cols, CV_8UC1);

	int avarageS = 0;
	int avarageM = 0;

	for (int l = 0; l < 1; l++) {

		std::cout << l << "\n";
		int i, j, k;

		for (int i = 0; i < img.rows; i++)
			for (int j = 0; j < img.cols; j++)
			{
				Vec3b rgb_img = img.at<Vec3b>(i, j);
				img1.at<uchar>(i, j) = (rgb_img[0] + rgb_img[1] + rgb_img[2]) / 3;

			}
		auto start1 = chrono::system_clock::now();

#pragma omp parallel for private(i, j,k) shared(img1)

		for (int k = 0; k < 100; k++) {
			for (int i = 1; i < img.rows - 1; i++) {
				for (int j = 1; j < img.cols - 1; j++) {
					
					img1.at<uchar>(i, j) = (img1.at<uchar>(i, j) * 4 / 16 +
						img1.at<uchar>(i - 1, j) * 2 / 16 +
						img1.at<uchar>(i - 1, j + 1) * 1 / 16 +
						img1.at<uchar>(i, j + 1) * 2 / 16 +
						img1.at<uchar>(i + 1, j + 1) * 1 / 16 +
						img1.at<uchar>(i + 1, j) * 2 / 16 +
						img1.at<uchar>(i + 1, j - 1) * 1 / 16 +
						img1.at<uchar>(i, j - 1) * 2 / 16 +
						img1.at<uchar>(i - 1, j - 1) * 1 / 16) / 9;
				}
			}
		}
		auto end1 = chrono::system_clock::now();

		auto division1 = chrono::duration_cast<chrono::milliseconds>(end1 - start1).count();

		std::cout << "Multi - " << division1 << " msec";

		avarageM += division1;

		auto start2 = chrono::system_clock::now();

		for (int k = 0; k < 1; k++) {
			for (int i = 1; i < img.rows - 1; i++) {
				for (int j = 1; j < img.cols - 1; j++) {

					img1.at<uchar>(i, j) = (img1.at<uchar>(i, j) * 4 / 16 +
						img1.at<uchar>(i - 1, j) * 2 / 16 +
						img1.at<uchar>(i - 1, j + 1) * 1 / 16 +
						img1.at<uchar>(i, j + 1) * 2 / 16 +
						img1.at<uchar>(i + 1, j + 1) * 1 / 16 +
						img1.at<uchar>(i + 1, j) * 2 / 16 +
						img1.at<uchar>(i + 1, j - 1) * 1 / 16 +
						img1.at<uchar>(i, j - 1) * 2 / 16 +
						img1.at<uchar>(i - 1, j - 1) * 1 / 16) / 9 ;
				}
			}
		}
		auto end2 = std::chrono::system_clock::now();
		

		auto division2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count();

		std::cout << "\nSingle - " << division2 << " msec\n";

		cv::imwrite("F:\\images\\new.jpg", img1);
		avarageS += division2;
	}
	std::cout << "avarage Single - " << avarageS / 10 << "\n avarage Multi - " << avarageM/10;
}