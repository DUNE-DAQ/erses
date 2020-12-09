
/** 
* @file ESStream.hpp This file defines ESStream ERS stream.
* This is part of the DUNE DAQ software, copyright 2020.
*  Licensing/copyright details are in the COPYING file that you should have
*  received with this code.
*
*/

#ifndef ERSES_ESSTREAM_HPP 
#define ERSES_ESSTREAM_HPP

#include <ers/OutputStream.h>
#include <nlohmann/json.hpp>
#include <pistache/client.h>
#include <string>
#include <vector>

namespace erses
{    
    /** This stream offers capability of publishing Issues to elastic search.
      * A stream configuration is composed of the stream name,
      * that is "erses". 
      * 
      * \brief ES stream implementation.
      */
    
    class ESStream : public ers::OutputStream
    {
      public:
	explicit ESStream( const std::string & url);
	
        void write( const ers::Issue & issue ) override;
        
      private:	
        std::string url_;
	std::string partition_;
	Pistache::Http::Client client_;
	void ers_to_json(const ers::Issue & issue, size_t chain, std::vector<nlohmann::json> & j_objs);
    };
} //namespace erses

#endif
