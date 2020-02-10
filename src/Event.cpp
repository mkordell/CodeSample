
/***************************************************************************************************************************************************
*
* Filename: Event.cpp
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

//includes here
#include <vector>
#include <random>
#include "Event.h"
#include "Nucleus.h"
#include "Nucleon.h"

//return random double between 0 and 1
double Event::ran() {std::uniform_real_distribution<double> uniran(0.,1.); return uniran(eng);}

//generate a single event by populating nuclei, colliding them, counting collision statistics
void Event::gen(){
	
	//resetting event - clearing to ensure clean slate for new event
	reset();
	
	//declare nuclei
	Nucleus nuc_a(a_type_, a_npro_, a_nneu_); Nucleus nuc_b(b_type_, b_npro_, b_nneu_);
	
	//fill nuclei
	nuc_a.fill(); nuc_b.fill();
	
	//while loop to allow for resampling of collision geometries until a collision happens
	bool good_coll = false;
	while(!good_coll){
		//generate an impact parameter, is this a glancing blow or head-on?
		//sample r^2 from 0 to max_dist between any nucleon in A and any nucleon in B
		double r_max = maxdist(nuc_a, nuc_b) + 1.; //additional 1. fm to push to the very extreme edge of the furthest nucleons in the nuclei
		double r_min = 0.; //later can allow for this and/or above to be settings for centrality bin / impact parameter studies
		double r_samp = sqrt(r_max*r_max - (r_max*r_max - r_min*r_min)*ran());
		double th = ran()*2.*pi;
		
		//finding the offset for 2nd nucleus (arbitrary) for the collision
		double offset_x = r_samp*cos(th);
		double offset_y = r_samp*sin(th);
		
		//loop over nucleons: 1) count number of nucleon-nucleon collisions  2) set status flag for participants 3) sum up overlapping collision area
		//collision takes place in z-direction (collisions are in x-y plane with nuclei flattened along z-direction)
		int n_col = 0; int n_par = 0; double area = 0.; int coll_dist = 1.;
		for(int inuc_a=0; inuc_a<a_npro_+a_nneu_; inuc_a++){
			for(int inuc_b=0; inuc_b<b_npro_+b_nneu_; inuc_b++){
				double dist2 = (nuc_a[inuc_a].x() - nuc_a[inuc_b].x() - offset_x)*(nuc_a[inuc_a].x() - nuc_a[inuc_b].x() - offset_x) +
				  (nuc_a[inuc_a].y() - nuc_a[inuc_b].y() - offset_y)*(nuc_a[inuc_a].y() - nuc_a[inuc_b].y() - offset_y);
				if(dist2<=coll_dist*coll_dist){
					++n_col; nuc_a[inuc_a].stat(1); nuc_a[inuc_b].stat(1);
					double dist = sqrt(dist2);
					area += 0.5*dist*sqrt(4.*coll_dist*coll_dist - dist*dist); //assuming nucleons are all the same size
				}
			}
		}
		
		//second loop, loop over each nucleus and count number of collision participants
		for(int inuc_a=0; inuc_a<a_npro_+a_nneu_; inuc_a++){if(nuc_a[inuc_a].stat() == 1){++n_par;}}
		for(int inuc_b=0; inuc_b<b_npro_+b_nneu_; inuc_b++){if(nuc_b[inuc_b].stat() == 1){++n_par;}}
		
		if(n_col > 0){num_coll_ = n_col; num_part_ = n_par; area_tot_ = area; good_coll=true;}
	}
}

//find the max distance between any nucleon in one nucleus and any nucleon in another nucleus in x-y
double Event::maxdist(Nucleus nuc_a, Nucleus nuc_b){
	double dist_out = 0.;
	for(int inuc_a=0; inuc_a<a_npro_+a_nneu_; inuc_a++){
		for(int inuc_b=0; inuc_b<b_npro_+b_nneu_; inuc_b++){
			double dist = (nuc_a[inuc_a].x() - nuc_b[inuc_b].x())*(nuc_a[inuc_a].x() - nuc_b[inuc_b].x()) +
			  (nuc_a[inuc_a].y() - nuc_b[inuc_b].y())*(nuc_a[inuc_a].y() - nuc_b[inuc_b].y());
			if(dist>dist_out){dist_out = dist;}
		}
	}
	
return std::sqrt(dist_out);
}