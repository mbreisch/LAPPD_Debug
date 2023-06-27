#include "Factory.h"

Tool* Factory(std::string tool) {
Tool* ret=0;

// if (tool=="Type") tool=new Type;
if (tool=="DummyTool") ret=new DummyTool;
if (tool=="EndAnalysis") ret=new EndAnalysis;
if (tool=="ErrorGrab") ret=new ErrorGrab;
if (tool=="EvaluateFile") ret=new EvaluateFile;
if (tool=="GetBeamTime") ret=new GetBeamTime;
if (tool=="GetEventInfos") ret=new GetEventInfos;
if (tool=="GetRunType") ret=new GetRunType;
if (tool=="GetTimeDifference") ret=new GetTimeDifference;
if (tool=="GetTimeStamp") ret=new GetTimeStamp;
if (tool=="ReadFile") ret=new ReadFile;
if (tool=="ReadMap") ret=new ReadMap;
if (tool=="SaveNewStore") ret=new SaveNewStore;
if (tool=="TimeCompare") ret=new TimeCompare;
if (tool=="TimeEvolution") ret=new TimeEvolution;
if (tool=="SimulateFrame") ret=new SimulateFrame;
	if (tool=="CheckFrame") ret=new CheckFrame;
return ret;
}
