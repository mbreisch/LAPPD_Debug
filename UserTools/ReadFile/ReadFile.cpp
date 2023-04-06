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
    if(!m_variables.Get("Prefix",Prefix)) Prefix="RAWDataR";
    if(!m_variables.Get("RunNumber",RunNumber)) RunNumber= 1234;

    //Generate full file 
    if(!m_variables.Get("PartNumber",PartNumber)) PartNumber=0;

    m_data->Path = Path+to_string(RunNumber)+"/";
    m_data->Prefix = Prefix;
    m_data->RunNumber = RunNumber;

    m_data->SwitchToEval = false;

    return true;
}


bool ReadFile::Execute()
{   
    if(m_data->SwitchToEval)
    {
        m_data->vars.Set("StopLoop",1);
        return true;
    }

    //Create file location
    File = m_data->Path+Prefix+to_string(RunNumber)+"S0p"+to_string(PartNumber);
    std::ifstream chk_file(File);
    if(!chk_file.good())
    {
        chk_file.close();
        cout << "File does not exist: " << File << endl;
        m_data->SwitchToEval = true;
        return true;  
    }

    FileStore0 = m_data->Path+"RAWLAPPD0";
    FileStore1 = m_data->Path+"RAWLAPPD1";
    FileStore2 = m_data->Path+"RAWLAPPD2";
    std::ifstream chk_file0(FileStore0);
    std::ifstream chk_file1(FileStore1);
    std::ifstream chk_file2(FileStore2);
    if(chk_file0.good() && chk_file2.good() && chk_file1.good())
    {
        m_data->SwitchToEval = true;
        m_data->vars.Set("StopLoop",1);
        return true;
    }

    //Create raw boost-stores, since they are nested the raw data file is loaded here+
    //See argument 0
    BoostStore *RawData = new BoostStore(false,0);
    RawData->Initialise(File);
    if(m_verbose>1){RawData->Print(false); cout<<"---------------------"<<endl;}
    
    //And the LAPPD data here,see argument 2
    BoostStore *LAPPDData = new BoostStore(false,2);
    RawData->Get("LAPPDData",*LAPPDData);
    if(m_verbose>1){LAPPDData->Print(false); cout<<"---------------------"<<endl;}
    
    //Give the PsecData object from the boost store to the local one
    long entries=0;
    LAPPDData->Header->Get("TotalEntries",entries);
    cout << "Working on partfile " << PartNumber << " has " << entries << " entries"<< endl;
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

    LAPPDData->Close();
    delete LAPPDData;
    LAPPDData = 0;
    RawData->Close();
    delete RawData;
    RawData = 0;
    
    PartNumber++;

    return true;
}


bool ReadFile::Finalise()
{

    return true;
}
