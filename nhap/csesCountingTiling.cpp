#include <bits/stdc++.h>
using namespace std;

// Hàm tính số cách xếp domino trong bảng 3 x n

const long long MOD = 1e9 + 7;

int n, m;

long long dp[1001][1 << 10];


void sinh(int cot, int hang, int maskHienTai, int maskTiepTheo){

    if (hang == n){
        dp[cot + 1][maskTiepTheo] = (dp[cot + 1][maskTiepTheo] + (dp[cot][maskHienTai] % MOD)) % MOD; // lưu kết quả -- và --  đánh dấu là cái này có khả năng xảy ra hay ko 
        
        return;
    }

    if ((maskHienTai >> hang) & 1){ // nghĩa là ô này bị chặn rồi
        sinh(cot, hang + 1, maskHienTai, maskTiepTheo);
    }
    else{ // nếu không bị chặn
        // đặt ngang
        sinh(cot, hang + 1, maskHienTai, maskTiepTheo | (1 << hang)); // đặt ngang thì sẽ chặn ô tiếp theo nén cho nó bật bit lên 

        if (hang + 1 < n && !((maskHienTai >> (hang + 1)) & 1) ){ // tức là hàng tiếp theo phải bé hơn n 
            //-- bên cạnh đó -- hàng tiếp theo không bị chặn
            sinh(cot, hang + 2, maskHienTai, maskTiepTheo);

        }

    }
}


int main(){
    cin >> n >> m;
    
    memset(dp, 0, sizeof(dp));

    dp[0][0] = 1;

    for (int cot = 0; cot < m; cot++){
        for (int bitmask = 0; bitmask < (1 << n); bitmask++){
            if (dp[cot][bitmask] > 0){ // có nghĩa là cái này có khả năng xảy ra, do trước đó đã có cái lấp tới đây rồi
                sinh(cot, 0, bitmask, 0);
            }
        }
    }

    cout << dp[m][0];


}

// bitmask như khả năng xảy ra của một cột vậy, có những trường hợp được, những trường hợp thì không