#ifndef TimeCompare_H
#define TimeCompare_H

#include <string>
#include <iostream>

#include "Tool.h"

using namespace std;

/**
 * \class TimeCompare
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
* Contact: b.richards@qmul.ac.uk
*/
class TimeCompare: public Tool {


 public:

  TimeCompare(); ///< Simple constructor
  bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
  bool Execute(); ///< Executre function used to perform Tool perpose. 
  bool Finalise(); ///< Finalise funciton used to clean up resorces.


 private:
    unsigned short ts_p1;
    unsigned short ts_p2;
    unsigned short ts_p3;
    unsigned short ts_p4;

    string Path;
    string storename;
    string entryname;
    int LAPPDID;


};


#endif
