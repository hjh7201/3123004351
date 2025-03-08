// Checker.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <map>
#include <iomanip> // 用于设置输出精度

// 读取文件内容
std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        exit(1);
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}

// 计算余弦相似度
double calculateCosineSimilarity(const std::string& text1, const std::string& text2) {
    std::map<char, int> vec1, vec2;

    // 计算字符频率
    for (char ch : text1) vec1[ch]++;
    for (char ch : text2) vec2[ch]++;

    // 计算点积和模
    double dotProduct = 0, magnitude1 = 0, magnitude2 = 0;
    for (const auto& pair : vec1) {
        dotProduct += pair.second * vec2[pair.first];
        magnitude1 += pair.second * pair.second;
    }
    for (const auto& pair : vec2) {
        magnitude2 += pair.second * pair.second;
    }

    magnitude1 = std::sqrt(magnitude1);
    magnitude2 = std::sqrt(magnitude2);

    if (magnitude1 == 0 || magnitude2 == 0) return 0.0;
    return dotProduct / (magnitude1 * magnitude2);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <original_file> <copy_file> <output_file>" << std::endl;
        return 1;
    }

    std::string originalText = readFile(argv[1]);
    std::string copyText = readFile(argv[2]);
    double similarity = calculateCosineSimilarity(originalText, copyText);

    std::ofstream outputFile(argv[3]);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open output file " << argv[3] << std::endl;
        return 1;
    }

    outputFile << std::fixed << std::setprecision(2) << similarity;
    outputFile.close();

    std::cout << "Similarity: " << similarity << std::endl;
    return 0;
}