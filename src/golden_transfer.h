#include <hls_stream.h>
#include "ap_axi_sdata.h"
#include <stdint.h>
#include "ap_int.h"

#ifndef GOLDEN_DAY_H
#define GOLDEN_DAY_H

#define WIDTH 1024
#define HEIGHT 1024

typedef unsigned char PIXEL;
typedef ap_axiu<64,0,0,0> trans_pkt;

void golden_transfer(hls::stream<trans_pkt>& src,hls::stream<trans_pkt>& dst,int rows, int cols);

#endif
