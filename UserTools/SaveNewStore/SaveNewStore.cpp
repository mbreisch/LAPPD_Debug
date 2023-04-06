#include "SaveNewStore.h"

SaveNewStore::SaveNewStore():Tool(){}


bool SaveNewStore::Initialise(std::string configfile, DataModel &data)
{
    if(configfile!="")  m_variables.Initialise(configfile);
    //m_variables.Print();

    m_data= &data;
    m_log= m_data->Log;

    if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;

    Path = m_data->Path;

    int storeexists = m_data->Stores.count("LAPPDStore0");
    if(storeexists==0)
    {
        m_data->Stores["LAPPDStore0"] = new BoostStore(false,2);
    }    
    storeexists = m_data->Stores.count("LAPPDStore1");
    if(storeexists==0)
    {
        m_data->Stores["LAPPDStore1"] = new BoostStore(false,2);
    }    
    storeexists = m_data->Stores.count("LAPPDStore2");
    if(storeexists==0)
    {
        m_data->Stores["LAPPDStore2"] = new BoostStore(false,2);
    }    

    return true;
}


bool SaveNewStore::Execute()
{
    if(m_data->SwitchToEval)
    {
        string FileStore0 = m_data->Path+"RAWLAPPD0";
        std::ifstream chk_file0(FileStore0);
        if(!chk_file0.good())
        {
            cout << FileStore0 << " does not exist as store file so I'll create it!" << endl;
            if(m_data->RAWLAPPD0.size()==0)
            {
                PsecData TMP;
                TMP.errorcodes.push_back(0xdeadbeef);
                m_data->RAWLAPPD0.insert(pair<int,PsecData>(-1,TMP));
                string newPATH;
                newPATH = Path + "RAWLAPPD0";
                m_data->Stores["LAPPDStore0"]->Set("RAWLAPPD0",m_data->RAWLAPPD0); 
                m_data->Stores["LAPPDStore0"]->Save(newPATH.c_str());
                m_data->Stores["LAPPDStore0"]->Delete(); 
            }
            chk_file0.close();
            m_data->Stores["LAPPDStore0"]->Close();
            delete m_data->Stores["LAPPDStore0"];
            m_data->Stores["LAPPDStore0"] = 0;
        }

        string FileStore1 = m_data->Path+"RAWLAPPD1";
        std::ifstream chk_file1(FileStore1);
        if(!chk_file1.good())
        {
            cout << FileStore1 << " does not exist as store file so I'll create it!" << endl;
            if(m_data->RAWLAPPD1.size()==0)
            {
                PsecData TMP;
                TMP.errorcodes.push_back(0xdeadbeef);
                m_data->RAWLAPPD1.insert(pair<int,PsecData>(-1,TMP));
                string newPATH;
                newPATH = Path + "RAWLAPPD1";
                m_data->Stores["LAPPDStore1"]->Set("RAWLAPPD1",m_data->RAWLAPPD1); 
                m_data->Stores["LAPPDStore1"]->Save(newPATH.c_str());
                m_data->Stores["LAPPDStore1"]->Delete(); 
            }
            chk_file1.close();
            m_data->Stores["LAPPDStore1"]->Close();
            delete m_data->Stores["LAPPDStore1"];
            m_data->Stores["LAPPDStore1"] = 0;
        }
        
        string FileStore2 = m_data->Path+"RAWLAPPD2";
        std::ifstream chk_file2(FileStore2);
        if(!chk_file2.good())
        {
            cout << FileStore2 << " does not exist as store file so I'll create it!" << endl;
            if(m_data->RAWLAPPD2.size()==0)
            {
                PsecData TMP;
                TMP.errorcodes.push_back(0xdeadbeef);
                m_data->RAWLAPPD2.insert(pair<int,PsecData>(-1,TMP));
                string newPATH;
                newPATH = Path + "RAWLAPPD2";
                m_data->Stores["LAPPDStore2"]->Set("RAWLAPPD2",m_data->RAWLAPPD2); 
                m_data->Stores["LAPPDStore2"]->Save(newPATH.c_str());
                m_data->Stores["LAPPDStore2"]->Delete(); 
            }
            chk_file2.close();
            m_data->Stores["LAPPDStore2"]->Close();
            delete m_data->Stores["LAPPDStore2"];
            m_data->Stores["LAPPDStore2"] = 0;
        }
    }else
    {
        string newPATH;
        newPATH = Path + "RAWLAPPD0";
        m_data->Stores["LAPPDStore0"]->Set("RAWLAPPD0",m_data->RAWLAPPD0);
        m_data->Stores["LAPPDStore0"]->Save(newPATH.c_str());
        m_data->Stores["LAPPDStore0"]->Delete(); 

        newPATH = Path + "RAWLAPPD1";
        m_data->Stores["LAPPDStore1"]->Set("RAWLAPPD1",m_data->RAWLAPPD1);
        m_data->Stores["LAPPDStore1"]->Save(newPATH.c_str());
        m_data->Stores["LAPPDStore1"]->Delete(); 

        newPATH = Path + "RAWLAPPD2";
        m_data->Stores["LAPPDStore2"]->Set("RAWLAPPD2",m_data->RAWLAPPD2);
        m_data->Stores["LAPPDStore2"]->Save(newPATH.c_str());
        m_data->Stores["LAPPDStore2"]->Delete(); 
    }

    return true;
}


bool SaveNewStore::Finalise()
{
    return true;
}
