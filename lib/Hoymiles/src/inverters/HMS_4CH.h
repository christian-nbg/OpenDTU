// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "HMS_Abstract.h"

class HMS_4CH : public HMS_Abstract {
public:
    explicit HMS_4CH(HoymilesRadio* radio, uint64_t serial);
    static bool isValidSerial(uint64_t serial);
    String typeName();
    const std::list<byteAssign_t>* getByteAssignment();

private:
    const std::list<byteAssign_t> byteAssignment = {
        { TYPE_DC, CH0, FLD_UDC, UNIT_V, 2, 2, 10, false, 1 },
        { TYPE_DC, CH0, FLD_IDC, UNIT_A, 6, 2, 100, false, 2 },
        { TYPE_DC, CH0, FLD_PDC, UNIT_W, 10, 2, 10, false, 1 },
        { TYPE_DC, CH0, FLD_YD, UNIT_WH, 22, 2, 1, false, 0 },
        { TYPE_DC, CH0, FLD_YT, UNIT_KWH, 14, 4, 1000, false, 3 },
        { TYPE_DC, CH0, FLD_IRR, UNIT_PCT, CALC_IRR_CH, CH0, CMD_CALC, false, 3 },

        { TYPE_DC, CH1, FLD_UDC, UNIT_V, 4, 2, 10, false, 1 },
        { TYPE_DC, CH1, FLD_IDC, UNIT_A, 8, 2, 100, false, 2 },
        { TYPE_DC, CH1, FLD_PDC, UNIT_W, 12, 2, 10, false, 1 },
        { TYPE_DC, CH1, FLD_YD, UNIT_WH, 24, 2, 1, false, 0 },
        { TYPE_DC, CH1, FLD_YT, UNIT_KWH, 18, 4, 1000, false, 3 },
        { TYPE_DC, CH1, FLD_IRR, UNIT_PCT, CALC_IRR_CH, CH1, CMD_CALC, false, 3 },

        { TYPE_DC, CH2, FLD_UDC, UNIT_V, 26, 2, 10, false, 1 },
        { TYPE_DC, CH2, FLD_IDC, UNIT_A, 30, 2, 100, false, 2 },
        { TYPE_DC, CH2, FLD_PDC, UNIT_W, 34, 2, 10, false, 1 },
        { TYPE_DC, CH2, FLD_YD, UNIT_WH, 46, 2, 1, false, 0 },
        { TYPE_DC, CH2, FLD_YT, UNIT_KWH, 38, 4, 1000, false, 3 },
        { TYPE_DC, CH2, FLD_IRR, UNIT_PCT, CALC_IRR_CH, CH2, CMD_CALC, false, 3 },

        { TYPE_DC, CH3, FLD_UDC, UNIT_V, 28, 2, 10, false, 1 },
        { TYPE_DC, CH3, FLD_IDC, UNIT_A, 32, 2, 100, false, 2 },
        { TYPE_DC, CH3, FLD_PDC, UNIT_W, 36, 2, 10, false, 1 },
        { TYPE_DC, CH3, FLD_YD, UNIT_WH, 48, 2, 1, false, 0 },
        { TYPE_DC, CH3, FLD_YT, UNIT_KWH, 42, 4, 1000, false, 3 },
        { TYPE_DC, CH3, FLD_IRR, UNIT_PCT, CALC_IRR_CH, CH3, CMD_CALC, false, 3 },

        { TYPE_AC, CH0, FLD_UAC, UNIT_V, 50, 2, 10, false, 1 },
        { TYPE_AC, CH0, FLD_IAC, UNIT_A, 58, 2, 100, false, 2 },
        { TYPE_AC, CH0, FLD_PAC, UNIT_W, 54, 2, 10, false, 1 },
        { TYPE_AC, CH0, FLD_PRA, UNIT_VA, 56, 2, 10, false, 1 },
        { TYPE_AC, CH0, FLD_F, UNIT_HZ, 52, 2, 100, false, 2 },
        { TYPE_AC, CH0, FLD_PF, UNIT_NONE, 60, 2, 1000, false, 3 },

        { TYPE_INV, CH0, FLD_T, UNIT_C, 62, 2, 10, true, 1 },
        { TYPE_INV, CH0, FLD_EVT_LOG, UNIT_NONE, 64, 2, 1, false, 0 },

        { TYPE_AC, CH0, FLD_YD, UNIT_WH, CALC_YD_CH0, 0, CMD_CALC, false, 0 },
        { TYPE_AC, CH0, FLD_YT, UNIT_KWH, CALC_YT_CH0, 0, CMD_CALC, false, 3 },
        { TYPE_AC, CH0, FLD_PDC, UNIT_W, CALC_PDC_CH0, 0, CMD_CALC, false, 1 },
        { TYPE_AC, CH0, FLD_EFF, UNIT_PCT, CALC_EFF_CH0, 0, CMD_CALC, false, 3 }
    };
};