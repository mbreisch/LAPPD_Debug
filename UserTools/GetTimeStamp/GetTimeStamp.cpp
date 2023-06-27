#include "GetTimeStamp.h"

GetTimeStamp::GetTimeStamp():Tool(){}


bool GetTimeStamp::Initialise(std::string configfile, DataModel &data)
{
    if(configfile!="")  m_variables.Initialise(configfile);
    //m_variables.Print();

    m_data= &data;
    m_log= m_data->Log;

    if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
    if(!m_variables.Get("LAPPDID",LAPPDID)) LAPPDID=-1;

    Path = m_data->Path;



    return true;
}


bool GetTimeStamp::Execute()
{
    m_data->TTree_TimeStamp = new TTree("GlobalTimeStamp", "GlobalTimeStamp");
    m_data->TTree_TimeStamp->Branch("TimeStamp", &ts, "ts/L");
    m_data->TTree_TimeStamp->Branch("Type", &ts_type, "ts_type/I");
    m_data->TTree_TimeStamp->Branch("LAPPDID", &LAPPDID, "LAPPDID/I");
    m_data->TTree_TimeStamp->Branch("Size", &Size, "Size/l");

    LAPPDID = LAPPDID;
    try
    {
        map<int,PsecData> tmpMap;

        if(LAPPDID==0)
        {
            tmpMap = m_data->RAWLAPPD0;
        }else if(LAPPDID==1)
        {
            tmpMap = m_data->RAWLAPPD1;
        }else if(LAPPDID==2)
        {
            tmpMap = m_data->RAWLAPPD2;
        }
        Size = tmpMap.size();
        
        if(m_verbose>1){cout<<"Run "<< m_data->RunNumber << " for LAPPD-ID " << LAPPDID << " with " << Size << " entries: Global Timestamp start ... ";}
        for(std::map<int, PsecData>::iterator it=tmpMap.begin(); it!=tmpMap.end(); ++it)
        {
            vector<unsigned short> TmpVector = it->second.RawWaveform;
            if(m_verbose>2){cout<<"Entry "<<it->first<<" got "<<TmpVector.size()<<" size"<<endl;}

            if(it->second.Timestamp=="")
            {
                continue;
            }

            int frame = TmpVector.size()/2;
            if(frame==16)
            {
                ts = std::stoull(it->second.Timestamp.c_str(),nullptr,10);
                ts_type = 0;
            }else if(frame==7795)
            {
                ts = std::stoull(it->second.Timestamp.c_str(),nullptr,10);
                ts_type = 1;
            }else
            {
                ts = std::stoull(it->second.Timestamp.c_str(),nullptr,10);
                ts_type = 2;
            }
            m_data->TTree_TimeStamp->Fill();
        }
        if(m_verbose>1){cout<<"Done!!"<<endl;}
    } catch (std::exception& e){
        std::cerr<<"Execute caught exception "<<e.what()<<std::endl;
        return false;
    }
    m_data->TTree_TimeStamp->Write();
    m_data->TTree_TimeStamp->Reset();

    return true;
}


bool GetTimeStamp::Finalise()
{
    return true;
}
