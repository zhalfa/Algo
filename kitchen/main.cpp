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

    std::ifstream ifile;

    ifile.open(file);
    if(ifile) {

        ifile.close();
    } else {
        std::cout<< file << " : file doesn't exist" << std::endl;
        return 0;
    }   
    
    kitchen kit_test;
    messageOutput log(true);

    courierDispatcher dispatcher(&kit_test);
    orderIngester ingester(&kit_test, &dispatcher);

    bool res = kit_test.prepareKitchen(&log);
    assert(res);

    if (res){

        kit_test.start();
        dispatcher.start();

        res = ingester.setFile(file);
        if (res){
            ingester.setRate(rate);
            ingester.run();

            while( !kit_test.isEmpty())
                boost::this_thread::sleep_for(boost::chrono::seconds(4));
        }

        kit_test.stop();

        dispatcher.stop();
        assert(dispatcher.isEmpty());

        size_t cnt = kit_test.getWasteCnt();
        std::cout << "wasted   :" << cnt << std::endl;
        std::cout << "pickuped :" << log.m_pickuped << std::endl;
    }
    return 0;
}
