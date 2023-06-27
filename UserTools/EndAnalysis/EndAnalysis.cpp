#include "EndAnalysis.h"

EndAnalysis::EndAnalysis():Tool(){}


bool EndAnalysis::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;
  m_log= m_data->Log;

  if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;


  return true;
}


bool EndAnalysis::Execute(){

  return true;
}


bool EndAnalysis::Finalise()
{
    m_data->RootFile->Close();
    delete m_data->RootFile;
    cout<<"Run "<< m_data->RunNumber << " finished analysing!"<<endl;
    return true;
}
