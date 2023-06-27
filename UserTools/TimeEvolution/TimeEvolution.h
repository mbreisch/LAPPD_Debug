#ifndef TimeEvolution_H
#define TimeEvolution_H

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "Tool.h"
#include "TTree.h"

/**
 * \class TimeEvolution
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
* Contact: b.richards@qmul.ac.uk
*/
class TimeEvolution: public Tool {


 public:

    TimeEvolution(); ///< Simple constructor
    bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
    bool Execute(); ///< Executre function used to perform Tool perpose. 
    bool Finalise(); ///< Finalise funciton used to clean up resorces.

    void GetTimeEvolution(std::map<int, PsecData> tmpMap, int size);
    void GetFullTimeEvolution(std::map<int, PsecData> tmpMap);

 private:

    string storename;
    string entryname;
    int LAPPDID;

    unsigned long long full_ts;
    unsigned long long full_bts;
    unsigned long long full_dt;
    unsigned long long full_pps;
    unsigned long long previous_pps_ts;
    unsigned long long previous_evo_point;
    float ppsdt;
    float datadt;
    float tevo;
    long Size;



};


#endif
