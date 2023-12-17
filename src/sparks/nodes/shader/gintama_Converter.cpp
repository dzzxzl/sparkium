#include "gintama_Converter.h"

namespace sparks{

MixShader::MixShader()
{
    in_slots_.resize(3);
    out_slots_.resize(1);
    out_slots_[0]->slotType_ = Slot::SlotType::output;
}

}
