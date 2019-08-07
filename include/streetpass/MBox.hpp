#pragma once

#include <3ds/types.h>
#include <memory>

#include "streetpass/Box.hpp"
#include "streetpass/CecInbox.hpp"
#include "streetpass/CecOutbox.hpp"
#include "streetpass/MBoxIcon.hpp"
#include "streetpass/MBoxProgramId.hpp"
#include "streetpass/MBoxTitle.hpp"

extern "C" {
#include "3ds/services/cecd.h"
}

namespace Streetpass {

class MBox {
public:
    explicit MBox(u32 id);
    explicit MBox(const std::string& id);
    explicit MBox(const std::vector<u8>& buffer);
    MBox(std::unique_ptr<CecInbox> inbox, std::unique_ptr<CecOutbox> outbox,
         std::unique_ptr<MBoxIcon> icon, std::unique_ptr<MBoxProgramId> programId,
         std::unique_ptr<MBoxTitle> title, const std::vector<u8>& infoHeaderBuffer);
    ~MBox();

    Result Close();
    Result Open();
    Result Save();

    u32 ProgramId() const;
    u32 PrivateId() const;
    u8 Flag() const;
    u8 Flag2() const;
    std::array<u8, 32> HmacKey() const;
    CecTimestamp LastAccessed() const;
    CecTimestamp LastReceived() const;
    CecTimestamp UnknownTime() const;

    std::vector<u8> data() const;

    CecMBoxInfoHeader& Header();
    const CecMBoxInfoHeader& Header() const;

    CecInbox& Inbox();
    const CecInbox& Inbox() const;
    CecOutbox& Outbox();
    const CecOutbox& Outbox() const;


    MBoxIcon& BoxIcon();
    const MBoxIcon& BoxIcon() const;

    MBoxProgramId& BoxProgramId();
    const MBoxProgramId& BoxProgramId() const;

    MBoxTitle& BoxTitle();
    const MBoxTitle& BoxTitle() const;

private:
    u32 boxId;
    CecMBoxInfoHeader mboxInfoHeader;

    std::unique_ptr<CecInbox> inbox;
    std::unique_ptr<CecOutbox> outbox;

    std::unique_ptr<MBoxIcon> mboxIcon;
    std::unique_ptr<MBoxProgramId> mboxProgramId;
    std::unique_ptr<MBoxTitle> mboxTitle;

};

} // namespace Streetpass
