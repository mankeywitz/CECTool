#include "tests/functions/GetChangeStateEventHandle.hpp"

extern "C" {
#include "3ds/services/cecd.h"
}

namespace Tests {

GetChangeStateEventHandle::GetChangeStateEventHandle(const std::string& filepath) : TestBase(filepath) {
}

GetChangeStateEventHandle::~GetChangeStateEventHandle() {
    log.close();
}

void GetChangeStateEventHandle::RunTest() {
    log << "Starting Test" << std::endl;

    Result res;
    Handle handle = 0;
    
    res = cecdGetChangeStateEventHandle(&handle);
    log << "  result: " << std::hex << res << " handle: " << handle << std::endl;

    log << "Test Finished" << std::endl;
}

} // namespace Tests
