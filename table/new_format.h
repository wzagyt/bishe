// table/new_format.h
#ifndef STORAGE_LEVELDB_TABLE_NEW_FORMAT_H_
#define STORAGE_LEVELDB_TABLE_NEW_FORMAT_H_

#include <stdint.h>
#include "leveldb/slice.h"
#include "leveldb/status.h"

namespace leveldb {

class Block;
class RandomAccessFile;
struct ReadOptions;

// 一个New SSTable最多包含的SST数量
static const int kMaxSstPerNewTable = 12;

// New SSTable 头部结构 (固定大小，位于文件开始)
struct NewTableHeader {
  uint32_t num_sst;                  // 当前包含的SST数量
  uint64_t sst_offsets[kMaxSstPerNewTable]; // 每个SST的起始偏移量
  uint64_t sst_sizes[kMaxSstPerNewTable];   // 每个SST的大小
  
  // 序列化/反序列化方法
  void EncodeTo(std::string* dst) const;
  Status DecodeFrom(Slice* input);

  // 返回序列化后的字节数
  static size_t EncodedLength() {
    return sizeof(uint32_t) + kMaxSstPerNewTable * (sizeof(uint64_t) * 2);
  }
};

// 每个SST段的尾部结构
struct SstFooter {
  uint64_t metaindex_handle_offset;  // 元数据索引的偏移量
  uint64_t metaindex_handle_size;    // 元数据索引的大小
  uint64_t index_handle_offset;      // 数据索引的偏移量
  uint64_t index_handle_size;        // 数据索引的大小
  
  // 序列化/反序列化方法
  void EncodeTo(std::string* dst) const;
  Status DecodeFrom(Slice* input);

  // 返回序列化后的字节数
  static size_t EncodedLength() {
    return sizeof(uint64_t) * 4;
  }
};

// 从文件中读取New Table Header
Status ReadNewTableHeader(RandomAccessFile* file, uint64_t file_size, 
                          NewTableHeader* header);

// 从文件中读取SST Footer
Status ReadSstFooter(RandomAccessFile* file, uint64_t sst_offset, 
                     uint64_t sst_size, SstFooter* footer);

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_TABLE_NEW_FORMAT_H_
