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

    storename = "LAPPDStore" + to_string(LAPPDID);

    return true;
}


bool ReadMap::Execute()
{
    if(m_data->SwitchToEval)
    {
        try
        {
            if(LAPPDID==0)
            {   
                cout << m_data->RAWLAPPD0.size() << endl;
                if(m_data->RAWLAPPD0.size()!=0)
                {
                    return true;
                }
            }else if(LAPPDID==1)
            {
                cout << m_data->RAWLAPPD1.size() << endl;
                if(m_data->RAWLAPPD1.size()!=0)
                {
                    return true;
                } 
            }else if(LAPPDID==2)
            {
                cout << m_data->RAWLAPPD2.size() << endl;
                if(m_data->RAWLAPPD2.size()!=0)
                {
                    return true;
                } 
            }

            int storeexists = m_data->Stores.count(storename);
            if(storeexists==0)
            {
                m_data->Stores[storename] = new BoostStore(false,2);
            }    
            string File = Path+Prefix+to_string(LAPPDID);
            m_data->Stores[storename]->Initialise(File);

            if(LAPPDID==0)
            {   
                m_data->Stores[storename]->Get("RAWLAPPD0",m_data->RAWLAPPD0);
            }else if(LAPPDID==1)
            {
                m_data->Stores[storename]->Get("RAWLAPPD1",m_data->RAWLAPPD1);
            }else if(LAPPDID==2)
            {
                m_data->Stores[storename]->Get("RAWLAPPD2",m_data->RAWLAPPD2);
            }

            m_data->Stores[storename]->Close();
            delete m_data->Stores[storename];
            m_data->Stores[storename] = 0;
        } catch (std::exception& e){
            std::cerr<<"Execute caught exception "<<e.what()<<std::endl;
            return false;
        }
    }

    return true;
}


bool ReadMap::Finalise()
{
    return true;
}
