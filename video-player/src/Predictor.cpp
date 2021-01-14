#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <fstream> 
#include <vector>
#include <math.h>
#include "Golomb.cpp"
#include <string>

using namespace cv;
using namespace std;

class Predictor {
	private:
		int m;
		int type;
		int typeVideo;
		int n_frames;
		int linhas;
		int colunas;
		int start;
		int period;
		Golomb *g;
		int block_size;
		int search_space;
		vector<Mat> lastFrame;
		int od;
		string fileName;
		WBitStream *wbs;
	public: 
	
	   	Predictor(int Type, int M,string file,int TypeVideo,int n_f,int flag,int bs,int ss,int peri){

			/**
			 * Predictor Constructor
			 * 
			 * @param Type type of the predictor
			 * @param M value of m
			 * @param file given file name
			 * @param TypeVideo type of video
			 * @param n_f number of frames
			 * @param flag flag
			 * @param bs block size
			 * @param ss search space
			 * @param peri periodicity
			 */

	   		fileName=file;
	   		if (flag == 1){
	   			block_size = bs;
	   			search_space = ss;
		   		m = M;
		   		type = Type;
		   		typeVideo = TypeVideo;
		   		start = 1;
		   		n_frames = n_f;
		   		period = peri;
		   		wbs = new WBitStream(file);
		   		wbs->writeNBits(TypeVideo,8);
		   		wbs->writeNBits(type,8);
		   		wbs->writeNBits(M,8);
		   		wbs->writeNBits(bs,8);
		   		wbs->writeNBits(ss,8);
		   		wbs->writeNBits(period,8);
		   	}else{
		   		RBitStream rbs(file);
		   		od = 0;

		   		typeVideo = rbs.readNBits(8);
		   		type = rbs.readNBits(8);
		   		m = rbs.readNBits(8);
		   		block_size = rbs.readNBits(8);
		   		search_space = rbs.readNBits(8);
		   		period = rbs.readNBits(8);
		   		colunas = stoi(rbs.readString(4));
		   		linhas = stoi(rbs.readString(4));
		   		n_frames = stoi(rbs.readString(4));

		   		cout << "Type of video: " << typeVideo << endl;
		   		cout << "Type of predictor: " << type << endl;
		   		cout << "M: " << m << endl;
		   		cout << "Columns: " << colunas << endl;
		   		cout << "Lines: " << linhas<< endl;
		   		cout << "Number of frames: " << n_frames<< endl;
		   		cout << "Search space: " << search_space << endl;
		   		cout << "Block size: " << block_size << endl;
		   		cout << "Period: " << period << endl;

		   		rbs.close();
		   		g = new Golomb(file,m,0);
				g->SkipNBytes(18);
		   	}
	   	}
	    
	    	int get_frames(){

				/**
				 * Function to get the number of frames
				 */

	    		return n_frames;
	    	}
	    	
	    	int get_type(){

				/**
				 * Function to get the predictor type
				 */
				
	    		return type;
	    	}
	    	
	    	int get_VideoType(){

				/**
				 * Function to get the video type
				 */

	    		return typeVideo;
	    	}
	    	
	    	void set_last_frame(vector<Mat> planes){

				/**
				 * Function to set the last frame as the given frame
				 * 
				 * @param planes channels of the frame
				 */

				lastFrame = planes;
	    	}
	    	
	    	int get_period(){

				/**
				 * Function to get the periodicity
				 */

	    		return period;
	    	}
	    	
	    	void close(){

				/**
				 * Function to close the golomb
				 */

	    		g->close();
	    	}
	    	
	    	void encode_by_blocks(vector<Mat> planes){

				/**
				 * Function to encode the given frame by blocks
				 * 
				 * @param planes channels of the frame
				 */

				 Mat current_block, possible_block, min_block;

				int lin, col;
				int count = 0;
				int count_blocks = 0;
				int lastSum = 100000;
				int currentSum = 0;
				int d_x = 0;
				int d_y = 0;
	    		for (Mat matrix :planes) {
	    			lin = matrix.rows;
	    			col = matrix.cols;

	    			for(int c = 0; c < lin; c += block_size){
						for(int i = 0; i < col; i += block_size){
							current_block = matrix.colRange(i, i + block_size).rowRange(c, c + block_size);
							for(int k = c - search_space; k < c + search_space + block_size; k++){
								for(int j = i - search_space; j < i + search_space + block_size; j++){
									if (k + block_size < lin && k >= 0 && j >= 0 && j + block_size < col){
										possible_block = lastFrame[count].colRange(j, j + block_size).rowRange(k, k + block_size);
										for(int a = 0; a < block_size; a++){
											for(int b = 0; b < block_size; b++){
												currentSum = currentSum + abs(((int)current_block.at<uchar>(a, b)-(int)possible_block.at<uchar>(a, b)));
											}
										}
										if (currentSum < lastSum){
											lastSum = currentSum;
											d_x = k;
											d_y = j;
											min_block = possible_block;
										}
									}
									currentSum = 0;
								}
							}
							lastSum = 10000;

							g->encode(c - d_x);
							g->encode(i - d_y);

							for(int n = 0; n < block_size; n++){
								for(int m = 0; m < block_size; m++){
									int z= (int)((int)min_block.at<uchar>(n, m) - (int)current_block.at<uchar>(n, m));
									g->encode(z);
								}
							}
							count_blocks++;
						}
					}
				count ++;
	    		}
	    	}
	    	
	    	vector<Mat> decode_by_blocks(){

				/**
				 * Function to decode by blocks
				 */

	    	    vector<Mat> res;
	    		if (typeVideo == 0){
	    			Mat m1(linhas,colunas,0);
	    			Mat m2(linhas,colunas,0);
	    			Mat m3(linhas,colunas,0);
	    			Mat current_block;
	    			int x;
	    			int y;
	    			int count = 0;
	    			for(int c = 0; c < linhas; c+=block_size){
						for(int i = 0; i < colunas; i+=block_size){
							x = -(g->decode()-c);
							y = -(g->decode()-i);
							current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
							for(int a = 0; a < block_size; a++){
								for(int b = 0; b < block_size; b++){
								m1.at<uchar>(c+a,i+b) = (unsigned char)-(g->decode()-(int) current_block.at<uchar>(a,b));							
								}
							}				
						}
					}
				count++;
				for(int c = 0; c < linhas; c+=block_size){
					for(int i = 0; i < colunas; i+=block_size){
						x = -(g->decode()-c);
						y = -(g->decode()-i);
						current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
						for(int a = 0; a < block_size; a++){
							for(int b = 0; b < block_size; b++){
								m2.at<uchar>(c+a,i+b) = (unsigned char)-(g->decode()-(int) current_block.at<uchar>(a,b));
							}
						}				
					}
				}
				count++;
				for(int c = 0; c < linhas; c+=block_size){
					for(int i = 0; i < colunas; i+=block_size){
						x = -(g->decode()-c);
						y = -(g->decode()-i);
						current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
						for(int a = 0; a < block_size; a++){
							for(int b = 0; b < block_size; b++){
								m3.at<uchar>(c+a,i+b) = (unsigned char)-(g->decode()-(int) current_block.at<uchar>(a,b));
							}
						}				
					}
				}
				res = {m1,m2,m3};
	    			
	    		}else if (typeVideo == 1){
	    			Mat m1(linhas,colunas,0);
	    			Mat m2(linhas,colunas,0);
	    			Mat temp2(linhas,colunas/2,0);
	    			Mat m3(linhas,colunas,0);
	    			Mat temp3(linhas,colunas/2,0);
	    			Mat current_block;
	    			int x;
	    			int y;
	    			int count = 0;
	    			int decoded;
	    			int pixel;
	    			for(int c = 0; c < linhas; c+=block_size){
						for(int i = 0; i < colunas; i+=block_size){
							x = -(g->decode()-c);
							y = -(g->decode()-i);
							current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
							for(int a = 0; a < block_size; a++){
								for(int b = 0; b < block_size; b++){
									m1.at<uchar>(c+a,i+b) = (unsigned char)-(g->decode()-(int) current_block.at<uchar>(a,b));							
								}
							}				
						}
					}
				count++;
				for(int c = 0; c < linhas; c+=block_size){
					for(int i = 0; i < colunas/2; i+=block_size){
						x = -(g->decode()-c);
						y = -(g->decode()-i);
						current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
						for(int a = 0; a < block_size; a++){
							for(int b = 0; b < block_size; b++){
								decoded = g->decode();
								pixel = -(decoded-((int) current_block.at<uchar>(a,b)));
								temp2.at<uchar>(c+a,i+b) = (unsigned char)(pixel);
							}
						}				
					}
				}
				count++;
				for(int c = 0; c < linhas; c+=block_size){
					for(int i = 0; i < colunas/2; i+=block_size){
						x = -(g->decode()-c);
						y = -(g->decode()-i);
						current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
						for(int a = 0; a < block_size; a++){
							for(int b = 0; b < block_size; b++){
								decoded = g->decode();
								pixel = -(decoded-(int) current_block.at<uchar>(a,b));
								temp3.at<uchar>(c+a,i+b) = (unsigned char)(pixel);
							}
						}				
					}
				}
				for(int c = 0; c < linhas; c++){
					for(int i = 0; i < colunas; i+=2){
						m2.at<uchar>(c,i) = temp2.at<uchar>(c,i/2);		
						m2.at<uchar>(c,i+1) = temp2.at<uchar>(c,i/2);		
						m3.at<uchar>(c,i) = temp3.at<uchar>(c,i/2);		
						m3.at<uchar>(c,i+1) = temp3.at<uchar>(c,i/2);	
					}
				}
				count++;
				res = {m1,m2,m3};
				set_last_frame({m1,temp2,temp3});
	    			
	    		}else if (typeVideo == 2){
	    			Mat m1(linhas,colunas,0);
	    			Mat m2(linhas,colunas,0);
	    			Mat temp2(linhas/2,colunas/2,0);
	    			Mat m3(linhas,colunas,0);
	    			Mat temp3(linhas/2,colunas/2,0);
	    			Mat current_block;
	    			int x;
	    			int y;
	    			int count = 0;
	    			int decoded;
	    			int pixel;
	    			for(int c = 0; c < linhas; c+=block_size){
						for(int i = 0; i < colunas; i+=block_size){
							x = -(g->decode()-c);
							y = -(g->decode()-i);
							current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
							for(int a = 0; a < block_size; a++){
								for(int b = 0; b < block_size; b++){
									m1.at<uchar>(c+a,i+b) = (unsigned char)-(g->decode()-(int) current_block.at<uchar>(a,b));							
								}
							}				
						}
					}
				count++;
				for(int c = 0; c < linhas/2; c+=block_size){
					for(int i = 0; i < colunas/2; i+=block_size){
						x = -(g->decode()-c);
						y = -(g->decode()-i);
						current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
						for(int a = 0; a < block_size; a++){
							for(int b = 0; b < block_size; b++){
								decoded = g->decode();
								pixel = -(decoded-((int) current_block.at<uchar>(a,b)));
								temp2.at<uchar>(c+a,i+b) = (unsigned char)(pixel);
							}
						}				
					}
				}
				count++;
				for(int c = 0; c < linhas/2; c+=block_size){
					for(int i = 0; i < colunas/2; i+=block_size){
						x = -(g->decode()-c);
						y = -(g->decode()-i);
						current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
						for(int a = 0; a < block_size; a++){
							for(int b = 0; b < block_size; b++){
								decoded = g->decode();
								pixel = -(decoded-(int) current_block.at<uchar>(a,b));
								temp3.at<uchar>(c+a,i+b) = (unsigned char)(pixel);
							}
						}				
					}
				}
				for(int c = 0; c < linhas; c+=2){
					for(int i = 0; i < colunas; i+=2){
						m2.at<uchar>(c,i) = temp2.at<uchar>(c/2,i/2);	
						m2.at<uchar>(c+1,i) = temp2.at<uchar>(c/2,i/2);	
						m2.at<uchar>(c,i+1) = temp2.at<uchar>(c/2,i/2);	
						m2.at<uchar>(c+1,i+1) = temp2.at<uchar>(c/2,i/2);	
						m3.at<uchar>(c,i) = temp3.at<uchar>(c/2,i/2);	
						m3.at<uchar>(c+1,i) = temp3.at<uchar>(c/2,i/2);	
						m3.at<uchar>(c,i+1) = temp3.at<uchar>(c/2,i/2);	
						m3.at<uchar>(c+1,i+1) = temp3.at<uchar>(c/2,i/2);
					}
				}
				count++;
				res = {m1,m2,m3};
				set_last_frame({m1,temp2,temp3});
	    			
	    		}
	    		return res;
	    	}
	    	
	   	void encodeJPEG1(Mat f){

			/**
			 * Function to encode given frame using JPEG1
			 * 
			 * @param f given frame
			 */

	   		int lin = f.rows;
	   		int col = f.cols;
	   		if (start == 1){
		   		string width  = to_string(col);
		   		string height = to_string(lin);
		   		string frames = to_string(n_frames);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
		   		for (int f = 0; f < 4-frames.length();f++){
		   			frames = "0"+frames;
		   		}
		   		wbs->writeString(width);
		   		wbs->writeString(height);
		   		wbs->writeString(frames);
		   		wbs->closeNoWrite();
		   		g = new Golomb(fileName,m,1);
		   		start=0;
			}
			int pixel;
			int lastPixel = 0;
			int ind = 1;
			Mat res(lin,col,0);
			int count=0;
			for(int c = 0; c < lin; c++){
				for(int i = 0; i < col; i++){
					if(i-1 >= 0){
						lastPixel = (int) f.at<uchar>(c,i-1);
					}else{
						lastPixel = 0;
					}
					pixel = (int) f.at<uchar>(c,i);
					g->encode(pixel-lastPixel);
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
	   	}

	   	void encodeJPEG2(Mat f){

			/**
			 * Function to encode given frame using JPEG2
			 * 
			 * @param f given frame
			 */

	   		int lin = f.rows;
	   		int col = f.cols;
	   		if (start == 1){
		   		string width  = to_string(col);
		   		string height = to_string(lin);
		   		string frames = to_string(n_frames);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
		   		for (int f = 0; f < 4-frames.length();f++){
		   			frames = "0"+frames;
		   		}
		   		wbs->writeString(width);
		   		wbs->writeString(height);
		   		wbs->writeString(frames);
		   		wbs->closeNoWrite();
		   		g = new Golomb(fileName,m,1);
		   		start=0;
			}
			int pixel;
			int lastPixel = 0;
			int ind = 1;
			Mat res(lin,col,0);
			int count=0;
			for(int c = 0; c < lin; c++){
				for(int i = 0; i < col; i++){
					if(c-1 >= 0){
						lastPixel = (int) f.at<uchar>(c-1,i);
					}else{
						lastPixel = 0;
					}
					pixel = (int) f.at<uchar>(c,i);
					g->encode(pixel-lastPixel);
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
	   	}
	   	
	   	void encodeJPEG3(Mat f){

			/**
			 * Function to encode given frame using JPEG3
			 * 
			 * @param f given frame
			 */

	   		int lin = f.rows;
	   		int col = f.cols;
	   		if (start == 1){
		   		string width  = to_string(col);
		   		string height = to_string(lin);
		   		string frames = to_string(n_frames);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
		   		for (int f = 0; f < 4-frames.length();f++){
		   			frames = "0"+frames;
		   		}
		   		wbs->writeString(width);
		   		wbs->writeString(height);
		   		wbs->writeString(frames);
		   		wbs->closeNoWrite();
		   		g = new Golomb(fileName,m,1);
		   		start=0;
			}
			int pixel;
			int lastPixel = 0;
			int ind = 1;
			Mat res(lin,col,0);
			int count=0;
			for(int c = 0; c < lin; c++){
				for(int i = 0; i < col; i++){
					if(c-1 >= 0 && i-1>=0){
						lastPixel = (int) f.at<uchar>(c-1,i-1);
					}else{
						lastPixel = 0;
					}
					pixel = (int) f.at<uchar>(c,i);
					g->encode(pixel-lastPixel);
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
	   	}
	   	
	   	void encodeJPEG4(Mat f){

			/**
			 * Function to encode given frame using JPEG4
			 * 
			 * @param f given frame
			 */

	   		int lin = f.rows;
	   		int col = f.cols;
	   		if (start == 1){
		   		string width  = to_string(col);
		   		string height = to_string(lin);
		   		string frames = to_string(n_frames);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
		   		for (int f = 0; f < 4-frames.length();f++){
		   			frames = "0"+frames;
		   		}
		   		wbs->writeString(width);
		   		wbs->writeString(height);
		   		wbs->writeString(frames);
		   		wbs->closeNoWrite();
		   		g = new Golomb(fileName,m,1);
		   		start=0;
			}
			int pixel;
			int lastPixel = 0;
			int ind = 1;
			Mat res(lin,col,0);
			int count=0;
			int a;
			int b;
			int z;
			for(int c = 0; c < lin; c++){
				for(int i = 0; i < col; i++){
					pixel = f.at<uchar>(c,i);
					if(i-1 >=0){
						a=(int)f.at<uchar>(c,i-1);
					}else{
						a=0;
					}
					if(c-1 >=0){
						b=(int)f.at<uchar>(c-1,i);
					}else{
						b=0;
					}
					if(i-1 >=0 && c-1 >=0){
						z=(int)f.at<uchar>(c-1,i-1);
					}else{
						z=0;
					}
					lastPixel = a+b-z;
					g->encode(pixel-lastPixel);
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
	   	}
	   	
	   	void encodeJPEG5(Mat f){

			/**
			 * Function to encode given frame using JPEG5
			 * 
			 * @param f given frame
			 */

	   		int lin = f.rows;
	   		int col = f.cols;
	   		if (start == 1){
		   		string width  = to_string(col);
		   		string height = to_string(lin);
		   		string frames = to_string(n_frames);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
		   		for (int f = 0; f < 4-frames.length();f++){
		   			frames = "0"+frames;
		   		}
		   		wbs->writeString(width);
		   		wbs->writeString(height);
		   		wbs->writeString(frames);
		   		wbs->closeNoWrite();
		   		g = new Golomb(fileName,m,1);
		   		start=0;
			}
			int pixel;
			int lastPixel = 0;
			int ind = 1;
			Mat res(lin,col,0);
			int count=0;
			int a;
			int b;
			int z;
			for(int c = 0; c < lin; c++){
				for(int i = 0; i < col; i++){
					pixel = f.at<uchar>(c,i);
					if(i-1 >=0){
						a=(int)f.at<uchar>(c,i-1);
					}else{
						a=0;
					}
					if(c-1 >=0){
						b=(int)f.at<uchar>(c-1,i);
					}else{
						b=0;
					}
					if(i-1 >=0 && c-1 >=0){
						z=(int)f.at<uchar>(c-1,i-1);
					}else{
						z=0;
					}
					lastPixel = a+(b-z)/2;
					g->encode(pixel-lastPixel);
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
	   	}
	   	
	   	void encodeJPEG6(Mat f){

			/**
			 * Function to encode given frame using JPEG6
			 * 
			 * @param f given frame
			 */

	   		int lin = f.rows;
	   		int col = f.cols;
	   		if (start == 1){
		   		string width  = to_string(col);
		   		string height = to_string(lin);
		   		string frames = to_string(n_frames);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
		   		for (int f = 0; f < 4-frames.length();f++){
		   			frames = "0"+frames;
		   		}
		   		wbs->writeString(width);
		   		wbs->writeString(height);
		   		wbs->writeString(frames);
		   		wbs->closeNoWrite();
		   		g = new Golomb(fileName,m,1);
		   		start=0;
			}
			int pixel;
			int lastPixel = 0;
			int ind = 1;
			Mat res(lin,col,0);
			int count=0;
			int a;
			int b;
			int z;
			for(int c = 0; c < lin; c++){
				for(int i = 0; i < col; i++){
					pixel = f.at<uchar>(c,i);
					if(i-1 >=0){
						a=(int)f.at<uchar>(c,i-1);
					}else{
						a=0;
					}
					if(c-1 >=0){
						b=(int)f.at<uchar>(c-1,i);
					}else{
						b=0;
					}
					if(i-1 >=0 && c-1 >=0){
						z=(int)f.at<uchar>(c-1,i-1);
					}else{
						z=0;
					}
					lastPixel = b+(a-z)/2;
					g->encode(pixel-lastPixel);
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
	   	}
	   	
	   	void encodeJPEG7(Mat f){

			/**
			 * Function to encode given frame using JPEG7
			 * 
			 * @param f given frame
			 */

	   		int lin = f.rows;
	   		int col = f.cols;
	   		if (start == 1){
		   		string width  = to_string(col);
		   		string height = to_string(lin);
		   		string frames = to_string(n_frames);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
		   		for (int f = 0; f < 4-frames.length();f++){
		   			frames = "0"+frames;
		   		}
		   		wbs->writeString(width);
		   		wbs->writeString(height);
		   		wbs->writeString(frames);
		   		wbs->closeNoWrite();
		   		g = new Golomb(fileName,m,1);
		   		start=0;
			}
			int pixel;
			int lastPixel = 0;
			int ind = 1;
			Mat res(lin,col,0);
			int count=0;
			int a;
			int b;
			int z;
			for(int c = 0; c < lin; c++){
				for(int i = 0; i < col; i++){
					pixel = f.at<uchar>(c,i);
					if(i-1 >=0){
						a=(int)f.at<uchar>(c,i-1);
					}else{
						a=0;
					}
					if(c-1 >=0){
						b=(int)f.at<uchar>(c-1,i);
					}else{
						b=0;
					}
					if(i-1 >=0 && c-1 >=0){
						z=(int)f.at<uchar>(c-1,i-1);
					}else{
						z=0;
					}
					lastPixel = (a+b)/2;
					g->encode(pixel-lastPixel);
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
	   	}
	   	
	   	void encodeJPEG_LS(Mat f){

			/**
			 * Function to encode given frame using JPEG-LS
			 * 
			 * @param f given frame
			 */

	   		int lin = f.rows;
	   		int col = f.cols;
	   		if (start == 1){
		   		string width  = to_string(col);
		   		string height = to_string(lin);
		   		string frames = to_string(n_frames);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
		   		for (int f = 0; f < 4-frames.length();f++){
		   			frames = "0"+frames;
		   		}
		   		wbs->writeString(width);
		   		wbs->writeString(height);
		   		wbs->writeString(frames);
		   		wbs->closeNoWrite();
		   		g = new Golomb(fileName,m,1);
		   		start=0;
			}
			int pixel;
			int lastPixel = 0;
			int ind = 1;
			Mat res(lin,col,0);
			int count=0;
			int a;
			int b;
			int z;
			int maximo;
			int minimo;
			for(int c = 0; c < lin; c++){
				for(int i = 0; i < col; i++){
					pixel = f.at<uchar>(c,i);
					if(i-1 >=0){
						a=(int)f.at<uchar>(c,i-1);
					}else{
						a=0;
					}
					if(c-1 >=0){
						b=(int)f.at<uchar>(c-1,i);
					}else{
						b=0;
					}
					if(i-1 >=0 && c-1 >=0){
						z=(int)f.at<uchar>(c-1,i-1);
					}else{
						z=0;
					}
					maximo = max(a,b);
					minimo = min(a,b);
					if(z>= maximo){
						lastPixel = minimo;					
					}else if(z <= minimo){
						lastPixel = maximo;
					}else{
						lastPixel = a+b-z;
					}
					g->encode(pixel-lastPixel);
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
	   	}
	   	
	   	Mat decodeJPEG1(){

			/**
			 * Function to decode a frame encoded in JPEG1
			 */

	   		Mat result;
	   		if (typeVideo == 0){
		   		int lastPixel=0;
		   		Mat result(linhas,colunas,0);
				int count = 0;
				for(int c = 0; c < linhas; c++){
					for(int i = 0; i < colunas; i++){
						if(i-1 >= 0){
							lastPixel = (int) result.at<uchar>(c,i-1);
						}else{
							lastPixel = 0;
						}
						int d = g->decode();
						result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
						count++;				
					}
				}
				return result;
			} else if (typeVideo == 1) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i++){
							if(i-1 >= 0){
								lastPixel = (int) result.at<uchar>(c,i-1);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i+=2){
							if(i-1 >= 0){
								lastPixel = (int) result.at<uchar>(c,i-1);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			} else if (typeVideo == 2) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i++){
							if(i-1 >= 0){
								lastPixel = (int) result.at<uchar>(c,i-1);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c+=2){
						for(int i = 0; i < colunas; i+=2){
							if(i-1 >= 0){
								lastPixel = (int) result.at<uchar>(c,i-1);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			}
			return result;
	   	}
	   	
	   	Mat decodeJPEG2(){

			/**
			 * Function to decode a frame encoded in JPEG2
			 */
			
	   		Mat result;
	   		if (typeVideo == 0){
		   		int lastPixel=0;
		   		Mat result(linhas,colunas,0);
				int count = 0;
				for(int c = 0; c < linhas; c++){
					for(int i = 0; i < colunas; i++){
						if(c-1 >= 0){
							lastPixel = (int) result.at<uchar>(c-1,i);
						}else{
							lastPixel = 0;
						}
						int d = g->decode();
						result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
						count++;				
					}
				}
				return result;
			} else if (typeVideo == 1) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i++){
							if(c-1 >= 0){
								lastPixel = (int) result.at<uchar>(c-1,i);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i+=2){
							if(c-1 >= 0){
								lastPixel = (int) result.at<uchar>(c-1,i);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			} else if (typeVideo == 2) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i++){
							if(c-1 >= 0){
								lastPixel = (int) result.at<uchar>(c-1,i);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c+=2){
						for(int i = 0; i < colunas; i+=2){
							if(c-1 >= 0){
								lastPixel = (int) result.at<uchar>(c-1,i);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			}
			return result;
	   	}
	   	
	   	Mat decodeJPEG3(){

			/**
			 * Function to decode a frame encoded in JPEG3
			 */

	   		Mat result;
	   		if (typeVideo == 0){
		   		int lastPixel=0;
		   		Mat result(linhas,colunas,0);
				int count = 0;
				for(int c = 0; c < linhas; c++){
					for(int i = 0; i < colunas; i++){
						if(c-1 >= 0 && i-1>=0){
							lastPixel = (int) result.at<uchar>(c-1,i-1);
						}else{
							lastPixel = 0;
						}
						int d = g->decode();
						result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
						count++;				
					}
				}
				return result;
			} else if (typeVideo == 1) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i++){
							if(c-1 >= 0 && i-1>=0){
								lastPixel = (int) result.at<uchar>(c-1,i-1);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i+=2){
							if(c-1 >= 0 && i-1>=0){
								lastPixel = (int) result.at<uchar>(c-1,i-1);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			} else if (typeVideo == 2) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i++){
							if(c-1 >= 0 && i-1>=0){
								lastPixel = (int) result.at<uchar>(c-1,i-1);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c+=2){
						for(int i = 0; i < colunas; i+=2){
							if(c-1 >= 0 && i-1>=0){
								lastPixel = (int) result.at<uchar>(c-1,i-1);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			}
			return result;
	   	}
	   	
	   	Mat decodeJPEG4(){

			/**
			 * Function to decode a frame encoded in JPEG4
			 */

	   		Mat result;
	   		int a;
			int b;
			int z;
	   		if (typeVideo == 0){
		   		int lastPixel=0;
		   		Mat result(linhas,colunas,0);
				int count = 0;
				for(int c = 0; c < linhas; c++){
					for(int i = 0; i < colunas; i++){
						if(i-1 >=0){
							a=(int)result.at<uchar>(c,i-1);
						}else{
							a=0;
						}
						if(c-1 >=0){
							b=(int)result.at<uchar>(c-1,i);
						}else{
							b=0;
						}
						if(i-1 >=0 && c-1 >=0){
							z=(int)result.at<uchar>(c-1,i-1);
						}else{
							z=0;
						}
						lastPixel = a+b-z;
						int d = g->decode();
						result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
						count++;				
					}
				}
				return result;
			} else if (typeVideo == 1) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i++){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							lastPixel = a+b-z;
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i+=2){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							lastPixel = a+b-z;
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			} else if (typeVideo == 2) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i++){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							lastPixel = a+b-z;
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c+=2){
						for(int i = 0; i < colunas; i+=2){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							lastPixel = a+b-z;
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			}
			return result;
	   	}
	   	
	   	Mat decodeJPEG5(){

			/**
			 * Function to decode a frame encoded in JPEG5
			 */

	   		Mat result;
	   		int a;
			int b;
			int z;
	   		if (typeVideo == 0){
		   		int lastPixel=0;
		   		Mat result(linhas,colunas,0);
				int count = 0;
				for(int c = 0; c < linhas; c++){
					for(int i = 0; i < colunas; i++){
						if(i-1 >=0){
							a=(int)result.at<uchar>(c,i-1);
						}else{
							a=0;
						}
						if(c-1 >=0){
							b=(int)result.at<uchar>(c-1,i);
						}else{
							b=0;
						}
						if(i-1 >=0 && c-1 >=0){
							z=(int)result.at<uchar>(c-1,i-1);
						}else{
							z=0;
						}
						lastPixel = a+(b-z)/2;
						int d = g->decode();
						result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
						count++;				
					}
				}
				return result;
			} else if (typeVideo == 1) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i++){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							lastPixel = a+(b-z)/2;
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i+=2){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							lastPixel = a+(b-z)/2;
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			} else if (typeVideo == 2) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i++){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							lastPixel = a+(b-z)/2;
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c+=2){
						for(int i = 0; i < colunas; i+=2){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							lastPixel = a+(b-z)/2;
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			}
			return result;
	   	}
	   	
	   	Mat decodeJPEG6(){

			/**
			 * Function to decode a frame encoded in JPEG6
			 */

	   		Mat result;
	   		int a;
			int b;
			int z;
	   		if (typeVideo == 0){
		   		int lastPixel=0;
		   		Mat result(linhas,colunas,0);
				int count = 0;
				for(int c = 0; c < linhas; c++){
					for(int i = 0; i < colunas; i++){
						if(i-1 >=0){
							a=(int)result.at<uchar>(c,i-1);
						}else{
							a=0;
						}
						if(c-1 >=0){
							b=(int)result.at<uchar>(c-1,i);
						}else{
							b=0;
						}
						if(i-1 >=0 && c-1 >=0){
							z=(int)result.at<uchar>(c-1,i-1);
						}else{
							z=0;
						}
						lastPixel = b+(a-z)/2;
						int d = g->decode();
						result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
						count++;				
					}
				}
				return result;
			} else if (typeVideo == 1) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i++){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							lastPixel = b+(a-z)/2;
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i+=2){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							lastPixel = b+(a-z)/2;
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			} else if (typeVideo == 2) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i++){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							lastPixel = b+(a-z)/2;
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c+=2){
						for(int i = 0; i < colunas; i+=2){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							lastPixel = b+(a-z)/2;
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			}
			return result;
	   	}
	   	
	   	Mat decodeJPEG7(){

			/**
			 * Function to decode a frame encoded in JPEG7
			 */

	   		Mat result;
	   		int a;
			int b;
			int z;
	   		if (typeVideo == 0){
		   		int lastPixel=0;
		   		Mat result(linhas,colunas,0);
				int count = 0;
				for(int c = 0; c < linhas; c++){
					for(int i = 0; i < colunas; i++){
						if(i-1 >=0){
							a=(int)result.at<uchar>(c,i-1);
						}else{
							a=0;
						}
						if(c-1 >=0){
							b=(int)result.at<uchar>(c-1,i);
						}else{
							b=0;
						}
						if(i-1 >=0 && c-1 >=0){
							z=(int)result.at<uchar>(c-1,i-1);
						}else{
							z=0;
						}
						lastPixel = (a+b)/2;
						int d = g->decode();
						result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
						count++;				
					}
				}
				return result;
			} else if (typeVideo == 1) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i++){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							lastPixel = (a+b)/2;
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i+=2){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							lastPixel = (a+b)/2;
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			} else if (typeVideo == 2) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i++){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							lastPixel = (a+b)/2;
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c+=2){
						for(int i = 0; i < colunas; i+=2){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							lastPixel = (a+b)/2;
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			}
			return result;
	   	}
	   	
	   	Mat decodeJPEGLS(){

			/**
			 * Function to decode a frame encoded in JPEG-LS
			 */

	   		Mat result;
	   		int lastPixel = 0;
	   		int a;
			int b;
			int z;
			int maximo;
			int minimo;
	   		if (typeVideo == 0){
		   		int lastPixel=0;
		   		Mat result(linhas,colunas,0);
				int count = 0;
				for(int c = 0; c < linhas; c++){
					for(int i = 0; i < colunas; i++){
						if(i-1 >=0){
							a=(int)result.at<uchar>(c,i-1);
						}else{
							a=0;
						}
						if(c-1 >=0){
							b=(int)result.at<uchar>(c-1,i);
						}else{
							b=0;
						}
						if(i-1 >=0 && c-1 >=0){
							z=(int)result.at<uchar>(c-1,i-1);
						}else{
							z=0;
						}
						maximo = max(a,b);
						minimo = min(a,b);
						if(z>= maximo){
							lastPixel = minimo;					
						}else if(z <= minimo){
							lastPixel = maximo;
						}else{
							lastPixel = a+b-z;
						}
						int d = g->decode();
						result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
						count++;				
					}
				}
				return result;
			} else if (typeVideo == 1) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i++){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							maximo = max(a,b);
							minimo = min(a,b);
							if(z>= maximo){
								lastPixel = minimo;					
							}else if(z <= minimo){
								lastPixel = maximo;
							}else{
								lastPixel = a+b-z;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i+=2){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							maximo = max(a,b);
							minimo = min(a,b);
							if(z>= maximo){
								lastPixel = minimo;					
							}else if(z <= minimo){
								lastPixel = maximo;
							}else{
								lastPixel = a+b-z;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			} else if (typeVideo == 2) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i++){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							maximo = max(a,b);
							minimo = min(a,b);
							if(z>= maximo){
								lastPixel = minimo;					
							}else if(z <= minimo){
								lastPixel = maximo;
							}else{
								lastPixel = a+b-z;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c+=2){
						for(int i = 0; i < colunas; i+=2){
							if(i-1 >=0){
								a=(int)result.at<uchar>(c,i-1);
							}else{
								a=0;
							}
							if(c-1 >=0){
								b=(int)result.at<uchar>(c-1,i);
							}else{
								b=0;
							}
							if(i-1 >=0 && c-1 >=0){
								z=(int)result.at<uchar>(c-1,i-1);
							}else{
								z=0;
							}
							maximo = max(a,b);
							minimo = min(a,b);
							if(z>= maximo){
								lastPixel = minimo;					
							}else if(z <= minimo){
								lastPixel = maximo;
							}else{
								lastPixel = a+b-z;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			}
			return result;
	   	}
};



















