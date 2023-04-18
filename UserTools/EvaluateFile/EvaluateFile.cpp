#include "EvaluateFile.h"

EvaluateFile::EvaluateFile():Tool(){}


bool EvaluateFile::Initialise(std::string configfile, DataModel &data)
{

    if(configfile!="")  m_variables.Initialise(configfile);
    //m_variables.Print();

    m_data= &data;
    m_log= m_data->Log;

    if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;

    Path = m_data->Path;
    cout <<Path<<endl;

    iter_0 = 0;
    iter_1 = 0;
    iter_2 = 0;

    return true;
}


bool EvaluateFile::Execute()
{
    if(m_data->SwitchToEval)
    {
        return true;
    }

    for(std::map<int, PsecData>::iterator it=m_data->PartFileMap.begin(); it!=m_data->PartFileMap.end(); ++it)
    {
        
        if(it->second.LAPPD_ID==0)
        {
            m_data->RAWLAPPD0.insert(std::pair<int,PsecData>(iter_0,it->second));
            iter_0++;
        }else if(it->second.LAPPD_ID==1)
        {
            m_data->RAWLAPPD1.insert(std::pair<int,PsecData>(iter_1,it->second));
            iter_1++;
        }else if(it->second.LAPPD_ID==2)
        {
            m_data->RAWLAPPD2.insert(std::pair<int,PsecData>(iter_2,it->second));
            iter_2++;
        }
    }
    cout << "L0 size " << iter_0 << " | L1 size " << iter_1 << " | L2 size " << iter_2 << endl;

    m_data->PartFileMap.clear();

    return true;
}


bool EvaluateFile::Finalise()
{

    return true;
}
