#include "gintama_Vector.h"

namespace sparks {

    Bump::Bump()
    {
        in_slots_.resize(3);
        out_slots_.resize(1);
        out_slots_[0]->slotType_ = Slot::SlotType::output;
    }
}
