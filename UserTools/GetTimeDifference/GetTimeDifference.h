#ifndef GetTimeDifference_H
#define GetTimeDifference_H

#include <string>
#include <iostream>

#include <string>
#include <iostream>
#include <map>
#include <vector>

#include "Tool.h"
#include "TTree.h"
#include "TFile.h"

using namespace std;
/**
 * \class GetTimeDifference
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
* Contact: b.richards@qmul.ac.uk
*/
class GetTimeDifference: public Tool {


 public:

  GetTimeDifference(); ///< Simple constructor
  bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
  bool Execute(); ///< Executre function used to perform Tool perpose. 
  bool Finalise(); ///< Finalise funciton used to clean up resorces.


 private:

    string Path;
    string storename;
    string entryname;
    int LAPPDID;
    int Size;

    unsigned long long PPS_RPi_dt;
    unsigned long long full_pps;
    unsigned long long full_rpi;


};


#endif