#include "Factory.h"

Tool* Factory(std::string tool) {
Tool* ret=0;

// if (tool=="Type") tool=new Type;
if (tool=="DummyTool") ret=new DummyTool;
if (tool=="ErrorGrab") ret=new ErrorGrab;
if (tool=="EvaluateFile") ret=new EvaluateFile;
if (tool=="GetBeamTime") ret=new GetBeamTime;
if (tool=="GetTimeStamp") ret=new GetTimeStamp;
if (tool=="ReadFile") ret=new ReadFile;
if (tool=="ReadMap") ret=new ReadMap;
if (tool=="SaveNewStore") ret=new SaveNewStore;
return ret;
}
