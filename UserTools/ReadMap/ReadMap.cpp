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

    Path = m_data->Path;
    Prefix = "RAWLAPPD";
    storename = "LAPPDStore";

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
        string File = Path+Prefix;
        m_data->Stores[storename]->Initialise(File);

        m_data->Stores[storename]->Get("RAWLAPPD0",m_data->RAWLAPPD0);
        m_data->Stores[storename]->Get("RAWLAPPD1",m_data->RAWLAPPD1);
        m_data->Stores[storename]->Get("RAWLAPPD2",m_data->RAWLAPPD2);


        m_data->Stores[storename]->Close();
        delete m_data->Stores[storename];
        m_data->Stores[storename] = 0;
    } catch (std::exception& e){
        std::cerr<<"Execute caught exception "<<e.what()<<std::endl;
        return false;
    }

    return true;
}


bool ReadMap::Finalise()
{
    cout<<"Run "<< m_data->RunNumber << " finished analysing!"<<endl;
    return true;
}
