#include "ReadFile.h"

ReadFile::ReadFile():Tool(){}


bool ReadFile::Initialise(std::string configfile, DataModel &data)
{
    if(configfile!="")  m_variables.Initialise(configfile);
    //m_variables.Print();

    m_data= &data;
    m_log= m_data->Log;

    if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;

    if(!m_variables.Get("Path",Path)) Path="./";
    if(!m_variables.Get("Path_Out",Path_Out)) Path_Out="./";
    if(!m_variables.Get("Prefix",Prefix)) Prefix="RAWDataR";
    if(!m_variables.Get("RunNumber",RunNumber)) RunNumber= 1234;

    //Generate full file 
    if(!m_variables.Get("PartNumber",PartNumber)) PartNumber=0;

    m_data->Path = Path+to_string(RunNumber)+"/";
    m_data->Path_Out = Path_Out+to_string(RunNumber)+"/";
    int result = mkdir(m_data->Path_Out.c_str(), 0777);
    if (result == 0) {
        std::cout << "Directory created successfully." << std::endl;
    } else {
        std::cerr << "Failed to create directory or already exists." << std::endl;
    }

    m_data->Prefix = Prefix;
    m_data->RunNumber = RunNumber;

    m_data->SwitchToEval = false;

    counter_of_doom = 0;

    return true;
}


bool ReadFile::Execute()
{   
    //Create file location
    try
    {
        File = m_data->Path+Prefix+to_string(RunNumber)+"S0p"+to_string(PartNumber);
        std::ifstream chk_file(File);
        if(!chk_file.good() || counter_of_doom>10)
        {
            chk_file.close();
            m_data->vars.Set("StopLoop",1);
            cout << "File does not exist or there were enough empty files: " << File << endl;
            return true;  
        }  
    }catch(const std::exception& e)
    {
        std::cerr << "File exist check failed! " << e.what() << '\n';
    }

    //Create raw boost-stores, since they are nested the raw data file is loaded here+
    //See argument 0
    BoostStore *RawData = new BoostStore(false,0);
    BoostStore *LAPPDData = new BoostStore(false,2);
    try
    {
        RawData->Initialise(File);
        if(m_verbose>1){RawData->Print(false); cout<<"---------------------"<<endl;}
        //And the LAPPD data here,see argument 2
        RawData->Get("LAPPDData",*LAPPDData);
        if(m_verbose>1){LAPPDData->Print(false); cout<<"---------------------"<<endl;}    
    }catch(const std::exception& e)
    {
        std::cerr << "Store init failed! " << e.what() << '\n';
    }

    try
    {    
        //Give the PsecData object from the boost store to the local one
        long entries=0;
        LAPPDData->Header->Get("TotalEntries",entries);
        cout << "-----------------------------"<< endl;
        cout << "Working on partfile " << PartNumber << " has " << entries << " entries"<< endl;
        if(entries==0)
        {
            counter_of_doom++;
        }else
        {
            counter_of_doom = 0;
        }
        for (int i_entry=0; i_entry < entries; i_entry++)
        {
            if(m_verbose>3){cout << "Starting entry " << i_entry << endl;}
            LAPPDData->GetEntry(i_entry);
            if(m_verbose>3){cout << "Got entry " << i_entry << endl;}
            //Create a PsecData Object to give the data to
            PsecData PDATA;
            LAPPDData->Get("LAPPDData",PDATA);
            if(m_verbose>2){PDATA.Print();}

            //... futher stuff
            m_data->PartFileMap.insert(std::pair<int,PsecData>(i_entry,PDATA));
        }
    }catch(const std::exception& e)
    {
        std::cerr << "Store load and fill failed! " << e.what() << '\n';
    }

    try
    {
        LAPPDData->Close();
        delete LAPPDData;
        LAPPDData = 0;
        RawData->Close();
        delete RawData;
        RawData = 0;
    }catch(const std::exception& e)
    {
        std::cerr << "Store close failed! " << e.what() << '\n';
    }

    PartNumber++;

    return true;
}


bool ReadFile::Finalise()
{
    cout<<"Run "<< m_data->RunNumber << " finished analysing!"<<endl;
    return true;
}
