#include "tests/functions/GetCecdState.hpp"

extern "C" {
#include "3ds/services/cecd.h"
}

namespace Tests {

GetCecdState::GetCecdState(const std::string& filepath) : TestBase(filepath) {
}

GetCecdState::~GetCecdState() {
    log.close();
}

void GetCecdState::RunTest() {
    log << "Starting Test" << std::endl;

    Result res;
    u32 state = 0;
    
    res = cecdGetCecdState(&state);
    log << "  result: " << std::hex << res << " state: " << state << std::endl;

    log << "Test Finished" << std::endl;
}

} // namespace Tests
