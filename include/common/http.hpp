#pragma once

#include <3ds.h>
#include <string>
#include "streetpass/StreetpassManager.hpp"

Result verifyServer(const std::string rootUrl, const std::string expectedVersion);
Result downloadMessage(const std::string rootUrl, const std::string titleId, const u64 consoleHash);
Result uploadMessage(const std::string rootUrl, const std::string titleId, const std::string fileName, const u64 consoleHash);
Result uploadMessageFromData(const std::string rootUrl, const std::string titleId, const std::string fileName, const std::vector<u8> fileData, const u64 consoleHash);
Result uploadAllMessages(const std::string rootUrl, const u64 consoleHash, Streetpass::StreetpassManager& sm);