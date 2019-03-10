#include "ImConfig.h"

int main() {
    ImConfig::ImConfigFactory icf("config.xml");
    ImConfig::ImConfig ic = icf.createImConfig("demo");

    printf("int: %d\n", ic.get<int>("int-demo"));
    printf("float: %g\n", ic.get<float>("float-demo"));
    printf("double: %lg\n", ic.get<double>("double-demo"));
    printf("string: %s\n", ic.get<std::string>("string-demo").c_str());
}
