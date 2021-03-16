#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <math.h>

#define E 2.7
#define PI 3.14159


double line_width = 2;	//4.5 or 2
int axis_width = 5;		//any int
int reserved_left = 300;
int reserved_down = 250;

int op_r = 2399 - reserved_down;
int op_c = reserved_left;

typedef unsigned char BYTE;

void draw_axis(int, BYTE*);
void draw_line(double, double, BYTE*);
void draw_dot(double, int, int, BYTE*);
long double cal_M(double, double);

int main() {
	BYTE* sheet = (BYTE*)malloc(sizeof(BYTE) * 2400 * 3500);
	memset(sheet, 255, sizeof(BYTE) * 2400 * 3500);

	draw_axis(axis_width, sheet);
	draw_line(line_width, 0.3, sheet);		//T = 500K
	draw_line(line_width, 0.5, sheet);		//T = 800K
	draw_line(line_width, 1, sheet);		//T = 1000K
	draw_line(line_width, 6, sheet);		//T = 6000K

	FILE* fp;
	char output_path[255] = "F:\\plank.raw";
	fp = fopen(output_path, "wb");

	fwrite(sheet, 1, 2400 * 3500, fp);
	
	fclose(fp);
	return 0;
}

void draw_axis(int w, BYTE* sheet) {
	//draw x
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 560; j++) {
			for (int k = 0; k < w; k++) {
				sheet[(op_r - k) * 3500 + op_c + i * 560 + j] = 0;
			}
		}
		for (int j = 0; j < w; j++) {
			for (int k = 0; k < 30; k++) {
				sheet[(op_r - w - k) * 3500 + op_c + (i + 1) * 560 + j] = 0;
			}
		}
	}

	//draw y
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 210; j++) {
			for (int k = 0; k < w; k++) {
				sheet[(op_r - i * 210 - j) * 3500 + op_c + k] = 0;
			}
		}
		for (int j = 0; j < w; j++) {
			for (int k = 0; k < 30; k++) {
				sheet[(op_r - (i + 1) * 210 - j) * 3500 + op_c + k] = 0;
			}
		}
	}
}

void draw_line(double line_width, double t, BYTE* sheet) {
	double wave_length;
	int delta_r = 0;
	for (int delta_c = 1; delta_c <= 2800; delta_c++) {
		/*if (delta_c <= 560) {
			wave_length = 1.0 * delta_c / 2800.0;
		}
		else if (delta_c <= 840) {
			wave_length = 3.0 * delta_c / 2800.0 - 0.4;
		}
		else if (delta_c <= 1120) {
			wave_length = 5.0 * delta_c / 2800.0 - 1;
		}
		else if (delta_c <= 1400) {
			wave_length = 1.0 * delta_c / 280.0 - 3;
		}
		else if (delta_c <= 1680) {
			wave_length = 3.0 * delta_c / 280.0 - 13;
		}
		else if (delta_c <= 2800) {
			wave_length = 5.0 * delta_c / 280.0 - 25;
		}*/
		wave_length = pow(10, (-2 + 5.0 / 2800.0 * delta_c));
		double _m = cal_M(wave_length, t);
		if (log10l(_m) < -1) {
			delta_r = 0;
		}
		else {
			double exp = log10l(_m);
			delta_r = (int)(210 + exp * 210);
		}
		if(delta_r > 0)
		draw_dot(line_width, op_r - delta_r - axis_width, op_c + delta_c + axis_width, sheet);
	}
}

void draw_dot(double w, int r, int c, BYTE* sheet) {
	if (w == 2) {
		for (int i = 0; i < 2; i++) {
			sheet[(r - 2) * 3500 + c + i] = 0;
			sheet[(r + 1) * 3500 + c + i] = 0;
		}
		for (int i = 0; i < 4; i++) {
			sheet[(r - 1) * 3500 + c - 1 + i] = 0;
			sheet[r * 3500 + c - 1 + i] = 0;
		}
	}
	if (w == 4.5) {
		for (int i = 0; i < 3; i++) {
			sheet[(r - 4) * 3500 + c - 1 + i] = 0;
			sheet[(r + 4) * 3500 + c - 1 + i] = 0;
		}
		for (int i = 0; i < 5; i++) {
			sheet[(r - 3) * 3500 + c - 2 + i] = 0;
			sheet[(r + 3) * 3500 + c - 2 + i] = 0;
		}
		for (int i = 0; i < 7; i++) {
			sheet[(r - 2) * 3500 + c - 3 + i] = 0;
			sheet[(r + 2) * 3500 + c - 3 + i] = 0;
		}
		for (int i = 0; i < 9; i++) {
			sheet[(r - 1) * 3500 + c - 4 + i] = 0;
			sheet[r * 3500 + c - 4 + i] = 0;
			sheet[(r + 1) * 3500 + c - 4 + i] = 0;
		}
	}
}

long double cal_M(double w_l, double t) {		//Calculate m through wavelength and temerature
	double c_1 = 2 * PI * 6.626 * 9;
	long double c_2 = (6.626 * 3 / (1.3806 * t * w_l));
	long double b_1 = (c_1 * pow(10, 3)) / powl(w_l, 5);
	long double b_2 = pow(10, 3) / (powl(E, c_2) - 1);
	//printf("wavelength=%lf,m =%lf\n",w_l,  b_1 * b_2);
	return b_1 * b_2;
}