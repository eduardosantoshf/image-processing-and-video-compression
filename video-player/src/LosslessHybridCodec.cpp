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
		int current_frame_number;
		Predictor *p;
		
	public: 

	   	LosslessHybridCodec(string v, int bs, int ss, int tv, int tp, int M, int pe, int f, string outFile){

			/**
			 * LosslessHybridCodec Constructor
			 * 
			 * @param v name of the file to be encoded
			 * @param bs block size
			 * @param ss search space
			 * @param tv type of the video
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
	   		current_frame_number = 0;
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
	   		} else {
	   			p = new Predictor(0,0,outFile,0,0,0,0,0,0);
	   		}

	   	}

		vector<Mat> RGB_to_YUV422(Mat frame, vector<Mat> planes) {

			/**
			 * Function to convert from RGB to YUV 4:2:2
			 * 
			 * @param frame frame to be converted
			 * @param planes vector with split channels
			 */

			cvtColor(frame, frame, COLOR_RGB2YUV);
			split(frame, planes);

			int lines = planes[0].rows;
			int columns = planes[0].cols;

			Mat new_plane_1(lines,columns / 2, 0);
			Mat new_plane_2(lines,columns / 2, 0);

			for(int c = 0; c < lines; c++){
				for(int i = 0; i < columns; i += 2){
					new_plane_1.at<uchar>(c, i / 2) = planes[1].at<uchar>(c, i);
					new_plane_2.at<uchar>(c, i / 2) = planes[2].at<uchar>(c, i);
				}
			}

			vector<Mat> yuv422_planes = {planes[0], new_plane_1, new_plane_2};

			return yuv422_planes;
		}

		vector<Mat> RGB_to_YUV420(Mat frame, vector<Mat> planes) {

			/**
			 * Function to convert from RGB to YUV 4:2:0
			 * 
			 * @param frame frame to be converted
			 * @param planes vector with split channels
			 */

			cvtColor(frame, frame, COLOR_RGB2YUV );
			split(frame, planes);

			int lines = planes[0].rows;
			int columns = planes[0].cols;

			Mat new_plane_1(lines / 2, columns / 2, 0);
			Mat new_plane_2(lines / 2, columns / 2, 0);

			for(int c = 0; c < lines; c += 2){
				for(int i = 0; i < columns; i += 2){
					new_plane_1.at<uchar>(c / 2,i / 2) = planes[1].at<uchar>(c, i);
					new_plane_2.at<uchar>(c / 2,i / 2) = planes[2].at<uchar>(c, i);
				}
			}

			vector<Mat> yuv420_planes = {planes[0], new_plane_1, new_plane_2};

			return yuv420_planes;
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
						vector<Mat> new_planes;

						if (type == 0) {
							if (current_frame_number % period == 0){
								split(frame, planes);
								p->encodeJPEG1(planes[0]);
								p->encodeJPEG1(planes[1]);
								p->encodeJPEG1(planes[2]);
							} else {
								split(frame, planes);
								p->encode_by_blocks(planes);
							}
							p->set_last_frame(planes);
						} else if (type == 1){
							new_planes = RGB_to_YUV422(frame, planes);

							if (current_frame_number % period == 0){
								for (int new_p = 0; new_p < 3; new_p++) p->encodeJPEG1(new_planes[new_p]);
							}
							
							else p->encode_by_blocks({new_planes[0],new_planes[1],new_planes[2]});

							p->set_last_frame({new_planes[0],new_planes[1],new_planes[2]});

						}else if (type == 2){
							new_planes = RGB_to_YUV420(frame, planes);

							if (current_frame_number % period == 0){
								for (int new_p = 0; new_p < 3; new_p++) p->encodeJPEG1(new_planes[new_p]);
							}
							
							else p->encode_by_blocks({new_planes[0],new_planes[1],new_planes[2]});

							p->set_last_frame({new_planes[0],new_planes[1],new_planes[2]});
						}
						cap >> frame;
						cout << "Encoding frame: " << count << endl; 
						count++;
						current_frame_number++;
						
					}
	   				break;
	   			case 1:
	   				while(!frame.empty()){
						vector<Mat> planes;
						vector<Mat> new_planes;

						if (type == 0) {
							if (current_frame_number % period == 0){
								split(frame, planes);
								p->encodeJPEG2(planes[0]);
								p->encodeJPEG2(planes[1]);
								p->encodeJPEG2(planes[2]);
							} else {
								split(frame, planes);
								p->encode_by_blocks(planes);
							}
							p->set_last_frame(planes);
						} else if (type == 1){
							new_planes = RGB_to_YUV422(frame, planes);

							if (current_frame_number % period == 0){
								for (int new_p = 0; new_p < 3; new_p++) p->encodeJPEG2(new_planes[new_p]);
							}
							
							else p->encode_by_blocks({new_planes[0],new_planes[1],new_planes[2]});

							p->set_last_frame({new_planes[0],new_planes[1],new_planes[2]});

						}else if (type == 2){
							new_planes = RGB_to_YUV420(frame, planes);

							if (current_frame_number % period == 0){
								for (int new_p = 0; new_p < 3; new_p++) p->encodeJPEG2(new_planes[new_p]);
							}
							
							else p->encode_by_blocks({new_planes[0],new_planes[1],new_planes[2]});
							
							p->set_last_frame({new_planes[0],new_planes[1],new_planes[2]});
						}
						cap >> frame;
						cout << "Encoding frame: " << count << endl; 
						count++;
						current_frame_number++;
						
					}

	   				break;

		   		case 2:
	   				while(!frame.empty()){
						vector<Mat> planes;
						vector<Mat> new_planes;

						if (type == 0) {
							if (current_frame_number % period == 0){
								split(frame, planes);
								p->encodeJPEG3(planes[0]);
								p->encodeJPEG3(planes[1]);
								p->encodeJPEG3(planes[2]);
							} else {
								split(frame, planes);
								p->encode_by_blocks(planes);
							}
							p->set_last_frame(planes);
						} else if (type == 1){
							new_planes = RGB_to_YUV422(frame, planes);

							if (current_frame_number % period == 0){
								for (int new_p = 0; new_p < 3; new_p++) p->encodeJPEG3(new_planes[new_p]);
							}
							
							else p->encode_by_blocks({new_planes[0],new_planes[1],new_planes[2]});

							p->set_last_frame({new_planes[0],new_planes[1],new_planes[2]});

						}else if (type == 2){
							new_planes = RGB_to_YUV420(frame, planes);

							if (current_frame_number % period == 0){
								for (int new_p = 0; new_p < 3; new_p++) p->encodeJPEG3(new_planes[new_p]);
							}
							
							else p->encode_by_blocks({new_planes[0],new_planes[1],new_planes[2]});
							
							p->set_last_frame({new_planes[0],new_planes[1],new_planes[2]});
						}
						cap >> frame;
						cout << "Encoding frame: " << count << endl; 
						count++;
						current_frame_number++;
						
					}

	   				break;

	  	   		case 3:
	   				while(!frame.empty()){
						vector<Mat> planes;
						vector<Mat> new_planes;

						if (type == 0) {
							if (current_frame_number % period == 0){
								split(frame, planes);
								p->encodeJPEG4(planes[0]);
								p->encodeJPEG4(planes[1]);
								p->encodeJPEG4(planes[2]);
							} else {
								split(frame, planes);
								p->encode_by_blocks(planes);
							}
							p->set_last_frame(planes);
						} else if (type == 1){
							new_planes = RGB_to_YUV422(frame, planes);

							if (current_frame_number % period == 0){
								for (int new_p = 0; new_p < 3; new_p++) p->encodeJPEG4(new_planes[new_p]);
							}
							
							else p->encode_by_blocks({new_planes[0],new_planes[1],new_planes[2]});

							p->set_last_frame({new_planes[0],new_planes[1],new_planes[2]});

						}else if (type == 2){
							new_planes = RGB_to_YUV420(frame, planes);

							if (current_frame_number % period == 0){
								for (int new_p = 0; new_p < 3; new_p++) p->encodeJPEG4(new_planes[new_p]);
							}
							
							else p->encode_by_blocks({new_planes[0],new_planes[1],new_planes[2]});
							
							p->set_last_frame({new_planes[0],new_planes[1],new_planes[2]});
						}
						cap >> frame;
						cout << "Encoding frame: " << count << endl; 
						count++;
						current_frame_number++;
						
					}

	   				break;

		   		case 4:
	   				while(!frame.empty()){
						vector<Mat> planes;
						vector<Mat> new_planes;

						if (type == 0) {
							if (current_frame_number % period == 0){
								split(frame, planes);
								p->encodeJPEG5(planes[0]);
								p->encodeJPEG5(planes[1]);
								p->encodeJPEG5(planes[2]);
							} else {
								split(frame, planes);
								p->encode_by_blocks(planes);
							}
							p->set_last_frame(planes);
						} else if (type == 1){
							new_planes = RGB_to_YUV422(frame, planes);

							if (current_frame_number % period == 0){
								for (int new_p = 0; new_p < 3; new_p++) p->encodeJPEG5(new_planes[new_p]);
							}
							
							else p->encode_by_blocks({new_planes[0],new_planes[1],new_planes[2]});

							p->set_last_frame({new_planes[0],new_planes[1],new_planes[2]});

						}else if (type == 2){
							new_planes = RGB_to_YUV420(frame, planes);

							if (current_frame_number % period == 0){
								for (int new_p = 0; new_p < 3; new_p++) p->encodeJPEG5(new_planes[new_p]);
							}
							
							else p->encode_by_blocks({new_planes[0],new_planes[1],new_planes[2]});
							
							p->set_last_frame({new_planes[0],new_planes[1],new_planes[2]});
						}
						cap >> frame;
						cout << "Encoding frame: " << count << endl; 
						count++;
						current_frame_number++;
						
					}

	   				break;

	  			case 5:
	   				while(!frame.empty()){
						vector<Mat> planes;
						vector<Mat> new_planes;

						if (type == 0) {
							if (current_frame_number % period == 0){
								split(frame, planes);
								p->encodeJPEG6(planes[0]);
								p->encodeJPEG6(planes[1]);
								p->encodeJPEG6(planes[2]);
							} else {
								split(frame, planes);
								p->encode_by_blocks(planes);
							}
							p->set_last_frame(planes);
						} else if (type == 1){
							new_planes = RGB_to_YUV422(frame, planes);

							if (current_frame_number % period == 0){
								for (int new_p = 0; new_p < 3; new_p++) p->encodeJPEG6(new_planes[new_p]);
							}
							
							else p->encode_by_blocks({new_planes[0],new_planes[1],new_planes[2]});

							p->set_last_frame({new_planes[0],new_planes[1],new_planes[2]});

						}else if (type == 2){
							new_planes = RGB_to_YUV420(frame, planes);

							if (current_frame_number % period == 0){
								for (int new_p = 0; new_p < 3; new_p++) p->encodeJPEG6(new_planes[new_p]);
							}
							
							else p->encode_by_blocks({new_planes[0],new_planes[1],new_planes[2]});
							
							p->set_last_frame({new_planes[0],new_planes[1],new_planes[2]});
						}
						cap >> frame;
						cout << "Encoding frame: " << count << endl; 
						count++;
						current_frame_number++;
						
					}

	   				break;

 	   			case 6:
	   				while(!frame.empty()){
						vector<Mat> planes;
						vector<Mat> new_planes;

						if (type == 0) {
							if (current_frame_number % period == 0){
								split(frame, planes);
								p->encodeJPEG7(planes[0]);
								p->encodeJPEG7(planes[1]);
								p->encodeJPEG7(planes[2]);
							} else {
								split(frame, planes);
								p->encode_by_blocks(planes);
							}
							p->set_last_frame(planes);
						} else if (type == 1){
							new_planes = RGB_to_YUV422(frame, planes);

							if (current_frame_number % period == 0){
								for (int new_p = 0; new_p < 3; new_p++) p->encodeJPEG7(new_planes[new_p]);
							}
							
							else p->encode_by_blocks({new_planes[0],new_planes[1],new_planes[2]});

							p->set_last_frame({new_planes[0],new_planes[1],new_planes[2]});
						}else if (type == 2){
							new_planes = RGB_to_YUV420(frame, planes);

							if (current_frame_number % period == 0){
								for (int new_p = 0; new_p < 3; new_p++) p->encodeJPEG7(new_planes[new_p]);
							}
							
							else p->encode_by_blocks({new_planes[0],new_planes[1],new_planes[2]});
							
							p->set_last_frame({new_planes[0],new_planes[1],new_planes[2]});
						}
						cap >> frame;
						cout << "Encoding frame: " << count << endl; 
						count++;
						current_frame_number++;
						
					}

	   				break;	   	

	   			case 7:
		   			while(!frame.empty()){
						vector<Mat> planes;
						vector<Mat> new_planes;
						
						if (type == 0) {
							if (current_frame_number % period == 0){
								split(frame, planes);
								p->encodeJPEG_LS(planes[0]);
								p->encodeJPEG_LS(planes[1]);
								p->encodeJPEG_LS(planes[2]);
							} else {
								split(frame, planes);
								p->encode_by_blocks(planes);
							}
							p->set_last_frame(planes);
						} else if (type == 1){
							new_planes = RGB_to_YUV422(frame, planes);

							if (current_frame_number % period == 0){
								for (int new_p = 0; new_p < 3; new_p++) p->encodeJPEG_LS(new_planes[new_p]);
							}
							
							else p->encode_by_blocks({new_planes[0],new_planes[1],new_planes[2]});

							p->set_last_frame({new_planes[0],new_planes[1],new_planes[2]});

						}else if (type == 2){
							new_planes = RGB_to_YUV420(frame, planes);

							if (current_frame_number % period == 0){
								for (int new_p = 0; new_p < 3; new_p++) p->encodeJPEG_LS(new_planes[new_p]);
							}
							
							else p->encode_by_blocks({new_planes[0],new_planes[1],new_planes[2]});
							
							p->set_last_frame({new_planes[0],new_planes[1],new_planes[2]});
						}
						cap >> frame;
						cout << "Encoding frame: " << count << endl; 
						count++;
						current_frame_number++;
						
					}
	   				break;
	   		}
	   		p->close();
	   	}
	   	
	   	int decode(){

			/**
			 * Function to decode the file
			 */

	   		Mat result, m1, m2, m3;

	   		vector<Mat> channels;

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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame(channels);
							} else {
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
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							} else {
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								//cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							} else {
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								//cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame(channels);
							} else {
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
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							} else {
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								//cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							} else {
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								//cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame(channels);
							} else {
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
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							} else {
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								//cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							} else {
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								//cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame(channels);
							} else {
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
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							} else {
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								//cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							} else {
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								//cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame(channels);
							} else {
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
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							} else {
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								//cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							} else {
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								//cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame(channels);
							} else {
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
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							} else {
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								//cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							} else {
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								//cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame(channels);
							} else {
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
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							} else {
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								//cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							} else {
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								//cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame(channels);
							} else {
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
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							} else {
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								//cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
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
								//cout << "Decoding frame: " << b+1 << endl;
								p->set_last_frame({m1,result1,result2});
							} else {
			   					channels = p->decode_by_blocks();
			   					merge(channels,result);
			   					if (tipo != 0){
			   						cvtColor(result, result, COLOR_YUV2RGB );
			   					}
								imshow("Display",result);
								if(waitKey(30) >=0){
									break;
								}
								//cout << "Decoding frame: " << b+1 << endl;
								cout << "Decoding frame " << b + 1 << " by blocks" << endl;
							}
							count++;
						}
					}
	   				break;
	   		}
	   		return 1;
	   	}	
};