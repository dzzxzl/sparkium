#include "sparks/nodes/shader/gintama_Layout.h"
#include "gintama_Layout.h"

namespace sparks{

int Node::next_id_ = 0;

void Slot::link(Slot * nextSlot)
{
    if(slotType_ == SlotType::output) {
        nextSlot_ = nextSlot;
    }
}


} // namespace sparks