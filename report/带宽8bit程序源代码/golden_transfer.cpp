#include "golden_transfer.h"
#include <iostream>
using namespace std;

PIXEL _R(uint32_t r,uint32_t g,uint32_t b){
	uint32_t res=(393*r+769*g+189*b)/1000;
	PIXEL ret;
	if(res>255)ret=255;
	else ret=res;
	return ret;
}
PIXEL _G(uint32_t r,uint32_t g,uint32_t b){
	uint32_t res=(349*r+686*g+168*b)/1000;
	PIXEL ret;
	if(res>255)ret=255;
	else ret=res;
	return ret;
}
PIXEL _B(uint32_t r,uint32_t g,uint32_t b){
	uint32_t res=(272*r+534*g+131*b)/1000;
	PIXEL ret;
	if(res>255)ret=255;
	else ret=res;
	return ret;
}
void golden_transfer(hls::stream<trans_pkt>& src,hls::stream<trans_pkt>& dst,int rows, int cols){
#pragma HLS INTERFACE axis port=src
#pragma HLS INTERFACE axis port=dst
#pragma HLS INTERFACE s_axilite port=rows bundle=CTRL
#pragma HLS INTERFACE s_axilite port=cols bundle=CTRL
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL

	trans_pkt data_p;

	uint32_t b,g,r;
	PIXEL B,G,R;
	

	ap_uint<13> row,col;
	for(row = 0; row < rows; row++){
#pragma HLS LOOP_TRIPCOUNT min=1 max=720
		for(col = 0; col < cols; col++){
#pragma HLS LOOP_TRIPCOUNT min=1 max=1024
#pragma HLS pipeline
			data_p=src.read(), b=(uint32_t)data_p.data;
			data_p=src.read(), g=(uint32_t)data_p.data;
			data_p=src.read(), r=(uint32_t)data_p.data;

			uint32_t _R,_G,_B;
			_R=(393*r+769*g+189*b)/1000;
			_G=(349*r+686*g+168*b)/1000;
			_B=(272*r+534*g+131*b)/1000;
			if(_R>255)R=255;
			else R=_R;
			if(_G>255)G=255;
			else G=_G;
			if(_B>255)B=255;
			else B=_B;

			data_p.last=0;
			data_p.data=B;
			dst.write(data_p);
			data_p.data=G;
			dst.write(data_p);
			if(row==rows-1&&col==cols-1)
				data_p.last=1;
			data_p.data=R;
			dst.write(data_p);
		}

	}
}
