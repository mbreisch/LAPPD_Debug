#include "GetRunType.h"

GetRunType::GetRunType():Tool(){}


bool GetRunType::Initialise(std::string configfile, DataModel &data)
{

    if(configfile!="")  m_variables.Initialise(configfile);
    //m_variables.Print();

    m_data= &data;
    m_log= m_data->Log;

    if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
    if(!m_variables.Get("Path_Raw",Path_Raw)) Path_Raw="./";
    if(!m_variables.Get("RunNumber",RunNumber)) RunNumber= 1234;

    Path_Raw = Path_Raw+to_string(RunNumber)+"/";
    Prefix = "RAWDataR";


    return true;
}


bool GetRunType::Execute()
{   
    int partiter = 0;
    string File;
    while(true)
    {
        try
        {
            File =Path_Raw+Prefix+to_string(RunNumber)+"S0p"+to_string(partiter);
            std::ifstream chk_file(File);
            if(!chk_file.good())
            {
                chk_file.close();
                m_data->vars.Set("StopLoop",1);
                cout << "File does not exist: " << File << endl;
                return true;  
            }else
            {
                break;
            }  
        }catch(const std::exception& e)
        {
            partiter++;
            std::cerr << "File exist check failed! " << e.what() << '\n';
        }
    }

    //Create raw boost-stores, since they are nested the raw data file is loaded here+
    //See argument 0
    BoostStore *RawData = new BoostStore(false,0);
    BoostStore *RunInfo = new BoostStore(false,2);
    try
    {
        RawData->Initialise(File);
        if(m_verbose>1){RawData->Print(false); cout<<"---------------------"<<endl;}
        //And the LAPPD data here,see argument 2
        RawData->Get("m_format",*RunInfo);
        if(m_verbose>1){RunInfo->Print(false); cout<<"---------------------"<<endl;}    
    }catch(const std::exception& e)
    {
        std::cerr << "Store init failed! " << e.what() << '\n';
    }
    
    return true;
}


bool GetRunType::Finalise()
{

    return true;
}
