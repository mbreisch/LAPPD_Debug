#ifndef ReadFile_H
#define ReadFile_H

#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "Tool.h"



using namespace std;

/**
 * \class ReadFile
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
* Contact: b.richards@qmul.ac.uk
*/
class ReadFile: public Tool {


 public:

    ReadFile(); ///< Simple constructor
    bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
    bool Execute(); ///< Executre function used to perform Tool perpose. 
    bool Finalise(); ///< Finalise funciton used to clean up resorces.


 private:

    string Path;
    string Path_Out;
    string File;
    string Prefix;
    string FileStore0;
    string FileStore1;
    string FileStore2;

    string choice;
    int RunNumber;
    int PartNumber;
    int counter_of_doom;

};


#endif
