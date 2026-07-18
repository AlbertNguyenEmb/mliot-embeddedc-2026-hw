#include "stm32f1xx_hal.h" // Sửa lại thư viện nếu bạn dùng dòng khác (VD: stm32f4xx_hal.h)

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void)
{
    // Khởi tạo thư viện HAL và hệ thống
    HAL_Init();
    //SystemClock_Config();
    MX_GPIO_Init();

    while (1)
    {
        uint8_t key_pressed = 0; // Biến lưu trạng thái phím (0: không nhấn, 1: K1, 2: K2, 3: K3, 4: K4)

        /* =========================================
        BƯỚC 1: QUÉT HÀNG 0 (PA0 = 0, PA1 = 1)
        ========================================= */
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); // Kéo Hàng 0 xuống 0V
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);   // Giữ Hàng 1 ở 3.3V
        HAL_Delay(5); // Delay nhỏ để ổn định điện áp (chống nhiễu/debouncing)

        // Đọc trạng thái cột 0 (PA2) -> Nếu = 0 là K1 đang nhấn
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET) {
            key_pressed = 1; 
        }
        // Đọc trạng thái cột 1 (PA3) -> Nếu = 0 là K2 đang nhấn
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET) {
            key_pressed = 2;
        }

        /* =========================================
        BƯỚC 2: QUÉT HÀNG 1 (PA0 = 1, PA1 = 0)
        ========================================= */
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);   // Kéo Hàng 0 lên 3.3V
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET); // Kéo Hàng 1 xuống 0V
        HAL_Delay(5);

        // Đọc trạng thái cột 0 (PA2) -> Nếu = 0 là K3 đang nhấn
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET) {
            key_pressed = 3;
        }
        // Đọc trạng thái cột 1 (PA3) -> Nếu = 0 là K4 đang nhấn
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET) {
            key_pressed = 4;
        }

        /* =========================================
        BƯỚC 3: ĐIỀU KHIỂN LED PHẢN HỒI
        ========================================= */
        // Lưu ý: LED trên board Blue Pill (PC13) thường sáng khi chân bị kéo xuống mức 0 (Active Low).
        
        if (key_pressed != 0) {
            // Nếu có BẤT KỲ phím nào được nhấn -> Bật LED
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); 
        } else {
            // Nếu không có phím nào được nhấn -> Tắt LED
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        }
    }
}

// Hàm khởi tạo GPIO
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Cấp xung nhịp cho port A và C
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // 1. Cấu hình PC13 (LED) làm Digital Output
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Push-Pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // 2. Cấu hình PA0, PA1 (Hàng) làm Digital Output
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 3. Cấu hình PA2, PA3 (Cột) làm Digital Input Pull-up
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP; // Bắt buộc phải có Pull-up
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 4. Trạng thái mặc định khi khởi động:
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_SET); // Hàng mặc định ở mức cao
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);             // Tắt LED PC13 
}