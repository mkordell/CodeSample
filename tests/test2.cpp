
/***************************************************************************************************************************************************
*
* Filename: test2.cpp
*
* Description: A test of the Particle class
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
#include "Particle.h"

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
	double x_vals[4]; double p_vals[4];
	for(int i=0; i<4; ++i){
		x_vals[i]=ran()*(10.*double(i)+double(i)); p_vals[i]=ran()*(10.*double(i)+double(i));
		if(ran() < 0.5){x_vals[i]*=-1.;} if(ran() < 0.5){p_vals[i]*=-1.;}
	}
	double val_m = ran()*1000.; int val_id = floor(5555*(ran()-0.5));
	
	//declaring a Particle with initial values to test various members
	Particle part(x_vals[0], x_vals[1], x_vals[2], x_vals[3], p_vals[0], p_vals[1], p_vals[2], p_vals[3], val_m, val_id, 0);
	
	//checking to ensure values were set appropriately (within some error bound)
	assert( is_close(part.t(), x_vals[0], error) );
	assert( is_close(part.x(), x_vals[1], error) );
	assert( is_close(part.y(), x_vals[2], error) );
	assert( is_close(part.z(), x_vals[3], error) );
	
	assert( is_close(part.e() , p_vals[0], error) );
	assert( is_close(part.px(), p_vals[1], error) );
	assert( is_close(part.py(), p_vals[2], error) );
	assert( is_close(part.pz(), p_vals[3], error) );
	
	assert( is_close(part.mass(), val_m, error) );
	assert( part.id() == val_id);
	
	//changing values in vec, then checking again
	//resampling new random numbers
	for(int i=0; i<4; ++i){
		x_vals[i]=ran()*(10.*double(i)+double(i)); p_vals[i]=ran()*(10.*double(i)+double(i));
		if(ran() < 0.5){x_vals[i]*=-1.;} if(ran() < 0.5){p_vals[i]*=-1.;}
	}
	val_m = ran()*1000.; val_id = floor(5555*(ran()-0.5));
	
	
	//setting new values
	part.t(x_vals[0]); part.x( x_vals[1]); part.y( x_vals[2]); part.z( x_vals[3]);
	part.e(p_vals[0]); part.px(p_vals[1]); part.py(p_vals[2]); part.pz(p_vals[3]);
	part.mass(val_m); part.id(val_id);
	
	//rechecking to ensure values were set appropriately (within some error bound)
	assert( is_close(part.t(), x_vals[0], error) );
	assert( is_close(part.x(), x_vals[1], error) );
	assert( is_close(part.y(), x_vals[2], error) );
	assert( is_close(part.z(), x_vals[3], error) );
	
	assert( is_close(part.e() , p_vals[0], error) );
	assert( is_close(part.px(), p_vals[1], error) );
	assert( is_close(part.py(), p_vals[2], error) );
	assert( is_close(part.pz(), p_vals[3], error) );
	
	assert( is_close(part.mass(), val_m, error) );
	assert( part.id() == val_id);
	
	//Success!
	std::cout << "\n\n SUCCESS: Test of Particle class passed.\n\n";
	
return 0;
}