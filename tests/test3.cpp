
/***************************************************************************************************************************************************
*
* Filename: test3.cpp
*
* Description: A test of the Nucleus class
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
#include "Nucleus.h"

std::mt19937_64 eng; //RNG - Mersenne Twist - 64 bit
double ran() {std::uniform_real_distribution<double> uniran(0.,1.); return uniran(eng);}

//returns true if the 2 given values are closer than the error bound given by the last value
bool is_close(double val1, double val2, double err){return (std::abs(val1 - val2) < err);}

int main(){
	//seeding the mt19937_64 object (RNG - Mersenne Twist - 64 bit) 'eng'
	std::random_device rd;
	std::array<int,std::mt19937_64::state_size> seedarray; std::generate_n(seedarray.data(), seedarray.size(), std::ref(rd));
	std::seed_seq seeds(std::begin(seedarray), std::end(seedarray)); eng.seed(seeds);
	
	//proton, neutron particle ids
	int id_p = 2212; int id_n = 2112;
	
	//creating values for invoking Nucleus objects
	int typeA=0; int npA=1; int nnA=0;
	int typeB=1; int npB=1; int nnB=1;
	int typeC=2; int npC=2+floor(ran()*100);  int nnC=2+floor(ran()*150);
	
	//declaring 3 nuclei with appropriate good settings (not necessarily physical, but this is just a test)
	//if these aren't set appropriately, then the executable will immediately fail with an error message
	Nucleus nucA(typeA, npA, nnA); Nucleus nucB(typeB, npB, nnB); Nucleus nucC(typeC, npC, nnC);
	
	//need to fill each nucleus before checking it to see if it has nucleons
	nucA.fill(); nucB.fill(); nucC.fill();
	
	//to make sure the nuclei are created appropriately, will check all the nucleons id's to ensure they're either protons (2212) or neutrons (2112)
	assert(nucA[0].id() == id_p);
	for(int i=0; i<2; ++i){assert( (nucB[i].id() == id_p) || (nucB[i].id() == id_n) );}
	for(int i=0; i<npC+nnC; ++i){assert( (nucC[i].id() == id_p) || (nucC[i].id() == id_n) );}
	
	//also, will check the sums to ensure that they're correct too!
	int npCheckB=0; int nnCheckB=0; int npCheckC=0; int nnCheckC=0;
	for(int i=0; i<2; ++i){ if(nucB[i].id() == id_p){++npCheckB;}  if(nucB[i].id() == id_n){++nnCheckB;} }
	for(int i=0; i<npC+nnC; ++i){ if(nucC[i].id() == id_p){++npCheckC;}  if(nucC[i].id() == id_n){++nnCheckC;} }
	assert(npCheckB == 1  ); assert(nnCheckB == 1  );
	assert(npCheckC == npC); assert(nnCheckC == nnC);
	
	//Success!
	std::cout << "\n\n SUCCESS: Test of Nucleus class passed.\n\n";
	
return 0;
}