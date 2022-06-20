#pragma once

#include <3ds.h>
#include <string>

Result verifyServer(const std::string rootUrl, const std::string expectedVersion);
Result downloadMessage(const std::string rootUrl, const std::string titleId);
Result uploadMessage(const std::string rootUrl, const std::string titleId, const u64 consoleHash);