#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <complex>
#include <complex.h>

#include "golden_transfer.h"

using namespace std;

int main () {

  PIXEL src[192];
  PIXEL dst[192];

  ifstream file("data.txt");
  string line;
  short int s1;
  int i = 0;

read_file:
  while(getline(file,line) && (i < 192))
  {
    istringstream sin(line);
    sin >> s1;
    src[i] = (PIXEL)s1;
    i = i + 1;
  }

	hls::stream<trans_pkt> _src;
	hls::stream<trans_pkt> _dst;
	trans_pkt data_p;
	int row, col;

mm2s:
	for(int i=0; i < 192; i+=8){
		data_p.data=0;
		for(int j=7;j>=0;j--){
			data_p.data=(data_p.data<<8)|(src[i+j]);
		}
		_src.write(data_p);
	}
//	for(row = 0; row < 2; row++)
//	{
//		for(col = 0; col < 2; col++)
//		{
//			data_p.data=0;
//			for(int k=0;k<3;k++){
//				data_p.data = (data_p.data<<8)+*(src + row*2*3 + col*3+k);
//			}
//			_src.write(data_p);
//		}
//	}

  golden_transfer(_src, _dst, 8, 8);

s2mm:
	for(int i=0;i<192;i+=8){
		data_p=_dst.read();
		for(int j=0;j<8;j++){
			dst[i+j]=data_p.data&0xff;
			data_p.data=data_p.data>>8;
		}
	}
//	for(row = 0; row < 2; row++)
//	{
//		for(col = 0; col < 2; col++)
//		{
//			data_p = _dst.read();
//			for(int k=2;k>=0;k--){
//				*(dst + row*2*3 + col*3+k) = data_p.data&0xff;
//				data_p.data=data_p.data>>8;
//			}
//		}
//	}

  int tf = 0;

//verify the result
  ifstream fileo("dst_2x2.txt");
  string lineo;
  short int s2;
  int j = 0;
  int err_cnt = 0;

  while(getline(fileo,lineo) && (j < 192))
  {
    istringstream sino(lineo);
    sino >> s2;
    if ((short int)dst[j] != s2)
    {
      tf = 1;
      err_cnt++;
    }
    j = j + 1;
  }


//  write the result to txt
    ofstream fout;
    fout.open("D:\_File\_fifth_semester\FPGA\project\src\imgo.txt");
    int m = 0;
    string pixelo;
    short int pixel;
	for(m = 0; m < (192); m++)
	{
	   pixel = dst[m];
	   fout << pixel << endl;
	}

  if (tf == 1)
  {
    fprintf(stdout, "*******************************************\n");
    fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
    fprintf(stdout, "*******************************************\n");

    int m = 0;
    short int pixel;
	for(m = 0; m < (192); m++)
	{
	   pixel = dst[m];
	   cout << pixel << endl;
	}
    return 1;
  }
  else
  {
    fprintf(stdout, "*******************************************\n");
    fprintf(stdout, "PASS: The output matches the golden output!\n");
    fprintf(stdout, "*******************************************\n");
    return 0;
  }

}
