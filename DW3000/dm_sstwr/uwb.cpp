// #include "dm_sstwr/uwb.h"
// #include "dw3000.h"

// dwt_config_t config = {
//     5,            /* Channel number. */
//     DWT_PLEN_128, /* Preamble length. Used in TX only. */
//     DWT_PAC8,     /* Preamble acquisition chunk size. Used in RX only. */
//     9,            /* TX preamble code. Used in TX only. */
//     9,            /* RX preamble code. Used in RX only. */
//     1, /* 0 to use standard 8 symbol SFD, 1 to use non-standard 8 symbol, 2
//           for non-standard 16 symbol SFD and 3 for 4z 8 symbol SDF type */
//     DWT_BR_6M8,       /* Data rate. */
//     DWT_PHRMODE_STD,  /* PHY header mode. */
//     DWT_PHRRATE_STD,  /* PHY header rate. */
//     (129 + 8 - 8),    /* SFD timeout (preamble length + 1 + SFD length - PAC
//                          size).    Used in RX only. */
//     DWT_STS_MODE_OFF, /* STS disabled */
//     DWT_STS_LEN_64,   /* STS length see allowed values in Enum
//                        * dwt_sts_lengths_e
//                        */
//     DWT_PDOA_M0       /* PDOA mode off */
// };
// extern dwt_txconfig_t txconfig_options;

// uint8_t tx_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 0, 0, UID, 0, FUNC_CODE_INTER,
//                     0,    0,    0, 0,    0,    0, 0, 0,   0, 0,
//                     0,    0,    0, 0,    0,    0, 0, 0,   0, 0,
//                     0,    0,    0, 0,    0,    0};
// uint8_t rx_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 0, 0, UID, 0, FUNC_CODE_INTER,
//                     0,    0,    0, 0,    0,    0, 0, 0,   0, 0,
//                     0,    0,    0, 0,    0,    0, 0, 0,   0, 0,
//                     0,    0,    0, 0,    0,    0};

// uint8_t frame_seq_nb = 0;
// uint8_t rx_buffer[NUM_NODES - 1][BUF_LEN];

// uint32_t status_reg = 0;
// int counter = 0;
// int ret;

// uint64_t rx_ts[NUM_NODES - 1];
// uint64_t desired_tx_ts, real_tx_ts;
// uint32_t tx_time;
// uint32_t t_reply;
// uint64_t t_round;
// double tof, distance;
// unsigned long previous_debug_millis = 0;
// unsigned long current_debug_millis = 0;
// int millis_since_last_serial_print;

// int target_uids[NUM_NODES - 1];


// // ---------------------------------------- Add Kalman filter ----------------------------------------
// // // Kalman filter variables
// // double Q = 0.01; // Process noise variance
// // double R = 0.001; // Measurement noise variance
// // double P = 1.0; // Estimate error covariance
// // double K = 0.0; // Kalman gain
// // double distance_estimate = 0.0; // Initial estimate of the distance

// // double kalman_filter(double measurement) {
// //     // Prediction update
// //     P = P + Q;

// //     // Measurement update
// //     K = P / (P + R); // Kalman gain
// //     distance_estimate = distance_estimate + K * (measurement - distance_estimate);
// //     P = (1 - K) * P; // Update the error covariance

// //     return distance_estimate;
// // }

// void set_target_uids() {
// /*
//  * U1 is the initiator, U2 - U6 are responders
//  * U1 - U6 are the target UIDs
//  */
// #ifdef MAIN_U1
//     switch (NUM_NODES) {
//         case 6:
//             target_uids[4] = U6;
//         case 5:
//             target_uids[3] = U5;
//         case 4:
//             target_uids[2] = U4;
//         case 3:
//             target_uids[1] = U3;
//         case 2:
//             target_uids[0] = U2;
//         default:
//             break;
//     }
// #elif defined(MAIN_U2)
//     switch (NUM_NODES) {
//         case 6:
//             target_uids[4] = U6;
//         case 5:
//             target_uids[3] = U5;
//         case 4:
//             target_uids[2] = U4;
//         case 3:
//             target_uids[1] = U3;
//         case 2:
//             target_uids[0] = U1;
//         default:
//             break;
//     }
// #elif defined(MAIN_U3)
//     switch (NUM_NODES) {
//         case 6:
//             target_uids[4] = U6;
//         case 5:
//             target_uids[3] = U5;
//         case 4:
//             target_uids[2] = U4;
//         case 3:
//             target_uids[1] = U2;
//         case 2:
//             target_uids[0] = U1;
//         default:
//             break;
//     }
// #elif defined(MAIN_U4)
//     switch (NUM_NODES) {
//         case 6:
//             target_uids[4] = U6;
//         case 5:
//             target_uids[3] = U5;
//         case 4:
//             target_uids[2] = U3;
//         case 3:
//             target_uids[1] = U2;
//         case 2:
//             target_uids[0] = U1;
//         default:
//             break;
//     }
// #elif defined(MAIN_U5)
//     switch (NUM_NODES) {
//         case 6:
//             target_uids[4] = U6;
//         case 5:
//             target_uids[3] = U4;
//         case 4:
//             target_uids[2] = U3;
//         case 3:
//             target_uids[1] = U2;
//         case 2:
//             target_uids[0] = U1;
//         default:
//             break;
//     }
// #elif defined(MAIN_U6)
//     switch (NUM_NODES) {
//         case 6:
//             target_uids[4] = U5;
//         case 5:
//             target_uids[3] = U4;
//         case 4:
//             target_uids[2] = U3;
//         case 3:
//             target_uids[1] = U2;
//         case 2:
//             target_uids[0] = U1;
//         default:
//             break;
//     }
// #endif
// }

// void start_uwb() {
//     while (!dwt_checkidlerc()) {
//         UART_puts("IDLE FAILED\r\n");
//         while (1);
//     }

//     if (dwt_initialise(DWT_DW_INIT) == DWT_ERROR) {
//         UART_puts("INIT FAILED\r\n");
//         while (1);
//     }

//     dwt_setleds(DWT_LEDS_DISABLE);

//     if (dwt_configure(&config)) {
//         UART_puts("CONFIG FAILED\r\n");
//         while (1);
//     }

//     dwt_configuretxrf(&txconfig_options);
//     dwt_setrxantennadelay(RX_ANT_DLY);
//     dwt_settxantennadelay(TX_ANT_DLY);
//     dwt_setrxaftertxdelay(TX_TO_RX_DLY_UUS);
// #ifdef INITIATOR
//     dwt_setrxtimeout(RX_TIMEOUT_UUS);
// #else
//     dwt_setrxtimeout(0);
// #endif
//     dwt_setlnapamode(DWT_LNA_ENABLE | DWT_PA_ENABLE);

//     set_target_uids();
//     Serial.println(APP_NAME);
//     Serial.println(UID);
//     Serial.println("Setup over........");
// }

// void initiator() {
//     if (counter == 0) {
//         tx_msg[MSG_SN_IDX] = frame_seq_nb;
//         tx_msg[MSG_FUNC_IDX] = FUNC_CODE_INTER;
//         dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS_BIT_MASK);
//         dwt_writetxdata((uint16_t)(MSG_LEN), tx_msg, 0);
//         dwt_writetxfctrl((uint16_t)(MSG_LEN), 0, 1);
//         dwt_starttx(DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);
//     } else {
//         dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS_BIT_MASK);
//         dwt_rxenable(DWT_START_RX_IMMEDIATE);
//     }

//     while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) &
//              (SYS_STATUS_RXFCG_BIT_MASK | SYS_STATUS_ALL_RX_TO |
//               SYS_STATUS_ALL_RX_ERR))) {
//     };

//     if (status_reg & SYS_STATUS_RXFCG_BIT_MASK) {
//         dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG_BIT_MASK);
//         dwt_readrxdata(rx_buffer[counter], BUF_LEN, 0);
//         rx_ts[counter] = get_rx_timestamp_u64();
//         if (rx_buffer[counter][MSG_SID_IDX] != target_uids[counter]) {
//             dwt_write32bitreg(SYS_STATUS_ID,
//                               SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);
//             counter = 0;
//             return;
//         }
//         ++counter;
//     } else {
//         tx_msg[MSG_SN_IDX] = frame_seq_nb;
//         tx_msg[MSG_FUNC_IDX] = FUNC_CODE_RESET;
//         dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS_BIT_MASK);
//         dwt_writetxdata((uint16_t)(MSG_LEN), tx_msg, 0);
//         dwt_writetxfctrl((uint16_t)(MSG_LEN), 0, 1);
//         dwt_starttx(DWT_START_TX_IMMEDIATE);
//         dwt_write32bitreg(SYS_STATUS_ID,
//                           SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);
//         counter = 0;
//         Sleep(1);
//         return;
//     }
//     if (counter == NUM_NODES - 1) {
//         /* calculate distance */
//         float clockOffsetRatio;
//         clockOffsetRatio = ((float)dwt_readclockoffset()) / (uint32_t)(1 << 26);
//         real_tx_ts = get_tx_timestamp_u64();
//         current_debug_millis = millis();
//         Serial.print(current_debug_millis - previous_debug_millis);
//         Serial.print("ms\t");
//         for (int i = 0; i < counter; i++) {
//             resp_msg_get_ts(&rx_buffer[i][UID], &t_reply);
//             t_round = rx_ts[i] - real_tx_ts;
//             tof = ((t_round - t_reply * (1 - clockOffsetRatio)) / 2.0) *
//                   DWT_TIME_UNITS;
//             distance = tof * SPEED_OF_LIGHT;

//             // Apply Kalman filter to the distance
//             // double filtered_distance = kalman_filter(distance);

//             Serial.print(rx_buffer[i][MSG_SID_IDX]);
//             Serial.print("\t");
//             Serial.print(rx_buffer[i][MSG_SN_IDX]);
//             Serial.print("\t");
//             snprintf(dist_str, sizeof(dist_str), "%3.2f m\t", distance);
//             // snprintf(dist_str, sizeof(dist_str), "%3.2f m\t", filtered_distance);
//             Serial.print(dist_str);
//         }
//         Serial.print("\t");
//         Serial.print(frame_seq_nb);
//         Serial.println();
//         previous_debug_millis = current_debug_millis;
//         counter = 0;
//         ++frame_seq_nb;
//         Sleep(INTERVAL);
//     }
// }

// void responder() {
//     dwt_rxenable(DWT_START_RX_IMMEDIATE);

//     while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) &
//              (SYS_STATUS_RXFCG_BIT_MASK | SYS_STATUS_ALL_RX_ERR))) {
//     };

//     if (status_reg & SYS_STATUS_RXFCG_BIT_MASK) {
//         dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG_BIT_MASK);
//         dwt_readrxdata(rx_buffer[counter], BUF_LEN, 0);
//         if (rx_buffer[counter][MSG_FUNC_IDX] == FUNC_CODE_RESET) {
//             counter = 0;
//             return;
//         }
//         if (rx_buffer[counter][MSG_SID_IDX] != target_uids[counter]) {
//             dwt_write32bitreg(SYS_STATUS_ID,
//                               SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);
//             counter = 0;
//             return;
//         }
//         rx_ts[counter] = get_rx_timestamp_u64();
//         ++counter;
//     } else {
//         dwt_write32bitreg(SYS_STATUS_ID,
//                           SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);
//         counter = 0;
//         return;
//     }
//     if (counter == POLL_NUM) {
//         tx_time =
//             (rx_ts[POLL_NUM - 1] + (RX_TO_TX_DLY_UUS * UUS_TO_DWT_TIME)) >> 8;
//         desired_tx_ts =
//             (((uint64_t)(tx_time & 0xFFFFFFFEUL)) << 8) + TX_ANT_DLY;
//         dwt_setdelayedtrxtime(tx_time);
//         for (int i = 0; i < counter; i++) {
//             resp_msg_set_ts(&tx_msg[target_uids[i]], desired_tx_ts - rx_ts[i]);
//         }
//         tx_msg[MSG_SN_IDX] = frame_seq_nb;
//         dwt_writetxdata((u_int16_t)(MSG_LEN), tx_msg, 0);
//         dwt_writetxfctrl((u_int16_t)(MSG_LEN), 0, 1);
//         ret = dwt_starttx(DWT_START_TX_DELAYED | DWT_RESPONSE_EXPECTED);
//         if (ret == DWT_SUCCESS) {
//             while (!(dwt_read32bitreg(SYS_STATUS_ID) &
//                      SYS_STATUS_TXFRS_BIT_MASK)) {
//             };
//             dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS_BIT_MASK);
//         }
//     }
//     if (counter == NUM_NODES - 1) {
//         float clockOffsetRatio;
//         clockOffsetRatio = ((float)dwt_readclockoffset()) / (uint32_t)(1 << 26);
//         real_tx_ts = get_tx_timestamp_u64();
//         // Serial.print(desired_tx_ts);
//         // Serial.print("\t");
//         // Serial.print(real_tx_ts);
//         // Serial.print("\t");
//         current_debug_millis = millis();
//         Serial.print(current_debug_millis - previous_debug_millis);
//         Serial.print("ms\t");
//         for (int i = REPORT_DISTANCE_FROM; i < counter; i++) {
//             resp_msg_get_ts(&rx_buffer[i][UID], &t_reply);
//             t_round = rx_ts[i] - real_tx_ts;
//             tof = ((t_round - t_reply * (1 - clockOffsetRatio)) / 2.0) *
//                   DWT_TIME_UNITS;
//             distance = tof * SPEED_OF_LIGHT;

//             // Apply Kalman filter to the distance
//             // double filtered_distance = kalman_filter(distance);

//             Serial.print(rx_buffer[i][MSG_SID_IDX]);
//             Serial.print("\t");
//             Serial.print(rx_buffer[i][MSG_SN_IDX]);
//             Serial.print("\t");
//             snprintf(dist_str, sizeof(dist_str), "%3.2f m\t", distance);
//             // snprintf(dist_str, sizeof(dist_str), "%3.2f m\t", filtered_distance);
//             Serial.print(dist_str);
//         }
//         Serial.println();
//         previous_debug_millis = current_debug_millis;
//         counter = 0;
//         ++frame_seq_nb;
//     }
// }











// ------------------------------------------- Set coordinate system -------------------------------------------

#include "dm_sstwr/uwb.h"
#include "dw3000.h"
#include <math.h>

dwt_config_t config = {
    5,            /* Channel number. */
    DWT_PLEN_128, /* Preamble length. Used in TX only. */
    DWT_PAC8,     /* Preamble acquisition chunk size. Used in RX only. */
    9,            /* TX preamble code. Used in TX only. */
    9,            /* RX preamble code. Used in RX only. */
    1, /* 0 to use standard 8 symbol SFD, 1 to use non-standard 8 symbol, 2
          for non-standard 16 symbol SFD and 3 for 4z 8 symbol SDF type */
    DWT_BR_6M8,       /* Data rate. */
    DWT_PHRMODE_STD,  /* PHY header mode. */
    DWT_PHRRATE_STD,  /* PHY header rate. */
    (129 + 8 - 8),    /* SFD timeout (preamble length + 1 + SFD length - PAC
                         size).    Used in RX only. */
    DWT_STS_MODE_OFF, /* STS disabled */
    DWT_STS_LEN_64,   /* STS length see allowed values in Enum
                       * dwt_sts_lengths_e
                       */
    DWT_PDOA_M0       /* PDOA mode off */
};
extern dwt_txconfig_t txconfig_options;

uint8_t tx_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 0, 0, UID, 0, FUNC_CODE_INTER,
                    0,    0,    0, 0,    0,    0, 0, 0,   0, 0,
                    0,    0,    0, 0,    0,    0, 0, 0,   0, 0,
                    0,    0,    0, 0,    0,    0};
uint8_t rx_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 0, 0, UID, 0, FUNC_CODE_INTER,
                    0,    0,    0, 0,    0,    0, 0, 0,   0, 0,
                    0,    0,    0, 0,    0,    0, 0, 0,   0, 0,
                    0,    0,    0, 0,    0,    0};

uint8_t frame_seq_nb = 0;
uint8_t rx_buffer[NUM_NODES - 1][BUF_LEN];

uint32_t status_reg = 0;
int counter = 0;
int ret;

uint64_t rx_ts[NUM_NODES - 1];
uint64_t desired_tx_ts, real_tx_ts;
uint32_t tx_time;
uint32_t t_reply;
uint64_t t_round;
double tof, distance;
unsigned long previous_debug_millis = 0;
unsigned long current_debug_millis = 0;
int millis_since_last_serial_print;

int target_uids[NUM_NODES - 1];
double d1, d2;

void set_target_uids() {
/*
 * U1 is the initiator, U2 - U6 are responders
 * U1 - U6 are the target UIDs
 */
#ifdef MAIN_U1
    switch (NUM_NODES) {
        case 6:
            target_uids[4] = U6;
        case 5:
            target_uids[3] = U5;
        case 4:
            target_uids[2] = U4;
        case 3:
            target_uids[1] = U3;
        case 2:
            target_uids[0] = U2;
        default:
            break;
    }
#elif defined(MAIN_U2)
    switch (NUM_NODES) {
        case 6:
            target_uids[4] = U6;
        case 5:
            target_uids[3] = U5;
        case 4:
            target_uids[2] = U4;
        case 3:
            target_uids[1] = U3;
        case 2:
            target_uids[0] = U1;
        default:
            break;
    }
#elif defined(MAIN_U3)
    switch (NUM_NODES) {
        case 6:
            target_uids[4] = U6;
        case 5:
            target_uids[3] = U5;
        case 4:
            target_uids[2] = U4;
        case 3:
            target_uids[1] = U2;
        case 2:
            target_uids[0] = U1;
        default:
            break;
    }
#elif defined(MAIN_U4)
    switch (NUM_NODES) {
        case 6:
            target_uids[4] = U6;
        case 5:
            target_uids[3] = U5;
        case 4:
            target_uids[2] = U3;
        case 3:
            target_uids[1] = U2;
        case 2:
            target_uids[0] = U1;
        default:
            break;
    }
#elif defined(MAIN_U5)
    switch (NUM_NODES) {
        case 6:
            target_uids[4] = U6;
        case 5:
            target_uids[3] = U4;
        case 4:
            target_uids[2] = U3;
        case 3:
            target_uids[1] = U2;
        case 2:
            target_uids[0] = U1;
        default:
            break;
    }
#elif defined(MAIN_U6)
    switch (NUM_NODES) {
        case 6:
            target_uids[4] = U5;
        case 5:
            target_uids[3] = U4;
        case 4:
            target_uids[2] = U3;
        case 3:
            target_uids[1] = U2;
        case 2:
            target_uids[0] = U1;
        default:
            break;
    }
#endif
}

void start_uwb() {
    while (!dwt_checkidlerc()) {
        UART_puts("IDLE FAILED\r\n");
        while (1);
    }

    if (dwt_initialise(DWT_DW_INIT) == DWT_ERROR) {
        UART_puts("INIT FAILED\r\n");
        while (1);
    }

    dwt_setleds(DWT_LEDS_DISABLE);

    if (dwt_configure(&config)) {
        UART_puts("CONFIG FAILED\r\n");
        while (1);
    }

    dwt_configuretxrf(&txconfig_options);
    dwt_setrxantennadelay(RX_ANT_DLY);
    dwt_settxantennadelay(TX_ANT_DLY);
    dwt_setrxaftertxdelay(TX_TO_RX_DLY_UUS);
#ifdef INITIATOR
    dwt_setrxtimeout(RX_TIMEOUT_UUS);
#else
    dwt_setrxtimeout(0);
#endif
    dwt_setlnapamode(DWT_LNA_ENABLE | DWT_PA_ENABLE);

    set_target_uids();
    Serial.println(APP_NAME);
    Serial.println(UID);
    Serial.println("Setup over........");
}

void calculate_tag_position(double d1, double d2, double* pos_x, double* pos_y) {
    double x1 = 0, y1 = 0; // Anchor 1 position
    double x2 = 1, y2 = 0; // Anchor 2 position

    double A = -2 * x1 + 2 * x2;
    double B = -2 * y1 + 2 * y2;
    double C = d1 * d1 - d2 * d2 - x1 * x1 + x2 * x2 - y1 * y1 + y2 * y2;

    *pos_x = C / A;
    *pos_y = sqrt(d1 * d1 - (*pos_x - x1) * (*pos_x - x1));
}

// void calculate_tag_position(double d1, double d2, double* pos_x, double* pos_y) {
//     double c = 1;
//     double cos_a = (d2 * d2 + c*c - d1 * d1) / (2 * d2 * c);

//     *pos_x = d2 * cos_a;
//     *pos_y = d2 * sqrt(1 - cos_a * cos_a);
// } 
// def tag_pos(a, b, c):
//     cos_a = (b * b + c*c - a * a) / (2 * b * c)
//     x = b * cos_a
//     y = b * cmath.sqrt(1 - cos_a * cos_a)
//     return round(x.real, 1), round(y.real, 1) 

void initiator() {
    if (counter == 0) {
        tx_msg[MSG_SN_IDX] = frame_seq_nb;
        tx_msg[MSG_FUNC_IDX] = FUNC_CODE_INTER;
        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS_BIT_MASK);
        dwt_writetxdata((uint16_t)(MSG_LEN), tx_msg, 0);
        dwt_writetxfctrl((uint16_t)(MSG_LEN), 0, 1);
        dwt_starttx(DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);
    } else {
        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS_BIT_MASK);
        dwt_rxenable(DWT_START_RX_IMMEDIATE);
    }

    while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) &
             (SYS_STATUS_RXFCG_BIT_MASK | SYS_STATUS_ALL_RX_TO |
              SYS_STATUS_ALL_RX_ERR))) {
    };

    if (status_reg & SYS_STATUS_RXFCG_BIT_MASK) {
        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG_BIT_MASK);
        dwt_readrxdata(rx_buffer[counter], BUF_LEN, 0);
        rx_ts[counter] = get_rx_timestamp_u64();
        if (rx_buffer[counter][MSG_SID_IDX] != target_uids[counter]) {
            dwt_write32bitreg(SYS_STATUS_ID,
                              SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);
            counter = 0;
            return;
        }
        ++counter;
    } else {
        tx_msg[MSG_SN_IDX] = frame_seq_nb;
        tx_msg[MSG_FUNC_IDX] = FUNC_CODE_RESET;
        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS_BIT_MASK);
        dwt_writetxdata((uint16_t)(MSG_LEN), tx_msg, 0);
        dwt_writetxfctrl((uint16_t)(MSG_LEN), 0, 1);
        dwt_starttx(DWT_START_TX_IMMEDIATE);
        dwt_write32bitreg(SYS_STATUS_ID,
                          SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);
        counter = 0;
        Sleep(1);
        return;
    }
    if (counter == NUM_NODES - 1) {
        /* calculate distance */
        float clockOffsetRatio;
        clockOffsetRatio = ((float)dwt_readclockoffset()) / (uint32_t)(1 << 26);
        real_tx_ts = get_tx_timestamp_u64();
        current_debug_millis = millis();
        Serial.print(current_debug_millis - previous_debug_millis);
        Serial.print("ms\t");
        for (int i = 0; i < counter; i++) {
            resp_msg_get_ts(&rx_buffer[i][UID], &t_reply);
            t_round = rx_ts[i] - real_tx_ts;
            tof = ((t_round - t_reply * (1 - clockOffsetRatio)) / 2.0) *
                  DWT_TIME_UNITS;
            distance = tof * SPEED_OF_LIGHT;
            Serial.print(rx_buffer[i][MSG_SID_IDX]);
            Serial.print("\t");
            Serial.print(rx_buffer[i][MSG_SN_IDX]);
            Serial.print("\t");
            snprintf(dist_str, sizeof(dist_str), "%3.2f m\t", distance);
            Serial.print(dist_str);

            if (i == 0) {
                d1 = distance;  // distance to first anchor
            } else if (i == 1) {
                d2 = distance;  // distance to second anchor
            }
        }
        
        if (counter == 2) {
            double pos_x, pos_y;
            calculate_tag_position(d1, d2, &pos_x, &pos_y);
            Serial.print("Tag position: (");
            Serial.print(pos_x);
            Serial.print(", ");
            Serial.print(pos_y);
            Serial.println(")");
        }

        Serial.print("\t");
        Serial.print(frame_seq_nb);
        Serial.println();
        previous_debug_millis = current_debug_millis;
        counter = 0;
        ++frame_seq_nb;
        Sleep(INTERVAL);
    }
}

void responder() {
    dwt_rxenable(DWT_START_RX_IMMEDIATE);

    while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) &
             (SYS_STATUS_RXFCG_BIT_MASK | SYS_STATUS_ALL_RX_ERR))) {
    };

    if (status_reg & SYS_STATUS_RXFCG_BIT_MASK) {
        dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG_BIT_MASK);
        dwt_readrxdata(rx_buffer[counter], BUF_LEN, 0);
        if (rx_buffer[counter][MSG_FUNC_IDX] == FUNC_CODE_RESET) {
            counter = 0;
            return;
        }
        if (rx_buffer[counter][MSG_SID_IDX] != target_uids[counter]) {
            dwt_write32bitreg(SYS_STATUS_ID,
                              SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);
            counter = 0;
            return;
        }
        rx_ts[counter] = get_rx_timestamp_u64();
        ++counter;
    } else {
        dwt_write32bitreg(SYS_STATUS_ID,
                          SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);
        counter = 0;
        return;
    }
    if (counter == POLL_NUM) {
        tx_time =
            (rx_ts[POLL_NUM - 1] + (RX_TO_TX_DLY_UUS * UUS_TO_DWT_TIME)) >> 8;
        desired_tx_ts =
            (((uint64_t)(tx_time & 0xFFFFFFFEUL)) << 8) + TX_ANT_DLY;
        dwt_setdelayedtrxtime(tx_time);
        for (int i = 0; i < counter; i++) {
            resp_msg_set_ts(&tx_msg[target_uids[i]], desired_tx_ts - rx_ts[i]);
        }
        tx_msg[MSG_SN_IDX] = frame_seq_nb;
        dwt_writetxdata((u_int16_t)(MSG_LEN), tx_msg, 0);
        dwt_writetxfctrl((u_int16_t)(MSG_LEN), 0, 1);
        ret = dwt_starttx(DWT_START_TX_DELAYED | DWT_RESPONSE_EXPECTED);
        if (ret == DWT_SUCCESS) {
            while (!(dwt_read32bitreg(SYS_STATUS_ID) &
                     SYS_STATUS_TXFRS_BIT_MASK)) {
            };
            dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS_BIT_MASK);
        }
    }
    if (counter == NUM_NODES - 1) {
        float clockOffsetRatio;
        clockOffsetRatio = ((float)dwt_readclockoffset()) / (uint32_t)(1 << 26);
        real_tx_ts = get_tx_timestamp_u64();
        // Serial.print(desired_tx_ts);
        // Serial.print("\t");
        // Serial.print(real_tx_ts);
        // Serial.print("\t");
        current_debug_millis = millis();
        Serial.print(current_debug_millis - previous_debug_millis);
        Serial.print("ms\t");
        for (int i = REPORT_DISTANCE_FROM; i < counter; i++) {
            resp_msg_get_ts(&rx_buffer[i][UID], &t_reply);
            t_round = rx_ts[i] - real_tx_ts;
            tof = ((t_round - t_reply * (1 - clockOffsetRatio)) / 2.0) *
                  DWT_TIME_UNITS;
            distance = tof * SPEED_OF_LIGHT;
            Serial.print(rx_buffer[i][MSG_SID_IDX]);
            Serial.print("\t");
            Serial.print(rx_buffer[i][MSG_SN_IDX]);
            Serial.print("\t");
            snprintf(dist_str, sizeof(dist_str), "%3.2f m\t", distance);
            Serial.print(dist_str);

            if (i == 0) {
                d1 = distance;  // distance to first anchor
            } else if (i == 1) {
                d2 = distance;  // distance to second anchor
            }
        }
        
        if (counter == 2) {
            double pos_x, pos_y;
            calculate_tag_position(d1, d2, &pos_x, &pos_y);
            Serial.print("Tag position: (");
            Serial.print(pos_x, 1);
            Serial.print(", ");
            Serial.print(pos_y, 1);
            Serial.println(")");
        }

        Serial.println();
        previous_debug_millis = current_debug_millis;
        counter = 0;
        ++frame_seq_nb;
    }
}
