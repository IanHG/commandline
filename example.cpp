#include "commandline.hpp"

int main(int argc, char* argv[])
{
   auto cc = commandline::parser()
      .option<std::string>("inputfile", "-i")
      .option<std::string>("outputfile", "-o")
      .parse(argc, argv)
      ;
   
   cc.list_options();
   cc.list_parsed();

   auto port = cc.get<std::string>("inputfile");
   auto lol = cc.get<std::string>("outputfile");
   std::cout << port << " " << lol << std::endl;

   return 0;
}
