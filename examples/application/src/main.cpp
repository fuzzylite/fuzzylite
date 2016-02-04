
#include "fl/Headers.h"

int main(int argc, char** argv){
    using namespace fl;
    
    Engine* mamdani = Console::mamdani();
    FL_LOG("Mamdani Demo: FuzzyLite Dataset");
    FL_LOG("===============================");
    FL_LOG(FldExporter().toString(mamdani));
    delete mamdani;
    
}