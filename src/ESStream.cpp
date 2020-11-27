/**
* @file ESStream.cpp ESStream class implementation
* 
* This is part of the DUNE DAQ software, copyright 2020.
* Licensing/copyright details are in the COPYING file that you should have
* received with this code.
*/

#include "ESStream.hpp"
#include <ers/StreamFactory.h>
#include <string>
#include <iostream>
#include <chrono>

ERS_REGISTER_OUTPUT_STREAM( erses::ESStream, "erses", uri )


/** Constructor that creates a new instance of the erses stream with the given configuration.
  * \param format elastic search connection string.
  */
erses::ESStream::ESStream( const std::string & uri): uri_(uri), partition_("test")
{
  // keep URL for ES
}

void erses::ESStream::ers_to_json(const ers::Issue & issue, nlohmann::json & message)
{
ERS_DEBUG(3, "Building message: " << issue.get_class_name()) ;
        message["partition"] = partition_.c_str();
        message["issue_name"] = issue.get_class_name() ;
        message["message"] = issue.message().c_str() ;
        message["severity"] = ers::to_string(issue.severity()) ;
        message["time"] = std::chrono::duration_cast<std::chrono::microseconds>(issue.ptime().time_since_epoch()).count() ;

        message["qualifiers"]=issue.qualifiers();
	message["params"]=issue.parameters();

	nlohmann::json cont;
        cont["cwd"] = issue.context().cwd() ;
        cont["file_name"] = issue.context().file_name() ;
        cont["function_name"] = issue.context().function_name() ;
        cont["host_name"] = issue.context().host_name() ;
        cont["package_name"] = issue.context().package_name() ;
        cont["user_name"] = issue.context().user_name() ;
        cont["application_name"] = issue.context().application_name() ;
        cont["user_id"] = issue.context().user_id() ;
        cont["process_id"] = issue.context().process_id() ;
        cont["thread_id"] = issue.context().thread_id() ;
        cont["line_number"] = issue.context().line_number() ;

	message["context"]=cont;

        if ( issue.cause() )
                {
		nlohmann::json cause;
                ers_to_json(*issue.cause(), cause) ; // fill it  
		message["cause"]=cause;
                }

}


/** Write method 
  * \param issue issue to be sent.
  */
void
erses::ESStream::write( const ers::Issue & issue )
{
   nlohmann::json j;
   ers_to_json(issue, j);
   std::cout << j.dump(3) << std::endl;
   chained().write( issue ); 
} // send

