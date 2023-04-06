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

    return true;
}


bool GetTimeStamp::Execute()
{
    string outpath = Path + "GlobalTimeStamp_L"+ to_string(LAPPDID);
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
        
        if(m_verbose>1){cout<<"Run "<< m_data->RunNumber << " : Global Timestamp start ... ";}
        for(std::map<int, PsecData>::iterator it=tmpMap.begin(); it!=tmpMap.end(); ++it)
        {
            vector<unsigned short> TmpVector = it->second.RawWaveform;
            if(m_verbose>2){cout<<"Entry "<<it->first<<" got "<<TmpVector.size()<<" size"<<endl;}

            int frame = TmpVector.size()/2;
            if(frame==16)
            {
                outfile << it->second.Timestamp << "," << "pps" << endl;
            }else if(frame==7795)
            {
                outfile << it->second.Timestamp << "," << "data" << endl;
            }else
            {
                outfile << it->second.Timestamp << "," << "chaos" << endl;
                cout << "ALARM" << endl;
                it->second.Print();
                for(unsigned short k: it->second.AccInfoFrame)
                {
                    cout << std::hex << k << endl;
                }
                cout << std::dec;
            }
        }
        if(m_verbose>1){cout<<"Done!!"<<endl;}
    } catch (std::exception& e){
        std::cerr<<"Execute caught exception "<<e.what()<<std::endl;
        return false;
    }

    outfile.close();

    return true;
}


bool GetTimeStamp::Finalise()
{

    return true;
}
