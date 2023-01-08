#pragma once

#include <array>
#include <memory>

#include <3ds/types.h>

#include "streetpass/MBox.hpp"
#include "streetpass/MBoxList.hpp"

namespace Streetpass {

class StreetpassManager {
public:
    explicit StreetpassManager();
    ~StreetpassManager();

    Result HexDump(const std::vector<u8>& buffer) const;
    Result ListBoxes() const;

    std::shared_ptr<MBox> OpenBox(const u8 slotNum) const;

    Result CreateBox(const u32 boxId, const std::string& boxName, std::unique_ptr<MBox> mbox);
    Result DeleteBox(const u8 slotNum);
    Result DeleteAllBoxes();
    void BackupBox(const u8 slotNum);
    void ExportStreetpasses(const u8 slotNum);
    Result ImportBox(const u32 boxId);
    void ImportStreetpasses(const u32 boxId);

    MBoxList& BoxList();
    const MBoxList& BoxList() const;

    Result ReloadBoxList();

private:
    std::unique_ptr<MBoxList> mboxList;
    mutable std::array<std::shared_ptr<MBox>, 12> boxes;
};

} // namespace Streetpass
