#include "packet/PacketDumper.h"
#include <exl/diag/assert.hpp>
#include <exl/nx/kernel/svc.h>
#include <memory>
#include <nn/fs.h>
#include <nn/fs/fs_directories.h>

#include "Minecraft.Core/System.h"
#include "Minecraft.Core/io/DataOutputStream.h"
#include "Minecraft.Core/io/OutputStream.h"
#include "Minecraft.Network/protocol/Packet.h"
#include "Minecraft.World/ArrayWithLength.h"
#include "Minecraft.World/level/Level.h"
#include "mallow/hook/helpers.hpp"

#include <string>
#include <unordered_map>

class PacketTypeHelper {
public:
    static std::string GetPacketName(unsigned char packetId) {
        static const std::unordered_map<unsigned char, std::string> packetTypeMap = {
            {0, "ClientboundKeepAlivePacket"},
            {1, "ClientboundLoginPacket"},
            {2, "ClientboundPreLoginPacket"},
            {3, "ClientboundChatPacket"},
            {4, "ClientboundSetTimePacket"},
            {5, "ClientboundSetEquippedItemPacket"},
            {6, "ClientboundSetSpawnPositionPacket"},
            {7, "ServerboundInteractPacket"},
            {8, "ClientboundSetHealthPacket"},
            {9, "ClientboundRespawnPacket"},
            {10, "ServerboundMovePlayerPacket"},
            {11, "ServerboundMovePlayerPacketPos"},
            {12, "ServerboundMovePlayerPacketRot"},
            {13, "ServerboundMovePlayerPacketPosRot"},
            {14, "ServerboundPlayerActionPacket"},
            {15, "ServerboundUseItemPacket"},
            {16, "ClientboundSetCarriedItemPacket"},
            {17, "ClientboundPlayerSleepPacket"},
            {18, "ClientboundAnimatePacket"},
            {19, "ServerboundPlayerCommandPacket"},
            {20, "ClientboundAddPlayerPacket"},
            {22, "ClientboundTakeItemEntityPacket"},
            {23, "ClientboundAddEntityPacket"},
            {24, "ClientboundAddMobPacket"},
            {25, "ClientboundAddPaintingPacket"},
            {26, "ClientboundAddExperienceOrbPacket"},
            {27, "ServerboundPlayerInputPacket"},
            {28, "ClientboundSetEntityMotionPacket"},
            {29, "ClientboundRemoveEntitiesPacket"},
            {30, "ClientboundMoveEntityPacket"},
            {31, "ClientboundMoveEntityPacketPos"},
            {32, "ClientboundMoveEntityPacketRot"},
            {33, "ClientboundMoveEntityPacketPosRot"},
            {34, "ClientboundTeleportEntityPacket"},
            {35, "ClientboundRotateHeadPacket"},
            {38, "ClientboundEntityEventPacket"},
            {39, "ClientboundSetEntityLinkPacket"},
            {40, "ClientboundSetEntityDataPacket"},
            {41, "ClientboundUpdateMobEffectPacket"},
            {42, "ClientboundRemoveMobEffectPacket"},
            {43, "ClientboundSetExperiencePacket"},
            {44, "ClientboundUpdateAttributesPacket"},
            {50, "ChunkVisibilityPacket"},
            {51, "BlockRegionUpdatePacket"},
            {52, "ClientboundChunkBlocksUpdatePacket"},
            {53, "ClientboundBlockUpdatePacket"},
            {54, "ClientboundBlockEventPacket"},
            {55, "ClientboundBlockDestructionPacket"},
            {60, "ClientboundExplodePacket"},
            {61, "ClientboundLevelEventPacket"},
            {62, "ClientboundSoundPacket"},
            {63, "ClientboundLevelParticlesPacket"},
            {70, "ClientboundGameEventPacket"},
            {71, "ClientboundAddGlobalEntityPacket"},
            {100, "ClientboundContainerOpenPacket"},
            {101, "ClientboundContainerClosePacket"},
            {102, "ServerboundContainerClickPacket"},
            {103, "ClientboundContainerSetSlotPacket"},
            {104, "ClientboundContainerSetContentPacket"},
            {105, "ClientboundContainerSetDataPacket"},
            {106, "ClientboundContainerAckPacket"},
            {107, "ServerboundSetCreativeModeSlotPacket"},
            {108, "ServerboundContainerButtonClickPacket"},
            {132, "ClientboundBlockEntityDataPacket"},
            {133, "ClientboundMoveVehiclePacket"},
            {134, "ClientboundSetPassengersPacket"},
            {135, "ServerboundAcceptTeleportationPacket"},
            {136, "ServerboundMoveVehiclePacket"},
            {137, "ServerboundUseItemOnPacket"},
            {138, "ServerboundPaddleBoatPacket"},
            {139, "ClientboundBossEventPacket"},
            {150, "CraftItemPacket"},
            {151, "TradeItemPacket"},
            {152, "DebugOptionsPacket"},
            {153, "ServerSettingsChangedPacket"},
            {154, "TexturePacket"},
            {155, "ChunkVisibilityAreaPacket"},
            {156, "UpdateProgressPacket"},
            {157, "TextureChangePacket"},
            {158, "UpdateGameRuleProgressPacket"},
            {159, "KickPlayerPacket"},
            {160, "TextureAndGeometryPacket"},
            {161, "TextureAndGeometryChangePacket"},
            {162, "MoveEntityPacketSmall"},
            {163, "MoveEntityPacketSmallPos"},
            {164, "MoveEntityPacketSmallRot"},
            {165, "MoveEntityPacketSmallPosRot"},
            {166, "XZPacket"},
            {167, "GameCommandPacket"},
            {168, "MapSelectInfoPacket"},
            {169, "VotePacket"},
            {170, "PlayerReadyPacket"},
            {171, "ClientboundPowerupPacket"},
            {200, "ClientboundAwardStatPacket"},
            {201, "PlayerInfoPacket"},
            {202, "ClientboundPlayerAbilitiesPacket"},
            {205, "ServerboundClientCommandPacket"},
            {206, "ScoreboardPacket"},
            {209, "ClientboundSetPlayerTeamPacket"},
            {210, "GameModePacket"},
            {211, "ClientboundMapItemDataPacket"},
            {212, "ClientboundOpenSignEditorPacket"},
            {213, "ClientboundPlayerCombatPacket"},
            {214, "ClientboundPlayerPositionPacket"},
            {216, "ClientboundResourcePackPacket"},
            {217, "ClientboundSetBorderPacket"},
            {218, "ClientboundSetCameraPacket"},
            {219, "ClientboundSetTitlesPacket"},
            {220, "ClientboundTabListPacket"},
            {221, "ServerboundChatAutoCompletePacket"},
            {222, "ServerboundChatPacket"},
            {223, "ServerboundContainerAckPacket"},
            {224, "ServerboundContainerClosePacket"},
            {225, "ServerboundCustomPayloadPacket"},
            {226, "ServerboundKeepAlivePacket"},
            {227, "ServerboundPlayerAbilitiesPacket"},
            {228, "ServerboundResourcePackPacket"},
            {229, "ServerboundSetCarriedItemPacket"},
            {230, "ServerboundSignUpdatePacket"},
            {231, "ServerboundTeleportToEntityPacket"},
            {232, "ServerboundPreLoginPacket"},
            {233, "ServerboundSwingPacket"},
            {234, "ClientboundDamageIndicatorPacket"},
            {235, "ClientboundBlockCollectionDestructionPacket"},
            {236, "ClientboundCooldownPacket"},
            {239, "ClientboundMGPlayerSettingsUpdatePacket"},
            {250, "ClientboundCustomPayloadPacket"},
            {253, "ClientboundChangeDifficultyPacket"},
            {254, "GetInfoPacket"},
            {255, "DisconnectPacket"}};

        auto it = packetTypeMap.find(packetId);
        if (it != packetTypeMap.end()) {
            return it->second;
        }
        return "UnknownPacket";
    }
};

bool isFileExist(const char* path) {
    nn::fs::DirectoryEntryType type;
    nn::fs::GetEntryType(&type, path);

    return type == nn::fs::DirectoryEntryType_File;
}

nn::Result writeFile(std::string const& str, s64 offset, void* data, size_t length) {
    nn::fs::DirectoryEntryType entryType;
    nn::Result rc = nn::fs::GetEntryType(&entryType, str.c_str());

    if (rc.GetInnerValueForDebug() == 0x202) {  // path does not exist
        if (nn::fs::CreateFile(str.c_str(), offset + length).IsFailure())
            return rc;
    }

    nn::fs::FileHandle handle;
    if ((nn::fs::OpenFile(&handle, str.c_str(),
                          nn::fs::OpenMode_ReadWrite | nn::fs::OpenMode_Append))
            .IsFailure())
        return rc;

    s64 fileSize;
    nn::Result result = nn::fs::GetFileSize(&fileSize, handle);
    if (result.IsFailure()) {
        nn::fs::CloseFile(handle);
        return result;
    }

    if (fileSize < offset + (s64)length) {  // make sure we have enough space
        result = nn::fs::SetFileSize(handle, offset + length);

        if (result.IsFailure()) {
            nn::fs::CloseFile(handle);
            return result;
        }
    }

    result = nn::fs::WriteFile(handle, offset, data, length,
                               nn::fs::WriteOption::CreateOption(nn::fs::WriteOptionFlag_Flush));

    nn::fs::CloseFile(handle);
    return result;
}

struct BufferedOutputStream : public OutputStream {
    BufferedOutputStream(OutputStream* outputStream, int bufferSize);
    virtual ~BufferedOutputStream() override;
    virtual void write(unsigned int) override;
    virtual void write(arrayWithLength<unsigned char>) override;
    virtual void write(arrayWithLength<unsigned char>, unsigned int, unsigned int) override;
    virtual void close() override;
    virtual void flush() override;

    OutputStream* mOutputStream;
    arrayWithLength<unsigned char> mBuffer;
    unsigned int mSize;
};

bool hasMountedSaveData = false;

void mountSaveData() {
    nn::Result result = nn::fs::MountSdCardForDebug("sd");

    char resultString[256];
    sprintf(resultString, "Mount result: %d", result.GetInnerValueForDebug());
    svcOutputDebugString(resultString, strlen(resultString));

    hasMountedSaveData = true;
}

struct PacketWritePacketHook : public mallow::hook::Trampoline<PacketWritePacketHook> {
    static std::unordered_map<unsigned char, int> packetIdCounter;

    static void Callback(std::shared_ptr<Packet> packet, DataOutputStream* outputStream, bool unk,
                         unsigned int& unk2) {
        BufferedOutputStream* bufferedOutputStream =
            reinterpret_cast<BufferedOutputStream*>(outputStream->mOutputStream);

        outputStream->flush();
        bufferedOutputStream->flush();

        Orig(packet, outputStream, unk, unk2);

        if (!hasMountedSaveData) {
            mountSaveData();
        }

        unsigned int length = outputStream->mSize;

        unsigned char* buffer =
            reinterpret_cast<unsigned char*>(bufferedOutputStream->mBuffer.data);

        unsigned char packetId = buffer[0];

        if (packetIdCounter[packetId] >= 25) {
            return;
        }

        std::vector<unsigned char> binaryOutput(buffer, buffer + length);

        long timestamp = packet->mCreatedTime;
        std::string packetName = PacketTypeHelper::GetPacketName(packetId);
        std::string filename = std::to_string(packetId) + "_" + packetName + "_" +
                               std::to_string(timestamp) + "_" + std::to_string(length) + "_" +
                               std::to_string(bufferedOutputStream->mSize) + ".bin";

        writeFile("sd:/packets/" + filename, 0, binaryOutput.data(), binaryOutput.size());

        packetIdCounter[packetId]++;
    }
};

std::unordered_map<unsigned char, int> PacketWritePacketHook::packetIdCounter;

/*
struct PacketWritePacketHook : public mallow::hook::Trampoline<PacketWritePacketHook> {
    static void Callback(std::shared_ptr<Packet> packet, DataOutputStream* outputStream, bool unk,
                         unsigned int& unk2) {
        Orig(packet, outputStream, unk, unk2);

        if (!hasMountedSaveData) {
            mountSaveData();
        }

        auto* bufferedOutputStream =
            static_cast<BufferedOutputStream*>(outputStream->mOutputStream);

        unsigned char packetId = bufferedOutputStream->mBuffer[0];
        uint64_t timestamp = packet->mCreatedTime;
        unsigned int length = outputStream->mSize;
        unsigned char* buffer =
            reinterpret_cast<unsigned char*>(bufferedOutputStream->mBuffer.data);

        std::vector<unsigned char> binaryOutput;
        binaryOutput.reserve(sizeof(timestamp) + sizeof(int) + length);

        auto appendToOutput = [&](const void* data, size_t size) {
            binaryOutput.insert(binaryOutput.end(), static_cast<const unsigned char*>(data),
                                static_cast<const unsigned char*>(data) + size);
        };

        appendToOutput(&timestamp, sizeof(timestamp));
        appendToOutput(&length, sizeof(int));
        appendToOutput(buffer, length);

        std::string packetName = PacketTypeHelper::GetPacketName(packetId);
        std::string filename =
            std::to_string(packetId) + "_" + packetName + "_" + std::to_string(timestamp) + ".bin";

        writeFile("sd:/packets/" + filename, 0, binaryOutput.data(), binaryOutput.size());
    }
};
*/

void PacketDumper::initHooks() {
    PacketWritePacketHook::InstallAtOffset(0x28C50C);
}
