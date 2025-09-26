#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

// Hằng số cho giá trị vô cùng lớn, dùng để khởi tạo
const int INF = 1e9;

// Kích thước ma trận
int n, m;
// Ma trận đầu vào
vector<string> grid;

// Tọa độ các điểm quan trọng
pair<int, int> start_pos, end_pos;
vector<pair<int, int>> checkpoints;

// Các hướng di chuyển: Lên, Xuống, Trái, Phải
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

// Hàm BFS để tính khoảng cách ngắn nhất từ một điểm (r, c) đến tất cả các điểm khác
vector<vector<int>> bfs(int r, int c) {
    vector<vector<int>> dist(n, vector<int>(m, -1)); // -1 nghĩa là chưa thăm
    queue<pair<int, int>> q;

    dist[r][c] = 0;
    q.push({r, c});

    while (!q.empty()) {
        pair<int, int> curr = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int nr = curr.first + dr[i];
            int nc = curr.second + dc[i];

            // Kiểm tra xem tọa độ mới có hợp lệ và chưa được thăm không
            if (nr >= 0 && nr < n && nc >= 0 && nc < m && dist[nr][nc] == -1) {
                dist[nr][nc] = dist[curr.first][curr.second] + 1;
                q.push({nr, nc});
            }
        }
    }
    return dist;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // --- BƯỚC 1: ĐỌC DỮ LIỆU VÀ XÁC ĐỊNH CÁC ĐIỂM QUAN TRỌNG ---
    cout << "Nhap so hang n va so cot m: ";
    cin >> n >> m;
    grid.resize(n);

    cout << "Nhap ma tran (su dung D cho diem dau/cuoi):" << endl;
    bool start_found = false;
    for (int i = 0; i < n; ++i) {
        cin >> grid[i];
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 'D') {
                if (!start_found) {
                    start_pos = {i, j};
                    start_found = true;
                } else {
                    end_pos = {i, j};
                }
            } else if (grid[i][j] == '1') {
                checkpoints.push_back({i, j});
            }
        }
    }
    
    int k = checkpoints.size(); // Số lượng checkpoint

    // Trường hợp đặc biệt: không có checkpoint nào
    if (k == 0) {
        vector<vector<int>> dist_from_start = bfs(start_pos.first, start_pos.second);
        int result = dist_from_start[end_pos.first][end_pos.second];
        if (result == -1) {
            cout << "Khong ton tai duong di." << endl;
        } else {
            cout << "Duong di ngan nhat: " << result << endl;
        }
        return 0;
    }

    // --- BƯỚC 2: TÍNH TRƯỚC KHOẢNG CÁCH GIỮA CÁC ĐIỂM QUAN TRỌNG ---
    
    // dist_s_c[i]: khoảng cách từ Start đến checkpoint i
    vector<int> dist_s_c(k);
    vector<vector<int>> dist_from_start = bfs(start_pos.first, start_pos.second);
    for(int i = 0; i < k; ++i) {
        dist_s_c[i] = dist_from_start[checkpoints[i].first][checkpoints[i].second];
    }
    
    // dist_c_e[i]: khoảng cách từ checkpoint i đến End
    vector<int> dist_c_e(k);
    // dist_c_c[i][j]: khoảng cách từ checkpoint i đến checkpoint j
    vector<vector<int>> dist_c_c(k, vector<int>(k));
    
    for(int i = 0; i < k; ++i) {
        vector<vector<int>> dist_from_ci = bfs(checkpoints[i].first, checkpoints[i].second);
        dist_c_e[i] = dist_from_ci[end_pos.first][end_pos.second];
        for(int j = 0; j < k; ++j) {
            dist_c_c[i][j] = dist_from_ci[checkpoints[j].first][checkpoints[j].second];
        }
    }

    // --- BƯỚC 3: QUY HOẠCH ĐỘNG VỚI BITMASK ---

    // dp[mask][i]: chi phí nhỏ nhất để đi từ Start, thăm các checkpoint trong `mask`, kết thúc tại checkpoint `i`
    vector<vector<int>> dp(1 << k, vector<int>(k, INF));
    
    // Khởi tạo trạng thái ban đầu: đi từ Start đến checkpoint i đầu tiên
    for (int i = 0; i < k; ++i) {
        if (dist_s_c[i] != -1) { // Nếu có đường đi từ Start đến checkpoint i
            dp[1 << i][i] = dist_s_c[i];
        }
    }

    // Vòng lặp chính của DP
    // Duyệt qua tất cả các trạng thái (tập hợp các checkpoint đã đi qua)
    for (int mask = 1; mask < (1 << k); ++mask) {
        // Duyệt qua checkpoint `i` là điểm kết thúc của đường đi hiện tại
        for (int i = 0; i < k; ++i) {
            // Nếu checkpoint `i` có trong `mask`
            if ((mask >> i) & 1) {
                // Thử đi đến `i` từ một checkpoint `j` khác
                // `j` là điểm kết thúc của đường đi cũ
                for (int j = 0; j < k; ++j) {
                    if (i != j && ((mask >> j) & 1)) {
                        // `prev_mask` là trạng thái trước khi đi đến `i`, tức là không có `i`
                        int prev_mask = mask ^ (1 << i);
                        
                        // Nếu trạng thái trước đó có thể đạt được và có đường đi từ j đến i
                        if (dp[prev_mask][j] != INF && dist_c_c[j][i] != -1) {
                            dp[mask][i] = min(dp[mask][i], dp[prev_mask][j] + dist_c_c[j][i]);
                        }
                    }
                }
            }
        }
    }

    // --- BƯỚC 4: TÍNH KẾT QUẢ CUỐI CÙNG ---
    int min_path = INF;
    int final_mask = (1 << k) - 1; // Mask khi đã thăm tất cả các checkpoint

    // Tìm đường đi ngắn nhất bằng cách thử kết thúc tại mỗi checkpoint `i`
    // rồi đi từ `i` đến điểm End
    for (int i = 0; i < k; ++i) {
        if (dp[final_mask][i] != INF && dist_c_e[i] != -1) {
            min_path = min(min_path, dp[final_mask][i] + dist_c_e[i]);
        }
    }

    if (min_path == INF) {
        cout << "Khong ton tai duong di." << endl;
    }else {
        cout << "Duong di ngan nhat la: " << min_path << endl;
    }

    return 0;
}