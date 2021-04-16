## erses - ERS output stream inserting ERS issues directly into elastic search
This ERS output stream implementation takes issues, transforms them into nlohmann::json objects and injects them directly into an elastic search instance. The instance used is specified as a parameter to the erses stream.
In order to use it the ERS output streams can be configured through the environment:

export  DUNEDAQ_ERS_STREAM_LIBS=erses

export DUNEDAQ_PARTITION=ChooseYourPartitionName

and then extending the DUNEDAQ_ERS_<STREAM> variables with *erses(dunedaqutilities/erses)*.

### Dependencies
this package depends on nlohmann_json and cpr. 
