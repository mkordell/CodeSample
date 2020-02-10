
/***************************************************************************************************************************************************
*
* Filename: Event.h
*
* Description: Invokes nuclei, creates a single nucleus-nucleus collision, calculates collision statistics
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
#ifndef EVENT_H
#define EVENT_H

//includes
#include <random>
#include "Event.h"
#include "Nucleus.h"

//event class takes in nuclei settings and collides them; can report event collision statistics
class Event{
  protected:
	int num_coll_; int num_part_; double area_tot_; //members for event collision statistics
	int a_type_; int a_npro_; int a_nneu_; int b_type_; int b_npro_; int b_nneu_; //members for nuclei settings
	
	std::mt19937_64 eng; //RNG - Mersenne Twist - 64 bit
	double ran(); //throw a random double between 0 and 1
	double maxdist(Nucleus nuc_a, Nucleus nuc_b); //find the max distance between nucleons in 2 nuclei
	
	//constants
	const double pi=3.14159265358979; //const double e=2.71828182845904523;
	
  public:
	//need settings for nucleus a and nucleus b
	//type in denotes type of nucleus 0=single nucleon, 1=deuteron, 2=heavy
	//n_pro_in is the number of protons in the nucleus, n_neu_in is the same for neutrons
	Event(int a_type_in, int a_npro_in, int a_nneu_in, int b_type_in, int b_npro_in, int b_nneu_in){
		a_type_ = a_type_in; a_npro_ = a_npro_in; a_nneu_ = a_nneu_in; b_type_ = b_type_in; b_npro_ = b_npro_in; b_nneu_ = b_nneu_in;
	}
	//generate a single event by populating nuclei, colliding them, counting collision statistics
	void gen();
	//clear stored event
	void reset(){num_coll_ = 0; num_part_ = 0; area_tot_ = 0.;}
	//getters for event statistics
	int n_coll(){return num_coll_;} int n_part(){return num_part_;} double area(){return area_tot_;}
};

#endif //EVENT_H
