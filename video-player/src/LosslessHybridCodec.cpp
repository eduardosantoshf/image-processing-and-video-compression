#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <fstream> 
#include <vector>
#include <math.h>
#include <ctime>
#include "Predictor.cpp"

using namespace std;
using namespace cv;

class LosslessHybridCodec {
	private:
		string File;
		int block_size;
		int search_space;
		int pred;
		int type;
		int m;
		int period;
		int flag;
		int n_of_current_frame;
		Predictor *p;
		
	public: 

	   	LosslessHybridCodec(string v, int bs, int ss, int tv, int tp, int M, int pe, int f, string outFile){

			/**
			 * LosslessHybridCodec Constructor
			 * 
			 * @param v name of the file to be encoded
			 * @param bs block size
			 * @param ss search space
			 * @param tv type of the videp
			 * @param tp type of the predictor
			 * @param M value of m
			 * @param pe periodicity
			 * @param f flag
			 * @param outFile name of the encoded file
			 */

	   		File = v;
	   		block_size = bs;
	   		search_space = ss;
	   		type = tv;
	   		pred = tp;
	   		m = M;
	   		period = pe;
	   		flag = f;
	   		n_of_current_frame = 0;
		   	if (flag == 1){
		   		Mat fra;
		   		VideoCapture capa(File);
		   		capa >> fra;
		   		int count = 0;
		   		while(!fra.empty()){
		   			count++;
		   			capa >> fra;
		   		}
	   			p = new Predictor(pred,m,outFile,type,count,flag,block_size,search_space,period);
	   		}else{
	   			p = new Predictor(0,0,outFile,0,0,0,0,0,0);
	   		}

	   	}
	   	
	   	void encode(){

			/**
			 * Function to encode the video
			 */

	   		int count = 1;
	   		Mat frame;
	   		Mat frame1;
	   		VideoCapture cap(File);
	   		cap >> frame;
	   		switch (pred){
	   			case 0:
	   				while(!frame.empty()){
						vector<Mat> planes;
						if (type == 0) {
							if (n_of_current_frame % period == 0){
								split(frame, planes);
								p->encodeJPEG1(planes[0]);
								p->encodeJPEG1(planes[1]);
								p->encodeJPEG1(planes[2]);
							}else{
								split(frame, planes);
								p->encode_by_blocks(planes);
							}
							p->set_last_frame(planes);
						} else if (type == 1){
							cvtColor(frame, frame, COLOR_RGB2YUV );
							split(frame, planes);
							int linhas = planes[0].rows;
							int colunas = planes[0].cols;
							Mat result1(linhas,colunas/2,0);
							Mat result2(linhas,colunas/2,0);
							for(int c = 0; c < linhas; c++){
								for(int i = 0; i < colunas; i+=2){
									result1.at<uchar>(c,i/2) = planes[1].at<uchar>(c,i);
									result2.at<uchar>(c,i/2) = planes[2].at<uchar>(c,i);
								}
							}
							if (n_of_current_frame % period == 0){
								p->encodeJPEG1(planes[0]);
								p->encodeJPEG1(result1);
								p->encodeJPEG1(result2);
							}else{
								p->encode_by_blocks({planes[0],result1,result2});
							}
							p->set_last_frame({planes[0],result1,result2});
						}else if (type == 2){
							cvtColor(frame, frame, COLOR_RGB2YUV );
							split(frame, planes);
							int linhas = planes[0].rows;
							int colunas = planes[0].cols;
							Mat result1(linhas/2,colunas/2,0);
							Mat result2(linhas/2,colunas/2,0);
							for(int c = 0; c < linhas; c+=2){
								for(int i = 0; i < colunas; i+=2){
									result1.at<uchar>(c/2,i/2) = planes[1].at<uchar>(c,i);
									result2.at<uchar>(c/2,i/2) = planes[2].at<uchar>(c,i);
								}
							}
							if (n_of_current_frame % period == 0){
								p->encodeJPEG1(planes[0]);
								p->encodeJPEG1(result1);
								p->encodeJPEG1(result2);
							}else{
								p->encode_by_blocks({planes[0],result1,result2});
							}
							p->set_last_frame({planes[0],result1,result2});
						}
						cap >> frame;
						count++;
						n_of_current_frame++;
						
					}
	   				break;
	   			case 1:
	   				while(!frame.empty()){
						vector<Mat> planes;
						if (type == 0) {
							if (n_of_current_frame % period == 0){
								split(frame, planes);
								p->encodeJPEG2(planes[0]);
								p->encodeJPEG2(planes[1]);
								p->encodeJPEG2(planes[2]);
							}else{
								split(frame, planes);
								p->encode_by_blocks(planes);
							}
							p->set_last_frame(planes);
						} else if (type == 1){
							cvtColor(frame, frame, COLOR_RGB2YUV );
							split(frame, planes);
							int linhas = planes[0].rows;
							int colunas = planes[0].cols;
							Mat result1(linhas,colunas/2,0);
							Mat result2(linhas,colunas/2,0);
							for(int c = 0; c < linhas; c++){
								for(int i = 0; i < colunas; i+=2){
									result1.at<uchar>(c,i/2) = planes[1].at<uchar>(c,i);
									result2.at<uchar>(c,i/2) = planes[2].at<uchar>(c,i);
								}
							}
							if (n_of_current_frame % period == 0){
								p->encodeJPEG2(planes[0]);
								p->encodeJPEG2(result1);
								p->encodeJPEG2(result2);
							}else{
								p->encode_by_blocks({planes[0],result1,result2});
							}
							p->set_last_frame({planes[0],result1,result2});
						}else if (type == 2){
							cvtColor(frame, frame, COLOR_RGB2YUV );
							split(frame, planes);
							int linhas = planes[0].rows;
							int colunas = planes[0].cols;
							Mat result1(linhas/2,colunas/2,0);
							Mat result2(linhas/2,colunas/2,0);
							for(int c = 0; c < linhas; c+=2){
								for(int i = 0; i < colunas; i+=2){
									result1.at<uchar>(c/2,i/2) = planes[1].at<uchar>(c,i);
									result2.at<uchar>(c/2,i/2) = planes[2].at<uchar>(c,i);
								}
							}
							if (n_of_current_frame % period == 0){
								p->encodeJPEG2(planes[0]);
								p->encodeJPEG2(result1);
								p->encodeJPEG2(result2);
							}else{
								p->encode_by_blocks({planes[0],result1,result2});
							}
							p->set_last_frame({planes[0],result1,result2});
						}
						cap >> frame;
						count++;
						n_of_current_frame++;
						
					}

	   				break;

		   		case 2:
	   				while(!frame.empty()){
						vector<Mat> planes;
						if (type == 0) {
							if (n_of_current_frame % period == 0){
								split(frame, planes);
								p->encodeJPEG3(planes[0]);
								p->encodeJPEG3(planes[1]);
								p->encodeJPEG3(planes[2]);
							}else{
								split(frame, planes);
								p->encode_by_blocks(planes);
							}
							p->set_last_frame(planes);
						} else if (type == 1){
							cvtColor(frame, frame, COLOR_RGB2YUV );
							split(frame, planes);
							int linhas = planes[0].rows;
							int colunas = planes[0].cols;
							Mat result1(linhas,colunas/2,0);
							Mat result2(linhas,colunas/2,0);
							for(int c = 0; c < linhas; c++){
								for(int i = 0; i < colunas; i+=2){
									result1.at<uchar>(c,i/2) = planes[1].at<uchar>(c,i);
									result2.at<uchar>(c,i/2) = planes[2].at<uchar>(c,i);
								}
							}
							if (n_of_current_frame % period == 0){
								p->encodeJPEG3(planes[0]);
								p->encodeJPEG3(result1);
								p->encodeJPEG3(result2);
							}else{
								p->encode_by_blocks({planes[0],result1,result2});
							}
							p->set_last_frame({planes[0],result1,result2});
						}else if (type == 2){
							cvtColor(frame, frame, COLOR_RGB2YUV );
							split(frame, planes);
							int linhas = planes[0].rows;
							int colunas = planes[0].cols;
							Mat result1(linhas/2,colunas/2,0);
							Mat result2(linhas/2,colunas/2,0);
							for(int c = 0; c < linhas; c+=2){
								for(int i = 0; i < colunas; i+=2){
									result1.at<uchar>(c/2,i/2) = planes[1].at<uchar>(c,i);
									result2.at<uchar>(c/2,i/2) = planes[2].at<uchar>(c,i);
								}
							}
							if (n_of_current_frame % period == 0){
								p->encodeJPEG3(planes[0]);
								p->encodeJPEG3(result1);
								p->encodeJPEG3(result2);
							}else{
								p->encode_by_blocks({planes[0],result1,result2});
							}
							p->set_last_frame({planes[0],result1,result2});
						}
						cap >> frame;
						count++;
						n_of_current_frame++;
						
					}

	   				break;

	  	   		case 3:
	   				while(!frame.empty()){
						vector<Mat> planes;
						if (type == 0) {
							if (n_of_current_frame % period == 0){
								split(frame, planes);
								p->encodeJPEG4(planes[0]);
								p->encodeJPEG4(planes[1]);
								p->encodeJPEG4(planes[2]);
							}else{
								split(frame, planes);
								p->encode_by_blocks(planes);
							}
							p->set_last_frame(planes);
						} else if (type == 1){
							cvtColor(frame, frame, COLOR_RGB2YUV );
							split(frame, planes);
							int linhas = planes[0].rows;
							int colunas = planes[0].cols;
							Mat result1(linhas,colunas/2,0);
							Mat result2(linhas,colunas/2,0);
							for(int c = 0; c < linhas; c++){
								for(int i = 0; i < colunas; i+=2){
									result1.at<uchar>(c,i/2) = planes[1].at<uchar>(c,i);
									result2.at<uchar>(c,i/2) = planes[2].at<uchar>(c,i);
								}
							}
							if (n_of_current_frame % period == 0){
								p->encodeJPEG4(planes[0]);
								p->encodeJPEG4(result1);
								p->encodeJPEG4(result2);
							}else{
								p->encode_by_blocks({planes[0],result1,result2});
							}
							p->set_last_frame({planes[0],result1,result2});
						}else if (type == 2){
							cvtColor(frame, frame, COLOR_RGB2YUV );
							split(frame, planes);
							int linhas = planes[0].rows;
							int colunas = planes[0].cols;
							Mat result1(linhas/2,colunas/2,0);
							Mat result2(linhas/2,colunas/2,0);
							for(int c = 0; c < linhas; c+=2){
								for(int i = 0; i < colunas; i+=2){
									result1.at<uchar>(c/2,i/2) = planes[1].at<uchar>(c,i);
									result2.at<uchar>(c/2,i/2) = planes[2].at<uchar>(c,i);
								}
							}
							if (n_of_current_frame % period == 0){
								p->encodeJPEG4(planes[0]);
								p->encodeJPEG4(result1);
								p->encodeJPEG4(result2);
							}else{
								p->encode_by_blocks({planes[0],result1,result2});
							}
							p->set_last_frame({planes[0],result1,result2});
						}
						cap >> frame;
						count++;
						n_of_current_frame++;
						
					}

	   				break;

		   		case 4:
	   				while(!frame.empty()){
						vector<Mat> planes;
						if (type == 0) {
							if (n_of_current_frame % period == 0){
								split(frame, planes);
								p->encodeJPEG5(planes[0]);
								p->encodeJPEG5(planes[1]);
								p->encodeJPEG5(planes[2]);
							}else{
								split(frame, planes);
								p->encode_by_blocks(planes);
							}
							p->set_last_frame(planes);
						} else if (type == 1){
							cvtColor(frame, frame, COLOR_RGB2YUV );
							split(frame, planes);
							int linhas = planes[0].rows;
							int colunas = planes[0].cols;
							Mat result1(linhas,colunas/2,0);
							Mat result2(linhas,colunas/2,0);
							for(int c = 0; c < linhas; c++){
								for(int i = 0; i < colunas; i+=2){
									result1.at<uchar>(c,i/2) = planes[1].at<uchar>(c,i);
									result2.at<uchar>(c,i/2) = planes[2].at<uchar>(c,i);
								}
							}
							if (n_of_current_frame % period == 0){
								p->encodeJPEG5(planes[0]);
								p->encodeJPEG5(result1);
								p->encodeJPEG5(result2);
							}else{
								p->encode_by_blocks({planes[0],result1,result2});
							}
							p->set_last_frame({planes[0],result1,result2});
						}else if (type == 2){
							cvtColor(frame, frame, COLOR_RGB2YUV );
							split(frame, planes);
							int linhas = planes[0].rows;
							int colunas = planes[0].cols;
							Mat result1(linhas/2,colunas/2,0);
							Mat result2(linhas/2,colunas/2,0);
							for(int c = 0; c < linhas; c+=2){
								for(int i = 0; i < colunas; i+=2){
									result1.at<uchar>(c/2,i/2) = planes[1].at<uchar>(c,i);
									result2.at<uchar>(c/2,i/2) = planes[2].at<uchar>(c,i);
								}
							}
							if (n_of_current_frame % period == 0){
								p->encodeJPEG5(planes[0]);
								p->encodeJPEG5(result1);
								p->encodeJPEG5(result2);
							}else{
								p->encode_by_blocks({planes[0],result1,result2});
							}
							p->set_last_frame({planes[0],result1,result2});
						}
						cap >> frame;
						count++;
						n_of_current_frame++;
						
					}

	   				break;

	  			case 5:
	   				while(!frame.empty()){
						vector<Mat> planes;
						if (type == 0) {
							if (n_of_current_frame % period == 0){
								split(frame, planes);
								p->encodeJPEG6(planes[0]);
								p->encodeJPEG6(planes[1]);
								p->encodeJPEG6(planes[2]);
							}else{
								split(frame, planes);
								p->encode_by_blocks(planes);
							}
							p->set_last_frame(planes);
						} else if (type == 1){
							cvtColor(frame, frame, COLOR_RGB2YUV );
							split(frame, planes);
							int linhas = planes[0].rows;
							int colunas = planes[0].cols;
							Mat result1(linhas,colunas/2,0);
							Mat result2(linhas,colunas/2,0);
							for(int c = 0; c < linhas; c++){
								for(int i = 0; i < colunas; i+=2){
									result1.at<uchar>(c,i/2) = planes[1].at<uchar>(c,i);
									result2.at<uchar>(c,i/2) = planes[2].at<uchar>(c,i);
								}
							}
							if (n_of_current_frame % period == 0){
								p->encodeJPEG6(planes[0]);
								p->encodeJPEG6(result1);
								p->encodeJPEG6(result2);
							}else{
								p->encode_by_blocks({planes[0],result1,result2});
							}
							p->set_last_frame({planes[0],result1,result2});
						}else if (type == 2){
							cvtColor(frame, frame, COLOR_RGB2YUV );
							split(frame, planes);
							int linhas = planes[0].rows;
							int colunas = planes[0].cols;
							Mat result1(linhas/2,colunas/2,0);
							Mat result2(linhas/2,colunas/2,0);
							for(int c = 0; c < linhas; c+=2){
								for(int i = 0; i < colunas; i+=2){
									result1.at<uchar>(c/2,i/2) = planes[1].at<uchar>(c,i);
									result2.at<uchar>(c/2,i/2) = planes[2].at<uchar>(c,i);
								}
							}
							if (n_of_current_frame % period == 0){
								p->encodeJPEG6(planes[0]);
								p->encodeJPEG6(result1);
								p->encodeJPEG6(result2);
							}else{
								p->encode_by_blocks({planes[0],result1,result2});
							}
							p->set_last_frame({planes[0],result1,result2});
						}
						cap >> frame;
						count++;
						n_of_current_frame++;
						
					}

	   				break;

 	   			case 6:
	   				while(!frame.empty()){
						vector<Mat> planes;
						if (type == 0) {
							if (n_of_current_frame % period == 0){
								split(frame, planes);
								p->encodeJPEG7(planes[0]);
								p->encodeJPEG7(planes[1]);
								p->encodeJPEG7(planes[2]);
							}else{
								split(frame, planes);
								p->encode_by_blocks(planes);
							}
							p->set_last_frame(planes);
						} else if (type == 1){
							cvtColor(frame, frame, COLOR_RGB2YUV );
							split(frame, planes);
							int linhas = planes[0].rows;
							int colunas = planes[0].cols;
							Mat result1(linhas,colunas/2,0);
							Mat result2(linhas,colunas/2,0);
							for(int c = 0; c < linhas; c++){
								for(int i = 0; i < colunas; i+=2){
									result1.at<uchar>(c,i/2) = planes[1].at<uchar>(c,i);
									result2.at<uchar>(c,i/2) = planes[2].at<uchar>(c,i);
								}
							}
							if (n_of_current_frame % period == 0){
								p->encodeJPEG7(planes[0]);
								p->encodeJPEG7(result1);
								p->encodeJPEG7(result2);
							}else{
								p->encode_by_blocks({planes[0],result1,result2});
							}
							p->set_last_frame({planes[0],result1,result2});
						}else if (type == 2){
							cvtColor(frame, frame, COLOR_RGB2YUV );
							split(frame, planes);
							int linhas = planes[0].rows;
							int colunas = planes[0].cols;
							Mat result1(linhas/2,colunas/2,0);
							Mat result2(linhas/2,colunas/2,0);
							for(int c = 0; c < linhas; c+=2){
								for(int i = 0; i < colunas; i+=2){
									result1.at<uchar>(c/2,i/2) = planes[1].at<uchar>(c,i);
									result2.at<uchar>(c/2,i/2) = planes[2].at<uchar>(c,i);
								}
							}
							if (n_of_current_frame % period == 0){
								p->encodeJPEG7(planes[0]);
								p->encodeJPEG7(result1);
								p->encodeJPEG7(result2);
							}else{
								p->encode_by_blocks({planes[0],result1,result2});
							}
							p->set_last_frame({planes[0],result1,result2});
						}
						cap >> frame;
						count++;
						n_of_current_frame++;
						
					}

	   				break;	   	

	   			case 7:
		   				while(!frame.empty()){
						vector<Mat> planes;
						if (type == 0) {
							if (n_of_current_frame % period == 0){
								split(frame, planes);
								p->encodeJPEG_LS(planes[0]);
								p->encodeJPEG_LS(planes[1]);
								p->encodeJPEG_LS(planes[2]);
							}else{
								split(frame, planes);
								p->encode_by_blocks(planes);
							}
							p->set_last_frame(planes);
						} else if (type == 1){
							cvtColor(frame, frame, COLOR_RGB2YUV );
							split(frame, planes);
							int linhas = planes[0].rows;
							int colunas = planes[0].cols;
							Mat result1(linhas,colunas/2,0);
							Mat result2(linhas,colunas/2,0);
							for(int c = 0; c < linhas; c++){
								for(int i = 0; i < colunas; i+=2){
									result1.at<uchar>(c,i/2) = planes[1].at<uchar>(c,i);
									result2.at<uchar>(c,i/2) = planes[2].at<uchar>(c,i);
								}
							}
							if (n_of_current_frame % period == 0){
								p->encodeJPEG_LS(planes[0]);
								p->encodeJPEG_LS(result1);
								p->encodeJPEG_LS(result2);
							}else{
								p->encode_by_blocks({planes[0],result1,result2});
							}
							p->set_last_frame({planes[0],result1,result2});
						}else if (type == 2){
							cvtColor(frame, frame, COLOR_RGB2YUV );
							split(frame, planes);
							int linhas = planes[0].rows;
							int colunas = planes[0].cols;
							Mat result1(linhas/2,colunas/2,0);
							Mat result2(linhas/2,colunas/2,0);
							for(int c = 0; c < linhas; c+=2){
								for(int i = 0; i < colunas; i+=2){
									result1.at<uchar>(c/2,i/2) = planes[1].at<uchar>(c,i);
									result2.at<uchar>(c/2,i/2) = planes[2].at<uchar>(c,i);
								}
							}
							if (n_of_current_frame % period == 0){
								p->encodeJPEG_LS(planes[0]);
								p->encodeJPEG_LS(result1);
								p->encodeJPEG_LS(result2);
							}else{
								p->encode_by_blocks({planes[0],result1,result2});
							}
							p->set_last_frame({planes[0],result1,result2});
						}
						cap >> frame;
						count++;
						n_of_current_frame++;
						
					}
	   				break;
	   		}
	   		p->close();
	   	}
	   	
	   	int decode(){

			/**
			 * Function to decode the file
			 */

	   		Mat result;
			Mat m1;
	   		Mat m2;
	   		Mat m3;
	   		vector<Mat> channels ;
	   		int fra = p->get_frames();
	   		int mode = p->get_type();
	   		int tipo = p->get_VideoType();
	   		int periodo = p->get_period();
	   		int count = 0;
			switch (mode){
	   			case 0:
					for(int b = 0; b<fra;b++){
						if (tipo == 0){
							if (count % periodo == 0){
								m1 = p->decodeJPEG1();
			   					m2 = p->decodeJPEG1();
			   					m3 = p->decodeJPEG1();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame(channels);
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								p->set_last_frame(channels);
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}else if (tipo == 1){
							if (count % periodo == 0){
								m1 = p->decodeJPEG1();
			   					m2 = p->decodeJPEG1();
			   					m3 = p->decodeJPEG1();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								int linhas = m1.rows;
								int colunas = m1.cols;
								Mat result1(linhas,colunas/2,0);
								Mat result2(linhas,colunas/2,0);
								for(int c = 0; c < linhas; c++){
									for(int i = 0; i < colunas; i+=2){
										result1.at<uchar>(c,i/2) = m2.at<uchar>(c,i);
										result2.at<uchar>(c,i/2) = m3.at<uchar>(c,i);
									}
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}else if (tipo == 2){
							if (count % periodo == 0){
								m1 = p->decodeJPEG1();
			   					m2 = p->decodeJPEG1();
			   					m3 = p->decodeJPEG1();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								int linhas = m1.rows;
								int colunas = m1.cols;
								Mat result1(linhas/2,colunas/2,0);
								Mat result2(linhas/2,colunas/2,0);
								for(int c = 0; c < linhas; c+=2){
									for(int i = 0; i < colunas; i+=2){
										result1.at<uchar>(c/2,i/2) = m2.at<uchar>(c,i);
										result2.at<uchar>(c/2,i/2) = m3.at<uchar>(c,i);
									}
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}
					}
	   				break;
	   			case 1:
	   				for(int b = 0; b<fra;b++){
						if (tipo == 0){
							if (count % periodo == 0){
								m1 = p->decodeJPEG2();
			   					m2 = p->decodeJPEG2();
			   					m3 = p->decodeJPEG2();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame(channels);
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								p->set_last_frame(channels);
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}else if (tipo == 1){
							if (count % periodo == 0){
								m1 = p->decodeJPEG2();
			   					m2 = p->decodeJPEG2();
			   					m3 = p->decodeJPEG2();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								int linhas = m1.rows;
								int colunas = m1.cols;
								Mat result1(linhas,colunas/2,0);
								Mat result2(linhas,colunas/2,0);
								for(int c = 0; c < linhas; c++){
									for(int i = 0; i < colunas; i+=2){
										result1.at<uchar>(c,i/2) = m2.at<uchar>(c,i);
										result2.at<uchar>(c,i/2) = m3.at<uchar>(c,i);
									}
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}else if (tipo == 2){
							if (count % periodo == 0){
								m1 = p->decodeJPEG2();
			   					m2 = p->decodeJPEG2();
			   					m3 = p->decodeJPEG2();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								int linhas = m1.rows;
								int colunas = m1.cols;
								Mat result1(linhas/2,colunas/2,0);
								Mat result2(linhas/2,colunas/2,0);
								for(int c = 0; c < linhas; c+=2){
									for(int i = 0; i < colunas; i+=2){
										result1.at<uchar>(c/2,i/2) = m2.at<uchar>(c,i);
										result2.at<uchar>(c/2,i/2) = m3.at<uchar>(c,i);
									}
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}
					}
	   				break;
		   		case 2:
		   			for(int b = 0; b<fra;b++){
						if (tipo == 0){
							if (count % periodo == 0){
								m1 = p->decodeJPEG3();
			   					m2 = p->decodeJPEG3();
			   					m3 = p->decodeJPEG3();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame(channels);
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								p->set_last_frame(channels);
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}else if (tipo == 1){
							if (count % periodo == 0){
								m1 = p->decodeJPEG3();
			   					m2 = p->decodeJPEG3();
			   					m3 = p->decodeJPEG3();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								int linhas = m1.rows;
								int colunas = m1.cols;
								Mat result1(linhas,colunas/2,0);
								Mat result2(linhas,colunas/2,0);
								for(int c = 0; c < linhas; c++){
									for(int i = 0; i < colunas; i+=2){
										result1.at<uchar>(c,i/2) = m2.at<uchar>(c,i);
										result2.at<uchar>(c,i/2) = m3.at<uchar>(c,i);
									}
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}else if (tipo == 2){
							if (count % periodo == 0){
								m1 = p->decodeJPEG3();
			   					m2 = p->decodeJPEG3();
			   					m3 = p->decodeJPEG3();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								int linhas = m1.rows;
								int colunas = m1.cols;
								Mat result1(linhas/2,colunas/2,0);
								Mat result2(linhas/2,colunas/2,0);
								for(int c = 0; c < linhas; c+=2){
									for(int i = 0; i < colunas; i+=2){
										result1.at<uchar>(c/2,i/2) = m2.at<uchar>(c,i);
										result2.at<uchar>(c/2,i/2) = m3.at<uchar>(c,i);
									}
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}
					}
	   				break;
	  	   		case 3:
	  	   			for(int b = 0; b<fra;b++){
						if (tipo == 0){
							if (count % periodo == 0){
								m1 = p->decodeJPEG4();
			   					m2 = p->decodeJPEG4();
			   					m3 = p->decodeJPEG4();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame(channels);
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								p->set_last_frame(channels);
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}else if (tipo == 1){
							if (count % periodo == 0){
								m1 = p->decodeJPEG4();
			   					m2 = p->decodeJPEG4();
			   					m3 = p->decodeJPEG4();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								int linhas = m1.rows;
								int colunas = m1.cols;
								Mat result1(linhas,colunas/2,0);
								Mat result2(linhas,colunas/2,0);
								for(int c = 0; c < linhas; c++){
									for(int i = 0; i < colunas; i+=2){
										result1.at<uchar>(c,i/2) = m2.at<uchar>(c,i);
										result2.at<uchar>(c,i/2) = m3.at<uchar>(c,i);
									}
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}else if (tipo == 2){
							if (count % periodo == 0){
								m1 = p->decodeJPEG4();
			   					m2 = p->decodeJPEG4();
			   					m3 = p->decodeJPEG4();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								int linhas = m1.rows;
								int colunas = m1.cols;
								Mat result1(linhas/2,colunas/2,0);
								Mat result2(linhas/2,colunas/2,0);
								for(int c = 0; c < linhas; c+=2){
									for(int i = 0; i < colunas; i+=2){
										result1.at<uchar>(c/2,i/2) = m2.at<uchar>(c,i);
										result2.at<uchar>(c/2,i/2) = m3.at<uchar>(c,i);
									}
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}
					}
	   				break;
		   		case 4:
		   			for(int b = 0; b<fra;b++){
						if (tipo == 0){
							if (count % periodo == 0){
								m1 = p->decodeJPEG5();
			   					m2 = p->decodeJPEG5();
			   					m3 = p->decodeJPEG5();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame(channels);
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								p->set_last_frame(channels);
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}else if (tipo == 1){
							if (count % periodo == 0){
								m1 = p->decodeJPEG5();
			   					m2 = p->decodeJPEG5();
			   					m3 = p->decodeJPEG5();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								int linhas = m1.rows;
								int colunas = m1.cols;
								Mat result1(linhas,colunas/2,0);
								Mat result2(linhas,colunas/2,0);
								for(int c = 0; c < linhas; c++){
									for(int i = 0; i < colunas; i+=2){
										result1.at<uchar>(c,i/2) = m2.at<uchar>(c,i);
										result2.at<uchar>(c,i/2) = m3.at<uchar>(c,i);
									}
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}else if (tipo == 2){
							if (count % periodo == 0){
								m1 = p->decodeJPEG5();
			   					m2 = p->decodeJPEG5();
			   					m3 = p->decodeJPEG5();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								int linhas = m1.rows;
								int colunas = m1.cols;
								Mat result1(linhas/2,colunas/2,0);
								Mat result2(linhas/2,colunas/2,0);
								for(int c = 0; c < linhas; c+=2){
									for(int i = 0; i < colunas; i+=2){
										result1.at<uchar>(c/2,i/2) = m2.at<uchar>(c,i);
										result2.at<uchar>(c/2,i/2) = m3.at<uchar>(c,i);
									}
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}
					}
	   				break;
	  			case 5:
	  				for(int b = 0; b<fra;b++){
						if (tipo == 0){
							if (count % periodo == 0){
								m1 = p->decodeJPEG6();
			   					m2 = p->decodeJPEG6();
			   					m3 = p->decodeJPEG6();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame(channels);
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								p->set_last_frame(channels);
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}else if (tipo == 1){
							if (count % periodo == 0){
								m1 = p->decodeJPEG6();
			   					m2 = p->decodeJPEG6();
			   					m3 = p->decodeJPEG6();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								int linhas = m1.rows;
								int colunas = m1.cols;
								Mat result1(linhas,colunas/2,0);
								Mat result2(linhas,colunas/2,0);
								for(int c = 0; c < linhas; c++){
									for(int i = 0; i < colunas; i+=2){
										result1.at<uchar>(c,i/2) = m2.at<uchar>(c,i);
										result2.at<uchar>(c,i/2) = m3.at<uchar>(c,i);
									}
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}else if (tipo == 2){
							if (count % periodo == 0){
								m1 = p->decodeJPEG6();
			   					m2 = p->decodeJPEG6();
			   					m3 = p->decodeJPEG6();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								int linhas = m1.rows;
								int colunas = m1.cols;
								Mat result1(linhas/2,colunas/2,0);
								Mat result2(linhas/2,colunas/2,0);
								for(int c = 0; c < linhas; c+=2){
									for(int i = 0; i < colunas; i+=2){
										result1.at<uchar>(c/2,i/2) = m2.at<uchar>(c,i);
										result2.at<uchar>(c/2,i/2) = m3.at<uchar>(c,i);
									}
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}
					}
	   				break;
	   			case 6:
	   				for(int b = 0; b<fra;b++){
						if (tipo == 0){
							if (count % periodo == 0){
								m1 = p->decodeJPEG7();
			   					m2 = p->decodeJPEG7();
			   					m3 = p->decodeJPEG7();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame(channels);
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								p->set_last_frame(channels);
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}else if (tipo == 1){
							if (count % periodo == 0){
								m1 = p->decodeJPEG7();
			   					m2 = p->decodeJPEG7();
			   					m3 = p->decodeJPEG7();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								int linhas = m1.rows;
								int colunas = m1.cols;
								Mat result1(linhas,colunas/2,0);
								Mat result2(linhas,colunas/2,0);
								for(int c = 0; c < linhas; c++){
									for(int i = 0; i < colunas; i+=2){
										result1.at<uchar>(c,i/2) = m2.at<uchar>(c,i);
										result2.at<uchar>(c,i/2) = m3.at<uchar>(c,i);
									}
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}else if (tipo == 2){
							if (count % periodo == 0){
								m1 = p->decodeJPEG7();
			   					m2 = p->decodeJPEG7();
			   					m3 = p->decodeJPEG7();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								int linhas = m1.rows;
								int colunas = m1.cols;
								Mat result1(linhas/2,colunas/2,0);
								Mat result2(linhas/2,colunas/2,0);
								for(int c = 0; c < linhas; c+=2){
									for(int i = 0; i < colunas; i+=2){
										result1.at<uchar>(c/2,i/2) = m2.at<uchar>(c,i);
										result2.at<uchar>(c/2,i/2) = m3.at<uchar>(c,i);
									}
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}
					}
	   				break;   					   					
	   			case 7:
	   				for(int b = 0; b<fra;b++){
						if (tipo == 0){
							if (count % periodo == 0){
								m1 = p->decodeJPEGLS();
			   					m2 = p->decodeJPEGLS();
			   					m3 = p->decodeJPEGLS();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame(channels);
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								p->set_last_frame(channels);
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}else if (tipo == 1){
							if (count % periodo == 0){
								m1 = p->decodeJPEGLS();
			   					m2 = p->decodeJPEGLS();
			   					m3 = p->decodeJPEGLS();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								int linhas = m1.rows;
								int colunas = m1.cols;
								Mat result1(linhas,colunas/2,0);
								Mat result2(linhas,colunas/2,0);
								for(int c = 0; c < linhas; c++){
									for(int i = 0; i < colunas; i+=2){
										result1.at<uchar>(c,i/2) = m2.at<uchar>(c,i);
										result2.at<uchar>(c,i/2) = m3.at<uchar>(c,i);
									}
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}else if (tipo == 2){
							if (count % periodo == 0){
								m1 = p->decodeJPEGLS();
			   					m2 = p->decodeJPEGLS();
			   					m3 = p->decodeJPEGLS();
			   					channels = {m1,m2,m3};
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								int linhas = m1.rows;
								int colunas = m1.cols;
								Mat result1(linhas/2,colunas/2,0);
								Mat result2(linhas/2,colunas/2,0);
								for(int c = 0; c < linhas; c+=2){
									for(int i = 0; i < colunas; i+=2){
										result1.at<uchar>(c/2,i/2) = m2.at<uchar>(c,i);
										result2.at<uchar>(c/2,i/2) = m3.at<uchar>(c,i);
									}
								}
								cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							}else{
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame by blocks: " << b+1 << endl;
							}
							count++;
						}
					}
	   				break;
	   		}
	   		return 1;
	   	}	
};



















