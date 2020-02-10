
/***************************************************************************************************************************************************
*
* Filename: Nucleus.cpp
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

//includes here
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <functional>
#include "Nucleus.h"
#include "Nucleon.h"

//return random double between 0 and 1
double Nucleus::ran() {std::uniform_real_distribution<double> uniran(0.,1.); return uniran(eng);}

//constructor; type in denotes type of nucleus, n_pro_in is the number of protons in the nucleus, n_neu_in is the same for neutrons
Nucleus::Nucleus(int type_in, int npro_in, int nneu_in){
		nuc_type_ = type_in; n_pro_ = npro_in; n_neu_ = nneu_in;
		//error catches
		if((nuc_type_ < 0 || nuc_type_ > 2) || (n_pro_ < 0) || (n_neu_ < 0)){ //catch for non-valid values
			std::cout << "\n\nNucleus was initialized with bad settings, please check given values.\n\n";
			exit(EXIT_FAILURE);
		}
		if((nuc_type_ == 0) && (n_pro_ + n_neu_ != 1)){ //catch for bad single nucleon setting
			std::cout << "\n\nA single nucleon nucleus was initialized with something other than a single nucleon.\n\n";
			exit(EXIT_FAILURE);
		}
		if((nuc_type_ == 1) && (n_pro_ != 1 || n_neu_ != 1)){ //catch for bad deuteron setting
			std::cout << "\n\nA deuteron was initialized with something other than a proton & neutron.\n\n";
			exit(EXIT_FAILURE);
		}
		if((nuc_type_ == 2) && (n_pro_ + n_neu_ < 3)){ //catch for *some* bad heavy nucleus settings
			std::cout << "\n\nA heavy nucleus was initialized with too few nucleons.\n\n";
			exit(EXIT_FAILURE);
		}
		
		//seeding the mt19937_64 object (RNG - Mersenne Twist - 64 bit) 'eng' PROPERLY!
		std::random_device rd;
		std::array<int,std::mt19937_64::state_size> seedarray; std::generate_n(seedarray.data(), seedarray.size(), std::ref(rd));
		std::seed_seq seeds(std::begin(seedarray), std::end(seedarray)); eng.seed(seeds);
}

//filling the nucleus with nucleons
//assuming that the number of protons, neutrons, and nucleus type have been set correctly(forced in constructor)
void Nucleus::fill(){
	//based on nucleus type, call a different nucleus sampler
	if(nuc_type_ == 0){
		single_nuc();
	}
	else if(nuc_type_ == 1){
		deuteron();
	}
	else{ //if(nuc_type_ == 2)
		heavy();
	}
	
	//now need to determine which nucleons are protons and which are neutrons
	int set_pro = 0; int set_neu = 0;
	for(int inuc=0; inuc<nucleons_.size(); ++inuc){
		double prob_pro = double(n_pro_ - set_pro)/double(n_pro_ + n_neu_ - set_pro - set_neu);
		if(prob_pro > ran()){nucleons_[inuc].id(2212); ++set_pro;}
		else{nucleons_[inuc].id(2112); ++set_neu;}
	}
}

//create a single nucleon in the nucleus list
void Nucleus::single_nuc(){nucleons_.push_back(Nucleon());}

//create a deuteron - a single proton + single neutron
void Nucleus::deuteron(){
	//deuteron parameters - fixed "magic numbers"
	double Rd = 3.*7.3; //max radius sampled, not a critical parameter for deuteron
	double ha = 0.228; //Hulthen parameter alpha
	double hb = 1.18; //Hulthen parameter beta
	double close = 1.; //closest distance nucleons can be in deuteron

	//choosing spacial position
	while (nucleons_.size() < 2){
		double r = Rd * (pow(ran(),(double)(1./3.))); //sampling a radius uniformly inside of a sphere
		double th = acos(2.*ran() - 1.); //sampling spherical angle theta
		double ph = ran()*2.*pi; //sampling spherical angle phi
		double x_val = r * sin(th) * cos(ph);
		double y_val = r * sin(th) * sin(ph);
		double z_val = r * cos(th);
		double Psi = (pow(r,-2.))*(pow((pow(e,-1.*ha*r)) - (pow(e,-1.*hb*r)),2.)); //sampling Hulthen probability distance
		double k = 0.97*ran(); //sample from 0 - Psi(max)
		
		if(Psi < k){continue;} //chosen point fails likelihood check
		if((nucleons_.size()>0) && (mindist(x_val, y_val, z_val) < close)){continue;} //chosen point too close to other nucleon
		
		nucleons_.push_back(Nucleon(0., x_val, y_val, z_val, 0.935, 0., 0., 0., 0.935, 0, 0));
	}
	
	center();
}

//sample positions of nucleons based on Woods-Saxon nucleus
void Nucleus::heavy(){
	//Woods-Saxon parameters
	const double WSR = 1.25*pow(n_pro_ + n_neu_,(1./3.)); //Woods-Saxon parameter R parametrized based number of nucleons
	const double WSa = 0.535; //Woods-Saxon parameter a
	const double RA = 3.*WSR; //max radius sampled, not a critical parameter for deuteron
	double close = 1.; //closest distance nucleons can be in nucleus
	
	while (nucleons_.size() < n_pro_ + n_neu_){
		double r = (5.00/3.00) * RA * (pow(ran(),(double)(1.0/3.0)));  //sampling a radius uniformly inside of a sphere
		double th = acos(2.0*ran() - 1.0); //sampling spherical angle theta
		double ph = ran()*2.0*pi; //sampling spherical angle phi
		double x_val = r * sin(th) * cos(ph);
		double y_val = r * sin(th) * sin(ph);
		double z_val = r * cos(th);
		double Psi = 1.00/(1.00 + pow(e, (r - WSR)/WSa)); //sampling Woods-Saxon probability distance
		double k = ran(); //sample from 0 - Psi(max)
		
		if(Psi < k){continue;} //chosen point fails likelihood check
		if((nucleons_.size()>0) && (mindist(x_val, y_val, z_val) < close)){continue;} //chosen point too close to other nucleon
		
		nucleons_.push_back(Nucleon(0., x_val, y_val, z_val, 0.935, 0., 0., 0., 0.935, 0, 0));
	}
	
	center();
}

//put center of mass of nucleus at x=0,y=0,z=0
void Nucleus::center(){
	//initializing CM position
	double cm_pos[3] = {0., 0., 0.};
	
	//finding the CM
	for(int inuc=0; inuc<nucleons_.size(); inuc++){cm_pos[0] += nucleons_[inuc].x(); cm_pos[1] += nucleons_[inuc].y(); cm_pos[2] += nucleons_[inuc].z();}
	
	//shifting all nucleons s.t. CM is at 0,0,0
	for(int inuc=0; inuc<nucleons_.size(); inuc++){
		nucleons_[inuc].x(nucleons_[inuc].x() - cm_pos[0]/double(nucleons_.size()));
		nucleons_[inuc].y(nucleons_[inuc].y() - cm_pos[1]/double(nucleons_.size()));
		nucleons_[inuc].z(nucleons_[inuc].z() - cm_pos[2]/double(nucleons_.size()));
	}
}

//find the distance to the closest nucleon from x_in,y_in,z_in
double Nucleus::mindist(double x_in, double y_in, double z_in){
	double dist_out = 9.e100;
	for(int inuc=0; inuc<nucleons_.size(); inuc++){
		double dist = (x_in - nucleons_[inuc].x())*(x_in - nucleons_[inuc].x()) +
		  (y_in - nucleons_[inuc].y())*(y_in - nucleons_[inuc].y()) + (z_in - nucleons_[inuc].z())*(z_in - nucleons_[inuc].z());
		if(dist<dist_out){dist_out = dist;}
	}
	
return std::sqrt(dist_out);
}
