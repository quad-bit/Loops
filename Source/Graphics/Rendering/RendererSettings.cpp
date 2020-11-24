#include "RendererSettings.h"

uint32_t RendererSettings::renderQueueId;
uint32_t RendererSettings::presentationQueueId;
uint32_t RendererSettings::computeQueueId;
uint32_t RendererSettings::transferQueueId;

QueueWrapper * RendererSettings::queueReq;
uint32_t RendererSettings::queueRequirementCount;