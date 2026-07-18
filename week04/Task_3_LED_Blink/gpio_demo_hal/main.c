#include "stm32f1xx_hal.h"

// Khai báo hàm cấu hình GPIO
void MX_GPIO_Init(void);

int main(void) {
    // 1. Khởi tạo thư viện HAL (Thiết lập SysTick timer để dùng hàm HAL_Delay)
    HAL_Init(); 
    
    // 2. Khởi tạo cấu hình cho chân GPIO
    MX_GPIO_Init();

    // 3. Vòng lặp vô hạn
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

    // ===================================================================
    // RÀNG BUỘC KỸ THUẬT: Bật APB2 Clock cho Port C TRƯỚC KHI cấu hình
    // ===================================================================
    // Macro này sẽ ghi vào thanh ghi RCC_APB2ENR để cấp xung nhịp cho GPIOC
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Cấu hình linh hoạt cho chân PC13
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   // Chế độ Digital Output Push-Pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;           // Không cấu hình điện trở kéo bên trong
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;  // Tốc độ đáp ứng thấp (đủ để nháy LED)

    // Áp dụng các cấu hình trên vào thanh ghi thực tế của vi điều khiển
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

// Cần định nghĩa hàm SysTick_Handler để HAL_Delay() hoạt động chính xác
void SysTick_Handler(void) {
    HAL_IncTick();
}