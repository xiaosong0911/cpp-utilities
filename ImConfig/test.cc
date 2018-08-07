#include "ImConfig.h"

int main() {
    ImConfig::ImConfig ic;
    ic.loadFile("config.xml");
    printf("int: %d\n", ic.get<int>("int-demo"));
    printf("float: %g\n", ic.get<float>("float-demo"));
    printf("double: %lg\n", ic.get<double>("double-demo"));
    printf("string: %s\n", ic.get<std::string>("string-demo").c_str());
    ic.saveFile("config.xml");
}
