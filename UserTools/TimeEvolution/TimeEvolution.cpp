#include "TimeEvolution.h"

TimeEvolution::TimeEvolution():Tool(){}


bool TimeEvolution::Initialise(std::string configfile, DataModel &data)
{
    if(configfile!="")  m_variables.Initialise(configfile);
    //m_variables.Print();

    m_data= &data;
    m_log= m_data->Log;

    if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
    if(!m_variables.Get("LAPPDID",LAPPDID)) LAPPDID=-1;

    storename= "LAPPDStore";
    entryname = "RAWLAPPD" + to_string(LAPPDID);

    m_data->TTree_FullTimeEvolution->Branch("FullTimeEvolution", &tevo, "tevo/i");
    m_data->TTree_PPSTimeEvolution->Branch("PPSTimeEvolution", &ppsdt, "ppsdt/l");
    m_data->TTree_DataTimeEvolution->Branch("DataTimeEvolution", &datadt, "datadt/l");

    previous_pps_ts = 0;
    previous_evo_point = 0;

    return true;
}


bool TimeEvolution::Execute()
{
    map<int,PsecData> tmpMap;
    try
    {
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
    }catch(std::exception& e){
        std::cerr<<"Execute caught exception "<<e.what()<<std::endl;
        return false;
    }

    if(m_verbose>1){cout<<"Run "<< m_data->RunNumber << " with " << Size << " entries: Beamgate start ... ";}

    try
    {
        GetTimeEvolution(tmpMap,7795);
    }catch(std::exception& e){
        std::cerr<<"Execute caught exception v7795 "<<e.what()<<std::endl;
        return false;
    }
    try
    {
        GetTimeEvolution(tmpMap,16);
    }catch(std::exception& e){
        std::cerr<<"Execute caught exception v16 "<<e.what()<<std::endl;
        return false;
    }
    try
    {
        GetFullTimeEvolution(tmpMap);
    }catch(std::exception& e){
        std::cerr<<"Execute caught exception vfull "<<e.what()<<std::endl;
        return false;
    }
    if(m_verbose>1){cout<<"Done!!"<<endl;}

    return true;
}


bool TimeEvolution::Finalise()
{  
    m_data->TTree_FullTimeEvolution->Write();
    m_data->TTree_PPSTimeEvolution->Write();
    m_data->TTree_DataTimeEvolution->Write();
    return true;
}


void TimeEvolution::GetTimeEvolution(std::map<int, PsecData> tmpMap, int size)
{
    previous_evo_point==0;
    for(std::map<int, PsecData>::iterator it=tmpMap.begin(); it!=tmpMap.end(); ++it)
    {
        vector<unsigned short> TmpVector = it->second.RawWaveform;
            
        if(m_verbose>2){cout<<"Entry "<<it->first<<" got "<<TmpVector.size()<<" size"<<endl;}

        if(TmpVector.size()==2*size && size==7795)
        {
            //Timestamp ts
            unsigned short ts_p1 = TmpVector.at(1548);
            unsigned short ts_p2 = TmpVector.at(3100);
            unsigned short ts_p3 = TmpVector.at(4652);
            unsigned short ts_p4 = TmpVector.at(6204);

            stringstream ss_TS;
            ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_p4;
            ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_p3;
            ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_p2;
            ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_p1;
            full_ts = std::stoull(ss_TS.str(),nullptr,16);
        }else if(TmpVector.size()==2*size && size==16)
        {
            //Timestamp pps
            unsigned short pps_p1 = TmpVector.at(5);
            unsigned short pps_p2 = TmpVector.at(4);
            unsigned short pps_p3 = TmpVector.at(3);
            unsigned short pps_p4 = TmpVector.at(2);

            stringstream ss_PPS;
            ss_PPS << std::setfill('0') << std::setw(4) << std::hex << pps_p4;
            ss_PPS << std::setfill('0') << std::setw(4) << std::hex << pps_p3;
            ss_PPS << std::setfill('0') << std::setw(4) << std::hex << pps_p2;
            ss_PPS << std::setfill('0') << std::setw(4) << std::hex << pps_p1;
            full_ts = std::stoull(ss_PPS.str(),nullptr,16);
        }else
        {
            continue;
        }

        if(previous_evo_point==0)
        {
            previous_evo_point = full_ts;
        }          
        if(previous_evo_point>full_ts)
        {
            previous_evo_point = full_ts;
            continue;
        } 

        if(size==7795)
        {
            datadt = (full_ts - previous_evo_point)/320000000;
            previous_evo_point = full_ts;   
            m_data->TTree_DataTimeEvolution->Fill();
        }else if(size==16)
        {
            ppsdt = (full_ts - previous_evo_point)/320000000;
            previous_evo_point = full_ts; 
            m_data->TTree_PPSTimeEvolution->Fill();            
        }  
    }
}


void TimeEvolution::GetFullTimeEvolution(std::map<int, PsecData> tmpMap)
{
    previous_evo_point==0;
    for(std::map<int, PsecData>::iterator it=tmpMap.begin(); it!=tmpMap.end(); ++it)
    {
        vector<unsigned short> TmpVector = it->second.RawWaveform;
            
        if(m_verbose>2){cout<<"Entry "<<it->first<<" got "<<TmpVector.size()<<" size"<<endl;}

        if(TmpVector.size()==2*7795)
        {
            //Timestamp ts
            unsigned short ts_p1 = TmpVector.at(1548);
            unsigned short ts_p2 = TmpVector.at(3100);
            unsigned short ts_p3 = TmpVector.at(4652);
            unsigned short ts_p4 = TmpVector.at(6204);

            stringstream ss_TS;
            ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_p4;
            ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_p3;
            ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_p2;
            ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_p1;
            full_ts = std::stoull(ss_TS.str(),nullptr,16);
        }else if(TmpVector.size()==2*16)
        {
            //Timestamp pps
            unsigned short pps_p1 = TmpVector.at(5);
            unsigned short pps_p2 = TmpVector.at(4);
            unsigned short pps_p3 = TmpVector.at(3);
            unsigned short pps_p4 = TmpVector.at(2);

            stringstream ss_PPS;
            ss_PPS << std::setfill('0') << std::setw(4) << std::hex << pps_p4;
            ss_PPS << std::setfill('0') << std::setw(4) << std::hex << pps_p3;
            ss_PPS << std::setfill('0') << std::setw(4) << std::hex << pps_p2;
            ss_PPS << std::setfill('0') << std::setw(4) << std::hex << pps_p1;
            full_ts = std::stoull(ss_PPS.str(),nullptr,16);
        }

        if(previous_evo_point==0)
        {
            previous_evo_point = full_ts;
        }    
        if(previous_evo_point>full_ts)
        {
            previous_evo_point = full_ts;
            continue;
        }   

        tevo = (full_ts - previous_evo_point)/320000000;
        previous_evo_point = full_ts; 
        m_data->TTree_FullTimeEvolution->Fill(); 
    }  
}
