
#include "fl/Headers.h"

int main(int argc, char** argv){
    FL_IUNUSED(argc);
    FL_IUNUSED(argv);
    
    using namespace fl;
    
    Engine* hybrid = Console::hybrid();
    FL_LOG("Hybrid Demo: FuzzyLite Dataset");
    FL_LOG("===============================");
    FL_LOG(FldExporter().toString(hybrid));
    delete hybrid;
    
}