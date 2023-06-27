#include "TimeCompare.h"

TimeCompare::TimeCompare():Tool(){}


bool TimeCompare::Initialise(std::string configfile, DataModel &data)
{
    if(configfile!="")  m_variables.Initialise(configfile);
    //m_variables.Print();

    m_data= &data;
    m_log= m_data->Log;

    if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
    if(!m_variables.Get("LAPPDID",LAPPDID)) LAPPDID=-1;

    storename= "LAPPDStore";
    entryname = "RAWLAPPD" + to_string(LAPPDID);

    Path = m_data->Path;
    return true;
}


bool TimeCompare::Execute()
{
    m_data->TTree_PPSinDelta = new TTree("PPSinDelta","PPSinDelta");
    m_data->TTree_PPSinDelta->Branch("PPSinDelta", &PPSinDelta);
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
        int Size = tmpMap.size();
            
        if(m_verbose>1){cout<<"Run "<< m_data->RunNumber << " for LAPPD-ID " << LAPPDID << " with " << Size << " entries: Time Compare start ... ";}
        for(std::map<int, PsecData>::iterator it=tmpMap.begin(); it!=tmpMap.end(); ++it)
        {
            vector<unsigned short> TmpVector = it->second.RawWaveform;
            if(m_verbose>2){cout<<"Entry "<<it->first<<" got "<<TmpVector.size()<<" size"<<endl;}
            if(TmpVector.size()==2*7795)
            {
               continue;
            }else if(TmpVector.size()==2*16)
            {
                //Timestamp ts
                ts_pps1_p1 = TmpVector.at(5);
                ts_pps1_p2 = TmpVector.at(4);
                ts_pps1_p3 = TmpVector.at(3);
                ts_pps1_p4 = TmpVector.at(2);                    
            
                ts_pps2_p1 = TmpVector.at(5+16);
                ts_pps2_p2 = TmpVector.at(4+16);
                ts_pps2_p3 = TmpVector.at(3+16);
                ts_pps2_p4 = TmpVector.at(2+16);                    

                stringstream ss_TS;
                ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_pps1_p4;
                ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_pps1_p3;
                ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_pps1_p2;
                ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_pps1_p1;
                unsigned long long full_pps1 = std::stoull(ss_TS.str(),nullptr,16);

                stringstream ss_TS2;
                ss_TS2 << std::setfill('0') << std::setw(4) << std::hex << ts_pps2_p4;
                ss_TS2 << std::setfill('0') << std::setw(4) << std::hex << ts_pps2_p3;
                ss_TS2 << std::setfill('0') << std::setw(4) << std::hex << ts_pps2_p2;
                ss_TS2 << std::setfill('0') << std::setw(4) << std::hex << ts_pps2_p1;
                unsigned long long full_pps2 = std::stoull(ss_TS2.str(),nullptr,16);

                PPSinDelta = ((long long)full_pps1 - (long long)full_pps2);
                m_data->TTree_PPSinDelta->Fill();
            }
        }
        if(m_verbose>1){cout<<"Done!!"<<endl;}
    } catch (std::exception& e){
        std::cerr<<"Execute caught exception "<<e.what()<<std::endl;
        return false;
    }

    m_data->TTree_PPSinDelta->Write();
    m_data->TTree_PPSinDelta->Reset();

    return true;
}


bool TimeCompare::Finalise()
{
    return true;
}
