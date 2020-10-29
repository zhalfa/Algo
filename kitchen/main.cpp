#include "unit_test.h"

#include "boost/program_options.hpp"

namespace po = boost::program_options;
namespace postyle = boost::program_options::command_line_style;

int main(int argc, char *argv[]){

    size_t rate = 0;
    string file;

    po::options_description desc("Options");
    desc.add_options()
        ("rate,R", po::value<size_t>(&rate)->default_value(2),
         "set how many orders kitchen receives per second" )
        ("file,f", po::value<std::string>(&file)->required(),
         "set json file as orders source")
        ("help,h","Print this help message");

    po::variables_map vm;

    try{
        po::store( po::command_line_parser(argc, argv).options(desc).style(postyle::unix_style).run(),vm );
        po::notify(vm);

        if (argc == 1 || vm.count("help") ){

            std::cout << "USAGE:  " << argv[0] << std::endl << desc << std::endl;
            return 0;
        }
    }catch (po::error& poe ) {

        std::cerr << poe.what() << std::endl;
        std::cerr<< "USAGE:  " << argv[0] << std::endl << desc << std::endl;
        return EXIT_FAILURE;
    }

}
