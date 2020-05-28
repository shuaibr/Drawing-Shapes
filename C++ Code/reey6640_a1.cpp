//============================================================================
// Name        : reey6640_a1.cpp
// Author      : Shuaib Reeyaz
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

// OpenGL and C headers
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// other include if you split the your design into several files, it is a good practice to do so

const float DEG2RAD = 3.14159/180;

void mainMenu(GLint menuOption);

void Delete(GLint);

// C++ headers for file I/O
#include <fstream>
#include <iostream>
using namespace std;

// function prototype
//#define some constant here
#define  MaxNumObj 100

// global variables
GLsizei winWidth = 640, winHeight = 480; // variables and initial for width and height of display window
GLint list2[100], tool = 0, type = 1, style = 1, chart_style = 1,
		selected = 0,
		selection =
		-1, move = 0, xbegin, ybegin, ptCtr = 0, numObj = 0, i;
GLfloat red = 1.0, green = 0.0, blue = 0.0;


// data structures, you define the structures for the drawing objects, you can use the data structure given below
class modelObject {
public:
	GLint x1, y1, x2, y2, t, s; // t is for types of object: 1 for rectangle, 2 for cicle; s for drawing styles: 1 for filled, 2 for outline
	GLfloat r, g, b; // RGB color
} list[100]; // this is an array data structure, you can also use link list for storing objects

class colorObject {
public:
	GLfloat r, g, b;
} colors[100];

class Point { // testing for drawing a sequence of points
public:
	GLint x, y;
	GLfloat r, g, b; // RGB color
} points[100];

void SaveBitmap(const char *, int, int, int, int);
// function section
// initial function to set up OpenGL state variable other than default.
void init(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0); // Set display-window color to white
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, winHeight, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glFlush();
}

/* Drawing a point for testing */
void setPixel(GLint x, GLint y) {
	glPointSize(3.0);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}


void moveToFront() {

	list2[0] = list[selected].x1;
	list2[1] = list[selected].y1;
	list2[2] = list[selected].x2;
	list2[3] = list[selected].y2;
	list2[4] = list[selected].t;
	list2[5] = list[selected].s;

	colors[0].r = list[selected].r;
	colors[0].g = list[selected].g;
	colors[0].b = list[selected].b;

	Delete(selected);
	numObj++;

	list[numObj - 1].x1 = list2[0];
	list[numObj - 1].y1 = list2[1];
	list[numObj - 1].x2 = list2[2];
	list[numObj - 1].y2 = list2[3];
	list[numObj - 1].t = list2[4];
	list[numObj - 1].s = list2[5];

	list[numObj - 1].r = colors[0].r;
	list[numObj - 1].g = colors[0].g;
	list[numObj - 1].b = colors[0].b;

	selected = numObj - 1;
}

void sendToBack() {

	//save shape properties to temp
	list2[0] = list[selected].x1;
	list2[1] = list[selected].y1;
	list2[2] = list[selected].x2;
	list2[3] = list[selected].y2;
	list2[4] = list[selected].t;
	list2[5] = list[selected].s;

	colors[0].r = list[selected].r;
	colors[0].g = list[selected].g;
	colors[0].b = list[selected].b;

	int i = selected;

	//loop through shapes
	for (i = selected; i > 1; i--) {
		list[i].x1 = list[i - 1].x1;
		list[i].y1 = list[i - 1].y1;
		list[i].x2 = list[i - 1].x2;
		list[i].y2 = list[i - 1].y2;
		list[i].r = list[i - 1].r;
		list[i].g = list[i - 1].g;
		list[i].b = list[i - 1].b;
		list[i].t = list[i - 1].t;
		list[i].s = list[i - 1].s;
	}

	numObj++;
	list[1].x1 = list2[0];
	list[1].y1 = list2[1];
	list[1].x2 = list2[2];
	list[1].y2 = list2[3];
	list[1].t = list2[4];
	list[1].s = list2[5];

	colors[0].r = list2[6];
	colors[0].g = list2[7];
	colors[0].b = list2[8];
	selected = 1;
	glFlush();
}

// this function draw an object
void plotObject(GLint x1, GLint y1, GLint x2, GLint y2) {
	glBegin(GL_LINE_LOOP);
	glVertex2i(x1, y1);
	glVertex2i(x1, y2);
	glVertex2i(x2, y2);
	glVertex2i(x2, y1);
	glEnd();
}

void circlePlotPoints(GLint xc, GLint yc, GLint x, GLint y) {
	setPixel(xc + x, yc + y);
	setPixel(xc - x, yc + y);
	setPixel(xc + x, yc - y);
	setPixel(xc - x, yc - y);
	setPixel(xc + y, yc + x);
	setPixel(xc - y, yc + x);
	setPixel(xc + y, yc - x);
	setPixel(xc - y, yc - x);
}

void circlePlotPointsFill(GLint xc, GLint yc, GLint x, GLint y) {
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glVertex2i(xc - x, yc + y);
	glVertex2i(xc + x, yc + y);
	glVertex2i(xc - x, yc - y);
	glVertex2i(xc + x, yc - y);
	glVertex2i(xc - y, yc + x);
	glVertex2i(xc + y, yc + x);
	glVertex2i(xc - y, yc - x);
	glVertex2i(xc + y, yc - x);
	glEnd();
}

void circleMidpoint(GLint xc, GLint yc, GLint r) {
	GLint p = 1 - r;
	GLint x = 0, y = r;

	circlePlotPoints(xc, yc, x, y);
	while (x < y) {
		x++;
		if (p < 0) {
			p += 2 * x + 1;
		} else {
			y--;
			p += 2 * (x - y) + 1;
		}
		circlePlotPoints(xc, yc, x, y);
	}
}

void circleMidpointFill(GLint xc, GLint yc, GLint r) {
	GLint p = 1 -r;
	GLint x = 0, y = r;

	circlePlotPointsFill(xc, yc, x, y);
	while (x < y) {
		x++;
		if (p < 0) {
			p += 2 * x + 1;
		} else {
			y--;
			p += 2 * (x - y) + 1;
		}
		circlePlotPointsFill(xc, yc, x, y);
	}
}

void select(GLint button, GLint action, GLint x, GLint y) {

	if (action == GLUT_DOWN && button != GLUT_RIGHT_BUTTON) {
		int i = numObj;
		int check = 0;

		while (i >= 0 && check == 0) {
			if (list[i].t == 1) {
				printf("select func b2f");
				if (list[i].x1 < x && list[i].x2 > x && list[i].y1 < y
						&& list[i].y2 > y) {
					check = 1;
					colors[1].r = list[i].r;
					colors[1].g = list[i].g;
					colors[1].b = list[i].b;

					if (selected != -1) {
						list[selected].r = colors[1].r;
						list[selected].g = colors[1].g;
						list[selected].b = colors[1].b;
					}

					list[i].r = 1.0;
					list[i].g = 1.0;
					list[i].b = 0.0;

					selected = i;
				}
				else if (list[i].x1 > x && list[i].x2 < x && list[i].y1 < y
						&& list[i].y2 > y) {
					check = 1;
					colors[1].r = list[i].r;
					colors[1].g = list[i].g;
					colors[1].b = list[i].b;

					if (selected != -1) {
						list[selected].r = colors[1].r;
						list[selected].g = colors[1].g;
						list[selected].b = colors[1].b;
					}

					list[i].r = 1.0;
					list[i].g = 1.0;
					list[i].b = 0.0;

					selected = i;
				}
				else if (list[i].x1 < x && list[i].x2 > x && list[i].y1 > y
						&& list[i].y2 < y) {
					check = 1;
					colors[1].r = list[i].r;
					colors[1].g = list[i].g;
					colors[1].b = list[i].b;

					if (selected != -1) {
						list[selected].r = colors[1].r;
						list[selected].g = colors[1].g;
						list[selected].b = colors[1].b;
					}

					list[i].r = 1.0;
					list[i].g = 1.0;
					list[i].b = 0.0;

					selected = i;
				}
				else if (list[i].x1 > x && list[i].x2 < x && list[i].y1 > y
						&& list[i].y2 < y) {
					check = 1;
					colors[1].r = list[i].r;
					colors[1].g = list[i].g;
					colors[1].b = list[i].b;

					if (selected != -1) {
						list[selected].r = colors[1].r;
						list[selected].g = colors[1].g;
						list[selected].b = colors[1].b;
					}

					list[i].r = 1.0;
					list[i].g = 1.0;
					list[i].b = 0.0;

					selected = i;
				}

			}
			if (list[i].t == 2) {

				if (sqrt(
						(abs(list[i].x1 - list[i].x2) ^ 2)
								+ (abs(list[i].y1 - list[i].y2) ^ 2))
						> sqrt(
								(abs(list[i].x1 - x) ^ 2)
										+ (abs(list[i].y1 - y) ^ 2))) {

							check = 1;
							colors[1].r = list[i].r;
							colors[1].g = list[i].g;
							colors[1].b = list[i].b;

							if (selected != -1) {
								list[selected].r = colors[1].r;
								list[selected].g = colors[1].g;
								list[selected].b = colors[1].b;
							}
							list[i].r = 1.0;
							list[i].g = 1.0;
							list[i].b = 0.0;

							selected = i;
					}
				}
			i--;
		}
		glutPostRedisplay();
	}
}


// this function draws the list of objects
void drawList() {
	glClear(GL_COLOR_BUFFER_BIT); // Clear display window.
	for (int i = 0; i <= numObj; i++) {
		//setPixel(points[i].x, points[i].y);

		glColor3f(list[i].r, list[i].g, list[i].b);

		//For outlined rectangle
		if (list[i].t == 1 && list[i].s == 2) {
			plotObject(list[i].x1, list[i].y1, list[i].x2, list[i].y2);
		}
		//For filled rectangle
		else if (list[i].t == 1 && list[i].s == 1) {
			glRecti(list[i].x1, list[i].y1, list[i].x2, list[i].y2);
		}
		//For outlined circle
		else if (list[i].t == 2 && list[i].s == 2) {
			circleMidpoint(list[i].x1, list[i].y1,
					abs(list[i].x2 - list[i].x1));
		}
		//For filled circle
		else if (list[i].t == 2 && list[i].s == 1) {
			circleMidpointFill(list[i].x1, list[i].y1,
					abs(list[i].x2 - list[i].x1));
		}
	}

	glFlush();
}

void mouseDraw(GLint button, GLint action, GLint xMouse, GLint yMouse) {

	glFlush();

	if (button == GLUT_LEFT_BUTTON) {
		if (action == GLUT_DOWN && button != GLUT_RIGHT_BUTTON
				&& numObj < MaxNumObj - 1) {

			list[numObj].x1 = xMouse;
			list[numObj].y1 = yMouse;
			list[numObj].x2 = xMouse;
			list[numObj].y2 = yMouse;
			list[numObj].t = type;
			list[numObj].s = 2;
			list[numObj].r = red;
			list[numObj].g = green;
			list[numObj].b = blue;
			glutPostRedisplay();
		} else if (action == GLUT_UP && button != GLUT_RIGHT_BUTTON) {
			list[numObj].x2 = xMouse;
			list[numObj].y2 = yMouse;
			list[numObj].s = style;
			numObj++;
			glutPostRedisplay();
		}
	}
	glFlush();
}
void deselect(GLint button, GLint action, GLint x, GLint y) {
	if (selection == 2) {
		list[selected].r = colors[1].r;
		list[selected].g = colors[1].g;
		list[selected].b = colors[1].b;
		glutPostRedisplay();
	}
	glutMouseFunc(mouseDraw);
	glFlush();

}


void SaveSVG() {

	FILE *outFile;
	outFile = fopen("beautiful drawing.xml", "w");
	char tmp[500];
	int topX = 0, topY = 0, cX = 0, cY = 0, r = 0;
	int objWidth, objHeight; //store calculated height and width

	//write header data for SVG
	sprintf(tmp, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
	fputs(tmp, outFile);
	sprintf(tmp, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n");
	fputs(tmp, outFile);
	sprintf(tmp, "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
	fputs(tmp, outFile);
	sprintf(tmp,
			"<svg width=\"%d\" height=\"%d\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n",
			winWidth, winHeight);
	fputs(tmp, outFile);

	for (int i = 1; i <= numObj - 1; i++) {
		if (list[i].t == 1) {
			if (list[i].x2 >= list[i].x1) {

				objWidth = list[i].x2 - list[i].x1;
				topX = list[i].x1;
			} else {
				objWidth = list[i].x1 - list[i].x2;
				topX = list[i].x2;
			}

			if (list[i].y2 >= list[i].y1) {
				objHeight = list[i].y2 - list[i].y1;
				topY = list[i].y1;

			} else {
				objHeight = list[i].y1 - list[i].y2;
				topY = list[i].y2;
			}

			if (list[i].s == 1) { //filled
				sprintf(tmp,
						"<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(%d,%d,%d);stroke-width:1;stroke:rgb(%d,%d,%d)\"/>\n",
						topX, topY, objWidth, objHeight,
						(int) (list[i].r * 255), (int) (list[i].g * 255),
						(int) (list[i].b * 255), (int) (list[i].r * 255),
						(int) (list[i].g * 255), (int) (list[i].b * 255));
				fputs(tmp, outFile);
			}
			if (list[i].s == 2) { //outline
				sprintf(tmp,
						"<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:none;stroke-width:1;stroke:rgb(%d,%d,%d)\"/>\n",
						topX, topY, objWidth, objHeight,
						(int) (list[i].r * 255), (int) (list[i].g * 255),
						(int) (list[i].b * 255));
				fputs(tmp, outFile);
			}

		} else if (list[i].t == 2) {
			cX = list[i].x1;
			cY = list[i].y1;
			r = abs(cX - list[i].x2);
			if (list[i].s == 1) {
				sprintf(tmp,
						"<circle cx=\"%d\" cy=\"%d\" r=\"%d\" style=\"fill:rgb(%d,%d,%d);stroke-width:1;stroke:rgb(%d,%d,%d)\"/>\n",
						cX, cY, r, (int) (list[i].r * 255),
						(int) (list[i].g * 255), (int) (list[i].b * 255),
						(int) (list[i].r * 255), (int) (list[i].g * 255),
						(int) (list[i].b * 255));

				fputs(tmp, outFile);
			}
			if (list[i].s == 2) {
				sprintf(tmp,
						"<circle cx=\"%d\" cy=\"%d\" r=\"%d\" style=\"fill:none;stroke-width:1;stroke:rgb(%d,%d,%d)\"/>\n",
						cX, cY, r, (int) (list[i].r * 255),
						(int) (list[i].g * 255), (int) (list[i].b * 255));

				fputs(tmp, outFile);
			}
		}

	}
}

//delete function
void Delete(GLint j)   // j is an index for the selected object
		{
	GLint i;
	for (i = j; i < numObj; i++) {
		list[i].x1 = list[i + 1].x1;
		list[i].y1 = list[i + 1].y1;
		list[i].x2 = list[i + 1].x2;
		list[i].y2 = list[i + 1].y2;
		list[i].r = list[i + 1].r;
		list[i].g = list[i + 1].g;
		list[i].b = list[i + 1].b;
		list[i].t = list[i + 1].t;
		list[i].s = list[i + 1].s;
	}
	numObj--;
	selection = 0;  // selection = 1 if an object is selected, 0 otherwise

}

// this function takes the mouse position while moving mouse, use this for intermediate drawing
void Motion(GLint x, GLint y) {
	// add the (x, y) coordinates of the second point to the intermediate rectangle
	list[numObj].x2 = x;
	list[numObj].y2 = y;
	// redisplay the object list after the above insertion. It will give a rubber band effect
	glutPostRedisplay();
	}

// reshape function for resized the window
void winReshapeFcn(GLint newWidth, GLint newHeight) {
	/* Reset viewport and projection parameters */
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(newWidth), GLdouble(newHeight), 0.0);
	/* Reset display-window size parameters. */
	winWidth = newWidth;
	winHeight = newHeight;
	drawList();
	glFlush();
}

void barGraph() {
	glRecti(150, 100, 250, 350);
	glRecti(350, 40, 450, 350);
	glRecti(550, 150, 650, 350);
	glutPostRedisplay();
}

void mainMenu(GLint menuOption) {
	switch (menuOption) {
	case 1:
		for (int i = numObj - 1; i >= 0; i--) {
			list[i].x1 = NULL;
			list[i].y1 = NULL;
			list[i].x2 = NULL;
			list[i].y2 = NULL;
			list[i].r = NULL;
			list[i].g = NULL;
			list[i].b = NULL;
			list[i].t = NULL;
			list[i].s = NULL;
		}
		numObj = 0;
		selection = 0;
		move = 0;
		break;
	case 2:
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void Move(GLint x1, GLint y1, GLint x2, GLint y2) { //doesnt work

	int newX = 0, newY = 0; //The new values
	int dx1 = 0, dy1 = 0; //Delta x1 and delta y1, new clicked position
	int Width, Height; //Overall width and height of the object


	if (list[i].t == 1) {
		if (list[i].x2 >= list[i].x1) {
			Width = list[i].x2 - list[i].x1;
			newX = list[i].x1;
		} else {
			Width = list[i].x1 - list[i].x2;
			newX = list[i].x2;
		}

		if (list[i].y2 >= list[i].y1) {
			Height = list[i].y2 - list[i].y1;
			newY = list[i].y1;
		} else {
			Height = list[i].y1 - list[i].y2;
			newY = list[i].y2;
		}

		list[i].x1 = newX;
		list[i].y1 = newY;
		list[i].y2 = newY + Height;
		list[i].x2 = newX + Width;

		dx1 = x1 - list[i].x1;
		dy1 = y1 - list[i].y1;

		list[i].x1 = x2 - dx1;
		list[i].y1 = y2 - dy1;
		list[i].y2 = list[i].x1 + Height;
		list[i].x2 = list[i].y1 + Width;

		glutPostRedisplay();

	}

}
void colorSubMenu(GLint colorOption) {
	switch (colorOption) {
	case 1: {
		red = 0.0;
		green = 0.0;
		blue = 1.0;
	}
		break;
	case 2: {
		red = 0.0;
		green = 1.0;
		blue = 0.0;
	}
		break;
	case 3: {
		red = 1.0;
		green = 0.0;
		blue = 0.0;
	}
		break;
	case 4: {
		red = 1.0;
		green = 1.0;
		blue = 1.0;
	}
		break;
	case 5: {
		red = 0.0;
		green = 0.0;
		blue = 0.0;
	}
		break;
	case 6: {
		red = 1.0;
		green = 0.0;
		blue = 1.0;
	}
		break;
	case 7: {
		red = 0.0;
		green = 1.0;
		blue = 1.0;
	}
	}
	glutPostRedisplay();
}

void styleSubMenu(GLint styleOption) {
	switch (styleOption) {
	case 1: {
		//filled
		style = 1;
	}
		break;
	case 2: {
		//outline
		style = 2;
	}
		break;
	}
	glutPostRedisplay();
}

void typeSubMenu(GLint typeOption) {
	switch (typeOption) {
	case 1: {
		type = 1;
	}
		break;

	case 2: {
		type = 2;
	}
	break;
	}
}

void dataSubMenu(GLint dataOption) {
	switch (dataOption) {
	case 1: {
		barGraph();
	}
		break;
	case 2: {
		//pieChart();
	}
	}
}

void fileSubMenu(GLint fileOption) {
	switch (fileOption) {
	case 1: {
		//save to svg
		SaveSVG();
	}
		break;
	case 2: {
		//read svg
		//ReadSVG();
	}
		break;
	case 3: {
		SaveBitmap("shapes.bmp", 0, 0, 640, 480);
	}
	}
}
void editSubMenu(GLint editOption) {
	switch (editOption) {
		case 1: {
		selection = 1;
		glutMouseFunc(select);

		}

		break;
		case 2: {
		selection = 2;
		glutMouseFunc(deselect);
		}

		break;
		case 3: {
		selection = 3;

		}

		break;
		case 4: {
		selection = 4;
		printf("case b2f");
		glutMouseFunc(select);
		moveToFront();

		}
		break;
		case 5: {
		selection = 5;
		glutMouseFunc(select);
		sendToBack();
		}
		break;
	case 6: {
		selection = 6;
		glutMouseFunc(Move);

	}
		break;
	}
}

#define GL_BGR_EXT 0x80E0
void SaveBitmap(const char *ptrcFileName, int nX, int nY, int nWidth,
		int nHeight) {
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;

	unsigned char *ptrImage = (unsigned char*) malloc(
			sizeof(unsigned char) * nWidth * nHeight * 3
					+ (4 - (3 * nWidth) % 4) * nHeight);

	FILE *ptrFile = fopen(ptrcFileName, "wb");

	//read pixels from framebuffer
	glReadPixels(nX, nY, nWidth, nHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE,
			ptrImage);

	// set memory buffer for bitmap header and informaiton header
	memset(&bf, 0, sizeof(bf));
	memset(&bi, 0, sizeof(bi));

	// configure the headers with the give parameters

	bf.bfType = 0x4d42;
	bf.bfSize = sizeof(bf) + sizeof(bi) + nWidth * nHeight * 3
			+ (4 - (3 * nWidth) % 4) * nHeight;
	bf.bfOffBits = sizeof(bf) + sizeof(bi);
	bi.biSize = sizeof(bi);
	bi.biWidth = nWidth + nWidth % 4;
	bi.biHeight = nHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 24;
	bi.biSizeImage = nWidth * nHeight * 3 + (4 - (3 * nWidth) % 4) * nHeight;

	// to files
	fwrite(&bf, sizeof(bf), 1, ptrFile);
	fwrite(&bi, sizeof(bi), 1, ptrFile);
	fwrite(ptrImage, sizeof(unsigned char),
			nWidth * nHeight * 3 + (4 - (3 * nWidth) % 4) * nHeight, ptrFile);

	fclose(ptrFile);
	free(ptrImage);

}
// main function
int main(int argc, char** argv) {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	glutInit(&argc, argv);

	// declare submenu variables and set menu here
	//GLint type_subMenu, style_subMenu, color_subMenu, edit_subMenu,
	//file_subMenu, dataview_subMenu;

	int style_subMenu;
	style_subMenu = glutCreateMenu(styleSubMenu);
	glutAddMenuEntry("Outline", 2);
	glutAddMenuEntry("Filled", 1);

	int color_subMenu;
	color_subMenu = glutCreateMenu(colorSubMenu);
	glutAddMenuEntry("Red", 3);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 1);
	glutAddMenuEntry("White", 4);
	glutAddMenuEntry("Black", 5);
	glutAddMenuEntry("Pink", 6);
	glutAddMenuEntry("Cyan", 7);

	int data_subMenu;
	data_subMenu = glutCreateMenu(dataSubMenu);
	glutAddMenuEntry("Bar Graph", 1);
	glutAddMenuEntry("Pie Chart", 2);

	int file_subMenu;
	file_subMenu = glutCreateMenu(fileSubMenu);
	glutAddMenuEntry("Save SVG ", 1);
	glutAddMenuEntry("Open SVG ", 2);
	glutAddMenuEntry("Save to Bitmap ", 3);

	int type_subMenu;
	type_subMenu = glutCreateMenu(typeSubMenu);
	glutAddMenuEntry("Rectangle", 1);
	glutAddMenuEntry("Circle", 2);

	int edit_subMenu;
	edit_subMenu = glutCreateMenu(editSubMenu);
	glutAddMenuEntry("Select", 1);
	glutAddMenuEntry("Deselect", 2);
	glutAddMenuEntry("Delete", 3);
	glutAddMenuEntry("Bring to front", 4);
	glutAddMenuEntry("Send to back", 5);
	glutAddMenuEntry("Move", 6);


	glutCreateMenu(mainMenu); // Create main pop-up menu.
	glutAddMenuEntry(" New ", 1);
	glutAddSubMenu(" Draw ", type_subMenu);
	glutAddSubMenu(" Edit ", edit_subMenu);
	glutAddSubMenu(" Style ", style_subMenu);
	glutAddSubMenu(" Colors ", color_subMenu);
	glutAddSubMenu(" Files ", file_subMenu);
	glutAddSubMenu(" Data Visualization ", data_subMenu);
	glutAddMenuEntry(" Quit", 2);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow(
			"SimpleDraw Sample for Assignment 1 (by your first name here)");

	init();

	// register call back functions
	glutDisplayFunc(drawList);
	glutReshapeFunc(winReshapeFcn);
	glutMouseFunc(mouseDraw);
	glutMotionFunc(Motion);


	//add right click menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}

