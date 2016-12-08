#ifndef COMMANDLINE_HPP_INCLUDED_
#define COMMANDLINE_HPP_INCLUDED_

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>

#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include <exception>

#include "fromstring.hpp"

namespace commandline
{
   template<class T>
   inline std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
   {
      os << "(";
      if(v.size() > 0)
      {
         for(size_t i = 0; i < v.size() - 1; ++i)
         {
            os << v[i] << ",";
         }
         os << v[v.size() - 1]; 
      }
      os << ")";
      return os;
   }

   inline std::ostream& operator<<(std::ostream& os, const std::type_index& t)
   {
      os << t.name();
      return os;
   }
   
   /*!
    *
    */
   struct parser_option
   {
      parser_option
         ( const std::string& name
         , const std::string& option
         , const std::vector<std::type_index>& type
         )
         : name_(name)
         , option_(option)
         , type_(type)
      {
      }

      std::string name_;
      std::string option_;
      std::vector<std::type_index> type_;
   };

   std::ostream& operator<<(std::ostream& os, const parser_option& o)
   {
      os << o.name_ << " " << o.option_;
      return os;
   }
   
   /*!
    *
    */
   struct parser_value
   {
      std::vector<std::type_index> type_;
      std::vector<std::string> value_;
   };

   std::ostream& operator<<(std::ostream& os, const parser_value& v)
   {
      os << v.type_ << " " << v.value_;
      return os;
   }

   /*!
    *
    */
   struct parser_type
   {
      int minarg_ = 0;
      std::list<parser_option> options_;
      std::map<std::string, parser_value> parsed_;
      
      parser_type& minarg(int min)
      {
         minarg_ = min;
         return *this;
      }
         
      //! add parser option
      template<class... T>
      parser_type& option
         ( const std::string& name
         , const std::string& option
         )
      {
         options_.emplace_back(parser_option(name, option, {std::type_index(typeid(T))...}));
         return *this;
      }
      
      //! get parsed value
      template<class T>
      T get
         ( const std::string& name
         , std::size_t iarg = 0
         )
      {
         auto val = parsed_.find(name);
         if(val == parsed_.end())
         {
            throw std::runtime_error(" Did not find input with name : " + name);
         }
         if(iarg >= val->second.type_.size())
         {
            throw std::runtime_error(" Does not have " + std::to_string(iarg) + " values for input with name :" + name);
         }
         
         return from_string<T>(val->second.value_[iarg]);
      }

      //!
      bool has
         ( const std::string& name
         , std::size_t iarg = 0
         )
      {
         auto val = parsed_.find(name);
         if(val == parsed_.end())
         {
            return false;
         }
         return true;
      }

      //! parse the commandline input
      parser_type& parse(int argc, char* argv[])
      {
         for(int iargc = 1; iargc < argc; ++iargc)
         {
            bool found = false;
            for(const auto& option : options_)
            {
               if(argv[iargc] == option.option_)
               {
                  found = true;
                  auto narg = option.type_.size();
                  parser_value val;
                  for(std::size_t i = 0; i < narg; ++i)
                  {
                     ++iargc;
                     for(const auto option_check : options_)
                     {
                        if(argv[iargc] == option_check.option_) throw std::runtime_error("passed another option to '" + option.option_ + "'.");
                     }
                     val.type_.emplace_back(option.type_[i]);
                     val.value_.emplace_back(argv[iargc]);
                  }
                  parsed_.emplace(option.name_, val);
                  break;
               }
            }
            if(!found)
            {
               std::cout << " did not find option " << argv[iargc] << std::endl;
            }
         }
         //auto val = parsed_.find("help");
         return *this;
      }
      
      //!
      parser_type& list_options()
      {
         std::cout << " +++ OPTIONS +++ " << std::endl;
         for(const auto& x : options_)
         {
            std::cout << x << std::endl;
         }
         return *this;
      }
      
      //!
      parser_type& list_parsed()
      {
         std::cout << " +++ PARSED +++ " << std::endl;
         for(const auto& x : parsed_)
         {
            std::cout << x.first << " " << x.second << std::endl;
         }
         return *this;
      }
   };
   
   //! get a parser instance
   inline parser_type parser()
   {
      return parser_type();
   }
} /* namespace commandline */

#endif /* COMMANDLINE_HPP_INCLUDE_ */
