/// Texture
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <string.h>

using namespace std;

typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char uchar;

fstream inf; // global in this file for convenience

// A few class definitions
class mRGB {
public:
	unsigned char r,g,b;

	mRGB(){r = g = b = 0;}
	mRGB(mRGB& p){r = p.r; g = p.g; b = p.b;}
	mRGB(uchar rr, uchar gg, uchar bb){r = rr; g = gg; b = bb;}
	void set(uchar rr, uchar gg, uchar bb){r = rr; g = gg; b = bb;}
};

ushort getShort() //helper function
{ //BMP format uses little-endian integer types
  // get a 2-byte integer stored in little-endian form
		char ic;
		ushort ip;
		inf.get(ic); ip = ic;  //first byte is little one
		inf.get(ic);  ip |= ((ushort)ic << 8); // or in high order byte
		return ip;
}

ulong getLong() //helper function
{  //BMP format uses little-endian integer types
   // get a 4-byte integer stored in little-endian form
		ulong ip = 0;
		char ic = 0;
		unsigned char uc = ic;
		inf.get(ic); uc = ic; ip = uc;
		inf.get(ic); uc = ic; ip |=((ulong)uc << 8);
		inf.get(ic); uc = ic; ip |=((ulong)uc << 16);
		inf.get(ic); uc = ic; ip |=((ulong)uc << 24);
		return ip;
	}

class RGBpixmap {
public:
	int nRows, nCols;
	mRGB *pixel;

	float gr, gg, gb;

	void MakeCheckerBoard();
	void MakeShadedCircle();
	void MakeRandom();
	void SetTexColor(float rin, float gin, float bin);
	void SetTexture(GLuint textureName);
	int readBMPFile(char *fname);
};

void RGBpixmap::SetTexColor(float rin, float gin, float bin)
{
	// Set multipliers to set the color of the parameterized textures
	gg = gin;
	gr = rin;
	gb = bin;
}

void RGBpixmap::MakeCheckerBoard()
{
	// Set size of the checkerboard
	nRows = nCols = 64;
	int numBytes = 3 * nRows*nCols;
	long count = 0;
	pixel = new mRGB[numBytes];

	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
		{
			int c = ((i & 8) ^ (j & 8)) * 255;
			pixel[count].r = (unsigned char)gr*c;
			pixel[count].g = (unsigned char)gg*c;
			pixel[count++].b = (unsigned char)gb*c;
		}
}

void RGBpixmap::MakeRandom()
{
	// assign random rgb values
	nRows = nCols = 64;
	int numBytes = 3 * nRows*nCols;
	long count = 0;
	pixel = new mRGB[numBytes];

	// Assign random values for each pixel
	// Include multipliers (gr, gg, gb) to give the texture a color
	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
		{
			int c = rand() % 255;
			pixel[count].r = (unsigned char)gr*c;
			pixel[count].g = (unsigned char)gg*c;
			pixel[count++].b = (unsigned char)gb*c;
		}
}

float fakeSphere(float s, float t) {
	float r = sqrt((s - 0.5)*(s - 0.5) + (t - 0.5)*(t - 0.5));
	// distance from sphere’s center to (s,t)
	if (r<0.3) return 1 - r / 0.3;
	// sphere intensity bright at center
	// dark along edges
	else return 0.2; // dark background
}

void RGBpixmap::MakeShadedCircle()
{
	// Make a shaded circle
	nRows = nCols = 64;
	int numBytes = 3 * nRows*nCols;
	pixel = new mRGB[numBytes];
	float r, val;
	long count = 0;

	// Assign pixel vales based on shaded circle given in class presentations
	// Include multipliers (gr, gg, gb) to give the texture a color
	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
		{
			int c = rand() % 255;
			pixel[count].r = fakeSphere((float)i / nRows, (float)j / nCols) * 255.0;
			pixel[count].g = fakeSphere((float)i / nRows, (float)j / nCols) * 255.0;
			pixel[count++].b = fakeSphere((float)i / nRows, (float)j / nCols) * 255.0;
		}
}

int RGBpixmap::readBMPFile(char *fname)
{  // Read into memory an mRGB image from an uncompressed BMP file.
	// return 0 on failure, 1 on success
	inf.open(fname, ios::in|ios::binary); //read binary char's
	if(!inf){ cout << " can't open file: " << fname << endl; return 0;}
	int k, row, col, numPadBytes, nBytesInRow;
	// read the file header information
	char ch1, ch2;
	inf.get(ch1); inf.get(ch2); //type: always 'BM'
	ulong fileSize =      getLong();
	ushort reserved1 =    getShort();    // always 0
	ushort reserved2= 	getShort();     // always 0
	ulong offBits =		getLong(); // offset to image - unreliable
	ulong headerSize =     getLong();     // always 40
	ulong numCols =		getLong(); // number of columns in image
	ulong numRows = 		getLong(); // number of rows in image
	ushort planes=    	getShort();      // always 1
	ushort bitsPerPixel=   getShort();    //8 or 24; allow 24 here
	ulong compression =    getLong();      // must be 0 for uncompressed
	ulong imageSize = 	getLong();       // total bytes in image
	ulong xPels =    	getLong();    // always 0
	ulong yPels =    	getLong();    // always 0
	ulong numLUTentries = getLong();    // 256 for 8 bit, otherwise 0
	ulong impColors = 	getLong();       // always 0
	if(bitsPerPixel != 24)
	{ // error - must be a 24 bit uncompressed image
		cout << "not a 24 bit/pixelimage, or is compressed!\n";
		inf.close(); return 0;
	}
	//add bytes at end of each row so total # is a multiple of 4
	// round up 3*numCols to next mult. of 4
	nBytesInRow = ((3 * numCols + 3)/4) * 4;
	numPadBytes = nBytesInRow - 3 * numCols; // need this many
	nRows = numRows; // set class's data members
	nCols = numCols;
    pixel = new mRGB[nRows * nCols]; //make space for array
	if(!pixel) return 0; // out of memory!
	long count = 0;
	char dum;
	for(row = 0; row < nRows; row++) // read pixel values
	{
		for(col = 0; col < nCols; col++)
		{
			char r,g,b;
			inf.get(b); inf.get(g); inf.get(r); //read bytes
			pixel[count].r = r; //place them in colors
			pixel[count].g = g;
			pixel[count++].b = b;
		}
   		for(k = 0; k < numPadBytes ; k++) //skip pad bytes at row's end
			inf >> dum;
	}
	inf.close(); return 1; // success
}

void RGBpixmap::SetTexture(GLuint textureName)
{
	// Set texture parameters to initial values
	// Set wrapping and interpolation

	// Bind 2D textures, based on input name
	// Set appropriate texture parameters
	// Generate 2D texture image - use class member variables: nCols, nRows, pixel
	glBindTexture(GL_TEXTURE_2D, textureName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nCols, nRows, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
}


void InitializeTexture(RGBpixmap textureObject, GLuint *texturename, char *filename)
{
	// Get OpenGL to automatically generate the texture "names"
	//glGenTextures(1, texturename);
	
	// Create the texture data
	textureObject.readBMPFile(filename);

	// Assign the texture - Link the information in pixel to OpenGL
	// Use "Bind" to let OpenGL know what texture we are using
	glBindTexture(GL_TEXTURE_2D, texturename[0]);
	
	// Set wrapping and interpolation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/**************************************
	//
	SE OBTIENE UN MEJOR RESULTADO CON GL_MODULATE QUE CON GL_ADD
	//
	**************************************/
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	// Specify how OpenGL will read our pixel values (by byte)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// "Create" the OpenGL texture - link the actual texture (from name) to the image data (pixel)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureObject.nCols, textureObject.nRows, 0, GL_RGB, GL_UNSIGNED_BYTE, textureObject.pixel);

	// Actually enable the texture
	glEnable(GL_TEXTURE_2D);
}