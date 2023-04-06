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
        string outpath = Path + "TimeCompare_L"+ to_string(LAPPDID);
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
            
            if(m_verbose>1){cout<<"Run "<< m_data->RunNumber << " : Time Compare start ... ";}
            for(std::map<int, PsecData>::iterator it=tmpMap.begin(); it!=tmpMap.end(); ++it)
            {
                vector<unsigned short> TmpVector = it->second.RawWaveform;
                if(m_verbose>1){cout<<"Entry "<<it->first<<" got "<<TmpVector.size()<<" size"<<endl;}
                if(TmpVector.size()==2*7795)
                {
                    //Timestamp ts
                    ts_p1 = TmpVector.at(1548);
                    ts_p2 = TmpVector.at(3100);
                    ts_p3 = TmpVector.at(4652);
                    ts_p4 = TmpVector.at(6204);
                }else if(TmpVector.size()==2*16)
                {
                    //Timestamp ts
                    ts_p1 = TmpVector.at(5);
                    ts_p2 = TmpVector.at(4);
                    ts_p3 = TmpVector.at(3);
                    ts_p4 = TmpVector.at(2);                    
                }
                stringstream ss_TS;
                ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_p4;
                ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_p3;
                ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_p2;
                ss_TS << std::setfill('0') << std::setw(4) << std::hex << ts_p1;
                unsigned long long full_ts = std::stoull(ss_TS.str(),nullptr,16);

                outfile << full_ts << "," << it->second.Timestamp << "," << it->second.FailedReadCounter << endl;
            }
            if(m_verbose>1){cout<<"Done!!"<<endl;}
        } catch (std::exception& e){
            std::cerr<<"Execute caught exception "<<e.what()<<std::endl;
            return false;
        }
        outfile.close();

    return true;
}


bool TimeCompare::Finalise(){

  return true;
}
