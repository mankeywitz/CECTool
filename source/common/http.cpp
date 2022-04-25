#include <cstdio>
#include <cstring>
#include <fstream>
#include <sys/stat.h>

#include "common/http.hpp"

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

Result downloadMessage(const std::string rootUrl, const std::string titleId) {
    const u32 BUFSIZE = 0x100000;
    const std::string url = rootUrl + titleId + "/download";
    const std::string downloadDir = "/3ds/CECTool/import/streetpasses/" + titleId;
    Result ret = 0;
    httpcContext context;
    u32 statuscode = 0;
    u8* buffer = (u8*)malloc(BUFSIZE);
    u32 returnedSize = 0;


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

    return ret;
}