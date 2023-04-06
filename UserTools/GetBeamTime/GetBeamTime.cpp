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

    storename= "LAPPDStore" + to_string(LAPPDID);
    entryname = "RAWLAPPD" + to_string(LAPPDID);

    Path = m_data->Path;

    return true;
}


bool GetBeamTime::Execute()
{
    if(m_data->SwitchToEval)
    {
        string outpath = Path + "BeamTime_L"+ to_string(LAPPDID);
        ofstream outfile(outpath.c_str(),ios_base::out | ios_base::trunc);
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

            for(std::map<int, PsecData>::iterator it=tmpMap.begin(); it!=tmpMap.end(); ++it)
            {
                vector<unsigned short> TmpVector = it->second.RawWaveform;
                if(m_verbose>1){cout<<"Entry "<<it->first<<" got "<<TmpVector.size()<<" size"<<endl;}
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
                    unsigned long long full_ts = std::stoull(ss_TS.str(),nullptr,16);

                    stringstream ss_BTS;
                    ss_BTS << std::setfill('0') << std::setw(4) << std::hex << bts_p4;
                    ss_BTS << std::setfill('0') << std::setw(4) << std::hex << bts_p3;
                    ss_BTS << std::setfill('0') << std::setw(4) << std::hex << bts_p2;
                    ss_BTS << std::setfill('0') << std::setw(4) << std::hex << bts_p1;
                    unsigned long long full_bts = std::stoull(ss_BTS.str(),nullptr,16);

                    unsigned long long dt = full_ts - full_bts;
                    //printf("ts 0x%016llx\n",full_ts);
                    //printf("bts 0x%016llx\n",full_bts);
                    //printf("dt %lld\n",dt);
                    outfile << full_ts << "," << full_bts << "," << dt << endl;
                }
            }
        } catch (std::exception& e){
            std::cerr<<"Execute caught exception "<<e.what()<<std::endl;
            return false;
        }
        outfile.close();
    }
    return true;
}


bool GetBeamTime::Finalise()
{
    return true;
}
