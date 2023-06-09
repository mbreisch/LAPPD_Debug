#include "ReadMap.h"

ReadMap::ReadMap():Tool(){}


bool ReadMap::Initialise(std::string configfile, DataModel &data)
{
    if(configfile!="")  m_variables.Initialise(configfile);
    //m_variables.Print();

    m_data= &data;
    m_log= m_data->Log;

    if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
    if(!m_variables.Get("LAPPDID",LAPPDID)) LAPPDID=-1;
    if(!m_variables.Get("RunNumber",RunNumber)) RunNumber= 1234;
    if(!m_variables.Get("Path",Path)) Path="./";

    float Test;
    if(!m_variables.Get("Test",Test)) Test= 1234;

    Prefix = "RAWLAPPD";
    storename = "LAPPDStore";

    m_data->RunNumber = RunNumber;
    m_data->Path =  Path +std::to_string(m_data->RunNumber) + "/";
    m_data->Path_Out =  Path +std::to_string(m_data->RunNumber) + "/";

    //Generate the rootfile
    std::string savelocation = m_data->Path_Out+ "Analysis.root";
    m_data->RootFile = new TFile(savelocation.c_str(),"RECREATE");

    return true;
}


bool ReadMap::Execute()
{   
    try
    {
        int storeexists = m_data->Stores.count(storename);
        if(storeexists==0)
        {
            m_data->Stores[storename] = new BoostStore(false,2);
        }    
        string File = m_data->Path+Prefix;
        m_data->Stores[storename]->Initialise(File);
        //m_data->Stores[storename]->Print(false);

        m_data->Stores[storename]->Get("RAWLAPPD0",m_data->RAWLAPPD0);
        m_data->Stores[storename]->Get("RAWLAPPD1",m_data->RAWLAPPD1);
        m_data->Stores[storename]->Get("RAWLAPPD2",m_data->RAWLAPPD2);

        std::cout << "Loaded LAPPD 0 data with " << m_data->RAWLAPPD0.size() << " entries..." << std::endl;
        std::cout << "Loaded LAPPD 1 data with " << m_data->RAWLAPPD1.size() << " entries..." << std::endl;
        std::cout << "Loaded LAPPD 2 data with " << m_data->RAWLAPPD2.size() << " entries..." << std::endl;
        std::cout << "-------------------------" << std::endl;

        m_data->Stores[storename]->Close();
        delete m_data->Stores[storename];
        m_data->Stores[storename] = 0;
    } catch (std::exception& e){
        std::cerr<<"Execute caught exception "<<e.what()<<std::endl;
        return false;
    }

    m_data->RootFile->cd();

    return true;
}


bool ReadMap::Finalise()
{
    return true;
}
