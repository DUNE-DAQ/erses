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
#include <boost/crc.hpp>

#include "cpr/cpr.h"

ERS_REGISTER_OUTPUT_STREAM( erses::ESStream, "erses", url )


/** Constructor that creates a new instance of the erses stream with the given configuration.
  * \param format elastic search connection string.
  */
erses::ESStream::ESStream( const std::string & url): url_(url), partition_("test")
{
  // keep URL for ES
}

void erses::ESStream::ers_to_json(const ers::Issue & issue, size_t chain, std::vector<nlohmann::json> & j_objs)
{
    nlohmann::json message;            
    message["partition"] = partition_.c_str();
    message["issue_name"] = issue.get_class_name() ;
    message["message"] = issue.message().c_str() ;
    message["severity"] = ers::to_string(issue.severity()) ;
    message["usecs_since_epoch"]=std::chrono::duration_cast<std::chrono::microseconds>(issue.ptime().time_since_epoch()).count() ;
    message["time"] = std::chrono::duration_cast<std::chrono::milliseconds>(issue.ptime().time_since_epoch()).count() ;
    //message["time"] = issue.time_t() ;

    
    message["qualifiers"] = issue.qualifiers() ;
    message["params"] = nlohmann::json::array({});
    for (auto p : issue.parameters()) {
        message["params"].push_back(p.first + ": " + p.second);
    }
    message["cwd"] = issue.context().cwd() ;
    message["file_name"] = issue.context().file_name() ;
    message["function_name"] = issue.context().function_name() ;
    message["host_name"] = issue.context().host_name() ;
    message["package_name"] = issue.context().package_name() ;
    message["user_name"] = issue.context().user_name() ;
    message["application_name"] = issue.context().application_name() ;
    message["user_id"] = issue.context().user_id() ;
    message["process_id"] = issue.context().process_id() ;
    message["thread_id"] = issue.context().thread_id() ;
    message["line_number"] = issue.context().line_number() ;
    message["chain"]=chain;

    if ( issue.cause() ) {
        ers_to_json(*issue.cause(), 2, j_objs) ; // fill it  
    }
    j_objs.push_back(message);
}


/** Write method 
  * \param issue issue to be sent.
  */
void erses::ESStream::write( const ers::Issue & issue )
{
    std::vector<nlohmann::json> j_objs;
    if (issue.cause() == nullptr) {
    	ers_to_json(issue, 0, j_objs);
    }
    else {
        ers_to_json(issue, 1, j_objs);
    }

    // build a unique hash for a group of nested issues
    std::ostringstream tmpstream(issue.message()) ;
    tmpstream << issue.context().process_id() << issue.time_t() << issue.context().application_name() << issue.context().host_name() << rand() ;
    std::string tmp = tmpstream.str() ;
    boost::crc_32_type crc32 ;
    crc32.process_bytes(tmp.c_str(), tmp.length());

    for (auto j : j_objs ) {
	j["group_hash"]=crc32.checksum();
	auto response = cpr::Post(cpr::Url{url}, cpr::Authentication{"duneonl", "xxxx"}, 
                                  cpr::Header{"Content-Type", "application/json" }, cpr::Body{j.dump()});
        std::cout << "Post result " << response.status_code << "\t" << response.text);
    }

    chained().write( issue ); 
}

