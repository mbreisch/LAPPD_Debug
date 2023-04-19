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

    storename= "LAPPDStore";
    entryname = "RAWLAPPD" + to_string(LAPPDID);

    Path = m_data->Path;

    
    m_data->TTree_TimeStamp->Branch("TimeStamp", &ts, "ts/L");
    m_data->TTree_TimeStamp->Branch("Type", &ts_type, "ts_type/i");
    m_data->TTree_TimeStamp->Branch("LAPPDID", &LAPPDID, "LAPPDID/i");
    m_data->TTree_TimeStamp->Branch("Size", &Size, "Size/l");

    return true;
}


bool GetTimeStamp::Execute()
{
    //string outpath = Path + "GlobalTimeStamp_L"+ to_string(LAPPDID);
    //ofstream outfile(outpath.c_str(),ios_base::out | ios_base::trunc);
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
        
        if(m_verbose>1){cout<<"Run "<< m_data->RunNumber << " with " << Size << " entries: Global Timestamp start ... ";}
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
                //outfile << it->second.Timestamp << "," << "pps" << endl;
            }else if(frame==7795)
            {
                ts = std::stoull(it->second.Timestamp.c_str(),nullptr,10);
                ts_type = 1;
                //outfile << it->second.Timestamp << "," << "data" << endl;
            }else
            {
                ts = std::stoull(it->second.Timestamp.c_str(),nullptr,10);
                ts_type = 2;
                //outfile << it->second.Timestamp << "," << "chaos" << endl;
                // cout << "ALARM" << endl;
                // it->second.Print();
                // for(unsigned short k: it->second.AccInfoFrame)
                // {
                //     cout << std::hex << k << endl;
                // }roo
                // cout << std::dec;
            }
            m_data->TTree_TimeStamp->Fill();
        }
        if(m_verbose>1){cout<<"Done!!"<<endl;}
    } catch (std::exception& e){
        std::cerr<<"Execute caught exception "<<e.what()<<std::endl;
        return false;
    }

    //outfile.close();

    return true;
}


bool GetTimeStamp::Finalise()
{
    m_data->TTree_TimeStamp->Write();
    return true;
}
