#include "GetTimeDifference.h"

GetTimeDifference::GetTimeDifference():Tool(){}


bool GetTimeDifference::Initialise(std::string configfile, DataModel &data)
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


bool GetTimeDifference::Execute()
{
    m_data->TTree_PPS_RPi_dt = new TTree("PPS_RPi_dt","PPS_RPi_dt");
    m_data->TTree_PPS_RPi_dt->Branch("PPS_RPi_dt", &PPS_RPi_dt, "PPS_RPi_dt/L");

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
        if(m_verbose>1){cout<<"Run "<< m_data->RunNumber << " for LAPPD-ID " << LAPPDID << " with " << Size << " entries: Time Difference start ... ";}
        for(std::map<int, PsecData>::iterator it=tmpMap.begin(); it!=tmpMap.end(); ++it)
        {
            vector<unsigned short> TmpVector = it->second.RawWaveform;
            if(m_verbose>2){cout<<"Entry "<<it->first<<" got "<<TmpVector.size()<<" size"<<endl;}

            if(it->second.Timestamp==""){continue;}

            if(TmpVector.size()==2*16)
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

                full_rpi = std::stoull(it->second.Timestamp.c_str(),nullptr,10);

                PPS_RPi_dt = full_rpi*1000000 - (full_pps*3.125);

                if(m_verbose>2){cout << "R: " << full_rpi << " | P: " << full_pps << " = " << PPS_RPi_dt << endl;}

                m_data->TTree_PPS_RPi_dt->Fill();
            }
        }
        if(m_verbose>1){cout<<"Done!!"<<endl;}
    }catch(std::exception& e){
        std::cerr<<"Execute caught exception "<<e.what()<<std::endl;
        return false;
    }

    m_data->TTree_PPS_RPi_dt->Write();
    m_data->TTree_PPS_RPi_dt->Reset();
    return true;
}


bool GetTimeDifference::Finalise()
{
    return true;
}
