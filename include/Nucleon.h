
/***************************************************************************************************************************************************
*
* Filename: Nucleon.h
*
* Description: Data for a single nucleon
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
#ifndef NUCLEON_H
#define NUCLEON_H

#include "Particle.h"

//Nucleon object inheriting from Particle object
//just demonstrating inheritance - would normally use only base particle class + flags to denote particle type & other data
class Nucleon : public Particle{
  protected:
	bool jet_state_; //just for demonstration, flag for if this nucleon could form a jet if it participates in a nucleon-nucleon collision
	
  public:
	//default constructor
	//initializing with position 0, momentum 0, E=935MeV, M=935Mev, id=0 (to be set to proton 2212, neutron 2112, with minus for antiparticle), status 0
	Nucleon() : Particle::Particle(0., 0., 0., 0., 0.935, 0., 0., 0., 0.935, 0, 0) {jet_state_ = false;}
	//nucleon given initial values as doubles/ints
	Nucleon(double tpos_in, double xpos_in, double ypos_in, double zpos_in, double e_in, double px_in, double py_in, double pz_in, double m_in,
	  int id_in, int stat_in) : Particle::Particle(tpos_in, xpos_in, ypos_in, zpos_in, e_in, px_in, py_in, pz_in, m_in, id_in, stat_in) {jet_state_ = false;}
	//nucleon given intial pos/mom as Vec4's
	Nucleon(Vec4<double> pos_in, Vec4<double> mom_in, double m_in, int id_in, int stat_in) : Particle::Particle(pos_in, mom_in, m_in, id_in, stat_in) {jet_state_ = false;}
	
	//setting or returning flag denoting if this nucleon could form a jet
	void jet(bool val); bool jet();
	
};
  
  
#endif //NUCLEON_H
