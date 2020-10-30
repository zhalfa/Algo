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
    
    kitchen kitchen_simulator;
    messageOutput log(true);

    courierDispatcher dispatcher(&kitchen_simulator);
    orderIngester ingester(&kitchen_simulator, &dispatcher);

    bool res = kitchen_simulator.prepareKitchen(&log);
    assert(res);

    if (res){

        kitchen_simulator.start();
        dispatcher.start();

        size_t orderCnt = 0;
        res = ingester.setFile(file);
        if (res){
            ingester.setRate(rate);
            orderCnt = ingester.run();

            while( !kitchen_simulator.isEmpty())
                boost::this_thread::sleep_for(boost::chrono::seconds(4));
        }

        kitchen_simulator.stop();

        dispatcher.stop();
        assert(dispatcher.isEmpty());

        size_t cnt = kitchen_simulator.getWasteCnt();
        std::cout << "ingested :" << orderCnt << std::endl;
        std::cout << "wasted   :" << cnt << std::endl;
        std::cout << "pickuped :" << log.m_pickuped << std::endl;
    }
    return 0;
}
