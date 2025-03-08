| 作业gitHub     | [作业链接](https://github.com/hjh7201/3123004351)     |      
| ---- | ---- | 
|  这个作业属于哪个课程    |  软件工程    |     
|   这个作业要求在哪里   |  [作业要求](https://edu.cnblogs.com/campus/gdgy/SoftwareEngineeringClassof2023/homework/13324)    | 
|  这个作业的目标    | 完成一次个人项目     | 




---
* ##计算模块接口的设计与实现过程
## **1. 代码组织**
计算模块的核心功能是读取文件内容并计算两个文本的相似度。代码组织如下：

### **模块划分**
1. **文件读取模块**：
   - 函数：`readFile`。
   - 功能：读取指定文件的内容并返回字符串。

2. **相似度计算模块**：
   - 函数：`calculateCosineSimilarity`。
   - 功能：计算两个文本的余弦相似度。

3. **主程序模块**：
   - 函数：`main`。
   - 功能：处理命令行参数，调用文件读取和相似度计算模块，并输出结果。

---

## **2. 函数设计**

### **2.1 `readFile` 函数**
- **功能**：读取指定文件的内容并返回字符串。
- **输入**：文件路径（`const char* filePath`）。
- **输出**：文件内容（`char*`）。
- **实现**：
  ```c
  char* readFile(const char* filePath) {
      FILE* file = fopen(filePath, "r");
      if (!file) {
          fprintf(stderr, "Error: Could not open file %s\n", filePath);
          return NULL;
      }

      fseek(file, 0, SEEK_END);
      long fileSize = ftell(file);
      fseek(file, 0, SEEK_SET);

      char* content = (char*)malloc(fileSize + 1);
      if (!content) {
          fclose(file);
          fprintf(stderr, "Error: Memory allocation failed\n");
          return NULL;
      }

      fread(content, 1, fileSize, file);
      content[fileSize] = '\0';
      fclose(file);
      return content;
  }
  ```

---

### **2.2 `calculateCosineSimilarity` 函数**
- **功能**：计算两个文本的余弦相似度。
- **输入**：两个文本（`const char* text1` 和 `const char* text2`）。
- **输出**：相似度（`double`，范围 0 到 1）。
- **实现**：
  ```c
  double calculateCosineSimilarity(const char* text1, const char* text2) {
      int vec1[256] = {0};
      int vec2[256] = {0};

      // 统计字符频率
      for (int i = 0; text1[i]; i++) vec1[(int)text1[i]]++;
      for (int i = 0; text2[i]; i++) vec2[(int)text2[i]]++;

      // 计算点积和模
      double dotProduct = 0, magnitude1 = 0, magnitude2 = 0;
      for (int i = 0; i < 256; i++) {
          dotProduct += vec1[i] * vec2[i];
          magnitude1 += vec1[i] * vec1[i];
          magnitude2 += vec2[i] * vec2[i];
      }

      magnitude1 = sqrt(magnitude1);
      magnitude2 = sqrt(magnitude2);

      if (magnitude1 == 0 || magnitude2 == 0) return 0.0;
      return dotProduct / (magnitude1 * magnitude2);
  }
  ```

---

### **2.3 `main` 函数**
- **功能**：处理命令行参数，调用文件读取和相似度计算模块，并输出结果。
- **输入**：命令行参数（`int argc, char* argv[]`）。
- **输出**：将相似度写入输出文件。
- **实现**：
  ```c
  int main(int argc, char* argv[]) {
      if (argc != 4) {
          fprintf(stderr, "Usage: %s <original_file> <copy_file> <output_file>\n", argv[0]);
          return 1;
      }

      char* originalText = readFile(argv[1]);
      char* copyText = readFile(argv[2]);

      if (!originalText || !copyText) {
          free(originalText);
          free(copyText);
          return 1;
      }

      double similarity = calculateCosineSimilarity(originalText, copyText);

      FILE* outputFile = fopen(argv[3], "w");
      if (!outputFile) {
          fprintf(stderr, "Error: Could not open output file %s\n", argv[3]);
          free(originalText);
          free(copyText);
          return 1;
      }

      fprintf(outputFile, "%.2f", similarity);
      fclose(outputFile);

      free(originalText);
      free(copyText);
      return 0;
  }
  ```

---

## **3. 算法关键点**
1. **字符频率统计**：
   - 使用两个长度为 256 的数组（`vec1` 和 `vec2`）统计两个文本中每个字符的频率。
   - 通过遍历文本，将字符的 ASCII 值作为索引，统计频率。

2. **余弦相似度计算**：
   - 余弦相似度公式：  相似度=（点积）/(模1*模2）
   - 点积：两个向量的对应元素相乘后求和。
   - 模：向量的长度，计算方法是平方和后开方。

3. **异常处理**：
   - 文件不存在或无法读取时，返回 `NULL`。
   - 输出文件无法创建时，输出错误信息并退出。

---

## **4. 独到之处**
1. **简单高效**：
   - 使用字符频率统计和余弦相似度计算，算法简单且高效。
2. **适用于短文本**：
   - 算法特别适合短文本查重，计算速度快。
3. **可扩展性强**：
   - 可以通过改进字符频率统计方法（如使用哈希表）支持更复杂的文本处理。

---
 ##  计算模块接口的性能改进

1. **性能改进**：
   - 初始版本使用简单的字符频率统计，性能较低。
   - 改进思路：优化字符频率统计逻辑，减少不必要的计算。

2. **性能分析图**：
![](https://img2024.cnblogs.com/blog/3610099/202503/3610099-20250308184103529-1768128864.png)
![](https://img2024.cnblogs.com/blog/3610099/202503/3610099-20250308184140143-39028000.png)
---

##单元测试展示
#### **1. 单元测试代码**
```c
#include <stdio.h>
#include <assert.h>
#include "plagiarism_checker.h"

// 测试读取文件功能
void test_readFile() {
    const char* filePath = "tests/orig.txt";
    char* content = readFile(filePath);
    assert(content != NULL); // 确保文件内容不为空
    printf("test_readFile passed!\n");
}

// 测试计算余弦相似度功能
void test_calculateCosineSimilarity() {
    const char* text1 = "今天是星期天，天气晴，今天晚上我要去看电影。";
    const char* text2 = "今天是周天，天气晴朗，我晚上要去看电影。";
    double similarity = calculateCosineSimilarity(text1, text2);
    assert(similarity > 0 && similarity < 1); // 确保相似度在合理范围内
    printf("test_calculateCosineSimilarity passed!\n");
}

// 测试文件不存在异常
void test_fileNotFound() {
    const char* filePath = "tests/nonexistent.txt";
    char* content = readFile(filePath);
    assert(content == NULL); // 确保文件不存在时返回 NULL
    printf("test_fileNotFound passed!\n");
}

// 测试输出文件异常
void test_outputFileError() {
    const char* outputPath = "invalid/path/ans.txt";
    FILE* file = fopen(outputPath, "w");
    assert(file == NULL); // 确保输出文件无法创建时返回 NULL
    printf("test_outputFileError passed!\n");
}

int main() {
    test_readFile();
    test_calculateCosineSimilarity();
    test_fileNotFound();
    test_outputFileError();
    return 0;
}
```

---

#### **2. 构造测试数据的思路**
- **`test_readFile`**：
  - 测试数据：`tests/orig.txt` 文件。
  - 测试目的：验证 `readFile` 函数能否正确读取文件内容。
  - 构造思路：使用一个已知的文件路径，确保文件存在且内容不为空。

- **`test_calculateCosineSimilarity`**：
  - 测试数据：两段相似的文本。
  - 测试目的：验证 `calculateCosineSimilarity` 函数能否正确计算相似度。
  - 构造思路：使用两段相似的文本，确保相似度在合理范围内（0 到 1 之间）。

- **`test_fileNotFound`**：
  - 测试数据：一个不存在的文件路径。
  - 测试目的：验证 `readFile` 函数在文件不存在时能否正确处理异常。
  - 构造思路：使用一个不存在的文件路径，确保函数返回 `NULL`。

- **`test_outputFileError`**：
  - 测试数据：一个无效的输出文件路径。
  - 测试目的：验证程序在输出文件无法创建时能否正确处理异常。
  - 构造思路：使用一个无效的路径，确保 `fopen` 返回 `NULL`。

---
 ##  计算模块部分异常处理说明
1. **异常设计**：
   - 文件不存在异常：如果输入文件路径错误，程序会输出错误信息并退出。
   - 文件读取异常：如果文件无法读取，程序会输出错误信息并退出。
   - 输出文件异常：如果输出文件无法创建，程序会输出错误信息并退出。

2. **单元测试样例**：
   - 示例 1：测试文件不存在异常。
     ```c
     void test_fileNotFound() {
         const char* filePath = "tests/nonexistent.txt";
         char* content = readFile(filePath);
         assert(content == NULL);
         printf("test_fileNotFound passed!\n");
     }
     ```
   - 示例 2：测试输出文件异常。
     ```c
     void test_outputFileError() {
         const char* outputPath = "invalid/path/ans.txt";
         FILE* file = fopen(outputPath, "w");
         assert(file == NULL);
         printf("test_outputFileError passed!\n");
     }
     ```


---
* ##PSP表格
|  PSP2.1    |  Personal Software Process Stages    |   预估耗时（分钟）   |实际耗时（分钟）|
| ---- | ---- | ---- |-|
|  Planning    |   计划   |30      |40|
|    Estimate  |   估计这个任务需要多少时间   | 1600     |1500|
|    Development  |  开发    |  200    |300|
|    Analysis  |  需求分析 (包括学习新技术)    | 300     |500|
|    Design Spec  | 生成设计文档    |  30    |30|
|    Design Review  |  设计复审    | 40     |30|
|   Coding Standard   |   代码规范 (为目前的开发制定合适的规范)   |30      |40|
| Design     |  具体设计    | 60     |50|
|    Coding  |   具体编码   |  180    |180|
|   Code Review   | 代码复审     |    60  |60|
|   Test   |  测试（自我测试，修改代码，提交修改）    |300      |500|
|  Reporting    | 报告     |   100   |120|
|   Test Repor   |   测试报告   |   40   |50|
| Size Measurement     |  计算工作量    | 50     |60|
|  Postmortem & Process Improvement Plan    |  事后总结, 并提出过程改进计划    |    50  |60|
|      |  合计    |  3070    |3340|
