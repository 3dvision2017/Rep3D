/**
    tools.hpp
    Purpose: tools used in the Rep3D class

    @authors Pascal Enderli, Lukas Frischknecht, Timothy Marvel
    @version 1.0 6/01/17
*/
#pragma once

#include <iostream>
#include <string>			// std::string
#include <Eigen/Dense>
#include <Eigen/Geometry> 	// quaternion; crossproduct
#include <fstream>
#include <vector>			// std::vector
#include <stdlib.h>
#include <cstddef>         	// std::size_t
#include <algorithm>	   	// std::find()
#include <iomanip>
#include <omp.h>			// Multithreading
#include <cmath>
#include <numeric>

using namespace std;
using namespace Eigen;


template<typename T>
inline T max(vector<T> v)
{
    auto biggest = max_element(begin(v), end(v));
	return *biggest;
}

template<typename T>
inline T min(vector<T> v)
{
    auto smallest = min_element(begin(v), end(v));
	return *smallest;
}

template<typename T>
inline double mean(vector<T> v)
{
    T sum = accumulate(v.begin(), v.end(), 0.0);
    return sum/v.size();
}

template<typename T>
inline double median(vector<T> v)
{
  T median;
  size_t size = v.size();

  sort(v.begin(), v.end());

  if (size  % 2 == 0)
  {
      median = (v[size / 2 - 1] + v[size / 2]) / 2;
  }

  else
  {
      median = v[size / 2];
  }

  return median;
}


template<typename T>
inline double stddev(vector<T> v)
{
	double sum = accumulate(v.begin(), v.end(), 0.0);
	double mean = sum / v.size();

	vector<double> diff(v.size());
	transform(v.begin(), v.end(), diff.begin(),bind2nd(minus<double>(), mean));
	double sq_sum = inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
	return sqrt(sq_sum / v.size());
}


inline bool inPolygon(const float* polyX,const float* polyY,const float u, const float v,const unsigned int n_corners)
{
	// source http://alienryderflex.com/polygon/

	//  n_corners	//how many corners the polygon has

	//  polyX/polyY: X & Y featureCoord of polygon clockwise or anticlockwise
	//  u/v			:x & y featureCoord of point to check
	//  The function will return YES if the point x,y is inside the polygon, or
	//  NO if it is not.  If the point is exactly on the edge of the polygon,
	//  then the function may return YES or NO.
	//
	//  Note that division by zero is avoided because the division is protected
	//  by the "if" clause which surrounds it.

	  int	j=n_corners-1 ;
	  bool	oddNodes=false;

	  for (unsigned int i=0; i<n_corners; i++)
	  {
	    if ( ((polyY[i]< v && polyY[j]>=v) || (polyY[j]< v && polyY[i]>=v)) &&  (polyX[i]<=u || polyX[j]<=u))
	    {
	      oddNodes^=(polyX[i]+(v-polyY[i])/(polyY[j]-polyY[i])*(polyX[j]-polyX[i])<u );
	    }
	    j=i;
	  }
	  return oddNodes;
}

static inline void loadBar(unsigned int x,unsigned int n,unsigned int w = 50)
{
	x+=1;
	if ( (x != n) && (x % (n/100+1) != 0) ) return;

	float ratio  =  x/(float)n;
	unsigned int   c      =  ratio * w;

	cout << setw(3) << (int)(ratio*100) << "% [";
	for (unsigned int x=0; x<c; x++) cout << "=";
	for (unsigned int x=c; x<w; x++) cout << " ";
	cout << "]\r" << flush;
}


inline vector<float> Kart_To_Sph(Vector3f v)
{
	vector<float> result(3); //[rho,phi,theta]

	result[0] = v.norm();			  //rho
	result[1] = atan2(v(1),v(0));	  //phi
	result[2] = acos(v(2)/result[0]); //theta;//acos(v(2)/result[0]);

	return result;
}

inline vector<float> Kart_To_Sph(vector<float> vec) //input: v=[x,y,z]
{
	Vector3f v(3);
	for(int i=0;i<3;i++)
	{
		v(0)=vec[0];
		v(1)=vec[1];
		v(2)=vec[2];
	}

	vector<float> result(3);

	result[0] = v.norm();			  //rho
	result[1] = atan2(v(1),v(0));	  //phi
	result[2] = acos(v(2)/result[0]); //theta

	return result; //output [rho,phi,theta]
}

inline vector<float> Sph_To_Kart(vector<float> v) //input: v=[rho,phi,theta]
{
	vector<float> result(3);

	result[0] = v[0]*sin(v[2])*cos(v[1]);	//x
	result[1] = v[0]*sin(v[2])*sin(v[1]);	//y
	result[2] = v[0]*cos(v[2]); 			//z

	return result; //output: result=[x,y,z]
}

