#pragma once

#include <stdint.h>
struct QueueWrapper;

struct RendererSettings
{
    static uint32_t renderQueueId;
    static uint32_t presentationQueueId;
    static uint32_t computeQueueId;
    static uint32_t transferQueueId;

    static QueueWrapper * queueReq;
    static uint32_t queueRequirementCount;

    static bool MSAA_Enabled;
};