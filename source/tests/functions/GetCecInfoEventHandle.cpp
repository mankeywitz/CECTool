#include "tests/functions/GetCecInfoEventHandle.hpp"

extern "C" {
#include "3ds/services/cecd.h"
}

namespace Tests {

GetCecInfoEventHandle::GetCecInfoEventHandle(const std::string& filepath) : TestBase(filepath) {

}

GetCecInfoEventHandle::~GetCecInfoEventHandle() {
    log.close();
}

void GetCecInfoEventHandle::RunTest() {
    log << "Starting Test" << std::endl;

    Result res;
    Handle handle = 0;
    
    res = CECDU_GetCecInfoEventHandle(&handle);
    log << "  result: " << std::hex << res << " handle: " << handle << std::endl;

    log << "Test Finished" << std::endl;
}

} // namespace Tests
