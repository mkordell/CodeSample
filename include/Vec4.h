
/***************************************************************************************************************************************************
*
* Filename: Vec4.h
*
* Description: Physical 4-Vector Object
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
#ifndef VEC4_H
#define VEC4_H

//this is a template for a 4-vector
//for a physical 4-vector, could have just stuck with doubles.
template <class T>
class Vec4{
	
  protected:
	//this doesn't really *need* to be protected, but it's not bad practice
	T val_[4];  //could have used stl vector, but not really any need to do so
	
  public:
	//default constructor
	Vec4() {val_[0] = 0.; val_[1] = 0.; val_[2] = 0.; val_[3] = 0.;}
	//constructor given values
	Vec4(T tin, T xin, T yin, T zin) {val_[0] = tin; val_[1] = xin; val_[2] = yin; val_[3] = zin;}
	
	//getter functions
	T  x() {return val_[1];} T  y() {return val_[2];} T  z() {return val_[3];} T t() {return val_[0];}
	T px() {return val_[1];} T py() {return val_[2];} T pz() {return val_[3];} T e() {return val_[0];}
	T bx() {return val_[1];} T by() {return val_[2];} T bz() {return val_[3];} T g() {return val_[0];}
	
	//setter functions
	void  x(T val) {val_[1] = val;} void  y(T val) {val_[2] = val;} void  z(T val) {val_[3] = val;} void t(T val) {val_[0] = val;}
	void px(T val) {val_[1] = val;} void py(T val) {val_[2] = val;} void pz(T val) {val_[3] = val;} void e(T val) {val_[0] = val;}
	void bx(T val) {val_[1] = val;} void by(T val) {val_[2] = val;} void bz(T val) {val_[3] = val;} void g(T val) {val_[0] = val;}
	
	//Lorentz boost functions
	Vec4<T> boost(Vec4 B){
		Vec4<T> Pout;
		T xlam[4][4], beta2;
		beta2 = B.bx()*B.bx() + B.by()*B.by() + B.bz()*B.bz();
		
		T beta2inv;
		if(beta2 > 0.){beta2inv = 1./beta2;}
		else{beta2inv = 0.;}
		
		xlam[0][0] = B.g();
		xlam[0][1] = -B.g()*B.bx();
		xlam[0][2] = -B.g()* B.by();
		xlam[0][3] = -B.g()*B.bz();
		xlam[1][0] = xlam[0][1];
		xlam[1][1] = 1.+(B.g() - 1.)*(B.bx()*B.bx())*beta2inv;
		xlam[1][2] = (B.g()-1.)*B.bx()*B.by()*beta2inv;
		xlam[1][3] = (B.g()-1.)*B.bx()*B.bz()*beta2inv;
		xlam[2][0] = xlam[0][2];
		xlam[2][1] = xlam[1][2];
		xlam[2][2] = 1.+(B.g()-1.)*(B.by()*B.by())*beta2inv;
		xlam[2][3] = (B.g()-1.)*B.by()*B.bz()*beta2inv;
		xlam[3][0] = xlam[0][3];
		xlam[3][1] = xlam[1][3];
		xlam[3][2] = xlam[2][3];
		xlam[3][3] = 1.+(B.g()-1.)*(B.bz()*B.bz())*beta2inv;
		
		Pout.e ( val_[0]*xlam[0][0] + val_[1]*xlam[0][1] + val_[2]*xlam[0][2] + val_[3]*xlam[0][3] );
		Pout.px( val_[0]*xlam[1][0] + val_[1]*xlam[1][1] + val_[2]*xlam[1][2] + val_[3]*xlam[1][3] );
		Pout.py( val_[0]*xlam[2][0] + val_[1]*xlam[2][1] + val_[2]*xlam[2][2] + val_[3]*xlam[2][3] );
		Pout.pz( val_[0]*xlam[3][0] + val_[1]*xlam[3][1] + val_[2]*xlam[3][2] + val_[3]*xlam[3][3] );
		
		return Pout;
	}
	
	Vec4<T> boost(T vx, T vy, T vz){Vec4<T> vin(vx, vy, vz, 1./(sqrt(1. - (vx*vx + vy*vy + vz*vz)))); return this->boost(vin);}
	
	//diff^2 function
	T dif2(Vec4 comp){return (val_[1]-comp.x())*(val_[1]-comp.x()) + (val_[2]-comp.y())*(val_[2]-comp.y()) + (val_[3]-comp.z())*(val_[3]-comp.z());}
};

#endif //VEC4_H
