extern "C" {
#include <libavutil/error.h>
}
#include <Windows.h>
#include <string>
#include <unordered_map>

#include "vm_utils.h"

namespace vm_utils {

std::string ff_err_to_str(int errorCode) {
  // FFmpeg 标准错误码映射表
  static const std::unordered_map<int, std::string> errorMap = {
      {0, "Success"},
      {AVERROR(EIO), "I/O error"},
      {AVERROR(ENOMEM), "Out of memory"},
      {AVERROR(EINVAL), "Invalid argument"},
      {AVERROR(ENOSYS), "Function not implemented"},
      {AVERROR(ENOENT), "No such file or directory"},
      {AVERROR(EPIPE), "Broken pipe"},
      {AVERROR(EAGAIN), "Resource temporarily unavailable"},
      {AVERROR(ENOSPC), "No space left on device"},
      {AVERROR(EEXIST), "File exists"},
      {AVERROR(ETIMEDOUT), "Connection timed out"},
      {AVERROR(EDOM), "Domain error"},
      {AVERROR(ERANGE), "Range error"},
      {AVERROR(EPROTO), "Protocol error"},
      {AVERROR(EILSEQ), "Illegal byte sequence"},
      {AVERROR_BSF_NOT_FOUND, "Bitstream filter not found"},
      {AVERROR_BUG, "Internal bug"},
      {AVERROR_BUFFER_TOO_SMALL, "Buffer too small"},
      {AVERROR_DECODER_NOT_FOUND, "Decoder not found"},
      {AVERROR_DEMUXER_NOT_FOUND, "Demuxer not found"},
      {AVERROR_ENCODER_NOT_FOUND, "Encoder not found"},
      {AVERROR_EOF, "End of file"},
      {AVERROR_EXIT, "Immediate exit requested"},
      {AVERROR_EXTERNAL, "External error"},
      {AVERROR_FILTER_NOT_FOUND, "Filter not found"},
      {AVERROR_INVALIDDATA, "Invalid data found"},
      {AVERROR_MUXER_NOT_FOUND, "Muxer not found"},
      {AVERROR_OPTION_NOT_FOUND, "Option not found"},
      {AVERROR_PATCHWELCOME, "Feature is under construction"},
      {AVERROR_PROTOCOL_NOT_FOUND, "Protocol not found"},
      {AVERROR_STREAM_NOT_FOUND, "Stream not found"},
      {AVERROR_UNKNOWN, "Unknown error"},
  };

  // 检查是否是已知错误码
  auto it = errorMap.find(errorCode);
  if (it != errorMap.end()) {
    return it->second;
  }

  // 对于未知错误码，使用 av_strerror 获取描述
  char errbuf[AV_ERROR_MAX_STRING_SIZE] = {0};
  if (av_strerror(errorCode, errbuf, sizeof(errbuf)) == 0) {
    return std::string(errbuf);
  }

  // 如果所有方法都失败，返回通用错误信息
  return "Unknown FFmpeg error (" + std::to_string(errorCode) + ")";
}

std::string ansi_to_utf8(const char *ansi_str) {
  // 1. ANSI → UTF-16（不自动添加 \0）
  int wlen =
      MultiByteToWideChar(CP_ACP, 0, ansi_str, strlen(ansi_str), nullptr, 0);
  std::wstring wstr(wlen, 0);
  MultiByteToWideChar(CP_ACP, 0, ansi_str, strlen(ansi_str), &wstr[0], wlen);

  // 2. UTF-16 → UTF-8（不自动添加 \0）
  int ulen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), nullptr,
                                 0, nullptr, nullptr);
  std::string utf8_str(ulen, 0);
  WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), &utf8_str[0], ulen,
                      nullptr, nullptr);

  return utf8_str;
}

} // namespace vm_utils