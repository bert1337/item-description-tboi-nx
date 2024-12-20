#pragma once
#include "nn/fs.hpp"

extern "C" {
    void* nnutilZlib_zcalloc(void* put_nullptr_here, size_t nmemb, size_t size);
    void nnutilZlib_zcfree(void* put_nullptr_here, void* ptr);
}

namespace nn::util {
    const size_t DecompressZlibWorkBufferSize = 7160;
    const size_t DecompressGzipWorkBufferSize = DecompressZlibWorkBufferSize;

    bool DecompressGzip(void* dst, size_t dstSize, const void* src, size_t srcSize, void* work, size_t workSize);
    size_t GetGzipDecompressedSize(const void* src, size_t srcSize);
    static ALWAYS_INLINE size_t GetGzipDecompressedSizeFromFile(nn::fs::FileHandle filehandle) {
			long in_size = 0;
			nn::fs::GetFileSize(&in_size, filehandle);
			uint32_t dec_size = 0;
			nn::fs::ReadFile(filehandle, in_size-4, &dec_size, 4);
            return (size_t)dec_size;
    }
    static ALWAYS_INLINE void* GzipAllocate(size_t size) {
        return nnutilZlib_zcalloc(nullptr, 1, size);
    }
    static ALWAYS_INLINE void GzipFree(void* ptr) {
        return nnutilZlib_zcfree(nullptr, ptr);
    }
}