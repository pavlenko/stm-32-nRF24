#include "PE_nRF24_api.h"
#include "PE_nRF24_spi.h"

const uint8_t PE_nRF24_REG_RX_PW[6] = {
        PE_nRF24_REG_RX_PW_P0,
        PE_nRF24_REG_RX_PW_P1,
        PE_nRF24_REG_RX_PW_P2,
        PE_nRF24_REG_RX_PW_P3,
        PE_nRF24_REG_RX_PW_P4,
        PE_nRF24_REG_RX_PW_P5,
};

const uint8_t PE_nRF24_REG_RX_ADDR[7] = {
        PE_nRF24_REG_RX_ADDR_P0,
        PE_nRF24_REG_RX_ADDR_P1,
        PE_nRF24_REG_RX_ADDR_P2,
        PE_nRF24_REG_RX_ADDR_P3,
        PE_nRF24_REG_RX_ADDR_P4,
        PE_nRF24_REG_RX_ADDR_P5,
};

PE_nRF24_RESULT_t PE_nRF24_getRegister(PE_nRF24_handle_t *handle, uint8_t addr, uint8_t *byte) {
    PE_nRF24_setSS0(handle);

    if (PE_nRF24_readMem(handle, PE_nRF24_CMD_R_REGISTER | addr, byte, 1) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    PE_nRF24_setSS1(handle);

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_setRegister(PE_nRF24_handle_t *handle, uint8_t addr, uint8_t *byte) {
    PE_nRF24_setSS0(handle);

    if (PE_nRF24_sendMem(handle, PE_nRF24_CMD_W_REGISTER | addr, byte, 1) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    PE_nRF24_setSS1(handle);

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_getPayload(PE_nRF24_handle_t *handle, uint8_t *data, uint8_t size) {
    return PE_nRF24_readMem(handle, PE_nRF24_CMD_R_RX_PAYLOAD, data, size);
}

PE_nRF24_RESULT_t PE_nRF24_setPayload(PE_nRF24_handle_t *handle, uint8_t *data, uint8_t size) {
    return PE_nRF24_sendMem(handle, PE_nRF24_CMD_W_TX_PAYLOAD, data, size);
}

PE_nRF24_RESULT_t PE_nRF24_readPacket(PE_nRF24_handle_t *handle, uint8_t *data, uint8_t size, uint16_t timeout) {
    if (handle->status != PE_nRF24_STATUS_READY) {
        return PE_nRF24_RESULT_ERROR;
    }

    handle->status = PE_nRF24_STATUS_BUSY_RX;

    PE_nRF24_setCE0(handle);

    PE_nRF24_setDirection(handle, PE_nRF24_DIRECTION_RX);

    PE_nRF24_setCE1(handle);

    if (timeout > 0) {
        uint32_t start = PE_nRF24_getMillis();

        while (PE_nRF24_checkIRQ(handle, PE_nRF24_IRQ_TX_DS) != PE_nRF24_RESULT_OK) {
            if ((PE_nRF24_getMillis() - start) > timeout) {
                handle->status = PE_nRF24_STATUS_READY;
                return PE_nRF24_RESULT_TIMEOUT;
            }
        }

        PE_nRF24_getPayload(handle, data, size);

        handle->status = PE_nRF24_STATUS_READY;
    } else {
        PE_nRF24_attachIRQ(handle, PE_nRF24_IRQ_RX_DR);

        handle->bufferData = data;
        handle->bufferSize = size;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_sendPacket(PE_nRF24_handle_t *handle, uint8_t *addr, uint8_t *data, uint8_t size, uint16_t timeout) {
    if (handle->status != PE_nRF24_STATUS_READY) {
        return PE_nRF24_RESULT_ERROR;
    }

    if (PE_nRF24_setTXAddress(handle, addr) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    handle->status = PE_nRF24_STATUS_BUSY_TX;

    PE_nRF24_setCE0(handle);

    PE_nRF24_setDirection(handle, PE_nRF24_DIRECTION_TX);
    PE_nRF24_setPayload(handle, data, size);

    PE_nRF24_setCE1(handle);

    if (timeout > 0) {
        uint32_t start = PE_nRF24_getMillis();

        while (PE_nRF24_checkIRQ(handle, PE_nRF24_IRQ_TX_DS) != PE_nRF24_RESULT_OK) {
            if ((PE_nRF24_getMillis() - start) > timeout) {
                handle->status = PE_nRF24_STATUS_READY;
                return PE_nRF24_RESULT_TIMEOUT;
            }
        }

        handle->status = PE_nRF24_STATUS_READY;
    } else {
        PE_nRF24_attachIRQ(handle, PE_nRF24_IRQ_TX_DS);
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_setAddressWidth(PE_nRF24_handle_t *handle, PE_nRF24_ADDR_WIDTH_t width) {
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_SETUP_AW, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~PE_nRF24_SETUP_AW;
    reg |= width;

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_SETUP_AW, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_getAddressWidth(PE_nRF24_handle_t *handle, PE_nRF24_ADDR_WIDTH_t *width) {
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_SETUP_AW, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    *width = (PE_nRF24_ADDR_WIDTH_t) (reg & PE_nRF24_SETUP_AW);

    if (*width == PE_nRF24_ADDR_WIDTH_ILLEGAL) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_setTXAddress(PE_nRF24_handle_t *handle, uint8_t *address) {
    uint8_t width;

    PE_nRF24_getAddressWidth(handle, (PE_nRF24_ADDR_WIDTH_t *) &width);

    if (PE_nRF24_sendMem(handle, PE_nRF24_CMD_W_REGISTER + PE_nRF24_REG_TX_ADDR, address, width + 2) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_setRXAddress(PE_nRF24_handle_t *handle, uint8_t *address, PE_nRF24_PIPE_t pipe) {
    uint8_t width;

    PE_nRF24_getAddressWidth(handle, (PE_nRF24_ADDR_WIDTH_t *) &width);

    if (pipe > PE_nRF24_PIPE_RX1) {
        width = 1;
    } else {
        width += 2;
    }

    if (PE_nRF24_sendMem(handle, PE_nRF24_CMD_W_REGISTER + PE_nRF24_REG_RX_ADDR[pipe], address, width) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_flushTX(PE_nRF24_handle_t *handle) {
    return PE_nRF24_sendByte(handle, PE_nRF24_CMD_FLUSH_TX);
}

PE_nRF24_RESULT_t PE_nRF24_flushRX(PE_nRF24_handle_t *handle) {
    return PE_nRF24_sendByte(handle, PE_nRF24_CMD_FLUSH_RX);
}

PE_nRF24_RESULT_t PE_nRF24_attachIRQ(PE_nRF24_handle_t *handle, PE_nRF24_IRQ_t mask) {
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~(mask & (uint8_t) PE_nRF24_IRQ_ALL);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_detachIRQ(PE_nRF24_handle_t *handle, PE_nRF24_IRQ_t mask) {
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg |= (mask & (uint8_t) PE_nRF24_IRQ_ALL);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_clearIRQ(PE_nRF24_handle_t *handle, PE_nRF24_IRQ_t mask) {
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_STATUS, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg |= (mask & PE_nRF24_IRQ_MASK_ALL);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_STATUS, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_checkIRQ(PE_nRF24_handle_t *handle, PE_nRF24_IRQ_t mask) {
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_STATUS, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    if ((reg & mask) != 0U) {
        return PE_nRF24_RESULT_OK;
    }

    return PE_nRF24_RESULT_ERROR;
}

PE_nRF24_RESULT_t PE_nRF24_setDirection(PE_nRF24_handle_t *handle, PE_nRF24_DIRECTION_t direction) {
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~PE_nRF24_CONFIG_PRIM_RX;
    reg |= (direction << PE_nRF24_CONFIG_PRIM_RX_Pos);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

__attribute__((weak))
uint32_t PE_nRF24_getMillis(void) {
    return 0;
}
