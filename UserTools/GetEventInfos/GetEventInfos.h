#ifndef GetEventInfos_H
#define GetEventInfos_H

#include <string>
#include <iostream>
#include <map>
#include <vector>

#include "Tool.h"
#include "TTree.h"
#include "TFile.h"

using namespace std;
/**
 * \class GetEventInfos
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
* Contact: b.richards@qmul.ac.uk
*/
class GetEventInfos: public Tool {


 public:

  GetEventInfos(); ///< Simple constructor
  bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
  bool Execute(); ///< Executre function used to perform Tool perpose. 
  bool Finalise(); ///< Finalise funciton used to clean up resorces.


 private:

    string Path;
    string storename;
    string entryname;
    int LAPPDID;
    int Size;
    unsigned long long TimeStamp;
    unsigned long long TimeStamp_Inner0;
    unsigned long long TimeStamp_Inner1;
    int Counter_acdc0;
    int Counter_acdc1;
    vector<int> Index;
    vector<unsigned short> Waveform_acdc0;
    vector<unsigned short> Waveform_acdc1;

    TFile* EventRootFile;
    TTree *TTree_Event;
    TTree *TTree_AllEvents;


};


#endif
