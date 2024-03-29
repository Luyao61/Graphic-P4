#include <math.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "Matrix4.h"
#include "Vector4.h"
#include "Vector3.h"

Matrix4::Matrix4()
{
    std::memset(m, 0, sizeof(m));
}

Matrix4::Matrix4(
                 float m00, float m01, float m02, float m03,
                 float m10, float m11, float m12, float m13,
                 float m20, float m21, float m22, float m23,
                 float m30, float m31, float m32, float m33 )
{
    this->set(m00, m01, m02, m03,
              m10, m11, m12, m13,
              m20, m21, m22, m23,
              m30, m31, m32, m33);
}

void Matrix4::set(float m00, float m01, float m02, float m03,
                  float m10, float m11, float m12, float m13,
                  float m20, float m21, float m22, float m23,
                  float m30, float m31, float m32, float m33)
{
    m[0][0] = m00;
    m[0][1] = m01;
    m[0][2] = m02;
    m[0][3] = m03;
    m[1][0] = m10;
    m[1][1] = m11;
    m[1][2] = m12;
    m[1][3] = m13;
    m[2][0] = m20;
    m[2][1] = m21;
    m[2][2] = m22;
    m[2][3] = m23;
    m[3][0] = m30;
    m[3][1] = m31;
    m[3][2] = m32;
    m[3][3] = m33;
}

float Matrix4::get(int column, int element)
{
    return m[column][element];
}

Matrix4& Matrix4::operator=(Matrix4 a)
{
    std::memcpy(m, a.m, sizeof(m));
    return *this;
}

float* Matrix4::ptr()
{
    return &m[0][0];
}

void Matrix4::identity()
{
    static const float ident[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    std::memcpy(m, ident, sizeof(m));
}

Matrix4 Matrix4::multiply(Matrix4 a)
{
    Matrix4 b;
    
    //Implement a more performant Matrix * Matrix multiplication
    
    //The current implementation below is not very efficient:
    //It allocates an additional 8 vectors on the stack and calls their constructors
    //It also calls off to additional functions (which create even more vectors!)
    //Which results in a lot of time being wasted on memory operations
    //This is bad, really bad, ultra bad D:
    
    //Hint: Loops!
    //Hint for the ambitious: SIMD!
    
    /*
    Vector4 row1(m[0][0], m[1][0], m[2][0], m[3][0]);
    Vector4 row2(m[0][1], m[1][1], m[2][1], m[3][1]);
    Vector4 row3(m[0][2], m[1][2], m[2][2], m[3][2]);
    Vector4 row4(m[0][3], m[1][3], m[2][3], m[3][3]);
    
    Vector4 col1(a.m[0][0], a.m[0][1], a.m[0][2], a.m[0][3]);
    Vector4 col2(a.m[1][0], a.m[1][1], a.m[1][2], a.m[1][3]);
    Vector4 col3(a.m[2][0], a.m[2][1], a.m[2][2], a.m[2][3]);
    Vector4 col4(a.m[3][0], a.m[3][1], a.m[3][2], a.m[3][3]);

    b.set(row1.dot(col1), row2.dot(col1), row3.dot(col1), row4.dot(col1),
             row1.dot(col2), row2.dot(col2), row3.dot(col2), row4.dot(col2),
             row1.dot(col3), row2.dot(col3), row3.dot(col3), row4.dot(col3),
             row1.dot(col4), row2.dot(col4), row3.dot(col4), row4.dot(col4) );
    
    */
    
    
    for (int i=0; i<4; i++ ){
        for (int j = 0; j<4; j++){
            b.m[i][j] = m[0][j] * a.m[i][0];
            for (int k = 1; k<4; k++){
                b.m[i][j] += m[k][j] * a.m[i][k];
            }
        }
    }
    

    return b;
}

Matrix4 Matrix4::operator * (Matrix4 a)
{
    return multiply(a);
}

Vector4 Matrix4::multiply(Vector4 a)
{
    Vector4 b;
    
    //Implement Matrix * Vector4 multiplication
    for (int i =0; i <4; i++) {
        for (int j = 0; j<4; j++) {
            *(b.ptr() + i) += m[j][i] * *(a.ptr() + j);
        }
    }
    
    return b;
}

Vector4 Matrix4::operator * (Vector4 a)
{
    return multiply(a);
}

Vector3 Matrix4::multiply(Vector3 a)
{
    Vector3 b;
    Vector4 temp;
    
    //Implement Matrix * Vector3 multiplication
    //Assume the 4th component is 0
    for (int i =0; i <4; i++) {
        for (int j = 0; j<3; j++) {
            *(b.ptr()+i) += m[j][i] * *(a.ptr() +j);
        }
    }
    
    
    return b;
}

Vector3 Matrix4::operator * (Vector3 a)
{
    return multiply(a);
}

Matrix4 Matrix4::makeRotateX(float angle)
{
    identity();
    
    //Configure this matrix to be a rotation about the X-Axis by 'angle' radians
    m[1][1] = cos(angle);
    m[1][2] = sin(angle);
    m[2][1] = -sin(angle);
    m[2][2] = cos(angle);
    
    return *this;
}

Matrix4 Matrix4::makeRotateY(float angle)
{
    identity();
    
	m[0][0] = cos(angle);
	m[0][2] = -sin(angle);
	m[2][0] = sin(angle);
	m[2][2] = cos(angle);
    
    return *this;
}

Matrix4 Matrix4::makeRotateZ(float angle)
{
    identity();
    
    //Configure this matrix to be a rotation about the Z-Axis by 'angle' radians
    m[0][0] = cos(angle);
    m[0][1] = sin(angle);
    m[1][0] = -sin(angle);
    m[1][1] = cos(angle);
    
    return *this;
}
///////unchangeds!!!!!!!!!!!!!!!!!!!!!!!~~~~~~~~~~~~~~
Matrix4 Matrix4::makeRotateArbitrary(Vector3 a, float angle)
{
    identity();
    a = a.normalize();
    float ux = *a.ptr();
    float uy = *(a.ptr()+1);
    float uz = *(a.ptr()+2);
    
    //Configure this matrix to be a rotation about the 'a' axis by 'angle' radians
    m[0][0] = cos(angle) + ux*ux*(1-cos(angle));
    m[1][0] = ux*uy*(1-cos(angle)) - uz*sin(angle);
    m[2][0] = ux*uz*(1-cos(angle)) + uy*sin(angle);
    m[0][1] = uy*ux*(1-cos(angle)) + uz*sin(angle);
    m[1][1] = cos(angle) + uy*uy*(1-cos(angle));
    m[2][1] = uy*uz*(1-cos(angle)) - ux*sin(angle);
    m[0][2] = uz*ux*(1-cos(angle)) - uy*sin(angle);
    m[1][2] = uz*uy*(1-cos(angle)) + ux*sin(angle);
    m[2][2] = cos(angle) + uz*uz*(1-cos(angle));

    return *this;
}

Matrix4 Matrix4::makeScale(float s)
{
    return makeScale(s, s, s);
}

Matrix4 Matrix4::makeScale(float sx, float sy, float sz)
{
    identity();
    
    //Configure this matrix to be a sclaing by sx, sy, sz
    m[0][0] = sx;
    m[1][1] = sy;
    m[2][2] = sz;

    return *this;
}

Matrix4 Matrix4::makeTranslate(float x, float y, float z)
{
    identity();
    
    //Configure this matrix to be a translation by vector 'a'
    m[3][0] = x;
    m[3][1] = y;
    m[3][2] = z;
    
    return *this;
}

Matrix4 Matrix4::makeTranslate(Vector3 a)
{
    return makeTranslate(a[0], a[1], a[2]);
}

Matrix4 Matrix4::transpose(void)
{
    Matrix4 b;
    for(int x = 0; x < 4; ++x)
    {
        for(int y = 0; y < 4; ++y)
        {
            b.m[y][x] = m[x][y];
        }
    }
    return b;
}

//Hint: Try basing this on code by cool people on the internet
//In this class it is okay to use code from the internet
//So long as you fully understand the code and can clearly explain it if asked to
//http://stackoverflow.com/questions/2624422/efficient-4x4-matrix-inverse-affine-transform
Matrix4 Matrix4::inverse(void)
{
    Matrix4 b;
    float s0 = m[0][0] * m[1][1] - m[1][0] * m[0][1];
    float s1 = m[0][0] * m[1][2] - m[1][0] * m[0][2];
    float s2 = m[0][0] * m[1][3] - m[1][0] * m[0][3];
    float s3 = m[0][1] * m[1][2] - m[1][1] * m[0][2];
    float s4 = m[0][1] * m[1][3] - m[1][1] * m[0][3];
    float s5 = m[0][2] * m[1][3] - m[1][2] * m[0][3];
    
    float c5 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
    float c4 = m[2][1] * m[3][3] - m[3][2] * m[2][3];
    float c3 = m[2][1] * m[3][3] - m[3][2] * m[2][3];
    float c2 = m[2][0] * m[3][3] - m[3][2] * m[2][3];
    float c1 = m[2][0] * m[3][3] - m[3][2] * m[2][3];
    float c0 = m[2][0] * m[3][3] - m[3][2] * m[2][3];
    
    float invdet = 1/ (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);
    float b00 = (m[1][1] * c5 - m[1][2] * c4 + m[1][3] * c3) * invdet;
    float b01 = (-m[0][1] * c5 + m[0][2] * c4 + m[0][3] * c3) * invdet;
    float b02 = (m[3][1] * s5 - m[3][2] * s4 + m[3][3] * s3) * invdet;
    float b03 = (-m[2][1] * s5 + m[2][2] * s4 + m[2][3] * s3) * invdet;
    
    float b10 = (-m[1][0] * c5 + m[1][2] * c2 - m[1][3] * c1) * invdet;
    float b11 = (m[0][0] * c5 - m[0][2] * c2 + m[0][3] * c1) * invdet;
    float b12 = (-m[3][0] * s5 + m[3][2] * s2 - m[3][3] * s1) * invdet;
    float b13 = (m[2][0] * s5 - m[2][2] * s2 + m[2][3] * s1) * invdet;
    
    float b20 = (m[1][0] * c4 - m[1][1] * c2 + m[1][3] * c0) * invdet;
    float b21 = (-m[0][0] * c4 + m[0][1] * c2 + m[0][3] * c0) * invdet;
    float b22 = (m[3][0] * s4 - m[3][1] * s2 + m[3][3] * s0) * invdet;
    float b23 = (-m[2][0] * s4 + m[2][1] * s2 + m[2][3] * s0) * invdet;
    
    float b30 = (-m[1][0] * c3 + m[1][1] * c1 - m[1][2] * c0) * invdet;
    float b31 = (m[0][0] * c3 - m[0][1] * c1 + m[0][2] * c0) * invdet;
    float b32 = (-m[3][0] * s3 + m[3][1] * s1 - m[3][2] * s0) * invdet;
    float b33 = (m[2][0] * s3 - m[2][1] * s1 + m[2][2] * s0) * invdet;
    
    
    b.set(b00, b01, b02, b03, b10, b11, b12, b13, b20, b21, b22, b23, b30, b31, b32, b33);
    
    
    //Not required
    //Calculate the inverse of this matrix
    
    return b;
}

Matrix4 Matrix4::rigidInverse(void)
{
    Matrix4 b;
    
    //Project 2
    //Calculate the inverse of this matrix with the assumption that it is a rigid transformation
    //This will be useful when implementing cameras!
    
    /*
	float x = m[0][0] * m[3][0] + m[0][1] * m[3][1] + m[0][2] * m[3][2];
	float y = m[1][0] * m[3][0] + m[1][1] * m[3][1] + m[1][2] * m[3][2];
	float z = m[2][0] * m[3][0] + m[2][1] * m[3][1] + m[2][2] * m[3][2];

	b.set(m[0][0], m[1][0], m[2][0], 0,
		  m[0][1], m[1][1], m[2][1], 0, 
		  m[0][2], m[1][2], m[2][2], 0, 
		  -x, -y, -z, 1);
     
     */

    return b;
}


Matrix4 Matrix4::makePerspectiveProjection(float fov, float width, float height, float near, float far)
{
    identity();
    
    //Project 3
    //Make this matrix a perspectice project matrix using fov, width, height, near and far
    //See the lecture slides for details
    
    return *this;
}

Matrix4 Matrix4::makeViewport(float xmin, float xmax, float ymin, float ymax)
{
    identity();
    
    //Project 3
    //Make this matrix a viewport matrix using xmin, xmax, ymin, and ymax
    //See the lecture slides for details
    
    return *this;
}

void Matrix4::print(std::string comment)
{
    //Width constants and variables
    static const int pointWidth = 1;
    static const int precisionWidth = 4;
    int integerWidth = 1;
    
    //Determine the necessary width to the left of the decimal point
    float* elementPtr = (float*)m;
    float maxValue = fabsf(*(elementPtr++));
    while(elementPtr++ < ((float*)m+16)) if(fabsf(*elementPtr) > maxValue) maxValue = fabsf(*elementPtr);
    while(maxValue >= 10.0) { ++integerWidth; maxValue /= 10.0; }
    
    //Sum up the widths to determine the cell width needed
    int cellWidth = integerWidth + pointWidth + precisionWidth;
    
    //Set the stream parameters for fixed number of digits after the decimal point
    //and a set number of precision digits
    std::cout << std::fixed;
    std::cout << std::setprecision(precisionWidth);
    
    //Print the comment
    std::cout << comment << std::endl;
    
    //Loop through the matrix elements, format each, and print them to screen
    float cellValue;
    for(int element = 0; element < 4; element++)
    {
        std::cout << std::setw(1) << (element == 0 ? "[" : " ");
        for(int vector = 0; vector < 4; vector++)
        {
            cellValue =  m[vector][element];
            std::cout << std::setw(cellWidth + (cellValue >= 0.0 ? 1 : 0)) << cellValue;
            std::cout << std::setw(0) << (vector < 3 ? " " : "");
        }
        std::cout << std::setw(1) << (element == 3 ? "]" : " ") << std::endl;
    }
}
