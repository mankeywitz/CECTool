#include "tests/functions/GetEventLogStart.hpp"

extern "C" {
#include "3ds/services/cecd.h"
}

namespace Tests {

GetEventLogStart::GetEventLogStart(const std::string& filepath) : TestBase(filepath) {

}

GetEventLogStart::~GetEventLogStart() {
    log.close();
}

void GetEventLogStart::RunTest() {
    log << "Starting Test" << std::endl;
    
    Result res;
    u32 start = 0;
    
    res = CECDU_GetEventLogStart(&start);
    log << "  result: " << std::hex << res << " start: " << start << std::endl;

    log << "Test Finished" << std::endl;
}

} // namespace Tests
