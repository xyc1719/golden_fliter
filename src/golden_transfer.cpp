#include "golden_transfer.h"
#include <iostream>
using namespace std;

void golden_transfer(hls::stream<trans_pkt>& src,hls::stream<trans_pkt>& dst,int rows, int cols){
#pragma HLS INTERFACE axis port=src
#pragma HLS INTERFACE axis port=dst
#pragma HLS INTERFACE s_axilite port=rows bundle=CTRL
#pragma HLS INTERFACE s_axilite port=cols bundle=CTRL
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL

	trans_pkt data_p;
	uint32_t clr[8][3];
	PIXEL out[8][3];
#pragma HLS array_partition variable=clr complete
#pragma HLS array_partition variable=out complete

	int i;
	cols=cols*rows;
	for(i=0;i<cols;i+=8){
#pragma HLS LOOP_TRIPCOUNT min=1 max=720*128
#pragma HLS unroll factor=4
		PIXEL x=0,y=0;
		data_p=src.read();
		for(int k=0;k<8;k++){
#pragma HLS unroll
			clr[x][y]=(uint32_t)(data_p.data&0xff);
			data_p.data>>=8;
			y++;
			if(y==3){
				y=0;
				x++;
			}
		}
		data_p=src.read();
		for(int k=0;k<8;k++){
#pragma HLS unroll
			clr[x][y]=(uint32_t)(data_p.data&0xff);
			data_p.data>>=8;
			y++;
			if(y==3){
				y=0;
				x++;
			}
		}
		data_p=src.read();
		for(int k=0;k<8;k++){
#pragma HLS unroll
			clr[x][y]=(uint32_t)(data_p.data&0xff);
			data_p.data>>=8;
			y++;
			if(y==3){
				y=0;
				x++;
			}
		}
		for(int k=0;k<8;k++){
#pragma HLS unroll
			uint32_t _R,_G,_B;
			_R=(393*clr[k][2]+769*clr[k][1]+189*clr[k][0])/1000;
			_G=(349*clr[k][2]+686*clr[k][1]+168*clr[k][0])/1000;
			_B=(272*clr[k][2]+534*clr[k][1]+131*clr[k][0])/1000;
			if(_R>255)out[k][2]=255;
			else out[k][2]=_R;
			if(_G>255)out[k][1]=255;
			else out[k][1]=_G;
			if(_B>255)out[k][0]=255;
			else out[k][0]=_B;
		}
		data_p.data=0,data_p.last=0;
		x=2,y=1;
		for(int k=0;k<8;k++){
#pragma HLS unroll
			data_p.data=(data_p.data<<8)|out[x][y];
			y--;
			if(y>3){
				x--,y=2;
			}
		}
		dst.write(data_p);
		data_p.data=0,data_p.last=0;
		x=5,y=0;
		for(int k=0;k<8;k++){
#pragma HLS unroll
			data_p.data=(data_p.data<<8)|out[x][y];
			y--;
			if(y>3){
				x--,y=2;
			}
		}
		dst.write(data_p);
		data_p.data=0,data_p.last=0;
		x=7,y=2;
		for(int k=0;k<8;k++){
#pragma HLS unroll
			data_p.data=(data_p.data<<8)|out[x][y];
			y--;
			if(y>3){
				x--,y=2;
			}
		}
		if(i+8>=cols)
			data_p.last=1;
		dst.write(data_p);
	}
}
