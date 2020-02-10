
/***************************************************************************************************************************************************
*
* Filename: Nucleus.h
*
* Description: Creates a specific nucleus, then fills it with nucleons
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

//header guards
#ifndef NUCLEUS_H
#define NUCLEUS_H

#include <vector>
#include <random>
#include "Nucleon.h"

//Nucleus object, fills nucleus based on number of protons, neutrons, and type (heavy, deuteron, or single nucleon for demonstration)
class Nucleus{
  protected:
	std::vector<Nucleon> nucleons_; //the more straightforward way to deal with a number of nucleons
	int nuc_type_; //flag to denote the type of nucleus: 0=single nucleon, 1=deuteron, 2=heavy
	int n_pro_, n_neu_; //number of protons and neutrons in the nucleus
	
	std::mt19937_64 eng; //RNG - Mersenne Twist - 64 bit
	double ran(); //throw a random double between 0 and 1
	
	void single_nuc(); void deuteron();	void heavy(); //function to sample positions of the nucleons
	void center(); //put center of mass of nucleus at x=0,y=0,z=0
	double mindist(double x_in, double y_in, double z_in); //find the distance to the closest nucleon from x_in,y_in,z_in
	
	//constants
	const double pi=3.14159265358979; const double e=2.71828182845904523;
	
  public:
	Nucleus(int type_in, int npro_in, int nneu_in); //constructor; type in denotes type of nucleus, n_pro_in is the number of protons in the nucleus, n_neu_in is the same for neutrons
	void fill(); //fill the nucleus with nucleons w.r.t. settings
	
	//accessing the i'th nucleon
	Nucleon& operator[](int i) {return nucleons_[i];}
	const Nucleon& operator[](int i) const {return nucleons_[i];}
};

#endif //NUCLEUS_H
