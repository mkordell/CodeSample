
/***************************************************************************************************************************************************
*
* Filename: test1.cpp
*
* Description: A test of the Vec4 template
*
* Copyright (c) 2020, Michael Kordell II
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
*    software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
***************************************************************************************************************************************************/

//includes
#include <assert.h>
#include <iostream>
#include <array>
#include <algorithm>
#include <functional>
#include <random>
#include "Vec4.h"

std::mt19937_64 eng; //RNG - Mersenne Twist - 64 bit
double ran() {std::uniform_real_distribution<double> uniran(0.,1.); return uniran(eng);}

//returns true if the 2 given values are closer than the error bound given by the last value
bool is_close(double val1, double val2, double err){return (std::abs(val1 - val2) < err);}

int main(){
	//seeding the mt19937_64 object (RNG - Mersenne Twist - 64 bit) 'eng'
	std::random_device rd;
	std::array<int,std::mt19937_64::state_size> seedarray; std::generate_n(seedarray.data(), seedarray.size(), std::ref(rd));
	std::seed_seq seeds(std::begin(seedarray), std::end(seedarray)); eng.seed(seeds);
	
	//maximum allowable error
	double error = 0.00001;
	
	//creating some non-overlapping random values
	double val0(10.*ran()), val1(10.*ran()+11.), val2(10.*ran()+22.), val3(10.*ran()+33.);
	if(ran() < 0.5){val0*=-1.;}  if(ran() < 0.5){val1*=-1.;}  if(ran() < 0.5){val2*=-1.;}  if(ran() < 0.5){val3*=-1.;}
	
	//declaring a Vec4<double> with initial values to test various members
	Vec4<double> vec(val0, val1, val2, val3);
	
	//checking to ensure values were set appropriately (within some error bound)
	assert( is_close(vec.t(), val0, error) );
	assert( is_close(vec.x(), val1, error) );
	assert( is_close(vec.y(), val2, error) );
	assert( is_close(vec.z(), val3, error) );
	
	//changing values in vec, then checking again
	//resampling new random numbers
	val0=10.*ran(); val1=10.*ran()+11.; val2=10.*ran()+22.; val3=10.*ran()+33.;
	if(ran() < 0.5){val0*=-1;}  if(ran() < 0.5){val1*=-1;}  if(ran() < 0.5){val2*=-1;}  if(ran() < 0.5){val3*=-1;}
	
	//setting new values
	vec.t(val0); vec.x(val1); vec.y(val2); vec.z(val3);
	
	//rechecking to ensure values were set appropriately (within some error bound)
	assert( is_close(vec.t(), val0, error) );
	assert( is_close(vec.x(), val1, error) );
	assert( is_close(vec.y(), val2, error) );
	assert( is_close(vec.z(), val3, error) );
	
	//Success!
	std::cout << "\n\n SUCCESS: Test of Vec4 class passed.\n\n";
	
return 0;
}