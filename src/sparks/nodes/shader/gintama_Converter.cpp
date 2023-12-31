#include "sparks/nodes/shader/gintama_Converter.h"
#include "gintama_Converter.h"

namespace sparks{

MixShader::MixShader()
{
    in_slots_.resize(3);
    out_slots_.resize(1);
    in_slots_[0] = new FloatSlot(0.5f);
    in_slots_[1] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    in_slots_[2] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0]->slotType_ = Slot::SlotType::output;
}
MixShader::~MixShader() {
    for (auto& slot : in_slots_) {
        delete slot;
    }
    for (auto& slot : out_slots_) {
        delete slot;
    }
}
Multiplier::Multiplier()
{
    in_slots_.resize(2);
    out_slots_.resize(1);
    in_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    in_slots_[1] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0]->slotType_ = Slot::SlotType::output;
}
Multiplier::~Multiplier() {
    for (auto& slot : in_slots_) {
        delete slot;
    }
    for (auto& slot : out_slots_) {
        delete slot;
    }
}

void Multiplier::process() {
    // process all input slots
    for(int i=0; i<2; i++){
        if(in_slots_[i]->lastNode_ != nullptr) {
            in_slots_[i]->lastNode_->process();
        }
    }
    // process current node
    glm::vec3 in_color1 = static_cast<Vec3Slot*>(in_slots_[0])->value_;
    glm::vec3 in_color2 = static_cast<Vec3Slot*>(in_slots_[1])->value_;
    // glm::vec3 res_colr = in_color1 * in_color2;
    glm::vec3 res_colr = in_color1 * in_color2;
    static_cast<Vec3Slot*>(out_slots_[0])->value_ = res_colr;
    // push to next node
    if(out_slots_[0]->nextNode_ != nullptr) {
        auto next_slot = out_slots_[0]->nextNode_->in_slots_[out_slots_[0]->nextSlotId_];
        static_cast<Vec3Slot*>(next_slot)->value_ = res_colr;
    }
}

ColorClamp::ColorClamp()
{
    in_slots_.resize(1);
    out_slots_.resize(1);
    in_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0]->slotType_ = Slot::SlotType::output;
}

ColorClamp::~ColorClamp() {
    for (auto& slot : in_slots_) {
        delete slot;
    }
    for (auto& slot : out_slots_) {
        delete slot;
    }
}

void ColorClamp::setColor( glm::vec3 color, float value ) {
    colors_.push_back(color);
    values_.push_back(value);
}

void ColorClamp::process() {
    // process all input slots
    for(int i=0; i<1; i++){
        if(in_slots_[i]->lastNode_ != nullptr) {
            in_slots_[i]->lastNode_->process();
        }
    }
    // process current node
    glm::vec3 in_color = static_cast<Vec3Slot*>(in_slots_[0])->value_;
    glm::vec3 res_colr = in_color;
    if(!grayscale_) {
        for(int i=0; i<colors_.size(); i++){
            if(in_color.r < values_[i]){
                res_colr.r = colors_[i].r;
            }
            if(in_color.g < values_[i]){
                res_colr.g = colors_[i].g;
            }
            if(in_color.b < values_[i]){
                res_colr.b = colors_[i].b;
            }
        }
    } else {
        float in_gray_scale = rgbtoGray(in_color);
        int idx = 0;
        while( idx < colors_.size() ) {
            if(in_gray_scale < values_[idx]) {
                break;
            }
            idx++;
        }
        if(idx == 0) {
            res_colr = start_color_;
        } else {
            res_colr = colors_[idx-1];
        }
    }
    static_cast<Vec3Slot*>(out_slots_[0])->value_ = res_colr;
    // push to next node
    if(out_slots_[0]->nextNode_ != nullptr) {
        auto next_slot = out_slots_[0]->nextNode_->in_slots_[out_slots_[0]->nextSlotId_];
        static_cast<Vec3Slot*>(next_slot)->value_ = res_colr;
    }
}

ColorRamp::ColorRamp()
{
    in_slots_.resize(1);
    out_slots_.resize(1);
    in_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0]->slotType_ = Slot::SlotType::output;
}

ColorRamp::~ColorRamp()
{
    for (auto& slot : in_slots_) {
        delete slot;
    }
    for (auto& slot : out_slots_) {
        delete slot;
    }
}

void ColorRamp::process()
{
    // process all input slots
    for(int i=0; i<1; i++){
        if(in_slots_[i]->lastNode_ != nullptr) {
            in_slots_[i]->lastNode_->process();
        }
    }
    // process current node
    glm::vec3 in_color = static_cast<Vec3Slot*>(in_slots_[0])->value_;
    float in_value = rgbtoGray(in_color);
    glm::vec3 res_colr = glm::vec3(in_value);
    // find nearest colors
    int idx = 0;
    while( idx < colors_.size() ) {
        if(in_value < values_[idx]) {
            break;
        }
        idx++;
    }
    float value1;
    glm::vec3 color1;
    float value2;
    glm::vec3 color2;
    if(idx == 0) {
        // then between start and first color
        value1 = 0.0f;
        color1 = start_color_;
        value2 = values_[0];
        color2 = colors_[0];
    } else if (idx == colors_.size()) {
        // then between last color and end
        value1 = values_[idx-1];
        color1 = colors_[idx-1];
        value2 = 1.0f;
        color2 = end_color_;
    } else {
        // then between two colors
        value1 = values_[idx-1];
        color1 = colors_[idx-1];
        value2 = values_[idx];
        color2 = colors_[idx];
    }
    // interpolate
    if ( interpolation_ == Interpolation::Linear ) {
        float fac = (in_value - value1) / (value2 - value1);
        if(fac<0.0f) {
            LAND_INFO("fac < 0.0f");
        }
        if(in_value < value1) {
            LAND_INFO("in_value: {} < value1: {}", in_value, value1);
        }
        res_colr = color1 * (1.0f - fac) + color2 * fac;
    } else {

    }
    static_cast<Vec3Slot*>(out_slots_[0])->value_ = res_colr;
    // push to next node
    if(out_slots_[0]->nextNode_ != nullptr) {
        auto next_slot = out_slots_[0]->nextNode_->in_slots_[out_slots_[0]->nextSlotId_];
        static_cast<Vec3Slot*>(next_slot)->value_ = res_colr;
    }
}

void ColorRamp::setColor(glm::vec3 color, float value)
{
    colors_.push_back(color);
    values_.push_back(value);
}

Inverter::Inverter()
{
    in_slots_.resize(1);
    out_slots_.resize(1);
    in_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0]->slotType_ = Slot::SlotType::output;
}

Inverter::~Inverter()
{
    for (auto& slot : in_slots_) {
        delete slot;
    }
    for (auto& slot : out_slots_) {
        delete slot;
    }
}

void Inverter::process()
{
    // process all input slots
    for(int i=0; i<1; i++){
        if(in_slots_[i]->lastNode_ != nullptr) {
            in_slots_[i]->lastNode_->process();
        }
    }
    // process current node
    glm::vec3 in_color = static_cast<Vec3Slot*>(in_slots_[0])->value_;
    glm::vec3 res_colr = in_color;
    if(invert_) {
        res_colr = glm::vec3(1.0f) - in_color;
    }
    static_cast<Vec3Slot*>(out_slots_[0])->value_ = res_colr;
    // push to next node
    if(out_slots_[0]->nextNode_ != nullptr) {
        auto next_slot = out_slots_[0]->nextNode_->in_slots_[out_slots_[0]->nextSlotId_];
        static_cast<Vec3Slot*>(next_slot)->value_ = res_colr;
    }
}

Color2Fac::Color2Fac()
{
    in_slots_.resize(1);
    out_slots_.resize(1);
    in_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0] = new FloatSlot(0.0f);
    out_slots_[0]->slotType_ = Slot::SlotType::output;
}

Color2Fac::~Color2Fac()
{
    for (auto& slot : in_slots_) {
        delete slot;
    }
    for (auto& slot : out_slots_) {
        delete slot;
    }
}

void Color2Fac::process()
{
    // process all input slots
    for(int i=0; i<1; i++){
        if(in_slots_[i]->lastNode_ != nullptr) {
            in_slots_[i]->lastNode_->process();
        }
    }
    // process current node
    glm::vec3 in_color = static_cast<Vec3Slot*>(in_slots_[0])->value_;
    float res_fac = rgbtoGray(in_color);
    static_cast<FloatSlot*>(out_slots_[0])->value_ = res_fac;
    // push to next node
    if(out_slots_[0]->nextNode_ != nullptr) {
        auto next_slot = out_slots_[0]->nextNode_->in_slots_[out_slots_[0]->nextSlotId_];
        static_cast<FloatSlot*>(next_slot)->value_ = res_fac;
    }
}

} // namespace sparks
sparks::PerturbNormal::PerturbNormal(SceneInfo * scene_info): scene_info_(scene_info)
{
    in_slots_.resize(4);
    out_slots_.resize(1);
    in_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    in_slots_[1] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    in_slots_[2] = new FloatSlot(0.1f);
    in_slots_[3] = new FloatSlot(1.0f);
    out_slots_[0]->slotType_ = Slot::SlotType::output;
}

sparks::PerturbNormal::~PerturbNormal()
{
    for (auto& slot : in_slots_) {
        delete slot;
    }
    for (auto& slot : out_slots_) {
        delete slot;
    }
}

void sparks::PerturbNormal::process()
{
    // process all input slots
    for(int i=0; i<2; i++){
        if(in_slots_[i]->lastNode_ != nullptr) {
            in_slots_[i]->lastNode_->process();
        }
    }
    // process current node
    glm::vec3 normal = scene_info_->hit_record_.geometry_normal;
    auto material = scene_info_->scene_->GetEntity(scene_info_->hit_record_.hit_entity_id).GetMaterial();
    auto shade_smooth = material.shade_smooth;
    if(shade_smooth) {
        normal = scene_info_->hit_record_.normal;
    }
    if(in_slots_[ slotID(InSlotName::Normal) ]->lastNode_ != nullptr) {
        normal = static_cast<Vec3Slot*>(in_slots_[ slotID(InSlotName::Normal) ])->value_;
    }
    glm::vec3 perturb_direction = static_cast<Vec3Slot*>(in_slots_[ slotID(InSlotName::PerturbDirection) ])->value_;
    float fac_scale = static_cast<FloatSlot*>(in_slots_[ slotID(InSlotName::Fac) ])->value_;
    fac_scale *= static_cast<FloatSlot*>(in_slots_[ slotID(InSlotName::Scale) ])->value_;
    glm::vec3 perturbed_normal = glm::normalize( normal + perturb_direction * fac_scale );

    glm::vec3 res_color = perturbed_normal;

    static_cast<Vec3Slot*>(out_slots_[0])->value_ = res_color;
    // push to next node
    if(out_slots_[0]->nextNode_ != nullptr) {
        auto next_slot = out_slots_[0]->nextNode_->in_slots_[out_slots_[0]->nextSlotId_];
        static_cast<Vec3Slot*>(next_slot)->value_ = res_color;
    }
}
