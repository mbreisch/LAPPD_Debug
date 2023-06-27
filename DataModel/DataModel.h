#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <map>
#include <string>
#include <vector>

#include "TTree.h"

#include "Store.h"
#include "BoostStore.h"
#include "Logging.h"
#include "Utilities.h"

#include <zmq.hpp>

#include <PsecData.h>

#include "TFile.h"

using namespace std;
/**
* \class DataModel
 *
 * This class Is a transient data model class for your Tools within the ToolChain. If Tools need to comunicate they pass all data objects through the data model. There fore inter tool data objects should be deffined in this class.
 *
 *
 * $Author: B.Richards $
 * $Date: 2019/05/26 18:34:00 $
 * Contact: b.richards@qmul.ac.uk
 *          
 */

class DataModel {


 public:
  
    DataModel(); ///< Simple constructor

    //TTree* GetTTree(std::string name);
    //void AddTTree(std::string name,TTree *tree);
    //void DeleteTTree(std::string name);

    Store vars; ///< This Store can be used for any variables. It is an inefficent ascii based storage    
    BoostStore CStore; ///< This is a more efficent binary BoostStore that can be used to store a dynamic set of inter Tool variables.
    std::map<std::string,BoostStore*> Stores; ///< This is a map of named BooStore pointers which can be deffined to hold a nammed collection of any tipe of BoostStore. It is usefull to store data that needs subdividing into differnt stores.
    
    Logging *Log; ///< Log class pointer for use in Tools, it can be used to send messages which can have multiple error levels and destination end points  

    zmq::context_t* context; ///< ZMQ contex used for producing zmq sockets for inter thread,  process, or computer communication

    //SC
    PsecData GlobalPsec;

    map<int,PsecData> PartFileMap;
    map<int,PsecData> RAWLAPPD0;
    map<int,PsecData> RAWLAPPD1;
    map<int,PsecData> RAWLAPPD2;

    std::string Path;
    std::string Path_Out;
    std::string Prefix;
    int RunNumber;
    bool SwitchToEval;

    TFile* RootFile;
    TTree *TTree_TimeStamp;
    TTree *TTree_BeamTime;
    TTree *TTree_PPS;
    TTree *TTree_FullTimeEvolution;
    TTree *TTree_DataTimeEvolution;
    TTree *TTree_PPSTimeEvolution;
    TTree *TTree_PPS_RPi_dt;
    TTree *TTree_PPSinDelta;

    
 private:


    
    //std::map<std::string,TTree*> m_trees; 
  
  
  
};



#endif
