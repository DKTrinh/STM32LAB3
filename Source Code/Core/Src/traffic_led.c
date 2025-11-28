/*
 * traffic_led.c
 *
 * Created on: Oct 27, 2025
 * Author: ADMIN
 */
#include "main.h"
#include "traffic_led.h"
#include "led_display.h"

#define LED_COUNT 3
#define RED     0
#define AMBER   1
#define GREEN   2

static uint8_t ledBuffer[LED_COUNT];
static uint8_t hRed, hAmber, hGreen;
static uint8_t vRed, vAmber, vGreen;
static uint8_t tRed, tAmber, tGreen;

void LED_TRAFFIC_INIT(void){
    // Tắt hết đèn khi khởi động (Active Low: SET = OFF)
    HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, GPIO_PIN_SET);
}

void LED_TRAFFIC_LOAD_BUFFER(void){
    hRed = ledBuffer[RED]; hAmber = ledBuffer[AMBER]; hGreen = ledBuffer[GREEN];
    vRed = ledBuffer[RED]; vAmber = ledBuffer[AMBER]; vGreen = ledBuffer[GREEN];
    tRed = ledBuffer[RED]; tAmber = ledBuffer[AMBER]; tGreen = ledBuffer[GREEN];
}

void LED_TRAFFIC_STORE_BUFFER(uint8_t value, uint8_t color){
    ledBuffer[color] = value;
}

// Giống LED_VERTICAL_RUN trong mẫu: GREEN -> AMBER -> RED
static void updateVertical(void){
    // 1. Chạy Xanh trước
    if(vGreen > 0){
        setVertClockBuffer(vGreen);
        HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, GPIO_PIN_RESET); // Xanh ON
        vGreen--;
    }
    // 2. Sau đó chạy Vàng
    else if(vAmber > 0){
        setVertClockBuffer(vAmber);
        HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, GPIO_PIN_RESET); // Vàng ON
        HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, GPIO_PIN_SET);
        vAmber--;
    }
    // 3. Cuối cùng chạy Đỏ
    else if(vRed > 0){
        setVertClockBuffer(vRed);
        HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, GPIO_PIN_RESET); // Đỏ ON
        HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, GPIO_PIN_SET);
        vRed--;
    }

    // Reset khi cả 3 biến đều về 0 (Logic giống code mẫu)
    if(vGreen == 0 && vAmber == 0 && vRed == 0){
        vRed = tRed; vAmber = tAmber; vGreen = tGreen;
    }
}

// Giống LED_HORIZONTAL_RUN trong mẫu: RED -> GREEN -> AMBER
static void updateHorizontal(void){
    // 1. Chạy Đỏ trước
    if(hRed > 0){
        setHoriClockBuffer(hRed);
        HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, GPIO_PIN_RESET); // Đỏ ON
        HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, GPIO_PIN_SET);
        hRed--;
    }
    // 2. QUAN TRỌNG: Sửa thứ tự thành Xanh (giống code mẫu)
    else if(hGreen > 0){
        setHoriClockBuffer(hGreen);
        HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, GPIO_PIN_RESET); // Xanh ON
        hGreen--;
    }
    // 3. Cuối cùng là Vàng (giống code mẫu)
    else if(hAmber > 0){
        setHoriClockBuffer(hAmber);
        HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, GPIO_PIN_RESET); // Vàng ON
        HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, GPIO_PIN_SET);
        hAmber--;
    }

    // Reset khi cả 3 biến đều về 0
    if(hRed == 0 && hGreen == 0 && hAmber == 0){
        hRed = tRed; hAmber = tAmber; hGreen = tGreen;
    }
}

void LED_TRAFFIC_RUN(void){
    updateHorizontal();
    updateVertical();
}

uint8_t ERROR_HANDLING(uint8_t value){
    if(value < 1 || value > 99) return 0;
    return 1;
}
