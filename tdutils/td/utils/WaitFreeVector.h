//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2022
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "td/utils/common.h"

namespace td {

template <class T>
class WaitFreeVector {
  static constexpr size_t MAX_VECTOR_SIZE = (1 << 15) - 10;

  vector<vector<T>> storage_;

 public:
  template <class... ArgsT>
  void emplace_back(ArgsT &&...args) {
    if (storage_.empty() || storage_.back().size() == MAX_VECTOR_SIZE) {
      storage_.emplace_back();
    }
    storage_.back().emplace_back(std::forward<ArgsT>(args)...);
  }

  void pop_back() {
    storage_.back().pop_back();
    if (storage_.back().empty()) {
      storage_.pop_back();
    }
  }

  void push_back(T &&value) {
    emplace_back(std::move(value));
  }

  void push_back(const T &value) {
    emplace_back(value);
  }

  const T &back() const {
    return storage_.back().back();
  }

  T &operator[](size_t index) {
    return storage_[index / MAX_VECTOR_SIZE][index % MAX_VECTOR_SIZE];
  }

  const T &operator[](size_t index) const {
    return storage_[index / MAX_VECTOR_SIZE][index % MAX_VECTOR_SIZE];
  }

  size_t size() const {
    size_t result = 0;
    for (auto &storage : storage_) {
      result += storage.size();
    }
    return result;
  }

  bool empty() const {
    return storage_.empty() || storage_[0].empty();
  }
};

}  // namespace td