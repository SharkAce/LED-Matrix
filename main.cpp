#include "include/led-matrix.h"
#include "include/graphics.h"
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#include <vector>
#include <string>
#include <iostream>

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

int main(int argc, char *argv[]){
	RGBMatrix::Options defaults;
	  defaults.hardware_mapping = "regular";
	  defaults.rows = 32;
	  defaults.cols = 64;
	  defaults.chain_length = 1;
	  defaults.parallel = 1;
	  defaults.show_refresh_rate = true;
	rgb_matrix::Font font;
	font.LoadFont("./fonts/4x6.bdf");
	rgb_matrix::Font *outline_font = font.CreateOutlineFont();
	rgb_matrix::Color color(0,255,0);

	RGBMatrix *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults);

	signal(SIGTERM, InterruptHandler);
	signal(SIGINT, InterruptHandler);
	while(!interrupt_received){
		//canvas->Fill(0,0,255);
		rgb_matrix::DrawText(canvas, font,
                           0, 10 + font.baseline(),
                           color, NULL, "Hello World",
                           1);
		usleep(1000);
	}
	canvas->Clear();
	delete canvas;
	return 0;
}

