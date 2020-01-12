#pragma once

#include <memory>
#include "primitives.hpp"
#include "emu/memory.hpp"

namespace emu {
    /// Absolute address on the 8086 are 20-bit however no 20-bit unsigned integer type exists in C++ so a 32-bit
    /// unsigned integer is used instead.
    using AbsAddr = u32;

    /// Values stored in memory are 8-bit wide.
    using MemValue = u8;

    /// Memory type for memory storing 8-bit byte values ad taking 32-bit addresses.
    using Mem = Memory<MemValue, AbsAddr>;

    /// Offset addresses within a given segment are 16-bit wide.
    using OffsetAddr = u16;

    /// Type of values stored by standard registers.
    using RegSize = u16;
}
