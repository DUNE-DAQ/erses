## erses - ERS output stream inserting ERS issues directly into elastic search
This ERS output stream implementation takes issues, transforms them into nlohmann::json objects and injects them directly into an elastic search instance. The instance used is specified as a parameter to the erses stream.
In order to use it the ERS output streams can be configured through the environment:
export TDAQ_ERS_ERROR="lstderr, erses(uri)"

### Dependencies
this package depends on nlohmann_json and Pistache. 
