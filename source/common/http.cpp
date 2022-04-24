#include <cstdio>

#include "common/http.hpp"

const u64 TIMEOUT = 5000000000;
const u32 BUFSIZE = 0x1000;

Result pingServer(const std::string url) {
    Result ret = 0;
    httpcContext context;
    u32 statuscode = 0;
    u8 buffer[BUFSIZ];
    u32 returnedSize = 0;


    printf("Pinging HTTP Server\n");
    printf("%s\n", url.c_str());


    printf("Creating context...\n");
    ret = httpcOpenContext(&context, HTTPC_METHOD_GET, url.c_str(), 1);
    if(R_FAILED(ret)) {
        printf("Context creation failed %lX\n", ret);
        return ret;
    }

    printf("Setting SSL Options...\n");
    ret = httpcSetSSLOpt(&context, SSLCOPT_DisableVerify);
    if(R_FAILED(ret)) {
        printf("Disable SSL verify failed %lX\n", ret);
        return ret;
    }

    printf("Setting Keep Alive...\n");
    ret = httpcSetKeepAlive(&context, HTTPC_KEEPALIVE_ENABLED);
    if(R_FAILED(ret)) {
        printf("Set keep-alive failed %lX\n", ret);
        return ret;
    }

    printf("Setting User-Agent...\n");
    ret = httpcAddRequestHeaderField(&context, "User-Agent", "streetpass-client/1.0.0");
    if(R_FAILED(ret)) {
        printf("User Agent Header failed %lX\n", ret);
        return ret;
    }

    printf("Setting Keep-Alive Header...\n");
    ret = httpcAddRequestHeaderField(&context, "Connection", "Keep-Alive");
    if(R_FAILED(ret)) {
        printf("Keep Alive Header failed %lX\n", ret);
        return ret;
    }

    printf("Starting Request....\n");
    ret = httpcBeginRequest(&context);
    if(R_FAILED(ret)) {
        printf("Begin request failed %lX\n", ret);
        httpcCloseContext(&context);
        return ret;
    }

    printf("Getting response code....\n");
    ret = httpcGetResponseStatusCodeTimeout(&context, &statuscode, TIMEOUT);
    if(R_FAILED(ret)) {
        printf("Getting status code failed %lX\n", ret);
        httpcCloseContext(&context);
        return ret;
    }

    printf("Status Code is %lu", statuscode);

    printf("Downloading data...\n");
    ret = httpcDownloadData(&context, buffer, BUFSIZ, &returnedSize);
    if(R_FAILED(ret)) {
        printf("Getting downloaded data failed %lX\n", ret);
        return ret;
    }

    printf("Downloaded data size is %lu\n", returnedSize);

    for(u32 i = 0; i < returnedSize; i++) {
        printf("%c", buffer[i]);
    }

    printf("\n");

    httpcCloseContext(&context);

    return ret;
}