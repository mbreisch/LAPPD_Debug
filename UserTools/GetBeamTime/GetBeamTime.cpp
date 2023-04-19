#include "GetBeamTime.h"

GetBeamTime::GetBeamTime():Tool(){}


bool GetBeamTime::Initialise(std::string configfile, DataModel &data)
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

    m_data->TTree_BeamTime->Branch("Psec_TimeStamp", &full_ts, "full_ts/L");
    m_data->TTree_BeamTime->Branch("Beamgate_TimeStamp", &full_bts, "full_bts/L");
    m_data->TTree_BeamTime->Branch("DeltaT_TimeStamp", &full_dt, "full_dt/L");
    m_data->TTree_PPS->Branch("PPS_TimeStamp", &full_pps, "full_pps/L");
    
    return true;
}


bool GetBeamTime::Execute()
{
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

        if(m_verbose>1){cout<<"Run "<< m_data->RunNumber << " with " << Size << " entries: Beamgate start ... ";}
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

                //Beamgate bts
                unsigned short bts_p1 = TmpVector.at(6201);
                unsigned short bts_p2 = TmpVector.at(4649);
                unsigned short bts_p3 = TmpVector.at(3097);
                unsigned short bts_p4 = TmpVector.at(1545);

                stringstream ss_TS;
                ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_p4;
                ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_p3;
                ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_p2;
                ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_p1;
                full_ts = std::stoull(ss_TS.str(),nullptr,16);

                stringstream ss_BTS;
                ss_BTS << std::setfill('0') << std::setw(4) << std::hex << bts_p4;
                ss_BTS << std::setfill('0') << std::setw(4) << std::hex << bts_p3;
                ss_BTS << std::setfill('0') << std::setw(4) << std::hex << bts_p2;
                ss_BTS << std::setfill('0') << std::setw(4) << std::hex << bts_p1;
                full_bts = std::stoull(ss_BTS.str(),nullptr,16);

                full_dt = (full_ts - full_bts)*3.125;

                m_data->TTree_BeamTime->Fill();
                //outfile << full_ts << "," << full_bts << "," << dt << endl;
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
                full_pps = std::stoull(ss_PPS.str(),nullptr,16);

                m_data->TTree_PPS->Fill();
            }
        }
        if(m_verbose>1){cout<<"Done!!"<<endl;}
    }catch(std::exception& e){
        std::cerr<<"Execute caught exception "<<e.what()<<std::endl;
        return false;
    }
    //outfile.close();

    return true;
}


bool GetBeamTime::Finalise()
{
    m_data->TTree_BeamTime->Write();
    m_data->TTree_PPS->Write();
    return true;
}
