#include "stm32f1xx_hal.h"

// Khai báo hàm cấu hình GPIO
void MX_GPIO_Init(void);

int main(void) {
    HAL_Init(); 
    
    MX_GPIO_Init();

    while(1) {
        // Đảo trạng thái (Toggle) chân PC13
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        
        // Trễ 500ms (500ms Sáng + 500ms Tắt = Chu kỳ 1 giây)
        HAL_Delay(5000); 
    }
}

/**
  * @brief Cấu hình GPIO
  */
void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Cấu hình linh hoạt cho chân PC13
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   // Chế độ Digital Output Push-Pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;           // Không cấu hình điện trở kéo bên trong
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;  // Tốc độ đáp ứng thấp (đủ để nháy LED)

    
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void SysTick_Handler(void) {
    HAL_IncTick();
}