#include <cstdio>
#include <cstring>
#include <fstream>
#include <sys/stat.h>

#include "common/http.hpp"
#include "common/STDirectory.hpp"

const u64 TIMEOUT = 5000000000;

Result verifyServer(const std::string rootUrl, const std::string expectedVersion) {
    const u32 BUFSIZE = 0x1000;
    const std::string url = rootUrl + "version";
    Result ret = 0;
    httpcContext context;
    u32 statuscode = 0;
    u8 buffer[BUFSIZE];
    u32 returnedSize = 0;


    printf("Pinging HTTP Server\n");
    printf("%s\n", url.c_str());
    memset(buffer, 0, BUFSIZE);

    ret = httpcOpenContext(&context, HTTPC_METHOD_GET, url.c_str(), 0);
    if(R_FAILED(ret)) {
        printf("Context creation failed %lX\n", ret);
        return ret;
    }

    ret = httpcSetSSLOpt(&context, SSLCOPT_DisableVerify);
    if(R_FAILED(ret)) {
        printf("Disable SSL verify failed %lX\n", ret);
        return ret;
    }

    ret = httpcSetKeepAlive(&context, HTTPC_KEEPALIVE_ENABLED);
    if(R_FAILED(ret)) {
        printf("Set keep-alive failed %lX\n", ret);
        return ret;
    }

    ret = httpcAddRequestHeaderField(&context, "User-Agent", "streetpass-client/1.0.0");
    if(R_FAILED(ret)) {
        printf("User Agent Header failed %lX\n", ret);
        return ret;
    }

    ret = httpcAddRequestHeaderField(&context, "Connection", "Keep-Alive");
    if(R_FAILED(ret)) {
        printf("Keep Alive Header failed %lX\n", ret);
        return ret;
    }

    ret = httpcBeginRequest(&context);
    if(R_FAILED(ret)) {
        printf("Begin request failed %lX\n", ret);
        httpcCloseContext(&context);
        return ret;
    }

    ret = httpcGetResponseStatusCodeTimeout(&context, &statuscode, TIMEOUT);
    if(R_FAILED(ret)) {
        printf("Getting status code failed %lX\n", ret);
        httpcCloseContext(&context);
        return ret;
    }

    printf("Status Code is %lu\n", statuscode);

    ret = httpcDownloadData(&context, buffer, BUFSIZE, &returnedSize);
    if(R_FAILED(ret)) {
        printf("Getting downloaded data failed %lX\n", ret);
        return ret;
    }

    std::string srvVersion((char*)buffer);

    httpcCloseContext(&context);

    if (srvVersion != expectedVersion) {
        printf("Error, incorrect server version\n");
        printf("Expected: %s, Actual: %s", expectedVersion.c_str(), srvVersion.c_str());
        ret = -1;
    } else {
        printf("Server verified!\n");
    }

    return ret;
}

Result downloadMessage(const std::string rootUrl, const std::string titleId, const u64 consoleHash) {
    const u32 BUFSIZE = 0x100000;
    const std::string url = rootUrl + titleId + "/download";
    const std::string downloadDir = "/3ds/CECTool/import/streetpasses/" + titleId;
    Result ret = 0;
    httpcContext context;
    u32 statuscode = 0;
    u8* buffer = (u8*)malloc(BUFSIZE);
    u32 returnedSize = 0;
    char hash_str[16];
    snprintf(hash_str, 16, "%llx", consoleHash);


    printf("Pinging HTTP Server\n");
    printf("%s\n", url.c_str());
    memset(buffer, 0, BUFSIZE);
    mkdir(downloadDir.c_str(), 777);

    ret = httpcOpenContext(&context, HTTPC_METHOD_GET, url.c_str(), 0);
    if(R_FAILED(ret)) {
        printf("Context creation failed %lX\n", ret);
        return ret;
    }

    ret = httpcSetSSLOpt(&context, SSLCOPT_DisableVerify);
    if(R_FAILED(ret)) {
        printf("Disable SSL verify failed %lX\n", ret);
        return ret;
    }

    ret = httpcSetKeepAlive(&context, HTTPC_KEEPALIVE_ENABLED);
    if(R_FAILED(ret)) {
        printf("Set keep-alive failed %lX\n", ret);
        return ret;
    }

    ret = httpcAddRequestHeaderField(&context, "User-Agent", "streetpass-client/1.0.0");
    if(R_FAILED(ret)) {
        printf("User Agent Header failed %lX\n", ret);
        return ret;
    }

    ret = httpcAddRequestHeaderField(&context, "Connection", "Keep-Alive");
    if(R_FAILED(ret)) {
        printf("Keep Alive Header failed %lX\n", ret);
        return ret;
    }

    ret = httpcAddRequestHeaderField(&context, "3ds-id", hash_str);
    if(R_FAILED(ret)) {
        printf("3ds ID header failed %lX\n", ret);
        return ret;
    }

    ret = httpcBeginRequest(&context);
    if(R_FAILED(ret)) {
        printf("Begin request failed %lX\n", ret);
        httpcCloseContext(&context);
        return ret;
    }

    ret = httpcGetResponseStatusCodeTimeout(&context, &statuscode, TIMEOUT);
    if(R_FAILED(ret)) {
        printf("Getting status code failed %lX\n", ret);
        httpcCloseContext(&context);
        return ret;
    }

    printf("Status Code is %lu\n", statuscode);

    ret = httpcDownloadData(&context, buffer, BUFSIZE, &returnedSize);
    if(R_FAILED(ret)) {
        printf("Size %lu", returnedSize);
        printf("Getting downloaded data failed %lX\n", ret);
        return ret;
    }

    httpcCloseContext(&context);

    printf("Download size is %lu", returnedSize);

    printf("Saving file...\n");
    std::ofstream out(downloadDir + "/download.bin", std::ios::out | std::ios::binary);
    out.write(reinterpret_cast<const char*>(buffer), returnedSize);
    out.close();

    printf("File saved.\n");
    free(buffer);

    return ret;
}

Result uploadMessage(const std::string rootUrl, const std::string titleId, const std::string fileName, const u64 consoleHash) {
    char hash_str[16];
    snprintf(hash_str, 16, "%llx", consoleHash);
    const std::string url = rootUrl + titleId + "/upload/" + fileName;
    const std::string uploadFilePath = "/3ds/CECTool/export/streetpasses/" + titleId + "/outbox/" + fileName;
    std::ifstream uploadFile(uploadFilePath, std::ios::in | std::ios::binary);
    Result ret = 0;
    httpcContext context;
    u32 statuscode = 0;

    uploadFile.seekg(0, uploadFile.end);
    int fSize = uploadFile.tellg();
    uploadFile.seekg(0, uploadFile.beg);

    char * uploadBuffer = new char [fSize];

    uploadFile.read(uploadBuffer, fSize);
    uploadFile.close();

    printf("Done reading file\n");

    printf("Pinging HTTP Server\n");
    printf("%s\n", url.c_str());

    ret = httpcOpenContext(&context, HTTPC_METHOD_POST, url.c_str(), 0);
    if(R_FAILED(ret)) {
        printf("Context creation failed %lX\n", ret);
        return ret;
    }

    ret = httpcSetSSLOpt(&context, SSLCOPT_DisableVerify);
    if(R_FAILED(ret)) {
        printf("Disable SSL verify failed %lX\n", ret);
        return ret;
    }

    ret = httpcSetKeepAlive(&context, HTTPC_KEEPALIVE_ENABLED);
    if(R_FAILED(ret)) {
        printf("Set keep-alive failed %lX\n", ret);
        return ret;
    }

    ret = httpcAddRequestHeaderField(&context, "User-Agent", "streetpass-client/1.0.0");
    if(R_FAILED(ret)) {
        printf("User Agent Header failed %lX\n", ret);
        return ret;
    }

    ret = httpcAddRequestHeaderField(&context, "Connection", "Keep-Alive");
    if(R_FAILED(ret)) {
        printf("Keep Alive Header failed %lX\n", ret);
        return ret;
    }

    ret = httpcAddRequestHeaderField(&context, "3ds-id", hash_str);
    if(R_FAILED(ret)) {
        printf("3ds ID header failed %lX\n", ret);
        return ret;
    }

    ret = httpcAddPostDataRaw(&context, (const u32*)uploadBuffer, fSize);
    if(R_FAILED(ret)) {
        printf("Adding post data failed %lX\n", ret);
        return ret;
    }

    ret = httpcBeginRequest(&context);
    if(R_FAILED(ret)) {
        printf("Begin request failed %lX\n", ret);
        httpcCloseContext(&context);
        return ret;
    }

    ret = httpcGetResponseStatusCode(&context, &statuscode);
    if(R_FAILED(ret)) {
        printf("Getting status code failed %lX\n", ret);
        printf("%i", RD_TIMEOUT);
        httpcCloseContext(&context);
        return ret;
    }

    printf("Status Code is %lu\n", statuscode);

    httpcCloseContext(&context);

    printf("Upload completed.\n");

    delete[] uploadBuffer;

    return ret;
}

Result uploadAllMessages(const std::string rootUrl, const u64 consoleHash, Streetpass::StreetpassManager& sm) {
    printf("Uploading messages...\n");
    const std::string rootFilePath = "/3ds/CECTool/export/streetpasses/";

    const std::vector<std::string> boxNames = sm.BoxList().BoxNames();
    
    for(u32 i = 0; i < sm.BoxList().NumberOfSlotsUsed(); i++) {
        printf("Processing %s \n", boxNames[i].c_str());

        const std::string uploadFolderPath = rootFilePath + boxNames[i] + "/outbox/";

        std::unique_ptr<STDirectory> uploadFolder =
                std::make_unique<STDirectory>(uploadFolderPath);

        const size_t numFiles = uploadFolder->count();

        for (size_t j = 0; j < numFiles; j++) {
            const std::string fileName = uploadFolder->item(j);

            printf("Uploading %s \n", fileName.c_str());

            uploadMessage(rootUrl, boxNames[i], fileName, consoleHash);
        }
    }
    return 0;
}