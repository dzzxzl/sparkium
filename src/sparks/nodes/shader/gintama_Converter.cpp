#include "sparks/nodes/shader/gintama_Converter.h"

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
        for(int i=0; i<colors_.size(); i++){
            if(in_gray_scale < values_[i]){
                res_colr = colors_[i];
            }
        }
    }
    static_cast<Vec3Slot*>(out_slots_[0])->value_ = res_colr;
    // push to next node
    if(out_slots_[0]->nextNode_ != nullptr) {
        auto next_slot = out_slots_[0]->nextNode_->in_slots_[out_slots_[0]->nextSlotId_];
        static_cast<Vec3Slot*>(next_slot)->value_ = res_colr;
    }
}

} // namespace sparks