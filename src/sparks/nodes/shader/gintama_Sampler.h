#pragma once

#include "sparks/nodes/shader/gintama_Layout.h"

namespace sparks {

class SpecularSampler: public Node {
    public:
        SpecularSampler() = delete;
        SpecularSampler(SceneInfo* scene_info);
        ~SpecularSampler() override;
        void process() override;
        // enum class InSlotName {
        //     Roughness = 0,
        //     Normal,
        //     View,
        //     Light
        // };
        enum class OutSlotName {
            Direction = 0,
            Weight
        };
        bool overwrite_weight_{false};
        SceneInfo* scene_info_;
        int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
        // int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
        Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
        // Slot * getInSlot(InSlotName slot_name) { return in_slots_[slotID(slot_name)]; }
};

class RoughGlassSampler: public Node {
    public:
        RoughGlassSampler() = delete;
        RoughGlassSampler(SceneInfo* scene_info);
        ~RoughGlassSampler() override;
        void process() override;
        enum class InSlotName {
            Normal = 0,
            IOR,
            Roughness
        };
        enum class OutSlotName {
            Direction = 0,
            Weight
        };
        bool overwrite_weight_{false};
        SceneInfo* scene_info_;
        int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
        int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
        Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
        Slot * getInSlot(InSlotName slot_name) { return in_slots_[slotID(slot_name)]; }
};

class GlassSampler: public Node {
    public:
        GlassSampler() = delete;
        GlassSampler(SceneInfo* scene_info);
        ~GlassSampler() override;
        void process() override;
        // enum class InSlotName {
        //     Roughness = 0,
        //     Normal,
        //     View,
        //     Light
        // };
        enum class OutSlotName {
            Direction = 0,
            Weight
        };
        // bool overwrite_weight_{false};
        SceneInfo* scene_info_;
        int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
        // int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
        Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
        // Slot * getInSlot(InSlotName slot_name) { return in_slots_[slotID(slot_name)]; }
};

class DiffuseSample: public Node {
    public:
        DiffuseSample() = delete;
        DiffuseSample(SceneInfo* scene_info);
        ~DiffuseSample() override;
        void process() override;
        // enum class InSlotName {
        //     Roughness = 0,
        //     Normal,
        //     View,
        //     Light
        // };
        enum class OutSlotName {
            Direction = 0,
            Weight
        };
        bool overwrite_weight_{false};
        SceneInfo* scene_info_;
        int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
        // int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
        Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
        // Slot * getInSlot(InSlotName slot_name) { return in_slots_[slotID(slot_name)]; }
};

class MixSampler: public Node {
    public:
        MixSampler() = delete;
        MixSampler(SceneInfo* scene_info);
        ~MixSampler() override;
        void process() override;
        enum class InSlotName {
            SamplerA = 0,
            SamplerB
        };
        enum class OutSlotName {
            Direction = 0,
            Weight,
        };
        bool overwrite_weight_{false};
        SceneInfo* scene_info_;
        int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
        int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
        Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
        Slot * getInSlot(InSlotName slot_name) { return in_slots_[slotID(slot_name)]; }
};



}