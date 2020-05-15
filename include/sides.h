#pragma once

namespace sides {
    enum Side {
        Top,
        Bottom,
        Left,
        Right,
        None
    };

    const inline Side get_opposite_side(Side side) {
        return 
            side == Top ? Bottom :
            side == Bottom ? Top :
            side == Left ? Right :
            side == Right ? Left :
                None;
    };
};