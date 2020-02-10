
/***************************************************************************************************************************************************
*
* Filename: Histogram.h
*
* Description: A histogram template used for statistical analysis
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
#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <cmath>

//there are other available histogram available in various libraries (GSL, boost, ROOT...)
//demonstrating a 1D histogram template that should handle any object with >= and < operators defined
//does not support weighted entries
template <class T>
class Histogram{
	
  protected:
	//better in practice to use stl vector for below, but just a demonstration
	//these probably should be protected as they really shouldn't be directly modified by anything outside the class
	T* binends_; int* hist_; T* stddev_; T* mean_; T* dev_mean_;
	int n_bins_;
	
	//to find a running mean, std. deviation, and deviation of mean
	void runstat (T val, int binnum){
		if (hist_[binnum] == 1) {mean_[binnum] = val; stddev_[binnum] = 0.;}
		if (hist_[binnum] != 1) {
			T new_mean = mean_[binnum] + (val - mean_[binnum])/hist_[binnum];
			T new_stddev = stddev_[binnum] + (val - mean_[binnum])*(val - new_mean);
			mean_[binnum] = new_mean; stddev_[binnum] = new_stddev;
		}
	}
	
  public:
	Histogram(T bins_in[], int n_bins){
		
		n_bins_ = n_bins; //keeping track of how many bins needed
		binends_ = new T[n_bins+1]; //since this stores the endpoints of bins, need one more (fencepost)
		//reserving memory
		hist_ = new int[n_bins]; stddev_ = new T[n_bins]; mean_ = new T[n_bins]; dev_mean_ = new T[n_bins];
		
		//setting initial values for the stored bin ends, and the initial values for the histogram + other stat data to 0
		for(int ibin=0; ibin<=n_bins; ++ibin){binends_[ibin]=bins_in[ibin];}
		for(int ibin=0; ibin<n_bins; ++ibin){hist_[ibin]=0; stddev_[ibin]=T(0.); mean_[ibin]=T(0.); dev_mean_[ibin]=T(0.);}
	}
	
	//destructor needs to be explicitly declared, since memory is being manually managed
	//again, using stl vector would alleviate the necessity for this, but is just a demonstration
	~Histogram(){
		delete [] binends_; delete [] hist_; delete [] stddev_; delete [] mean_; delete [] dev_mean_;
		binends_=nullptr; hist_=nullptr; stddev_=nullptr; mean_=nullptr; dev_mean_=nullptr;
	}
	
	//binning the given value into the histogram
	void fill(T val_in){
		int ihist = 0;
		while ((val_in>=binends_[ihist+1]) && (ihist<=n_bins_+1)){ihist++;}
		if (val_in<binends_[0]){ihist=-1;}
		if ((ihist<=n_bins_)&&(ihist>=0)){
			hist_[ihist]++;
			runstat(val_in, ihist);
		}
	}
	
	//CAUTION, there are no bounds checking for any of the below; if this was a proper library then it may be a good idea to add checks
	//return lower and upper bounds for the i'th bin
	T bin_low(int i){return binends_[i];} T bin_high(int i){return binends_[i+1];}
	//return the number of bins
	int n_bins(){return n_bins_;}
	//return the number of entries in bin i, return the uncertainty in this number (assuming poisson fill)
	int val_bin(int i){return hist_[i];} double errval_bin(int i){return std::sqrt(hist_[i]);}
	//return the x-average of bin i
	T mean_bin(int i){return mean_[i];}
	//return the standard_deviation of the x-values in i'th bin
	T stddev_bin(int i){if(hist_[i]>1){return pow(stddev_[i]/(hist_[i]-1.), 0.5);} else{return (binends_[i+1]-binends_[i])/2.;}}
	//return  the uncertainty of the mean in bin i
	T errmean_bin(int i){if(hist_[i]>1){return pow(stddev_[i]/(hist_[i]-1.), 0.5)/(pow(hist_[i], 0.5));} else{return (binends_[i+1]-binends_[i])/2.;}}
	
};

#endif //HISTOGRAM_H
