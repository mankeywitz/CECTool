#include "tests/functions/Open.hpp"

extern "C" {
#include "3ds/services/cecd.h"
}

namespace Tests {

Open::Open(const std::string& filepath) : TestBase(filepath) {

}

Open::~Open() {
    log.close();
}

void Open::RunTest() {
    log << "Starting Test" << std::endl;
    
    Result res;
    u32 fileSize = 0;
    u32 programId = 0x00000000;

    log << "Open CEC_PATH_ROOT_DIR:CEC_READ Test" << std::endl;

    res = CECDU_Open(programId, CEC_PATH_ROOT_DIR, CEC_READ, &fileSize);
    log << "   programId: " << std::hex << programId << " result: " << res << std::dec
        << " fileSize: " << fileSize << std::endl;

    fileSize = 0;
    programId = 0xFFFFFFFF;
    res = CECDU_Open(programId, CEC_PATH_ROOT_DIR, CEC_READ, &fileSize);
    log << "   programId: " << std::hex << programId << " result: " << res << std::dec
        << " fileSize: " << fileSize << std::endl;

    log << "Open CEC_PATH_ROOT_DIR:CEC_WRITE Test" << std::endl;
    
    fileSize = 0;
    programId = 0x00000000;
    res = CECDU_Open(programId, CEC_PATH_ROOT_DIR, CEC_WRITE, &fileSize);
    log << "   programId: " << std::hex << programId << " result: " << res << std::dec
        << " fileSize: " << fileSize << std::endl;

    fileSize = 0;
    programId = 0xFFFFFFFF;
    res = CECDU_Open(programId, CEC_PATH_ROOT_DIR, CEC_WRITE, &fileSize);
    log << "   programId: " << std::hex << programId << " result: " << res << std::dec
        << " fileSize: " << fileSize << std::endl;

    log << "Open CEC_PATH_ROOT_DIR:CEC_CREATE Test" << std::endl;
    
    fileSize = 0;
    programId = 0x00000000;
    res = CECDU_Open(programId, CEC_PATH_ROOT_DIR, CEC_CREATE, &fileSize);
    log << "   programId: " << std::hex << programId << " result: " << res
        << " fileSize: " << fileSize << std::endl;

    fileSize = 0;
    programId = 0xFFFFFFFF;
    res = CECDU_Open(programId, CEC_PATH_ROOT_DIR, CEC_CREATE, &fileSize);
    log << "   programId: " << std::hex << programId << " result: " << res
        << " fileSize: " << fileSize << std::endl;

    log << "Open CEC_PATH_ROOT_DIR:CEC_CHECK Test" << std::endl;
    
    fileSize = 0;
    programId = 0x00000000;
    res = CECDU_Open(programId, CEC_PATH_ROOT_DIR, CEC_CHECK, &fileSize);
    log << "   programId: " << std::hex << programId << " result: " << res
        << " fileSize: " << fileSize << std::endl;

    fileSize = 0;
    programId = 0xFFFFFFFF;
    res = CECDU_Open(programId, CEC_PATH_ROOT_DIR, CEC_CHECK, &fileSize);
    log << "   programId: " << std::hex << programId << " result: " << res
        << " fileSize: " << fileSize << std::endl;

    log << "Open CEC_PATH_ROOT_DIR:openFlag Range Test" << std::endl;
    
    programId = 0x00000000;
    log << "  programId: " << std::hex << programId << std::endl;
    
    for (u32 openFlag = 0; openFlag < 128; openFlag++) {
        fileSize = 0;
        res = CECDU_Open(programId, CEC_PATH_ROOT_DIR, openFlag, &fileSize);
        log << "    openFlag: " << openFlag << " result: " << res << " fileSize: " << fileSize << std::endl;
    }

    programId = 0xFFFFFFFF;
    log << "  programId: " << std::hex << programId << std::endl;

    for (u32 openFlag = 0; openFlag < 128; openFlag++) {
        fileSize = 0;
        res = CECDU_Open(programId, CEC_PATH_ROOT_DIR, openFlag, &fileSize);
        log << "    openFlag: " << openFlag << " result: " << res << " fileSize: " << fileSize << std::endl;
    }

    log << "Test Finished" << std::endl;
}

} // namespace Tests
