#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

using namespace std;

// Cấu trúc của một nút trong cây Trie
struct NutTrie {
    // Mỗi nút sẽ lưu một map, với key là ký tự và value là con trỏ đến nút con
    unordered_map<char, unique_ptr<NutTrie>> cacCon;
    
    // Biến cờ để đánh dấu đây có phải là ký tự kết thúc của một từ hợp lệ không (ví dụ từ kết thúc của từ "sách" là "h")
    bool laKetThucTu;

    // Hàm khởi tạo cho một nút mới
    NutTrie() : laKetThucTu(false) {}
};

class CayTrie {
private:
    // Nút gốc của cây
    unique_ptr<NutTrie> goc;

    void timTatCaTu(const NutTrie* nut, string tuHienTai, vector<string>& goiY) const {
        // Nếu nút không tồn tại thì dừng lại
        if (!nut) return;

        // Nếu nút này là điểm kết thúc của một từ, thêm từ đó vào danh sách gợi ý
        if (nut->laKetThucTu) {
            goiY.push_back(tuHienTai);
        }
        
        // Duyệt qua tất cả các con của nút hiện tại
        for (const auto& cap : nut->cacCon) {
            char kyTuTiepTheo = cap.first;
            const NutTrie* nutCon = cap.second.get();
            
            // Gọi đệ quy để tiếp tục tìm kiếm ở nhánh tiếp theo
            timTatCaTu(nutCon, tuHienTai + kyTuTiepTheo, goiY);
        }
    }

public:
    CayTrie() : goc(make_unique<NutTrie>()) {}

    void chen(const string& tu) {
        NutTrie* hienTai = goc.get();
        for (char kyTu : tu) {
            // Nếu ký tự (byte) chưa tồn tại trong map các con, tạo một nút mới
            if (hienTai->cacCon.find(kyTu) == hienTai->cacCon.end()) {
                hienTai->cacCon[kyTu] = make_unique<NutTrie>();
            }
            // Di chuyển con trỏ 'hienTai' xuống nút con
            hienTai = hienTai->cacCon[kyTu].get();
        }
        // Sau khi duyệt hết từ, đánh dấu nút cuối cùng là kết thúc của một từ
        hienTai->laKetThucTu = true;
    }

    vector<string> layGoiY(const string& tienTo) const {
        vector<string> goiY;
        const NutTrie* hienTai = goc.get();

        // Bước 1: Di chuyển đến nút cuối cùng của tiền tố
        for (char kyTu : tienTo) {
            auto it = hienTai->cacCon.find(kyTu);
            // Nếu không tìm thấy đường đi cho tiền tố, trả về danh sách rỗng
            if (it == hienTai->cacCon.end()) {
                return goiY;
            }
            hienTai = it->second.get();
        }

        // Bước 2: Từ nút của tiền tố, gọi hàm đệ quy để tìm tất cả các từ
        timTatCaTu(hienTai, tienTo, goiY);

        return goiY;
    }
};


void inGoiY(const string& tienTo, const vector<string>& goiY) {
    cout << "Goi y \"" << tienTo << "\":\n";
    if (goiY.empty()) {
        cout << "  (khong tim thay)\n";
    } else {
        for (const auto& tu : goiY) {
            cout << "  - " << tu << "\n";
        }
    }
    cout << "--------------------------------\n";
}

int main() {

    
    CayTrie tuDien;
    
    // Ở đây mình sẽ thêm các từ vào từ điển bằng cách thực hiện tuDien.chen()
    
    return 0;
}
/*hướng dẫn sử dụng và cách thức hoạt động cây Trie:
Hãy tưởng tượng Trie như một cuốn từ điển được sắp xếp theo từng chữ cái, thay vì từng từ. Đây chính là bí quyết giúp nó tìm kiếm nhanh như chớp khi bạn gõ.
Cách Hoạt Động và "Quét" Thông Minh:
Hãy xem cách Trie lưu trữ một vài từ đơn giản: cá, cây, cảm, và bóng.
Cấu trúc của nó sẽ trông như thế này:
Từ một nút gốc rỗng, sẽ có hai nhánh chính tách ra: một cho chữ 'c' và một cho chữ 'b'.
Đi theo nhánh 'c', ta sẽ đến một nút mà từ đó lại rẽ ra ba nhánh con: 'á', 'â', và 'ả'.
Nhánh 'á' sẽ dẫn đến nút cuối cùng, được đánh dấu là kết thúc của từ "cá".
Nhánh 'â' dẫn đến nút 'y', và nút 'y' được đánh dấu là kết thúc của từ "cây".
Nhánh 'ả' dẫn đến nút 'm', và nút 'm' được đánh dấu là kết thúc của từ "cảm".
Nhánh 'b' sẽ dẫn đến một chuỗi các nút tạo thành từ "bóng".
Bây giờ, khi bạn gõ tiền tố "câ", điều kỳ diệu xảy ra:
Đi theo đường dẫn: Trie không quét toàn bộ từ điển. Nó ngay lập tức đi từ gốc -> c -> â. Quá trình này chỉ mất 2 bước, tương ứng với 2 ký tự bạn gõ.
Loại bỏ dữ liệu không liên quan: Ngay tại thời điểm này, Trie đã hoàn toàn bỏ qua nhánh 'b' và tất cả các từ không liên quan như "bóng". Đây chính là sức mạnh cốt lõi: nó loại bỏ phần lớn dữ liệu ngay lập tức.
Quét giới hạn: Từ nút 'â', thuật toán mới bắt đầu "quét" các nhánh con bên dưới nó. Nó thấy có một nhánh duy nhất dẫn đến nút 'y'. Khi đến nút 'y', nó thấy nút này được đánh dấu là kết thúc của một từ.
Kết quả: Hệ thống ngay lập tức trả về gợi ý là "cây".
Tóm lại, Trie không quét một cách "mù quáng". Nó dùng tiền tố như một địa chỉ chính xác để nhảy đến một khu vực nhỏ trong dữ liệu, rồi mới thực hiện việc quét giới hạn trong khu vực đó, giúp tốc độ tìm kiếm gần như tức thời.
*/