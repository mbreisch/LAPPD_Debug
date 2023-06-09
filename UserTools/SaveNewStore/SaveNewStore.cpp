#include "SaveNewStore.h"

SaveNewStore::SaveNewStore():Tool(){}


bool SaveNewStore::Initialise(std::string configfile, DataModel &data)
{
    if(configfile!="")  m_variables.Initialise(configfile);
    //m_variables.Print();

    m_data= &data;
    m_log= m_data->Log;

    if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;

    Path = m_data->Path_Out;

    int storeexists = m_data->Stores.count("LAPPDStore");
    if(storeexists==0)
    {
        m_data->Stores["LAPPDStore"] = new BoostStore(false,2);
    }    

    return true;
}


bool SaveNewStore::Execute()
{
    return true;
}


bool SaveNewStore::Finalise()
{
    cout<<"Run "<< m_data->RunNumber << " finished loading! Got "<< m_data->RAWLAPPD0.size() << " | " << m_data->RAWLAPPD1.size() << " | " << m_data->RAWLAPPD2.size() << endl;

    string newPATH;
    newPATH = Path + "RAWLAPPD";
    m_data->Stores["LAPPDStore"]->Set("RAWLAPPD0",m_data->RAWLAPPD0);
    m_data->Stores["LAPPDStore"]->Set("RAWLAPPD1",m_data->RAWLAPPD1);
    m_data->Stores["LAPPDStore"]->Set("RAWLAPPD2",m_data->RAWLAPPD2);
    m_data->Stores["LAPPDStore"]->Save(newPATH.c_str());
    m_data->Stores["LAPPDStore"]->Delete(); 

    m_data->Stores["LAPPDStore"]->Close();
    delete m_data->Stores["LAPPDStore"];
    m_data->Stores["LAPPDStore"] = 0;

    return true;
}
