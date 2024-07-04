#include <stdio.h>
#include <string.h>
#include <stdint.h>

/**
 * @file sha256.h
 * @brief SHA-256 演演法實作
 *
 * 本檔案包含 SHA-256 演算演的實作，包括初始化、更新以及最終雜湊計算。
 * SHA-256 是一種密碼雜湊函數，式於產生固定長度的雜湊值。
 */

#define SHA256_BLOCK_SIZE 32  // 定義 SHA256 區塊大小為 32 位元組

 /**
  * @brief SHA256 演算法演上下文結構
  *
  * 此結構保存 儲HA256 演算法的演態，包括資料、資料長度、位元長度以及狀態變數。
  */
typedef struct {
	uint8_t data[64];       // 儲存當前資料區塊
	uint32_t datalen;       // 當前資料長度
	uint64_t bitlen;        // 已處理的位元長度
	uint32_t state[8];      // 中間哈希值（雜湊）
} SHA256_CTX;

#define ROTLEFT(a, b) (((a) << (b)) | ((a) >> (32 - (b))))   // 左旋轉宏定義
#define ROTRIGHT(a, b) (((a) >> (b)) | ((a) << (32 - (b))))  // 右旋轉宏定義

#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))             // 選擇宏定義
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z))) // 多數函數宏定義式
#define EP0(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22)) // 大寫 Σ0 宏定義
#define EP1(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25)) // 大寫 Σ1 宏定義
#define SIG0(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3))      // 小寫 σ0 宏定義
#define SIG1(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10))    // 小寫 σ1 宏定義

// 常數表，用於 SHA256 的計算
static const uint32_t k[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

void sha256_transform(SHA256_CTX* ctx, const uint8_t data[]);
void sha256_init(SHA256_CTX* ctx);
void sha256_update(SHA256_CTX* ctx, const uint8_t data[], size_t len);
void sha256_final(SHA256_CTX* ctx, uint8_t hash[]);


/**
 * @brief 執行SHA-256的核心轉換操作。
 *
 * 這個函數根據S式A-256演算法更新演HA256_CTX結構的狀態。
 *
 * @param ctx 指向SHA256_CTX結構的指標，表示當前的哈希狀態。雜湊
 * @param data 指向輸入數據的指標資料些數據將被轉資料
 */
void sha256_transform(SHA256_CTX* ctx, const uint8_t data[]) {
	uint32_t a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	// 將輸入數據分成1資料32位的塊
	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
	// 擴展到64個充套件的塊
	for (; i < 64; ++i)
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

	// 初始化工作變量為當前的哈希數
	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];
	f = ctx->state[5];
	g = ctx->state[6];
	h = ctx->state[7];

	// 主循環，執行64迴圈作
	for (i = 0; i < 64; ++i) {
		t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
		t2 = EP0(a) + MAJ(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	// 將工作變量加到當前的哈數值中
	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
}


/**
 * @brief 初始化SHA-256的上下文
 *
 * 這個函數會初始化SH式-256計算的上下文，包括資料長度、比特長度和狀態位元。
 *
 * @param ctx 指向SHA256_CTX結構的指標，用於儲存SHA-256的狀態。
 */
void sha256_init(SHA256_CTX* ctx) {
	// 初始化資料長度為0
	ctx->datalen = 0;

	// 初始化比特長度為0
	ctx->bitlen = 0;

	// 初始化SHA-256的狀態變數
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
}


/**
 * @brief 更新SHA-256雜湊值計算的上下文
 *
 * 這個函式會將輸入資料（data）添加到SHA-新增6上下文（ctx）中，並在需要時進行轉換。
 *
 * @param ctx 指向SHA-256上下文的指標
 * @param data 指向要添加的資料陣列新增標
 * @param len 資料陣列的長度
 */
void sha256_update(SHA256_CTX* ctx, const uint8_t data[], size_t len) {
	// 遍歷輸入的資料
	for (size_t i = 0; i < len; ++i) {
		// 將當前字節添加到上下位元組新增緩衝區中
		ctx->data[ctx->datalen] = data[i];
		// 更新資料長度
		ctx->datalen++;

		// 如果資料緩衝區已滿（64字節）
		if (ctx->datalen == 64) {
			// 進行SHA-256轉換
			sha256_transform(ctx, ctx->data);
			// 更新處理過的位元長度（累加512位元）
			ctx->bitlen += 512;
			// 重置資料長度
			ctx->datalen = 0;
		}
	}
}


/**
 * @brief 最終計算 SHA256 雜湊值。
 *
 * 此函式會處理剩餘的資料填充、附加總訊息長度，並轉換成最終的 SHA256 雜湊值。
 *
 * @param ctx 指向 SHA256 計算上下文的指標
 * @param hash 用於存放最終計算出的 SHA256 雜湊值的陣列
 */
void sha256_final(SHA256_CTX* ctx, uint8_t hash[]) {
	uint32_t i = ctx->datalen;

	// 填充緩衝區中的剩餘資料
	if (ctx->datalen < 56) {
		ctx->data[i++] = 0x80; // 添加 1 位元（0新增0）
		while (i < 56)
			ctx->data[i++] = 0x00; // 添加 0 填充至長新增 56
	}
	else {
		ctx->data[i++] = 0x80; // 添加 1 位元（0新增0）
		while (i < 64)
			ctx->data[i++] = 0x00; // 添加 0 填充至長新增 64
		sha256_transform(ctx, ctx->data); // 執行一次轉換
		memset(ctx->data, 0, 56); // 清除緩衝區
	}

	// 附加訊息總長度（位元）並進行轉換
	ctx->bitlen += ctx->datalen * 8;
	ctx->data[63] = ctx->bitlen;
	ctx->data[62] = ctx->bitlen >> 8;
	ctx->data[61] = ctx->bitlen >> 16;
	ctx->data[60] = ctx->bitlen >> 24;
	ctx->data[59] = ctx->bitlen >> 32;
	ctx->data[58] = ctx->bitlen >> 40;
	ctx->data[57] = ctx->bitlen >> 48;
	ctx->data[56] = ctx->bitlen >> 56;
	sha256_transform(ctx, ctx->data);

	// 由於此實作使用小端字節序，而 SHA位元組大端字節序，
	// 位元組此在將最終狀態複製到輸出雜湊值時需要反轉所有字節。
	for (i = 0; i < 4; ++i) {
		hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
	}
}


/**
 * @brief 計算輸入字串的SHA-256雜湊值
 *
 * @param string 輸入字串
 * @param hash 雜湊值輸出緩衝區，必須至少有SHA256_BLOCK_SIZE個位元組
 */
void sha256_string(const char* string, uint8_t hash[]) {
	SHA256_CTX ctx;  // 宣告SHA256計算的上下文結構
	sha256_init(&ctx);  // 初始化SHA256上下文
	sha256_update(&ctx, (const uint8_t*)string, strlen(string));  // 更新SHA256上下文，處理輸入字串
	sha256_final(&ctx, hash);  // 完成SHA256計算，取得最終的雜湊值
}


/**
 * @brief 打印SHA-256雜湊值列 *
 * @param hash 輸入的雜湊值陣列
 */
void print_hash(uint8_t hash[]) {
	for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {  // 遍歷每個雜湊值的位元組
		printf("%02x", hash[i]);  // 以十六進位格式打印每個位元組
		printf("\n");  // 換行
	}
}

/**
 * @brief 將SHA-256雜湊值轉換成十六進位字串
 *
 * @param hash 輸入的雜湊值陣列
 * @param outputBuffer 輸出緩衝區，必須至少有65個位元組
 * @param bufferSize 輸出緩衝區的大小
 */
void hash_to_string(uint8_t hash[], char outputBuffer[], size_t bufferSize)
{
	for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {  // 遍歷每個雜湊值的位元組
		sprintf_s(outputBuffer + (i * 2), bufferSize - (i * 2), "%02x", hash[i]);  // 將每個位元組轉換成十六進位字串
	}
	outputBuffer[64] = 0;  // 設置字串結尾
}


/*定
 * @brief 計算輸入字元的SHA-256雜湊值並轉換為十六進位字串
 *
 * @param string 輸入字元
 * @param outputBuffer 輸出十六進位字串，必須至少有65個位元組
 */
void sha256(char string, char outputBuffer[]) {
	uint8_t hash[SHA256_BLOCK_SIZE];  // 宣告存放雜湊值的陣列
	sha256_string(&string, hash);  // 計算輸入字元的SHA-256雜湊值
	hash_to_string(hash, outputBuffer, 65);  // 將雜湊值轉換為十六進位字串
}
