#include "GetEventInfos.h"

GetEventInfos::GetEventInfos():Tool(){}


bool GetEventInfos::Initialise(std::string configfile, DataModel &data)
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

    std::string savelocation = m_data->Path_Out+ "Event.root";
    EventRootFile = new TFile(savelocation.c_str(),"RECREATE");
    TTree_Event = new TTree("Event", "Event");
    TTree_AllEvents = new TTree("AllEvent", "AllEvent");
    
    TTree_AllEvents->Branch("TimeStamp", &TimeStamp, "ts/L");
    TTree_AllEvents->Branch("Waveform_acdc0", &Waveform_acdc0);
    TTree_AllEvents->Branch("Waveform_acdc1", &Waveform_acdc1);
    TTree_AllEvents->Branch("TimeStamp_Inner0", &TimeStamp_Inner0, "TimeStamp_Inner0/L");
    TTree_AllEvents->Branch("TimeStamp_Inner1", &TimeStamp_Inner1, "TimeStamp_Inner1/L");
    TTree_AllEvents->Branch("Counter_acdc0", &Counter_acdc0, "ts/I");
    TTree_AllEvents->Branch("Counter_acdc1", &Counter_acdc1, "ts/I");

    TTree_Event->Branch("LAPPDID", &LAPPDID, "ts/I");
    TTree_Event->Branch("TimeStamp", &TimeStamp, "ts/L");
    TTree_Event->Branch("Index", &Index);
    TTree_Event->Branch("Waveform_acdc0", &Waveform_acdc0);
    TTree_Event->Branch("Waveform_acdc1", &Waveform_acdc1);
    TTree_Event->Branch("TimeStamp_Inner0", &TimeStamp_Inner0, "TimeStamp_Inner0/L");
    TTree_Event->Branch("TimeStamp_Inner1", &TimeStamp_Inner1, "TimeStamp_Inner1/L");
    TTree_Event->Branch("Counter_acdc0", &Counter_acdc0, "ts/I");
    TTree_Event->Branch("Counter_acdc1", &Counter_acdc1, "ts/I");

    return true;
}


bool GetEventInfos::Execute()
{
    EventRootFile->cd();
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
        if(m_verbose>1){cout<<"Run "<< m_data->RunNumber << " with " << Size << " entries: Event Info grab start ... ";}
        for(std::map<int, PsecData>::iterator it=tmpMap.begin(); it!=tmpMap.end(); ++it)
        {
            vector<unsigned short> TmpVector = it->second.RawWaveform;
            if(m_verbose>2){cout<<"Entry "<<it->first<<" got "<<TmpVector.size()<<" size"<<endl;}

            if(TmpVector.size()==2*16)
            {   
                if(it->second.Timestamp=="")
                {
                    TimeStamp = 405;
                }else
                {
                    TimeStamp = std::stoull(it->second.Timestamp.c_str(),nullptr,10);
                }
                Index = it->second.BoardIndex;
                Waveform_acdc0.assign(TmpVector.begin(),TmpVector.begin()+16);
                Waveform_acdc1.assign(TmpVector.begin()+16,TmpVector.end());

                unsigned short p1 = TmpVector.at(9);
                unsigned short p2 = TmpVector.at(8);
                stringstream ss_c1;
                ss_c1 << std::setfill('0') << std::setw(4) << std::hex << p2;
                ss_c1 << std::setfill('0') << std::setw(4) << std::hex << p1;
                Counter_acdc0 = std::stoi(ss_c1.str(),nullptr,16);

                unsigned short p3 = TmpVector.at(9+16);
                unsigned short p4 = TmpVector.at(8+16);
                stringstream ss_c2;
                ss_c2 << std::setfill('0') << std::setw(4) << std::hex << p4;
                ss_c2 << std::setfill('0') << std::setw(4) << std::hex << p3;
                Counter_acdc1 = std::stoi(ss_c2.str(),nullptr,16);
                if(m_verbose>2){cout << "PPS: " << Counter_acdc0 << " | " << Counter_acdc1 << endl;}

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
                TimeStamp_Inner0 = std::stoull(ss_PPS.str(),nullptr,16);

                //Timestamp pps
                pps_p1 = TmpVector.at(5+16);
                pps_p2 = TmpVector.at(4+16);
                pps_p3 = TmpVector.at(3+16);
                pps_p4 = TmpVector.at(2+16);

                stringstream ss_PPS2;
                ss_PPS2 << std::setfill('0') << std::setw(4) << std::hex << pps_p4;
                ss_PPS2 << std::setfill('0') << std::setw(4) << std::hex << pps_p3;
                ss_PPS2 << std::setfill('0') << std::setw(4) << std::hex << pps_p2;
                ss_PPS2 << std::setfill('0') << std::setw(4) << std::hex << pps_p1;
                TimeStamp_Inner1 = std::stoull(ss_PPS.str(),nullptr,16);

            }else if(TmpVector.size()==2*7795)
            {                
                if(it->second.Timestamp=="")
                {
                    TimeStamp = 405;
                }else
                {
                    TimeStamp = std::stoull(it->second.Timestamp.c_str(),nullptr,10);
                }
                Index = it->second.BoardIndex;
                Waveform_acdc0.assign(TmpVector.begin(),TmpVector.begin()+7795);
                Waveform_acdc1.assign(TmpVector.begin()+7795,TmpVector.end());

                unsigned short p1 = TmpVector.at(1549);
                unsigned short p2 = TmpVector.at(3101);
                stringstream ss_c1;
                ss_c1 << std::setfill('0') << std::setw(4) << std::hex << p2;
                ss_c1 << std::setfill('0') << std::setw(4) << std::hex << p1;
                Counter_acdc0 = std::stoi(ss_c1.str(),nullptr,16);

                unsigned short p3 = TmpVector.at(1549+7795);
                unsigned short p4 = TmpVector.at(3101+7795);
                stringstream ss_c2;
                ss_c2 << std::setfill('0') << std::setw(4) << std::hex << p4;
                ss_c2 << std::setfill('0') << std::setw(4) << std::hex << p3;
                Counter_acdc1 = std::stoi(ss_c2.str(),nullptr,16);
                if(m_verbose>2){cout << "DATA: " << Counter_acdc0 << " | " << Counter_acdc1 << endl;}

                //Beamgate bts
                unsigned short ts_p1 = TmpVector.at(1548);
                unsigned short ts_p2 = TmpVector.at(3100);
                unsigned short ts_p3 = TmpVector.at(4652);
                unsigned short ts_p4 = TmpVector.at(6204);

                stringstream ss_BTS;
                ss_BTS << std::setfill('0') << std::setw(4) << std::hex << ts_p4;
                ss_BTS << std::setfill('0') << std::setw(4) << std::hex << ts_p3;
                ss_BTS << std::setfill('0') << std::setw(4) << std::hex << ts_p2;
                ss_BTS << std::setfill('0') << std::setw(4) << std::hex << ts_p1;
                TimeStamp_Inner0 = std::stoull(ss_BTS.str(),nullptr,16);

                ts_p1 = TmpVector.at(1548+7795);
                ts_p2 = TmpVector.at(3100+7795);
                ts_p3 = TmpVector.at(4652+7795);
                ts_p4 = TmpVector.at(6204+7795);

                stringstream ss_BTS2;
                ss_BTS2 << std::setfill('0') << std::setw(4) << std::hex << ts_p4;
                ss_BTS2 << std::setfill('0') << std::setw(4) << std::hex << ts_p3;
                ss_BTS2 << std::setfill('0') << std::setw(4) << std::hex << ts_p2;
                ss_BTS2 << std::setfill('0') << std::setw(4) << std::hex << ts_p1;
                TimeStamp_Inner1 = std::stoull(ss_BTS2.str(),nullptr,16);
            }else
            {
                TimeStamp = 404;
                Index = {404};
                Waveform_acdc0 = {404};
                Waveform_acdc1 = {404};
                Counter_acdc0 = 404;
                Counter_acdc1 = 404;
                TimeStamp_Inner0 = 404;
                TimeStamp_Inner1 = 404;
            }
            TTree_Event->Fill();
            TTree_Event->Write();
            TTree_Event->Reset();
            TTree_AllEvents->Fill();
        }
        if(m_verbose>1){cout<<"Done!!"<<endl;}
    } catch (std::exception& e){
        std::cerr<<"Execute caught exception "<<e.what()<<std::endl;
        return false;
    }

    m_data->RootFile->cd();

    return true;
}


bool GetEventInfos::Finalise()
{
    EventRootFile->cd();
    TTree_AllEvents->Write();
    m_data->RootFile->cd();
    EventRootFile->Close();
    delete EventRootFile;
    return true;
}
