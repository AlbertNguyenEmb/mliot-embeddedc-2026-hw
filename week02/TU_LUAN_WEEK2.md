*** ĐÁP ÁN TỰ LUẬN HW02 ***

[TASK 4.2] PHÂN BỔ BỘ NHỚ VẬT LÝ:
1. BIKE_MODEL: Nằm trong phân vùng .rodata (Read-Only Data) thuộc bộ nhớ Flash/ROM vì đây là mảng hằng số (const) không bị thay đổi trong quá trình chạy.
2. total_odometer: Nằm trong phân vùng .bss thuộc bộ nhớ RAM, vì đây là biến toàn cục (global) được khởi tạo bằng 0.
3. current_speed: Nằm trong phân vùng Stack thuộc bộ nhớ RAM, vì đây là biến cục bộ (local) được khai báo bên trong phạm vi hàm main().

[TASK 4.3] HIỆN TƯỢNG KHI KÍCH HOẠT CRASH LAB:
- Trên môi trường máy tính (GCC): Chương trình sẽ bị lỗi Segmentation Fault (core dumped) và buộc phải thoát ra, do kích thước Stack của hệ điều hành bị phình to vượt quá giới hạn cấp phép.
- Trên vi điều khiển thực tế (ARM Cortex-M): CPU sẽ phát hiện lỗi truy cập vùng nhớ không hợp lệ hoặc hỏng Stack và lập tức nhảy vào hàm ngắt ngoại lệ "HardFault_Handler" để xử lý sự cố hoặc reset hệ thống.

[TASK 5] TỐI ƯU HÓA LINKER SCRIPT:
- Lựa chọn đúng: Cách 1 (uint16_t trip_history[20000] = {0};)
- Giải thích: 
Khi khởi tạo mảng toàn bằng số 0 (Cách 1), trình biên dịch xếp mảng này vào vùng .bss. Đặc thù của .bss là không tốn không gian lưu trữ trên Flash, nó chỉ báo cho vi điều khiển biết cần cấp phát 40KB trên RAM lúc khởi động. (Thỏa mãn do RAM trống 128KB, Flash chỉ tốn 0KB).
Nếu dùng Cách 2 (khởi tạo với các số 1, 2, 3), mảng sẽ bị xếp vào vùng .data. Vùng .data bắt buộc vi điều khiển phải ghi nhận toàn bộ giá trị ban đầu này cứng vào bộ nhớ Flash. 40KB dữ liệu này sẽ làm tràn bộ nhớ Flash (do chỉ còn trống 20KB), dẫn đến việc Linker báo lỗi tràn bộ nhớ và không thể nạp chương trình xuống chip.