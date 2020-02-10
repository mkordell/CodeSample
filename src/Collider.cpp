
/***************************************************************************************************************************************************
*
* Filename: Collider.cpp
*
* Description: Loop over generated events, settings read-in and statistical output
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
#include <fstream>
#include <sstream>
#include <ctime>
#include "Event.h"
#include "Histogram.h"

//Return predicted running time
double tpred(const int n, const int nmax, const double tst) {return floor(((double)(clock() - tst)/CLOCKS_PER_SEC)*((double)(nmax)/((double)(n)) - 1.)*(1./60.) + 0.5);}

//Return current run time
double trun(const double tst) {return floor(((double)(clock() - tst)/CLOCKS_PER_SEC)*(1./60.) + 0.5);}

int main(int argc, char* argv[]){
	
	//declaring vars to use
	int nuctypea, nuctypeb, num_pro_a, num_pro_b, num_neu_a, num_neu_b, n_eve;
	std::string binfile_n; std::string binfile_a; std::string settingfile; std::string outfile;
	double* binarrayN; double* binarrayA;
	
	//default values
	nuctypea  = 2  ; //heavy nucleus a
	nuctypeb  = 2  ; //heavy nucleus b
	num_pro_a = 82 ; //number of protons in a lead 208 nucleus
	num_pro_b = 82 ; //number of protons in a lead 208 nucleus
	num_neu_a = 126; //number of neutrons in a lead 208 nucleus
	num_neu_b = 126; //number of neutrons in a lead 208 nucleus
	n_eve     = 1000; //default number of events is 10k
	
	binfile_n   = "settings/binfile_n.dat";
	binfile_a   = "settings/binfile_a.dat";
	settingfile = "settings/settings.dat";
	outfile     = "output/output.dat";

	//reading command line arguments
	std::string argument = ""; int nflags = 11; bool setflag[nflags]; for(int iflags=0; iflags<nflags; ++iflags){setflag[iflags]=false;}
	if(argc > 1){argument = argv[1];}
	//listing out command line arguments, with available switches
	if(argc%2 != 1 || argument == "-h" || argument == "-H" || argument == "-help" || argument == "-Help" || argument == "-HELP"){
		std::cout << " Usage for command line arguments:\n";
		std::cout << " Give the switch as an argument, followed by the setting for that switch\n";
		std::cout << " Ex.: ./collider.exe -nucA 0 -nucB 2 -nproA 1 -nproB 79 -nneuA 0 -nneuB 118\n";
		std::cout << " Will run a p+A event set, reading from the default settings/binfile.dat and settings/settings.dat\n\n";
		std::cout << " Available switches are:\n";
		std::cout << " Switch: '-NumE' to set the total number of events that will be generated.\n";
		std::cout << " Switch: '-nucA' to set the type of nucleus for nucleus A (0=single nucleon, 1=deuteron, 2=heavy).\n";
		std::cout << " Switch: '-nucB' to set the type of nucleus for nucleus B (0=single nucleon, 1=deuteron, 2=heavy).\n";
		std::cout << "      If either of these are explicitly set, then npro* and nneu* should also be appropriately changed.";
		std::cout << " Switch: '-nproA' to set the number of protons in nucleus A.\n";
		std::cout << " Switch: '-nproB' to set the number of protons in nucleus B.\n";
		std::cout << " Switch: '-nneuA' to set the number of neutrons in nucleus A.\n";
		std::cout << " Switch: '-nneuB' to set the number of neutrons in nucleus B.\n";
		std::cout << " Switch: '-binfilen' to change the name of the file with binends used to histogram n_coll and n_part over the events.  " << 
		  "Default: 'settings/binfile_n.dat'\n";
		std::cout << " Switch: '-binfilea' to change the name of the file with binends used to histogram nucleon-nucleon overlap area over the events.  " <<
		  "Default: 'settings/binfile_a.dat'\n";
		std::cout << " Switch: '-setfile' to change the name of the file where settings can be read in from. Default: 'settings/settings.dat'\n";
		std::cout << " Switch: '-outfile' to change the name of the file where the output histograms are written to. Default: 'settings/output.dat'\n";
		std::cout << " Notes:\n";
		std::cout << " Any parameters set here will overwrite any defaults or settings in the code proper, or those read from a settings file.\n";
		std::cout << " There are no explicit catches for bad values; some may catch, but expect undefined behaviour.\n\n";
		return 0;
	}
	//looping over all command line arguments to set variables
	else{
		for (int i=1; i<argc; i+=2){
			argument = argv[i];
			if(     argument == "-NumE"    ){n_eve       = std::stoi(argv[i+1]); setflag[0]  = true;}
			else if(argument == "-nucA"    ){nuctypea    = std::stoi(argv[i+1]); setflag[1]  = true;}
			else if(argument == "-nucB"    ){nuctypeb    = std::stoi(argv[i+1]); setflag[2]  = true;}
			else if(argument == "-nproA"   ){num_pro_a   = std::stoi(argv[i+1]); setflag[3]  = true;}
			else if(argument == "-nproB"   ){num_pro_b   = std::stoi(argv[i+1]); setflag[4]  = true;}
			else if(argument == "-nneuA"   ){num_neu_a   = std::stoi(argv[i+1]); setflag[5]  = true;}
			else if(argument == "-nneuB"   ){num_neu_b   = std::stoi(argv[i+1]); setflag[6]  = true;}
			else if(argument == "-binfilen"){binfile_n   = argv[i+1];            setflag[7]  = true;}
			else if(argument == "-binfilea"){binfile_a   = argv[i+1];            setflag[8]  = true;}
			else if(argument == "-setfile" ){settingfile = argv[i+1];            setflag[9]  = true;}
			else if(argument == "-outfile" ){outfile     = argv[i+1];            setflag[10] = true;}
			else{std::cout << " Switch " << argument << " was not recognized.\nFor help, run with -h switch.\nPress enter to continue running.\n"; std::cin >> argument;}
		}
	}
	
	//need to read-in and parse settings file.  Then overwrite default values with values there, but ONLY if it wasn't already overridden on command line
	std::ifstream settings (settingfile.c_str());
	std::stringstream argstream;
	while(std::getline(settings, argument)){
		//lines beginning with # denote comments
		if((argument.front() == '#') || (argument.empty())){continue;}
		
		//parsing line into 2 strings
		argstream << argument;
		std::string str1; std::string str2;
		argstream << str1 << str2;
		
		//if first string matches an argument, set the appropriate value to the second string IFF it wasnt' set on command line
		if(     str1 == "NumE"     && !setflag[0] ){n_eve       = std::stoi(str2);}
		else if(str1 == "nucA"     && !setflag[1] ){nuctypea    = std::stoi(str2);}
		else if(str1 == "nucB"     && !setflag[2] ){nuctypeb    = std::stoi(str2);}
		else if(str1 == "nproA"    && !setflag[3] ){num_pro_a   = std::stoi(str2);}
		else if(str1 == "nproB"    && !setflag[4] ){num_pro_b   = std::stoi(str2);}
		else if(str1 == "nneuA"    && !setflag[5] ){num_neu_a   = std::stoi(str2);}
		else if(str1 == "nneuB"    && !setflag[6] ){num_neu_b   = std::stoi(str2);}
		else if(str1 == "binfilen" && !setflag[7] ){binfile_n   = str2;           }
		else if(str1 == "binfilea" && !setflag[8] ){binfile_a   = str2;           }
		else if(str1 == "outfile"  && !setflag[10]){outfile     = str2;           }
	}
	
	//reporting current settings
	std::string nA = "A"; std::string nB = "A";
	if(     nuctypea == 0){nA="p";}
	else if(nuctypea == 1){nA="d";}
	
	if(     nuctypeb == 0){nB="p";}
	else if(nuctypeb == 1){nB="d";}
	
	std::cout << "\n\n";
	std::cout << "Running " << n_eve << " " << nA << "+" << nB << " Events (" << num_pro_a << " protons and " << num_neu_a <<
	  " neutrons against " << num_pro_b << " protons and " << num_neu_b << " neutrons" << ").\n";
	std::cout << "Bin ends for collision statistics are :" << binfile_n << " and " << binfile_a << "\n";
	std::cout << "Output written to file: " << outfile << "\n";
	std::cout << "\n\n";
	
	//setting up histograms
	//first, need to read in binfiles
	//determining the number of bins from file
	std::ifstream binningN(binfile_n.c_str()); std::ifstream binningA(binfile_a.c_str()); std::string line;
	int nbinsN = 0; int nbinsA = 0;
	while(std::getline(binningN, line)){nbinsN++;} while(std::getline(binningA, line)){nbinsA++;}
	binningN.close(); binningN.clear(); binningA.close(); binningA.clear();
	
	//Allocating binarray
	binarrayN = new double[nbinsN]; binarrayA = new double[nbinsA];
	
	//reading into binarray with bin values from file
	nbinsN = 0; nbinsA = 0; double binval = 0.;
	binningN.open(binfile_n.c_str()); binningA.open(binfile_a.c_str());
	while(binningN >> binval){binarrayN[nbinsN]=binval; nbinsN++;} while(binningA >> binval){binarrayA[nbinsA]=binval; nbinsA++;}
	binningN.close(); binningA.close();
	
	//declaring histograms
	//Using double histograms for the double ones because I want double binends to make the bin centers fall exactly on integer values
	Histogram<double> h_n_coll(binarrayN, nbinsN-1); Histogram<double> h_n_part(binarrayN, nbinsN-1); Histogram<double> h_area(binarrayA, nbinsA-1);
	
	//declaring event object
	Event event(nuctypea, num_pro_a, num_neu_a, nuctypeb, num_pro_b, num_neu_b);
	
	//event loop
	clock_t tstart = clock();
	for(int i_eve=0; i_eve<n_eve; ++i_eve){
		event.gen(); //generating a single event
		h_n_coll.fill(event.n_coll()); h_n_part.fill(event.n_part()); h_area.fill(event.area()); //filling histograms with statistical info.
		
		//keeping track of progress and time; estimating time remaining; reporting every 1000 events
		if(i_eve%100==0){
			std::cout << "  " << i_eve << " out of " << n_eve << " Events generated   " << ((double)i_eve/n_eve)*100. << "% finished \n";
			std::cout << "  Est. time remaining: " << tpred(i_eve, n_eve, tstart) << " minutes" << " (" << trun(tstart) << " elapsed)" << "\n";
			std::cout << "  Avg. time per event: " << ((double)(clock() - tstart)/CLOCKS_PER_SEC)/i_eve << " seconds\n";
			std::cout << "  Avg. # events / sec: " << i_eve/((double)(clock() - tstart)/CLOCKS_PER_SEC) << "\n\n";
		}
	}
	
	//Event loop completion message
	std::cout << "All requested events have been generated.  Writing out statistics and closing.\n\n";
	std::cout << "\nTime taken was " << ((double)(clock() - tstart)/CLOCKS_PER_SEC)/60. << " minutes \n";
	std::cout << "Average time per event was " << ((double)(clock() - tstart)/CLOCKS_PER_SEC)/n_eve << " seconds \n";
	std::cout << "Avg. # events / sec: " << n_eve/((double)(clock() - tstart)/CLOCKS_PER_SEC) << "\n";
	
	//opening up output file to write histograms to
	std::ofstream fileout(outfile.c_str());
	
	//writing to file
	fileout << "\n";
	fileout << "N_coll Histogram:";
	fileout << "N_coll, Entries";
	for(int ihist=0; ihist<h_n_coll.n_bins(); ++ihist){
		fileout << h_n_coll.mean_bin(ihist) << ", " << h_n_coll.val_bin(ihist) << "\n";
	}
	fileout << "\n\n\n\n\n\n\n\n\n\n";
	fileout << "N_part Histogram:";
	fileout << "N_part, Entries";
	for(int ihist=0; ihist<h_n_part.n_bins(); ++ihist){
		fileout << h_n_part.mean_bin(ihist) << ", " << h_n_part.val_bin(ihist) << "\n";
	}
	fileout << "\n\n\n\n\n\n\n\n\n\n";
	fileout << "Area Histogram:";
	fileout << "bin_AvgArea, Entries";
	for(int ihist=0; ihist<h_area.n_bins(); ++ihist){
		fileout << h_area.mean_bin(ihist) << ", " << h_area.val_bin(ihist) << "\n";
	}
	fileout.close();
	
	
	//releasing memory
	delete [] binarrayN; delete [] binarrayA;
	
return 0;
}