Chào mọi người.

Mình là Jack.

Mình muốn gửi cho các bạn bài quiz thứ 2. Không phải bài kiểm tra!

Các bạn có thể viết chương trình bằng Python hoặc C++.

Vui lòng gửi mã nguồn khi đã sẵn sàng, nếu có thể, trước cuối tuần.

Và, không trao đổi ý tưởng với nhau, hãy tự thực hiện theo phương pháp riêng của bạn.

Mục tiêu: Tính khoảng trống giữa lớp vỏ trong của pin và bên trong cuộn jelly hoặc separator bằng cách thực hiện phát hiện cạnh trên hình ảnh.

Định dạng file

Input: ảnh xám 1 kênh, 16-bit

Output: ảnh màu 3 kênh, 8-bit

Chi tiết input

Mỗi ảnh tương ứng với một trong tám điểm kiểm tra đã được định nghĩa trước (P1–P8) của pin.

Với mỗi điểm, xác định vùng quan tâm (ROI) để thực hiện phát hiện cạnh. ROI phải có thể tham số hóa.

Thực hiện phát hiện cạnh để đo khoảng trống giữa lớp vỏ trong của pin và cuộn jelly hoặc separator bên trong.

Tham khảo: “Px_result.png”

Lưu ý: Định nghĩa các điểm ảnh (P1–P8)

P1: Vị trí trên-phải trong vùng trung tâm của pin dạng prismatic.

P2: Vị trí trên-trái trong vùng trung tâm của pin dạng prismatic.

P3: Vị trí dưới-trái trong vùng trung tâm của pin dạng prismatic.

P4: Vị trí dưới-phải trong vùng trung tâm của pin dạng prismatic.

P5: Vị trí trên-giữa bên trái của pin dạng prismatic (khoảng 1/3 chiều cao pin).

P6: Vị trí dưới-giữa bên trái của pin dạng prismatic (khoảng 2/3 chiều cao pin).

P7: Vị trí trên-giữa bên phải của pin dạng prismatic (khoảng 1/3 chiều cao pin).

P8: Vị trí dưới-giữa bên phải của pin dạng prismatic (khoảng 2/3 chiều cao pin).

Lưu ý:
Do separator có thể bị rách hoặc sờn một phần, việc phát hiện cạnh và đo khoảng trống chỉ được thực hiện trên vùng nguyên vẹn (không rách) của separator.