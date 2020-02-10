
/***************************************************************************************************************************************************
*
* Filename: Particle.h
*
* Description: Particle object - intended to demonstrate inheritance
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
#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec4.h"

//Particle class - intended to be inherited by specific particle types
//to be fair, this might be better served by just having a flag to denote different particles in most situations
class Particle{
	
  protected:
	//this doesn't really *need* to be protected, but it's not bad practice
	Vec4<double> position_; Vec4<double> momentum_;
	double mass_;
	int id_, status_;
	
  public:
	//default constructor
	Particle() {
		position_.t(0.); position_.x(0.); position_.y(0.); position_.z(0.);
		momentum_.e(0.); momentum_.px(0.); momentum_.py(0.); momentum_.pz(0.);
		mass_=0; id_=0; status_=0;
	}
	//constructor given the values as 4-vectors
	Particle(Vec4<double> pos_in, Vec4<double> mom_in, double m_in, int id_in, int stat_in) {
		position_=pos_in; momentum_=mom_in; mass_=m_in; id_=id_in; status_=stat_in;
	}
	//constructor given the values as 4-vectors
	Particle(double tpos_in, double xpos_in, double ypos_in, double zpos_in, double e_in, double px_in, double py_in, double pz_in,
	  double m_in, int id_in, int stat_in) {
		position_.t(tpos_in); position_.x(xpos_in); position_.y(ypos_in); position_.z(zpos_in);
		momentum_.e(e_in); momentum_.px(px_in); momentum_.py(py_in); momentum_.pz(pz_in);
		mass_=m_in; id_=id_in; status_=stat_in;
	}
	
	//getter functions
	double  x() {return position_.x(); } double  y() {return position_.y(); } double  z() {return position_.z(); } double t() {return position_.t();}
	double px() {return momentum_.px();} double py() {return momentum_.py();} double pz() {return momentum_.pz();} double e() {return momentum_.e();}
	double mass() {return mass_;} int id() {return id_;} int stat() {return status_;}
	Vec4<double>  pos() {return position_;}
	Vec4<double>    P() {return momentum_;}
	
	//setter functions
	void  x(double val) {position_.x(val); } void  y(double val) {position_.y(val); } void  z(double val) {position_.z(val); } void t(double val) {position_.t(val);}
	void px(double val) {momentum_.px(val);} void py(double val) {momentum_.py(val);} void pz(double val) {momentum_.pz(val);} void e(double val) {momentum_.e(val);}
	void mass(double val) {mass_ = val;} void id(int val) {id_ = val;} void stat(int val) {status_ = val;}
	void pos(Vec4<double> posin){position_ = posin;}
	void P(  Vec4<double> Pin)  {momentum_ = Pin;}
	
	//functions for derived values
	double pT2(){return momentum_.px()*momentum_.px() + momentum_.py()*momentum_.py();}
	double pMag2(){return momentum_.px()*momentum_.px() + momentum_.py()*momentum_.py() + momentum_.pz()*momentum_.pz();}
	double pDif2(Particle comp){return momentum_.dif2(comp.P());}
	double posDif2(Particle comp){return position_.dif2(comp.pos());}
};

#endif //PARTICLE_H
