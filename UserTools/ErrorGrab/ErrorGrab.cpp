#include "ErrorGrab.h"

ErrorGrab::ErrorGrab():Tool(){}


bool ErrorGrab::Initialise(std::string configfile, DataModel &data)
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


bool ErrorGrab::Execute()
{
    string outpath = m_data->Path_Out + "ErrorCodes_L"+ to_string(LAPPDID)+".txt";
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

        if(m_verbose>1){cout<<"Run "<< m_data->RunNumber << " : Errorcodes start ... ";}
        for(std::map<int, PsecData>::iterator it=tmpMap.begin(); it!=tmpMap.end(); ++it)
        {
            vector<unsigned int> errors = it->second.errorcodes;

            if(errors.size()==0)
            {
                //skip
            }else if(errors.size()==1)
            {
                if(errors.at(0)==0x00000000)
                {
                    //skip
                }else
                {
                    outfile << "Event " << it->first << " has " << it->second.FailedReadCounter << " failed reads!" << endl;
                    outfile << std::hex << errors.at(0) << std::dec << endl;
                    outfile << " --------- " << endl;
                }
            }else
            {
                outfile << "Event " << it->first << endl;
                    
                for(unsigned int ierr: errors)
                {
                    outfile << std::hex << ierr << std::dec << endl;
                }
                outfile << " --------- " << endl;
            }
        }
        if(m_verbose>1){cout<<"Done!!"<<endl;}
    } catch (std::exception& e){
        std::cerr<<"Execute caught exception "<<e.what()<<std::endl;
        return false;
    }
    outfile.close();

    std::cout << "-------------------------" << std::endl;

    return true;
}


bool ErrorGrab::Finalise()
{    
    return true;
}
