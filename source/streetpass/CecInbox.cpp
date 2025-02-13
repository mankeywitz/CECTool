#include <cstdio>
#include "streetpass/CecInbox.hpp"

namespace Streetpass {

CecInbox::CecInbox(u32 id, std::unique_ptr<BoxInfo> inboxInfo) : boxId(id),
                   boxInfo(std::move(inboxInfo)), messages() {
    if (boxInfo->NumberOfMessages() > 0) {
        for (auto messageHeader : boxInfo->MessageHeaders()) {
            const u32 messageSize = messageHeader.messageSize;
            std::vector<u8> messageBuffer(messageSize);
            Result res = cecdReadMessage(boxId, false, messageHeader.messageId, sizeof(CecMessageId), nullptr,
                                           messageBuffer.data(), messageSize);
            if (R_FAILED(res)) {
                printf("Inbox ReadMessage failed: %lX\n", res);
            } else {
                messages.emplace_back(messageBuffer);
            }
        }
    }
    //printf("    boxInfo->NumberOfMessages: %lx\n", boxInfo->NumberOfMessages());
}

CecInbox::CecInbox(u32 id, std::unique_ptr<BoxInfo> inboxInfo, const std::vector<Message>& messages)
    : boxId(id), boxInfo(std::move(inboxInfo)), messages(messages) {
}

CecInbox::CecInbox(u32 id) : boxId(id), messages() {
    u32 inboxInfoSize = 0;
    Result res = cecdOpen(id, CEC_PATH_INBOX_INFO, CEC_READ, &inboxInfoSize);
    if (R_FAILED(res)) {
        printf("Inbox BoxInfo_____ Open failed: %lX\n", res);
        boxInfo = std::make_unique<BoxInfo>();
    } else {
        std::vector<u8> inboxInfoBuffer(inboxInfoSize);
        res = cecdOpenAndRead(inboxInfoBuffer.data(), inboxInfoSize, nullptr, id,
                              CEC_PATH_INBOX_INFO, CEC_READ | CEC_CHECK);

        if (R_FAILED(res)) {
            printf("Inbox BoxInfo_____ OpenAndRead failed: %lX\n", res);
            boxInfo = std::make_unique<BoxInfo>();
        } else {
            boxInfo = std::make_unique<BoxInfo>(inboxInfoBuffer);
        }
    }
}

CecInbox::~CecInbox() {}

Result CecInbox::AddMessage(const Message& message) {
    const auto& messageHeader = message.MessageHeader();
    const CecMessageId messageId = message.MessageId();
    const u32 messageSize = message.MessageSize();

    Result res = cecdWriteMessage(boxId, false, messageId.data, sizeof(CecMessageId),
                                  message.data().data(), messageSize);
    if (R_FAILED(res)) {
        printf("AddMessage WriteMessage failed: %lX\n", res);
        return res;
    }

    boxInfo->AddMessageHeader(messageHeader);
    return 0;
}

Result CecInbox::DeleteMessage(const CecMessageId& messageId) {
    Result res = cecdDelete(boxId, CEC_PATH_INBOX_MSG, false, messageId.data, sizeof(CecMessageId));
    if (R_FAILED(res)) {
        printf("Message Delete failed: %lX\n", res);
        return res;
    }

    boxInfo->DeleteMessageHeader(messageId);
    return 0;
}

Result CecInbox::DeleteAllMessages() {
    boxInfo->DeleteAllMessageHeaders();
    return 0;
}

BoxInfo& CecInbox::Info() {
    return *boxInfo;
}

const BoxInfo& CecInbox::Info() const {
    return *boxInfo;
}

std::vector<Message> CecInbox::Messages() {
    return messages;
}

const std::vector<Message> CecInbox::Messages() const {
    return messages;
}

} // namespace Streetpass
