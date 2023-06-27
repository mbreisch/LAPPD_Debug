#include "CheckFrame.h"

CheckFrame::CheckFrame():Tool(){}


bool CheckFrame::Initialise(std::string configfile, DataModel &data)
{
    if(configfile!="")  m_variables.Initialise(configfile);
    //m_variables.Print();

    m_data= &data;
    m_log= m_data->Log;

    if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;

    LAPPD_ID = -1;
    m_variables.Get("LAPPD_ID",LAPPD_ID);
    m_variables.Get("Port_0",Port_0);
    m_variables.Get("Port_1",Port_1);
    LAPPD_on_ACC = {Port_0,Port_1};

    PreviousBuffer.insert(pair<int, unsigned short>(Port_0, 0));
    PreviousBuffer.insert(pair<int, unsigned short>(Port_1, 0));

    return true;
}


bool CheckFrame::Execute()
{
    vector<unsigned short> TMP_ACC = m_data->GlobalPsec.AccInfoFrame;
    std::cout << "Current values on port " << Port_0 << ": " << TMP_ACC[16+Port_0] << " and on port " << Port_1 << ": " << TMP_ACC[16+Port_1] << std::endl;
    if(PreviousBuffer[Port_0]==0 && PreviousBuffer[Port_1]==0)
    {
        PreviousBuffer[Port_0] = TMP_ACC[16+Port_0];
        PreviousBuffer[Port_1] = TMP_ACC[16+Port_1];
    }else
    {
        if(PreviousBuffer[Port_0]==TMP_ACC[16+Port_0] && PreviousBuffer[Port_1]==TMP_ACC[16+Port_1])
        {
            unsigned int dumpIndex = 0x00 | (1<<LAPPD_on_ACC[0]) | (1<<LAPPD_on_ACC[1]);
            printf("Found 404 stop, dumping 0x%02x\n",dumpIndex);
            m_data->GlobalPsec.Stop = false;
        }else
        {
            PreviousBuffer[Port_0] = TMP_ACC[16+Port_0];
            PreviousBuffer[Port_1] = TMP_ACC[16+Port_1];
        }
    }
    TMP_ACC.clear();

    usleep(1000000);

    return true;
}


bool CheckFrame::Finalise()
{
    return true;
}
