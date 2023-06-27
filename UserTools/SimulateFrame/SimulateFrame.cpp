#include "SimulateFrame.h"

SimulateFrame::SimulateFrame():Tool(){}


bool SimulateFrame::Initialise(std::string configfile, DataModel &data)
{
    if(configfile!="")  m_variables.Initialise(configfile);
    //m_variables.Print();

    m_data= &data;
    m_log= m_data->Log;

    if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
    m_variables.Get("Port_0",Port_0);
    m_variables.Get("Port_1",Port_1);

    loop = 0;
    m_data->GlobalPsec.AccInfoFrame = {0x1234,0xaaaa,0x301,0x2021,0x614,0,0,0x3,0x2,0,0,0,0x3,0,0,0,0,0,0,0,0,0,0,0,0xbbbb,0xbb00,0,0,0,0,0xaaaa,0x4321};
    m_data->GlobalPsec.Stop = false;

    return true;
}


bool SimulateFrame::Execute()
{
    tmp_frame = m_data->GlobalPsec.AccInfoFrame;

    if(m_data->GlobalPsec.Stop==false)
    {
        if(Random01()<0.005)
        {
            m_data->GlobalPsec.Stop = true;
        }else
        {
            float run_val = 7795*loop/10;
            if(run_val>=7795){loop=0;}
            tmp_frame.at(16+Port_0) = (int)run_val;
            tmp_frame.at(16+Port_1) = (int)run_val + Random01()*10;
            m_data->GlobalPsec.Stop = false;
        }
    }
    m_data->GlobalPsec.AccInfoFrame = tmp_frame;
    loop++;

    return true;
}


bool SimulateFrame::Finalise()
{
    return true;
}

float SimulateFrame::Random01(){
  auto dt = std::chrono::duration_cast<std::chrono::nanoseconds>(m_clock.now().time_since_epoch()).count();
  std::mt19937 generator((unsigned)dt);
  std::uniform_real_distribution<float> dis(0.0, 1.0);

  float randomRealBetweenZeroAndOne = dis(generator);

  return randomRealBetweenZeroAndOne;
}
