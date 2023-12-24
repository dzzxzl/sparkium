#include "sparks/nodes/shader/gintama_Sampler.h"



sparks::SpecularSampler::SpecularSampler(SceneInfo * scene_info): scene_info_(scene_info)
{
    out_slots_.resize(2);
    out_slots_[slotID(OutSlotName::Direction)] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[slotID(OutSlotName::Weight)] = new FloatSlot(0.0f);
    out_slots_[slotID(OutSlotName::Direction)]->slotType_ = Slot::SlotType::output;
    out_slots_[slotID(OutSlotName::Weight)]->slotType_ = Slot::SlotType::output;
}

sparks::SpecularSampler::~SpecularSampler()
{
    for (auto& slot : out_slots_) {
        delete slot;
    }
}

void sparks::SpecularSampler::process()
{

}

sparks::RoughGlassSampler::RoughGlassSampler(SceneInfo * scene_info)
{
}

sparks::RoughGlassSampler::~RoughGlassSampler()
{
}

void sparks::RoughGlassSampler::process()
{
}

sparks::GlassSampler::GlassSampler(SceneInfo * scene_info)
{
}

sparks::GlassSampler::~GlassSampler()
{
}

void sparks::GlassSampler::process()
{
}

sparks::DiffuseSample::DiffuseSample(SceneInfo * scene_info)
{
}

sparks::DiffuseSample::~DiffuseSample()
{
}

void sparks::DiffuseSample::process()
{
}

sparks::MixSampler::MixSampler(SceneInfo * scene_info)
{
}

sparks::MixSampler::~MixSampler()
{
}

void sparks::MixSampler::process()
{
}
